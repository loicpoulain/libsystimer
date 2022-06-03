#
# libsystimer

This project includes both a generic **systimer library**, which is a simple wrapper for managing timerfd based timer (milliseconds), and a QT QSysTimer library, sitting on top of systimer and offering **QSysTimer** class to QT applications.

## QT QSysTimer class

A QSysTimer object can be used in the same way as standard QTimer, but with the possibility to choose the clock ID. Depending the selected clock ID, the timer can continue ticking during system sleep and even waking it up.

in one of:

- CLOCK_REALTIME
A settable system-wide real-time clock.

- CLOCK_MONOTONIC
A nonsettable monotonically increasing clock that measures time from some unspecified point in the past that does not change after system startup.

- CLOCK_PROCESS_CPUTIME_ID
A clock that measures (user and system) CPU time consumed by (all of the threads in) the calling process.

- CLOCK_THREAD_CPUTIME_ID
A clock that measures (user and system) CPU time consumed by the calling thread.

- CLOCK_MONOTONIC_RAW
Similar to CLOCK_MONOTONIC, but provides access to a raw hardware-based time that is not subject to NTP adjustments or the incremental adjustments performed by adjtime.

- CLOCK_REALTIME_COARSE
A faster but less precise version of CLOCK_REALTIME. Use when you need very fast, but not fine-grained timestamps.

- CLOCK_MONOTONIC_COARSE
A faster but less precise version of CLOCK_MONOTONIC. Use when you need very fast, but not fine-grained timestamps

- CLOCK_BOOTTIME
Like CLOCK_MONOTONIC, this is a monotonically increasing clock. However, whereas the CLOCK_MONOTONIC clock does not measure the time while a system is suspended, the CLOCK_BOOTTIME clock does include the time during which the system is suspended.  This is useful for applications that need to be suspend-aware. CLOCK_REALTIME is not suitable for such applications, since that clock is affected by discontinuous changes to the system clock.

- CLOCK_REALTIME_ALARM
This clock is like CLOCK_REALTIME, but will wake the system if it is suspended. The caller must have the CAP_WAKE_ALARM capability in order to set a timer against this clock.

- CLOCK_BOOTTIME_ALARM
This clock is like CLOCK_BOOTTIME, but will wake the system if it is suspended. The caller must have the CAP_WAKE_ALARM capability in order to set a timer against this clock.