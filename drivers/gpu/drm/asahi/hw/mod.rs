// SPDX-License-Identifier: GPL-2.0-only OR MIT

//! Per-SoC hardware configuration structures
//!
//! This module contains the definitions used to store per-GPU and per-SoC configuration data.

use crate::fw::types::*;
use kernel::prelude::*;

pub(crate) mod t600x;
pub(crate) mod t602x;
pub(crate) mod t8103;
pub(crate) mod t8112;

/// GPU generation enumeration. Note: Part of the UABI.
#[derive(Debug, PartialEq, Copy, Clone)]
#[repr(u32)]
pub(crate) enum GpuGen {
    G13 = 13,
    G14 = 14,
}

/// GPU variant enumeration. Note: Part of the UABI.
#[derive(Debug, PartialEq, Copy, Clone)]
#[repr(u32)]
pub(crate) enum GpuVariant {
    P = 'P' as u32,
    G = 'G' as u32,
    S = 'S' as u32,
    C = 'C' as u32,
    D = 'D' as u32,
}

/// GPU revision enumeration. Note: Part of the UABI.
#[derive(Debug, PartialEq, Copy, Clone)]
#[repr(u32)]
pub(crate) enum GpuRevision {
    A0 = 0x00,
    A1 = 0x01,
    B0 = 0x10,
    B1 = 0x11,
    C0 = 0x20,
    C1 = 0x21,
}

/// GPU core type enumeration. Note: Part of the firmware ABI.
#[derive(Debug, Copy, Clone)]
#[repr(u32)]
pub(crate) enum GpuCore {
    // Unknown = 0,
    // G5P = 1,
    // G5G = 2,
    // G9P = 3,
    // G9G = 4,
    // G10P = 5,
    // G11P = 6,
    // G11M = 7,
    // G11G = 8,
    // G12P = 9,
    // G13P = 10,
    G13G = 11,
    G13S = 12,
    G13C = 13,
    // G14P = 14,
    G14G = 15,
    G14S = 16,
    G14C = 17,
    G14D = 18, // Split out, unlike G13D
}

/// An MMIO mapping used by the firmware.
#[derive(Debug, Copy, Clone)]
pub(crate) struct IOMapping {
    /// Base physical address of the mapping.
    pub(crate) base: usize,
    /// Whether this mapping should be replicated to all dies
    pub(crate) per_die: bool,
    /// Number of mappings.
    pub(crate) count: usize,
    /// Size of one mapping.
    pub(crate) size: usize,
    /// Stride between mappings.
    pub(crate) stride: usize,
    /// Whether the mapping should be writable.
    pub(crate) writable: bool,
}

impl IOMapping {
    /// Convenience constructor for a new IOMapping.
    pub(crate) const fn new(
        base: usize,
        per_die: bool,
        count: usize,
        size: usize,
        stride: usize,
        writable: bool,
    ) -> IOMapping {
        IOMapping {
            base,
            per_die,
            count,
            size,
            stride,
            writable,
        }
    }
}

/// Render command configs that vary from SoC to SoC.
#[derive(Debug, Copy, Clone)]
pub(crate) struct HwRenderConfig {
    /// Vertex/tiling-related configuration register (lsb: disable clustering)
    pub(crate) tiling_control: u32,
}

/// Static hardware clustering configuration for multi-cluster SoCs.
#[derive(Debug)]
pub(crate) struct HwClusteringConfig {
    pub(crate) meta1_blocksize: usize,
    pub(crate) meta2_size: usize,
    pub(crate) meta3_size: usize,
    pub(crate) meta4_size: usize,
    pub(crate) max_splits: usize,
}

/// Static hardware configuration for a given SoC model.
#[derive(Debug)]
pub(crate) struct HwConfig {
    /// Chip ID in hex format (e.g. 0x8103 for t8103).
    pub(crate) chip_id: u32,
    /// GPU generation.
    pub(crate) gpu_gen: GpuGen,
    /// GPU variant type.
    pub(crate) gpu_variant: GpuVariant,
    /// GPU core type ID (as known by the firmware).
    pub(crate) gpu_core: GpuCore,

    /// Base clock used used for timekeeping.
    pub(crate) base_clock_hz: u32,
    /// Output address space for the UAT on this SoC.
    pub(crate) uat_oas: u32,
    /// Number of dies on this SoC.
    pub(crate) num_dies: u32,
    /// Maximum number of clusters on this SoC.
    pub(crate) max_num_clusters: u32,
    /// Maximum number of cores per cluster for this GPU.
    pub(crate) max_num_cores: u32,
    /// Maximum number of frags per cluster for this GPU.
    pub(crate) max_num_frags: u32,
    /// Maximum number of GPs per cluster for this GPU.
    pub(crate) max_num_gps: u32,

    /// Required size of the first preemption buffer.
    pub(crate) preempt1_size: usize,
    /// Required size of the second preemption buffer.
    pub(crate) preempt2_size: usize,
    /// Required size of the third preemption buffer.
    pub(crate) preempt3_size: usize,

    /// Required size of the compute preemption buffer.
    pub(crate) compute_preempt1_size: usize,

    pub(crate) clustering: Option<HwClusteringConfig>,

    /// Rendering-relevant configuration.
    pub(crate) render: HwRenderConfig,

    /// Required MMIO mappings for this GPU/firmware.
    pub(crate) io_mappings: &'static [Option<IOMapping>],
    /// SRAM base
    pub(crate) sram_base: Option<usize>,
    /// SRAM size
    pub(crate) sram_size: Option<usize>,
}

/// Dynamic (fetched from hardware/DT) configuration.
#[derive(Debug)]
pub(crate) struct DynConfig {
    /// GPU ID configuration read from hardware.
    pub(crate) id: GpuIdConfig,
    /// Firmware version.
    #[allow(dead_code)]
    pub(crate) firmware_version: KVec<u32>,

    pub(crate) hw_data_a: KVVec<u8>,
    pub(crate) hw_data_b: KVVec<u8>,
    pub(crate) hw_globals: KVVec<u8>,
}

/// Specific GPU ID configuration fetched from SGX MMIO registers.
#[derive(Debug)]
pub(crate) struct GpuIdConfig {
    /// GPU generation (should match static config).
    pub(crate) gpu_gen: GpuGen,
    /// GPU variant type (should match static config).
    pub(crate) gpu_variant: GpuVariant,
    /// GPU silicon revision.
    pub(crate) gpu_rev: GpuRevision,
    /// Total number of GPU clusters.
    pub(crate) num_clusters: u32,
    /// Maximum number of GPU cores per cluster.
    pub(crate) num_cores: u32,
    /// Number of frags per cluster.
    pub(crate) num_frags: u32,
    /// Number of GPs per cluster.
    pub(crate) num_gps: u32,
    /// Total number of active cores for the whole GPU.
    pub(crate) total_active_cores: u32,
    /// Mask of active cores per cluster.
    pub(crate) core_masks: KVec<u32>,
    /// Packed mask of all active cores.
    pub(crate) core_masks_packed: KVec<u32>,
}
