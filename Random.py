import random
import math as m
import os
import shutil
import sys


#dictionary of parameters
par = {} 

if len(sys.argv) != 2:
    print("Specify the name of the parameter file")
    sys.exit(1)

par_file = sys.argv[1]

#par_file = input('Name of the parameters file: ')

with open(par_file) as f:
    for line in f:
        (key, val) = line.split()
        par[key] = float(val)


N = par['N']
v0 = par['a'] * par['b'] * par['c']

#Particles in a Smectic C (columnar-like) phase

ncols = int(N / par['nz'])  # number of columns

Lbox = [1., 1., 1.]

Lbox[2] = par['nz'] * par['dz']


#Smectic C tilt

patch_range = par['patch_range'] #additional distance due to interaction range

if par['dz'] > par['c'] + patch_range:

    theta = 0

else:

    theta = m.acos(par['dz'] / (par['c'] + patch_range))


R = [[1., 0., 0.], [0., m.cos(theta), m.sin(theta)], [0., -m.sin(theta), m.cos(theta)]]

y_tilt = par['dz'] * m.tan(theta)


Lbox[1] = (par['nz']) * y_tilt

Lbox[0] = (par['N'] * v0) / (par['phi'] * Lbox[1] * Lbox[2])


# Specify the path for the directory
folder_path = "Configurations/"

# Create the directory of the Configurations

initial_configuration = 0

if os.path.exists(folder_path):
    print(f"Directory '{folder_path}' already exists.")

    add_configurations = input('Do you want to add configurations? y, n\n')

    if add_configurations == 'y':

        current_configurations = os.listdir(folder_path)

        last_configuration = sorted(current_configurations, key=lambda x: int(x.split('.')[0]), reverse=True)[0]

        initial_configuration = int(last_configuration.split('.')[0]) + 1

        print(f'Number of current configurations: {initial_configuration}')


    else:

        delete_folder = input('Do you want to erase the directory? y, n\n')
        if delete_folder == 'y': 
            shutil.rmtree(folder_path)
            try:
                os.remove("file_names.txt")
            except OSError:
                pass

        else:
            exit(-1)

        os.makedirs(folder_path)

else:
    os.makedirs(folder_path)

rotation_probability = par['rot_prob']

#tot_configurations = input('Number of configurations: ')

tot_configurations = 50

file_names = []

wrong_configurations = 0


for nc in range(initial_configuration, int(tot_configurations), 1):

    #first_plane_z_position = Lbox[2] * (random.random() - 0.5)
    first_plane_z_position = -0.5 * Lbox[2]

    part_r = [] #list of particles

    first_particles = []    #list of particles of the first layer

    # Insert first particles avoiding overlap

    for ncol in range(ncols):

        print(f'Col #{ncol}')

        conf_found = True

        counter = 1
        overlap = True
        rotation = random.random()

        if rotation > rotation_probability:

            rotation = 0    #a b c
        
        else:

            rotation = 1    #b a c


        while overlap:

            if counter > 100000:

                print('Configuration not found.')
                conf_found = False
                break

            overlap = False
            counter += 1 

            #random_z_dispacement = 0.25 * par['dz'] * random.random()


            x = Lbox[0] * (random.random() - 0.5)
            y = Lbox[1] * (random.random() - 0.5)# + random_z_dispacement * m.sin(theta)
            #y = Lbox[1] * (random.random() - 0.5)
            #z = - 0.5 * Lbox[2] + 0.1 * par['dz'] * m.sin(2. * m.pi * (0.5 + y / Lbox[1]))
            z = first_plane_z_position# + random_z_dispacement * m.cos(theta)
            #z = first_plane_z_position

            for part in first_particles:

                dist_x = x - part[0]
                dist_y = y - part[1]

                dist_x -= Lbox[0] * round(dist_x / Lbox[0])
                dist_y -= Lbox[1] * round(dist_y / Lbox[1])

                dist_x = abs( dist_x )
                dist_y = abs( dist_y )

                part_rot = part[3]

                if rotation == 0:

                    if part_rot == 0:

                        if dist_x <= par['a'] and dist_y * m.cos(theta) <= par['b']:

                            overlap = True

                    else:

                        if dist_x <= 0.5 * (par['a'] + par['b']) and dist_y * m.cos(theta) <= 0.5 * (par['a'] + par['b']):

                            overlap = True

                else:

                    if part_rot == 0:

                        if dist_x <= 0.5 * (par['a'] + par['b']) and dist_y * m.cos(theta) <= 0.5 * (par['a'] + par['b']):

                            overlap = True

                    else:
                        if dist_x <= par['b'] and dist_y * m.cos(theta) <= par['a']:

                            overlap = True


        
        if conf_found == False:
            break

        first_particles.append([x, y, z, rotation])

    if conf_found == False:
        wrong_configurations += 1
        continue

    # Append particles to particle list

    for part in first_particles:

        x = part[0]
        y_part = part[1]
        z_part = part[2]
        rotation = part[3]

        for np in range(int(par['nz'])):

            y = y_part - y_tilt * np 
            z = z_part + par['dz'] * np 

            y -= Lbox[1] * round(y / Lbox[1])
            z -= Lbox[2] * round(z / Lbox[2])
            
            part_r.append([x, y, z, rotation])    #append part to list of particles

    ofile = str(folder_path) + str(nc - wrong_configurations) + ".txt"

    with open(ofile, "w") as fl:

        fl.write(f"{Lbox[0]} {Lbox[1]} {Lbox[2]}\n")

        for n_p in range(int(N)):

            if part_r[n_p][3] == 0:

                fl.write(f"BOX {par['a']} {par['b']} {par['c']} {part_r[n_p][0]} {part_r[n_p][1]} {part_r[n_p][2]} {R[0][0]} {R[0][1]} {R[0][2]} {R[1][0]} {R[1][1]} {R[1][2]} {R[2][0]} {R[2][1]} {R[2][2]}\n")

            else:
                
                fl.write(f"BOX {par['b']} {par['a']} {par['c']} {part_r[n_p][0]} {part_r[n_p][1]} {part_r[n_p][2]} {R[0][0]} {R[0][1]} {R[0][2]} {R[1][0]} {R[1][1]} {R[1][2]} {R[2][0]} {R[2][1]} {R[2][2]}\n")



    #file_names.append(ofile)

    with open("file_names.txt", "a") as fl:
        fl.write(f"{ofile}\n")
        
