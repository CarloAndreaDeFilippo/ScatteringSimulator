#include <cuComplex.h>
#include <cuda.h>

#include <iostream>
#include <vector>

#include "../Rho1D.hpp"

extern "C" __global__ void rho1D_kernel_add(const double*, size_t,
                                            const double*, size_t,
                                            cuDoubleComplex*);

void Rho1D::computeRhoCUDA(const std::vector<double>& projBase) {
  size_t N = projBase.size();
  size_t M = qVector.qqmax;

  if (M == 0 || N == 0) return;

  double *d_proj = nullptr, *d_qvals = nullptr;
  cuDoubleComplex* d_rho = nullptr;

  cudaMalloc(&d_proj, N * sizeof(double));
  cudaMalloc(&d_qvals, M * sizeof(double));
  cudaMalloc(&d_rho, M * sizeof(cuDoubleComplex));

  cudaMemcpy(d_proj, projBase.data(), N * sizeof(double),
             cudaMemcpyHostToDevice);

  cudaMemcpy(d_qvals, qVector.qValues.data(),
             M * sizeof(double), cudaMemcpyHostToDevice);

  // Copy rho (current accumulated values)
  cudaMemcpy(d_rho, rho.data(), M * sizeof(cuDoubleComplex),
             cudaMemcpyHostToDevice);

  int blockSize = 256;
  int gridSize = (M + blockSize - 1) / blockSize;

  rho1D_kernel_add<<<gridSize, blockSize>>>(d_proj, N, d_qvals, M, d_rho);

  cudaError_t err = cudaGetLastError();
  if (err != cudaSuccess) {
    printf("CUDA kernel launch error: %s\n", cudaGetErrorString(err));
  }

  cudaDeviceSynchronize();
  err = cudaGetLastError();
  if (err != cudaSuccess) {
    printf("CUDA kernel runtime error: %s\n", cudaGetErrorString(err));
  }

  std::vector<cuDoubleComplex> tmp(M);
  cudaMemcpy(tmp.data(), d_rho, M * sizeof(cuDoubleComplex),
             cudaMemcpyDeviceToHost);

  for (size_t j = 0; j < M; ++j)
    rho[j] = std::complex<double>(cuCreal(tmp[j]), cuCimag(tmp[j]));

  cudaFree(d_proj);
  cudaFree(d_qvals);
  cudaFree(d_rho);
}