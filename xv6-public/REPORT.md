# Assignment 5 — Design Summary

## Q1: Peterson
Peterson's algorithm provides mutual exclusion for two processes using only shared `flag[]` and `turn`. Each process announces intent, gives the other process priority, and busy-waits only while the other process is interested and has the turn. The shared counter reaches 20 after 10 critical-section entries by each process.

## Q2: Producer–Consumer
The bounded buffer uses a five-slot circular array. `empty` counts available slots, `full` counts occupied slots, and `mutex` protects `in`, `out`, and buffer updates. When `empty` is zero the producer sleeps in the kernel semaphore; when `full` is zero the consumer sleeps. The sequence 1..20 is inserted and removed without loss or duplication.

## Q3: Readers–Writers
Readers share access while no writer is active. The first reader acquires `rw_mutex` and the last reader releases it. Writers acquire `rw_mutex` exclusively. `read_count` is protected by `mutex`. A turnstile is included so that a waiting writer can prevent a continuous stream of new readers from overtaking it, improving the readers-priority solution toward fairness.

## Q4: Dining Philosophers
Each fork is a binary semaphore. Resource ordering is used: each philosopher acquires the lower-numbered fork first. Because every philosopher follows the same increasing resource order, a circular wait cannot form. Therefore deadlock is avoided while all five philosophers can complete five eating cycles.
