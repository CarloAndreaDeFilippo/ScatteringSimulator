#include <cuComplex.h>
#include <cuda.h>

#include <cmath>

extern "C" {

__global__ void rho1D_kernel_add(const double* proj, size_t N,
                                 const double* qvals, size_t M,
                                 cuDoubleComplex* rho) {
  size_t qq = blockIdx.x * blockDim.x + threadIdx.x;

  if (qq >= M) return;

  double q = qvals[qq];
  cuDoubleComplex sum = make_cuDoubleComplex(0.0, 0.0);

  for (size_t i = 0; i < N; ++i) {
    double phase = proj[i] * q;
    sum = cuCadd(sum, make_cuDoubleComplex(cos(phase), -sin(phase)));
  }

  rho[qq] = cuCadd(rho[qq], sum);
}
}
