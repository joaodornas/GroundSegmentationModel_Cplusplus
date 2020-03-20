#pragma once

#pragma warning (disable: 4018)
#pragma warning (disable: 4838)
#pragma warning (disable: 4996)

#include "Globals.h"
#include "Eval.h"
#include "Segment.h"

//COMPUTES THE EVALUATION MATRICES COMPUTATION USING OPENCL
void Eval(struct Sectors_struct & ocl_Test, struct Sectors_struct & ocl_Sp, bool & inliers, struct DeviceAndHostMemoryX<int> & inliers_idx, struct DeviceAndHostMemoryX<int> & ocl_PG_Ground, Eigen::MatrixXf & Model_Hg)
{

		cl_int err = 0;

		// -----------------------------------------------------------------------

		RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Eval_PART1_STEPS1234);

		// -----------------------------------------------------------------------

		// -----------------------------------------------------------------------
		// CREATE BUFFERS
		// -----------------------------------------------------------------------

		_DATA.nModel_t = ocl_Sp.ocl_Z.dim_of_data;
		_DATA.nTest_t = ocl_Test.ocl_Z.dim_of_data;
		_DATA.nOP_t = _VELODYNEOPTIONS.N_size;

		_DATA.nModel_i = static_cast<int>(_DATA.nModel_t);
		_DATA.nTest_i = static_cast<int>(_DATA.nTest_t);
		_DATA.nOP_i = static_cast<int>(_DATA.nOP_t);

		struct EVAL_struct _EVAL;

		_EVAL.allocData(_DATA.nTest_t, _DATA.nModel_t, _DATA.nOP_t);

		getGWWIandADDPAD(_DATA.nModel_t, _DATA.nModel_t, _DATA._one_t);

		for (size_t i = 0; i < _DATA.nModel_t; i++)
		{
			for (size_t j = 0; j < _DATA.nModel_t; j++)
			{
				if (i == j)
				{
					_EVAL.ocl_matrix_Inverse_m.host[i + j * _DATA.nModel_t] = _DATA._one_f;
				}
			}
		}

		struct Sectors_struct ocl_OP;

		ocl_OP.allocData(_VELODYNEOPTIONS.N_size, _DATA._one_t, _DATA._one_t);

		for (int i = 0; i < _VELODYNEOPTIONS.N; i++)
		{
			float bin_val = _VELODYNEOPTIONS.Rstep * (static_cast<float>(i) + _DATA._one_f);

			ocl_OP.ocl_OP.host[i] = bin_val;
		}

		_EVAL.bufferData();

		//////////////////////////////////////////////////////////////////////////
		//PART 1: BUILD COVARIANCES MATRICES /////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		// -----------------------------------------------------------------------
		// DEFINITION OF STEPS OF COMPUTATION 
		// -----------------------------------------------------------------------

		//PART 1: BUILD COVARIANCES MATRICES
		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel

		// -----------------------------------------------------------------------
		// SETTING KERNELS ARGUMENTS
		// -----------------------------------------------------------------------

		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 0, sizeof(cl_mem), &ocl_Test.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 1, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 2, sizeof(cl_mem), &ocl_Sp.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 3, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_K_star_X_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 5, sizeof(float), &_MODELOPTIONS.sigma_f);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 6, sizeof(float), &_MODELOPTIONS.length_scale);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// RUNNING KERNELS
		// -----------------------------------------------------------------------

		getGWWIandADDPAD(_DATA.nTest_t, _DATA.nModel_t, _DATA._one_t);

		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0,0,0
		);
		SAMPLE_CHECK_ERRORS(err);

		/*
		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);*/

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 0, sizeof(cl_mem), &ocl_Sp.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 1, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 2, sizeof(cl_mem), &ocl_Test.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 3, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_K_X_star_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 5, sizeof(float), &_MODELOPTIONS.sigma_f);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 6, sizeof(float), &_MODELOPTIONS.length_scale);
		SAMPLE_CHECK_ERRORS(err);

		getGWWIandADDPAD(_DATA.nModel_t, _DATA.nTest_t, _DATA._one_t);

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0,0,0
		);
		SAMPLE_CHECK_ERRORS(err);

		/*
		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);*/

		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 0, sizeof(cl_mem), &ocl_Test.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 1, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 2, sizeof(cl_mem), &ocl_Test.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 3, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_K_star_star_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 5, sizeof(float), &_MODELOPTIONS.sigma_f);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 6, sizeof(float), &_MODELOPTIONS.length_scale);
		SAMPLE_CHECK_ERRORS(err);

		getGWWIandADDPAD(_DATA.nTest_t, _DATA.nTest_t, _DATA._one_t);

		// STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0,0,0
		);
		SAMPLE_CHECK_ERRORS(err);

		/*
		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);*/

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise, 0, sizeof(cl_mem), &ocl_Sp.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise, 1, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise, 2, sizeof(cl_mem), &ocl_Sp.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise, 3, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_K_X_X_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise, 5, sizeof(float), &_MODELOPTIONS.sigma_f);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise, 6, sizeof(float), &_MODELOPTIONS.sigma_n);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise, 7, sizeof(float), &_MODELOPTIONS.length_scale);
		SAMPLE_CHECK_ERRORS(err);

		getGWWIandADDPAD(_DATA.nModel_t, _DATA.nModel_t, _DATA._one_t);

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernelNoise,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0,0,0
		);
		SAMPLE_CHECK_ERRORS(err);

		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		//------------------------------------------------------------------------

		RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Eval_PART1_STEPS1234);

		//------------------------------------------------------------------------

		//////////////////////////////////////////////////////////////////////////
		//PART 2: Inverse of K_X_X ///////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		//PART 2: BUILD INVERSE OF K_X_X_m
		//STEP 5:
		//inverse_m = K_X_X_m; nModelXnModel
		//STEP 6:
		//Inverse_m = inverse.inverse(); nModelXnModel

		RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Eval_PART2_STEPS56);

		// -----------------------------------------------------------------------
		// SETTING KERNELS ARGUMENTS
		// -----------------------------------------------------------------------

		//STEP 5:
		//inverse_tmp = K_X_X_m; nModelXnModel
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixCopy, 0, sizeof(cl_mem), &_EVAL.ocl_matrix_K_X_X_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixCopy, 1, sizeof(cl_mem), &_EVAL.ocl_matrix_Inverse_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixCopy, 2, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);

		getGWWIandADDPAD(_DATA.nModel_t, _DATA.nModel_t, _DATA._one_t);

		//STEP 5:
		//inverse = K_X_X_m; nModelXnModel
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.Eval_matrixCopy,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0, 0, 0
		);
		SAMPLE_CHECK_ERRORS(err);
		
		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		//STEP 6: Inverse Matrix with SPOTRF/SPOTRI
		getMatrixInverse(_EVAL.ocl_matrix_Inverse_m);


		//------------------------------------------------------------------------

		RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Eval_PART2_STEPS56);

		//------------------------------------------------------------------------

		//PART 3: f_step1
		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel

		//////////////////////////////////////////////////////////////////////////
	    //PART 3: f_step1/////////////////////////////////////////////////////////
	    //////////////////////////////////////////////////////////////////////////

		RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Eval_PART3_STEP7);

		// -----------------------------------------------------------------------
		// SETTING KERNELS ARGUMENTS
		// -----------------------------------------------------------------------

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 0, sizeof(cl_mem), &_EVAL.ocl_matrix_K_star_X_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 1, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 2, sizeof(cl_mem), &_EVAL.ocl_matrix_Inverse_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 3, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_f_step1_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 5, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// RUNNING KERNELS
		// -----------------------------------------------------------------------

		getGWWIandADDPAD(_DATA.nTest_t, _DATA.nModel_t, _DATA._one_t);

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0,0,0
		);
		SAMPLE_CHECK_ERRORS(err);

		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		//------------------------------------------------------------------------

		RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Eval_PART3_STEP7);

		//------------------------------------------------------------------------

		//PART 4: f_step2
		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel

		//////////////////////////////////////////////////////////////////////////
	    //PART 4: f_step2/////////////////////////////////////////////////////////
	    //////////////////////////////////////////////////////////////////////////

		RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Eval_PART4_STEP8);

		// -----------------------------------------------------------------------
		// SETTING KERNELS ARGUMENTS
		// -----------------------------------------------------------------------
		
		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 0, sizeof(cl_mem), &_EVAL.ocl_matrix_f_step1_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 1, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 2, sizeof(cl_mem), &ocl_Sp.ocl_Z.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 3, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_f_step2_m.device);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// RUNNING KERNELS
		// -----------------------------------------------------------------------

		getGWWIandADDPAD(_DATA.nTest_t, _DATA.nModel_t, _DATA._one_t);

		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.Eval_matrixVector,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0,0,0
		);
		SAMPLE_CHECK_ERRORS(err);

		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		//------------------------------------------------------------------------

		RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Eval_PART4_STEP8);

		//------------------------------------------------------------------------

		//PART 5: Vf_step1
		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest

		//////////////////////////////////////////////////////////////////////////
	    //PART 5: Vf_step1////////////////////////////////////////////////////////
	    //////////////////////////////////////////////////////////////////////////

		RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Eval_PART5_STEP9);

		// -----------------------------------------------------------------------
		// SETTING KERNELS ARGUMENTS
		// -----------------------------------------------------------------------

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 0, sizeof(cl_mem), &_EVAL.ocl_matrix_f_step1_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 1, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 2, sizeof(cl_mem), &_EVAL.ocl_matrix_K_X_star_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 3, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_Vf_step1_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 5, sizeof(int), &_DATA.nModel_i);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// RUNNING KERNELS
		// -----------------------------------------------------------------------

		getGWWIandADDPAD(_DATA.nTest_t, _DATA.nTest_t, _DATA._one_t);

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0,0,0
		);
		SAMPLE_CHECK_ERRORS(err);

		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		//------------------------------------------------------------------------

		RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Eval_PART5_STEP9);

		//------------------------------------------------------------------------

		//PART 6: Vf_step2
		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest

		//////////////////////////////////////////////////////////////////////////
	    //PART 6: Vf_step2////////////////////////////////////////////////////////
	    //////////////////////////////////////////////////////////////////////////

		RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Eval_PART6_STEP10);

		// -----------------------------------------------------------------------
		// SETTING KERNELS ARGUMENTS
		// -----------------------------------------------------------------------

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction, 0, sizeof(cl_mem), &_EVAL.ocl_matrix_K_star_star_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction, 1, sizeof(cl_mem), &_EVAL.ocl_matrix_Vf_step1_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction, 2, sizeof(cl_mem), &_EVAL.ocl_matrix_Vf_step2_m.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction, 3, sizeof(int), &_DATA.nTest_i);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// RUNNING KERNELS
		// -----------------------------------------------------------------------

		getGWWIandADDPAD(_DATA.nTest_t, _DATA.nTest_t, _DATA._one_t);

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.Eval_matrixSubtraction,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			0,0,0
		);
		SAMPLE_CHECK_ERRORS(err);

		err = clEnqueueBarrier(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		//------------------------------------------------------------------------

		RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Eval_PART6_STEP10);

		//------------------------------------------------------------------------

		//PART 7: threshold
		//STEP 11:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest

		_EVAL.ocl_matrix_Vf_step2_m.MapData(_DATA._zero_t);
		_EVAL.ocl_matrix_f_step2_m.MapData(_DATA._zero_t);
	
		//////////////////////////////////////////////////////////////////////////
	    //PART 7: Threshold STEP:A////////////////////////////////////////////////
	    //////////////////////////////////////////////////////////////////////////

		RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Eval_PART7A);

		// -----------------------------------------------------------------------
		// TEST POINTS
		// -----------------------------------------------------------------------

		//STEP 11:A:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestX.nTest

		int count = 0;
		for (size_t iZ = 0; iZ < _DATA.nTest_t; iZ++)
		{
			float Vf_step2 = _EVAL.ocl_matrix_Vf_step2_m.host[iZ*_DATA.nTest_t + iZ];

			float Test_Z = ocl_Test.ocl_Z.host[iZ];
			float f_step2 = _EVAL.ocl_matrix_f_step2_m.host[iZ];

			bool Vf_test = (Vf_step2 < _MODELOPTIONS.TModel);
			bool f_test = (((Test_Z - f_step2) / sqrt(_MODELOPTIONS.sigma_n + Vf_step2)) < _MODELOPTIONS.TData);

			// -----------------------------------------------------------------------

			SanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.EvalFunction + "-threshold", Vf_step2, _MODELOPTIONS.TModel, Vf_test, Test_Z, f_step2, _MODELOPTIONS.sigma_n,_MODELOPTIONS.TData, f_test, _DATA.iS, (int)iZ);
			
			// -----------------------------------------------------------------------

			if (Vf_test && f_test)
			{
				count = count + 1;

				_EVAL.ocl_Eval_idxs.host[count] = static_cast<int>(iZ);

			}
		}

		_EVAL.ocl_Eval_counter.host[0] = count;

		//------------------------------------------------------------------------

		RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Eval_PART7A);

		//------------------------------------------------------------------------

		if (_EVAL.ocl_Eval_counter.host[0] == _DATA._zero_i)
		{
			inliers = false;

			//////////////////////////////////////////////////////////////////////////
			//ESTIMATE HG/////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////

			ocl_OP.bufferData();

			// -----------------------------------------------------------------------
			// SETTING KERNELS ARGUMENTS
			// -----------------------------------------------------------------------

			//STEP 1:
			//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 0, sizeof(cl_mem), &ocl_OP.ocl_OP.device);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 1, sizeof(int), &_DATA.nOP_i);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 2, sizeof(cl_mem), &ocl_Sp.ocl_OP.device);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 3, sizeof(int), &_DATA.nModel_i);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_K_OP_X_m.device);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 5, sizeof(float), &_MODELOPTIONS.sigma_f);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel, 6, sizeof(float), &_MODELOPTIONS.length_scale);
			SAMPLE_CHECK_ERRORS(err);

			// -----------------------------------------------------------------------
			// RUNNING KERNELS
			// -----------------------------------------------------------------------

			getGWWIandADDPAD(_DATA.nOP_t, _DATA.nModel_t, _DATA._one_t);

			//STEP 1:
			//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
			err = clEnqueueNDRangeKernel(
				_OPENCLOBJECT.queue,
				_OPENCLOBJECT.my_kernels.Gauss_SquareExponentialKernel,
				_GPUOPTIONS.Work_Dim,
				0,
				_DATA.global_size,
				_DATA.local_size,
				0, 0, 0
			);
			SAMPLE_CHECK_ERRORS(err);

			err = clEnqueueBarrier(_OPENCLOBJECT.queue);
			SAMPLE_CHECK_ERRORS(err);

			err = clFinish(_OPENCLOBJECT.queue);
			SAMPLE_CHECK_ERRORS(err);

			// -----------------------------------------------------------------------
			// SETTING KERNELS ARGUMENTS
			// -----------------------------------------------------------------------

			//STEP 2:
			//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 0, sizeof(cl_mem), &_EVAL.ocl_matrix_K_OP_X_m.device);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 1, sizeof(int), &_DATA.nOP_i);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 2, sizeof(cl_mem), &_EVAL.ocl_matrix_Inverse_m.device);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 3, sizeof(int), &_DATA.nModel_i);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_f_OP_step1_m.device);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply, 5, sizeof(int), &_DATA.nModel_i);
			SAMPLE_CHECK_ERRORS(err);

			// -----------------------------------------------------------------------
			// RUNNING KERNELS
			// -----------------------------------------------------------------------

			getGWWIandADDPAD(_DATA.nOP_t, _DATA.nModel_t, _DATA._one_t);

			//STEP 2:
			//f_OP_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
			err = clEnqueueNDRangeKernel(
				_OPENCLOBJECT.queue,
				_OPENCLOBJECT.my_kernels.GEMM_matrixMultiply,
				_GPUOPTIONS.Work_Dim,
				0,
				_DATA.global_size,
				_DATA.local_size,
				0, 0, 0
			);
			SAMPLE_CHECK_ERRORS(err);

			err = clEnqueueBarrier(_OPENCLOBJECT.queue);
			SAMPLE_CHECK_ERRORS(err);

			err = clFinish(_OPENCLOBJECT.queue);
			SAMPLE_CHECK_ERRORS(err);

			// -----------------------------------------------------------------------
			// SETTING KERNELS ARGUMENTS
			// -----------------------------------------------------------------------

			//STEP 3:
			//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 0, sizeof(cl_mem), &_EVAL.ocl_matrix_f_OP_step1_m.device);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 1, sizeof(int), &_DATA.nOP_i);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 2, sizeof(cl_mem), &ocl_Sp.ocl_Z.device);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 3, sizeof(int), &_DATA.nModel_i);
			SAMPLE_CHECK_ERRORS(err);
			err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Eval_matrixVector, 4, sizeof(cl_mem), &_EVAL.ocl_matrix_f_OP_step2_m.device);
			SAMPLE_CHECK_ERRORS(err);

			// -----------------------------------------------------------------------
			// RUNNING KERNELS
			// -----------------------------------------------------------------------

			getGWWIandADDPAD(_DATA.nOP_t, _DATA.nModel_t, _DATA._one_t);

			//STEP 3:
			//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
			err = clEnqueueNDRangeKernel(
				_OPENCLOBJECT.queue,
				_OPENCLOBJECT.my_kernels.Eval_matrixVector,
				_GPUOPTIONS.Work_Dim,
				0,
				_DATA.global_size,
				_DATA.local_size,
				0, 0, 0
			);
			SAMPLE_CHECK_ERRORS(err);

			err = clEnqueueBarrier(_OPENCLOBJECT.queue);
			SAMPLE_CHECK_ERRORS(err);

			err = clFinish(_OPENCLOBJECT.queue);
			SAMPLE_CHECK_ERRORS(err);

			_EVAL.ocl_matrix_f_OP_step2_m.MapData(_DATA._zero_t);

			ocl_OP.releaseDataDevice();
			
			/////////////////////////////////////////////////////////////////////////
			//GET THE MODEL//////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////
			ocl_Sp.MapData(_DATA._zero_t);

			for (size_t i = 0; i < _DATA.nOP_t; i++)
			{
				float Hg = _EVAL.ocl_matrix_f_OP_step2_m.host[i];

				Model_Hg(i,_DATA.iS) = Hg;
			}
			/////////////////////////////////////////////////////////////////////////

		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
		    //PART 7: Threshold STEP:B////////////////////////////////////////////////
		    //////////////////////////////////////////////////////////////////////////

			RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Eval_PART7B);

			// -----------------------------------------------------------------------
			// ALLOCATING DEVICES-SIDE RESOURCES
			// -----------------------------------------------------------------------

			_DATA.Sp_dim_t = _DATA.nModel_t + (size_t)_EVAL.ocl_Eval_counter.host[0];
			_DATA.Sp_dim_i = static_cast<int>(_DATA.Sp_dim_t);

			// -----------------------------------------------------------------------
			// EXTRACT SECTOR
			// -----------------------------------------------------------------------

			for (size_t j = 0; j < (size_t)_EVAL.ocl_Eval_counter.host[0]; j++)
			{
				int _idx = (int)_EVAL.ocl_Eval_idxs.host[j];

				int idx = ocl_Test.ocl_original_idx.host[_idx];

				inliers_idx.host[j + _DATA.nModel_t] = idx;

			}

			inliers = true;

			//------------------------------------------------------------------------

			RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Eval_PART7B);

		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//SANITY CHECK ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector,_LOGINFOOPTIONS.codeSteps.EvalFunction, _EVAL, ocl_OP, _DATA.nModel_t, _DATA.nTest_t, _DATA.nOP_t, inliers_idx.host);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//FREE MEMORY ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ocl_OP.releaseDataHost();
		
		_EVAL.releaseDataDevice();
		_EVAL.releaseDataHost();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

