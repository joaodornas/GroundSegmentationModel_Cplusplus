#pragma once

#pragma warning (disable: 4244) //Disable warning related to double to float conversion
#pragma warning (disable: 4018) //Disable warning related to unsigned/signed mismatch integers
#pragma warning (disable: 4101) //Disable warning related to unreferenced local variable


#include "Model.h"
#include "Globals.h"
#include "PolarGridMap.h"
#include "Seed.h"
#include "Test.h"
#include "Eval.h"
#include "Segment.h"
#include "Result.h"


struct OPENCLOBJECT _OPENCLOBJECT;
struct GPUOPTIONS _GPUOPTIONS;
struct VELODYNEOPTIONS _VELODYNEOPTIONS;
struct MODELOPTIONS _MODELOPTIONS;
struct DATAOPTIONS _DATAOPTIONS;
struct DATA _DATA;
struct LOGINFOOPTIONS _LOGINFOOPTIONS;

void run_Model()
{
	clearTime();

	RegisterStep(_LOGINFOOPTIONS.START,_LOGINFOOPTIONS.verbose, _LOGINFOOPTIONS.codeSteps.RunModelLoop, _DATA.iFile);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//LOAD THE POLAR GRID 3D Data POINTS FROM BINARY FILE
	////////////////////////////////////////////////////////////////////////////////////////////////
	struct DeviceAndHostMemoryX<float> ocl_PG_X;
	struct DeviceAndHostMemoryX<float> ocl_PG_Y;
	struct DeviceAndHostMemoryX<float> ocl_PG_Z;
	struct DeviceAndHostMemoryX<int> ocl_PG_Sector;
	struct DeviceAndHostMemoryX<int> ocl_PG_Bins;
	struct DeviceAndHostMemoryX<int> ocl_PG_Ground;
	struct PGi_struct PGi[361];
	struct Bins_struct Bins[161];
	Eigen::MatrixXf Model_Hg = Eigen::MatrixXf::Zero(_VELODYNEOPTIONS.N_size + _DATA._one_t, _VELODYNEOPTIONS.M_size + _DATA._one_t);

	PolarGridMap(ocl_PG_Ground, PGi, Bins, ocl_PG_X, ocl_PG_Y, ocl_PG_Z, ocl_PG_Sector, ocl_PG_Bins);
	////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////
	//DEFINE MINIMUM VALUE OF Z FOR THRESHOLD
	////////////////////////////////////////////////////////////////////////////////////////////////
	if (_MODELOPTIONS.setManualMinZ)
	{
		_DATA.min_Z = _MODELOPTIONS.manualMinZ;

	}
	else
	{
		getMinZ(_LOGINFOOPTIONS.codeSteps.getMinZFunction, _DATA.PG_dim_t, ocl_PG_Z, ocl_PG_Sector, _DATA.min_Z);
	}

	SaveSanityCheck(_LOGINFOOPTIONS.doSanityCheck, _LOGINFOOPTIONS.codeSteps.getMinZFunction, _DATA.min_Z);
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	//LOOP OVER M SECTORS TO EXTRACT 3D Data POINTS FROM EACH SECTOR AT A TIME//////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	int start_sector = 0;
	int end_sector = 0;

	if (_LOGINFOOPTIONS.doSanityCheck)
	{
		if ((_LOGINFOOPTIONS.doSanityCheckBin) && (_LOGINFOOPTIONS.doSanityCheckSector))
		{
			start_sector = 1;
			end_sector = (int)_VELODYNEOPTIONS.M;
		}
		else if (_LOGINFOOPTIONS.doSanityCheckBin)
		{
			start_sector = 1;
			end_sector = (int)_VELODYNEOPTIONS.M;
		}
		else if (_LOGINFOOPTIONS.doSanityCheckSector)
		{
			
			if (_LOGINFOOPTIONS.SanityCheckSector != _DATA._zero_i)
			{
				start_sector = _LOGINFOOPTIONS.SanityCheckSector;
				end_sector = _LOGINFOOPTIONS.SanityCheckSector;
			}
			else
			{
				start_sector = 1;
				end_sector = (int)_VELODYNEOPTIONS.M;
			}

		}
		else
		{
			if (_VELODYNEOPTIONS.doOnlyFrontView)
			{
				start_sector = 1;
				end_sector = ((int)_VELODYNEOPTIONS.M - _VELODYNEOPTIONS.Sectors_Front_Left) + _VELODYNEOPTIONS.Sectors_Front_Right;
			}
			else
			{
				start_sector = 1;
				end_sector = (int)_VELODYNEOPTIONS.M;
			}
			
		}

	}
	else
	{
		if (_VELODYNEOPTIONS.doOnlyFrontView)
		{
			start_sector = 1;
			end_sector = ((int)_VELODYNEOPTIONS.M - _VELODYNEOPTIONS.Sectors_Front_Right) + _VELODYNEOPTIONS.Sectors_Front_Left;
		}
		else
		{
			start_sector = 1;
			end_sector = (int)_VELODYNEOPTIONS.M;
		}
	}

	struct DeviceAndHostMemoryX<bool> idx_sectors;

	idx_sectors.allocData(_VELODYNEOPTIONS.M_size + _DATA._one_t,_DATA._one_t,_DATA._one_t);

	if (_VELODYNEOPTIONS.doOnlyFrontView)
	{

		for (int i = 1; i <= _VELODYNEOPTIONS.Sectors_Front_Left; i++)
		{
			idx_sectors.host[i] = true;

		}

		for (int i = _VELODYNEOPTIONS.Sectors_Front_Right; i < (int)_VELODYNEOPTIONS.M; i++)
		{
			idx_sectors.host[i] = true;

		}

	}
	else
	{
		
		for (int i = 1; i <= (int)_VELODYNEOPTIONS.M; i++)
		{
			
			idx_sectors.host[i] = true;

		}
	}

	if ((_LOGINFOOPTIONS.doSanityCheck) && (_LOGINFOOPTIONS.doSanityCheckSector))
	{
		for (int i = 1; i <= (int)_VELODYNEOPTIONS.M; i++)
		{
			idx_sectors.host[i] = false;

			if (i == _LOGINFOOPTIONS.SanityCheckSector)
			{
				idx_sectors.host[i] = true;
			}

		}
	}

	for (size_t iS = 1; iS <= _VELODYNEOPTIONS.M_size; iS++)
	{
		if (idx_sectors.host[iS])
		{
			_DATA.iS = static_cast<int>(iS);

			_DATA.PGi_dim_t = PGi[_DATA.iS].PGi_dim_t;
			_DATA.PGi_dim_i = PGi[_DATA.iS].PGi_dim_i;

			RegisterStep(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.verbose, _LOGINFOOPTIONS.codeSteps.SectorsLoop, _DATA.iS);

			struct Sectors_struct ocl_PGi;

			struct DeviceAndHostMemoryX<int> inliers_idx;

			/////////////////////////////////////////////////////////////
			//START LOOP TO FIT THE GAUSSIAN EQUATION TO FIND THE GROUND
			/////////////////////////////////////////////////////////////
			_DATA.iG = 0; bool inliers = true;
			while (inliers)
			{
				_DATA.iG++;
				RegisterStep(_LOGINFOOPTIONS.START, _LOGINFOOPTIONS.verbose, _LOGINFOOPTIONS.codeSteps.GaussLoop, _DATA.iG);

				struct Sectors_struct ocl_Sp;

				if (_DATA.iG == _DATA._one_i)
				{
					//////////////////////////////////////////////////////////////////////////
					//EXTRACT THE SEED 3D POINTS INSIDE A SECTOR//////////////////////////////
					//////////////////////////////////////////////////////////////////////////
					Seed(ocl_PGi, ocl_Sp, PGi, inliers_idx);
					//////////////////////////////////////////////////////////////////////////
				}
				else
				{
					if (_DATA.Sp_dim_t > _DATA._zero_t)
					{
						//////////////////////////////////////////////////////////////////////////
						//ADD SNEW TO SP /////////////////////////////////////////////////////////
						//////////////////////////////////////////////////////////////////////////
						groupSpUSnew(ocl_PGi, ocl_Sp, PGi, inliers_idx);
						//////////////////////////////////////////////////////////////////////////
					}
					else
					{
						inliers = false;
					}

				}

				if ((_DATA.Sp_dim_t > _DATA._zero_t) && (_DATA.Sp_dim_t < PGi[_DATA.iS].PGi_dim_t))
				{
					//////////////////////////////////////////////////////////////////////////
					//GET NEW SAMPLE OF TEST 3D Data POINTS///////////////////////////////////
					//////////////////////////////////////////////////////////////////////////
					struct Sectors_struct ocl_Test;
					getTest(ocl_Test, PGi, ocl_PGi, ocl_Sp);
					/////////////////////////////////////////////////////////////////////////


					if (_DATA.Test_dim_t > _DATA._zero_t)
					{
						/////////////////////////////////////////////////////////////////////////
						//RUN EVALUATION OF THE MODEL////////////////////////////////////////////
						/////////////////////////////////////////////////////////////////////////
						Eval(ocl_Test, ocl_Sp, inliers, inliers_idx, ocl_PG_Ground, Model_Hg);
						/////////////////////////////////////////////////////////////////////////

						ocl_Test.releaseDataHost();
						ocl_Test.releaseDataDevice();
						_DATA.Test_dim_t = _DATA._zero_t;
						_DATA.Test_dim_i = _DATA._zero_i;

					}
					else
					{
						inliers = false;
					}

				}
				else
				{
					inliers = false;
				}

				if (_DATA.Sp_dim_t > _DATA._zero_t)
				{

					ocl_Sp.releaseDataHost();
					ocl_Sp.releaseDataDevice();

				}

				RegisterStep(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.verbose, _LOGINFOOPTIONS.codeSteps.GaussLoop, _DATA.iG);
			}

			if (_DATA.Sp_dim_t > _DATA._zero_t)
			{
				//////////////////////////////////////////////////////////////////////////
				//CLEAR SECTOR DATA //////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////
				_DATA.Sp_dim_t = _DATA._zero_t;
				_DATA.Sp_dim_i = _DATA._zero_i;
				ocl_PGi.releaseDataHost();
				inliers_idx.releaseDataHost();
				//////////////////////////////////////////////////////////////////////////
			}

			RegisterStep(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.verbose, _LOGINFOOPTIONS.codeSteps.SectorsLoop, _DATA.iS);

		}

	}

	//////////////////////////////////////////////////////////////////////////////////
	//APPLY SEGMENTATION TO FIND THE GROUND///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	doSegment(PGi, Bins, ocl_PG_Ground, Model_Hg, ocl_PG_Bins, ocl_PG_Sector, ocl_PG_Z);
	//////////////////////////////////////////////////////////////////////////////////

	RegisterStep(_LOGINFOOPTIONS.STOP, _LOGINFOOPTIONS.verbose, _LOGINFOOPTIONS.codeSteps.RunModelLoop, _DATA.iFile);

	//////////////////////////////////////////////////////////////////////////////////
	//SAVE RESULTS IN A PICTURE //////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	if (_LOGINFOOPTIONS.doSanityCheck)
	{
		if (_LOGINFOOPTIONS.doSanityCheckBin)
		{

			if (_LOGINFOOPTIONS.SanityCheckBin != _DATA._zero_i)
			{
				_DATA.iB = _LOGINFOOPTIONS.SanityCheckBin;
				savePicturesWithGround(_LOGINFOOPTIONS.pictureMode_BIN, ocl_PG_Ground, ocl_PG_X, ocl_PG_Y, ocl_PG_Z, ocl_PG_Sector, ocl_PG_Bins);
			}
			else
			{
				for (int iB = 1; iB <= static_cast<int>(_VELODYNEOPTIONS.N); iB++)
				{
					_DATA.iB = iB;
					savePicturesWithGround(_LOGINFOOPTIONS.pictureMode_BIN, ocl_PG_Ground, ocl_PG_X, ocl_PG_Y, ocl_PG_Z, ocl_PG_Sector, ocl_PG_Bins);
				}
			}

		}

		if (_LOGINFOOPTIONS.doSanityCheckSector)
		{
			
			if (_LOGINFOOPTIONS.SanityCheckSector != _DATA._zero_i)
			{
				_DATA.iS = _LOGINFOOPTIONS.SanityCheckSector;
				savePicturesWithGround(_LOGINFOOPTIONS.pictureMode_SECTOR, ocl_PG_Ground, ocl_PG_X, ocl_PG_Y, ocl_PG_Z, ocl_PG_Sector, ocl_PG_Bins);
			}
			else
			{
				for (int iS = 1; iS <= (int)_VELODYNEOPTIONS.M; iS++)
				{
					_DATA.iS = iS;

					savePicturesWithGround(_LOGINFOOPTIONS.pictureMode_SECTOR, ocl_PG_Ground, ocl_PG_X, ocl_PG_Y, ocl_PG_Z, ocl_PG_Sector, ocl_PG_Bins);
				}
			}
		}

		savePicturesWithGround(_LOGINFOOPTIONS.pictureMode_COMPLETE, ocl_PG_Ground, ocl_PG_X, ocl_PG_Y, ocl_PG_Z, ocl_PG_Sector, ocl_PG_Bins);

	}
	else
	{
		savePicturesWithGround(_LOGINFOOPTIONS.pictureMode_COMPLETE, ocl_PG_Ground, ocl_PG_X, ocl_PG_Y, ocl_PG_Z, ocl_PG_Sector, ocl_PG_Bins);
	}
	//////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////
	//SAVE RESULTS FROM THE MODEL IN A TXT FILE///////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	//saveDataOuptut(ocl_PG_Ground,ocl_PG_X,ocl_PG_Y,ocl_PG_Z);
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	//SHOW REPORT ////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	showReport();
	clearTime();
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	//CLEAR ALL DATA /////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	
	ocl_PG_Ground.releaseDataHost();

	ocl_PG_X.releaseDataHost();
	ocl_PG_Y.releaseDataHost();
	ocl_PG_Z.releaseDataHost();
	ocl_PG_Sector.releaseDataHost();
	ocl_PG_Bins.releaseDataHost();

	for (size_t i = 0; i <= _VELODYNEOPTIONS.M_size; i++)
	{
		PGi[i].X.releaseDataHost();
		PGi[i].Y.releaseDataHost();
		PGi[i].Z.releaseDataHost();
		PGi[i].OP.releaseDataHost();
		PGi[i].Ground.releaseDataHost();
		PGi[i].Bins.releaseDataHost();
		PGi[i].Original_Idx.releaseDataHost();

		PGi[i].Bins_count.releaseDataHost();
	}

	for (size_t i = 0; i <= _VELODYNEOPTIONS.N_size; i++)
	{
		if (Bins[i].Bins_dim_t > _DATA._zero_t)
		{
			Bins[i].X.releaseDataHost();
			Bins[i].Y.releaseDataHost();
			Bins[i].Z.releaseDataHost();
			Bins[i].OP.releaseDataHost();
			Bins[i].Ground.releaseDataHost();
			Bins[i].Sectors.releaseDataHost();
			Bins[i].Original_Idx.releaseDataHost();

			Bins[i].Sectors_count.releaseDataHost();
		}
	}
	
	idx_sectors.releaseDataHost();
	//////////////////////////////////////////////////////////////////////////////////

}

