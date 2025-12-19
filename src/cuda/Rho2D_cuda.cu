#include <cuComplex.h>
#include <cuda_runtime.h>

#include <iostream>

#include "../Rho2D.hpp"

extern "C" __global__ void rho2D_kernel_add(
    const double*, const double*, size_t,
    const double*, size_t,
    const double*, size_t,
    cuDoubleComplex*, cuDoubleComplex*);

void Rho2D::computeRhoCUDA(const std::vector<double>& projBase1, const std::vector<double>& projBase2) {
  size_t N = projBase1.size();
  size_t M1 = qPlane.q1Vector.qqmax;
  size_t M2 = qPlane.q2Vector.qqmax;
  size_t P = M1 * M2;

  // flatten rho arrays
  std::vector<cuDoubleComplex> h_pp(P), h_pn(P);
  for (size_t i = 0; i < M1; ++i)
    for (size_t j = 0; j < M2; ++j) {
      h_pp[i * M2 + j] = make_cuDoubleComplex(pos_pos[i][j].real(), pos_pos[i][j].imag());
      h_pn[i * M2 + j] = make_cuDoubleComplex(pos_neg[i][j].real(), pos_neg[i][j].imag());
    }

  // allocate GPU memory
  double *d_proj1, *d_proj2, *d_q1, *d_q2;
  cuDoubleComplex *d_pp, *d_pn;

  cudaMalloc(&d_proj1, N * sizeof(double));
  cudaMalloc(&d_proj2, N * sizeof(double));
  cudaMalloc(&d_q1, M1 * sizeof(double));
  cudaMalloc(&d_q2, M2 * sizeof(double));
  cudaMalloc(&d_pp, P * sizeof(cuDoubleComplex));
  cudaMalloc(&d_pn, P * sizeof(cuDoubleComplex));

  // copy to device
  cudaMemcpy(d_proj1, projBase1.data(), N * sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(d_proj2, projBase2.data(), N * sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(d_q1, qPlane.q1Vector.qValues.data(), M1 * sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(d_q2, qPlane.q2Vector.qValues.data(), M2 * sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(d_pp, h_pp.data(), P * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);
  cudaMemcpy(d_pn, h_pn.data(), P * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);

  // kernel launch
  dim3 block(16, 16);
  dim3 grid((M2 + block.x - 1) / block.x,
            (M1 + block.y - 1) / block.y);

  rho2D_kernel_add<<<grid, block>>>(d_proj1, d_proj2, N,
                                    d_q1, M1,
                                    d_q2, M2,
                                    d_pp, d_pn);

  cudaError_t err = cudaGetLastError();
  if (err != cudaSuccess) {
    printf("CUDA kernel launch error: %s\n", cudaGetErrorString(err));
    std::abort();
  }

  cudaDeviceSynchronize();

  err = cudaGetLastError();
  if (err != cudaSuccess) {
    printf("CUDA kernel runtime error: %s\n", cudaGetErrorString(err));
    std::abort();
  }

  // copy back
  cudaMemcpy(h_pp.data(), d_pp, P * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);
  cudaMemcpy(h_pn.data(), d_pn, P * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);

  // store back in pos_pos and pos_neg
  for (size_t i = 0; i < M1; ++i)
    for (size_t j = 0; j < M2; ++j) {
      pos_pos[i][j] = {cuCreal(h_pp[i * M2 + j]), cuCimag(h_pp[i * M2 + j])};
      pos_neg[i][j] = {cuCreal(h_pn[i * M2 + j]), cuCimag(h_pn[i * M2 + j])};
    }

  cudaFree(d_proj1);
  cudaFree(d_proj2);
  cudaFree(d_q1);
  cudaFree(d_q2);
  cudaFree(d_pp);
  cudaFree(d_pn);
}
