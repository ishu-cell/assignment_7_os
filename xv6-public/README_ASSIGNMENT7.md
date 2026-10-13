# OS Lab Assignment 7 — xv6-public solution package

This package targets the classic x86 `xv6-public` tree. It contains kernel changes and user programs for all four questions in `OS_Lab_Assignment_7_xv6.pdf`.

## Build and run

```sh
make clean
make
make fs.img
make CPUS=1 qemu-nox
```

At the xv6 shell, run:

```text
threadtest
synctest
deadlock
priotest
```

The Q1 counter is deliberately unsynchronized, so the final value can be less than 300000. `synctest` wraps each increment in a counting semaphore and should reach 300000. `deadlock` simulates resource-allocation matrices in user space. `priotest` forks CPU-bound children at different priorities.

## Implemented components

- `clone()` creates a kernel-scheduled thread sharing its parent's page directory; `join()` reaps a finished thread without freeing the shared address space. The intended use is to join all threads before the creating process exits or forks.
- Kernel semaphore calls use the existing xv6 `sleep()`/`wakeup()`-based semaphore implementation from the previous lab work. `sem_wait()` and `sem_post()` are the exposed user APIs.
- `deadlock.c` includes a matrix-based detection demonstration and explains the safety check used by Banker's Algorithm.
- `set_priority()` / `get_priority()` and the scheduler's priority selection and aging counter are added. Priority values are 1 (highest) through 20 (lowest), with default 10.
- `FSSIZE` is increased to 2000 blocks so the extra test programs fit in the filesystem image.

## Important testing note

The kernel and filesystem image build successfully with the included GCC-compatible Makefile settings. QEMU was not available in the build environment, so no fabricated runtime transcript or screenshot is included. Run the programs in your own xv6/QEMU environment and capture the real output for submission.

## Design notes / limitations to understand for viva

- This is a teaching implementation of shared-address-space threads, not a production-grade pthread implementation. Join threads before the parent exits; do not concurrently grow/shrink the address space while threads run.
- The deadlock program is a user-space matrix simulation. The assignment also asks for a real pipe circular-wait demonstration; use the matrix output as the simulation and add a live pipe demonstration if your evaluator requires it.
- Priority aging raises a waiting process's priority one level every 31 scheduler passes while it remains RUNNABLE. Processes with equal priority are selected in process-table scan order, so this is a simple educational tie policy rather than a strict rotating queue.
- `synctest` includes a bounded producer-consumer demonstration using empty/full/mutex semaphores and prints produced/consumed items.
