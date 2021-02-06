import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import joypy, matplotlib
import os, sys


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
if bin_or_not == 'bin':
    layout_list = [
                '_binbfsthreads_1intertwine_2.csv',
                '_bindfsthreads_1intertwine_2.csv',
                '_binstatdfsthreads_1intertwine_2.csv',
                '_binstatblockthreads_1intertwine_2.csv',
                ]

else:
    layout_list = [
                '_bfsthreads_1intertwine_0.csv',
                '_dfsthreads_1intertwine_0.csv',
                '_statdfsthreads_1intertwine_0.csv',
                '_statblockthreads_1intertwine_0.csv',
                ]

head_dir = 'logs_gbt_higgs_nobin'

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
boxplot_df.to_pickle('GBT_Layout_vs_' + block_or_lat + '_'+ sys.argv[2]+ '.pkl')
print boxplot_df.columns
plt.rc("font", size=17.5)

fig, ax = joypy.joyplot(boxplot_df, by="Layout", column="Latency", 
        grid=False, fill=False, background='k', linecolor="w", linewidth=1.75,
        legend=False, figsize=(5.5,3.6), overlap=0.6)

fig.subplots_adjust(bottom=0.26)

if block_or_lat == 'latency':
    plt.xlabel('Latency(ms)', fontsize=17.5)
else:
    plt.xlabel('Number of I/O Blocks', fontsize=17.5)
plt.xticks(rotation=25, fontsize=17.5)
plt.show()

plt.rc("font", size=17.5)
plt.ylabel('layout')
plt.rc("font", size=17.5)
fig.savefig('GBT_Layout_vs_' + block_or_lat + '_'+ sys.argv[2]+'.pdf', fbbox_inches='tight')

