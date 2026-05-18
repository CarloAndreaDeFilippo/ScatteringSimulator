# ScatteringSimulator

C++ program for simulating scattering in a system of particles with different shapes, parallelized with OpenMP or CUDA. Supports C++17 (default) with std::filesystem or C++11 with Boost for legacy cluster compatibility.

## Theoretical background

Each particle in the system is represented by a random cloud of scattering points. The program computes the static structure factor of the input set of particles [^1]:

$$S(\vec{q}) = \left\langle \frac{1}{N} \rho_{\vec{q}} \rho_{-\vec{q}} \right\rangle$$

where $N$ is the number of scattering points, $\langle ... \rangle$ is the average over different configurations, $\vec{q}$ is the scattering vector, and $\rho_{\vec{q}}$ is the Fourier transform of the microscopic density:

$$\rho_{\vec{q}} = \sum_i^N e^{- i \vec{q} \cdot \vec{r}_i}$$

where $\vec{r}_i$ is the position of the scattering points.

## Installation

Supported on Linux and Windows.

### Requirements

* CMake 3.18+
* C++17 compiler (or C++11 with Boost for legacy systems)

### Optional Dependencies

* **vcpkg**: Only required if using `-DUSE_BOOST=ON` for C++11 compatibility. Ensure the `VCPKG_ROOT` environment variable is set to your vcpkg installation directory.
* **Boost Libraries**: Only with `-DUSE_BOOST=ON` (boost::filesystem and boost::system)
* **OpenMP**: For CPU parallelization (optional)
* **CUDA**: For NVIDIA GPU parallelization (optional)

### Building

Building the project using CMake and presets:

#### Linux (Default: C++17, std::filesystem)

```
cmake --preset linux
cmake --build build -j
```

#### Windows

```
cmake --preset windows
cmake --build build --config Release -j
```

> NB: the presets have Visual Studio 18 2026 as generator, it is possible to change it with the CMake -G flag (e.g. Visual Studio 17 2022).

#### CMake options

It is possible to enable OpenMP and CUDA during preset choice:

| Option                | Description                         | Default value |
| :-------------------- | :---------------------------------- | :------------ |
| ```-DUSE_OPENMP=ON``` | Enable CPU parallelization          | ```OFF```     |
| ```-DUSE_CUDA=ON```   | Enable GPU parallelization (NVIDIA) | ```OFF```     |

> NB: If CUDA compiler is not automatically found by CMake, it is possible to specify its location with -DCMAKE_CUDA_COMPILER=COMPILER_PATH and -DCMAKE_CUDA_HOST_COMPILER=HOST_COMPILER_PATH (e.g. /usr/local/cuda-11.8/bin/nvcc and /usr/bin/g++-11, respectively).

### Usage

To run the program once built:

#### Linux

```
./build/scatteringSimulator SETTINGS_FILE
```

#### Windows

```
.\build\Release\scatteringSimulator.exe SETTINGS_FILE
```

If OpenMP is enabled, you can select the number of threads at runtime with ```OMP_NUM_THREADS=NUMBER_OF_THREADS```

> If the project is built with USE_CUDA=ON, the program will use the GPU regardless of the USE_OPENMP setting.


## Documentation

The code takes two input files: the particle configuration (shapes and positions) and the simulation configuration (which defines the type of simulation to run).

### Particle configuration file

The first row of the input file contains the box dimensions $(Lx, Ly, Lz)$, e.g. ```50 50 50```. Each subsequent row represents a particle in the system, which has the following structure:

```
SHAPE_NAME SHAPE_ATTRIBUTES x y z Rxx Rxy Rxz Ryx Ryy Ryz Rzx Rzy Rzz
```

where 
* ```SHAPE_NAME``` and ```SHAPE_ATTRIBUTES``` define the type of particle
* $(x, y, z)$ is the center of mass
* $R$ is the rotation matrix of the particle that describes its orientation.

#### Particle geometries

| Shape          | Name     | Shape attributes                               |
| :------------- | :------- | :--------------------------------------------- |
| Sphere         | `SPH`    | `D` (diameter)                                 |
| Cylinder       | `CYL`    | `D` (diameter), `L` (length)                   |
| Spherocylinder | `SPHCYL` | `D` (diameter), `L` (length of cylinder)       |
| Box            | `BOX`    | `a` `b` `c` (axes)                             |
| Ellipsoid      | `ELL`    | `s_a` `s_b` `s_c` (semiaxes)                   |
| Superquadric   | `SQUAD`  | `sa, sb, sc` (semiaxes), `r, s, t` (exponents) |

The `docs` folder contains an example configuration with all available particle types:

<img src="/docs/particleShapesExample.png" width=50% height=50%>

### Simulation configuration file

The second file ```SETTINGS_FILE```, in the JSON format (see [nlohmann json](https://github.com/nlohmann/json)), contains all simulation-related information, such as the type of simulation and the mesh density of the scattering points. An example  ```settings.json``` is provided.
In particular, it is possible to choose:
* ```scattType``` (type of scattering): 
    * ```Sq``` for the structure factor: each particle is represented by a single scattering point in its center of mass
    * ```Iq``` for the full scattering intensity: each particle is represented by a random mesh of scattering points. The number of scattering points depends on the particle volume and the density of points ```rhoSP```, which can be modified in the JSON file.
* ```scattVectors```: a list of directions along which the 1D scattering is computed. Each vector defines a ```direction``` (that will be normalized automatically), the range of scattering vector magnitudes $|\vec{q}|$ to evaluate (```qmin``` and ```qmax```), and the spacing between consecutive $|\vec{q}|$ values (```dq```). Note: a ```dq``` value too small (on the order of $2\pi / L_{box}$) can lead to finite-size effects.
* ```scattPlanes```: a list of planes along which the 2D scattering is computed. Each plane is specified by two vectors, ```q1Vector``` and ```q2Vector```, which are defined in the same way as the directions used for ```scattVectors```.
* ```configurationFolder```: folder that contains all the configurations to analyze.
* ```outputFolder```: folder that will contain the output data.
* ```saveCogli2```: choose whether to save a visualization file compatible with [cogli2](https://sourceforge.net/projects/cogli1/), where each scattering point is represented by a sphere.
    * ```cogli2Folder```: folder that will contain the cogli2 output files.

### Output

The program computes the scattering data and saves the results in the chosen output folder:
```
outputFolder
│
└─── rho1D
│   └───confName0
│       │   axis_0.txt
│       │   ...
│   └───confName1
│       │   axis_0.txt
│       │   ...
└─── rho2D
│   └───confName0
│       │   axis_0.txt
│       │   ...
│   └───confName1
│       │   axis_0.txt
│       │   ...
```

The ```Analysis``` folder contains two scripts used to analyze and plot the results from a set of different configurations of the same system, allowing you to obtain more accurate averaged results.

The ```averageScatt1D.py``` script processes 1D scattering data from multiple configurations, computes axis-wise averages, saves the averaged datasets to files in the data folder, and generates a combined plot showing the results for all axes.

Similarly, ```averageScatt2D.py``` processes 2D scattering data from multiple configurations across all selected planes, computes plane-wise averages, saves the averaged datasets to files in the data folder, and produces a heatmap plot for each plane.

[^1]: ACS Nano 2022, 16, 2, 2558–2568, [https://doi.org/10.1021/acsnano.1c09208](https://doi.org/10.1021/acsnano.1c09208)
