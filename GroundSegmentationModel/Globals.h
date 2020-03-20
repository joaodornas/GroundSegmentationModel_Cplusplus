#pragma once

#include <ctime>
#include "utils.h"
#include <vector>
#include <string>
#include <experimental/filesystem>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <map>

#include <CL/cl.h>

#include "basic.hpp"

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <Eigen/Dense>

#include "utils.h"
#include "stdafx.h"

template <typename T>
struct DeviceAndHostMemoryX
{
	size_t matrix_memory_size = 0;
	size_t aligned_size = 0;
	size_t dim_of_data = 0;
	size_t dim_x = 0;
	size_t dim_y = 0;
	size_t dim_z = 0;
	cl_mem device = NULL;
	T * host = nullptr;

	bool host_set_to_free;
	bool device_set_to_free;

	void allocData(size_t dim_x, size_t dim_y, size_t dim_z)
	{
		if (host != NULL)
		{
			throw "data not NULL";
		}
		else
		{
			//alloc memory - host

			dim_of_data = dim_x * dim_y * dim_z;

			matrix_memory_size = dim_of_data * sizeof(T);

			aligned_size = zeroCopySizeAlignment(matrix_memory_size, _OPENCLOBJECT.device);

			_OPENCLOBJECT.alignmentForPtr = zeroCopyPtrAlignment(_OPENCLOBJECT.device);

			host = (T*)aligned_malloc(aligned_size, _OPENCLOBJECT.alignmentForPtr);

			std::fill(host, host + dim_of_data, T(0));

			// --

			host_set_to_free = false;

		}

	}

	void clearData()
	{
		std::fill(host, host + dim_of_data, T(0));
	}

	void releaseDataHost()
	{
		if (host == NULL)
		{
			throw "host NULL";
		}
		else
		{
			aligned_free(host);

			host_set_to_free = true;
			
		}

	}

	void bufferData()
	{

		if (device != NULL)
		{
			throw "device not NULL";
		}
		else
		{
			cl_int err = 0;

			device = clCreateBuffer(
				_OPENCLOBJECT.context,
				CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
				matrix_memory_size,
				host,
				&err
			);
			SAMPLE_CHECK_ERRORS(err);

			device_set_to_free = false;

		}

	}

	void bufferAllocData()
	{
		
		if (device != NULL)
		{
			throw "device not NULL";
		}
		else
		{
			cl_int err = 0;

			device = clCreateBuffer(
				_OPENCLOBJECT.context,
				CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,
				matrix_memory_size,
				NULL,
				&err
			);
			SAMPLE_CHECK_ERRORS(err);

			device_set_to_free = false;

		}
	}

	void MapData(size_t _offset)
	{
		if (device == NULL)
		{
			throw "device NULL";
		}
		else
		{
			cl_int err = 0;

			clEnqueueMapBuffer(
				_OPENCLOBJECT.queue,
				device,
				CL_TRUE,    // blocking map
				CL_MAP_READ,
				_offset,
				(matrix_memory_size - (_offset * sizeof(T))),
				0, 0, 0,
				&err
			);
			SAMPLE_CHECK_ERRORS(err);

		}

	}

	void MapAllocData(size_t _offset)
	{
		if (device == NULL)
		{
			throw "device NULL";
		}
		else
		{
			cl_int err = 0;

			host = (T*)clEnqueueMapBuffer(
				_OPENCLOBJECT.queue,
				device,
				CL_TRUE,    // blocking map
				CL_MAP_READ,
				_offset,
				(matrix_memory_size - (_offset * sizeof(T))),
				0, NULL, NULL,
				&err
			);
			SAMPLE_CHECK_ERRORS(err);

		}

	}

	void unMapData()
	{
		clEnqueueUnmapMemObject(
			_OPENCLOBJECT.queue, 
			device, 
			host, 
			0, NULL, NULL
		);

	}
		
	void readBuffer(size_t _offset)
	{
			
		if (device == NULL)
		{
			throw "device NULL";
		}
		else
		{
			cl_int err = 0;

			err = clEnqueueWriteBuffer(
				_OPENCLOBJECT.queue,
				device,
				CL_TRUE,
				_offset,
				(size_of_data - (_offset * sizeof(T))),
				host,
				NULL,
				NULL,
				0);

		}

	}

	void readBufferRect(size_t dim_x, size_t dim_y, size_t dim_z, size_t data_device_offset, size_t data_host_offset, size_t ldda, size_t ldha, float* host_destination)
	{
		if (device == NULL)
		{
			throw "device NULL";
		}
		else
		{
			cl_int err = 0;

			size_t device_offset[3] = { data_device_offset * sizeof(T), 0, 0 };
			size_t host_offset[3] = { data_host_offset * sizeof(T), 0, 0 };
			size_t region[3] = { dim_x * sizeof(T), dim_y, dim_z };

			cl_int err = clEnqueueReadBufferRect(
				_OPENCLOBJECT.queue,
				device, 
				CL_TRUE,
				device_offset, 
				host_offset, 
				region,
				ldda * sizeof(T),
				0,
				ldha * sizeof(T),
				0,
				host_detination,
				0,
				NULL, 
				0);

		}
	}
	
