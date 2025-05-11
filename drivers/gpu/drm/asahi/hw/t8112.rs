// SPDX-License-Identifier: GPL-2.0-only OR MIT

//! Hardware configuration for t8112 platforms (M2).

use super::*;

pub(crate) const HWCONFIG: super::HwConfig = HwConfig {
    chip_id: 0x8112,
    gpu_gen: GpuGen::G14,
    gpu_variant: GpuVariant::G,
    gpu_core: GpuCore::G14G,

    base_clock_hz: 24_000_000,
    uat_oas: 40,
    num_dies: 1,
    max_num_clusters: 1,
    max_num_cores: 10,
    max_num_frags: 10,
    max_num_gps: 4,

    preempt1_size: 0x540,
    preempt2_size: 0x280,
    preempt3_size: 0x20,
    compute_preempt1_size: 0x10000, // TODO: Check
    clustering: None,

    render: HwRenderConfig {
        // TODO: this is unused here, may be present in newer FW
        tiling_control: 0xa041,
    },

    io_mappings: &[
        Some(IOMapping::new(0x204d00000, false, 1, 0x14000, 0, true)), // Fender
        Some(IOMapping::new(0x20e100000, false, 1, 0x4000, 0, false)), // AICTimer
        Some(IOMapping::new(0x23b0c4000, false, 1, 0x4000, 0, true)),  // AICSWInt
        Some(IOMapping::new(0x204000000, false, 1, 0x20000, 0, true)), // RGX
        None,                                                          // UVD
        None,                                                          // unused
        None,                                                          // DisplayUnderrunWA
        Some(IOMapping::new(0x23b2c0000, false, 1, 0x1000, 0, false)), // AnalogTempSensorControllerRegs
        None,                                                          // PMPDoorbell
        Some(IOMapping::new(0x204d80000, false, 1, 0x8000, 0, true)),  // MetrologySensorRegs
        Some(IOMapping::new(0x204d61000, false, 1, 0x1000, 0, true)),  // GMGIFAFRegs
        Some(IOMapping::new(0x200000000, false, 1, 0xd6400, 0, true)), // MCache registers
        None,                                                          // AICBankedRegisters
        None,                                                          // PMGRScratch
        None, // NIA Special agent idle register die 0
        None, // NIA Special agent idle register die 1
        Some(IOMapping::new(0x204e00000, false, 1, 0x10000, 0, true)), // CRE registers
        Some(IOMapping::new(0x27d050000, false, 1, 0x4000, 0, true)), // Streaming codec registers
        Some(IOMapping::new(0x23b3d0000, false, 1, 0x1000, 0, true)), //
        Some(IOMapping::new(0x23b3c0000, false, 1, 0x1000, 0, false)), //
    ],
    sram_base: None,
    sram_size: None,
};
