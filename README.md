## Quine McCluskey Algorithm
This is an algorithm covered in introductory digital logic courses for optimizing combinational logic.
It seemed fun to implement.

## Compilation
Just run `make` with GCC installed.
It just uses STL functionality, so no external libraries are necessary

## Usage
From the command line, just run

`./quine ma mb mc ... mz`

where each `m` is the index of a minterm in the unoptimized expression.

## Output
The program outputs a list of potential optizations (as there can be more than one).
This can be selected from for a desired minterm combination.

## Bugs
This has not yet been rigorously tested, though it seems to work on a few representative sample problems.
It is known to fail when no minterms are entered.
It also gives a blank output when every value from 0...2^n-1 is listed.
This kind of makes sense, because the output is just 1 (not an expression), but this should be improved.

## Improvement
1. Clean up output
2. Increase efficiency of data structure usage and algorithms (in particular, use `std::unordered_set` where possible)
3. Replace Petrick's Method with row/column dominance
4. Add functionality to take Boolean expression as input, and then optimize
5. More robust input validation
6. Fix data types types in code
7. Don't cares
