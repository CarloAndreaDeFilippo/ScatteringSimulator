#include <cuComplex.h>
#include <cuda_runtime.h>

#include <iostream>

#include "../Rho2D.hpp"
#include "GPUMemoryManager.hpp"

extern "C" __global__ void compute_projection_2d_kernel(const double* centers,
                                                        const double* q1_axis,
                                                        const double* q2_axis,
                                                        size_t N, double* proj1,
                                                        double* proj2);

extern "C" __global__ void rho2D_kernel(const double* proj1,
                                        const double* proj2, size_t N,
                                        const double* q1vals, size_t M1,
                                        const double* q2vals, size_t M2,
                                        cuDoubleComplex* pos_pos,
                                        cuDoubleComplex* pos_neg);

void Rho2D::calculateRhoGPU(const GPUMemoryManager& gpuMemory) {
  size_t N = gpuMemory.getNumScatteringPoints();

  // Allocate GPU projections
  double *d_proj1, *d_proj2;
  cudaMalloc(&d_proj1, N * sizeof(double));
  cudaMalloc(&d_proj2, N * sizeof(double));

  // Upload both q-axes to GPU
  double *d_q1Axis, *d_q2Axis;
  cudaMalloc(&d_q1Axis, 3 * sizeof(double));
  cudaMalloc(&d_q2Axis, 3 * sizeof(double));

  cudaMemcpy(d_q1Axis, qPlane.q1Vector.qAxis.data(), 3 * sizeof(double),
             cudaMemcpyHostToDevice);
  cudaMemcpy(d_q2Axis, qPlane.q2Vector.qAxis.data(), 3 * sizeof(double),
             cudaMemcpyHostToDevice);

  // Compute both projections
  dim3 block(256);
  dim3 grid((N + 255) / 256);

  compute_projection_2d_kernel<<<grid, block>>>(
      gpuMemory.getDeviceScatteringCenters(), d_q1Axis, d_q2Axis, N, d_proj1,
      d_proj2);

  // Compute rho with GPU projections
  computeRho2D_GPUprojections(d_proj1, d_proj2, N);

  // Cleanup
  cudaFree(d_proj1);
  cudaFree(d_proj2);
  cudaFree(d_q1Axis);
  cudaFree(d_q2Axis);
}

void Rho2D::computeRho2D_GPUprojections(double* d_proj1, double* d_proj2,
                                        size_t N) {
  size_t M1 = qPlane.q1Vector.qqmax;
  size_t M2 = qPlane.q2Vector.qqmax;
  size_t P = M1 * M2;

  // Flatten rho arrays
  std::vector<cuDoubleComplex> h_pp(P), h_pn(P);
  for (size_t i = 0; i < M1; ++i) {
    for (size_t j = 0; j < M2; ++j) {
      h_pp[i * M2 + j] =
          make_cuDoubleComplex(pos_pos[i][j].real(), pos_pos[i][j].imag());
      h_pn[i * M2 + j] =
          make_cuDoubleComplex(pos_neg[i][j].real(), pos_neg[i][j].imag());
    }
  }

  // Allocate GPU memory
  double *d_q1, *d_q2;
  cuDoubleComplex *d_pp, *d_pn;

  cudaMalloc(&d_q1, M1 * sizeof(double));
  cudaMalloc(&d_q2, M2 * sizeof(double));
  cudaMalloc(&d_pp, P * sizeof(cuDoubleComplex));
  cudaMalloc(&d_pn, P * sizeof(cuDoubleComplex));

  // Copy to device
  cudaMemcpy(d_q1, qPlane.q1Vector.qValues.data(), M1 * sizeof(double),
             cudaMemcpyHostToDevice);
  cudaMemcpy(d_q2, qPlane.q2Vector.qValues.data(), M2 * sizeof(double),
             cudaMemcpyHostToDevice);
  cudaMemcpy(d_pp, h_pp.data(), P * sizeof(cuDoubleComplex),
             cudaMemcpyHostToDevice);
  cudaMemcpy(d_pn, h_pn.data(), P * sizeof(cuDoubleComplex),
             cudaMemcpyHostToDevice);

  cudaError_t err = cudaGetLastError();
  if (err != cudaSuccess) {
    printf("CUDA kernel launch error: %s\n", cudaGetErrorString(err));
    std::abort();
  }

  int blockSizeX = 32;
  int blockSizeY = 8;  // MUST match BATCH_SIZE in the kernel

  size_t chunk_size = 4096;
  int gridX = (N + chunk_size - 1) / chunk_size;

  // Shared memory (sine and cosine)
  size_t shared_mem_bytes = blockSizeY * (M1 + M2) * 2 * sizeof(double);

  dim3 grid(gridX);
  dim3 block(blockSizeX, blockSizeY);

  rho2D_kernel<<<grid, block, shared_mem_bytes>>>(d_proj1, d_proj2, N, d_q1, M1,
                                                  d_q2, M2, d_pp, d_pn);

  // Copy back
  cudaMemcpy(h_pp.data(), d_pp, P * sizeof(cuDoubleComplex),
             cudaMemcpyDeviceToHost);
  cudaMemcpy(h_pn.data(), d_pn, P * sizeof(cuDoubleComplex),
             cudaMemcpyDeviceToHost);

  err = cudaGetLastError();
  if (err != cudaSuccess) {
    printf("CUDA kernel runtime error: %s\n", cudaGetErrorString(err));
    std::abort();
  }

  // Store back
  for (size_t i = 0; i < M1; ++i) {
    for (size_t j = 0; j < M2; ++j) {
      pos_pos[i][j] = std::complex<double>(cuCreal(h_pp[i * M2 + j]),
                                           cuCimag(h_pp[i * M2 + j]));
      pos_neg[i][j] = std::complex<double>(cuCreal(h_pn[i * M2 + j]),
                                           cuCimag(h_pn[i * M2 + j]));
    }
  }

  cudaFree(d_q1);
  cudaFree(d_q2);
  cudaFree(d_pp);
  cudaFree(d_pn);
}