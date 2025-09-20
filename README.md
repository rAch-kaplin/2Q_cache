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
cd build
ctest -V
```

To view available build targets:
```bash
cmake --build build --target help
```