int main(int argc, const char** argv)
{

		////////////////////////////////////////////////////////////////////////////////////////////////
		//PARSE COMMAND LINE INFORMATION ///////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
		parseParameters(argc, argv);
		////////////////////////////////////////////////////////////////////////////////////////////////

		if (_LOGINFOOPTIONS.parsed_OK)
		{

			////////////////////////////////////////////////////////////////////////////////////////////////
			//LOAD DATA, VELODYNE, MODEL, GPU  AND LOGOPTIONS OPTIONS///////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////
			setParameters();
			////////////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////////////////////////////
			//START CPU/GPU DEVICE /////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////
			startGPU();
			////////////////////////////////////////////////////////////////////////////////////////////////

			if (_LOGINFOOPTIONS.showDeviceInfo)
			{
				//////////////////////////////////////////////////////////////////////////////////////////////////////
				//SHOW DEVICE INFO  //////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////////////////
				showDeviceInfo();
				//////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			else
			{

				////////////////////////////////////////////////////////////////////////////////////////////////
				//START MOVIE  /////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////////
				if (_LOGINFOOPTIONS.saveMovie)
				{
					_LOGINFOOPTIONS.videoWriter = new cv::VideoWriter();
				}
				////////////////////////////////////////////////////////////////////////////////////////////////

				std::string outputFolder = _DATAOPTIONS.folderOutput;

				//create directory
				if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
					std::experimental::filesystem::create_directory(outputFolder);
				}

				if (_LOGINFOOPTIONS.doSanityCheck)
				{
					std::string sanity_folder = outputFolder + "\\SanityCheck";

					//create directory
					if (!std::experimental::filesystem::is_directory(sanity_folder) || !std::experimental::filesystem::exists(sanity_folder)) {
						std::experimental::filesystem::create_directory(sanity_folder);
					}
				}

				if (_LOGINFOOPTIONS.doSanityCheckSector)
				{
					std::string sector_folder = outputFolder + "\\SanityCheck\\Sectors";

					//create directory
					if (!std::experimental::filesystem::is_directory(sector_folder) || !std::experimental::filesystem::exists(sector_folder)) {
						std::experimental::filesystem::create_directory(sector_folder);
					}
				}

				if (_LOGINFOOPTIONS.doSanityCheckBin)
				{
					std::string bin_folder = outputFolder + "\\SanityCheck\\Bins";

					//create directory
					if (!std::experimental::filesystem::is_directory(bin_folder) || !std::experimental::filesystem::exists(bin_folder)) {
						std::experimental::filesystem::create_directory(bin_folder);
					}
				}

				outputFolder.clear();

				int totalFiles = 0;
				if (_LOGINFOOPTIONS.doSanityCheck)
				{
					totalFiles = 1;
				}
				else
				{
					totalFiles = _DATAOPTIONS.maxNumberOfFiles;
				}

				if (_DATAOPTIONS.oneFile)
				{

					_DATA.iFile = 0;

					size_t lastindex = _DATAOPTIONS.fileRunning.find_last_of(".");
					std::string rawname = _DATAOPTIONS.fileRunning.substr(0, lastindex);

					lastindex = rawname.find_last_of("\\");
					_DATAOPTIONS.fileName = rawname.substr(lastindex + 1, rawname.length());

					rawname.clear();
					lastindex = NULL;
					//////////////////////////////////////////////////////////////////////////////////////////////////////
					//RUN 3D GROUND-SEGMENTATION MODEL, ONLY ONE FILE/////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////////////////////

					run_Model();


				}
				else
				{
					//////////////////////////////////////////////////////////////////////////////////////////////////////
					//RUN 3D GROUND-SEGMENTATION MODEL, ONE FILE AT A TIME////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////////////////////
					_DATA.iFile = 0;
					std::experimental::filesystem::path pathToDisplay(_DATAOPTIONS.folderInput);
					for (auto& file : std::experimental::filesystem::directory_iterator(pathToDisplay))
					{
						_DATAOPTIONS.fileRunning = file.path().u8string();

						size_t lastindex = _DATAOPTIONS.fileRunning.find_last_of(".");
						std::string rawname = _DATAOPTIONS.fileRunning.substr(0, lastindex);

						lastindex = rawname.find_last_of("\\");
						_DATAOPTIONS.fileName = rawname.substr(lastindex + 1, rawname.length());

						if (_DATA.iFile < totalFiles)
						{
							run_Model();

							_DATA.iFile++;

							rawname.clear();
							lastindex = NULL;
						}
						else
						{
							break;
						}

					}

				}

				//////////////////////////////////////////////////////////////////////////////////////////////////////
				//SAVE PARAMETERS USED ///////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////////////////
				SaveParameters(_LOGINFOOPTIONS.doSaveParameters);
				//////////////////////////////////////////////////////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////////////////////////////////
				//RELEASE MOVIE  /////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////////////////
				if ((_LOGINFOOPTIONS.saveMovie) && (_LOGINFOOPTIONS.videoWriter->isOpened()))
				{
					_LOGINFOOPTIONS.videoWriter->release();
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////

			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//STOP CPU/GPU DEVICE ////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			stopGPU();
			//////////////////////////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//CLEAR VARIABLES ////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			clearVariables();
			//////////////////////////////////////////////////////////////////////////////////////////////////////
		}

	return 0;

}



