import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import joypy, matplotlib
import os, sys
from matplotlib import cm

'''
Run Instructions:
python3 boxplot_layout.py <block/latency> <violin/joy> <bin/no>
'''

block_or_lat = str(sys.argv[1])
bin_or_not = str(sys.argv[2])

round_dig = 2
plot_y_offset = 3500
plot_x_offset = -0.3

layout_names = ['BIN BFS', 'BIN DFS', 'BIN WDFS', 'BIN BLOCK WDFS']
#binstatdfsthreads_16intertwine_4.csv
if bin_or_not == 'bin':
    layout_list = [
                '_binbfsthreads_16intertwine_4.csv',
                '_bindfsthreads_16intertwine_4.csv',
                '_binstatdfsthreads_16intertwine_4.csv',
                '_binstatblockthreads_16intertwine_4.csv',
                ]

else:
    layout_list = [
                '_bfsthreads_16intertwine_4.csv',
                '_dfsthreads_16intertwine_4.csv',
                '_statdfsthreads_16intertwine_4.csv',
                '_statblockthreads_16intertwine_4.csv',
                ]


head_dir = 'logs_c_gbt2'

layout_filepath_list = [str(block_or_lat + layout_name) for layout_name in layout_list]

paths = [os.path.join(head_dir, fpath) for fpath in layout_filepath_list]
csv_list = [np.genfromtxt(path, delimiter=',') for path in paths]
csv_list_clean = [item[~np.isnan(item)] for item in csv_list]

id_arr = []
latency_arr = []
layout_arr = []
for idd,i in enumerate(csv_list_clean):
    length  = len(i)
    temp  = [j for j in range(length)]
    layouts = [layout_names[idd] for j in range(length)]
    latency_arr.extend(i.tolist())
    id_arr.extend(temp)
    layout_arr.extend(layouts)


data_dict = {}

mean_list = []
y_pos_list = []

for count, arr in enumerate(csv_list_clean):
    mean = round(np.mean(arr), round_dig)
    mean_list.append(mean)
    y_pos_list.append(np.max(arr) + plot_y_offset)

'''
for key, value in zip(layout_names, csv_list_clean):
    data_dict[key] = value
'''

data_dict['Observation'] = id_arr
data_dict['Latency'] = latency_arr
data_dict['Layout'] = layout_arr
boxplot_df = pd.DataFrame(data_dict)

boxplot_df.to_pickle('Regression_GBT_Layout_vs_' + block_or_lat + '_'+ sys.argv[2]+ '.pkl')

print boxplot_df.columns

plt.rc("font", size=19.5)

fig, ax = joypy.joyplot(boxplot_df, ylabels = False, by="Layout", column="Latency", colormap=cm.Set2,
        grid=False, linewidth=1.45,
        legend=False, figsize=(6.5,4.6), overlap=0.6)
fig.subplots_adjust(bottom=0.147, left=0.015, right=0.9486, top=1.01)


plt.xlim(left=0)
if block_or_lat == 'latency':
    plt.xlabel('Latency(ms)', fontsize=19.5)
else:
    plt.xlabel('Number of I/O Blocks', fontsize=19.5)
if block_or_lat == 'latency':
    plt.xticks([0, 200, 400, 600, 800], fontsize=19.5)
else:
    plt.xticks([0, 1000, 2000, 3000, 4000, 5000], fontsize=19.5)
plt.show()

plt.ylabel('layout')
plt.rc("font", size=19.5)

fig.savefig('Regression_GBT_Layout_vs_' + block_or_lat + '_'+ sys.argv[2]+'.pdf', fbbox_inches='tight')
