// SPDX-License-Identifier: GPL-2.0-only OR MIT

//! GPU initialization / global structures

use super::channels;
use super::types::*;
use crate::{
    default_zeroed,
    gem,
    mmu,
    no_debug,
    trivial_gpustruct, //
};

pub(crate) mod raw {
    use super::*;

    #[derive(Debug, Default)]
    #[repr(C)]
    pub(crate) struct ChannelRing<T: GpuStruct + Debug + Default, U: Copy> {
        pub(crate) state: Option<GpuWeakPointer<T>>,
        pub(crate) ring: Option<GpuWeakPointer<[U]>>,
    }

    #[versions(AGX)]
    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct PipeChannels {
        pub(crate) vtx: ChannelRing<channels::ChannelState, channels::PipeMsg::ver>,
        pub(crate) frag: ChannelRing<channels::ChannelState, channels::PipeMsg::ver>,
        pub(crate) comp: ChannelRing<channels::ChannelState, channels::PipeMsg::ver>,
    }
    #[versions(AGX)]
    default_zeroed!(PipeChannels::ver);

    #[derive(Debug, Default)]
    #[repr(C)]
    pub(crate) struct FwStatusFlags {
        pub(crate) halt_count: AtomicU64,
        __pad0: Pad<0x8>,
        pub(crate) halted: AtomicU32,
        __pad1: Pad<0xc>,
        pub(crate) resume: AtomicU32,
        __pad2: Pad<0xc>,
        pub(crate) unk_40: u32,
        __pad3: Pad<0xc>,
        pub(crate) unk_ctr: u32,
        __pad4: Pad<0xc>,
        pub(crate) unk_60: u32,
        __pad5: Pad<0xc>,
        pub(crate) unk_70: u32,
        __pad6: Pad<0xc>,
    }

    #[derive(Debug, Default)]
    #[repr(C)]
    pub(crate) struct FwStatus {
        pub(crate) fwctl_channel: ChannelRing<channels::FwCtlChannelState, channels::FwCtlMsg>,
        pub(crate) flags: FwStatusFlags,
    }

    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct HwDataShared1 {
        pub(crate) table: Array<16, i32>,
        pub(crate) unk_44: Array<0x60, u8>,
        pub(crate) unk_a4: u32,
        pub(crate) unk_a8: u32,
    }
    default_zeroed!(HwDataShared1);

    #[derive(Debug, Default)]
    #[repr(C)]
    pub(crate) struct HwDataShared2Curve {
        pub(crate) unk_0: u32,
        pub(crate) unk_4: u32,
        pub(crate) t1: Array<16, u16>,
        pub(crate) t2: Array<16, i16>,
        pub(crate) t3: Array<8, Array<16, i32>>,
    }

