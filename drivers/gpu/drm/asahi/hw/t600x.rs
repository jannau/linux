// SPDX-License-Identifier: GPL-2.0-only OR MIT

//! Hardware configuration for t600x (M1 Pro/Max/Ultra) platforms.

use super::*;

const fn iomaps(mcc_count: usize, has_die1: bool) -> [Option<IOMapping>; 20] {
    [
        Some(IOMapping::new(0x404d00000, false, 1, 0x1c000, 0, true)), // Fender
        Some(IOMapping::new(0x20e100000, false, 1, 0x4000, 0, false)), // AICTimer
        Some(IOMapping::new(0x28e104000, false, 1, 0x4000, 0, true)),  // AICSWInt
        Some(IOMapping::new(0x404000000, false, 1, 0x20000, 0, true)), // RGX
        None,                                                          // UVD
        None,                                                          // unused
        None,                                                          // DisplayUnderrunWA
        Some(IOMapping::new(0x28e494000, true, 1, 0x4000, 0, false)), // AnalogTempSensorControllerRegs
        None,                                                         // PMPDoorbell
        Some(IOMapping::new(0x404d80000, false, 1, 0x8000, 0, true)), // MetrologySensorRegs
        Some(IOMapping::new(0x204d61000, false, 1, 0x1000, 0, true)), // GMGIFAFRegs
        Some(IOMapping::new(
            0x200000000,
            true,
            mcc_count,
            0xd8000,
            0x1000000,
            true,
        )), // MCache registers
        None,                                                         // AICBankedRegisters
        None,                                                         // PMGRScratch
        Some(IOMapping::new(0x2643c4000, false, 1, 0x1000, 0, true)), // NIA Special agent idle register die 0
        if has_die1 {
            // NIA Special agent idle register die 1
            Some(IOMapping::new(0x22643c4000, false, 1, 0x1000, 0, true))
        } else {
            None
        },
        None,                                                          // CRE registers
        None,                                                          // Streaming codec registers
        Some(IOMapping::new(0x28e3d0000, false, 1, 0x1000, 0, true)),  // ?
        Some(IOMapping::new(0x28e3c0000, false, 1, 0x2000, 0, false)), // ?
    ]
}

pub(crate) const HWCONFIG_T6002: super::HwConfig = HwConfig {
    chip_id: 0x6002,
    gpu_gen: GpuGen::G13,
    gpu_variant: GpuVariant::D,
    gpu_core: GpuCore::G13C,

    base_clock_hz: 24_000_000,
    uat_oas: 42,
    num_dies: 2,
    max_num_clusters: 8,
    max_num_cores: 8,
    max_num_frags: 8,
    max_num_gps: 4,

    preempt1_size: 0x540,
    preempt2_size: 0x280,
    preempt3_size: 0x20,
    compute_preempt1_size: 0x3bd00,
    clustering: Some(HwClusteringConfig {
        meta1_blocksize: 0x44,
        meta2_size: 0xc0 * 8,
        meta3_size: 0x280 * 8,
        meta4_size: 0x30 * 16,
        max_splits: 16,
    }),

    render: HwRenderConfig {
        tiling_control: 0xa540,
    },

    io_mappings: &iomaps(8, true),
    sram_base: None,
    sram_size: None,
};

pub(crate) const HWCONFIG_T6001: super::HwConfig = HwConfig {
    chip_id: 0x6001,
    gpu_variant: GpuVariant::C,
    gpu_core: GpuCore::G13C,

    num_dies: 1,
    max_num_clusters: 4,
    io_mappings: &iomaps(8, false),
    ..HWCONFIG_T6002
};

pub(crate) const HWCONFIG_T6000: super::HwConfig = HwConfig {
    chip_id: 0x6000,
    gpu_variant: GpuVariant::S,
    gpu_core: GpuCore::G13S,

    max_num_clusters: 2,
    io_mappings: &iomaps(4, false),
    ..HWCONFIG_T6001
};
