import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


filename = "rho2D.txt"

df = pd.read_csv(
    filename,
    header=0,
    sep=" ",
    names=["q1", "q2", "I"],
)

#print(df)

pivotted= df.pivot_table(index="q2", columns="q1", values="I")

#sns.heatmap(pivotted)
ax = sns.heatmap(pivotted, vmin=0, vmax=1e3)
ax.invert_yaxis()
plt.xlabel('y')
plt.ylabel('z')
#plt.show()
plt.savefig("heatmap.png", format="png", bbox_inches="tight", dpi=600)