# OS Lab Assignment 6 — Deadlocks and Avoidance (xv6-public)

This package implements all four questions from Lab Assignment 6 for the classic **xv6-public x86** tree.
The assignment PDF asks for Banker's Algorithm, RAG/wait-for-graph deadlock detection, resource ordering with real xv6 processes, and a combined synchronization/deadlock-avoidance program. The programs below implement those requirements.

## Programs

### Q1 — `bankers.c`
- 5 processes and 3 resource types.
- Uses `Allocation`, `Max`, `Available`, and computes `Need = Max - Allocation` at runtime.
- Runs the Banker's Safety Algorithm and prints a safe sequence.
- Tests a safe request (`P1: 1 0 2`) and an unsafe request (`P0: 0 0 2`).
- Unsafe request is rolled back and prints: `Request denied — would lead to unsafe state`.

### Q2 — `deadlockdetect.c`
- Builds a wait-for graph from allocation/request matrices.
- Uses DFS with node coloring to detect cycles.
- Tests one acyclic case and one 3-process circular-wait case.
- Prints graph edges and the exact cycle.

### Q3 — `resourceorder.c`
- Uses the semaphore system calls already added for the synchronization lab.
- BAD phase: A acquires Lock1 then waits for Lock2; B acquires Lock2 then waits for Lock1. A timeout kills the intentionally stuck children so the program can continue.
- FIXED phase: both processes acquire `Lock1 -> Lock2` only. The same workload completes.
- This demonstrates that a global ordering removes circular wait.

### Q4 — `syncdeadlock.c`
- Creates 5 real xv6 processes.
- Resource pools: R0 Printer (2 instances), R1 Scanner (1), R2 Disk (2).
- Each process needs exactly two resource types.
- Each resource pool is protected by a counting semaphore.
- Uses global resource ordering `R0 < R1 < R2` to prevent circular wait.
- Each process performs 3 cycles: request -> grant -> work -> release.

## Existing kernel support used

This package is based on the completed synchronization version of classic xv6-public. It includes:
- `semaphore.c` kernel counting semaphores using `sleep()`/`wakeup()`.
- Shared-memory support used by the earlier Peterson/producer-consumer programs.
- Semaphore syscalls wired through `syscall.h`, `syscall.c`, `sysproc.c`, `user.h`, and `usys.S`.

The Assignment 6 programs use the semaphore interface rather than adding another synchronization implementation.

## Build and run on Ubuntu

```bash
cd xv6-public
make clean
make
make fs.img
make CPUS=1 qemu-nox
```

Inside xv6:

```text
$ bankers
$ deadlockdetect
$ resourceorder
$ syncdeadlock
```

`resourceorder` intentionally pauses for its bad-order demonstration. It prints a timeout message and kills the two stuck children, then runs the fixed version.

Exit QEMU with **Ctrl-A, then X**.

## Verification status

The kernel and all four new user programs were compiled successfully in the provided build environment. QEMU was not available in that environment, so no fake QEMU transcript is claimed as a real run. `output_logs/EXPECTED_OUTPUT.txt` contains a clearly labelled expected-output guide; after running xv6 on Ubuntu, capture the real terminal output/screenshots and place them in `output_logs/` for submission.
