#include <vector>
#include <map>
/// user classes
#include "MultiplicationHelper.h"
#include "Matrix.h"
#include <omp.h>

// used to make automated testing procedues easier.
/// autofill max and min values
USHORT MAX_VALUE = 1;
USHORT MIN_VALUE = 1;
/// row and colume fill globals
USHORT ROW_LENGTH = 10;
USHORT COL_LENGTH = 10;


///Global matrix objects
std::shared_ptr<Matrix> matrix_final0 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_final1 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_final2 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_final3 = std::make_shared<Matrix>();

std::shared_ptr<Matrix> matrix_00 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_01 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_10 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_11 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_20 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_21 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_30 = std::make_shared<Matrix>();
std::shared_ptr<Matrix> matrix_31 = std::make_shared<Matrix>();

/// setting up matrix config helper function
void matrixConfig(Matrix & mo, Matrix & m1, Matrix & mf, USHORT row, USHORT col) {
	mo.setRowCount(row);
	mo.setColCount(col);
	mo.autoFillMatrix(MIN_VALUE ,MAX_VALUE);
	m1.setRowCount(row);
	m1.setColCount(col);
	m1.autoFillMatrix(MIN_VALUE, MAX_VALUE);
	
	mf.setRowCount(mo.getRowCount());
	mf.setColCount(m1.getColCount());
	for (size_t i = 0; i < mo.getRowCount(); i++)
	{
		mf.getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < m1.getColCount(); k++)
		{
			mf.getValue()[i].push_back(0);
		}
	}
}

int main() 
{
	/// giving each matrix set its own helper
	Helper mathHelper0(matrix_00, matrix_01, matrix_final0);
	Helper mathHelper1(matrix_10, matrix_11, matrix_final1);
	Helper mathHelper2(matrix_20, matrix_21, matrix_final2);
	Helper mathHelper3(matrix_30, matrix_31, matrix_final3);

	/// Examples of running my threaded methods
	// no threading
	matrixConfig(*matrix_00, *matrix_01, *matrix_final0, ROW_LENGTH, COL_LENGTH);
	mathHelper0.multiplyMatrix_thread0();
	// First loop
	//matrixConfig(*matrix_10, *matrix_11, *matrix_final1, ROW_LENGTH, COL_LENGTH);
	//mathHelper1.multiplyMatrix_thread1();
	// Second loop
	//matrixConfig(*matrix_20, *matrix_21, *matrix_final2, ROW_LENGTH, COL_LENGTH);
	//mathHelper2.multiplyMatrix_thread2();
	//last loop - not used as all single threads and cant manage thread count on singles
	//matrixConfig(*matrix_30, *matrix_31, *matrix_final3, ROW_LENGTH,COL_LENGTH);
	//mathHelper3.multiplyMatrix_thread3();
	

	/// used to pause the application
	int pausePlease;
	std::cin >> pausePlease;

	return 0;
}
