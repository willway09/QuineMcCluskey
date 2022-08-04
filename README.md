## Quine McCluskey Algorithm
This is an algorithm covered in introductory digital logic courses for optimizing combinational logic.
It seemed fun to implement.

## Compilation
Just run `make` with GCC installed.
It just uses STL functionality, so no external libraries are necessary

## Usage
From the command line, just run

`./quine ma mb mc ... mz`

where each `m*` is the index of a minterm in the unoptimized expression (it is a positive integer, such as 1 or 13).

Appending `d` to a `m*` term (for example, `1d` or `5d`) flags it as a don't care term, which the algorithm will use for optimization.

Additoinally, flags can be used to modify output.
Currently there are just 2.
`-out`, followed by either `logic`, `C`, `VHDL`,  or `verilog` will change the solution output format to one corresponding to Boolean Algebra, C boolean operations, VHDL combinational logic, or Verilog combinational logic respectively.
Additionally, using the `-imptables` flag will enable output of the implicant lists in each iteration of the first phase of the algorithm.
This can be useful for learning purposes, or validating the algorithm.

## Examples
`./quine 1 5 7 9 13`  
Run the Quine McCluskey algorithm with 1, 5, 7, 9, and 13 ad minterms.

`./quine 8 9 2d 7 3d 5 -imptables`  
Run the Quine McCluskey algorithm with 5, 7, 8, and 9 as minterms, and 2 and 3 as don't cares. Output intermediate implicant list tables.

`./quine 5 1 7 8 3 5d -out C`  
Run the Quine McCluskey algorithm with 1, 3, 5, 7 and 8 as minterms, and 5 as don't care. Output in C boolean operations format.

`./quine 6 4 7 9 0 1d 13d -out VHDL -imptables`  
Run the Quine McCluskey algorithm with 0, 4, 6, 7, and 8 as minterms, and 1 and 13 as don't cares. Output in VHDL combinational logic format. Output intermediate implicant list tables.

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