void getMatrixInverse(struct DeviceAndHostMemoryX<float> & A_m)
{
	
	int N = static_cast<int>(std::sqrt(A_m.dim_of_data));

	spotrf(_DATA.Matrix_Triangular_Upper, N, A_m, N);

	spotri(_DATA.Matrix_Triangular_Upper, N, A_m, N);

}

//////////////////////////////////////////////////////////////////////////////////////////////
// LAPACK - BLAS /////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void sgemm(int transA, int transB, int M, int N, int K, float Alpha, struct DeviceAndHostMemoryX<float> & A_m, int A_offset, int ldda, struct DeviceAndHostMemoryX<float> & B_m, int B_offset, int lddb, float Beta, struct DeviceAndHostMemoryX<float> & C_m, int C_offset, int lddc)
{
	cl_int err = 0;

	/*
	kernel void sGEMM (
		int TRANSA,
		int TRANSB,
		int M,
		int N,
		int K,
		int ALPHA,
		global float* A,
		int A_offset,
		int LDA,
		global float* B,
		int B_offset,
		int LDB,
		float BETA,
		global float* C,
		int C_offset,
		int LDC
	)
	*/

	// -----------------------------------------------------------------------
	// SETTING KERNELS ARGUMENTS
	// -----------------------------------------------------------------------

	//sGEMM:
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 0, sizeof(int), &transA);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 1, sizeof(int), &transB);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 2, sizeof(int), &M);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 3, sizeof(int), &N);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 4, sizeof(int), &K);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 5, sizeof(float), &Alpha);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 6, sizeof(cl_mem), &A_m.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 7, sizeof(int), &A_offset);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 8, sizeof(int), &ldda);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 9, sizeof(cl_mem), &B_m.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 10, sizeof(int), &B_offset);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 11, sizeof(int), &lddb);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 12, sizeof(float), &Beta);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 13, sizeof(cl_mem), &C_m.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 14, sizeof(int), &C_offset);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM, 15, sizeof(int), &lddc);
	SAMPLE_CHECK_ERRORS(err);

	// -----------------------------------------------------------------------
	// RUNNING KERNELS
	// -----------------------------------------------------------------------

	getGWWIandADDPAD(static_cast<size_t>(M), static_cast<size_t>(N), _DATA._one_t);

	//sGEMM:
	err = clEnqueueNDRangeKernel(
		_OPENCLOBJECT.queue,
		_OPENCLOBJECT.my_kernels.matrixInverse_sGEMM,
		_GPUOPTIONS.Work_Dim,
		0,
		_DATA.global_size,
		_DATA.local_size,
		0, 0, 0
	);
	SAMPLE_CHECK_ERRORS(err);

	err = clEnqueueBarrier(_OPENCLOBJECT.queue);
	SAMPLE_CHECK_ERRORS(err);

	err = clFinish(_OPENCLOBJECT.queue);
	SAMPLE_CHECK_ERRORS(err);
}

