#pragma once
#pragma warning (disable: 4018)

#ifdef _WIN64
typedef __int64 streamsize;
#else
typedef int streamsize;
#endif

#include "Globals.h"
#include "PolarGridMap.h"

using namespace std;

//LOAD THE POLAR GRID 3D Data POINTS FROM BINARY FILE
void PolarGridMap(struct DeviceAndHostMemoryX<int> & ocl_PG_Ground, struct PGi_struct(&PGi)[361], struct Bins_struct(&Bins)[161], struct DeviceAndHostMemoryX<float> & ocl_PG_X, struct DeviceAndHostMemoryX<float> & ocl_PG_Y, struct DeviceAndHostMemoryX<float> & ocl_PG_Z, struct DeviceAndHostMemoryX<int> & ocl_PG_Sector, struct DeviceAndHostMemoryX<int> & ocl_PG_Bins)
{
	RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.PolarGridFunction);

	//------------------------------------------------------------------------

	cl_int err = 0;

	// -----------------------------------------------------------------------
	// ALLOCATING DEVICES-SIDE RESOURCES
	// -----------------------------------------------------------------------
	
	std::unique_ptr<std::ifstream> file(new std::ifstream(_DATAOPTIONS.fileRunning.c_str(), std::ios::binary));

	std::streampos fsize = 0;
	fsize = file->tellg();
	file->seekg(0, std::ios::end);
	fsize = file->tellg() - fsize;
	file->close();
	file.reset();
	size_t fileBytes = (size_t)fsize/sizeof(float);
	
	// allocate buffer
	std::unique_ptr<float[]> fileData (new float[fileBytes * sizeof(float)]);
	int nElements = 4;

	_DATA.PG_dim_t = (size_t)fileBytes / nElements;
	_DATA.PG_dim_i = static_cast<int>(_DATA.PG_dim_t);

	struct DeviceAndHostMemoryX<float> ocl_PG_OP;
	struct DeviceAndHostMemoryX<int> ocl_PG_original_idx;
	struct DeviceAndHostMemoryX<int> ocl_PG_Bins_count;
	struct DeviceAndHostMemoryX<int> ocl_PG_Sectors_count;
	struct DeviceAndHostMemoryX<float> ocl_PG_cos;
	struct DeviceAndHostMemoryX<float> ocl_PG_acos;
	struct DeviceAndHostMemoryX<float> ocl_PG_ret;
	struct DeviceAndHostMemoryX<float> ocl_PG_x;

	ocl_PG_X.allocData(_DATA.PG_dim_t,_DATA._one_t,_DATA._one_t);
	ocl_PG_Y.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_Z.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_Ground.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_OP.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_original_idx.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_Bins.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_Bins_count.allocData(_VELODYNEOPTIONS.N_size + _DATA._one_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_Sector.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_Sectors_count.allocData(_VELODYNEOPTIONS.M_size + _DATA._one_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_cos.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_acos.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_ret.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	ocl_PG_x.allocData(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);
	
	// -----------------------------------------------------------------------
	// LOAD BINARY DATA FROM 3D LIDAR VELODYNE LASER
	// -----------------------------------------------------------------------

	std::unique_ptr<char[]> fileName (new char[_DATAOPTIONS.fileRunning.length() + 1]);
	strcpy(&fileName[0], _DATAOPTIONS.fileRunning.c_str());

	// load point cloud
	std::unique_ptr<FILE, int(*)(FILE*)> stream ( std::fopen(&fileName[0], "rb"), &fclose );
	fread(&fileData[0], sizeof(float), fileBytes, stream.get());

	for (int j = 0; j < static_cast<int>(fileBytes/nElements); j++) {
		
		float _X = fileData[0 + (j * nElements)];
		float _Y = fileData[1 + (j * nElements)];
		float _Z = fileData[2 + (j * nElements)];

		ocl_PG_X.host[j] = _X;
		ocl_PG_Y.host[j] = _Y;
		ocl_PG_Z.host[j] = _Z;

		ocl_PG_original_idx.host[j] = j;

	}

	std::fclose(stream.get());

	// -----------------------------------------------------------------------
	// WRITE BUFFERS
	// -----------------------------------------------------------------------

	ocl_PG_X.bufferData();
	ocl_PG_Y.bufferData();
	ocl_PG_OP.bufferData();
	ocl_PG_Sector.bufferData();
	ocl_PG_Bins.bufferData();
	ocl_PG_cos.bufferData();
	ocl_PG_acos.bufferData();
	ocl_PG_ret.bufferData();
	ocl_PG_x.bufferData();

	// -----------------------------------------------------------------------
	// SETTING KERNELS ARGUMENTS
	// -----------------------------------------------------------------------

	//STEP: PolarGrid_getGetOPSectorsAndBins
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 0, sizeof(cl_mem), &ocl_PG_X.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 1, sizeof(cl_mem), &ocl_PG_Y.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 2, sizeof(cl_mem), &ocl_PG_OP.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 3, sizeof(cl_mem), &ocl_PG_Sector.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 4, sizeof(cl_mem), &ocl_PG_Bins.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 5, sizeof(float), &_VELODYNEOPTIONS.PI);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 6, sizeof(float), &_VELODYNEOPTIONS.M);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 7, sizeof(float), &_VELODYNEOPTIONS.R);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 8, sizeof(float), &_VELODYNEOPTIONS.Rstep);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 9, sizeof(cl_mem), &ocl_PG_cos.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 10, sizeof(cl_mem), &ocl_PG_acos.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 11, sizeof(cl_mem), &ocl_PG_ret.device);
	SAMPLE_CHECK_ERRORS(err);
	err = clSetKernelArg(_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins, 12, sizeof(cl_mem), &ocl_PG_x.device);
	SAMPLE_CHECK_ERRORS(err);

	// -----------------------------------------------------------------------
	// RUNNING KERNELS
	// -----------------------------------------------------------------------

	getGWWIandADDPAD(_DATA.PG_dim_t, _DATA._one_t, _DATA._one_t);

	//STEP: PolarGrid_getOPSectorsAndBins
	err = clEnqueueNDRangeKernel(
		_OPENCLOBJECT.queue,
		_OPENCLOBJECT.my_kernels.PolarGrid_getOPSectorsAndBins,
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
	// READ BUFFERS
	// -----------------------------------------------------------------------

	ocl_PG_OP.MapData(_DATA._zero_t);
	ocl_PG_Sector.MapData(_DATA._zero_t);
	ocl_PG_Bins.MapData(_DATA._zero_t);

	// -----------------------------------------------------------------------
	// COUNT SECTORS
	// -----------------------------------------------------------------------

	for (size_t i = 0; i < _DATA.PG_dim_t; i++)
	{
		int idx_Sector = ocl_PG_Sector.host[i];
		int idx_Bin = ocl_PG_Bins.host[i];

		int count_Sector = 1 + ocl_PG_Sectors_count.host[idx_Sector];
		int count_Bin = 1 + ocl_PG_Bins_count.host[idx_Bin];

		ocl_PG_Sectors_count.host[idx_Sector] = count_Sector;
		ocl_PG_Bins_count.host[idx_Bin] = count_Bin;

	}

	// -----------------------------------------------------------------------
	// EXTRACT SECTORS
	// -----------------------------------------------------------------------

	for (size_t i = 0; i <= _VELODYNEOPTIONS.M_size; i++)
	{
		PGi[i].PGi_dim_t = static_cast<size_t>(ocl_PG_Sectors_count.host[i]);
		PGi[i].PGi_dim_i = ocl_PG_Sectors_count.host[i];

		PGi[i].X.allocData(PGi[i].PGi_dim_t,_DATA._one_t,_DATA._one_t);
		PGi[i].Y.allocData(PGi[i].PGi_dim_t, _DATA._one_t, _DATA._one_t);
		PGi[i].Z.allocData(PGi[i].PGi_dim_t, _DATA._one_t, _DATA._one_t);
		PGi[i].OP.allocData(PGi[i].PGi_dim_t, _DATA._one_t, _DATA._one_t);
		PGi[i].Ground.allocData(PGi[i].PGi_dim_t, _DATA._one_t, _DATA._one_t);
		PGi[i].Model.allocData(PGi[i].PGi_dim_t, _DATA._one_t, _DATA._one_t);
		PGi[i].Bins.allocData(PGi[i].PGi_dim_t, _DATA._one_t, _DATA._one_t);
		PGi[i].Original_Idx.allocData(PGi[i].PGi_dim_t, _DATA._one_t, _DATA._one_t);

		PGi[i].Bins_count.allocData(_VELODYNEOPTIONS.N_size + _DATA._one_t, _DATA._one_t, _DATA._one_t);
	}

	for (size_t i = 0; i <= _VELODYNEOPTIONS.N_size; i++)
	{
		Bins[i].Bins_dim_t = static_cast<size_t>(ocl_PG_Bins_count.host[i]);
		Bins[i].Bins_dim_i = ocl_PG_Bins_count.host[i];

		if (Bins[i].Bins_dim_t > _DATA._zero_t)
		{
			Bins[i].X.allocData(Bins[i].Bins_dim_t, _DATA._one_t, _DATA._one_t);
			Bins[i].Y.allocData(Bins[i].Bins_dim_t, _DATA._one_t, _DATA._one_t);
			Bins[i].Z.allocData(Bins[i].Bins_dim_t, _DATA._one_t, _DATA._one_t);
			Bins[i].OP.allocData(Bins[i].Bins_dim_t, _DATA._one_t, _DATA._one_t);
			Bins[i].Ground.allocData(Bins[i].Bins_dim_t, _DATA._one_t, _DATA._one_t);
			Bins[i].Model.allocData(Bins[i].Bins_dim_t, _DATA._one_t, _DATA._one_t);
			Bins[i].Sectors.allocData(Bins[i].Bins_dim_t, _DATA._one_t, _DATA._one_t);
			Bins[i].Original_Idx.allocData(Bins[i].Bins_dim_t, _DATA._one_t, _DATA._one_t);

			Bins[i].Sectors_count.allocData(_VELODYNEOPTIONS.M_size + _DATA._one_t, _DATA._one_t, _DATA._one_t);
		}
	}
	
	std::unique_ptr<int[]> counter_N(new int[_VELODYNEOPTIONS.N_size + _DATA._one_t]);
	for (size_t i = 0; i <= _VELODYNEOPTIONS.N_size; i++)
	{
		counter_N[i] = _DATA._zero_i;
	}

	std::unique_ptr<int[]> counter_M(new int[_VELODYNEOPTIONS.M_size + _DATA._one_t]);
	for (size_t i = 0; i <= _VELODYNEOPTIONS.M_size; i++)
	{
		counter_M[i] = _DATA._zero_i;
	}

	for (size_t i = 0; i < _DATA.PG_dim_t; i++)
	{
		int iSector = ocl_PG_Sector.host[i];
		int iBin = ocl_PG_Bins.host[i];

		int count_M = counter_M[iSector];
		int count_N = counter_N[iBin];

		float X = ocl_PG_X.host[i];
		float Y = ocl_PG_Y.host[i];
		float Z = ocl_PG_Z.host[i];
		float OP = ocl_PG_OP.host[i];

		int bin = ocl_PG_Bins.host[i];
		int sector = ocl_PG_Sector.host[i];
		int idx = ocl_PG_original_idx.host[i];

		PGi[iSector].X.host[count_M] = X;
		PGi[iSector].Y.host[count_M] = Y;
		PGi[iSector].Z.host[count_M] = Z;
		PGi[iSector].OP.host[count_M] = OP;
		PGi[iSector].Bins.host[count_M] = bin;
		PGi[iSector].Original_Idx.host[count_M] = idx;
		
		PGi[iSector].Bins_count.host[bin] = PGi[iSector].Bins_count.host[bin] + _DATA._one_i;

		counter_M[iSector] = counter_M[iSector] + _DATA._one_i;

		Bins[iBin].X.host[count_N] = X;
		Bins[iBin].Y.host[count_N] = Y;
		Bins[iBin].Z.host[count_N] = Z;
		Bins[iBin].OP.host[count_N] = OP;
		Bins[iBin].Sectors.host[count_N] = sector;
		Bins[iBin].Original_Idx.host[count_N] = idx;

		Bins[iBin].Sectors_count.host[sector] = Bins[iBin].Sectors_count.host[sector] + _DATA._one_i;

		counter_N[iBin] = counter_N[iBin] + _DATA._one_i;

	}
	/**
	if (_LOGINFOOPTIONS.doSanityCheck)
	{
		if (_LOGINFOOPTIONS.doSanityCheckBin)
		{

			if (_LOGINFOOPTIONS.SanityCheckBin != _DATA._zero_i)
			{
				int Bin = _LOGINFOOPTIONS.SanityCheckBin;
				SaveSanityCheckBin(_LOGINFOOPTIONS.doSanityCheckBin, _LOGINFOOPTIONS.codeSteps.PolarGridFunction, Bins[Bin].X.host, Bins[Bin].Y.host, Bins[Bin].Z.host, Bins[Bin].Sectors.host, Bins[Bin].Original_Idx.host, Bins[Bin].Bins_dim_t, Bin);
			}
			else
			{
				for (int Bin = 0; Bin <= static_cast<int>(_VELODYNEOPTIONS.N); Bin++)
				{
					SaveSanityCheckBin(_LOGINFOOPTIONS.doSanityCheckBin, _LOGINFOOPTIONS.codeSteps.PolarGridFunction, Bins[Bin].X.host, Bins[Bin].Y.host, Bins[Bin].Z.host, Bins[Bin].Sectors.host, Bins[Bin].Original_Idx.host, Bins[Bin].Bins_dim_t, Bin);
				}
			}
		}
		else if (_LOGINFOOPTIONS.doSanityCheckSector)
		{
			if (_LOGINFOOPTIONS.SanityCheckSector != _DATA._zero_i)
			{
				int Sector = _LOGINFOOPTIONS.SanityCheckSector;
				SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.PolarGridFunction, PGi[Sector].X.host, PGi[Sector].Y.host, PGi[Sector].Z.host, PGi[Sector].Bins.host, PGi[Sector].Original_Idx.host, PGi[Sector].PGi_dim_t, Sector);
			}
			else 
			{
				for (int Sector = 0; Sector <= static_cast<int>(_VELODYNEOPTIONS.M); Sector++)
				{
					SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.PolarGridFunction, PGi[Sector].X.host, PGi[Sector].Y.host, PGi[Sector].Z.host, PGi[Sector].Bins.host, PGi[Sector].Original_Idx.host, PGi[Sector].PGi_dim_t, Sector);
				}
			}
		}

	}
	**/
	
	counter_M.reset();
	counter_N.reset();

	// -----------------------------------------------------------------------
	// RELEASE LOADED DATA
	// -----------------------------------------------------------------------

	fileData.reset();
	fileName.reset();
	stream.reset();

	//------------------------------------------------------------------------
	
	RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.PolarGridFunction);

	//------------------------------------------------------------------------

	ocl_PG_X.releaseDataDevice();
	ocl_PG_Y.releaseDataDevice();
	ocl_PG_OP.releaseDataDevice();
	ocl_PG_Sector.releaseDataDevice();
	ocl_PG_Bins.releaseDataDevice();
	ocl_PG_cos.releaseDataDevice();
	ocl_PG_acos.releaseDataDevice();
	ocl_PG_ret.releaseDataDevice();
	ocl_PG_x.releaseDataDevice();

	//ocl_PG_X.releaseDataHost();
	//ocl_PG_Y.releaseDataHost();
	//ocl_PG_Z.releaseDataHost();
	//ocl_PG_Ground.releaseHost();
	ocl_PG_OP.releaseDataHost();
	ocl_PG_original_idx.releaseDataHost();
	//ocl_PG_Bins.releaseDataHost();
	ocl_PG_Bins_count.releaseDataHost();
	//ocl_PG_Sector.releaseDataHost();
	ocl_PG_Sectors_count.releaseDataHost();
	ocl_PG_cos.releaseDataHost();
	ocl_PG_acos.releaseDataHost();
	ocl_PG_ret.releaseDataHost();
	ocl_PG_x.releaseDataHost();


}

void getMinZ(std::string codeStep, size_t PG_dim, struct DeviceAndHostMemoryX<float> & ocl_PG_Z, struct DeviceAndHostMemoryX<int> & ocl_PG_Sector, float & min_Z)
{
	RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.getMinZFunction);

	//------------------------------------------------------------------------

	min_Z = std::numeric_limits<float>::infinity();

	struct DeviceAndHostMemoryX<float> selected;
	selected.allocData(PG_dim,_DATA._one_t,_DATA._one_t);

	size_t count = 0;

	for (int i = 0; i < PG_dim; i++)
	{
		int Sector = ocl_PG_Sector.host[i];

		if ((Sector >= _VELODYNEOPTIONS.Sectors_Front_Right) || (Sector <= _VELODYNEOPTIONS.Sectors_Front_Left))
		{
			float Z = ocl_PG_Z.host[i];
			
			/*if (min_Z > Z)
			{
				min_Z = Z;
			}*/

			selected.host[count] = Z;
			count++;
		}

	}

	float avg = getMean(selected.host, count);
	float std = getStd(selected.host, avg, count);

	removeOutlier(selected, avg, std, count);

	min_Z = getMean(selected.host, count);

	//------------------------------------------------------------------------

	RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.getMinZFunction);


}
