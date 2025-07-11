import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

foldname = "rho2D/"

filenames = [foldname + f for f in os.listdir(foldname)]

df_tot = [pd.read_csv(fname, header=0, sep=" ", names=["q1", "q2", "I"]) for fname in filenames]


average_rho = df_tot[0][['q1', 'q2']].copy()

average_rho['I'] = sum(df['I'] for df in df_tot) / len(df_tot)


pivotted= average_rho.pivot_table(index="q2", columns="q1", values="I")

#sns.heatmap(pivotted)
ax = sns.heatmap(pivotted)
ax.invert_yaxis()
plt.xlabel('y')
plt.ylabel('z')
#plt.show()
plt.savefig("rho2D.png", format="png", bbox_inches="tight", dpi=600)