void ssyrk(int uplo, int transA, int N, int K, float Alpha, struct DeviceAndHostMemoryX<float> & A_m, int A_offset, int ldda, float Beta, struct DeviceAndHostMemoryX<float> & C_m, int C_offset, int lddc)
{
	cl_int err = 0;

	/*
	kernel void matrixInverse_sSYRK (
		int UPLO,
		int TRANS,
		int N,
		int K,
		float ALPHA,
		global float* A,
		int A_offset,
		int LDA,
		float BETA,
		global float* C,
		int C_offset,
		int LDC
	)
	*/

	// -----------------------------------------------------------------------
	// SETTING KERNELS ARGUMENTS
	// -----------------------------------------------------------------------

	//sSYRK:
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 0, sizeof(int), &uplo);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 1, sizeof(int), &transA);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 2, sizeof(int), &N);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 3, sizeof(int), &K);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 4, sizeof(float), &Alpha);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 5, sizeof(cl_mem), &A_m.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 6, sizeof(int), &A_offset);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 7, sizeof(int), &ldda);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 8, sizeof(int), &Beta);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 9, sizeof(cl_mem), &C_m.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 10, sizeof(int), &C_offset);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK, 11, sizeof(int), &lddc);
	SAMPLE_CHECK_ERRORS(err);
	

	// -----------------------------------------------------------------------
	// RUNNING KERNELS
	// -----------------------------------------------------------------------


	getGWWIandADDPAD(static_cast<size_t>(N), static_cast<size_t>(K), _DATA._one_t);

	//sSYRK:
	err = clEnqueueNDRangeKernel(
		_OPENCLOBJECT.queue,
		_OPENCLOBJECT.my_kernels.matrixInverse_sSYRK,
		_GPUOPTIONS.Work_Dim,
		0,
		_DATA.global_size,
		_DATA.local_size,
		0, 0, 0
	);
	SAMPLE_CHECK_ERRORS(err);

	err = clEnqueueBarrier(_OPENCLOBJECT.queue);
	SAMPLE_CHECK_ERRORS(err);

	err = clFinish(_OPENCLOBJECT.queue);
	SAMPLE_CHECK_ERRORS(err);
}

