/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * This file is part of libsyspower.
 *
 * Copyright (C) 2022 Loic Poulain <loic.poulain@linaro.org>
 */

#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <systimer.h>

int systimer_create(clockid_t clock_id)
{
	int fd;

	fd = timerfd_create(clock_id, O_NONBLOCK);
	if (fd == -1)
		return -errno;

	return fd;
}

void systimer_destroy(int fd)
{
	close(fd);
}

int systimer_start(int fd, unsigned long timeout_ms)
{
	struct itimerspec expire = {
		.it_value.tv_sec = timeout_ms / 1000,
		.it_value.tv_nsec = timeout_ms % 1000 * 1000,
	};

	if (timerfd_settime(fd, 0 /* relative to now */, &expire, NULL))
		return -errno;

	return 0;
}

unsigned long systimer_gettime_ms(clockid_t clock_id)
{
	struct timespec now;
	unsigned long value;

	if (clock_gettime(clock_id, &now) == -1)
		return 0;

	value = (unsigned long)now.tv_sec * 1000;
	value += now.tv_nsec / 1000;

	return value;
}

unsigned long systimer_gettime_s(clockid_t clock_id)
{
	struct timespec now;

	if (clock_gettime(clock_id, &now) == -1)
		return 0;

	return now.tv_sec;
}

bool systimer_expired(int fd)
{
	uint64_t exp;
	int ret;

	ret = read(fd, &exp, sizeof(uint64_t));
	if (ret < 0)
		return false;

	return true;
}

bool systimer_active(int fd)
{
	struct itimerspec its;

	if (timerfd_gettime(fd, &its) == -1)
		return false;

	return (its.it_value.tv_sec != 0 || its.it_value.tv_nsec != 0);
}

unsigned long systimer_value(unsigned int fd)
{
	struct itimerspec its;

	if (timerfd_gettime(fd, &its) == -1)
		return false;

	return (its.it_value.tv_sec *1000 + its.it_value.tv_nsec / 1000);
}

int systimer_wait(unsigned int fd)
{
	fd_set rfds;
	int ret;

	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	do {
		ret = select(fd + 1, &rfds, NULL, NULL, NULL);
	} while (ret < 0 && errno == EINTR);

	if (ret == -1)
		return ret;
	if (!ret)
		return -1;

	return ret;
}

int systimer_stop(unsigned int fd)
{
	struct itimerspec expire = {
		.it_value.tv_sec = 0,
		.it_value.tv_nsec = 0,
	};

	/* disarm */
	if (timerfd_settime(fd, 0, &expire, NULL))
		return -errno;

	return 0;
}

