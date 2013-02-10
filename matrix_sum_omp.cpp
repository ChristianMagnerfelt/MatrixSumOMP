/*! 
 *	\brief	Matrix summation using OpenMP
 *
 */
#include <omp.h>
#include <iostream>
#include <sstream>
#include <algorithm>

double start_time, end_time;

const std::size_t g_maxSize = 10000;	/* maximum matrix size */
const std::size_t g_maxThreads = 8;		/* maximum number of threads */

std::size_t g_numThreads;
std::size_t g_size;

/* !
 *	\brief	A matrix class implementing the RAII pinciple
 */
class Matrix{
	public:
		typedef int value_type;
		
		//! Disable copying
		Matrix(const Matrix &) = delete;
		Matrix & operator=(const Matrix &) = delete;
		
		Matrix (std::size_t size) : m_size(size)
		{ 
			m_data = new value_type[m_size * m_size];
		}
		~Matrix()
		{
			delete [] m_data;
		}
		value_type * operator [](std::size_t index){ return &(m_data[m_size * index]); }
		
		value_type * begin(){ return &(m_data[0]); }
		value_type * end()
		{ 
			if(m_size > 0) return &(m_data[m_size * m_size]);
			else return 0;
		}
		std::size_t size(){ return m_size; }
	private:
		std::size_t m_size;	
		value_type * m_data;
};

/* read command line, initialize, and create threads */
int main(int argc, const char *argv [])
{
	std::size_t i, j;
	Matrix::value_type total = 0;
	
	/* read command line args if any */
	g_size = (argc > 1)? atoi(argv[1]) : g_maxSize;
	g_numThreads = (argc > 2)? atoi(argv[2]) : g_maxThreads;
	if (g_size > g_maxSize) g_size = g_maxSize;
	if (g_numThreads > g_maxThreads) g_numThreads = g_maxThreads;

	/* Allocate matrix */
	Matrix matrix(g_size);
	
	/* Set the number of threads available to open mp */
	omp_set_num_threads(g_numThreads);
	
	/* initialize the matrix */
	for (i = 0; i < matrix.size(); i++) {
		for (j = 0; j < matrix.size(); j++) {
			matrix[i][j] = rand() % 99;
		}
	}
  
	start_time = omp_get_wtime();
	

	#pragma omp parallel for reduction (+:total) private(j)	
	for (i = 0; i < matrix.size(); i++)
	{
		for (j = 0; j < matrix.size(); j++){
			total += matrix[i][j];
		}
	}
    // Implicit barrier
    
    //total = std::accumulate(matrix.begin(), matrix.end(), Matrix::value_type());
	
	end_time = omp_get_wtime();
		
	std::cout << "The total is " << total << std::endl;
	std::cout << "It took " << end_time - start_time << " seconds" << std::endl;
	
	return 0;
}