void strsm(int side, int uplo, int transA, int diag, int M, int N, float Alpha, struct DeviceAndHostMemoryX<float> & A_m, int A_offset, int lda, struct DeviceAndHostMemoryX<float> & B_m, int B_offset, int ldb)
{

	cl_int err = 0;

	/*
	kernel void sTRSM(
		int SIDE,
		int UPLO,
		int TRANSA,
		int DIAG,
		int M,
		int N,
		float ALPHA,
		global float* A,
		int A_offset,
		int LDA,
		global float* B,
		int B_offset,
		int LDB
	)
	*/

	// -----------------------------------------------------------------------
	// SETTING KERNELS ARGUMENTS
	// -----------------------------------------------------------------------

	//C_m = A_m * B_m;
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 0, sizeof(int), &side);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 1, sizeof(int), &uplo);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 2, sizeof(int), &transA);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 3, sizeof(int), &diag);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 4, sizeof(int), &M);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 5, sizeof(int), &N);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 6, sizeof(float), &Alpha);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 7, sizeof(cl_mem), &A_m.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 8, sizeof(int), &A_offset);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 9, sizeof(int), &lda);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 10, sizeof(cl_mem), &B_m.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 11, sizeof(int), &B_offset);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM, 12, sizeof(int), &ldb);
	SAMPLE_CHECK_ERRORS(err);

	// -----------------------------------------------------------------------
	// RUNNING KERNELS
	// -----------------------------------------------------------------------

	getGWWIandADDPAD(static_cast<size_t>(M), static_cast<size_t>(N), _DATA._one_t);

	//C_m = A_m * B_m;
	err = clEnqueueNDRangeKernel(
		_OPENCLOBJECT.queue,
		_OPENCLOBJECT.my_kernels.matrixInverse_sTRSM,
		_GPUOPTIONS.Work_Dim,
		0,
		_DATA.global_size,
		_DATA.local_size,
		0, 0, 0
	);
	SAMPLE_CHECK_ERRORS(err);

	err = clEnqueueBarrier(_OPENCLOBJECT.queue);
	SAMPLE_CHECK_ERRORS(err);

	err = clFinish(_OPENCLOBJECT.queue);
	SAMPLE_CHECK_ERRORS(err);

}

