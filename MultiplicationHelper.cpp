#pragma once
#include "MultiplicationHelper.h"
#define HAVE_STRUCT_TIMESPEC
#include "Matrix.h"
#include <omp.h>
#include <iostream>
#include <windows.h>

// get num of cores
const int maxNumThreads = omp_get_max_threads();

//no thread function
void *Matrix_Multiplication_noThread(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;

	for (int i = 0; i < args->m_matrix0->getRowCount(); ++i) {
		for (int j = 0; j < args->m_matrix1->getColCount(); ++j) {
			for (int k = 0; k < args->m_matrix0->getColCount(); ++k)
			{
				args->m_matrix_final->getValue()[i][j] += args->m_matrix0->getValue()[i][k] * args->m_matrix1->getValue()[k][j];
			}
		}
	}
	return arguments;
}

//first Loop thread function
void *Matrix_Multiplication_stage_0(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;

	// set num of threads to core's count and run threading in loop only
	omp_set_num_threads(maxNumThreads);
#pragma omp parallel for 
	for (int j = 0; j < args->m_matrix1->getColCount(); ++j) {
		for (int k = 0; k < args->m_matrix0->getColCount(); ++k)
		{
			args->m_matrix_final->getValue()[args->i][j] += args->m_matrix0->getValue()[args->i][k] * args->m_matrix1->getValue()[k][j];
		}
	}

	return arguments;
}


//Second Loop thread function
void *Matrix_Multiplication_stage_1(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;

	// set num of threads to core's count and run threading in loop only
	omp_set_num_threads(maxNumThreads);
#pragma omp parallel for 
	for (int k = 0; k < args->m_matrix0->getColCount(); ++k)
	{
		args->m_matrix_final->getValue()[args->i][args->j] += args->m_matrix0->getValue()[args->i][k] * args->m_matrix1->getValue()[k][args->j];
	}

	return arguments;
}


//last Loop threading function
void *Matrix_Multiplication_stage_2(void *arguments)
{
	struct threadArgs *args = (struct threadArgs *)arguments;

	// set num of threads to core's count and run single thread for this scope
	omp_set_num_threads(maxNumThreads);
#pragma omp single
	args->m_matrix_final->getValue()[args->i][args->j] += args->m_matrix0->getValue()[args->i][args->k] * args->m_matrix1->getValue()[args->k][args->j];

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
	{
		// run threaded function initialiser
		Matrix_Multiplication_noThread((void*)args.get());
	}
	// complete and add to timer
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