	void releaseDataDevice()
	{
		if (device == NULL)
		{
			throw "device NULL";
		}
		else
		{
			cl_int err = 0;
			err = clReleaseMemObject(device);
			SAMPLE_CHECK_ERRORS(err);

			device = NULL;

			device_set_to_free = true;
		}

	}

public:

	DeviceAndHostMemoryX() 
	{
	
		device = NULL;

	};

	~DeviceAndHostMemoryX() 
	{
	
		if ( (host != NULL) && (!host_set_to_free) )
		{
			aligned_free(host);
		}

		if ( (device != NULL) && (!device_set_to_free) ) 
		{

			cl_int err = 0;
			err = clReleaseMemObject(device);

			device = NULL;

		}
	
	};

};

struct Sectors_struct
{
	struct DeviceAndHostMemoryX<float> ocl_Z;
	struct DeviceAndHostMemoryX<float> ocl_OP;
	struct DeviceAndHostMemoryX<int> ocl_Bins;
	struct DeviceAndHostMemoryX<int> ocl_original_idx;

	void allocData(size_t dim_x, size_t dim_y, size_t dim_z)
	{
		ocl_Z.allocData(dim_x,dim_y,dim_z);
		ocl_OP.allocData(dim_x, dim_y, dim_z);
		ocl_Bins.allocData(dim_x, dim_y, dim_z);
		ocl_original_idx.allocData(dim_x, dim_y, dim_z);

	}

	void clearData()
	{
		ocl_Z.clearData();
		ocl_OP.clearData();
		ocl_Bins.clearData();
		ocl_original_idx.clearData();

	}

	void releaseDataHost()
	{
		ocl_Z.releaseDataHost();
		ocl_OP.releaseDataHost();
		ocl_Bins.releaseDataHost();
		ocl_original_idx.releaseDataHost();

	}

	void bufferData()
	{

		ocl_Z.bufferData();
		ocl_OP.bufferData();
		ocl_Bins.bufferData();
		ocl_original_idx.bufferData();

	}

	void MapData(size_t _offset)
	{
		ocl_Z.MapData(_offset);
		ocl_OP.MapData(_offset);
		ocl_Bins.MapData(_offset);
		ocl_original_idx.MapData(_offset);

	}

	void releaseDataDevice()
	{
		ocl_Z.releaseDataDevice();
		ocl_OP.releaseDataDevice();
		ocl_Bins.releaseDataDevice();
		ocl_original_idx.releaseDataDevice();

	}

	Sectors_struct() {};

	~Sectors_struct() {};

};

struct EVAL_struct {

	// -----------------------------------------------------------------------
	// EVAL VARIABLES
	// -----------------------------------------------------------------------

	size_t _one_t = 1;
	size_t _zero_t = 0;

	//STEP 1:
	//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
	struct DeviceAndHostMemoryX<float> ocl_matrix_K_star_X_m;

	//STEP 2:
	//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
	struct DeviceAndHostMemoryX<float> ocl_matrix_K_X_star_m;

	//STEP 3:
	//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
	struct DeviceAndHostMemoryX<float> ocl_matrix_K_star_star_m;

	//STEP 4:
	//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
	struct DeviceAndHostMemoryX<float> ocl_matrix_K_X_X_m;

	//STEP 6:
	//Inverse_m = inverse_tmp.inverse(); nModelXnModel
	struct DeviceAndHostMemoryX<float> ocl_matrix_Inverse_m;

	//STEP 7:
	//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
	struct DeviceAndHostMemoryX<float> ocl_matrix_f_step1_m;

	//STEP 8:
	//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
	struct DeviceAndHostMemoryX<float> ocl_matrix_f_step2_m;

	//STEP 9:
	//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
	struct DeviceAndHostMemoryX<float> ocl_matrix_Vf_step1_m;

	//STEP 10:
	//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
	struct DeviceAndHostMemoryX<float> ocl_matrix_Vf_step2_m;

	//STEP 11:
	//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest
	struct DeviceAndHostMemoryX<int> ocl_Eval_counter;
	struct DeviceAndHostMemoryX<int> ocl_Eval_idxs;

	// -----------------------------------------------------------------------
	// ESTIMATE HG VARIABLES
	// -----------------------------------------------------------------------

	//STEP 1:
	//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
	struct DeviceAndHostMemoryX<float> ocl_matrix_K_OP_X_m;

	//STEP 2:
	//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
	struct DeviceAndHostMemoryX<float> ocl_matrix_f_OP_step1_m;

	//STEP 3:
	//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
	struct DeviceAndHostMemoryX<float> ocl_matrix_f_OP_step2_m;