void slauum(int uplo, int N, struct DeviceAndHostMemoryX<float> & A_m, int ldda)
{

	int nb;

	getGWWIandADDPAD(N, _DATA._one_t, _DATA._one_t);

	nb = static_cast<int>(_DATA.global_size[0] / _DATA.local_size[0]);

	struct DeviceAndHostMemoryX<float> work;
	work.allocData(nb*nb,_DATA._one_t,_DATA._one_t);

	
	if (uplo == _DATA.Matrix_Triangular_Upper)
	{
		/* Compute inverse of upper triangular matrix */
		for (int i = 0; i < N; i = i + nb) 
		{
			int ib = std::min(nb, (N - i));

			/* Compute the product U * U'. */
			int A_offset = (i) + (i)*ldda;
			int B_offset = (0) + (i)*ldda;
			int C_offset = 0;

			strsm(_DATA.Matrix_Side_Right, _DATA.Matrix_Triangular_Upper, _DATA.Matrix_Transpose, _DATA.Matrix_No_Unit_Triangular, i, ib, _DATA._one_f, A_m, A_offset, ldda, A_m, B_offset,ldda);

			/*
			magma_sgetmatrix(ib, ib,dA(i, i), ldda,work, 0, ib, queue);

			lapackf77_slauum(MagmaUpperStr, &ib, work, &ib, info);

			magma_ssetmatrix(ib, ib,work, 0, ib,dA(i, i), ldda, queue);
			*/

			if (i + ib < N) 
			{
				A_offset = (0)+(i+ib)*ldda;
				B_offset = (i) + (i+ib)*ldda;
				C_offset = (0)+(i)*ldda;

				sgemm(_DATA.Matrix_No_Transpose, _DATA.Matrix_Transpose,i, ib, (N - i - ib), _DATA._one_f, A_m, A_offset, ldda, A_m, B_offset, ldda, _DATA._one_f,A_m, C_offset, ldda);

				A_offset = (i)+(i+ib)*ldda;
				B_offset = (i) + (i)*ldda;

				ssyrk(_DATA.Matrix_Triangular_Upper, _DATA.Matrix_No_Transpose, ib, (N - i - ib), _DATA._one_f, A_m, A_offset, ldda, _DATA._one_f, A_m, B_offset, ldda);
			}
		}
	}
	else 
	{
		/* Compute the product L' * L. */
		for (int i = 0; i < N; i = i + nb) 
		{
			int ib = std::min(nb, (N - i));

			int A_offset = (i)+(i)*ldda;
			int B_offset = (i) + (0)*ldda;
			int C_offset = 0;

			strsm(_DATA.Matrix_Side_Left, _DATA.Matrix_Triangular_Lower, _DATA.Matrix_Transpose, _DATA.Matrix_No_Unit_Triangular, ib, i, _DATA._one_f, A_m, A_offset, ldda, A_m, B_offset, ldda);

			/*
			magma_sgetmatrix(ib, ib,dA(i, i), ldda,work, 0, ib, queue);

			lapackf77_slauum(MagmaLowerStr, &ib, work, &ib, info);

			magma_ssetmatrix(ib, ib,work, 0, ib,dA(i, i), ldda, queue);
			*/


			if ((i + ib) < N) 
			{
				A_offset = (i+ib) + (i)*ldda;
				B_offset = (i+ib)+(0)*ldda;
				C_offset = (i) + (0)*ldda;

				sgemm(_DATA.Matrix_Transpose, _DATA.Matrix_No_Transpose, ib, i, (N - i - ib), _DATA._one_f, A_m, A_offset, ldda, A_m, B_offset, ldda, _DATA._one_f, A_m, C_offset, ldda);

				A_offset = (i + ib) + (i)*ldda;
				B_offset = (i) + (i)*ldda;

				ssyrk(_DATA.Matrix_Triangular_Lower, _DATA.Matrix_Transpose, ib, (N - i - ib), _DATA._one_f, A_m, A_offset, ldda, _DATA._one_f, A_m, B_offset, ldda);

			}
		}
	}

	work.releaseDataHost();
	
}

