# Regular expression matcher

Formal languages HW №3.

## Installation

```bash
git clone https://github.com/Glebanister/fl_regex --recursive
cd fl_regex
mkdir build && cd build
cmake ..
make
```

## Testing

```bash
cd build
./test_regex
```

## Stress testing

```bash
cd build
./stress_regex N
```

Where `N` * 1000 is stress string size, 50'000 by default.
