import math as m
import os

class Box:

    a = 2
    b = 4
    c = 18
    cm = []
    R0 = []
    R1 = []
    R2 = []

class Cylinder:

    D = 2
    L = 4
    cm = []
    R0 = []
    R1 = []
    R2 = []

confFolder = "FinalConfigurations"

outputFolder = "Cylinders"

cofigurations = os.listdir(confFolder)

for conf in cofigurations:

    boxes = []

    file = open(f"{confFolder}/{conf}", "r")

    #Read first line for box
    line = file.readline()

    res = line.strip()
    res = res.split()

    Lbox = [float(res[0]), float(res[1]), float(res[2])]

    #Read other lines
    line = file.readline()

    while line:
        res = line.strip()
        res = res.split()

        box = Box()

        box.a = float(res[1])
        box.b = float(res[2])
        box.c = float(res[3])

        box.cm = [float(res[4]), float(res[5]), float(res[6])]

        box.R0 = [float(res[7]), float(res[8]), float(res[9])]
        box.R1 = [float(res[10]), float(res[11]), float(res[12])]
        box.R2 = [float(res[13]), float(res[14]), float(res[15])]
        
        boxes.append(box)

        line = file.readline()  # Read the next line

    # Close the file
    file.close() 


    cylinders = []

    for box in boxes:

        rotation = 0
        if box.a > box.b:
            rotation = 1

        theta = m.acos(box.R1[1])

        ncylinders = [-0.5, 0.5]

        if max(box.a, box.b) == 6:
            ncylinders = [-1, 0, 1]
        elif max(box.a, box.b) == 8:
            ncylinders = [-1.5, -0.5, 0.5, 1.5]
        elif max(box.a, box.b) == 10:
            ncylinders = [-2, -1, 0, 1, 2]

        for ncyl in ncylinders:

            cyl = Cylinder()

            cyl.cm = box.cm[:]
            if rotation == 0:
                cyl.R0 = box.R0[:]
                cyl.R1 = box.R1[:]
                cyl.R2 = box.R2[:]
            else:
                
                cyl.R0 = [0, -m.cos(theta), -m.sin(theta)]
                cyl.R1 = [1, 0, 0]
                cyl.R2 = [0, -m.sin(theta), m.cos(theta)]

            cyl.L = box.c
            cyl.D = max(box.a, box.b)/len(ncylinders)

            shift = float(ncyl) * cyl.D

            if rotation == 0:
                cyl.cm[1] += shift * m.cos(theta)
                cyl.cm[2] += shift * m.sin(theta)
            else:
                cyl.cm[0] += shift

            #print(ncyl, cyl.cm)
            cylinders.append(cyl)


    ofile = f"{outputFolder}/{conf}"

    with open(ofile, "w") as f:

        f.write(f"{Lbox[0]} {Lbox[1]} {Lbox[2]}\n")

        for cyl in cylinders:
            #f.write(f"{cyl.cm[0]} {cyl.cm[1]} {cyl.cm[2]} {cyl.R2[0]} {cyl.R2[1]} {cyl.R2[2]} @ {0.5 * cyl.D} {cyl.L} C[green]\n")
            f.write(f"CYL {cyl.D} {cyl.L} {cyl.cm[0]} {cyl.cm[1]} {cyl.cm[2]} {cyl.R0[0]} {cyl.R0[1]} {cyl.R0[2]} {cyl.R1[0]} {cyl.R1[1]} {cyl.R1[2]} {cyl.R2[0]} {cyl.R2[1]} {cyl.R2[2]}\n")
