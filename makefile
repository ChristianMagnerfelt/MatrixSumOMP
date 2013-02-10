debug: matrix_sum_omp.cpp
	g++ -Wall -g -O0 -o MatrixSumOMP matrix_sum_omp.cpp -fopenmp -std=c++0x
	
release: matrix_sum_omp.cpp
	g++ -Wall -O3 -o MatrixSumOMP matrix_sum_omp.cpp -fopenmp -std=c++0x
	
clean:
	rm MatrixSumOMP
