
# Compiler and compiler flags
CXX      := g++
NVCC     := nvcc
NVCCFLAGS := -std=c++11 -O3 -DUSE_CUDA -ccbin g++-11
CXXFLAGS := -std=c++11
LFLAGS   :=

# Build options for OMP and CUDA (0 false, 1 true)
USE_OPENMP  ?= 1
USE_CUDA ?= 0

# Directories
SRCDIR   := src
OBJDIR   := obj
EXECUTABLE := scatteringSimulator.out

# C++ sources
CPP_SOURCES := $(wildcard $(SRCDIR)/*.cpp)

# Remove CUDA-specific files from CPP list
CPP_SOURCES := $(filter-out $(SRCDIR)/Rho1D_cuda.cpp, $(CPP_SOURCES))
CPP_SOURCES := $(filter-out $(SRCDIR)/rho1d_kernel.cu, $(CPP_SOURCES))


# Additional CUDA flags
ifeq ($(USE_CUDA),1)
    NVCC_ARCH_FLAGS = -gencode arch=compute_61,code=sm_61
    CUDA_SOURCES := src/cuda/Rho1D_cuda.cu src/cuda/rho1d_kernel.cu src/cuda/Rho2D_cuda.cu src/cuda/rho2d_kernel.cu
    CXXFLAGS += -DUSE_CUDA
    CUDA_LFLAGS :=
else
    CUDA_SOURCES :=
endif

# Additional OpenMP flags
ifeq ($(USE_OPENMP),1)
    CXXFLAGS += -DUSE_OPENMP -fopenmp
    LFLAGS   += -fopenmp
endif


# Build selection
BUILD ?= release

ifeq ($(BUILD),debug)
  CXXFLAGS += -g -O2
else ifeq ($(BUILD),release)
  CXXFLAGS += -O3
else
  $(error Unknown build type '$(BUILD)'. Use BUILD=debug or BUILD=release)
endif

# Objects
CPP_OBJECTS  := $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(CPP_SOURCES)))
CUDA_OBJECTS := $(patsubst src/cuda/%.cu,$(OBJDIR)/%.cu.o,$(CUDA_SOURCES))



all: $(EXECUTABLE)

$(EXECUTABLE): $(CPP_OBJECTS) $(CUDA_OBJECTS)
ifeq ($(USE_CUDA),1)
	$(NVCC) $^ -o $@ $(CUDA_LFLAGS)
else
	$(CXX)  $^ -o $@ $(LFLAGS)
endif


$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile C++ with g++
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile CUDA with nvcc
$(OBJDIR)/%.cu.o: src/cuda/%.cu | $(OBJDIR)
	$(NVCC) $(NVCCFLAGS) $(NVCC_ARCH_FLAGS) -c $< -o $@


clean:
	rm -f $(OBJDIR)/*.o $(EXECUTABLE)
