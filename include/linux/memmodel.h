// SPDX-License-Identifier: GPL-2.0
// Copyright(c) The Asahi Linux Contributors

#ifndef _LINUX_MEMMODEL_H
#define _LINUX_MEMMODEL_H

struct task_struct;

/* Memory model prctl */
int arch_prctl_mem_model_get(struct task_struct *t);
int arch_prctl_mem_model_set(struct task_struct *t, unsigned long val);

#endif /* _LINUX_MEMMODEL_H */