    #[derive(Debug, Default)]
    #[repr(C)]
    pub(crate) struct HwDataShared2G14 {
        pub(crate) unk_0: Array<5, u32>,
        pub(crate) unk_14: u32,
        pub(crate) unk_18: Array<8, u32>,
        pub(crate) curve1: HwDataShared2Curve,
        pub(crate) curve2: HwDataShared2Curve,
    }

    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct HwDataShared2 {
        pub(crate) table: Array<10, i32>,
        pub(crate) unk_28: Array<0x10, u8>,
        pub(crate) g14: HwDataShared2G14,
        pub(crate) unk_500: u32,
        pub(crate) unk_504: u32,
        pub(crate) unk_508: u32,
        pub(crate) unk_50c: u32,
    }
    default_zeroed!(HwDataShared2);

    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct HwDataShared3 {
        pub(crate) unk_0: u32,
        pub(crate) unk_4: u32,
        pub(crate) unk_8: u32,
        pub(crate) table: Array<16, u32>,
        pub(crate) unk_4c: u32,
    }
    default_zeroed!(HwDataShared3);

    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct HwDataA130Extra {
        pub(crate) unk_0: Array<0x38, u8>,
        pub(crate) unk_38: u32,
        pub(crate) unk_3c: u32,
        pub(crate) gpu_se_inactive_threshold: u32,
        pub(crate) unk_44: u32,
        pub(crate) gpu_se_engagement_criteria: i32,
        pub(crate) gpu_se_reset_criteria: u32,
        pub(crate) unk_50: u32,
        pub(crate) unk_54: u32,
        pub(crate) unk_58: u32,
        pub(crate) unk_5c: u32,
        pub(crate) gpu_se_filter_a_neg: u32,
        pub(crate) gpu_se_filter_1_a_neg: u32,
        pub(crate) gpu_se_filter_a: u32,
        pub(crate) gpu_se_filter_1_a: u32,
        pub(crate) gpu_se_ki_dt: u32,
        pub(crate) gpu_se_ki_1_dt: u32,
        pub(crate) unk_78: u32,
        pub(crate) unk_7c: u32,
        pub(crate) gpu_se_kp: u32,
        pub(crate) gpu_se_kp_1: u32,
        pub(crate) unk_88: u32,
        pub(crate) unk_8c: u32,
        pub(crate) max_pstate_scaled_1: u32,
        pub(crate) unk_94: u32,
        pub(crate) unk_98: u32,
        pub(crate) unk_9c: u32,
        pub(crate) unk_a0: u32,
        pub(crate) unk_a4: u32,
        pub(crate) gpu_se_filter_time_constant_ms: u32,
        pub(crate) gpu_se_filter_time_constant_1_ms: u32,
        pub(crate) gpu_se_filter_time_constant_clks: U64,
        pub(crate) gpu_se_filter_time_constant_1_clks: U64,
        pub(crate) unk_c0: u32,
        pub(crate) unk_c4: u32,
        pub(crate) unk_c8: Array<0x4c, u8>,
        pub(crate) unk_114: u32,
        pub(crate) unk_118: u32,
        pub(crate) unk_11c: u32,
        pub(crate) unk_120: u32,
        pub(crate) unk_124: u32,
        pub(crate) max_pstate_scaled_2: u32,
        pub(crate) unk_12c: Array<0x8c, u8>,
    }
    default_zeroed!(HwDataA130Extra);

    #[repr(C)]
    pub(crate) struct T81xxData {
        pub(crate) unk_d8c: u32,
        pub(crate) unk_d90: u32,
        pub(crate) unk_d94: u32,
        pub(crate) unk_d98: u32,
        pub(crate) unk_d9c: u32,
        pub(crate) unk_da0: u32,
        pub(crate) unk_da4: u32,
        pub(crate) unk_da8: u32,
        pub(crate) unk_dac: u32,
        pub(crate) unk_db0: u32,
        pub(crate) unk_db4: u32,
        pub(crate) unk_db8: u32,
        pub(crate) unk_dbc: u32,
        pub(crate) unk_dc0: u32,
        pub(crate) unk_dc4: u32,
        pub(crate) unk_dc8: u32,
        pub(crate) max_pstate_scaled: u32,
    }
    default_zeroed!(T81xxData);

    #[derive(Debug, Default, Clone, Copy)]
    #[repr(C)]
    pub(crate) struct IOMapping {
        pub(crate) phys_addr: U64,
        pub(crate) virt_addr: U64,
        pub(crate) total_size: u32,
        pub(crate) element_size: u32,
        pub(crate) readwrite: U64,
    }

    #[versions(AGX)]
    const IO_MAPPING_COUNT: usize = {
        #[ver(V < V13_0B4)]
        {
            0x14
        }
        #[ver(V >= V13_0B4 && V < V13_3)]
        {
            0x17
        }
        #[ver(V >= V13_3 && V < V13_5)]
        {
            0x18
        }
        #[ver(V >= V13_5)]
        {
            0x19
        }
    };

    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct HwDataBAuxPStates {
        pub(crate) cs_max_pstate: u32,
        pub(crate) cs_frequencies: Array<0x10, u32>,
        pub(crate) cs_voltages: Array<0x10, Array<0x2, u32>>,
        pub(crate) cs_voltages_sram: Array<0x10, Array<0x2, u32>>,
        pub(crate) cs_unkpad: u32,
        pub(crate) afr_max_pstate: u32,
        pub(crate) afr_frequencies: Array<0x8, u32>,
        pub(crate) afr_voltages: Array<0x8, Array<0x2, u32>>,
        pub(crate) afr_voltages_sram: Array<0x8, Array<0x2, u32>>,
        pub(crate) afr_unkpad: u32,
    }