void spotri(int uplo, int N, struct DeviceAndHostMemoryX<float> & A_m, int ldda)
{
	/* Invert the triangular Cholesky factor U or L */
	strtri(uplo, _DATA.Matrix_No_Unit_Triangular, N, A_m, ldda);

	/* Form inv(U) * inv(U)**T or inv(L)**T * inv(L) */
	slauum(uplo, N, A_m, ldda);

}

void strtri(int uplo, int diag, int N, struct DeviceAndHostMemoryX<float> & A_m, int ldda)
{
	int nb;

	getGWWIandADDPAD(N, _DATA._one_t, _DATA._one_t);

	nb = static_cast<int>(_DATA.global_size[0] / _DATA.local_size[0]);

	if (uplo == _DATA.Matrix_Triangular_Upper)
	{

		/* Compute inverse of upper triangular matrix */
		for (int j = 0; j < N; j = j + nb)
		{

			int jb = std::min(nb, (N - j));

			int A_offset = (0) + (0)*ldda;
			int B_offset = (0) + (j)*ldda;

			/* Compute rows 1:j-1 of current block column */
			strsm(_DATA.Matrix_Side_Left, _DATA.Matrix_Triangular_Upper, _DATA.Matrix_No_Transpose, _DATA.Matrix_No_Unit_Triangular, j, jb, _DATA._one_f, A_m, A_offset, ldda, A_m, B_offset, ldda);

			A_offset = (j)+(j)*ldda;
			B_offset = (0) + (j)*ldda;

			strsm(_DATA.Matrix_Side_Right, _DATA.Matrix_Triangular_Upper, _DATA.Matrix_No_Transpose, _DATA.Matrix_No_Unit_Triangular, j, jb, _DATA._m_one_f, A_m, A_offset, ldda, A_m, B_offset, ldda);

			/*
			
			magma_sgetmatrix_async( jb, jb,
						dA(j, j), ldda,
						work, 0, jb, queues[1], NULL );

				magma_queue_sync( queues[1] );

				// Compute inverse of current diagonal block //
			lapackf77_strtri(MagmaUpperStr, lapack_const(diag_), &jb, work, &jb, info);

			//
			//magma_ssetmatrix_async( jb, jb,
			//		work, 0, jb,
			//		dA(j, j), ldda, queues[0], NULL );
			//

			magma_ssetmatrix(jb, jb,
				work, 0, jb,
				dA(j, j), ldda, queues[0]);

			*/

		}
	}
	else
	{

		/* Compute inverse of lower triangular matrix */
		int nn = ((N - 1) / nb)*nb + 1;

		for (int j = nn - 1; j >= 0; j = j - nb)
		{

			int jb = std::min(nb, (N - j));

			if ((j + jb) < N)
			{

				/* Compute rows j+jb:n of current block column */
				int A_offset = (j + jb) + (j + jb)*ldda;
				int B_offset = (j + jb) + (j)*ldda;

				strsm(_DATA.Matrix_Side_Left, _DATA.Matrix_Triangular_Lower, _DATA.Matrix_No_Transpose, _DATA.Matrix_No_Unit_Triangular, (N - j - jb), jb, _DATA._one_f, A_m, A_offset, ldda, A_m, B_offset, ldda);

				A_offset = (j)+(j)*ldda;
				B_offset = (j + jb) + (j)*ldda;

				strsm(_DATA.Matrix_Side_Right, _DATA.Matrix_Triangular_Lower, _DATA.Matrix_No_Transpose, _DATA.Matrix_No_Unit_Triangular, (N - j - jb), jb, _DATA._m_one_f, A_m, A_offset, ldda, A_m, B_offset, ldda);

			}

			/*
			
			magma_sgetmatrix_async( jb, jb,
						dA(j, j), ldda,
						work, 0, jb, queues[1], NULL );

				magma_queue_sync( queues[1] );

				// Compute inverse of current diagonal block //
			lapackf77_strtri(MagmaLowerStr, lapack_const(diag_), &jb, work, &jb, info);
			
			//
			//magma_ssetmatrix_async( jb, jb,
			//		work, 0, jb,
			//		dA(j, j), ldda, queues[0], NULL );
			//

			magma_ssetmatrix(jb, jb,
				work, 0, jb,
				dA(j, j), ldda, queues[0]);
			
			*/


		}
	}

}

