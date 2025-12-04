import pandas as pd
import matplotlib.pyplot as plt
import os

dataFoldName = "../Data"

foldname = f"{dataFoldName}/rho1D"

configurationFolders = [f"{foldname}/{f}" for f in os.listdir(foldname)]

axesNames = [os.path.splitext(f)[0] for f in os.listdir(configurationFolders[0])]

averageData = []

for axisName in axesNames:
    all_dfs = []
    for confFold in configurationFolders:
        df = pd.read_csv(f"{confFold}/{axisName}.txt",
                         header=None, sep=" ", names=["q", "I"])
        all_dfs.append(df)

    #Stack all configurations into one DataFrame (ignoring empties)
    all_data = pd.concat([d for d in all_dfs if not d.empty], ignore_index=True)

    #Get average data
    avg = all_data.groupby(["q"], as_index=False)["I"].mean()
    averageData.append(avg)

    #Save averaged data
    avg.to_csv(f"{dataFoldName}/avAxis_{axisName}.txt", sep=" ", index=None)

#Plot of all the axes
plt.figure(figsize=(8,6))
plt.loglog()

for na, axis in enumerate(averageData):
    plt.plot(axis['q'], axis['I'], 'o--', markersize=2, label=f"Axis {axesNames[na]}")

plt.xlabel("q")
plt.ylabel("I")
plt.legend()
plt.savefig(f"{dataFoldName}/scatt1D.png", format="png", bbox_inches="tight", dpi=600)