    #[versions(AGX)]
    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct HwDataB {
        #[ver(V < V13_0B4)]
        pub(crate) unk_0: U64,

        pub(crate) unk_8: U64,

        #[ver(V < V13_0B4)]
        pub(crate) unk_10: U64,

        pub(crate) unk_18: U64,
        pub(crate) unk_20: U64,
        pub(crate) unk_28: U64,
        pub(crate) unk_30: U64,
        pub(crate) timestamp_area_base: U64,
        pub(crate) pad_40: Pad<0x20>,

        #[ver(V < V13_0B4)]
        pub(crate) yuv_matrices: Array<0xf, Array<3, Array<4, i16>>>,

        #[ver(V >= V13_0B4)]
        pub(crate) yuv_matrices: Array<0x3f, Array<3, Array<4, i16>>>,

        pub(crate) pad_1c8: Pad<0x8>,
        pub(crate) io_mappings: Array<IO_MAPPING_COUNT::ver, IOMapping>,

        #[ver(V >= V13_0B4)]
        pub(crate) sgx_sram_ptr: U64,

        pub(crate) chip_id: u32,
        pub(crate) unk_454: u32,
        pub(crate) unk_458: u32,
        pub(crate) unk_45c: u32,
        pub(crate) unk_460: u32,
        pub(crate) unk_464: u32,
        pub(crate) unk_468: u32,
        pub(crate) unk_46c: u32,
        pub(crate) unk_470: u32,
        pub(crate) unk_474: u32,
        pub(crate) unk_478: u32,
        pub(crate) unk_47c: u32,
        pub(crate) unk_480: u32,
        pub(crate) unk_484: u32,
        pub(crate) unk_488: u32,
        pub(crate) unk_48c: u32,
        pub(crate) base_clock_khz: u32,
        pub(crate) power_sample_period: u32,
        pub(crate) pad_498: Pad<0x4>,
        pub(crate) unk_49c: u32,
        pub(crate) unk_4a0: u32,
        pub(crate) unk_4a4: u32,
        pub(crate) pad_4a8: Pad<0x4>,
        pub(crate) unk_4ac: u32,
        pub(crate) pad_4b0: Pad<0x8>,
        pub(crate) unk_4b8: u32,
        pub(crate) unk_4bc: Array<0x4, u8>,
        pub(crate) unk_4c0: u32,
        pub(crate) unk_4c4: u32,
        pub(crate) unk_4c8: u32,
        pub(crate) unk_4cc: u32,
        pub(crate) unk_4d0: u32,
        pub(crate) unk_4d4: u32,
        pub(crate) unk_4d8: Array<0x4, u8>,
        pub(crate) unk_4dc: u32,
        pub(crate) unk_4e0: U64,
        pub(crate) unk_4e8: u32,
        pub(crate) unk_4ec: u32,
        pub(crate) unk_4f0: u32,
        pub(crate) unk_4f4: u32,
        pub(crate) unk_4f8: u32,
        pub(crate) unk_4fc: u32,
        pub(crate) unk_500: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_504_0: u32,

        pub(crate) unk_504: u32,
        pub(crate) unk_508: u32,
        pub(crate) unk_50c: u32,
        pub(crate) unk_510: u32,
        pub(crate) unk_514: u32,
        pub(crate) unk_518: u32,
        pub(crate) unk_51c: u32,
        pub(crate) unk_520: u32,
        pub(crate) unk_524: u32,
        pub(crate) unk_528: u32,
        pub(crate) unk_52c: u32,
        pub(crate) unk_530: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_534_0: u32,

        pub(crate) unk_534: u32,
        pub(crate) unk_538: u32,

        pub(crate) num_frags: u32,
        pub(crate) unk_540: u32,
        pub(crate) unk_544: u32,
        pub(crate) unk_548: u32,
        pub(crate) unk_54c: u32,
        pub(crate) unk_550: u32,
        pub(crate) unk_554: u32,
        pub(crate) uat_ttb_base: U64,
        pub(crate) gpu_core_id: u32,
        pub(crate) gpu_rev_id: u32,
        pub(crate) num_cores: u32,
        pub(crate) max_pstate: u32,

        #[ver(V < V13_0B4)]
        pub(crate) num_pstates: u32,

        pub(crate) frequencies: Array<0x10, u32>,
        pub(crate) voltages: Array<0x10, [u32; 0x8]>,
        pub(crate) voltages_sram: Array<0x10, [u32; 0x8]>,

        #[ver(V >= V13_3)]
        pub(crate) unk_9f4_0: Pad<64>,

        pub(crate) sram_k: Array<0x10, u32>,
        pub(crate) unk_9f4: Array<0x10, u32>,
        pub(crate) rel_max_powers: Array<0x10, u32>,
        pub(crate) rel_boost_freqs: Array<0x10, u32>,

        #[ver(V >= V13_3)]
        pub(crate) unk_arr_0: Array<32, u32>,

        #[ver(V < V13_0B4)]
        pub(crate) min_sram_volt: u32,

        #[ver(V < V13_0B4)]
        pub(crate) unk_ab8: u32,

        #[ver(V < V13_0B4)]
        pub(crate) unk_abc: u32,

        #[ver(V < V13_0B4)]
        pub(crate) unk_ac0: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) aux_ps: HwDataBAuxPStates,

