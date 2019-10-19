# DOUBLESTAR
intermediate floating point precision by iterating hardware-supported DOUBLEs

iRRAM's first attempt/iteration uses hardware-supported fast DOUBLE,
then switches to software-based GMP.
This incurs Strassen-Schönhage's asymptotically optimal cost O(n log n loglog n)
although with large constant, making iRRAM typically 20-times slower than DOUBLE.

This project tries to soften this transition by using COMBINATIONS of DOUBLE for intermediate precision:
see https://en.wikipedia.org/wiki/Quadruple-precision_floating-point_format#Double-double_arithmetic
