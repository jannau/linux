// SPDX-License-Identifier: GPL-2.0-only OR MIT
/*
 * Apple Silicon hardware tunable support
 *
 * Each tunable is a list with each entry containing a offset into the MMIO
 * region, a mask of bits to be cleared and a set of bits to be set. These
 * tunables are passed along by the previous boot stages and vary from device
 * to device such that they cannot be hardcoded in the individual drivers.
 *
 * Copyright (C) The Asahi Linux Contributors
 */

#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/soc/apple/tunable.h>

int devm_apple_tunable_parse(struct device *dev, struct device_node *np,
			     struct apple_tunable *tunable, const char *name)
{
	const __be32 *p;
	struct property *prop;
	int i;

	prop = of_find_property(np, name, NULL);
	if (!prop)
		return -ENOENT;

	if (prop->length % (3 * sizeof(u32)))
		return -EINVAL;

	tunable->sz = prop->length / (3 * sizeof(u32));
	tunable->values = devm_kcalloc(dev, tunable->sz,
				       sizeof(*tunable->values), GFP_KERNEL);
	if (!tunable->values) {
		tunable->sz = 0;
		return -ENOMEM;
	}

	for (i = 0, p = NULL; i < tunable->sz; ++i) {
		p = of_prop_next_u32(prop, p, &tunable->values[i].offset);
		p = of_prop_next_u32(prop, p, &tunable->values[i].mask);
		p = of_prop_next_u32(prop, p, &tunable->values[i].value);
	}

	return 0;
}
EXPORT_SYMBOL(devm_apple_tunable_parse);

void devm_apple_tunable_free(struct device *dev, struct apple_tunable *tunable)
{
	devm_kfree(dev, tunable->values);
	tunable->sz = 0;
}
EXPORT_SYMBOL(devm_apple_tunable_free);

void apple_tunable_apply(void __iomem *regs, struct apple_tunable *tunable)
{
	size_t i;

	for (i = 0; i < tunable->sz; ++i) {
		u32 val, old_val;

		val = old_val = readl_relaxed(regs + tunable->values[i].offset);
		val &= ~tunable->values[i].mask;
		val |= tunable->values[i].value;
		if (val != old_val)
			writel_relaxed(val, regs + tunable->values[i].offset);
	}
}
EXPORT_SYMBOL(apple_tunable_apply);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Sven Peter <sven@svenpeter.dev>");
MODULE_DESCRIPTION("Apple Silicon hardware tunable support");
