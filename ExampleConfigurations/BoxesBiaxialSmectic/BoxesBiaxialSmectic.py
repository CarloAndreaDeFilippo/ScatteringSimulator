import random
import math as m

#dictionary of parameters
par = {} 

with open("par.txt") as f:
    for line in f:
        (key, val) = line.split()
        par[key] = float(val)


N = par['N']
v0 = par['a'] * par['b'] * par['c']

#Particles in a Smectic (columnar-like) phase

ncols = int(N / par['nz'])  # number of columns

Lbox = [1., 1., 1.]

Lbox[2] = (par['nz'] + 1.) * par['dz']
Lbox[0] = m.sqrt( (par['N'] * v0) / (par['phi'] * Lbox[2]) )
Lbox[1] = Lbox[0]

#part_r = [[0 for i in range(3)] for j in range(int(N))]

part_r = []

R = [[1., 0., 0.], [0., 1., 0.], [0., 0., 1.]]

n_p = 0

first_particles = []

# Place the first column

x = random.random() * (Lbox[0] - 0.5)
y = random.random() * (Lbox[1] - 0.5)
z = - 0.5 * Lbox[2]

#append particle to the list of part. of the first layer
first_particles.append([x, y, z])

# Append particles to particle list

for np in range(int(par['nz'])):

    z = par['dz'] * np - 0.5 * Lbox[2]
    part_r.append([x, y, z])    #append part to list of particles


# Insert other particles avoiding overlap

for nc in range(1, ncols, 1):

    print(nc)

    counter = 1
    overlap = True

    while overlap:

        if counter > 1000:

            print('Configuration not found.')
            exit(-1)

        overlap = False
        counter += 1 

        x = random.random() * (Lbox[0] - 0.5)
        y = random.random() * (Lbox[1] - 0.5)
        z = - 0.5 * Lbox[2]

        for part in first_particles:

            #print(f'{part[0]} {part[1]} {part[2]}')
            if abs( x - part[0] ) <= par['a']:

                if abs( y - part[1] ) <= par['b']:
        
                    overlap = True
    
    first_particles.append([x, y, z])

    for np in range(int(par['nz'])):

        z = par['dz'] * np - 0.5 * Lbox[2]
        part_r.append([x, y, z])


ofile = "particles.txt"

with open(ofile, "w") as fl:

    for n_p in range(int(N)):
        fl.write(f"BOX {par['a']} {par['b']} {par['c']} {part_r[n_p][0]} {part_r[n_p][1]} {part_r[n_p][2]} {R[0][0]} {R[0][1]} {R[0][2]} {R[1][0]} {R[1][1]} {R[1][2]} {R[2][0]} {R[2][1]} {R[2][2]}\n")
