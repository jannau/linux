// SPDX-License-Identifier: GPL-2.0

//! Synchronisation primitives.
//!
//! This module contains the kernel APIs related to synchronisation that have been ported or
//! wrapped for usage by Rust code in the kernel.

mod arc;

use core::{cell::UnsafeCell, mem::MaybeUninit};

pub use arc::{Arc, ArcBorrow, UniqueArc};

/// Represents a lockdep class. It's a wrapper around C's `lock_class_key`.
#[repr(transparent)]
pub struct LockClassKey(UnsafeCell<MaybeUninit<bindings::lock_class_key>>);

// SAFETY: This is a wrapper around a lock class key, so it is safe to use references to it from
// any thread.
unsafe impl Sync for LockClassKey {}

impl LockClassKey {
    /// Creates a new lock class key.
    pub const fn new() -> Self {
        Self(UnsafeCell::new(MaybeUninit::uninit()))
    }

    #[allow(dead_code)]
    pub(crate) fn get(&self) -> *mut bindings::lock_class_key {
        self.0.get().cast()
    }
}
