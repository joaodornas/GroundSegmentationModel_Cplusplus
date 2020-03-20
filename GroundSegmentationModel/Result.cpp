#pragma once

#include "Globals.h"
#include "Result.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////
// MODEL OUTPUT //////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


void saveDataOuptut(struct DeviceAndHostMemoryX<int> & ocl_PG_Ground, struct DeviceAndHostMemoryX<float> & ocl_PG_X, struct DeviceAndHostMemoryX<float> & ocl_PG_Y, struct DeviceAndHostMemoryX<float> & ocl_PG_Z)
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SAVE 3D Data POINTS COORDINATES WITH THE GROUND PARAMETER IN A BINARY FILE, JUST LIKE VELODYNE BINARY INPUT FILE//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (_LOGINFOOPTIONS.saveResult)
	{

		std::string codeStep = "Saving Ground Information...";
		std::string division = "--------------------------------------------------";

		std::cout << "<" + division + ">\n";
		std::cout << "<" + codeStep + ">\n";
		std::cout << "<" + division + ">\n";

		std::string outputFolder = _DATAOPTIONS.folderOutput + "\\Ground\\";

		//create directory
		if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
			std::experimental::filesystem::create_directory(outputFolder);
		}

		size_t lastindex = _DATAOPTIONS.fileRunning.find_last_of(".");
		std::string rawname = _DATAOPTIONS.fileRunning.substr(0, lastindex);

		lastindex = rawname.find_last_of("\\");
		std::string fileName = rawname.substr(lastindex + 1, rawname.length());

		std::ofstream out(outputFolder + fileName + "-X-Y-Z-Ground.txt", std::ofstream::out | std::ofstream::trunc);

		//out << "X,Y,Z,Ground\n";

		for (size_t ii = 0; ii < _DATA.PG_dim_t; ii++)
		{
			float Px = ocl_PG_X.host[ii];
			float Py = ocl_PG_Y.host[ii];
			float Pz = ocl_PG_Z.host[ii];
			int Ground = ocl_PG_Ground.host[ii];

			out << Px << std::scientific;
			out << ",";
			out << Py << std::scientific;
			out << ",";
			out << Pz << std::scientific;
			out << ",";
			out << Ground << std::scientific;

			out << std::endl;

		}

		out.close();

		lastindex = NULL;
		rawname.clear();

	}

};


