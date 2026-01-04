// SPDX-License-Identifier: GPL-2.0-only OR MIT
#![allow(clippy::unusual_byte_groupings)]

//! GPU initialization data builder.
//!
//! The root of all interaction between the GPU firmware and the host driver is a complex set of
//! nested structures that we call InitData. This includes both GPU hardware/firmware configuration
//! and the pointers to the ring buffers and global data fields that are used for communication at
//! runtime.
//!
//! Many of these structures are poorly understood, so there are lots of hardcoded unknown values
//! derived from observing the InitData structures that macOS generates.

use crate::fw::initdata::*;
use crate::fw::types::*;
use crate::module_parameters;
use crate::{
    driver::AsahiDevice,
    gem,
    gpu,
    hw,
    mmu, //
};
use kernel::macros::versions;
use kernel::prelude::*;
use kernel::try_init;

use ::pin_init;
use ::pin_init::Init;

/// Builder helper for the global GPU InitData.
#[versions(AGX)]
pub(crate) struct InitDataBuilder<'a> {
    dev: &'a AsahiDevice,
    alloc: &'a mut gpu::KernelAllocators,
    cfg: &'static hw::HwConfig,
    dyncfg: &'a hw::DynConfig,
}

#[versions(AGX)]
impl<'a> InitDataBuilder::ver<'a> {
    /// Create a new InitData builder
    pub(crate) fn new(
        dev: &'a AsahiDevice,
        alloc: &'a mut gpu::KernelAllocators,
        cfg: &'static hw::HwConfig,
        dyncfg: &'a hw::DynConfig,
    ) -> InitDataBuilder::ver<'a> {
        InitDataBuilder::ver {
            dev,
            alloc,
            cfg,
            dyncfg,
        }
    }

    /// Create the HwDataA structure. This mostly contains power-related configuration.
    fn hwdata_a(&mut self) -> Result<GpuArray<u8>> {
        let mut data = self
            .alloc
            .private
            .array_empty(self.dyncfg.hw_data_a.len())?;
        data.as_mut_slice().copy_from_slice(&self.dyncfg.hw_data_a);
        Ok(data)
    }

    /// Create the HwDataB structure. This mostly contains GPU-related configuration.
    fn hwdata_b(&mut self) -> Result<GpuObject<HwDataB::ver>> {
        self.alloc.private.new_init(pin_init::zeroed::<HwDataB::ver>(), |_inner, _ptr| {
            init!(raw::HwDataB::ver {
                ..Zeroable::init_zeroed()
            })
            .chain(|raw| {
                // SAFETY: Bootloader is supposed to put a correctly formed HwDataB there
                unsafe {
                    let ptr = raw as *mut raw::HwDataB::ver as *mut u8;
                    let size = core::mem::size_of::<raw::HwDataB::ver>();
                    core::slice::from_raw_parts_mut(ptr, size)
                        .copy_from_slice(&self.dyncfg.hw_data_b[..size]);
                }
                Ok(())
            })
        })
    }

    /// Create the Globals structure, which contains global firmware config including more power
    /// configuration data and globals used to exchange state between the firmware and driver.
    fn globals(&mut self) -> Result<GpuObject<Globals::ver>> {
        self.alloc.private.new_init(pin_init::zeroed::<Globals::ver>(), |_inner, _ptr| {
            init!(raw::Globals::ver {
                ..Zeroable::init_zeroed()
            })
            .chain(|raw| {
                // SAFETY: Bootloader is supposed to put a correctly formed Globals there
                unsafe {
                    let ptr = raw as *mut raw::Globals::ver as *mut u8;
                    let size = core::mem::size_of::<raw::Globals::ver>();
                    core::slice::from_raw_parts_mut(ptr, size)
                        .copy_from_slice(&self.dyncfg.hw_globals[..size]);
                }
                raw.fault_control = *module_parameters::fault_control.value();
                // Paranoia
                raw.pending_submissions = AtomicU32::new(0);
                Ok(())
            })
        })
    }

    /// Create the RuntimePointers structure, which contains pointers to most of the other
    /// structures including the ring buffer channels, statistics structures, and HwDataA/HwDataB.
    fn runtime_pointers(&mut self) -> Result<GpuObject<RuntimePointers::ver>> {
        let hwa = self.hwdata_a()?;
        let hwb = self.hwdata_b()?;

        let mut buffer_mgr_ctl = gem::new_kernel_object(self.dev, 0x4000)?;
        buffer_mgr_ctl.vmap()?.memset(0);

        GpuObject::new_init_prealloc(
            self.alloc.private.alloc_object()?,
            |_ptr| {
                let alloc = &mut *self.alloc;
                try_init!(RuntimePointers::ver {
                    stats <- {
                        let alloc = &mut *alloc;
                        try_init!(Stats::ver {
                            vtx: alloc.private.new_default::<GpuGlobalStatsVtx>()?,
                            frag: alloc.private.new_init(
                                pin_init::init_zeroed::<GpuGlobalStatsFrag::ver>(),
                                |_inner, _ptr| {
                                    try_init!(raw::GpuGlobalStatsFrag::ver {
                                        total_cmds: 0,
                                        unk_4: 0,
                                        stats: Default::default(),
                                    })
                                }
                            )?,
                            comp: alloc.private.new_default::<GpuStatsComp>()?,
                        })
                    },

                    hwdata_a: hwa,
                    unkptr_190: alloc.private.array_empty_tagged(0x80, b"I190")?,
                    unkptr_198: alloc.private.array_empty_tagged(0xc0, b"I198")?,
                    hwdata_b: hwb,

                    unkptr_1b8: alloc.private.array_empty_tagged(0x1000, b"I1B8")?,
                    unkptr_1c0: alloc.private.array_empty_tagged(0x300, b"I1C0")?,
                    unkptr_1c8: alloc.private.array_empty_tagged(0x1000, b"I1C8")?,

                    buffer_mgr_ctl,
                    buffer_mgr_ctl_low_mapping: None,
                    buffer_mgr_ctl_high_mapping: None,
                })
            },
            |inner, _ptr| {
                try_init!(raw::RuntimePointers::ver {
                    pipes: Default::default(),
                    device_control: Default::default(),
                    event: Default::default(),
                    fw_log: Default::default(),
                    ktrace: Default::default(),
                    stats: Default::default(),

                    stats_vtx: inner.stats.vtx.gpu_pointer(),
                    stats_frag: inner.stats.frag.gpu_pointer(),
                    stats_comp: inner.stats.comp.gpu_pointer(),

                    hwdata_a: inner.hwdata_a.gpu_pointer(),
                    unkptr_190: inner.unkptr_190.gpu_pointer(),
                    unkptr_198: inner.unkptr_198.gpu_pointer(),
                    hwdata_b: inner.hwdata_b.gpu_pointer(),
                    hwdata_b_2: inner.hwdata_b.gpu_pointer(),

                    fwlog_buf: None,

                    unkptr_1b8: inner.unkptr_1b8.gpu_pointer(),

                    #[ver(G < G14X)]
                    unkptr_1c0: inner.unkptr_1c0.gpu_pointer(),
                    #[ver(G < G14X)]
                    unkptr_1c8: inner.unkptr_1c8.gpu_pointer(),

                    buffer_mgr_ctl_gpu_addr: U64(gpu::IOVA_KERN_GPU_BUFMGR_LOW),
                    buffer_mgr_ctl_fw_addr: U64(gpu::IOVA_KERN_GPU_BUFMGR_HIGH),

                    __pad0: Default::default(),
                    unk_160: U64(0),
                    unk_168: U64(0),
                    unk_1d0: 0,
                    unk_1d4: 0,
                    unk_1d8: Default::default(),

                    __pad1: Default::default(),
                    gpu_scratch: raw::RuntimeScratch::ver {
                        unk_6b38: 0xff,
                        ..Default::default()
                    },
                })
            },
        )
    }

    /// Create the FwStatus structure, which is used to coordinate the firmware halt state between
    /// the firmware and the driver.
    fn fw_status(&mut self) -> Result<GpuObject<FwStatus>> {
        self.alloc
            .shared
            .new_object(Default::default(), |_inner| Default::default())
    }

    /// Create one UatLevelInfo structure, which describes one level of translation for the UAT MMU.
    fn uat_level_info(
        cfg: &'static hw::HwConfig,
        index_shift: usize,
        num_entries: usize,
    ) -> raw::UatLevelInfo {
        raw::UatLevelInfo {
            index_shift: index_shift as _,
            unk_1: 14,
            unk_2: 14,
            unk_3: 8,
            unk_4: 0x4000,
            num_entries: num_entries as _,
            unk_8: U64(1),
            unk_10: U64(((1u64 << cfg.uat_oas) - 1) & !(mmu::UAT_PGMSK as u64)),
            index_mask: U64(((num_entries - 1) << index_shift) as u64),
        }
    }

    /// Build the top-level InitData object.
    #[inline(never)]
    pub(crate) fn build(&mut self) -> Result<KBox<GpuObject<InitData::ver>>> {
        let runtime_pointers = self.runtime_pointers()?;
        let globals = self.globals()?;
        let fw_status = self.fw_status()?;
        let shared_ro = &mut self.alloc.shared_ro;

        let obj = self.alloc.private.new_init(
            try_init!(InitData::ver {
                unk_buf: shared_ro.array_empty_tagged(0x4000, b"IDTA")?,
                runtime_pointers,
                globals,
                fw_status,
            }),
            |inner, _ptr| {
                let cfg = &self.cfg;
                try_init!(raw::InitData::ver {
                    #[ver(V == V13_5 && G != G14X)]
                    ver_info: Array::new([0x6ba0, 0x1f28, 0x601, 0xb0]),
                    #[ver(V == V13_5 && G == G14X)]
                    ver_info: Array::new([0xb390, 0x70f8, 0x601, 0xb0]),
                    unk_buf: inner.unk_buf.gpu_pointer(),
                    unk_8: 0,
                    unk_c: 0,
                    runtime_pointers: inner.runtime_pointers.gpu_pointer(),
                    globals: inner.globals.gpu_pointer(),
                    fw_status: inner.fw_status.gpu_pointer(),
                    uat_page_size: 0x4000,
                    uat_page_bits: 14,
                    uat_num_levels: 3,
                    uat_level_info: Array::new([
                        Self::uat_level_info(cfg, 36, 8),
                        Self::uat_level_info(cfg, 25, 2048),
                        Self::uat_level_info(cfg, 14, 2048),
                    ]),
                    __pad0: Default::default(),
                    host_mapped_fw_allocations: 1,
                    unk_ac: 0,
                    unk_b0: 0,
                    unk_b4: 0,
                    unk_b8: 0,
                })
            },
        )?;
        Ok(KBox::new(obj, GFP_KERNEL)?)
    }
}
