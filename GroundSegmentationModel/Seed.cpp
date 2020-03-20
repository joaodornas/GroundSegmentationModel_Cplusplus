#pragma once
#pragma warning (disable: 4018)

#include "Globals.h"
#include "Seed.h"

void Seed(struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361], struct DeviceAndHostMemoryX<int> & inliers_idx)
{
	RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.sectorFunction);

	//------------------------------------------------------------------------
	
	countSeed(ocl_PGi, ocl_Sp, PGi);

	if (_DATA.Sp_dim_t > _DATA._zero_t)
	{
		extractSeed(ocl_PGi, ocl_Sp, PGi, inliers_idx);
	}

	//------------------------------------------------------------------------

	RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.sectorFunction);

}


void groupSpUSnew(struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361], struct DeviceAndHostMemoryX<int> & inliers_idx)
{

	RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.groupSpUSnewFunction);

	//------------------------------------------------------------------------

	cl_int err = 0;

	// -----------------------------------------------------------------------
	// ALLOCATING DEVICES-SIDE RESOURCES
	// -----------------------------------------------------------------------

	ocl_Sp.allocData(_DATA.Sp_dim_t,_DATA._one_t,_DATA._one_t);

	// -----------------------------------------------------------------------
	// EXTRACT SEED
	// -----------------------------------------------------------------------

	int counter = 0;
	for (size_t i = 0; i < PGi[_DATA.iS].PGi_dim_t; i++)
	{

		int idx = inliers_idx.host[i];

		for (size_t j = 0; j < PGi[_DATA.iS].PGi_dim_t; j++)
		{

			int original_idx = ocl_PGi.ocl_original_idx.host[j];

			if (idx == original_idx)
			{
				float Z = ocl_PGi.ocl_Z.host[j];
				float OP = ocl_PGi.ocl_OP.host[j];
				int Bins = ocl_PGi.ocl_Bins.host[j];

				ocl_Sp.ocl_Z.host[counter] = Z;
				ocl_Sp.ocl_OP.host[counter] = OP;
				ocl_Sp.ocl_Bins.host[counter] = Bins;
				ocl_Sp.ocl_original_idx.host[counter] = original_idx;

				counter = counter + 1;
			}
		}

	}

	ocl_Sp.bufferData();

	//------------------------------------------------------------------------

	SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.groupSpUSnewFunction, ocl_PGi, ocl_Sp, PGi, inliers_idx.host);

	//------------------------------------------------------------------------

	RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.groupSpUSnewFunction);

}

//COUNT THE SEED 3D POINTS INSIDE A SECTOR
void countSeed(struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361])
{
	RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.countSeedFunction);

	//------------------------------------------------------------------------

	// -----------------------------------------------------------------------
	// COUNT SEED
	// -----------------------------------------------------------------------

	int counter = 0;

	for (size_t i = 0; i < PGi[_DATA.iS].PGi_dim_t; i++)
	{

		if (PGi[_DATA.iS].Z.host[i] <= _DATA.min_Z)
		{
			
			counter = counter + 1;

		}

	}

	_DATA.Sp_dim_t = static_cast<size_t>(counter);

	//------------------------------------------------------------------------

	float Sp_float = static_cast<float>(_DATA.Sp_dim_t);

	SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.countSeedFunction, Sp_float);
	
	//------------------------------------------------------------------------

	RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.countSeedFunction);

}

//EXTRACT THE SEED 3D POINTS INSIDE A SECTOR
void extractSeed(struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361], struct DeviceAndHostMemoryX<int> & inliers_idx)
{
	RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.extractSeedFunction);

	//------------------------------------------------------------------------

	cl_int err = 0;

	// -----------------------------------------------------------------------
	// ALLOCATING DEVICES-SIDE RESOURCES
	// -----------------------------------------------------------------------

	inliers_idx.allocData(PGi[_DATA.iS].PGi_dim_t,_DATA._one_t,_DATA._one_t);

	ocl_Sp.allocData(_DATA.Sp_dim_t,_DATA._one_t,_DATA._one_t);

	ocl_PGi.allocData(PGi[_DATA.iS].PGi_dim_t, _DATA._one_t, _DATA._one_t);

	// -----------------------------------------------------------------------
	// EXTRACT SEED
	// -----------------------------------------------------------------------

	int counter = 0;
	for (size_t i = 0; i < PGi[_DATA.iS].PGi_dim_t; i++)
	{

		float Z = PGi[_DATA.iS].Z.host[i];
		float OP = PGi[_DATA.iS].OP.host[i];
		int Bins = PGi[_DATA.iS].Bins.host[i];
		int idx = PGi[_DATA.iS].Original_Idx.host[i];

		if (PGi[_DATA.iS].Z.host[i] <= _DATA.min_Z)
		{

			ocl_Sp.ocl_Z.host[counter] = Z;
			ocl_Sp.ocl_OP.host[counter] = OP;
			ocl_Sp.ocl_Bins.host[counter] = Bins;
			ocl_Sp.ocl_original_idx.host[counter] = idx;

			inliers_idx.host[counter] = idx;

			counter = counter + 1;
		}

		ocl_PGi.ocl_Z.host[i] = Z;
		ocl_PGi.ocl_OP.host[i] = OP;
		ocl_PGi.ocl_Bins.host[i] = Bins;
		ocl_PGi.ocl_original_idx.host[i] = idx;

	}

	ocl_Sp.bufferData();

	//------------------------------------------------------------------------

	SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.extractSeedFunction, ocl_PGi, ocl_Sp, PGi, inliers_idx.host);

	//------------------------------------------------------------------------

	RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.extractSeedFunction);

}