void spotrf(int uplo, int N, struct DeviceAndHostMemoryX<float> & A_m, int ldda)
{
	
	int nb;

	getGWWIandADDPAD(N, _DATA._one_t, _DATA._one_t);

	nb = static_cast<int>(_DATA.global_size[0] / _DATA.local_size[0]);

	/*
	err = magma_smalloc_cpu(&work, nb*nb);
	if (err != MAGMA_SUCCESS) {
		*info = MAGMA_ERR_HOST_ALLOC;
		return *info;
	}
	*/

	
	if (uplo == _DATA.Matrix_Triangular_Upper) 
	{
		// --------------------
		// compute Cholesky factorization A = U'*U
		// using the left looking algorithm
		for (int j = 0; j < N; j += nb) 
		{
			int A_offset = 0;
			int B_offset = 0;
			int C_offset = 0;

			// apply all previous updates to diagonal block
			int jb = std::min(nb, N - j);
			if (j > 0) 
			{
				A_offset = (0) + (j)*ldda;
				B_offset = (j) + (j)*ldda;

				ssyrk(_DATA.Matrix_Triangular_Upper, _DATA.Matrix_Transpose, jb, j,_DATA._m_one_f, A_m, A_offset, ldda,_DATA._one_f, A_m, B_offset, ldda);
			}

			// start asynchronous data transfer
			//chk(magma_sgetmatrix_async(jb, jb, dA(j, j), ldda, work, 0, jb, queue, &event));

			// apply all previous updates to block row right of diagonal block
			if (j + jb < N) 
			{
				A_offset = (0) + (j)*ldda;
				B_offset = (0)+(j+jb)*ldda;
				C_offset = (j) + (j+jb)*ldda;

				sgemm(_DATA.Matrix_Transpose, _DATA.Matrix_No_Transpose,jb, N - j - jb, j, _DATA._m_one_f, A_m, A_offset, ldda, A_m, B_offset, ldda,_DATA._one_f, A_m, C_offset, ldda);
			}

			// simultaneous with above sgemm, transfer data, factor
			// diagonal block on CPU, and test for positive definiteness
			
			//chk(magma_event_sync(event));

			//lapackf77_spotrf(MagmaUpperStr, &jb, work, &jb, info);
			
			//magma_ssetmatrix_async(jb, jb, work, 0, jb, dA(j, j), ldda, queue, &event);

			// apply diagonal block to block row right of diagonal block
			if (j + jb < N) 
			{
				//magma_event_sync(event);

				A_offset = (j) + (j)*ldda;
				B_offset = (j) + (j + jb)*ldda;

				strsm(_DATA.Matrix_Side_Left, _DATA.Matrix_Triangular_Upper, _DATA.Matrix_Transpose, _DATA.Matrix_No_Unit_Triangular,jb, N - j - jb,_DATA._one_f, A_m, A_offset, ldda, A_m, B_offset, ldda);

			}
		}
	}
	else 
	{
		// --------------------
		// compute Cholesky factorization A = L*L'
		// using the left looking algorithm
		for (int j = 0; j < N; j += nb) 
		{
			int A_offset = 0;
			int B_offset = 0;
			int C_offset = 0;

			// apply all previous updates to diagonal block
			int jb = std::min(nb, N - j);
			if (j > 0) 
			{

				A_offset = (j) + (0)*ldda;
				B_offset = (j)+(j)*ldda;

				ssyrk(_DATA.Matrix_Triangular_Lower, _DATA.Matrix_No_Transpose, jb, j, _DATA._m_one_f, A_m, A_offset, ldda, _DATA._one_f, A_m, B_offset, ldda);
			}

			// start asynchronous data transfer
			//chk(magma_sgetmatrix_async(jb, jb, dA(j, j), ldda, work, 0, jb, queue, &event));

			// apply all previous updates to block column below diagonal block
			if (j + jb < N) 
			{

				A_offset = (j) + (j+jb)*ldda;
				B_offset = (j) + (0)*ldda;
				C_offset = (j+jb)+(j)*ldda;

				sgemm(_DATA.Matrix_No_Transpose, _DATA.Matrix_Transpose, jb, N - j - jb, j, _DATA._m_one_f, A_m, A_offset, ldda, A_m, B_offset, ldda, _DATA._one_f, A_m, C_offset, ldda);
			}

			// simultaneous with above sgemm, transfer data, factor
			// diagonal block on CPU, and test for positive definiteness
			//chk(magma_event_sync(event));
			
			//lapackf77_spotrf(MagmaLowerStr, &jb, work, &jb, info);
			
			//magma_ssetmatrix_async(jb, jb, work, 0, jb, dA(j, j), ldda, queue, &event);

			// apply diagonal block to block column below diagonal
			if (j + jb < N) 
			{
				//magma_event_sync(event);
				
				A_offset = (j)+(j)*ldda;
				B_offset = (j+jb)+(j)*ldda;

				strsm(_DATA.Matrix_Side_Right, _DATA.Matrix_Triangular_Lower, _DATA.Matrix_Transpose, _DATA.Matrix_No_Unit_Triangular,N - j - jb,jb, _DATA._one_f, A_m, A_offset, ldda, A_m, B_offset, ldda);
			}
		}
	}
	
}