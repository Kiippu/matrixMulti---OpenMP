#pragma once
#include "MultiplicationHelper.h"
#define HAVE_STRUCT_TIMESPEC
#include "Matrix.h"
#include <omp.h>
#include <iostream>
#include <windows.h>

// get num of cores
const int maxNumThreads = 8;//omp_get_max_threads();

//no thread function
void *Matrix_Multiplication_noThread(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;
	const USHORT& count0 = args->m_matrix0->getColCount();
	const USHORT& count1 = args->m_matrix1->getColCount();
	const USHORT& count2 = args->m_matrix0->getColCount();
	auto& matrix_final = args->m_matrix_final->getValue();
	auto& matrix_0 = args->m_matrix0->getValue();
	auto& matrix_1 = args->m_matrix1->getValue();

	for (int i = 0; i < count0; ++i) {
		for (int j = 0; j < count1; ++j) {
			for (int k = 0; k < count2; ++k)
			{
				matrix_final[i][j] += matrix_0[i][k] * matrix_1[k][j];
			}
		}
	}
	return arguments;
}

//first Loop thread function
void *Matrix_Multiplication_stage_0(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;
	const USHORT& count0 = args->m_matrix0->getColCount();
	const USHORT& count1 = args->m_matrix1->getColCount();
	auto& matrix_final = args->m_matrix_final->getValue();
	auto& matrix_0 = args->m_matrix0->getValue();
	auto& matrix_1 = args->m_matrix1->getValue();

	// set num of threads to core's count and run threading in loop only
	omp_set_dynamic(0);
	omp_set_num_threads(maxNumThreads);
#pragma omp parallel for 
	for (int j = 0; j < count1; ++j) {
		for (int k = 0; k < count0; ++k)
		{
			matrix_final[args->i][j] += matrix_0[args->i][k] * matrix_1[k][j];
		}
	}

	return arguments;
}


//Second Loop thread function
void *Matrix_Multiplication_stage_1(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;
	const USHORT& count = args->m_matrix0->getColCount();
	auto& matrix_final = args->m_matrix_final->getValue();
	auto& matrix_0 = args->m_matrix0->getValue();
	auto& matrix_1 = args->m_matrix1->getValue();
	// set num of threads to core's count and run threading in loop only
	//omp_set_dynamic(0);
	omp_set_num_threads(maxNumThreads);
#pragma omp parallel for
	for (int k = 0; k < count; ++k)
	{
		//printf("1: Thread# %d: k = %d\n", omp_get_thread_num(), k);
		matrix_final[args->i][args->j] += matrix_0[args->i][k] * matrix_1[k][args->j];
	}

	return arguments;
}


//last Loop threading function
void *Matrix_Multiplication_stage_2(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;
	unsigned count = args->m_matrix0->getColCount();
	auto& matrix_final = args->m_matrix_final->getValue();
	auto& matrix_0 = args->m_matrix0->getValue();
	auto& matrix_1 = args->m_matrix1->getValue();
	// set num of threads to core's count and run single thread for this scope
	omp_set_dynamic(0);
	omp_set_num_threads(maxNumThreads);
#pragma omp single
	{
		matrix_final[args->i][args->j] += matrix_0[args->i][args->k] * matrix_1[args->k][args->j];
	}
	return arguments;
}

Helper::Helper(std::shared_ptr<Matrix> m0, std::shared_ptr<Matrix> m1, std::shared_ptr<Matrix> mf)
{
	m_matrix0 = m0;
	m_matrix1 = m1;
	m_matrix_final = mf;
}


void Helper::multiplyMatrix_thread0()
{
	
	printf("<<Starting no thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "no thread: ");
	// set up threads avalible variables
	std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
	args->m_matrix0 = m_matrix0;
	args->m_matrix1 = m_matrix1;
	args->m_matrix_final = m_matrix_final;

	Matrix_Multiplication_noThread((void*)args.get());

	masterDelta.addFinishTime("no thread: ");
	masterDelta.printFinalTimeSheet();
}

void Helper::multiplyMatrix_thread1()
{
	std::vector<std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting first loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in first loop");
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i)
	{
		// set up threads avalible variables
		std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
		args->i = i;
		args->m_matrix0 = m_matrix0;
		args->m_matrix1 = m_matrix1;
		args->m_matrix_final = m_matrix_final;
		prtList.push_back(args);
		{
			// run threaded function initialiser
			{
				Matrix_Multiplication_stage_0((void*)args.get());
			}
		}
	}
	// complete and add to timer
	printf("Starting first loop threading complete \n\n");
	masterDelta.addFinishTime("thread in first loop");
	masterDelta.printFinalTimeSheet();

}

void Helper::multiplyMatrix_thread2()
{
	std::vector<std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting second loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in second loop");
	const int num_procs = omp_get_num_procs();
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i)
	{
		for (USHORT j = 0; j < m_matrix1->getColCount(); ++j) {
			// set up threads avalible variables
			std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
			args->j = i;
			args->i = j;
			args->m_matrix0 = m_matrix0;
			args->m_matrix1 = m_matrix1;
			args->m_matrix_final = m_matrix_final;
			prtList.push_back(args);
			{
				{
					Matrix_Multiplication_stage_1((void*)args.get());
				}
			}
		}
	}
		// complete and add to timer
	printf("Starting second loop threading complete...\n\n");
	masterDelta.addFinishTime("thread in second loop");
	masterDelta.printFinalTimeSheet();

}

void Helper::multiplyMatrix_thread3()
{
	std::vector<std::shared_ptr<threadArgs>> prtList;
	printf("<<Starting last loop thread mutliplication>>\n");
	masterDelta.addStartTime(TIME_MEASURMENT::Milli, "thread in last loop ");
	for (USHORT i = 0; i < m_matrix0->getRowCount(); ++i) {
		for (USHORT j = 0; j < m_matrix1->getColCount(); ++j) {
			for (USHORT k = 0; k < m_matrix0->getColCount(); ++k)
			{
				// set up threads avalible variables
				std::shared_ptr<threadArgs> args = std::make_shared<threadArgs>();
				args->j = i;
				args->i = j;
				args->k = k;
				args->m_matrix0 = m_matrix0;
				args->m_matrix1 = m_matrix1;
				args->m_matrix_final = m_matrix_final;
				prtList.push_back(args);
				{
					Matrix_Multiplication_stage_2((void*)args.get());
				}
			}
		}
	}

	// compleete and add to timer
	printf("last loop threading complete...\n\n");
	masterDelta.addFinishTime("thread in last loop ");
	masterDelta.printFinalTimeSheet();

}
