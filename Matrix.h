#pragma once
#include <vector>
#include <Windows.h>

/*

Class:	Matrix
Description: Matrix class to house all matirx specific methods and handle matrix values

*/

class Matrix
{
public:
	Matrix() {};
	~Matrix() {};
	/// Accessor to matrix
	std::vector< std::vector<USHORT>> & getValue();
	/// Add value into matrix
	bool addValue(USHORT row, USHORT col, USHORT val);

	/// accessor and mutator for row counts
	bool setRowCount(USHORT k);
	const USHORT & getRowCount();
	/// accessor and mutator for col counts
	bool setColCount(USHORT k);
	const USHORT & getColCount();
	/// auto fill matrix with random min to max values
	bool autoFillMatrix(USHORT min, USHORT max);
	void clear();

private:
	std::vector< std::vector<USHORT>> m_matrix;
	USHORT m_rowCount;
	USHORT m_colCount;
};
