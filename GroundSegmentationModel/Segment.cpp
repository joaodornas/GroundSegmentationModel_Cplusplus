#pragma once

#include "Globals.h"
#include "Segment.h"

void doSegment(struct PGi_struct(&PGi)[361], struct Bins_struct(&Bins)[161], struct DeviceAndHostMemoryX<int> & ocl_PG_Ground, Eigen::MatrixXf & Model_Hg, struct DeviceAndHostMemoryX<int> & ocl_PG_Bins, struct DeviceAndHostMemoryX<int> & ocl_PG_Sector, struct DeviceAndHostMemoryX<float> & ocl_PG_Z)
{
	RegisterEvent(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.codeSteps.Segment_doSegment);

	//------------------------------------------------------------------------

	cl_int err = 0;

	// -----------------------------------------------------------------------
	// DO SEGMENT
	// -----------------------------------------------------------------------

	for (size_t i = 0; i < _DATA.PG_dim_t; i++)
	{
		int Bin = ocl_PG_Bins.host[i];
		int Sector = ocl_PG_Sector.host[i];

		float Z = ocl_PG_Z.host[i];

		if (std::abs(Z - Model_Hg(Bin,Sector)) <= _MODELOPTIONS.Tg)
		{
			ocl_PG_Ground.host[i] = _DATA._one_i;

		}
		else
		{
			ocl_PG_Ground.host[i] = _DATA._zero_i;
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

		int G = ocl_PG_Ground.host[i];
		
		PGi[iSector].Ground.host[count_M] = G;
		Bins[iBin].Ground.host[count_N] = G;

		counter_M[iSector] = counter_M[iSector] + _DATA._one_i;
		counter_N[iBin] = counter_N[iBin] + _DATA._one_i;

	}

	counter_M.reset();
	counter_N.reset();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SANITY CHECK ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (_LOGINFOOPTIONS.doSanityCheck)
	{

		if ((_LOGINFOOPTIONS.doSanityCheckSector) && (_LOGINFOOPTIONS.doSanityCheckBin))
		{
			if (_LOGINFOOPTIONS.SanityCheckBin == _DATA._zero_i)
			{
				for (int iBin = 1; iBin <= static_cast<int>(_VELODYNEOPTIONS.N_size); iBin++)
				{
					SaveSanityCheckBin(_LOGINFOOPTIONS.doSanityCheckBin, _LOGINFOOPTIONS.codeSteps.Segment_doSegment, Bins[iBin].Ground.host, Bins[iBin].Z.host, Bins[iBin].Sectors.host, Bins[iBin].Original_Idx.host, Bins[iBin].Bins_dim_t, _DATA._one_t, iBin);
				}

			}

			if (_LOGINFOOPTIONS.SanityCheckBin != _DATA._zero_i)
			{
				int iBin = _LOGINFOOPTIONS.SanityCheckBin;
				SaveSanityCheckBin(_LOGINFOOPTIONS.doSanityCheckBin, _LOGINFOOPTIONS.codeSteps.Segment_doSegment, Bins[iBin].Ground.host, Bins[iBin].Z.host, Bins[iBin].Sectors.host, Bins[iBin].Original_Idx.host, Bins[iBin].Bins_dim_t, _DATA._one_t, iBin);

			}

			if (_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i)
			{
				for (int iSector = 1; iSector <= static_cast<int>(_VELODYNEOPTIONS.M_size); iSector++)
				{
					SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.Segment_doSegment, PGi[iSector].Ground.host, PGi[iSector].Z.host, PGi[iSector].Bins.host, PGi[iSector].Original_Idx.host, PGi[iSector].PGi_dim_t, _DATA._one_t, iSector);
				}
			}

			if (_LOGINFOOPTIONS.SanityCheckSector != _DATA._zero_i)
			{
				int iSector = _LOGINFOOPTIONS.SanityCheckSector;
				SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.Segment_doSegment, PGi[iSector].Ground.host, PGi[iSector].Z.host, PGi[iSector].Bins.host, PGi[iSector].Original_Idx.host, PGi[iSector].PGi_dim_t, _DATA._one_t, iSector);
			}

		}
		else if (_LOGINFOOPTIONS.doSanityCheckSector)
		{
			if (_LOGINFOOPTIONS.SanityCheckSector == _DATA._zero_i)
			{
				for (int iSector = 1; iSector <= static_cast<int>(_VELODYNEOPTIONS.M_size); iSector++)
				{
					SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.Segment_doSegment, PGi[iSector].Ground.host, PGi[iSector].Z.host, PGi[iSector].Bins.host, PGi[iSector].Original_Idx.host, PGi[iSector].PGi_dim_t, _DATA._one_t, iSector);
				}
			}

			if (_LOGINFOOPTIONS.SanityCheckSector != _DATA._zero_i)
			{
				int iSector = _LOGINFOOPTIONS.SanityCheckSector;
				SaveSanityCheckSector(_LOGINFOOPTIONS.doSanityCheckSector, _LOGINFOOPTIONS.codeSteps.Segment_doSegment, PGi[iSector].Ground.host, PGi[iSector].Z.host, PGi[iSector].Bins.host, PGi[iSector].Original_Idx.host, PGi[iSector].PGi_dim_t, _DATA._one_t, iSector);
			}

		}
		else if (_LOGINFOOPTIONS.doSanityCheckBin)
		{
			if (_LOGINFOOPTIONS.SanityCheckBin == _DATA._zero_i)
			{
				for (int iBin = 1; iBin <= static_cast<int>(_VELODYNEOPTIONS.N_size); iBin++)
				{
					SaveSanityCheckBin(_LOGINFOOPTIONS.doSanityCheckBin, _LOGINFOOPTIONS.codeSteps.Segment_doSegment, Bins[iBin].Ground.host, Bins[iBin].Z.host, Bins[iBin].Sectors.host, Bins[iBin].Original_Idx.host, Bins[iBin].Bins_dim_t, _DATA._one_t, iBin);
				}

			}

			if (_LOGINFOOPTIONS.SanityCheckBin != _DATA._zero_i)
			{
				int iBin = _LOGINFOOPTIONS.SanityCheckBin;
				SaveSanityCheckBin(_LOGINFOOPTIONS.doSanityCheckBin, _LOGINFOOPTIONS.codeSteps.Segment_doSegment, Bins[iBin].Ground.host, Bins[iBin].Z.host, Bins[iBin].Sectors.host, Bins[iBin].Original_Idx.host, Bins[iBin].Bins_dim_t, _DATA._one_t, iBin);

			}

		}
		

		SaveSanityCheck(_LOGINFOOPTIONS.doSanityCheck, "Model_Hg", Model_Hg, _VELODYNEOPTIONS.N_size, _VELODYNEOPTIONS.M_size);

	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------

	RegisterEvent(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.codeSteps.Segment_doSegment);

}
