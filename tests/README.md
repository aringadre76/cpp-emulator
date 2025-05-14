
# Tests

Each subdirectory holds the expected outputs for STDOUT (`"1.out"`) and STDERR (`"2.out"`) for a given STDIN (`"0.in"`). The provided bash script will verify the Banana emulator implementation for a corresponding .slug file against the specified test subdirectory.

Example usage:

```bash
./run_test.sh ./hello_world1 ../build/Banana hws/hello_world1.slug
./run_test.sh ./hello_world2 ../build/Banana hws/hello_world2.slug
./run_test.sh ./hello_world3 ../build/Banana hws/hello_world3.slug
```
