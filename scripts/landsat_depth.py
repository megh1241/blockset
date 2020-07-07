import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import matplotlib
import os, sys

save_dir = '/home/ubuntu/pacset/plots'
head_dir = '/home/ubuntu/pacset/scripts/landasat_depth'
layout_filepath_list = [
                        'latency_blockstatthreads_1intertwine_0.csv',
                        'latency_binblockstatthreads_1intertwine_2.csv',
                        ]

paths = [os.path.join(head_dir, fpath) for fpath in layout_filepath_list]
csv_list = [np.genfromtxt(path, delimiter=',') for path in paths]
csv_list_clean = [item[~np.isnan(item)] for item in csv_list]
layout_names = ['No interleaving',  '2 levels']

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
#ax.set(xlabel='layout', ylabel='Number of I/O Blocks(4K)')
ax.set(xlabel='Depth Intertwined', ylabel='Latency (ms)')
plt.tight_layout()
plt.savefig(os.path.join(save_dir, 'Depth_vs_latency.png'), dpi=150)
