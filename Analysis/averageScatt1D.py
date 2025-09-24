import pandas as pd
import matplotlib.pyplot as plt
import os

dataFoldName = "../Data"

foldname = f"{dataFoldName}/rho1D"

configurationFolders = [f"{foldname}/{f}" for f in os.listdir(foldname)]

axesNames = [os.path.splitext(f)[0] for f in os.listdir(configurationFolders[0])]

averageData = [pd.read_csv(f"{configurationFolders[0]}/{f}",header=0, sep=" ", names=["q", "I"]) for f in os.listdir(configurationFolders[0])]

for confFold in configurationFolders[1:]:

    rhoFiles = [f"{confFold}/{f}" for f in os.listdir(confFold)]
    df = [pd.read_csv(file,header=0, sep=" ", names=["q", "I"]) for file in rhoFiles]

    for axis in range(len(df)):
        averageData[axis]["I"] = averageData[axis]["I"] + df[axis]["I"]


for axis in range(len(averageData)):
        averageData[axis]["I"] /= len(configurationFolders)

plt.loglog()
plt.xlim(0.1, 3.5)

for na, axis in enumerate(averageData):
    plt.plot(axis['q'], axis['I'], 'o--', markersize=2, label=f"Axis {axesNames[na]}")

plt.legend()
plt.savefig(f"{dataFoldName}/scatt1D.png", format="png", bbox_inches="tight", dpi=600)

for na, axisData in enumerate(averageData):
    axisData.to_csv(f"{dataFoldName}/avScatt{axesNames[na]}.txt", sep = " ", index=None)