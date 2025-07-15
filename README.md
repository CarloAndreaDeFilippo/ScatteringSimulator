# ScatteringSimulator

C++11 program for simulating scattering in a system of particles with different shapes, parallelized with OpenMP.

## Theoretical background

Each particle in the system is represented by a random cloud of scattering points. The program computes the static structure factor of the input set of particles [^1]:

$$S(\vec{q}) = \left\langle \frac{1}{N} \rho_{\vec{q}} \rho_{-\vec{q}} \right\rangle$$

where $N$ is the number of scattering points, $\langle ... \rangle$ is the average over different configurations, $\vec{q}$ is the scattering vector, and $\rho_{\vec{q}}$ is the Fourier transform of the microscopic density:

$$\rho_{\vec{q}} = \sum_i^N e^{- i \vec{q} \cdot \vec{r}_i}$$

where $\vec{r}_i$ is the position of the scattering points.

## Installation

Currently, only Linux is supported.

### Requirements

* C++11 compiler (g++)
* OpenMP-enabled compiler (for parallelization)

To build:

```
cd ScatteringSimulator
make -j
```

To use once built:

```
./scatteringSimulator settings.json
```

## Documentation

The code takes two input files: the particle configuration (shapes and positions) and the simulation configuration (which defines the type of simulation to run).

### Particle configuration file

The first row of the input file contains the box dimensions $(Lx, Ly, Lz)$, e.g. ```50 50 50```. Each subsequent row represents a particle in the system.

The following particle shapes are supported:
* Sphere
* Cylinder
* Spherocylinder (or capsule)
* Box
* Ellipsoid
* Superquadric

Each line has the following structure:

```
SHAPE_NAME SHAPE_ATTRIBUTES x y z Rxx Rxy Rxz Ryx Ryy Ryz Rzx Rzy Rzz
```

where 
* ```SHAPE_NAME``` and ```SHAPE_ATTRIBUTES``` define the type of particle
* $(x, y, z)$ is the center of mass
* $R$ is the rotation matrix of the particle that describes its orientation.

The `docs` folder contains an example configuration with all available particle types:

![Available particle shapes](/docs/particleShapesExample.png)

Below are the shape names and their attributes for all available shapes.

#### Sphere
```
SPH D
```
where $D$ is the diameter of the sphere.

#### Cylinder
```
CYL D L
```
where $D$ is the diameter of the cylinder and $L$ is its length.

#### Spherocylinder
```
SPHCYL D L
```
where $D$ is the diameter of the cylinder and $L$ is its length. The total length of the spherocylinder is $L + D$.

#### Box
```
BOX a b c
```
where $(a, b, c)$ are the three axes of the box.

#### Ellipsoid
```
ELL s_a s_b s_c
```
where $(s_a, s_b, s_c)$ are the three semiaxes of the ellipsoid.

#### Superquadric
```
SQUAD s_a s_b s_c r s t
```
where $(s_a, s_b, s_c)$ are the three semiaxes of the superquadric, and $(r, s, t)$ are the exponents that define the superquadric:
$$\left|\frac{x}{s_a} \right|^r + \left|\frac{x}{s_b} \right|^s + \left|\frac{x}{s_c} \right|^t = 1 $$

### Simulation configuration file

The second file, in the JSON format, contains all simulation-related information, such as the type of simulation and the mesh density of the scattering points. An example  ```settings.json``` is provided.
In particular, it is possible to choose:
* ```simType``` (type of simulation): only ```1D``` at the moment
* ```scattType``` (type of scattering): 
    * ```Sq``` for the structure factor: each particle is represented by a single scattering point in its center of mass
    * ```Iq``` for the full scattering intensity: each particle is represented by a random mesh of scattering points. The number of scattering points depends on the particle volume and the density of points ```rhoSP```, which can be modified in the JSON file.
* ```scattVectors``` (scattering vector data to be computed): it describes a series of directions (```direction``` in the JSON file), their ranges of $q$ values to compute (```qmin``` and ```qmax```), and the spacing between the $q$ values (```dq```). Note: a ```dq``` value too small (on the order of $2\pi / L_{box}$) can lead to finite-size effects.
* ```configurationFolder```: folder that contains all the configurations to analyze.
* ```outputFolder```: folder that will contain the output data.
* ```saveCogli2```: choose whether to save a visualization file compatible with [cogli2](https://sourceforge.net/projects/cogli1/), where each scattering point is represented by a sphere.
    * ```cogli2Folder```: folder that will contain the cogli2 output files.

### Output

For ```simType``` set to ```1D```, the program computes the scattering data along the specified axes and saves the results in the chosen output folder:
```
project
│
└───outputFolder
│   └───confName0
│       │   axis_0.txt
│       │   axis_1.txt
│       │   ...
│   └───confName1
│       │   axis_0.txt
│       │   axis_1.txt
│       │   ...
```



[^1]: ACS Nano 2022, 16, 2, 2558–2568, [https://doi.org/10.1021/acsnano.1c09208](https://doi.org/10.1021/acsnano.1c09208)
