# Assignment 6 — Design Report

## Q1: Banker's Algorithm

The classic 5-process/3-resource example is used. `Need[i][j]` is computed as `Max[i][j] - Allocation[i][j]`. The Safety Algorithm copies Available into Work, repeatedly finds an unfinished process whose Need is no greater than Work, and releases that process's Allocation into Work. If every process can finish, the state is safe and the resulting order is the safe sequence.

The Resource Request Algorithm first checks `Request <= Need` and `Request <= Available`. It temporarily grants the request, executes the Safety Algorithm, commits the grant only when the resulting state is safe, and otherwise restores the old Allocation/Available values.

## Q2: Deadlock Detection

For each process `Pi` requesting resource `Rk`, an edge `Pi -> Pj` is added whenever `Pj` currently holds an instance of `Rk`. The resulting wait-for graph is tested with DFS. A back edge to a gray node identifies a cycle. The program reconstructs and prints the cycle, including a three-process circular wait.

## Q3: Resource Ordering

Two binary semaphores represent Lock1 and Lock2. In the bad phase, the two processes intentionally acquire the resources in opposite orders, producing circular wait. In the fixed phase, every process follows the same global order, Lock1 before Lock2. With a strict total order, a cycle in the wait-for graph is impossible: a process waiting for a higher-numbered resource cannot eventually wait on a lower-numbered resource and return to the original process.

## Q4: Combined Synchronization and Deadlock Avoidance

Five real xv6 child processes share three limited resource pools: two printers, one scanner, and two disks. Each process needs two resource types. Counting semaphores enforce the instance limits. The explicit prevention strategy is the same global resource ordering used in Q3: resources are always acquired from R0 to R2 and released in reverse order. Therefore circular wait cannot occur. Every child repeats the workload several times and prints every request, grant, work, and release transition.
