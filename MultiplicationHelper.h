#pragma once
#include <memory>
#include "GlobalTimer.h"
#include "Matrix.h"

/*

Class:	Helper
Description: Helper class for doing matric multiplication with threading types

*/

class Helper
{
public:
	Helper(std::shared_ptr<Matrix> m0, std::shared_ptr<Matrix> m1, std::shared_ptr<Matrix> mf);
	~Helper() {};
	/// no threads
	void multiplyMatrix_thread0();
	/// thread each first loop
	void multiplyMatrix_thread1();
	/// thread each second loop
	void multiplyMatrix_thread2();
	/// thread each last loop
	void multiplyMatrix_thread3();

private:

	std::shared_ptr<Matrix> m_matrix0;
	std::shared_ptr<Matrix> m_matrix1;
	std::shared_ptr<Matrix> m_matrix_final;

	GlobalTimer masterDelta;
};

/// struct for passing needed data into each thread.
struct threadArgs {
	std::shared_ptr<Matrix> m_matrix0;
	std::shared_ptr<Matrix> m_matrix1;
	std::shared_ptr<Matrix> m_matrix_final;
	unsigned long i;
	unsigned long j;
	unsigned long k;
};