        #[ver(V >= V13_3)]
        pub(crate) pad_ac4_0: Array<0x44c, u8>,

        pub(crate) pad_ac4: Pad<0x8>,
        pub(crate) unk_acc: u32,
        pub(crate) unk_ad0: u32,
        pub(crate) pad_ad4: Pad<0x10>,
        pub(crate) unk_ae4: Array<0x4, u32>,
        pub(crate) pad_af4: Pad<0x4>,
        pub(crate) unk_af8: u32,
        pub(crate) pad_afc: Pad<0x8>,
        pub(crate) unk_b04: u32,
        pub(crate) unk_b08: u32,
        pub(crate) unk_b0c: u32,

        #[ver(G >= G14X)]
        pub(crate) pad_b10_0: Array<0x8, u8>,

        pub(crate) unk_b10: u32,
        pub(crate) timer_offset: U64,
        pub(crate) unk_b1c: u32,
        pub(crate) unk_b20: u32,
        pub(crate) unk_b24: u32,
        pub(crate) unk_b28: u32,
        pub(crate) unk_b2c: u32,
        pub(crate) unk_b30: u32,
        pub(crate) unk_b34: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_b38_0: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_b38_4: u32,

        #[ver(V >= V13_3)]
        pub(crate) unk_b38_8: u32,

        pub(crate) unk_b38: Array<0xc, u32>,
        pub(crate) unk_b68: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_b6c: Array<0xd0, u8>,

        #[ver(G >= G14X)]
        pub(crate) unk_c3c_0: Array<0x8, u8>,

        #[ver(G < G14X && V >= V13_5)]
        pub(crate) unk_c3c_8: Array<0x10, u8>,

