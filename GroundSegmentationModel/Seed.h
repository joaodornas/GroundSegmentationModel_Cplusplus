#pragma once

void Seed(struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361], struct DeviceAndHostMemoryX<int> & inliers_idx);

void extractSeed(struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361], struct DeviceAndHostMemoryX<int> & inliers_idx);

void countSeed(struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361]);

void groupSpUSnew(struct Sectors_struct & ocl_PGi, struct Sectors_struct & ocl_Sp, struct PGi_struct(&PGi)[361], struct DeviceAndHostMemoryX<int> & inliers_idx);




