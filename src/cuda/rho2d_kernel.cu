#include <cuComplex.h>
#include <math.h>

extern "C" __global__ void rho2D_kernel_add(const double* proj1,
                                            const double* proj2, size_t N,
                                            const double* q1vals, size_t M1,
                                            const double* q2vals, size_t M2,
                                            cuDoubleComplex* pos_pos,
                                            cuDoubleComplex* pos_neg) {
  int qq1 = blockIdx.y * blockDim.y + threadIdx.y;
  int qq2 = blockIdx.x * blockDim.x + threadIdx.x;

  if (qq1 >= M1 || qq2 >= M2) return;

  double q1 = q1vals[qq1];
  double q2 = q2vals[qq2];

  cuDoubleComplex sumPP = make_cuDoubleComplex(0.0, 0.0);
  cuDoubleComplex sumPN = make_cuDoubleComplex(0.0, 0.0);

  for (size_t i = 0; i < N; ++i) {
    double phase1 = proj1[i] * q1;
    double phase2 = proj2[i] * q2;

    double anglePP = phase1 + phase2;
    double anglePN = phase1 - phase2;

    double sPP, cPP, sPN, cPN;
    sincos(anglePP, &sPP, &cPP);
    sincos(anglePN, &sPN, &cPN);
    sumPP = cuCadd(sumPP, make_cuDoubleComplex(cPP, -sPP));
    sumPN = cuCadd(sumPN, make_cuDoubleComplex(cPN, -sPN));
  }

  int idx = qq1 * M2 + qq2;

  pos_pos[idx] = cuCadd(pos_pos[idx], sumPP);
  pos_neg[idx] = cuCadd(pos_neg[idx], sumPN);
}
