# Rubik’s Cube Solver (C++)

![C++](https://img.shields.io/badge/C%2B%2B-17-blue) ![CMake](https://img.shields.io/badge/CMake-3.10%2B-blue) ![Algorithms](https://img.shields.io/badge/Algorithms-implemented-orange) ![Pathfinding](https://img.shields.io/badge/Pathfinding-IDA%2A-%238E44AD) ![Data Structures](https://img.shields.io/badge/Data%20Structures-Graphs-lightgrey) ![Hashing](https://img.shields.io/badge/Hashing-Custom-green)

## 🚀 Project Overview

This project is a high-performance Rubik’s Cube solver implemented in modern C++. It demonstrates the use of advanced search algorithms, optimized heuristics, and clean object-oriented design principles.

The solver includes an interactive terminal interface that allows users to scramble the cube, apply individual moves, and invoke multiple solving strategies, including BFS, IDDFS, and the highly efficient IDA* (Iterative Deepening A*) algorithm.

## ⚙️ Key Features

### Modular Architecture

Built around an abstract base class (RubiksCube) that supports multiple cube state representations (1D array, 3D array, Bitboard, etc.).

### Interactive Terminal UI

A menu-driven Terminal User Interface (TUI) enables intuitive interaction and experimentation.

### Cube Manipulation Tools

Random Shuffle: Scrambles the cube with a capped limit of 10 moves for guaranteed quick solvability.

Manual Moves: Supports standard cube notation (e.g., U, L', F2).

Instant Reset: Restores the cube to the solved state on demand.

### Pathfinding Algorithms

Breadth-First Search (BFS): Guarantees the shortest path to the solution.

Iterative Deepening Depth-First Search (IDDFS): Memory-efficient optimal solver.

IDA* (Iterative Deepening A*): Uses a precomputed pattern database (corner heuristics) to achieve extremely fast solve times—often within milliseconds.

### Performance Reporting

All solvers provide detailed statistics, including solution length and execution time.

## 💡 Algorithms Implemented

The Rubik’s Cube has $43$ quintillion possible states. This project employs both classical and heuristic-driven algorithms to efficiently navigate that search space.

### 1. Breadth-First Search (BFS)

Approach: Explores cube states level by level, ensuring the first solution found is the optimal one.

Data Structures: std::queue for frontier expansion and std::unordered_map for visited states.

Best For: Short scrambles ($< 7$ moves).

### 2. Iterative Deepening Depth-First Search (IDDFS)

Approach: Combines the optimality of BFS with the low memory usage of DFS by gradually increasing depth limits.

Complexity: Space complexity of $O(d)$, significantly more memory-efficient than BFS.

Best For: Medium scrambles ($< 10$ moves).

### 3. IDA* (Iterative Deepening A*) with Pattern Database

Approach: Utilizes heuristic estimates from a precomputed Corner Pattern Database to prune large portions of the search tree.

Data Resources: Loads cornerDepth5V1.txt, mapping corner configurations to their minimum required solving depth.

Performance: Solves deep scrambles ($10–20$ moves) in under $0.1$ seconds.

## 🧩 Cube Representation

The primary state representation uses a 1D array (RubiksCube1dArray). This representation allows efficient hashing and comparison—critical for fast state lookups in algorithms like BFS and IDA*.

Hashing: Custom hash functions (Hash1d) ensure rapid and collision-resistant state indexing.

## 📦 Build & Run Instructions

This project uses CMake as its build system.

### Prerequisites

A C++17-compatible compiler (GCC, Clang, or MSVC)

CMake 3.10+

### Build Steps (Linux/macOS)

#### Clone the Repository