	void allocData(size_t nTest, size_t nModel, size_t nOP)
	{
		
		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		ocl_matrix_K_star_X_m.allocData(nTest,nModel, _one_t);

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		ocl_matrix_K_X_star_m.allocData(nModel,nTest, _one_t);

		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		ocl_matrix_K_star_star_m.allocData(nTest,nTest, _one_t);

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		ocl_matrix_K_X_X_m.allocData(nModel,nModel,_one_t);

		//STEP 6:
		//Inverse_m = inverse_tmp.inverse(); nModelXnModel
		ocl_matrix_Inverse_m.allocData(nModel,nModel,_one_t);

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		ocl_matrix_f_step1_m.allocData(nTest,nModel,_one_t);

		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		ocl_matrix_f_step2_m.allocData(nTest,_one_t,_one_t);

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		ocl_matrix_Vf_step1_m.allocData(nTest,nTest,_one_t);

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		ocl_matrix_Vf_step2_m.allocData(nTest,nTest,_one_t);

		//STEP 11:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest
		ocl_Eval_counter.allocData(_one_t, _one_t, _one_t);
		ocl_Eval_idxs.allocData(nTest,_one_t,_one_t);

		//STEP 1:
		//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
		ocl_matrix_K_OP_X_m.allocData(nOP,nModel,_one_t);

		//STEP 2:
		//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
		ocl_matrix_f_OP_step1_m.allocData(nOP,nModel,_one_t);

		//STEP 3:
		//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
		ocl_matrix_f_OP_step2_m.allocData(nOP,_one_t,_one_t);

	}

	void bufferData()
	{
		
		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		ocl_matrix_K_star_X_m.bufferData();

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		ocl_matrix_K_X_star_m.bufferData();

		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		ocl_matrix_K_star_star_m.bufferData();

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		ocl_matrix_K_X_X_m.bufferData();

		//STEP 6:
		//Inverse_m = inverse_tmp.inverse(); nModelXnModel
		ocl_matrix_Inverse_m.bufferData();

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		ocl_matrix_f_step1_m.bufferData();

		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		ocl_matrix_f_step2_m.bufferData();

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		ocl_matrix_Vf_step1_m.bufferData();

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		ocl_matrix_Vf_step2_m.bufferData();

		//STEP 11:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest
		//ocl_Eval_counter.bufferData();
		//ocl_Eval_idxs.bufferData();

		//STEP 1:
		//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
		ocl_matrix_K_OP_X_m.bufferData();

		//STEP 2:
		//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
		ocl_matrix_f_OP_step1_m.bufferData();

		//STEP 3:
		//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
		ocl_matrix_f_OP_step2_m.bufferData();
	}

	void bufferAllocData()
	{

		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		ocl_matrix_K_star_X_m.bufferAllocData();

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		ocl_matrix_K_X_star_m.bufferAllocData();

		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		ocl_matrix_K_star_star_m.bufferAllocData();

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		ocl_matrix_K_X_X_m.bufferAllocData();

		//STEP 6:
		//Inverse_m = inverse_tmp.inverse(); nModelXnModel
		ocl_matrix_Inverse_m.bufferAllocData();

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		ocl_matrix_f_step1_m.bufferAllocData();

		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		ocl_matrix_f_step2_m.bufferAllocData();

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		ocl_matrix_Vf_step1_m.bufferAllocData();

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		ocl_matrix_Vf_step2_m.bufferAllocData();

		//STEP 11:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest
		//ocl_Eval_counter.bufferAllocData();
		//ocl_Eval_idxs.bufferAllocData();

		//STEP 1:
		//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
		ocl_matrix_K_OP_X_m.bufferAllocData();

		//STEP 2:
		//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
		ocl_matrix_f_OP_step1_m.bufferAllocData();

		//STEP 3:
		//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
		ocl_matrix_f_OP_step2_m.bufferAllocData();
	}

	void releaseDataDevice()
	{
		
		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		ocl_matrix_K_star_X_m.releaseDataDevice();

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		ocl_matrix_K_X_star_m.releaseDataDevice();

		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		ocl_matrix_K_star_star_m.releaseDataDevice();

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		ocl_matrix_K_X_X_m.releaseDataDevice();

		//STEP 6:
		//Inverse_m = inverse_tmp.inverse(); nModelXnModel
		ocl_matrix_Inverse_m.releaseDataDevice();

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		ocl_matrix_f_step1_m.releaseDataDevice();

		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		ocl_matrix_f_step2_m.releaseDataDevice();

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		ocl_matrix_Vf_step1_m.releaseDataDevice();

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		ocl_matrix_Vf_step2_m.releaseDataDevice();

		//STEP 11:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest
		//ocl_Eval_counter.releaseDataDevice();
		//ocl_Eval_idxs.releaseDataDevice();

		//STEP 1:
		//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
		ocl_matrix_K_OP_X_m.releaseDataDevice();

		//STEP 2:
		//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
		ocl_matrix_f_OP_step1_m.releaseDataDevice();

		//STEP 3:
		//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
		ocl_matrix_f_OP_step2_m.releaseDataDevice();

	}

