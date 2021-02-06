import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

bar_width  = 0.012
line_width = 2.5
opacity = 0.7
num_layouts=6

df = pd.read_csv('redis_layout_final.csv' , delimiter=',')
buck_arr = df.values[:,0]
lat_arr = df.values[:,1]

layout_names = ['BFS', 'DFS', 'BIN BFS', 'BIN DFS', 'BIN WDFS', 'BIN Blockwise WDFS']
index = np.arange(0, 0.2, 0.2/6)
ticks = range(1, num_layouts+1)

color_arr = ['plum', 'yellow', 'tomato', 'skyblue', 'fuchsia', 'greenyellow']
hatch_arr = ['\\', '||', '-', '++', '//', 'oo']

#fig, (ax1, ax2) =  plt.subplots(figsize = (1.5, 2.5))
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(3.8,2.65), sharex=True)

bucket_bar = ax1.bar(index,
                        buck_arr,
                        bar_width,
                        alpha=opacity,
                        ecolor='k',
                        edgecolor='k',
                    )

for i in range(num_layouts):
    bucket_bar[i].set_color(color_arr[i])
    bucket_bar[i].set_hatch(hatch_arr[i])
    bucket_bar[i].set_edgecolor('k')

lat_bar = ax2.bar(index,
                        lat_arr,
                        bar_width,
                        alpha=opacity,
                        ecolor='k',
                        edgecolor='k',
                    )

for i in range(num_layouts):
    lat_bar[i].set_color(color_arr[i])
    lat_bar[i].set_hatch(hatch_arr[i])
    lat_bar[i].set_edgecolor('k')


ax1.set_ylabel('Inference Latency(s)', fontsize='large')
#ax2.set_ylabel('Inference Latency(ms)', fontsize='large')
plt.tick_params(axis='both', which='major', labelsize='large')
plt.xticks(index, ticks, rotation=0)
ax1.set_xlabel('Layout (Cifar-10)', fontsize='large')
ax2.set_xlabel('Layout (HIGGS)', fontsize='large')
#plt.subplots_adjust(top=2.0)
plt.figlegend(bucket_bar, layout_names, frameon=False, bbox_to_anchor=(1.00,1.020), fontsize='medium', ncol=3, columnspacing=0.2)
#plt.legend(bucket_bar, layout_names, fontsize='small')
plt.tight_layout()

plt.subplots_adjust(top=0.62)
plt.tight_layout()
plt.savefig('Lambda_combined_figure.pdf', fbbox_inches='tight', bbox_inches='tight', format='pdf')
plt.show()



