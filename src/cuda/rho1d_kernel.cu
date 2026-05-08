#include <cuComplex.h>
#include <cuda.h>

#include <cmath>

extern "C" {

__global__ void rho1D_kernel(const double* proj, size_t nSP,
                             const double* qvals, size_t M,
                             cuDoubleComplex* rho) {
  int qq = blockIdx.y;
  if (qq >= M) return;

  double q = qvals[qq];
  size_t tIdx = blockIdx.x * blockDim.x + threadIdx.x;

  double sum_r = 0.0;
  double sum_i = 0.0;

  // Each thread calculates its single point's contribution
  if (tIdx < nSP) {
    double phase = proj[tIdx] * q;
    double sPhase, cPhase;
    sincos(phase, &sPhase, &cPhase);
    sum_r = sPhase;
    sum_i = -cPhase;
  }

  __shared__ double s_r[256];
  __shared__ double s_i[256];

  s_r[threadIdx.x] = sum_r;
  s_i[threadIdx.x] = sum_i;
  __syncthreads();

  // Parallel reduction to sum all the contributions
  for (int stride = blockDim.x / 2; stride > 0; stride >>= 1) {
    if (threadIdx.x < stride) {
      s_r[threadIdx.x] += s_r[threadIdx.x + stride];
      s_i[threadIdx.x] += s_i[threadIdx.x + stride];
    }
    __syncthreads();
  }

  if (threadIdx.x == 0) {
    double* rho_ptr = (double*)&rho[qq];  // Complex is two consecutive doubles

    atomicAdd(&rho_ptr[0], s_r[0]);
    atomicAdd(&rho_ptr[1], s_i[0]);
  }
}
}
