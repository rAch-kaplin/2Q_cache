<div align="center">

  # Implementation of Cache2Q and Ideal Caching Algorithms in C++

  ![C++](https://img.shields.io/badge/C++-23%2B-blue.svg)

</div>

---

## About

This project implements two advanced caching algorithms:

- **Cache2Q** — An improved version of LRU that works more effectively with a variety of data access patterns.
- **IdealCache (Belady’s Algorithm)** — Theoretically optimal method that selects an element that will not be needed soon for deletion.

---
## Comparative table of the results of the work of Ideal cache and 2Qcache

|№| Data format `cache size` `Number of elements` `Elements` | 2Q_cache `HITS` | Ideal_cache `HITS` | 
|--|--------------------------------------|----------------------|------------|
|5| 15 40 2 5 8 2 5 8 11 14 2 5 8 11 14 17 20 2 5 8 11 14 17 20 23 26 2 5 8 11 14 17 20 23 26 29 32 2 5 8 11 14 | 18 | 29 |
|2| 10 30 5 12 5 18 12 5 3 18 12 5 3 18 12 5 3 18 12 5 3 18 12 5 3 18 12 5 3 18 5 3 | 24 | 26 |
|6| 10 32 4 7 10 4 7 10 13 16 4 7 10 13 16 19 22 4 7 10 13 16 19 22 25 28 4 7 10 13 16 19 4 7 | 10 | 23 |
|4| 10 28 3 4 3 5 4 6 5 7 6 8 7 9 8 10 9 11 10 12 11 13 12 14 13 15 14 16 13 17 | 12 | 13 |

---
## Quick Start:

Clone the repository and build the project:
```bash
git clone git@github.com:rAch-kaplin/2Q_cache.git
cd 2Q_cache
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Run Cache:
```bash
./build/cache
```

Run Tests:
```bash
ctest --test-dir build -V
```

To view available build targets:
```bash
cmake --build build --target help
```
