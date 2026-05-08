#pragma once

#pragma once

#include <cuda_runtime.h>

#include <array>
#include <vector>

class GPUMemoryManager {
 private:
  double* d_scatteringPoints = nullptr;
  size_t nSP = 0;

 public:
  GPUMemoryManager() = default;

  ~GPUMemoryManager() { cleanup(); }

  // Upload scattering points to GPU
  void uploadScatteringPoints(const std::vector<ScatteringPoint>& sPoints) {
    cleanup();  // Free previous data if any

    nSP = sPoints.size();

    // Allocate device memory: N points × 3 coordinates
    cudaMalloc(&d_scatteringPoints, nSP * 3 * sizeof(double));

    // Upload to GPU memory
    cudaMemcpy(d_scatteringPoints, sPoints.data(), nSP * 3 * sizeof(double),
               cudaMemcpyHostToDevice);
  }

  double* getDeviceScatteringCenters() const { return d_scatteringPoints; }

  size_t getNumScatteringPoints() const { return nSP; }

  // Free GPU memory
  void cleanup() {
    if (d_scatteringPoints != nullptr) {
      cudaFree(d_scatteringPoints);
      d_scatteringPoints = nullptr;
    }
    nSP = 0;
  }
};