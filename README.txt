Matrix sum
-----------------
Author:
Christian Magnerfelt

Email:
magnerf@kth.se

Description:
calculates the sum as well as finds the max/min and their position in a square matrix.
Parallism is implemented by open mp parallel for.

Compiler:
This program requires at least GCC 4.4.7

Usage:
	./MatrixSumOMP {matrix size} {num of threads}
	
Build:
	make debug
	make release