void savePicturesWithGround(std::string mode, DeviceAndHostMemoryX<int> & ocl_PG_Ground, struct DeviceAndHostMemoryX<float> & ocl_PG_X, struct DeviceAndHostMemoryX<float> & ocl_PG_Y, struct DeviceAndHostMemoryX<float> & ocl_PG_Z, struct DeviceAndHostMemoryX<int> & ocl_PG_Sector, struct DeviceAndHostMemoryX<int> & ocl_PG_Bins)
{
	// -----------------------------------------------------------------------
	// LOAD CALIBRATION CAM INFO
	// -----------------------------------------------------------------------

	if ( (_LOGINFOOPTIONS.savePicture) || (_LOGINFOOPTIONS.saveMovie))
	{
		loadCalibrationCamToCam();
		loadCalibrationRigid();
		projectionMatrixVeloToImg();
	}

	if ( (_LOGINFOOPTIONS.savePicture) || (_LOGINFOOPTIONS.saveMovie) )
	{
		
		cv::Mat Image_Camera_Lidar_Obj;
		cv::Mat Image_Camera_Lidar_Obj_Ground;
		cv::Mat Output;
		cv::Mat Result;
		cv::Mat Original;

		std::string outputFolder;

		double alpha = 0.5;

		int iscolor = 1;

		getJetColorMap();

		std::string image_filename_str = _DATAOPTIONS.folderPictures + "\\" + _DATAOPTIONS.fileName + ".png";

		std::unique_ptr<char[]> image_filename(new char[image_filename_str.length() + 1]);

		std::strcpy(image_filename.get(), image_filename_str.c_str());

		Original = cv::imread(image_filename.get(), CV_LOAD_IMAGE_COLOR);

		Image_Camera_Lidar_Obj = cv::imread(image_filename.get(), CV_LOAD_IMAGE_COLOR);
		Image_Camera_Lidar_Obj_Ground = cv::imread(image_filename.get(), CV_LOAD_IMAGE_COLOR);

		cv::Mat Image_Lidar(Original.rows, Original.cols, CV_8UC3, cv::Scalar(0,0,0));

		Output = cv::imread(image_filename.get(), CV_LOAD_IMAGE_COLOR);

		image_filename.reset();

		projectVelodyneToCam(ocl_PG_X, ocl_PG_Y, ocl_PG_Z);

		for (size_t j = 0; j < _DATA.PG_dim_t; j++) 
		{
			float _X = ocl_PG_X.host[j];
			float _Y = ocl_PG_Y.host[j];
			float _Z = ocl_PG_Z.host[j];
			int _S = ocl_PG_Sector.host[j];
			int _B = ocl_PG_Bins.host[j];
			float _OP = std::sqrt((_X * _X) + (_Y * _Y));

			float _X_prj = _VELODYNEOPTIONS.XYZT_prj(j, 0);
			float _Y_prj = _VELODYNEOPTIONS.XYZT_prj(j, 1);

			if ((_X > _DATA._zero_f) && (_OP <= (float)_VELODYNEOPTIONS.R))
			{
				float R = 0;
				float G = 0;
				float B = 0;

				if (
					
					( (!_LOGINFOOPTIONS.doSanityCheck) && (mode == _LOGINFOOPTIONS.pictureMode_COMPLETE) ) ||
					( (_LOGINFOOPTIONS.doSanityCheck) && (mode == _LOGINFOOPTIONS.pictureMode_COMPLETE) ) ||

					( (_LOGINFOOPTIONS.doSanityCheck) && (_LOGINFOOPTIONS.doSanityCheckSector) && (_DATA.iS == _S) && (mode == _LOGINFOOPTIONS.pictureMode_SECTOR) ) ||
					( (_LOGINFOOPTIONS.doSanityCheck) && (_LOGINFOOPTIONS.doSanityCheckBin) && (_DATA.iB == _B) && (mode == _LOGINFOOPTIONS.pictureMode_BIN) )
					
					)
				{
					
					if (ocl_PG_Ground.host[j] == _DATA._one_i)
					{
						R = 255;
						G = 0;
						B = 0;
					}
					else
					{
						R = 0;
						G = 255;
						B = 0;
					}

					cv::circle(Image_Lidar, 
						cv::Point((int)_X_prj, (int)_Y_prj),
						2, 
						cv::Scalar(B, G, R),
						-1, 
						8,
						0);

					cv::circle(Image_Camera_Lidar_Obj_Ground,
						cv::Point((int)_X_prj, (int)_Y_prj),
						2,
						cv::Scalar(B, G, R),
						-1,
						8,
						0);

					if (ocl_PG_Ground.host[j] == _DATA._one_i)
					{
						R = 255;
						G = 0;
						B = 0;

						cv::circle(Image_Camera_Lidar_Obj,
							cv::Point((int)_X_prj, (int)_Y_prj),
							2,
							cv::Scalar(B, G, R),
							-1,
							8,
							0);
					}
				}
				
			}

		}


		if (

			(!_LOGINFOOPTIONS.doSanityCheck) ||

			( (_LOGINFOOPTIONS.doSanityCheck) && (_LOGINFOOPTIONS.doSanityCheckSector) && (mode == _LOGINFOOPTIONS.pictureMode_SECTOR)) ||
			( (_LOGINFOOPTIONS.doSanityCheck) && (_LOGINFOOPTIONS.doSanityCheckBin) && (mode == _LOGINFOOPTIONS.pictureMode_BIN))

			)
		{

			if ( (_LOGINFOOPTIONS.doSanityCheckSector) && (_DATA.iS <= static_cast<int>(_VELODYNEOPTIONS.M)))
			{
				outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Sectors\\" + "Sector-" + std::to_string(_DATA.iS) + "\\";
			}
			else if ((_LOGINFOOPTIONS.doSanityCheckBin) && (_DATA.iB <= static_cast<int>(_VELODYNEOPTIONS.N)))
			{
				outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\" + "Bins\\" + "Bin-" + std::to_string(_DATA.iB) + "\\";
			}
			else if ((_LOGINFOOPTIONS.doSanityCheck) && (_DATA.iS > static_cast<int>(_VELODYNEOPTIONS.M)))
			{
				outputFolder = _DATAOPTIONS.folderOutput + "\\SanityCheck\\";
			}
			else
			{
				outputFolder = _DATAOPTIONS.folderOutput + "\\Pictures\\";
			}

			//create directory
			if (!std::experimental::filesystem::is_directory(outputFolder) || !std::experimental::filesystem::exists(outputFolder)) {
				std::experimental::filesystem::create_directory(outputFolder);
			}

			std::string codeStep = "Saving Picture...";
			std::string division = "--------------------------------------------------";

			if (_LOGINFOOPTIONS.verbose)
			{
				std::cout << "<" + division + ">\n";
				std::cout << "<" + codeStep + ">\n";
				std::cout << "<" + division + ">\n";
			}

			std::string outputFolder_str = outputFolder + _DATAOPTIONS.fileName + ".png";

			std::unique_ptr<char[]> output_filename(new char[outputFolder_str.length() + 1]);

			std::strcpy(output_filename.get(), outputFolder_str.c_str());

			cv::Mat Result_tmp1;
			cv::Mat Result_tmp2;

			cv::hconcat(Original, Image_Camera_Lidar_Obj, Result_tmp1);

			cv::hconcat(Image_Lidar, Image_Camera_Lidar_Obj_Ground, Result_tmp2);

			cv::vconcat(Result_tmp1, Result_tmp2, Result);

			cv::imwrite(output_filename.get(), Result);

			cv::Mat Result_sized;

			cv::resize(Result, Result_sized, cv::Size(_LOGINFOOPTIONS.picture_Width, _LOGINFOOPTIONS.picture_Height), 0, 0, CV_INTER_LINEAR);

			if ( (_LOGINFOOPTIONS.saveMovie) && (!_LOGINFOOPTIONS.videoWriter->isOpened()) )
			{
				std::string videos_folder = _DATAOPTIONS.folderOutput + "\\Videos\\";

				//create directory
				if (!std::experimental::filesystem::is_directory(videos_folder) || !std::experimental::filesystem::exists(videos_folder)) {
					std::experimental::filesystem::create_directory(videos_folder);
				}

				_LOGINFOOPTIONS.videoWriter->open(videos_folder + "video.mpg", CV_FOURCC('M', 'P', 'E', 'G'), (double)_LOGINFOOPTIONS.fps, cv::Size(Result_sized.cols, Result_sized.rows), true);

				
				_LOGINFOOPTIONS.videoWriter->write(Result_sized);


			}
			else if ((_LOGINFOOPTIONS.saveMovie) && (_LOGINFOOPTIONS.videoWriter->isOpened()))
			{
				
				_LOGINFOOPTIONS.videoWriter->write(Result_sized);

			}

			output_filename.reset();

			Result_tmp1.release();
			Result_tmp2.release();

			Result_sized.release();

		}

		Image_Lidar.release();

		Image_Camera_Lidar_Obj.release();

		Image_Camera_Lidar_Obj_Ground.release();

		Output.release();
		
		Result.release();
		
	}

}


