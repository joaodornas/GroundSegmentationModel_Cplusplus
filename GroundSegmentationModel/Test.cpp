#pragma once

#include "Globals.h"
#include "Test.h"

void getTest(struct Sectors_struct & ocl_Test, struct PGi_struct(&PGi)[361], struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp)
{
	RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.GetTestFunction);

	//------------------------------------------------------------------------

	cl_int err = 0;
	
	// -----------------------------------------------------------------------
	// ALLOCATING DEVICES-SIDE RESOURCES
	// -----------------------------------------------------------------------

	struct Sectors_struct ocl_Test_tmp;

	_DATA.Test_dim_tmp_t = PGi[_DATA.iS].PGi_dim_t;
	_DATA.Test_dim_tmp_i = PGi[_DATA.iS].PGi_dim_i;

	ocl_Test_tmp.allocData(_DATA.Test_dim_tmp_t,_DATA._one_t,_DATA._one_t);
	
	// -----------------------------------------------------------------------
	// CREATE BUFFERS
	// -----------------------------------------------------------------------

	ocl_PGi.bufferData();
	
	ocl_Test_tmp.bufferData();

	struct DeviceAndHostMemoryX<int> ocl_RemoveCode;

	ocl_RemoveCode.allocData(_DATA._one_t, _DATA._one_t, _DATA._one_t);

	ocl_RemoveCode.host[0] = 555;

	ocl_RemoveCode.bufferData();

	// -----------------------------------------------------------------------
	// SETTING KERNELS ARGUMENTS
	// -----------------------------------------------------------------------

	//STEP: Extract Test from PGi
	//Test = PGi - Sp
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 0, sizeof(cl_mem), &ocl_Test_tmp.ocl_Z.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 1, sizeof(cl_mem), &ocl_Test_tmp.ocl_OP.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 2, sizeof(cl_mem), &ocl_Test_tmp.ocl_Bins.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 3, sizeof(cl_mem), &ocl_Test_tmp.ocl_original_idx.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 4, sizeof(cl_mem), &ocl_PGi.ocl_Z.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 5, sizeof(cl_mem), &ocl_PGi.ocl_OP.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 6, sizeof(cl_mem), &ocl_PGi.ocl_Bins.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 7, sizeof(cl_mem), &ocl_PGi.ocl_original_idx.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 8, sizeof(cl_mem), &ocl_Sp.ocl_original_idx.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 9, sizeof(int), &_DATA.Sp_dim_i);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step1, 10, sizeof(cl_mem), &ocl_RemoveCode.device);
	SAMPLE_CHECK_ERRORS(err);

	// -----------------------------------------------------------------------
	// RUNNING KERNELS
	// -----------------------------------------------------------------------

	getGWWIandADDPAD(PGi[_DATA.iS].PGi_dim_t, _DATA._one_t, _DATA._one_t);

	//STEP: Extract Test from PGi
	//PGi = Test - Sp
	err = clEnqueueNDRangeKernel(
		_OPENCLOBJECT.queue,
		_OPENCLOBJECT.my_kernels.Test_getTest_Step1,
		_GPUOPTIONS.Work_Dim,
		0,
		_DATA.global_size,
		_DATA.local_size,
		NULL, NULL, NULL
	);
	SAMPLE_CHECK_ERRORS(err);

	err = clFinish(_OPENCLOBJECT.queue);
	SAMPLE_CHECK_ERRORS(err);

	// -----------------------------------------------------------------------
	// REMOVE UNUSED DATA
	// -----------------------------------------------------------------------

	ocl_Test_tmp.ocl_Z.MapData(_DATA._zero_t);

	size_t counter = 0;
	for (size_t i = 0; i < PGi[_DATA.iS].PGi_dim_t; i++)
	{
		if (ocl_Test_tmp.ocl_Z.host[i] == ocl_RemoveCode.host[0])
		{
			counter++;
		}
	}

	_DATA.Test_dim_t = PGi[_DATA.iS].PGi_dim_t - counter;
	_DATA.Test_dim_i = static_cast<int>(_DATA.Test_dim_t);

	if (_DATA.Test_dim_t > _DATA._zero_t)
	{
		ocl_Test.allocData(_DATA.Test_dim_t, _DATA._one_t, _DATA._one_t);

		ocl_Test.bufferData();

		struct DeviceAndHostMemoryX<int> ocl_idxs;
		ocl_idxs.allocData(_DATA.Test_dim_t, _DATA._one_t, _DATA._one_t);

		counter = 0;
		for (int i = 0; i < PGi[_DATA.iS].PGi_dim_i; i++)
		{
			if (ocl_Test_tmp.ocl_Z.host[i] != ocl_RemoveCode.host[0])
			{
				ocl_idxs.host[counter] = i;
				counter++;
			}
		}

		ocl_idxs.bufferData();

		// -----------------------------------------------------------------------
		// SETTING KERNELS ARGUMENTS
		// -----------------------------------------------------------------------

		//STEP: Remove Unused Data from Test
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 0, sizeof(cl_mem), &ocl_Test.ocl_Z.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 1, sizeof(cl_mem), &ocl_Test.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 2, sizeof(cl_mem), &ocl_Test.ocl_Bins.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 3, sizeof(cl_mem), &ocl_Test.ocl_original_idx.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 4, sizeof(cl_mem), &ocl_Test_tmp.ocl_Z.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 5, sizeof(cl_mem), &ocl_Test_tmp.ocl_OP.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 6, sizeof(cl_mem), &ocl_Test_tmp.ocl_Bins.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 7, sizeof(cl_mem), &ocl_Test_tmp.ocl_original_idx.device);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 8, sizeof(int), &_DATA.Test_dim_tmp_i);
		SAMPLE_CHECK_ERRORS(err);
		err = clSetKernelArg(_OPENCLOBJECT.my_kernels.Test_getTest_Step2, 9, sizeof(cl_mem), &ocl_idxs.device);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// RUNNING KERNELS
		// -----------------------------------------------------------------------

		getGWWIandADDPAD(_DATA.Test_dim_t, _DATA._one_t, _DATA._one_t);

		//STEP: Remove Unused Data from Test
		err = clEnqueueNDRangeKernel(
			_OPENCLOBJECT.queue,
			_OPENCLOBJECT.my_kernels.Test_getTest_Step2,
			_GPUOPTIONS.Work_Dim,
			0,
			_DATA.global_size,
			_DATA.local_size,
			NULL, NULL, NULL
		);
		SAMPLE_CHECK_ERRORS(err);

		err = clFinish(_OPENCLOBJECT.queue);
		SAMPLE_CHECK_ERRORS(err);

		// -----------------------------------------------------------------------
		// CLEAR DATA
		// -----------------------------------------------------------------------

		ocl_idxs.releaseDataHost();

		ocl_idxs.releaseDataDevice();

		ocl_Test.MapData(_DATA._zero_t);

	}

	// -----------------------------------------------------------------------
	// CLEAR DATA
	// -----------------------------------------------------------------------

	ocl_PGi.releaseDataDevice();

	ocl_Test_tmp.releaseDataDevice();

	ocl_Test_tmp.releaseDataHost();

	_DATA.Test_dim_tmp_t = _DATA._zero_t;
	_DATA.Test_dim_tmp_i = _DATA._zero_i;

	ocl_RemoveCode.releaseDataDevice();

	ocl_RemoveCode.releaseDataHost();

	//------------------------------------------------------------------------

	SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.GetTestFunction, ocl_Test, _DATA.Test_dim_t);

	//------------------------------------------------------------------------

	RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.GetTestFunction);

}