#pragma once

//LOAD THE POLAR GRID 3D Data POINTS FROM BINARY FILE
void PolarGridMap(struct DeviceAndHostMemoryX<int> & ocl_PG_Ground, struct PGi_struct(&PGi)[361], struct Bins_struct(&Bins)[161], struct DeviceAndHostMemoryX<float> & ocl_PG_X, struct DeviceAndHostMemoryX<float> & ocl_PG_Y, struct DeviceAndHostMemoryX<float> & ocl_PG_Z, struct DeviceAndHostMemoryX<int> & ocl_PG_Sector, struct DeviceAndHostMemoryX<int> & ocl_PG_Bins);

void getMinZ(std::string codeStep, size_t PG_dim, struct DeviceAndHostMemoryX<float> & ocl_PG_Z, struct DeviceAndHostMemoryX<int> & ocl_PG_Sector, float & min_Z);