void projectVelodyneToCam(struct DeviceAndHostMemoryX<float> & ocl_PG_X, struct DeviceAndHostMemoryX<float> & ocl_PG_Y, struct DeviceAndHostMemoryX<float> & ocl_PG_Z)
{

	//dimension of data and projection matrix
	int dim_norm = _VELODYNEOPTIONS.NCoordinates;
	int dim_proj = _VELODYNEOPTIONS.NCoordinates + 1;
	int dim_points = static_cast<int>(_DATA.PG_dim_t);

	Eigen::MatrixXf XYZT = Eigen::MatrixXf::Zero(dim_points, _VELODYNEOPTIONS.NCoordinates + 1);
	Eigen::MatrixXf XYZT_tp = Eigen::MatrixXf::Zero(_VELODYNEOPTIONS.NCoordinates + 1, _VELODYNEOPTIONS.NCoordinates);
	Eigen::MatrixXf XYZT_prj_tp = Eigen::MatrixXf::Zero(_VELODYNEOPTIONS.NCoordinates + 1, _VELODYNEOPTIONS.NCoordinates);
	Eigen::MatrixXf XYZT_prj_tmp = Eigen::MatrixXf::Zero(_VELODYNEOPTIONS.NCoordinates, _VELODYNEOPTIONS.NCoordinates + 1);

	Eigen::MatrixXf Ones = Eigen::MatrixXf::Ones(1, dim_norm - 1);

	for (int i = 0; i < dim_points; i++) 
	{

		XYZT(i, 0) = ocl_PG_X.host[i]; 
		XYZT(i, 1) = ocl_PG_Y.host[i];
		XYZT(i, 2) = ocl_PG_Z.host[i];
		XYZT(i, 3) = _DATA._one_f; 

	}
	
	//do transformation in homogenuous coordinates
	XYZT_tp = XYZT.transpose();
	XYZT_prj_tp = _VELODYNEOPTIONS.P_Velo_to_Img * XYZT_tp;
	XYZT_prj_tmp = XYZT_prj_tp.transpose();

	//normalize homogeneous coordinates :
	Eigen::VectorXf last_dim = XYZT_prj_tmp.col(dim_norm - _DATA._one_i);

	Eigen::MatrixXf last_dim_ones = last_dim * Ones;

	_VELODYNEOPTIONS.XYZT_prj = Eigen::MatrixXf::Zero(dim_points, dim_norm - _DATA._one_i);

	Eigen::MatrixXf tmp = Eigen::MatrixXf::Zero(dim_points, dim_norm - _DATA._one_i);

	tmp.col(0) << XYZT_prj_tmp.col(0);
	tmp.col(1) << XYZT_prj_tmp.col(1);

	for (int i = 0; i < dim_points; i++)
	{
		for (int j = 0; j < (dim_norm - 1); j++)
		{
			_VELODYNEOPTIONS.XYZT_prj(i, j) = tmp(i, j) / last_dim_ones(i,j);

		}
	}
	
}