        #[ver(V >= V13_5)]
        pub(crate) unk_c3c_18: Array<0x20, u8>,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_c3c: u32,
    }
    #[versions(AGX)]
    default_zeroed!(HwDataB::ver);

    #[derive(Debug)]
    #[repr(C, packed)]
    pub(crate) struct GpuStatsVtx {
        // This changes all the time and we don't use it, let's just make it a big buffer
        pub(crate) opaque: Array<0x3000, u8>,
    }
    default_zeroed!(GpuStatsVtx);

    #[versions(AGX)]
    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct GpuStatsFrag {
        // This changes all the time and we don't use it, let's just make it a big buffer
        // except for these two fields which may need init.
        #[ver(G >= G14X)]
        pub(crate) unk1_0: Array<0x910, u8>,
        pub(crate) unk1: Array<0x100, u8>,
        pub(crate) cur_stamp_id: i32,
        pub(crate) unk2: Array<0x14, u8>,
        pub(crate) unk_id: i32,
        pub(crate) unk3: Array<0x1000, u8>,
    }

    #[versions(AGX)]
    impl Default for GpuStatsFrag::ver {
        fn default() -> Self {
            Self {
                #[ver(G >= G14X)]
                unk1_0: Default::default(),
                unk1: Default::default(),
                cur_stamp_id: -1,
                unk2: Default::default(),
                unk_id: -1,
                unk3: Default::default(),
            }
        }
    }

    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct GpuGlobalStatsVtx {
        pub(crate) total_cmds: u32,
        pub(crate) stats: GpuStatsVtx,
    }
    default_zeroed!(GpuGlobalStatsVtx);

    #[versions(AGX)]
    #[derive(Debug, Default)]
    #[repr(C)]
    pub(crate) struct GpuGlobalStatsFrag {
        pub(crate) total_cmds: u32,
        pub(crate) unk_4: u32,
        pub(crate) stats: GpuStatsFrag::ver,
    }

    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct GpuStatsComp {
        // This changes all the time and we don't use it, let's just make it a big buffer
        pub(crate) opaque: Array<0x3000, u8>,
    }
    default_zeroed!(GpuStatsComp);

    #[versions(AGX)]
    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct RuntimeScratch {
        pub(crate) unk_280: Array<0x6800, u8>,
        pub(crate) unk_6a80: u32,
        pub(crate) gpu_idle: u32,
        pub(crate) unkpad_6a88: Pad<0x14>,
        pub(crate) unk_6a9c: u32,
        pub(crate) unk_ctr0: u32,
        pub(crate) unk_ctr1: u32,
        pub(crate) unk_6aa8: u32,
        pub(crate) unk_6aac: u32,
        pub(crate) unk_ctr2: u32,
        pub(crate) unk_6ab4: u32,
        pub(crate) unk_6ab8: u32,
        pub(crate) unk_6abc: u32,
        pub(crate) unk_6ac0: u32,
        pub(crate) unk_6ac4: u32,
        pub(crate) unk_ctr3: u32,
        pub(crate) unk_6acc: u32,
        pub(crate) unk_6ad0: u32,
        pub(crate) unk_6ad4: u32,
        pub(crate) unk_6ad8: u32,
        pub(crate) unk_6adc: u32,
        pub(crate) unk_6ae0: u32,
        pub(crate) unk_6ae4: u32,
        pub(crate) unk_6ae8: u32,
        pub(crate) unk_6aec: u32,
        pub(crate) unk_6af0: u32,
        pub(crate) unk_ctr4: u32,
        pub(crate) unk_ctr5: u32,
        pub(crate) unk_6afc: u32,
        pub(crate) pad_6b00: Pad<0x38>,

        #[ver(G >= G14X)]
        pub(crate) pad_6b00_extra: Array<0x4800, u8>,

        pub(crate) unk_6b38: u32,
        pub(crate) pad_6b3c: Pad<0x84>,
    }
    #[versions(AGX)]
    default_zeroed!(RuntimeScratch::ver);

    #[versions(AGX)]
    #[repr(C)]
    pub(crate) struct RuntimePointers<'a> {
        pub(crate) pipes: Array<4, PipeChannels::ver>,

        pub(crate) device_control:
            ChannelRing<channels::ChannelState, channels::DeviceControlMsg::ver>,
        pub(crate) event: ChannelRing<channels::ChannelState, channels::RawEventMsg>,
        pub(crate) fw_log: ChannelRing<channels::FwLogChannelState, channels::RawFwLogMsg>,
        pub(crate) ktrace: ChannelRing<channels::ChannelState, channels::RawKTraceMsg>,
        pub(crate) stats: ChannelRing<channels::ChannelState, channels::RawStatsMsg::ver>,

        pub(crate) __pad0: Pad<0x50>,
        pub(crate) unk_160: U64,
        pub(crate) unk_168: U64,
        pub(crate) stats_vtx: GpuPointer<'a, super::GpuGlobalStatsVtx>,
        pub(crate) stats_frag: GpuPointer<'a, super::GpuGlobalStatsFrag::ver>,
        pub(crate) stats_comp: GpuPointer<'a, super::GpuStatsComp>,
        pub(crate) hwdata_a: GpuPointer<'a, &'a [u8]>,
        pub(crate) unkptr_190: GpuPointer<'a, &'a [u8]>,
        pub(crate) unkptr_198: GpuPointer<'a, &'a [u8]>,
        pub(crate) hwdata_b: GpuPointer<'a, super::HwDataB::ver>,
        pub(crate) hwdata_b_2: GpuPointer<'a, super::HwDataB::ver>,
        pub(crate) fwlog_buf: Option<GpuWeakPointer<[channels::RawFwLogPayloadMsg]>>,
        pub(crate) unkptr_1b8: GpuPointer<'a, &'a [u8]>,

        #[ver(G < G14X)]
        pub(crate) unkptr_1c0: GpuPointer<'a, &'a [u8]>,
        #[ver(G < G14X)]
        pub(crate) unkptr_1c8: GpuPointer<'a, &'a [u8]>,

        pub(crate) unk_1d0: u32,
        pub(crate) unk_1d4: u32,
        pub(crate) unk_1d8: Array<0x3c, u8>,
        pub(crate) buffer_mgr_ctl_gpu_addr: U64,
        pub(crate) buffer_mgr_ctl_fw_addr: U64,
        pub(crate) __pad1: Pad<0x5c>,
        pub(crate) gpu_scratch: RuntimeScratch::ver,
    }
    #[versions(AGX)]
    no_debug!(RuntimePointers::ver<'_>);

    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct PendingStamp {
        pub(crate) info: AtomicU32,
        pub(crate) wait_value: AtomicU32,
    }
    default_zeroed!(PendingStamp);

    #[derive(Debug, Clone, Copy)]
    #[repr(C, packed)]
    pub(crate) struct FaultInfo {
        pub(crate) unk_0: u32,
        pub(crate) unk_4: u32,
        pub(crate) queue_uuid: u32,
        pub(crate) unk_c: u32,
        pub(crate) unk_10: u32,
        pub(crate) unk_14: u32,
    }
    default_zeroed!(FaultInfo);

    #[derive(Debug, Clone, Copy)]
    #[repr(C)]
    pub(crate) struct PowerZoneGlobal {
        pub(crate) target: u32,
        pub(crate) target_off: u32,
        pub(crate) filter_tc: u32,
    }
    default_zeroed!(PowerZoneGlobal);

    #[versions(AGX)]
    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct Globals {
        pub(crate) ktrace_enable: u32,
        pub(crate) unk_4: Array<0x20, u8>,

        #[ver(V >= V13_2)]
        pub(crate) unk_24_0: u32,

        pub(crate) unk_24: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) debug: u32,

        #[ver(V >= V13_3)]
        pub(crate) unk_28_4: u32,

        pub(crate) unk_28: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_2c_0: u32,

        pub(crate) unk_2c: u32,
        pub(crate) unk_30: u32,
        pub(crate) unk_34: u32,
        pub(crate) unk_38: Array<0x1c, u8>,

        // pub(crate) sub: GlobalsSub::ver,
        pub(crate) unk_54: u16,
        pub(crate) unk_56: u16,
        pub(crate) unk_58: u16,
        pub(crate) unk_5a: U32,
        pub(crate) unk_5e: U32,
        pub(crate) unk_62: U32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_66_0: Array<0xc, u8>,

        pub(crate) unk_66: U32,
        pub(crate) unk_6a: Array<0x16, u8>,
        // end GlobalsSub::ver

        pub(crate) unk_80: Array<0xf80, u8>,
        pub(crate) unk_1000: Array<0x7000, u8>,
        pub(crate) unk_8000: Array<0x900, u8>,

        #[ver(G >= G14X)]
        pub(crate) unk_8900_pad: Array<0x484c, u8>,

        #[ver(V >= V13_3)]
        pub(crate) unk_8900_pad2: Array<0x54, u8>,

        pub(crate) unk_8900: u32,
        pub(crate) pending_submissions: AtomicU32,
        pub(crate) max_power: u32,
        pub(crate) max_pstate_scaled: u32,
        pub(crate) max_pstate_scaled_2: u32,
        pub(crate) unk_8914: u32,
        pub(crate) unk_8918: u32,
        pub(crate) max_pstate_scaled_3: u32,
        pub(crate) unk_8920: u32,
        pub(crate) power_zone_count: u32,
        pub(crate) avg_power_filter_tc_periods: u32,
        pub(crate) avg_power_ki_dt: u32,
        pub(crate) avg_power_kp: u32,
        pub(crate) avg_power_min_duty_cycle: u32,
        pub(crate) avg_power_target_filter_tc: u32,
        pub(crate) power_zones: Array<5, PowerZoneGlobal>,
        pub(crate) unk_8978: Array<0x44, u8>,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_89bc_0: Array<0x3c, u8>,

        pub(crate) unk_89bc: u32,
        pub(crate) fast_die0_release_temp: u32,
        pub(crate) unk_89c4: i32,
        pub(crate) fast_die0_prop_tgt_delta: u32,
        pub(crate) fast_die0_kp: u32,
        pub(crate) fast_die0_ki_dt: u32,
        pub(crate) unk_89d4: Array<0xc, u8>,
        pub(crate) unk_89e0: u32,
        pub(crate) max_power_2: u32,
        pub(crate) ppm_kp: u32,
        pub(crate) ppm_ki_dt: u32,
        pub(crate) unk_89f0: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_89f4_0: Array<0x8, u8>,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_89f4_8: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_89f4_c: Array<0x50, u8>,

        #[ver(V >= V13_3)]
        pub(crate) unk_89f4_5c: Array<0xc, u8>,

        pub(crate) unk_89f4: u32,
        pub(crate) hws1: HwDataShared1,
        pub(crate) hws2: HwDataShared2,

        #[ver(V >= V13_0B4)]
        pub(crate) idle_off_standby_timer: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_hws2_4: Array<0x8, u32>,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_hws2_24: u32,

        pub(crate) unk_hws2_28: u32,

        pub(crate) hws3: HwDataShared3,
        pub(crate) unk_9004: Array<8, u8>,
        pub(crate) unk_900c: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_9010_0: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_9010_4: Array<0x14, u8>,

        pub(crate) unk_9010: Array<0x2c, u8>,
        pub(crate) unk_903c: u32,
        pub(crate) unk_9040: Array<0xc0, u8>,
        pub(crate) unk_9100: Array<0x6f00, u8>,
        pub(crate) unk_10000: Array<0xe50, u8>,
        pub(crate) unk_10e50: u32,
        pub(crate) unk_10e54: Array<0x2c, u8>,

        #[ver((G >= G14X && V < V13_3) || (G <= G14 && V >= V13_3))]
        pub(crate) unk_x_pad: Array<0x4, u8>,

        // bit 0: sets sgx_reg 0x17620
        // bit 1: sets sgx_reg 0x17630
        pub(crate) fault_control: u32,
        pub(crate) do_init: u32,
        pub(crate) unk_10e88: Array<0x188, u8>,
        pub(crate) idle_ts: U64,
        pub(crate) idle_unk: U64,
        pub(crate) progress_check_interval_3d: u32,
        pub(crate) progress_check_interval_ta: u32,
        pub(crate) progress_check_interval_cl: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_1102c_0: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_1102c_4: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_1102c_8: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_1102c_c: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_1102c_10: u32,

        pub(crate) unk_1102c: u32,
        pub(crate) idle_off_delay_ms: AtomicU32,
        pub(crate) fender_idle_off_delay_ms: u32,
        pub(crate) fw_early_wake_timeout_ms: u32,
        #[ver(V == V13_3)]
        pub(crate) ps_pad_0: Pad<0x8>,
        pub(crate) pending_stamps: Array<0x100, PendingStamp>,
        #[ver(V != V13_3)]
        pub(crate) ps_pad_0: Pad<0x8>,
        pub(crate) unkpad_ps: Pad<0x78>,
        pub(crate) unk_117bc: u32,
        pub(crate) fault_info: FaultInfo,
        pub(crate) counter: u32,
        pub(crate) unk_118dc: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_118e0_0: Array<0x9c, u8>,

        #[ver(G >= G14X)]
        pub(crate) unk_118e0_9c: Array<0x580, u8>,

        #[ver(V >= V13_3)]
        pub(crate) unk_118e0_9c_x: Array<0x8, u8>,

        pub(crate) cl_context_switch_timeout_ms: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) cl_kill_timeout_ms: u32,

        pub(crate) cdm_context_store_latency_threshold: u32,
        pub(crate) unk_118e8: u32,
        pub(crate) unk_118ec: Array<0x400, u8>,
        pub(crate) unk_11cec: Array<0x54, u8>,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_11d40: Array<0x19c, u8>,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_11edc: u32,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_11ee0: Array<0x1c, u8>,

        #[ver(V >= V13_0B4)]
        pub(crate) unk_11efc: u32,

        #[ver(V >= V13_3)]
        pub(crate) unk_11f00: Array<0x280, u8>,
    }
    #[versions(AGX)]
    default_zeroed!(Globals::ver);

    #[derive(Debug, Default, Clone, Copy)]
    #[repr(C, packed)]
    pub(crate) struct UatLevelInfo {
        pub(crate) unk_3: u8,
        pub(crate) unk_1: u8,
        pub(crate) unk_2: u8,
        pub(crate) index_shift: u8,
        pub(crate) num_entries: u16,
        pub(crate) unk_4: u16,
        pub(crate) unk_8: U64,
        pub(crate) unk_10: U64,
        pub(crate) index_mask: U64,
    }

    #[versions(AGX)]
    #[derive(Debug)]
    #[repr(C)]
    pub(crate) struct InitData<'a> {
        #[ver(V >= V13_0B4)]
        pub(crate) ver_info: Array<0x4, u16>,

        pub(crate) unk_buf: GpuPointer<'a, &'a [u8]>,
        pub(crate) unk_8: u32,
        pub(crate) unk_c: u32,
        pub(crate) runtime_pointers: GpuPointer<'a, super::RuntimePointers::ver>,
        pub(crate) globals: GpuPointer<'a, super::Globals::ver>,
        pub(crate) fw_status: GpuPointer<'a, super::FwStatus>,
        pub(crate) uat_page_size: u16,
        pub(crate) uat_page_bits: u8,
        pub(crate) uat_num_levels: u8,
        pub(crate) uat_level_info: Array<0x3, UatLevelInfo>,
        pub(crate) __pad0: Pad<0x14>,
        pub(crate) host_mapped_fw_allocations: u32,
        pub(crate) unk_ac: u32,
        pub(crate) unk_b0: u32,
        pub(crate) unk_b4: u32,
        pub(crate) unk_b8: u32,
    }
}

