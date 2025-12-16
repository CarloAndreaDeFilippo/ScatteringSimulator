import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

dataFoldName = Path("../Data")

foldname = dataFoldName / "rho1D"

configurationFolders = [fold for fold in foldname.iterdir() if fold.is_dir()]

axesNames = [f.stem for f in configurationFolders[0].iterdir() if f.is_file()]

averageData = []

for axisName in axesNames:
    all_dfs = []
    for confFold in configurationFolders:
        df = pd.read_csv(confFold / f"{axisName}.txt", header=None, sep=" ", names=["q", "I"])
        all_dfs.append(df)

    #Stack all configurations into one DataFrame (ignoring empties)
    all_data = pd.concat([d for d in all_dfs if not d.empty], ignore_index=True)

    #Get average data
    avg = all_data.groupby(["q"], as_index=False)["I"].mean()
    averageData.append(avg)

    #Save averaged data
    avg.to_csv(dataFoldName / f"avAxis_{axisName}.txt", sep=" ", index=None)

#Selected range
q_min = 0.1
q_max = 4.

#Max and min values in the selected range
I_min = min(axis.loc[(axis['q'] >= q_min) & (axis['q'] <= q_max), 'I'].min() for axis in averageData)
I_max = max(axis.loc[(axis['q'] >= q_min) & (axis['q'] <= q_max), 'I'].max() for axis in averageData)

#Plot of all the axes
plt.figure(figsize=(8,6))
plt.loglog()
plt.xlim(0.9 * q_min, 1.1 * q_max)
plt.ylim(0.9 * I_min, 1.1 * I_max)

for na, axis in enumerate(averageData):
    plt.plot(axis['q'], axis['I'], 'o--', markersize=2, label=f"Axis {axesNames[na]}")

plt.xlabel("q")
plt.ylabel("I")
plt.legend()
plt.savefig(dataFoldName / f"scatt1D.png", format="png", bbox_inches="tight", dpi=600)