// SPDX-License-Identifier: GPL-2.0+ OR MIT
/*
 * C preprocessor macros for t600x multi die support.
 */

#ifndef __DTS_APPLE_MULTI_DIE_CPP_H
#define __DTS_APPLE_MULTI_DIE_CPP_H

/* copied include/linux/stringify.h */
#define __stringify_1(x...)     #x
#define __stringify(x...)       __stringify_1(x)

#define __concat_1(x, y...)     x ## y
#define __concat(x, y...)       __concat_1(x, y)

#define die_node(a) __concat(a, DIE)
#define die_label(a) __stringify(__concat(a, DIE))

#endif /* !__LINUX_STRINGIFY_H */
