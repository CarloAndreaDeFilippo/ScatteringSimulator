# ScatteringSimulator

C++11 program that executes a scattering simulation of a system of particles of different shape, multithreaded with OpenMP.

## Theoretical background

Each particle in the system is substituted with a random cloud of scattering points. The program computes the static structure factor of the input set of particles [^1]:

$$S(\vec{q}) = \left\langle \frac{1}{N} \rho_{\vec{q}} \rho_{-\vec{q}} \right\rangle$$

where $N$ is the number of scattering points, $\langle ... \rangle$ is the average over different configurations, $\vec{q}$ is the direction of the scattering vector (i.e. the direction of the scattered wave), and $\rho_{\vec{q}}$ is the Fourier transform of the microscopic density:

$$\rho_{\vec{q}} = \sum_i^N e^{- i \vec{q} \cdot \vec{r}_i}$$

where $\vec{r}_i$ is the position of the scattering points.

## Installation

At the moment, only Linux platforms are available.

### Requirements

* C++11 compiler (g++)
* OpenMP for parallelization

To build:

```
cd ScatteringSimulator
make -j
```

## Documentation

The code takes as input two files: the particle configuration (particles shapes and positions) and the simulation configuration file (which type of simulation the user wants to perform).

### Particle configuration file

The input file first row contains the length $(Lx, Ly, Lz)$ of the box, e.g. ```50 50 50```. Then, each row represents a particle in the system.

Different shapes of particles are implemented:
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

In the docs folder there is a configuration example with all the available particles:

![Available particle shapes](/docs/particleShapesExample.png)

In the following, the shape names and their attributes for all the available shapes.

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
where $D$ is the diameter of the cylinder and $L$ is its length. The total length of the particle is ```L + D```.

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

In the second file are present all the simulation related info, such as the type of simulation or the mesh density of the scattering points.


[^1]: ACS Nano 2022, 16, 2, 2558–2568