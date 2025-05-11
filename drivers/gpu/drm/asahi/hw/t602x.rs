// SPDX-License-Identifier: GPL-2.0-only OR MIT

//! Hardware configuration for t600x (M1 Pro/Max/Ultra) platforms.

use super::*;

const fn iomaps(chip_id: u32, mcc_count: usize) -> [Option<IOMapping>; 24] {
    [
        Some(IOMapping::new(0x404d00000, false, 1, 0x144000, 0, true)), // Fender
        Some(IOMapping::new(0x20e100000, false, 1, 0x4000, 0, false)),  // AICTimer
        Some(IOMapping::new(0x28e106000, false, 1, 0x4000, 0, true)),   // AICSWInt
        Some(IOMapping::new(0x404000000, false, 1, 0x20000, 0, true)),  // RGX
        None,                                                           // UVD
        None,                                                           // unused
        None,                                                           // DisplayUnderrunWA
        Some(match chip_id {
            0x6020 => IOMapping::new(0x28e460000, true, 1, 0x4000, 0, false),
            _ => IOMapping::new(0x28e478000, true, 1, 0x4000, 0, false),
        }), // AnalogTempSensorControllerRegs
        None,                                                           // PMPDoorbell
        Some(IOMapping::new(0x404e08000, false, 1, 0x8000, 0, true)),   // MetrologySensorRegs
        None,                                                           // GMGIFAFRegs
        Some(IOMapping::new(
            0x200000000,
            true,
            mcc_count,
            0xd8000,
            0x1000000,
            true,
        )), // MCache registers
        Some(IOMapping::new(0x28e118000, false, 1, 0x4000, 0, false)),  // AICBankedRegisters
        None,                                                           // PMGRScratch
        None, // NIA Special agent idle register die 0
        None, // NIA Special agent idle register die 1
        None, // CRE registers
        None, // Streaming codec registers
        Some(IOMapping::new(0x28e3d0000, false, 1, 0x4000, 0, true)), // ?
        Some(IOMapping::new(0x28e3c0000, false, 1, 0x4000, 0, false)), // ?
        Some(IOMapping::new(0x28e3d8000, false, 1, 0x4000, 0, true)), // ?
        Some(IOMapping::new(0x404eac000, true, 1, 0x4000, 0, true)), // ?
        None,
        None,
    ]
}

// TODO: Tentative
pub(crate) const HWCONFIG_T6022: super::HwConfig = HwConfig {
    chip_id: 0x6022,
    gpu_gen: GpuGen::G14,
    gpu_variant: GpuVariant::D,
    gpu_core: GpuCore::G14D,

    base_clock_hz: 24_000_000,
    uat_oas: 42,
    num_dies: 2,
    max_num_clusters: 8,
    max_num_cores: 10,
    max_num_frags: 10,
    max_num_gps: 4,

    preempt1_size: 0x540,
    preempt2_size: 0x280,
    preempt3_size: 0x40,
    compute_preempt1_size: 0x25980 * 2, // Conservative guess
    clustering: Some(HwClusteringConfig {
        meta1_blocksize: 0x44,
        meta2_size: 0xc0 * 16,
        meta3_size: 0x280 * 16,
        meta4_size: 0x10 * 128,
        max_splits: 64,
    }),

    render: HwRenderConfig {
        tiling_control: 0x180340,
    },

    io_mappings: &iomaps(0x6022, 8),
    sram_base: Some(0x404d60000),
    sram_size: Some(0x20000),
};

pub(crate) const HWCONFIG_T6021: super::HwConfig = HwConfig {
    chip_id: 0x6021,
    gpu_variant: GpuVariant::C,
    gpu_core: GpuCore::G14C,

    num_dies: 1,
    max_num_clusters: 4,
    compute_preempt1_size: 0x25980,
    io_mappings: &iomaps(0x6021, 8),
    ..HWCONFIG_T6022
};

pub(crate) const HWCONFIG_T6020: super::HwConfig = HwConfig {
    chip_id: 0x6020,
    gpu_variant: GpuVariant::S,
    gpu_core: GpuCore::G14S,

    max_num_clusters: 2,
    io_mappings: &iomaps(0x6020, 4),
    ..HWCONFIG_T6021
};
