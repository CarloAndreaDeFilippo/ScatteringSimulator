#dictionary of parameters
par = {} 

with open("par.txt") as f:
    for line in f:
        (key, val) = line.split()
        par[key] = float(val)


N = par['nx'] * par['ny'] * par['nz']


#Particles in a SC lattice

#! Take as input the phi of the system 
Lbox = [100., 100., 100.]

part_r = [[0 for i in range(3)] for j in range(int(N))]


n_p = 0

for k in range(int(par['nz'])):
    for j in range(int(par['ny'])):
        for i in range(int(par['nx'])):
            if n_p < N:
                part_r[n_p][0] = par['dx'] * i - 0.5 * Lbox[0]
                part_r[n_p][1] = par['dy'] * j - 0.5 * Lbox[1]
                part_r[n_p][2] = par['dz'] * k - 0.5 * Lbox[2]
            n_p += 1


ofile = "particles.txt"

with open(ofile, "w") as fl:

    for n_p in range(int(N)):
        fl.write(f"BOX {par['a']} {par['b']} {par['c']} {part_r[n_p][0]} {part_r[n_p][1]} {part_r[n_p][2]} 1. 0. 0. 0. 1. 0. 0. 0. 1.\n")
