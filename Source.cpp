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
USHORT ROW_LENGTH = 50;
USHORT COL_LENGTH = 50;


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

	/// setting rows and colums for each matrix to mutiply
	/*matrix_00->setRowCount(ROW_LENGTH);
	matrix_00->setColCount(COL_LENGTH);
	matrix_01->setRowCount(ROW_LENGTH);
	matrix_01->setColCount(COL_LENGTH);

	matrix_10->setRowCount(ROW_LENGTH);
	matrix_10->setColCount(COL_LENGTH);
	matrix_11->setRowCount(ROW_LENGTH);
	matrix_11->setColCount(COL_LENGTH);

	matrix_20->setRowCount(ROW_LENGTH);
	matrix_20->setColCount(COL_LENGTH);
	matrix_21->setRowCount(ROW_LENGTH);
	matrix_21->setColCount(COL_LENGTH);


	matrix_30->setRowCount(ROW_LENGTH);
	matrix_30->setColCount(COL_LENGTH);
	matrix_31->setRowCount(ROW_LENGTH);
	matrix_31->setColCount(COL_LENGTH);

	/// populate matrix with autofill method
	matrix_00->autoFillMatrix(MIN_VALUE, MAX_VALUE);
	matrix_01->autoFillMatrix(MIN_VALUE, MAX_VALUE);

	matrix_10->autoFillMatrix(MIN_VALUE, MAX_VALUE);
	matrix_11->autoFillMatrix(MIN_VALUE, MAX_VALUE);

	matrix_20->autoFillMatrix(MIN_VALUE, MAX_VALUE);
	matrix_21->autoFillMatrix(MIN_VALUE, MAX_VALUE);

	matrix_30->autoFillMatrix(MIN_VALUE, MAX_VALUE);
	matrix_31->autoFillMatrix(MIN_VALUE, MAX_VALUE);

	/// populate final matrix0
	matrix_final0->setRowCount(matrix_00->getRowCount());
	matrix_final0->setColCount(matrix_01->getColCount());
	for (size_t i = 0; i < matrix_00->getRowCount(); i++)
	{
		matrix_final0->getValue().push_back(std::vector<USHORT>());
		auto count = matrix_01->getColCount();
		for (size_t k = 0; k < matrix_01->getColCount(); k++)
		{
			matrix_final0->getValue()[i].push_back(0);
		}
	}

	/// populate final matrix1
	matrix_final1->setRowCount(matrix_10->getRowCount());
	matrix_final1->setColCount(matrix_11->getColCount());
	for (size_t i = 0; i < matrix_10->getRowCount(); i++)
	{
		matrix_final1->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < matrix_11->getColCount(); k++)
		{
			matrix_final1->getValue()[i].push_back(0);
		}
	}

	/// populate final matrix2
	matrix_final2->setRowCount(matrix_20->getRowCount());
	matrix_final2->setColCount(matrix_21->getColCount());
	for (size_t i = 0; i < matrix_20->getRowCount(); i++)
	{
		matrix_final2->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < matrix_21->getColCount(); k++)
		{
			matrix_final2->getValue()[i].push_back(0);
		}
	}

	/// populate final matrix3
	matrix_final3->setRowCount(matrix_30->getRowCount());
	matrix_final3->setColCount(matrix_31->getColCount());
	for (size_t i = 0; i < matrix_30->getRowCount(); i++)
	{
		matrix_final3->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < matrix_31->getColCount(); k++)
		{
			matrix_final3->getValue()[i].push_back(0);
		}
	}*/

	/// Examples of running my threaded methods
	matrixConfig(*matrix_00, *matrix_01, *matrix_final0, 10, 10);
	mathHelper0.multiplyMatrix_thread0();
	matrixConfig(*matrix_10, *matrix_11, *matrix_final1, 10, 10);
	mathHelper1.multiplyMatrix_thread1();
	matrixConfig(*matrix_20, *matrix_21, *matrix_final2, 10, 10);
	mathHelper2.multiplyMatrix_thread2();
	/// setting matrix before running the matrix's test
	matrixConfig(*matrix_30, *matrix_31, *matrix_final3, 10,10);
	mathHelper3.multiplyMatrix_thread3();
	
	///UNCOMMENT TO: Visualize the matricies
	for (size_t i = 0; i < matrix_30->getRowCount(); i++)
	{
		matrix_final3->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < matrix_31->getColCount(); k++)
		{
			printf("%d ,", matrix_final3->getValue()[i][k]);
		}
		printf("\n");
	}

	/*printf("\n\n");
	for (size_t i = 0; i < matrix_20->getRowCount(); i++)
	{
		matrix_final2->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < matrix_21->getColCount(); k++)
		{
			printf("%d ,", matrix_final2->getValue()[i][k]);
		}
		printf("\n");
	}
*/
	printf("\n\n");
	for (size_t i = 0; i < matrix_10->getRowCount(); i++)
	{
		matrix_final1->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < matrix_11->getColCount(); k++)
		{
			printf("%d ,", matrix_final1->getValue()[i][k]);
		}
		printf("\n");
	}

	printf("\n\n");
	for (size_t i = 0; i < matrix_00->getRowCount(); i++)
	{
		matrix_final0->getValue().push_back(std::vector<USHORT>());
		for (size_t k = 0; k < matrix_00->getColCount(); k++)
		{
			printf("%d ,", matrix_final0->getValue()[i][k]);
		}
		printf("\n");
	}


	/// used to pause the application
	int pausePlease;
	std::cin >> pausePlease;

	return 0;
}