#[derive(Debug)]
pub(crate) struct ChannelRing<T: GpuStruct + Debug + Default, U: Copy>
where
    for<'a> <T as GpuStruct>::Raw<'a>: Debug,
{
    pub(crate) state: GpuObject<T>,
    pub(crate) ring: GpuArray<U>,
}

impl<T: GpuStruct + Debug + Default, U: Copy> ChannelRing<T, U>
where
    for<'a> <T as GpuStruct>::Raw<'a>: Debug,
{
    pub(crate) fn to_raw(&self) -> raw::ChannelRing<T, U> {
        raw::ChannelRing {
            state: Some(self.state.weak_pointer()),
            ring: Some(self.ring.weak_pointer()),
        }
    }
}

trivial_gpustruct!(FwStatus);
trivial_gpustruct!(GpuGlobalStatsVtx);
#[versions(AGX)]
trivial_gpustruct!(GpuGlobalStatsFrag::ver);
trivial_gpustruct!(GpuStatsComp);


#[versions(AGX)]
trivial_gpustruct!(HwDataB::ver);

#[versions(AGX)]
#[derive(Debug)]
pub(crate) struct Stats {
    pub(crate) vtx: GpuObject<GpuGlobalStatsVtx>,
    pub(crate) frag: GpuObject<GpuGlobalStatsFrag::ver>,
    pub(crate) comp: GpuObject<GpuStatsComp>,
}

