#pragma once

#include <Eigen/eigen>

void doSegment(struct PGi_struct(&PGi)[361], struct Bins_struct(&Bins)[161], struct DeviceAndHostMemoryX<int> & ocl_PG_Ground, Eigen::MatrixXf & Model_Hg, struct DeviceAndHostMemoryX<int> & ocl_PG_Bins, struct DeviceAndHostMemoryX<int> & ocl_PG_Sector, struct DeviceAndHostMemoryX<float> & ocl_PG_Z);
