import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os

params = {'legend.fontsize': 'x-large',
        'figure.figsize': (20, 8),
        'axes.labelsize': 'x-large',
        'axes.titlesize':'x-large',
        'xtick.labelsize':'x-large',
        'ytick.labelsize':'x-large'}

plt.rcParams.update(params)

df = pd.read_csv('aggregate.csv', delimiter=',', header=None)
latency_arr = df.values
bfs_latency = latency_arr[:, 0]
dfs_latency = latency_arr[:, 2]
binblockwdfs_latency = latency_arr[:, 1]


fig, ax = plt.subplots(figsize=(3.75,2.85))
index = np.arange(0,2.5,0.5)
bar_width = 0.12
line_width = 2.75
eline_width = 0.75
opacity = 0.7

bfs_series = plt.bar(index,
                        bfs_latency,
                        bar_width,
                        alpha=opacity,
                        label='BFS',
                        color='tomato',
                        ecolor='k',
                        edgecolor='k',
                        hatch='--'
                        )

dfs_series = plt.bar(index+bar_width,
                        dfs_latency,
                        bar_width,
                        alpha=opacity,
                        label='DFS',
                        color='skyblue',
                        ecolor='k',
                        edgecolor='k',
                        hatch='++\n++'
                        )

blockwdfs_series = plt.bar(index+2*bar_width,
                        binblockwdfs_latency,
                        bar_width,
                        alpha=opacity,
                        label='BIN Blockwise\nW-DFS',
                        color='greenyellow',
                        ecolor='k',
                        edgecolor='k',
                        hatch='oo'
                        )


datasets = ['Cifar', 'Landsat', 'Higgs', 'Year', 'WEC']
plt.subplots_adjust(left=0.1, right=0.9465, top=1.2)
plt.tight_layout()
plt.tick_params(axis='both', which='major', labelsize='medium')
plt.xticks(index, datasets, rotation=0)
plt.xlabel('Dataset', fontsize='large')
plt.ylabel('Latency(ms)', fontsize='large')
plt.legend(fontsize='medium')
plt.savefig('aggregate_figure.pdf', fbbox_inches='tight', format='pdf')
#plt.setp(ax.get_legend().get_title(), fontsize='x-large')
plt.show()
