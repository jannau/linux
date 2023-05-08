// SPDX-License-Identifier: GPL-2.0-only
/*
 * Contains implementation-defined CPU feature definitions.
 */

#include <asm/cpufeature.h>

void __init init_cpucap_indirect_list_from_array(const struct arm64_cpu_capabilities *caps);
bool feature_matches(u64 reg, const struct arm64_cpu_capabilities *entry);

bool has_tso_fixed(const struct arm64_cpu_capabilities *entry, int scope)
{
	/* List of CPUs that always use the TSO memory model */
	static const struct midr_range fixed_tso_list[] = {
		MIDR_ALL_VERSIONS(MIDR_NVIDIA_DENVER),
		MIDR_ALL_VERSIONS(MIDR_NVIDIA_CARMEL),
		MIDR_ALL_VERSIONS(MIDR_FUJITSU_A64FX),
		{ /* sentinel */ }
	};

	return is_midr_in_range_list(read_cpuid_id(), fixed_tso_list);
}

static const struct arm64_cpu_capabilities arm64_impdef_features[] = {
#ifdef CONFIG_ARM64_MEMORY_MODEL_CONTROL
	{
		.desc = "TSO memory model (Fixed)",
		.capability = ARM64_HAS_TSO_FIXED,
		.type = ARM64_CPUCAP_SYSTEM_FEATURE,
		.matches = has_tso_fixed,
	},
#endif
	{},
};

void __init init_cpucap_indirect_list_impdef(void)
{
	init_cpucap_indirect_list_from_array(arm64_impdef_features);
}