	void releaseDataHost()
	{
		
		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		ocl_matrix_K_star_X_m.releaseDataHost();

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		ocl_matrix_K_X_star_m.releaseDataHost();

		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		ocl_matrix_K_star_star_m.releaseDataHost();

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		ocl_matrix_K_X_X_m.releaseDataHost();

		//STEP 6:
		//Inverse_m = inverse_tmp.inverse(); nModelXnModel
		ocl_matrix_Inverse_m.releaseDataHost();

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		ocl_matrix_f_step1_m.releaseDataHost();

		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		ocl_matrix_f_step2_m.releaseDataHost();

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		ocl_matrix_Vf_step1_m.releaseDataHost();

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		ocl_matrix_Vf_step2_m.releaseDataHost();

		//STEP 11:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest
		ocl_Eval_counter.releaseDataHost();
		ocl_Eval_idxs.releaseDataHost();

		//STEP 1:
		//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
		ocl_matrix_K_OP_X_m.releaseDataHost();

		//STEP 2:
		//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
		ocl_matrix_f_OP_step1_m.releaseDataHost();

		//STEP 3:
		//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
		ocl_matrix_f_OP_step2_m.releaseDataHost();

	}

	void clearData()
	{
		
		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		ocl_matrix_K_star_X_m.clearData();

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		ocl_matrix_K_X_star_m.clearData();

		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		ocl_matrix_K_star_star_m.clearData();

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		ocl_matrix_K_X_X_m.clearData();

		//STEP 6:
		//Inverse_m = inverse_tmp.inverse(); nModelXnModel
		ocl_matrix_Inverse_m.clearData();

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		ocl_matrix_f_step1_m.clearData();

		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		ocl_matrix_f_step2_m.clearData();

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		ocl_matrix_Vf_step1_m.clearData();

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		ocl_matrix_Vf_step2_m.clearData();

		//STEP 11:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest
		ocl_Eval_counter.clearData();
		ocl_Eval_idxs.clearData();

		//STEP 1:
		//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
		ocl_matrix_K_OP_X_m.clearData();

		//STEP 2:
		//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
		ocl_matrix_f_OP_step1_m.clearData();

		//STEP 3:
		//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
		ocl_matrix_f_OP_step2_m.clearData();

	}

	void MapData()
	{

		//STEP 1:
		//K_star_X_m = Covariance(Xstar_vec, X_vec); nTestXnModel
		ocl_matrix_K_star_X_m.MapData(_zero_t);

		//STEP 2:
		//K_X_star_m = Covariance(X_vec, Xstar_vec); nModelXnTest
		ocl_matrix_K_X_star_m.MapData(_zero_t);

		//STEP 3:
		//K_star_star_m = Covariance(Xstar_vec, Xstar_vec); nTestXnTest
		ocl_matrix_K_star_star_m.MapData(_zero_t);

		//STEP 4:
		//K_X_X_m = Covariance(X_vec, X_vec); nModelXnModel
		ocl_matrix_K_X_X_m.MapData(_zero_t);

		//STEP 6:
		//Inverse_m = inverse_tmp.inverse(); nModelXnModel
		ocl_matrix_Inverse_m.MapData(_zero_t);

		//STEP 7:
		//f_step1 = K_star_X_m * Inverse_m; nTestXnModel * nModelXnModel
		ocl_matrix_f_step1_m.MapData(_zero_t);

		//STEP 8:
		//f_step2 = f_step1 * Xz_vec; nTestXnModel * nModel
		ocl_matrix_f_step2_m.MapData(_zero_t);

		//STEP 9:
		//Vf_step1 = f_step1 * K_X_star_m; nTestXnModel * nModel*nTest
		ocl_matrix_Vf_step1_m.MapData(_zero_t);

		//STEP 10:
		//Vf_step2 = K_star_star_m - Vf_step1; nTestXnTest
		ocl_matrix_Vf_step2_m.MapData(_zero_t);

		//STEP 11:
		//(Vf_step2(zz,zz) < Tmodel) && (((Zi - f_step2[zz]) / (sqrt(sigma_n + Vf_step2(zz,zz))))); nTestXnTest
		//ocl_Eval_counter.MapData();
		//ocl_Eval_idxs.MapData();

		//STEP 1:
		//K_OP_X_m = Covariance(X_vec, X_vec); nOPXnModel
		ocl_matrix_K_OP_X_m.MapData(_zero_t);

		//STEP 2:
		//f_OP_step1 = K_OP_X_m * Inverse_m; nOPXnModel * nModelXnModel
		ocl_matrix_f_OP_step1_m.MapData(_zero_t);

		//STEP 3:
		//f_OP_step2 = f_OP_step1 * Xz_vec; nOPXnModel * nModel
		ocl_matrix_f_OP_step2_m.MapData(_zero_t);

	}

	public:

		EVAL_struct() {};

		~EVAL_struct() {};


};

struct PGi_struct {

