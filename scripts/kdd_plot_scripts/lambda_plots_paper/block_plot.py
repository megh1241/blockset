import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

bar_width  = 0.012
line_width = 2.5
opacity = 0.7
num_layouts=5

df = pd.read_csv('blocksize_new.csv' , delimiter=',')
buck_arr = df.values[:,0]
lat_arr = df.values[:,1]

block_widths = [4, 8, 16, 32, 64]
index = np.arange(0, 0.1, 0.1/5)
ticks = range(1, num_layouts+1)

color_arr = ['yellow', 'tomato', 'skyblue', 'fuchsia', 'greenyellow']
hatch_arr = ['||', '-', '++', '//', 'oo']

#fig, (ax1, ax2) =  plt.subplots(figsize = (1.5, 2.5))
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(9,5), sharex=True)

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


ax1.set_ylabel('Number of Blocks Read', fontsize='x-large')
ax2.set_ylabel('Latency per read', fontsize='x-large')
plt.tick_params(axis='both', which='major', labelsize='x-large')
plt.xticks(index, ticks, rotation=0)
ax1.set_xlabel('Block size (Number of nodes per block)', fontsize='x-large')
ax2.set_xlabel('Block size (Number of nodes per block)', fontsize='x-large')
#plt.subplots_adjust(top=2.8)
plt.figlegend(bucket_bar, block_widths, bbox_to_anchor=(0.485,0.9), fontsize='medium', ncol=5, columnspacing=0.2 )

#plt.legend(bucket_bar, layout_names, fontsize='small')
#plt.tight_layout()

#plt.subplots_adjust(top=0.8)
plt.tight_layout()
plt.savefig('Lambda_blocks.pdf', fbbox_inches='tight', bbox_inches='tight', format='pdf')
plt.show()



