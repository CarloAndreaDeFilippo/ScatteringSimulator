#include <cuComplex.h>
#include <cuda.h>

#include <iostream>
#include <vector>

#include "../Rho1D.hpp"
#include "GPUMemoryManager.hpp"

extern "C" __global__ void compute_projection_1d_kernel(const double* centers,
                                                        const double* q_axis,
                                                        size_t N,
                                                        double* projections);

extern "C" __global__ void rho1D_kernel(const double* proj, size_t N,
                                        const double* qvals, size_t M,
                                        cuDoubleComplex* rho);

void Rho1D::calculateRhoGPU(const GPUMemoryManager& gpuMemory) {
  size_t nSP = gpuMemory.getNumScatteringPoints();

  // Allocate GPU projections
  double* d_projections;
  cudaMalloc(&d_projections, nSP * sizeof(double));

  // Upload q-axis to GPU
  double* d_qAxis;
  cudaMalloc(&d_qAxis, 3 * sizeof(double));
  cudaMemcpy(d_qAxis, qVector.qAxis.data(), 3 * sizeof(double),
             cudaMemcpyHostToDevice);

  // Compute projections on GPU
  dim3 block(256);
  dim3 gridProjection((nSP + 255) / 256);

  compute_projection_1d_kernel<<<gridProjection, block>>>(
      gpuMemory.getDeviceScatteringCenters(), d_qAxis, nSP, d_projections);

  size_t M = qVector.qqmax;

  // Upload q-values
  double* d_qvals;
  cudaMalloc(&d_qvals, M * sizeof(double));
  cudaMemcpy(d_qvals, qVector.qValues.data(), M * sizeof(double),
             cudaMemcpyHostToDevice);

  // Upload current rho values
  cuDoubleComplex* d_rho;
  cudaMalloc(&d_rho, M * sizeof(cuDoubleComplex));

  std::vector<cuDoubleComplex> h_rho(M);
  for (size_t i = 0; i < M; ++i) {
    h_rho[i] = make_cuDoubleComplex(rho[i].real(), rho[i].imag());
  }
  cudaMemcpy(d_rho, h_rho.data(), M * sizeof(cuDoubleComplex),
             cudaMemcpyHostToDevice);

  int blockSize = 256;

  // X dimension covers all SP
  int gridX = (nSP + blockSize - 1) / blockSize;

  // Y dimension covers all M q-values
  int gridY = M;

  dim3 grid(gridX, gridY);

  rho1D_kernel<<<grid, blockSize>>>(d_projections, nSP, d_qvals, M, d_rho);

  // Copy back results
  cudaMemcpy(h_rho.data(), d_rho, M * sizeof(cuDoubleComplex),
             cudaMemcpyDeviceToHost);

  for (size_t i = 0; i < M; ++i) {
    rho[i] = std::complex<double>(cuCreal(h_rho[i]), cuCimag(h_rho[i]));
  }

  // Cleanup

  cudaFree(d_qvals);
  cudaFree(d_rho);
  cudaFree(d_projections);
  cudaFree(d_qAxis);
}
