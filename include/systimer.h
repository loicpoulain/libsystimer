/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * This file is part of libsystimer.
 *
 * Copyright (C) 2022 Loic Poulain <loic.poulain@linaro.org>
 */

#ifndef __LIBSYSTIMER_SYSPOWER_H__
#define __LIBSYSTIMER_SYSPOWER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

int systimer_create(clockid_t clock_id);
void systimer_destroy(int fd);
int systimer_start(int fd, unsigned long timeout_ms);
bool systimer_expired(int fd);
int systimer_stop(unsigned int fd);
unsigned long systimer_gettime_ms(clockid_t clock_id);
unsigned long systimer_gettime_s(clockid_t clock_id);
int systimer_wait(unsigned int fd);
bool systimer_active(int fd);
unsigned long systimer_value(unsigned int fd);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LIBSYSPOWER_SYSPOWER_H__ */
