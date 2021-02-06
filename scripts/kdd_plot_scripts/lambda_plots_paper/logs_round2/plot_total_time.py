import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import joypy, matplotlib
import os, sys
from  matplotlib import cm


bin_numbers = [2**i for i in range(8)]
total_latencies = []
for bin_num in bin_numbers:
    filename = str(bin_num) + '_bin'
    values = np.genfromtxt(filename, delimiter=',')
    values = list(values)
    #values.pop(-1)
    total_latencies.append(np.sum(values))


color_arr = ['plum', 'yellow', 'tomato', 'skyblue', 'fuchsia', 'greenyellow', 'red', 'green']
hatch_arr = ['\\', '||', '-', '++', '//', 'oo', '**', '~~']

index = np.arange(0, 0.2, 0.2/8)
ticks = range(1, 9)
bar_width  = 0.02
line_width = 2.5
opacity = 0.7

ax = plt.gca()


lat_bar = ax.bar(index,
                        total_latencies,
                        bar_width,
                        alpha=opacity,
                        ecolor='k',
                        edgecolor='k',
                    )

for i in range(8):
    lat_bar[i].set_color(color_arr[i])
    lat_bar[i].set_hatch(hatch_arr[i])
    lat_bar[i].set_edgecolor('k')


ax.set_ylabel('Total Latency(ms)', fontsize='x-large')
plt.tick_params(axis='both', which='major', labelsize='x-large')
plt.xticks(index, bin_numbers, rotation=0)
ax.set_xlabel('Number of bins', fontsize='x-large')
plt.subplots_adjust(left=0.16)

plt.savefig('/Users/admin/mlsys_2020_plot_scripts/lambda_plots_paper/Lambda_total_bar.pdf', bbox_inches='tight', format='pdf')
plt.show()

