# Assignment 5 — Design Report

## Question 1: Peterson's Algorithm

A fixed user virtual address (`SHMBASE`) is mapped to one kernel-allocated physical page. The parent calls `shm_get()` before `fork()`. `fork()` maps the same physical page into the child, so both processes access the same `flag[2]`, `turn`, and `shared_counter` variables. The critical section itself is protected only by Peterson's `flag[]` and `turn`; no xv6 lock is used for the Peterson mutual-exclusion logic.

## Question 2: Producer–Consumer

`semaphore.c` implements counting semaphores in the kernel. `sem_wait()` decrements a positive count or sleeps when the count is zero. `sem_post()` increments the count and wakes sleepers. The bounded buffer uses `empty = 5`, `full = 0`, and `mutex = 1`. The producer inserts integers 1 through 20 and the consumer removes them in FIFO order.

## Question 3: Readers–Writers

`mutex` protects `read_count`, while `rw_mutex` provides exclusive writer access or group reader access. A `turnstile` is used as a fairness enhancement: when a writer is waiting, new readers cannot continually overtake it. Existing readers can finish together, and multiple readers can therefore access `shared_data` concurrently when no writer is active.

## Question 4: Dining Philosophers

Five binary semaphores represent the five forks. Each philosopher computes its two fork numbers and always acquires the lower-numbered fork first. This resource-ordering strategy removes circular wait, one of the Coffman conditions for deadlock, so a circular deadlock cannot form. Each philosopher performs five cycles.
