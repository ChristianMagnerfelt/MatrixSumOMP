/*! 
 *	\brief	Matrix summation using OpenMP
 *
 */
#include <omp.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>

//! global variables
const std::size_t g_maxSize = 10000;	//!< Maximum matrix size
const std::size_t g_maxThreads = 128;	//!< Maximum number of threads
std::size_t g_numThreads;
std::size_t g_size;

/* !
 *	\brief	A matrix class implementing the RAII pinciple
 */
class Matrix
{
	public:
		typedef int value_type;
		
		struct Position
		{
			std::size_t x;
			std::size_t y;
		};
		
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

/*!
 * 	\brief	read command line, initialize, and create threads. 
 *			Calculates matrix sum and finds min and max value. 
 */
int main(int argc, const char *argv [])
{
	double start_time, end_time;
	
	
	// Read command line args if any
	g_size = (argc > 1)? atoi(argv[1]) : g_maxSize;
	g_numThreads = (argc > 2)? atoi(argv[2]) : g_maxThreads;
	if (g_size > g_maxSize) g_size = g_maxSize;
	if (g_numThreads > g_maxThreads) g_numThreads = g_maxThreads;

	// Allocate matrix
	Matrix matrix(g_size);
	
	// Set the number of threads available to open mp
	omp_set_num_threads(g_numThreads);
	
	// Initialize the matrix
	for (std::size_t i = 0; i < matrix.size(); ++i) {
		for (std::size_t j = 0; j < matrix.size(); ++j) {
			matrix[i][j] = rand() % 99;
		}
	}
  
	start_time = omp_get_wtime();
	
	Matrix::value_type sum = 0;
	// Calculate matrix sum
	#pragma omp parallel for reduction (+:sum)
	for (std::size_t i = 0; i < matrix.size(); ++i)
	{
		for (std::size_t j = 0; j < matrix.size(); ++j){
			sum += matrix[i][j];
		}
	}
	// Implicit barrier
    
    //sum = std::accumulate(matrix.begin(), matrix.end(), Matrix::value_type());
    

    // Find max value and its position
    Matrix::value_type max = std::numeric_limits<Matrix::value_type>::min();
    Matrix::Position maxPos;
    #pragma omp parallel for
	for (std::size_t i = 0; i < matrix.size(); ++i)
	{
		Matrix::value_type currentMax = std::numeric_limits<Matrix::value_type>::min();
		Matrix::Position currentPos;
		for (std::size_t j = 0; j < matrix.size(); ++j){
			if(matrix[i][j] > currentMax)
			{
				currentMax = matrix[i][j];
				currentPos.x = j;
				currentPos.y = i;
			}
		}
		#pragma omp critical
		{
			// Check and update global max if current max is greater
			if(currentMax > max)
			{
				max = currentMax;
				maxPos = currentPos;
			}
		}
	}
    // Implicit barrier
    
    // Find max value and its position
    Matrix::value_type min = std::numeric_limits<Matrix::value_type>::max();
    Matrix::Position minPos;
    #pragma omp parallel for
	for (std::size_t i = 0; i < matrix.size(); ++i)
	{
		Matrix::value_type currentMin = std::numeric_limits<Matrix::value_type>::max();
		Matrix::Position currentPos;
		for (std::size_t j = 0; j < matrix.size(); ++j){
			if(matrix[i][j] < currentMin)
			{
				currentMin = matrix[i][j];
				currentPos.x = j;
				currentPos.y = i;
			}
		}
		#pragma omp critical
		{
			// Check and update global min if current min is less
			if(currentMin < min)
			{
				min = currentMin;
				minPos = currentPos;
			}
		}
	}
	// Implicit barrier
	
	end_time = omp_get_wtime();
		
	std::cout << "The total sum is " << sum << std::endl;
	std::cout << "Max value is " << max << " at [" << maxPos.x << "," << maxPos.y << "]" << std::endl;
	std::cout << "Min value is " << min << " at [" << minPos.x << "," << minPos.y << "]" << std::endl;
	std::cout << "It took " << end_time - start_time << " seconds" << std::endl;
	
	return 0;
}
