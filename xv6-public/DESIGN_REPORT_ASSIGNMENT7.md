# Assignment 7 — Design report

## Q1: User-level threads supported by the kernel

`clone(fcn,arg,stack)` allocates a process-table entry and kernel stack but reuses the caller's page directory and address-space size. The copied trap frame is adjusted so execution starts at `fcn(arg)` on the supplied user stack. The entry is marked as a thread. `join()` sleeps until a child thread reaches ZOMBIE, then frees its kernel stack and process-table entry without freeing the shared page directory. `threadtest` launches three workers, each incrementing a shared global counter 100,000 times. Since increment is a read-modify-write operation, lost updates can make the unsynchronized result smaller than 300,000.

## Q2: Synchronization with semaphores

The inherited kernel semaphore table provides blocking `sem_wait` and waking `sem_post` operations using xv6's sleep/wakeup mechanism. `synctest` protects the shared counter increment with a semaphore initialized to one, enforcing mutual exclusion and producing 300,000. Its producer-consumer section uses `empty=5`, `full=0`, and `mutex=1` to represent free slots, occupied slots, and exclusive buffer access.

## Q3: Deadlock detection / avoidance

`deadlock.c` represents current allocations, outstanding requests, and available resource units with static matrices. Detection repeatedly marks a process finishable when its request is no greater than the current work vector, then releases its allocation into that vector. Any process remaining unfinished is reported as deadlocked. Banker's avoidance uses `Need=Max-Allocation` and only commits a hypothetical request if the resulting state has a safe sequence. The sample is a matrix-level simulation, not a kernel resource allocator.

## Q4: Priority scheduling with aging

Each process has priority 1–20 (default 10) and a runnable wait counter. The scheduler chooses the runnable process with the smallest numeric priority. A process waiting more than 30 scheduler passes gets its priority number reduced by one, down to 1, and its wait counter resets. This improves starvation resistance for low-priority processes. `priotest` creates four CPU-bound children with distinct priorities and reports their completion.

## Build / verification

The classic x86 xv6 kernel and filesystem image compile with the included Makefile. Runtime output should be captured from QEMU on the submission machine; this report does not claim that screenshots or a QEMU transcript were captured in the build environment.