#[versions(AGX)]
#[derive(Debug)]
pub(crate) struct RuntimePointers {
    pub(crate) stats: Stats::ver,

    pub(crate) hwdata_a: GpuArray<u8>,
    pub(crate) unkptr_190: GpuArray<u8>,
    pub(crate) unkptr_198: GpuArray<u8>,
    pub(crate) hwdata_b: GpuObject<HwDataB::ver>,

    pub(crate) unkptr_1b8: GpuArray<u8>,
    pub(crate) unkptr_1c0: GpuArray<u8>,
    pub(crate) unkptr_1c8: GpuArray<u8>,

    pub(crate) buffer_mgr_ctl: gem::ObjectRef,
    pub(crate) buffer_mgr_ctl_low_mapping: Option<mmu::KernelMapping>,
    pub(crate) buffer_mgr_ctl_high_mapping: Option<mmu::KernelMapping>,
}

#[versions(AGX)]
impl GpuStruct for RuntimePointers::ver {
    type Raw<'a> = raw::RuntimePointers::ver<'a>;
}

#[versions(AGX)]
trivial_gpustruct!(Globals::ver);

#[versions(AGX)]
#[derive(Debug)]
pub(crate) struct InitData {
    pub(crate) unk_buf: GpuArray<u8>,
    pub(crate) runtime_pointers: GpuObject<RuntimePointers::ver>,
    pub(crate) globals: GpuObject<Globals::ver>,
    pub(crate) fw_status: GpuObject<FwStatus>,
}

#[versions(AGX)]
impl GpuStruct for InitData::ver {
    type Raw<'a> = raw::InitData::ver<'a>;
}
