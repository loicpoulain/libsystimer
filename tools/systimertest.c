#include <errno.h>
#include <systimer.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array)[0]))

static const char *systimer_list[] = {
	[CLOCK_REALTIME] = 		"CLOCK_REALTIME          ",
	[CLOCK_MONOTONIC] = 		"CLOCK_MONOTONIC         ",
	[CLOCK_PROCESS_CPUTIME_ID] = 	"CLOCK_PROCESS_CPUTIME_ID",
	[CLOCK_THREAD_CPUTIME_ID] = 	"CLOCK_THREAD_CPUTIME_ID ",
	[CLOCK_MONOTONIC_RAW] = 	"CLOCK_MONOTONIC_RAW     ",
	[CLOCK_REALTIME_COARSE] = 	"CLOCK_REALTIME_COARSE   ",
	[CLOCK_MONOTONIC_COARSE] = 	"CLOCK_MONOTONIC_COARSE  ",
	[CLOCK_BOOTTIME] = 		"CLOCK_BOOTTIME          ",
	[CLOCK_REALTIME_ALARM] = 	"CLOCK_REALTIME_ALARM    ",
	[CLOCK_BOOTTIME_ALARM] = 	"CLOCK_BOOTTIME_ALARM    ",
};

void print_clock(unsigned int clock_id)
{
	unsigned long value = systimer_gettime_s(clock_id);

	printf("[%02u] %s\t%lu\n", clock_id, systimer_list[clock_id], value);
}

void list_clocks(void)
{
	unsigned int i = 0;

	for (i = 0; i < ARRAY_SIZE(systimer_list); i++) {
		print_clock(i);
	}
}

void wait_for(unsigned int clock_id, unsigned int seconds)
{
	int fd, ret;

	if (clock_id >= ARRAY_SIZE(systimer_list)) {
		fprintf(stderr, "Unknown clock id %u\n", clock_id);
		exit(EINVAL);
	}

	printf("Using %s\n", systimer_list[clock_id]);

	fd = systimer_create(clock_id);
	if (fd < 0) {
		perror("Unable to create systimer");
		return;
	}

	ret = systimer_start(fd, seconds * 1000);
	if (ret) {
		perror("Unable to start systimer");
		return;
	}

	ret = systimer_wait(fd);
	if (ret < 0) {
		perror("Error waiting for the timer");
		return;
	}

	systimer_destroy(fd);
}

void usage(char *name)
{
	printf("Usage: %s [list-clocks|wait <seconds> [clock-id]]\n", name);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		usage(argv[0]);
		exit(EINVAL);
	}

	if (argc == 2) {
		if (!strcmp("list-clocks", argv[1]))
			list_clocks();
	} else if (argc == 3 || argc == 4) {
		if (strcmp("wait", argv[1])) {
			usage(argv[0]);
			exit(EINVAL);
		}
		if (argc == 3)
			wait_for(CLOCK_MONOTONIC, atoi(argv[2]));
		else
			wait_for(atoi(argv[3]), atoi(argv[2]));
	} else {
		usage(argv[0]);
		exit(EINVAL);
	}

	return 0;
}