	size_t PGi_dim_t;
	int PGi_dim_i;
	struct DeviceAndHostMemoryX<float> X;
	struct DeviceAndHostMemoryX<float> Y;
	struct DeviceAndHostMemoryX<float> Z;
	struct DeviceAndHostMemoryX<float> OP;
	struct DeviceAndHostMemoryX<int> Ground;
	struct DeviceAndHostMemoryX<float> Model;
	struct DeviceAndHostMemoryX<int> Bins;
	struct DeviceAndHostMemoryX<int> Bins_count;
	struct DeviceAndHostMemoryX<int> Original_Idx;

	PGi_struct() {};

	~PGi_struct() {};

};

struct Bins_struct {

	size_t Bins_dim_t;
	int Bins_dim_i;
	struct DeviceAndHostMemoryX<float> X;
	struct DeviceAndHostMemoryX<float> Y;
	struct DeviceAndHostMemoryX<float> Z;
	struct DeviceAndHostMemoryX<float> OP;
	struct DeviceAndHostMemoryX<int> Ground;
	struct DeviceAndHostMemoryX<float> Model;
	struct DeviceAndHostMemoryX<int> Sectors;
	struct DeviceAndHostMemoryX<int> Sectors_count;
	struct DeviceAndHostMemoryX<int> Original_Idx;

	Bins_struct() {};

	~Bins_struct() {};

};

struct CAM_struct {

	Eigen::MatrixXf CAM;

	CAM_struct() {};

	~CAM_struct() {};

};

struct Calib_struct {

	Eigen::MatrixXf cornerdist;

	struct CAM_struct S[101];
	struct CAM_struct K[101];
	struct CAM_struct D[101];
	struct CAM_struct R[101];
	struct CAM_struct T[101];
	struct CAM_struct S_rect[101];
	struct CAM_struct R_rect[101];
	struct CAM_struct P_rect[101];

	Calib_struct() {};

	~Calib_struct() {};

};

struct registerTime_struct
{
	double start = 0;
	double finish = 0;
	double elapsed = 0;
	int count = 0;

	registerTime_struct() {};

	~registerTime_struct() {};

};

struct OPENCLOBJECT
{
public:
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_command_queue queue_multiple_kernels;
	size_t alignmentForPtr;

	cl_command_queue_properties queue_properties_multiple_kernels[3];
	cl_command_queue_properties queue_properties;
	std::unique_ptr<cl_context_properties> additional_context_props;

	cl_program all_program;

	struct my_kernels
	{
		cl_kernel PolarGrid_getOPSectorsAndBins;
		cl_kernel Seed_copySp;
		cl_kernel Seed_SpUSnew_Step1;
		cl_kernel Seed_SpUSnew_Step2;
		cl_kernel Gauss_SquareExponentialKernel;
		cl_kernel Gauss_SquareExponentialKernelNoise;
		cl_kernel Test_getTest_Step1;
		cl_kernel Test_getTest_Step2;
		
		cl_kernel Eval_matrixCopy;
		cl_kernel Eval_matrixVector;
		cl_kernel Eval_matrixSubtraction;
		cl_kernel GEMM_matrixMultiply;

		cl_kernel matrixInverse_sGEMM;
		cl_kernel matrixInverse_sTRSM;
		cl_kernel matrixInverse_sSYRK;
		

	} my_kernels;
	
};

struct GPUOPTIONS {

public:

	std::string platform;
	std::string type;
	std::string device;
	std::string arithmetic;

	std::string PolarGrid_getOPSectorsAndBins;

	std::string Seed_copySp;
	std::string Seed_SpUSnew_Step1;
	std::string Seed_SpUSnew_Step2;
	std::string Seed_generateSp;
	
	std::string Gauss_SquareExponentialKernel;
	std::string Gauss_SquareExponentialKernelNoise;

	std::string Test_getTest_Step1;
	std::string Test_getTest_Step2;

	std::string Eval_matrixCopy;
	std::string Eval_matrixMultiply;
	std::string Eval_matrixVector;
	std::string Eval_matrixSubtraction;

	std::string GEMM_matrixMultiply;

	std::string matrixInverse_sGEMM;
	std::string matrixInverse_sTRSM;
	std::string matrixInverse_sSYRK;
	
	std::wstring all_kernelsFILE;

	size_t param_DEVICE_NAME_LENGTH;
	char param_DEVICE_NAME;
	size_t param_MAX_WORK_GROUP_SIZE;
	cl_uint param_MAX_WORK_ITEM_DIMENSIONS;
	size_t param_MAX_WORK_ITEM_SIZES[3];
	size_t param_MIN_WORK_GROUP_SIZE;
	size_t param_PREFERRED_WORK_GROUP_SIZE_MULTIPLE;
	size_t param_SMALLER_MIN_WORK_GROUP_SIZE;
	size_t param_DEVICE_OPENCL_C_VERSION_LENGTH;
	
	std::unique_ptr<char[]> param_DEVICE_OPENCL_C_VERSION;
	std::unique_ptr<char[]> param_CL_C_VERSION_2_0;

