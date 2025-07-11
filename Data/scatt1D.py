import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

def smooth(y, box_pts):
    box = np.ones(box_pts)/box_pts
    y_smooth = np.convolve(y, box, mode='same')
    return y_smooth


foldname1 = "rho1D_1/"
foldname2 = "rho1D_2/"

rho1_files = [foldname1 + f for f in os.listdir(foldname1)]
rho2_files = [foldname2 + f for f in os.listdir(foldname2)]


#folders = [foldname + "A", foldname + "B", foldname + "C", foldname + "D", foldname + "E"]

#rho1_files = [fold + "/Data/rho1D_1.txt" for fold in folders]

df_rho1 = [pd.read_csv(file,header=0, sep=" ", names=["q", "I"]) for file in rho1_files]

average_rho1 = df_rho1[0][['q']].copy()

average_rho1['I'] = sum(df['I'] for df in df_rho1) / len(df_rho1)


#rho2_files = [fold + "/Data/rho1D_2.txt" for fold in folders]

df_rho2 = [pd.read_csv(file,header=0, sep=" ", names=["q", "I"]) for file in rho2_files]

average_rho2 = df_rho2[0][['q']].copy()

average_rho2['I'] = sum(df['I'] for df in df_rho2) / len(df_rho2)



plt.loglog()
#plt.xlim(0.1, 3.5)


#plt.plot(average_rho1['q'], smooth(average_rho1['I'],3), 'r-', lw=2)

plt.plot(average_rho1['q'], average_rho1['I'], 'o--', markersize=2, label = '$I_x$', zorder=1)
plt.plot(average_rho2['q'], average_rho2['I'], 'o--', markersize=2, label = '$I_z$', zorder=2)
plt.axhline(y = 1, color = 'black', label = '0')

plt.legend()

plt.savefig('1D.png', format="png", bbox_inches="tight", dpi=600)