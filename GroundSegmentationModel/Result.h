#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////
// MODEL OUTPUT //////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void saveDataOuptut(struct DeviceAndHostMemoryX<int> & ocl_PG_Ground, struct DeviceAndHostMemoryX<float> & ocl_PG_X, struct DeviceAndHostMemoryX<float> & ocl_PG_Y, struct DeviceAndHostMemoryX<float> & ocl_PG_Z);

void savePicturesWithGround(std::string mode,  DeviceAndHostMemoryX<int> & ocl_PG_Ground, struct DeviceAndHostMemoryX<float> & ocl_PG_X, struct DeviceAndHostMemoryX<float> & ocl_PG_Y, struct DeviceAndHostMemoryX<float> & ocl_PG_Z, struct DeviceAndHostMemoryX<int> & ocl_PG_Sector, struct DeviceAndHostMemoryX<int> & ocl_PG_Bins);

void projectVelodyneToCam(struct DeviceAndHostMemoryX<float> & ocl_PG_X, struct DeviceAndHostMemoryX<float> & ocl_PG_Y, struct DeviceAndHostMemoryX<float> & ocl_PG_Z);

//void saveMovie();

void clearJetColorMap();

void getJetColorMap();