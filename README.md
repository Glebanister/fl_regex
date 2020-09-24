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
./stress_regex [th_sym] [mb]
```

Where

- `th_sym` * 1000 is stress string size, 10'000 by default.

- `mb` maximal memory usage, 2048 by default.

### Examples

```bash
./build/stress_regex 100000 8000
avaiable memory: 7.812 GB

expression: (a | a)*
string: 'aaa....' (100.00 M symbols)
time: 2.69 s
memory used: 5.867 GB

expression: a*
string: 'aaa....' (100.00 M symbols)
time: 0.66 s
memory used: 3.912 GB
```
