#include <cuda_runtime.h>

extern "C" __global__ void compute_projection_1d_kernel(const double* centers,
                                                        const double* q_axis,
                                                        size_t N,
                                                        double* projections) {
  size_t tIdx = blockIdx.x * blockDim.x + threadIdx.x;

  if (tIdx >= N) return;

  double cx = centers[tIdx * 3 + 0];
  double cy = centers[tIdx * 3 + 1];
  double cz = centers[tIdx * 3 + 2];

  double qx = q_axis[0];
  double qy = q_axis[1];
  double qz = q_axis[2];

  projections[tIdx] = cx * qx + cy * qy + cz * qz;
}

extern "C" __global__ void compute_projection_2d_kernel(const double* centers,
                                                        const double* q1_axis,
                                                        const double* q2_axis,
                                                        size_t N, double* proj1,
                                                        double* proj2) {
  size_t tIdx = blockIdx.x * blockDim.x + threadIdx.x;

  if (tIdx >= N) return;

  double cx = centers[tIdx * 3 + 0];
  double cy = centers[tIdx * 3 + 1];
  double cz = centers[tIdx * 3 + 2];

  double q1x = q1_axis[0], q1y = q1_axis[1], q1z = q1_axis[2];
  double q2x = q2_axis[0], q2y = q2_axis[1], q2z = q2_axis[2];

  proj1[tIdx] = cx * q1x + cy * q1y + cz * q1z;
  proj2[tIdx] = cx * q2x + cy * q2y + cz * q2z;
}