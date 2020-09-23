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

- `mb` maximal memory usage, 1024 by default.

### Examples

```bash
$ ./stress_regex 50000 6000
avaiable memory: 5.859 GB
expression: (a | b | cb)*
string: 'aaa....' (50.00 M symbols)
time: 2.06 s
memory used: 3.912 GB

$ ./stress_regex 5000
avaiable memory: 1.000 GB
expression: (a | b | cb)*
string: 'aaa....' (5.00 M symbols)
time: 0.23 s
memory used: 0.391 GB

$ ./stress_regex
avaiable memory: 1.000 GB
expression: (a | b | cb)*
string: 'aaa....' (10.00 M symbols)
time: 0.44 s
memory used: 0.782 GB
```