void clearJetColorMap()
{
	//LOGINFOOPTIONS

	_LOGINFOOPTIONS.RGB_R.reset();
	_LOGINFOOPTIONS.RGB_G.reset();
	_LOGINFOOPTIONS.RGB_B.reset();

}


void getJetColorMap()
{
	// -----------------------------------------------------------------------
	// JET COLORMAP R-G-B SCALE
	// -----------------------------------------------------------------------

	int count = 0;

	_LOGINFOOPTIONS.RGB_R = make_unique<float[]>(64);
	_LOGINFOOPTIONS.RGB_G = make_unique<float[]>(64);
	_LOGINFOOPTIONS.RGB_B = make_unique<float[]>(64);

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 143.437500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 159.375000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 175.312500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 191.250000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 207.187500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 223.125000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 239.062500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 15.9375000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 31.8750000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 47.8125000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 63.7500000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 79.6875000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 95.6250000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 111.562500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 127.500000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 143.437500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 159.375000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 175.312500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 191.250000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 207.187500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 223.125000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 239.062500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 0;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 255;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 15.9375000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 239.062500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 31.8750000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 223.125000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 47.8125000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 207.187500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 63.7500000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 191.250000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 79.6875000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 175.312500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 95.6250000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 159.375000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 111.562500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 143.437500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 127.500000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 127.500000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 143.437500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 111.562500000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 159.375000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 95.6250000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 175.312500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 79.6875000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 191.250000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 63.7500000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 207.187500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 47.8125000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 223.125000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 31.8750000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 239.062500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 15.9375000000000;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 255;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 239.062500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 223.125000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 207.187500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 191.250000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 175.312500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 159.375000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 143.437500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 127.500000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 111.562500000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 95.6250000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 79.6875000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 63.7500000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 47.8125000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 31.8750000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 15.9375000000000;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 255;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 239.062500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 223.125000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 207.187500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 191.250000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 175.312500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 159.375000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 143.437500000000;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;

	count++;

	_LOGINFOOPTIONS.RGB_R[count] = 127.500000000000;
	_LOGINFOOPTIONS.RGB_G[count] = 0;
	_LOGINFOOPTIONS.RGB_B[count] = 0;
}
