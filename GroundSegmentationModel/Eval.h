#pragma once

//COMPUTES THE EVALUATION MATRICES COMPUTATION USING OPENCL
void Eval(struct Sectors_struct & ocl_Test, struct Sectors_struct & ocl_Sp, bool & inliers, struct DeviceAndHostMemoryX<int> & inliers_idx, struct DeviceAndHostMemoryX<int> & ocl_PG_Ground, Eigen::MatrixXf & Model_Hg);

void getMatrixInverse(struct DeviceAndHostMemoryX<float> & A_m);

//////////////////////////////////////////////////////////////////////////////////////////////
// LAPACK - BLAS /////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void strsm(int side, int uplo, int transA, int diag, int M, int N, float Alpha, struct DeviceAndHostMemoryX<float> & A_m, int A_offset, int lda, struct DeviceAndHostMemoryX<float> & B_m, int B_offset, int ldb);

void sgemm(int transA, int transB, int M, int N, int K, float Alpha, struct DeviceAndHostMemoryX<float> & A_m, int A_offset, int ldda, struct DeviceAndHostMemoryX<float> & B_m, int B_offset, int lddb, float Beta, struct DeviceAndHostMemoryX<float> & C_m, int C_offset, int lddc);

void ssyrk(int uplo, int transA, int N, int K, float Alpha, struct DeviceAndHostMemoryX<float> & A_m, int A_offset, int ldda, float Beta, struct DeviceAndHostMemoryX<float> & B_m, int B_offset, int lddb);

void strsm(int side, int uplo, int transA, int diag, int M, int N, float Alpha, struct DeviceAndHostMemoryX<float> & A_m, int A_offset, int lda, struct DeviceAndHostMemoryX<float> & B_m, int B_offset, int ldb);

void slauum(int uplo, int N, struct DeviceAndHostMemoryX<float> & A_m, int ldda);

void spotri(int uplo, int N, struct DeviceAndHostMemoryX<float> & A_m, int ldda);

void strtri(int uplo, int diag, int N, struct DeviceAndHostMemoryX<float> & ocl_matrix_Inverse_m, int ldda);

void spotrf(int uplo, int N, struct DeviceAndHostMemoryX<float> & A_m, int ldda);


