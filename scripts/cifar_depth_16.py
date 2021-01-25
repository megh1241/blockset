import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import matplotlib
import os, sys

save_dir = '/home/ubuntu/pacset/plots'
head_dir = '/home/ubuntu/pacset/scripts/logs_cifar_16_final'
layout_filepath_list = [
                        'Blocks_statdfsthreads_1intertwine_0.csv',
                        'Blocks_binstatdfsthreads_1intertwine_1.csv',
                        'Blocks_binstatdfsthreads_1intertwine_2.csv',
                        'Blocks_binstatdfsthreads_1intertwine_3.csv',
                        'Blocks_binstatdfsthreads_1intertwine_4.csv',
                        'Blocks_binstatdfsthreads_1intertwine_5.csv'
                        ]

paths = [os.path.join(head_dir, fpath) for fpath in layout_filepath_list]
csv_list = [np.genfromtxt(path, delimiter=',') for path in paths]
csv_list_clean = [item[~np.isnan(item)][0:1000] for item in csv_list]
layout_names = ['No interleaving', '1 levels', '2 levels', '3 levels', '4 levels', '5 levels']

data_dict = {}

for count, arr in enumerate(csv_list_clean):
    print (layout_names[count], end=': ')
    print (np.mean(arr))

for key, value in zip(layout_names, csv_list_clean):
    data_dict[key] = value

boxplot_df = pd.DataFrame(data_dict)

sns.set(font_scale=1.15)
sns.set(style="whitegrid")
ax = sns.violinplot(data=boxplot_df, split=True, inner='box')
ax.set(xlabel='layout', ylabel='Number of I/O Blocks(4K)')
#ax.set(xlabel='Depth Intertwined', ylabel='Latency (ms)')
plt.tight_layout()
plt.savefig(os.path.join(save_dir, 'Depth_vs_IO_stat.png'), dpi=150)
