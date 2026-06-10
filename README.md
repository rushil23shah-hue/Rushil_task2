CPU Scheduler and Cache Hierarchy Simulator
Overview

This project is a CPU Scheduling and Memory Hierarchy Simulator developed for the ProjectX selection task.

The objective was not only to simulate task execution but also to model realistic operating system and computer architecture concepts including:

Multi-core CPU scheduling
Round Robin scheduling
Cache hierarchy (L1, L2, L3, RAM)
Cache promotion policies
FIFO cache replacement
Optimal cache replacement
Performance analysis and comparison

The simulator processes tasks from an input file and executes them cycle-by-cycle while tracking cache behavior, CPU utilization, memory access costs, and scheduling performance.

Features Implemented
Round Robin Scheduler
Quantum = 3 cycles
Tasks arrive at intervals of one cycle
Tasks are moved back to the ready queue after quantum expiration
Completed tasks are removed from execution
Multi-Core Execution

The simulator supports configurable CPU core counts.

Implemented:

Single-core execution
Dual-core execution

This allows direct comparison of scheduling performance and execution speed.

Cache Hierarchy Simulation

The simulator models:

CPU → L1 Cache → L2 Cache → L3 Cache → RAM

Cache latencies:

Level	Latency
L1	4 cycles
L2	12 cycles
L3	40 cycles
RAM	200 cycles
Hierarchical Cache Promotion

Unlike a simplified cache model, blocks are not instantly copied into all cache levels.

Implemented behavior:

RAM Access

RAM → L3

A block fetched from RAM is first loaded only into L3.

L3 Hit

L3 → L2

The block is promoted upward with additional promotion cost.

L2 Hit

L2 → L1

The block is promoted upward with additional promotion cost.

This models realistic hierarchical cache movement rather than free promotion.

Cache Replacement Policies
FIFO Replacement

When cache capacity is exceeded:

Remove oldest block and insert the new block.

Implemented using FIFO eviction.

Optimal Replacement (Bonus)

An Optimal replacement policy was implemented for comparison.

Strategy:

Remove the block whose next use is farthest in the future.

If a block will never be used again, it is removed immediately.

This provides a theoretical lower bound on memory access cost.

CPU Utilization Tracking

Each core tracks:

Busy cycles
Total cycles
Utilization percentage

Formula:

Utilization = (Busy Cycles / Total Cycles) × 100

Memory Statistics

The simulator records:

L1 Hits
L2 Hits
L3 Hits
RAM Accesses
Promotion Cycles
Total Memory Cycles

This provides insight into cache effectiveness.

Core Performance Comparison

The same workload is executed with:

1 Core
2 Cores

Metrics compared:

Total CPU Cycles
CPU Utilization
Speedup

Example:

1 Core:
Total Cycles = 9

2 Cores:
Total Cycles = 5

Speedup = 1.8x

FIFO vs Optimal Comparison

The simulator compares:

FIFO Cache Policy
Optimal Cache Policy

and reports:

Total Memory Cycles
Percentage Improvement

Example:

FIFO:
5840 cycles

Optimal:
5741 cycles

Reduction:
1.7%

Input Format

Example:

TASK T1 BURST 5 MEM M1 M4 M7

TASK T2 BURST 4 MEM M2 M5

TASK T3 BURST 6 MEM M3 M1 M8

Meaning:

Task ID = T1
Burst Time = 5
Memory Access Pattern = M1 M4 M7

Tasks arrive at intervals of one cycle.

Project Architecture
Task

Stores:

Task ID
Burst Time
Remaining Time
Memory Blocks
Arrival Time
Core

Stores:

Current Task
Quantum Usage
Busy Cycles
Scheduler

Responsible for:

Ready Queue Management
Round Robin Scheduling
CacheLevel

Responsible for:

Cache Storage
FIFO Eviction
Optimal Eviction
MemoryHierarchy

Responsible for:

L1/L2/L3 Lookup
RAM Access
Promotion Handling
Memory Statistics
Special Design Decisions
Hierarchical Promotion Cost

Cache promotions are not free.

Examples:

L3 → L2

and

L2 → L1

consume additional cycles.

Configurable Cache Capacity

L1 cache capacity can be reduced for demonstration purposes to clearly visualize cache replacement behavior.

Multi-Core Comparison Mode

The simulator can execute the same workload under different core counts to evaluate scheduling efficiency.

Optimal Policy Benchmarking

Optimal replacement is included as a benchmark to compare against FIFO and evaluate cache efficiency.

How to Run

Compile:

g++ -std=c++17 main.cpp -o sim.exe -mconsole

Run:

./sim.exe

Conclusion

This project combines Operating Systems and Computer Architecture concepts into a single simulator.

The implementation demonstrates:

Scheduling algorithms
Multi-core execution
Memory hierarchy simulation
Cache replacement strategies
Performance analysis

while providing measurable comparisons between different execution and caching approaches.
