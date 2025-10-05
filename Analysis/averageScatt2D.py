import pandas as pd
import matplotlib.pyplot as plt
import os

dataFoldName = "../Data"

foldname = f"{dataFoldName}/rho2D" 

configurationFolders = [f"{foldname}/{f}" for f in os.listdir(foldname)]

planeNames = [os.path.splitext(f)[0] for f in os.listdir(configurationFolders[0])]

averageData = []

for planeName in planeNames:
    all_dfs = []
    for confFold in configurationFolders:
        df = pd.read_csv(f"{confFold}/{planeName}.txt",
                         header=None, sep=" ", names=["q1", "q2", "I"])
        all_dfs.append(df)

    #Stack all configurations into one DataFrame (ignoring empties)
    all_data = pd.concat([d for d in all_dfs if not d.empty], ignore_index=True)

    #Get average data
    avg = all_data.groupby(["q1", "q2"], as_index=False)["I"].mean()
    averageData.append(avg)

    #Plot plane
    grid = avg.pivot_table(index="q2", columns="q1", values="I", aggfunc="mean")
    X = grid.index.values
    Y = grid.columns.values
    Z = grid.values

    plt.figure(figsize=(8,6))
    plt.pcolormesh(Y, X, Z, shading="auto", cmap="viridis")
    plt.colorbar(label="I")
    plt.xlabel("q1")
    plt.ylabel("q2")
    plt.savefig(f"{dataFoldName}/heatmap_{planeName}.png", format="png", bbox_inches="tight", dpi=600)
    plt.close()

    #Save averaged data
    avg.to_csv(f"{dataFoldName}/avPlane_{planeName}.txt", sep=" ", index=None)