	int TILE_GROUP_M;
	int TILE_GROUP_N;
	int TILE_SIZE_M;
	int TILE_SIZE_N;
	int TILE_SIZE_K;

	cl_uint Work_Dim;

};

struct VELODYNEOPTIONS {

public:

	float M; //Total Number of Sectors
	size_t M_size;
	float N; //Total Number of Bins per Sector
	size_t N_size;
	float R; //Maximum value of Radius around 3D LIDAR Laser

	float PI; //PI number

	float Rstep; //Rstep = R/N;

	int NCoordinates = 3;
	int CAM = 2;

	int Sectors_Front_Left = 40;
	int Sectors_Front_Right = 320;

	bool doOnlyFrontView;

	struct Calib_struct _CALIB;

	Eigen::MatrixXf XYZT_prj;

	Eigen::MatrixXf P_Velo_to_Img;
	Eigen::MatrixXf Tr_velo_to_cam;
	Eigen::MatrixXf R_cam_to_rect;

};

struct MODELOPTIONS {

public:

	//3D LIDAR GROUND SEGMENTATION MODEL PARAMETERS

	float length_scale;
	float sigma_f;
	float sigma_n;

	float Z_threshold;
	float Hg_threshold;
	float TData;
	float TModel;
	float Tg;

	bool setManualMinZ;
	float manualMinZ;

};

struct DATAOPTIONS {

public:

	std::string folderInput; //INPUT FOLDERS FOR BINARY FILES
	std::string folderOutput; //OUTPUT FOLDERS FOR BINARY FILES
	std::string folderPictures; //INPUT FOLDERS FOR PICTURE FILES 

	std::string folderCamCalibration;  

	std::unique_ptr<char[]> CamToCamCalibrationFile;

	std::unique_ptr<char[]> VeloToCamCalibrationFile;

	std::string fileRunning; //FILE BEING PROCESSED AT THE MOMENT
	std::string fileName; //NAME OF FILE BEING PROCESSED AT THE MOMENT

	bool oneFile; // true -> 1 File will be processed, false -> the whole folder will be processed

	int maxNumberOfFiles;

};

struct DATA  {

public:

	int iFile; //the file being processed
	int iS;
	int iG;
	int iB;

	bool gaussLoopStart; //starting Gauss Loop

	size_t dim_pad[3];

	size_t _zero_t;
	size_t _one_t;
	int _zero_i;
	int _one_i;
	float _zero_f;
	float _one_f;
	float _m_one_f;
	float inf;
	bool _false;
	float _sign_pos;
	float _sign_neg;
	float _invert_sign;

	float min_Z;

	size_t nModel_t = 0;
	size_t nTest_t = 0;
	size_t nOP_t = 0;

	int nModel_i = 0;
	int nTest_i = 0;
	int nOP_i = 0;

	//3D LIDAR GRID POINTS
	size_t PG_dim_t = 0;
	int PG_dim_i = 0;

	//SECTOR POINTS
	size_t PGi_dim_t = 0;
	int PGi_dim_i = 0;

	//SEED POINTS EVALUATED INSIDE GAUSS REGRESSION
	size_t Sp_dim_t = 0;
	int Sp_dim_i = 0;

	size_t Snew_dim_t = 0;
	int Snew_dim_i = 0;

	//TEST POINTS
	size_t Test_dim_t = 0;
	int Test_dim_i = 0;

	//TEST POINTS
	size_t Test_dim_tmp_t = 0;
	int Test_dim_tmp_i = 0;

	//OPENCL OPTIONS
	size_t global_size[3];
	size_t local_size[3];

	size_t maximum_matrix_column_size = 400;

	//Triangular Matrix
	int Matrix_Triangular_Upper = 1;
	int Matrix_Triangular_Lower = 0;

	//Transpose Matrix
	int Matrix_Transpose = 1;
	int Matrix_No_Transpose = 0;

	//Matrix Size
	int Matrix_Side_Left = 1;
	int Matrix_Side_Right = 0;

	//Matrix Diagonal
	int Matrix_Unit_Triangular = 1;
	int Matrix_No_Unit_Triangular = 0;

	//Matrix Swap
	int Matrix_Swap_Columns = 0;
	int Matrix_Swap_Rows = 1;
	int Matrix_Swap_Vector = 2;


};

struct LOGINFOOPTIONS {

public:

	bool parsed_OK;

	bool verbose;
	bool showReport;
	bool saveReport;
	bool doSanityCheck;

	bool saveResult;
	bool savePicture;
	bool saveMovie;
	bool plotPicture;

	int timeBtwPictures;

	int SanityCheckSector;
	bool doSanityCheckSector;

	int SanityCheckBin;
	bool doSanityCheckBin;

	bool doSanityCheckDim;

	bool doSaveParameters;

	std::string pictureMode_BIN = "BIN";
	std::string pictureMode_SECTOR = "SECTOR";
	std::string pictureMode_COMPLETE = "COMPLETE";

