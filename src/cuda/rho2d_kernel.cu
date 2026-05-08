#include <cuComplex.h>
#include <math.h>

#define BATCH_SIZE 8
#define TASKS_PER_PASS 16  // Number of (q1, q2) points each thread will compute

extern "C" {

__global__ void rho2D_kernel(const double* proj1, const double* proj2, size_t N,
                             const double* q1vals, size_t M1,
                             const double* q2vals, size_t M2,
                             cuDoubleComplex* pos_pos,
                             cuDoubleComplex* pos_neg) {
  // A block processes a chunk of 4096 points
  size_t CHUNK_SIZE = 4096;
  size_t start_pt = blockIdx.x * CHUNK_SIZE;
  size_t end_pt = start_pt + CHUNK_SIZE;
  if (end_pt > N) end_pt = N;
  if (start_pt >= N) return;

  int tIdx = threadIdx.y * blockDim.x + threadIdx.x;
  int num_threads = blockDim.x * blockDim.y;
  int P = M1 * M2;

  extern __shared__ double sharedMem[];
  double* s_C1 = sharedMem;
  double* s_S1 = s_C1 + BATCH_SIZE * M1;
  double* s_C2 = s_S1 + BATCH_SIZE * M1;
  double* s_S2 = s_C2 + BATCH_SIZE * M2;

  int total_passes =
      (P + (num_threads * TASKS_PER_PASS) - 1) / (num_threads * TASKS_PER_PASS);

  for (int pass = 0; pass < total_passes; ++pass) {
    int pass_q_start = pass * num_threads * TASKS_PER_PASS;

    int loc_q_start = pass_q_start + tIdx * TASKS_PER_PASS;
    int loc_q_end = loc_q_start + TASKS_PER_PASS;
    if (loc_q_end > P) loc_q_end = P;
    if (loc_q_start > P) loc_q_start = P;

    int num_loc_tasks = loc_q_end - loc_q_start;

    // Precomputing indices
    int loc_qq1[TASKS_PER_PASS];
    int loc_qq2[TASKS_PER_PASS];
    for (int k = 0; k < num_loc_tasks; ++k) {
      int global_q = loc_q_start + k;
      loc_qq1[k] = global_q / M2;
      loc_qq2[k] = global_q % M2;
    }

    // Local accumulators
    double loc_pp_r[TASKS_PER_PASS] = {0};
    double loc_pp_i[TASKS_PER_PASS] = {0};
    double loc_pn_r[TASKS_PER_PASS] = {0};
    double loc_pn_i[TASKS_PER_PASS] = {0};

    for (size_t i_base = start_pt; i_base < end_pt; i_base += BATCH_SIZE) {
      int current_batch =
          (end_pt - i_base < BATCH_SIZE) ? (end_pt - i_base) : BATCH_SIZE;

      // Computing sines and cosines with sincos
      int pt = threadIdx.y;
      if (pt < current_batch) {
        double p1 = proj1[i_base + pt];
        double p2 = proj2[i_base + pt];

        for (int q = threadIdx.x; q < M1; q += blockDim.x) {
          sincos(p1 * q1vals[q], &s_S1[pt * M1 + q], &s_C1[pt * M1 + q]);
        }
        for (int q = threadIdx.x; q < M2; q += blockDim.x) {
          sincos(p2 * q2vals[q], &s_S2[pt * M2 + q], &s_C2[pt * M2 + q]);
        }
      }
      __syncthreads();

      // Computing final phase with sum and difference identities
      for (int k = 0; k < num_loc_tasks; ++k) {
        int qq1 = loc_qq1[k];
        int qq2 = loc_qq2[k];

        double sum_pp_r = 0, sum_pp_i = 0;
        double sum_pn_r = 0, sum_pn_i = 0;

        for (int b = 0; b < current_batch; ++b) {
          double c1 = s_C1[b * M1 + qq1];
          double s1 = s_S1[b * M1 + qq1];
          double c2 = s_C2[b * M2 + qq2];
          double s2 = s_S2[b * M2 + qq2];

          sum_pp_r += (c1 * c2 - s1 * s2);
          sum_pp_i += (-s1 * c2 - c1 * s2);
          sum_pn_r += (c1 * c2 + s1 * s2);
          sum_pn_i += (s1 * c2 - c1 * s2);
        }

        loc_pp_r[k] += sum_pp_r;
        loc_pp_i[k] += sum_pp_i;
        loc_pn_r[k] += sum_pn_r;
        loc_pn_i[k] += sum_pn_i;
      }
      __syncthreads();
    }

    for (int k = 0; k < num_loc_tasks; ++k) {
      int global_q = loc_q_start + k;
      double* pp_ptr = (double*)&pos_pos[global_q];
      double* pn_ptr = (double*)&pos_neg[global_q];

      atomicAdd(&pp_ptr[0], loc_pp_r[k]);
      atomicAdd(&pp_ptr[1], loc_pp_i[k]);
      atomicAdd(&pn_ptr[0], loc_pn_r[k]);
      atomicAdd(&pn_ptr[1], loc_pn_i[k]);
    }
  }
}
}