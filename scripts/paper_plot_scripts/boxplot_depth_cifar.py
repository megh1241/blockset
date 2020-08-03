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
plot_type = str(sys.argv[2])

round_dig = 2
plot_y_offset = 3500
plot_x_offset = -0.3

layout_names = [str(i) for i in range(9)]
layout_list = [ 'Blocks_binstatblockthreads_1intertwine_' + str(i) + '.csv' for i in range(9)]

save_dir = '/home/ubuntu/pacset/plots'
head_dir = '/home/ubuntu/pacset/scripts/paper_plot_scripts/logs_d'


paths = [os.path.join(head_dir, fpath) for fpath in layout_list]
csv_list = [np.genfromtxt(path, delimiter=',') for path in paths]
csv_list_clean = [item[~np.isnan(item)] for item in csv_list]

data_dict = {}

mean_list = []
y_pos_list = []

for count, arr in enumerate(csv_list_clean):
    print (layout_names[count], end=': ')
    mean = round(np.mean(arr), round_dig)
    mean_list.append(mean)
    y_pos_list.append(np.max(arr) + plot_y_offset)
    print(mean)

for key, value in zip(layout_names, csv_list_clean):
    data_dict[key] = value

boxplot_df = pd.DataFrame(data_dict)
x_pos_list = range(len(mean_list))

sns.set(font_scale=1.15)
sns.set(style="whitegrid")

if sys.argv[2] == 'violin' or sys.argv[2] == 'Violin':
    ax = sns.violinplot(data=boxplot_df, split=True, inner='box')
    if block_or_lat == 'blocks' or block_or_lat == 'Blocks':
        ax.set(xlabel='Number of levels interleaved', ylabel='Number of I/O Blocks')
    else:
        ax.set(xlabel='Number of levels interleaved', ylabel='Latency (ms)')

    for i in range(len(mean_list)):
        ax.text(x_pos_list[i] + plot_x_offset, y_pos_list[i], 'mean:'+ str(mean_list[i]), fontsize=6)

else:
    fig, ax = joypy.joyplot(boxplot_df)
    plt.ylabel('Number of levels interleaved')
    if block_or_lat == 'blocks' or block_or_lat == 'Blocks':
        plt.xlabel('Number of I/O Blocks')
    else:
        plt.xlabel('Latency(ms)')

plt.tight_layout()
plt.savefig(os.path.join(save_dir, 'Depth_Layout_vs_' + block_or_lat + '_'+ sys.argv[2]  +'.pdf'), format='pdf')
