import random
import math as m

#dictionary of parameters
par = {} 

with open("par.txt") as f:
    for line in f:
        (key, val) = line.split()
        par[key] = float(val)


N = par['nx'] * par['ny'] * par['nz']


#Particles in a Smectic (columnar-like) phase

Lbox = [100., 100., 100.]

part_r = [[0 for i in range(3)] for j in range(int(N))]


# Sm-C

#deg = 17.15

deg = 0 #Sm-A

theta = m.pi * deg / 180.

R = [[1., 0., 0.], [0., m.cos(theta), m.sin(theta)], [0., -m.sin(theta), m.cos(theta)]]

y_tilt = par['dz'] * m.tan(theta)


n_p = 0

for i in range(int(par['nx'])):

    #Remove crystallinity in the x direction by randomly translating the y direction
    y_phase = random.random() * par['b']    

    x_randomicity = 0.5 * par['dx'] * random.random()     #defines the noise in the x direction
    y_randomicity = 0.5 * par['dy'] * random.random()     #defines the noise in the y direction

    for j in range(int(par['ny'])):
        for k in range(int(par['nz'])):
            if n_p < N:
                part_r[n_p][0] = par['dx'] * i - 0.5 * Lbox[0] + x_randomicity  
                part_r[n_p][1] = par['dy'] * j - 0.5 * Lbox[1] + y_phase - y_tilt * k + y_randomicity
                part_r[n_p][2] = par['dz'] * k - 0.5 * Lbox[2]
            n_p += 1


ofile = "particles.txt"

with open(ofile, "w") as fl:

    for n_p in range(int(N)):
        fl.write(f"BOX {par['a']} {par['b']} {par['c']} {part_r[n_p][0]} {part_r[n_p][1]} {part_r[n_p][2]} {R[0][0]} {R[0][1]} {R[0][2]} {R[1][0]} {R[1][1]} {R[1][2]} {R[2][0]} {R[2][1]} {R[2][2]}\n")
