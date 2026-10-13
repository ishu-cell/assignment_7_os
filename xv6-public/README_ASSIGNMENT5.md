# OS Lab Assignment 5 — Process Synchronization using xv6

This package is based on the classic **xv6-public x86** source tree used in the uploaded project. The assignment PDF describes `kernel/` and `user/` directories used by newer xv6-riscv trees; this package intentionally uses the flat xv6-public layout so it matches the source tree you were using on Ubuntu.

## Build on Ubuntu

```bash
cd ~/xv6-public
make clean
make
make fs.img
```

If your Ubuntu installation has the same old-xv6/new-GCC compatibility issue, the Makefile already contains `-Wno-error=array-bounds`.

Run one CPU for clearer synchronization output:

```bash
make CPUS=1 qemu-nox
```

Inside xv6 run:

```text
$ peterson
$ prodcons
$ readwrite
$ dining
```

Exit QEMU with `Ctrl-A`, then `X`.

## Implemented kernel features

### Shared memory
`shm_get()` allocates one shared physical page on first use and maps it at `SHMBASE` (`0x7ffff000`). A child created after the page exists receives a mapping to the same physical page. The shared page is not freed with an individual process's address space. This is sufficient for the required parent/child sharing model.

### Semaphores
`semaphore.c` implements counting semaphores backed by xv6 `sleep()`/`wakeup()` and per-semaphore spinlocks. User programs call `sem_init`, `sem_wait`, `sem_post`, and `sem_destroy` through new system calls.

## Q1 — Peterson
The shared page contains `flag[2]`, `turn`, and `shared_counter`. The parent and one child execute ten iterations. Peterson's algorithm is the actual mutual-exclusion mechanism; no semaphore or xv6 lock protects its critical section. Expected final counter: 20.

## Q2 — Producer/Consumer
The buffer has five slots. `empty` starts at 5, `full` at 0, and `mutex` at 1. The producer inserts 1..20; the consumer removes them FIFO. Diagnostic messages are printed before semaphore waits so blocking on an empty/full condition can be observed.

## Q3 — Readers/Writers
There are three readers and two writers. Readers share `rw_mutex`; writers hold it exclusively. `read_count` is protected by `mutex`. A `turnstile` prevents indefinite writer starvation by stopping new readers from overtaking a waiting writer. Output includes timestamps (`uptime()`), PIDs, and active-reader count.

## Q4 — Dining Philosophers
Five philosopher processes use five binary semaphores as forks. Each philosopher acquires the lower-numbered fork first and the higher-numbered fork second. This resource-ordering strategy prevents circular wait and therefore avoids deadlock. Each philosopher completes five cycles.

## Submission
The assignment asks for source files, Makefile, README, and real terminal logs/screenshots. The `output_logs/` directory contains instructions for saving your own genuine QEMU transcripts.
