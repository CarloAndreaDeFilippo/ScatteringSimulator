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

#Smectic C tilt

patch_range = 0.2

if par['dz'] > par['c'] + patch_range:

    theta = 0

else:

    theta = m.acos(par['dz'] / (par['c'] + patch_range))


#deg = 17.15 

#theta = m.pi * deg / 180.

R = [[1., 0., 0.], [0., m.cos(theta), m.sin(theta)], [0., -m.sin(theta), m.cos(theta)]]

y_tilt = par['dz'] * m.tan(theta)


rand_displacement = 0.25 * par['c']

part_r = [] #list of particles

first_particles = []    #list of particles of the first layer

# Insert first particles avoiding overlap

for nc in range(ncols):

    print(f'Col #{nc}')

    counter = 1
    overlap = True

    while overlap:

        if counter > 100000:

            print('Configuration not found.')
            exit(-1)

        overlap = False
        counter += 1 

        x = Lbox[0] * (random.random() - 0.5)
        y = Lbox[1] * (random.random() - 0.5) + random.random() * rand_displacement * m.cos(theta)
        #z = - 0.5 * Lbox[2] + 0.1 * par['dz'] * m.sin(2. * m.pi * (0.5 + y / Lbox[1]))
        z = - 0.5 * Lbox[2] + random.random() * rand_displacement * m.sin(theta)

        for part in first_particles:

            dist_x = x - part[0]
            dist_y = y - part[1]

            dist_x -= Lbox[0] * round(dist_x / Lbox[0])
            dist_y -= Lbox[1] * round(dist_y / Lbox[1])

            dist_x = abs( dist_x )
            dist_y = abs( dist_y )

            if dist_x <= par['a']:

                if dist_y * m.cos(theta) <= par['b']:
        
                    overlap = True
    
    first_particles.append([x, y, z])



# Append particles to particle list

for part in first_particles:

    x = part[0]
    y_part = part[1]
    z_part = part[2]

    for np in range(int(par['nz'])):

        y = y_part - y_tilt * np 
        z = z_part + par['dz'] * np 

        y -= Lbox[1] * round(y / Lbox[1])
        
        '''
        if y < - 0.5 * Lbox[1]:

            y += Lbox[1]

        elif y > 0.5 * Lbox[1]:

            y -= Lbox[1]
        '''
        part_r.append([x, y, z])    #append part to list of particles


ofile = "particles.txt"

with open(ofile, "w") as fl:

    for n_p in range(int(N)):
        fl.write(f"BOX {par['a']} {par['b']} {par['c']} {part_r[n_p][0]} {part_r[n_p][1]} {part_r[n_p][2]} {R[0][0]} {R[0][1]} {R[0][2]} {R[1][0]} {R[1][1]} {R[1][2]} {R[2][0]} {R[2][1]} {R[2][2]}\n")