	bool showDeviceInfo;

	std::string START = "start";
	std::string STOP = "stop";

	std::unique_ptr<float[]> RGB_R;
	std::unique_ptr<float[]> RGB_G;
	std::unique_ptr<float[]> RGB_B;

	int picture_Width = 2500;
	int picture_Height = 800;
	
	float fps;

	cv::VideoWriter * videoWriter;

	struct codeSteps {

		std::string RunModelLoop = "Run Model Loop";
		std::string SectorsLoop = "Sectors Loop";
		std::string GaussLoop = "Gauss Loop";

		std::string PolarGridFunction = "PolarGridFunction";
		std::string sectorFunction = "sectorFunction";
		std::string getMinZFunction = "getMinZFunction";
		std::string countSeedFunction = "countSeedFunction";
		std::string extractSeedFunction = "extractSeedFunction";
		std::string groupSpUSnewFunction = "groupSpUSnewFunction";
		std::string GetTestFunction = "GetTestFunction";
		std::string EvalFunction = "EvalFunction";
		std::string Eval_PART1_STEPS1234 = "Eval_PART1_STEPS1234";
		std::string Eval_PART2_STEPS56 = "Eval_PART2_STEPS56";
		std::string Eval_PART3_STEP7 = "Eval_PART3_STEP7";
		std::string Eval_PART4_STEP8 = "Eval_PART4_STEP8";
		std::string Eval_PART5_STEP9 = "Eval_PART5_STEP9";
		std::string Eval_PART6_STEP10 = "Eval_PART6_STEP10";
		std::string Eval_PART7A = "Eval_PART7A";
		std::string Eval_PART7B = "Eval_PART7B";
		std::string Segment_getHg = "Segment_getHg";
		std::string Segment_doSegment = "Segment_doSegment";

		std::string PlotFunction = "PlotFunction";


	} codeSteps;

	struct registerTime_struct cputime_RunModelLoop;
	struct registerTime_struct cputime_SectorsLoop;
	struct registerTime_struct cputime_GaussLoop;
	struct registerTime_struct cputime_SegmentLoop;

	struct registerTime_struct cputime_PolarGridFunction;
	struct registerTime_struct cputime_sectorFunction;
	struct registerTime_struct cputime_getMinZFunction;
	struct registerTime_struct cputime_countSeedFunction;
	struct registerTime_struct cputime_extractSeedFunction;
	struct registerTime_struct cputime_groupSpUSnewFunction;
	struct registerTime_struct cputime_GetTestFunction;
	struct registerTime_struct cputime_Eval_PART1_STEPS1234;
	struct registerTime_struct cputime_Eval_PART2_STEPS56;
	struct registerTime_struct cputime_Eval_PART3_STEP7;
	struct registerTime_struct cputime_Eval_PART4_STEP8;
	struct registerTime_struct cputime_Eval_PART5_STEP9;
	struct registerTime_struct cputime_Eval_PART6_STEP10;
	struct registerTime_struct cputime_Eval_PART7A;
	struct registerTime_struct cputime_Eval_PART7B;
	struct registerTime_struct cputime_Segment_getHg;
	struct registerTime_struct cputime_Segment_doSegment;


};

extern struct OPENCLOBJECT _OPENCLOBJECT;
extern struct GPUOPTIONS _GPUOPTIONS;
extern struct VELODYNEOPTIONS _VELODYNEOPTIONS;
extern struct MODELOPTIONS _MODELOPTIONS;
extern struct DATAOPTIONS _DATAOPTIONS;
extern struct DATA _DATA;
extern struct LOGINFOOPTIONS _LOGINFOOPTIONS;


//////////////////////////////////////////////////////////////////////////////////////////////
// LOG INFORMATION ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void showReport();

void clearTime();

std::string showReportStep(std::string codeStep, double elapsed, int nRepetitions);

void RegisterEvent(std::string event, std::string codeStep);

void RegisterStep(std::string event, bool verbose, std::string msg, int iStep);

//////////////////////////////////////////////////////////////////////////////////////////////
// SANITY CHECK //////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void SaveSanityCheckSector(bool shouldDo, std::string codeStep, EVAL_struct & _eval, Sectors_struct & ocl_OP, size_t nModel, size_t nTest, size_t nOP, int inliers[]);
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, float & number);
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361], int inliers[]);
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, struct Sectors_struct & ocl_Struct, size_t dim);
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, float host[], size_t dim_X, size_t dim_Y);
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, int ground[], float Z[], int Bin[], int idx[], size_t dim_X, size_t dim_Y, int iSector);
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, int & _S, float & _X, float & _Y, float & _Z, float & R, float & G, float & B, int & idx);
void SaveSanityCheckSector(bool shouldDo, std::string codeStep, float X[], float Y[], float Z[], int Bins[], int original_idx[], size_t dim, int Sector);

void SanityCheckSector(bool shouldDo, std::string codeStep, float Vf_step2, float TModel, bool Vf_test, float Test_Z, float f_step2, float sigma_n, float TData, bool f_test, int Sector, int iZ);

