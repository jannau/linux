// SPDX-License-Identifier: GPL-2.0-only OR MIT

//! Hardware configuration for t8103 platforms (M1).

use super::*;

pub(crate) const HWCONFIG: super::HwConfig = HwConfig {
    chip_id: 0x8103,
    gpu_gen: GpuGen::G13,
    gpu_variant: GpuVariant::G,
    gpu_core: GpuCore::G13G,

    base_clock_hz: 24_000_000,
    uat_oas: 40,
    num_dies: 1,
    max_num_clusters: 1,
    max_num_cores: 8,
    max_num_frags: 8,
    max_num_gps: 4,

    preempt1_size: 0x540,
    preempt2_size: 0x280,
    preempt3_size: 0x20,
    compute_preempt1_size: 0x7f80,
    clustering: None,

    render: HwRenderConfig {
        // bit 0: disable clustering (always)
        tiling_control: 0xa041,
    },

    io_mappings: &[
        Some(IOMapping::new(0x204d00000, false, 1, 0x1c000, 0, true)), // Fender
        Some(IOMapping::new(0x20e100000, false, 1, 0x4000, 0, false)), // AICTimer
        Some(IOMapping::new(0x23b104000, false, 1, 0x4000, 0, true)),  // AICSWInt
        Some(IOMapping::new(0x204000000, false, 1, 0x20000, 0, true)), // RGX
        None,                                                          // UVD
        None,                                                          // unused
        None,                                                          // DisplayUnderrunWA
        Some(IOMapping::new(0x23b2e8000, false, 1, 0x1000, 0, false)), // AnalogTempSensorControllerRegs
        Some(IOMapping::new(0x23bc00000, false, 1, 0x1000, 0, true)),  // PMPDoorbell
        Some(IOMapping::new(0x204d80000, false, 1, 0x5000, 0, true)),  // MetrologySensorRegs
        Some(IOMapping::new(0x204d61000, false, 1, 0x1000, 0, true)),  // GMGIFAFRegs
        Some(IOMapping::new(0x200000000, false, 1, 0xd6400, 0, true)), // MCache registers
        None,                                                          // AICBankedRegisters
        Some(IOMapping::new(0x23b738000, false, 1, 0x1000, 0, true)),  // PMGRScratch
        None, // NIA Special agent idle register die 0
        None, // NIA Special agent idle register die 1
        None, // CRE registers
        None, // Streaming codec registers
        None, //
        None, //
    ],
    sram_base: None,
    sram_size: None,
};