void SaveSanityCheckBin(bool shouldDo, std::string codeStep, float X[], float Y[], float Z[], int Sectors[], int original_idx[], size_t dim, int Bin);
void SaveSanityCheckBin(bool shouldDo, std::string codeStep, int ground[], float Z[], int Sector[], int idx[], size_t dim_X, size_t dim_Y, int iB);

void SaveSanityCheck(bool shouldDo, std::string codeStep, float host[], size_t dim_X, size_t dim_Y);
void SaveSanityCheck(bool shouldDo, std::string codeStep, float data);

void SaveSanityCheckDims(bool shouldDo, std::string codeStep, std::string event);

void SaveSanityCheck(bool shouldDo, std::string codeStep, Eigen::MatrixXf matrix, size_t dim_X, size_t dim_Y);

template <typename T>
void saveMatrix(std::string fullfileName, DeviceAndHostMemoryX<T> & host, size_t dim_X, size_t dim_Y);

void saveMatrix(std::string fullfileName, float host[], size_t dim_X, size_t dim_Y);

void saveMatrix(std::string fullfileName, Eigen::MatrixXf matrix, size_t dim_X, size_t dim_Y);

void saveMatrix(std::string fullfileName, int host[], size_t dim_X, size_t dim_Y);

void saveSector(std::string fullfileName, Sectors_struct & sector, size_t dim_X);

void SaveParameters(bool shouldDo);

//////////////////////////////////////////////////////////////////////////////////////////////
// OPENCL OBJECTS ////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

std::string get_build_options(std::string arithmetic, int TILE_GROUP_M, int TILE_SIZE_M, int TILE_GROUP_N, int TILE_SIZE_N, int TILE_SIZE_K);

void getGWWIandADDPAD(size_t dim_X, size_t dim_Y, size_t dim_Z);

void getGEMMparams(int dim_X, int dim_Y, int dim_K);

void DeviceInfo();

void startGPU();

void startOpenCL();

void stopGPU();

void releaseOpenCL();

void defineProgramsAndKernels();

void releaseProgramsAndKernels();

void showDeviceInfo();

//////////////////////////////////////////////////////////////////////////////////////////////
// CAM CALIBRATION ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void loadCalibrationCamToCam();

void loadCalibrationRigid();

void projectionMatrixVeloToImg();

Eigen::MatrixXf readVariable(FILE* fid, std::string name, size_t M, size_t N);

//////////////////////////////////////////////////////////////////////////////////////////////
// MODEL PARAMETERS //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void setParameters();

void parseParameters(int argc, const char** argv);

void checkCommonOptionsParameters(std::string arg_name, std::string arg_data);

float getMean(float host[], size_t dim);

void removeNAN(DeviceAndHostMemoryX<float> & matrix, size_t dim);

float getMeanAbs(float host[], size_t dim);

float getKurtosis(float host[], float avg, float std, size_t dim);

float getStd(float host[], float mean, size_t dim);

float getPeakDensity(float host[], float mean, float std, size_t dim);

void quicksort(DeviceAndHostMemoryX<float> & matrix, int lo, int hi);

int partition(DeviceAndHostMemoryX<float> & matrix, int lo, int hi);

float getMinBin(struct DeviceAndHostMemoryX<float> & Bin, size_t dim);

void removeOutlier(struct DeviceAndHostMemoryX<float> & Bin, float mean, float std, size_t dim);

//////////////////////////////////////////////////////////////////////////////////////////////
// RELEASE MEMORY OBJECTS ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void releaseLogInfo();

void clearVariables();

void aligned_free(void *aligned);

void* aligned_malloc(size_t size, size_t alignment);

size_t zeroCopySizeAlignment(size_t requiredSize, cl_device_id device);

cl_uint zeroCopyPtrAlignment(cl_device_id device);

//////////////////////////////////////////////////////////////////////////////////////////////
// OPENCL BASIC FUNCTIONS ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

cl_platform_id selectPlatform(const string& platform_name_or_index);

std::vector<cl_device_id> selectDevices(cl_platform_id platform, const string& device_type);

cl_device_id selectDevice(const string& device_name_or_index, const string& device_type_name);

cl_device_type parseDeviceType(const string& device_type_name);

cl_context createContext(const cl_context_properties* additional_context_props);

cl_command_queue createQueue(cl_command_queue_properties queue_properties = 0);

cl_command_queue createQueueWithProperties(cl_command_queue_properties queue_properties[]);

void readProgramFile(const std::wstring& program_file_name, std::vector<char>& program_text_prepared);

void readFile(const std::wstring& file_name, std::vector<char>& Data);

cl_program createAndBuildProgram(const std::vector<char>& program_text_prepared, cl_context& context, size_t num_of_devices, const cl_device_id* devices, const string& build_options);

cl_program OpenCLProgram(const std::wstring& program_file_name, const string& program_text, const string& build_options);
