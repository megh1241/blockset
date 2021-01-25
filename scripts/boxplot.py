import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import matplotlib
import os, sys

save_dir = '/home/ubuntu/pacset/plots'
head_dir = 'logs'
layout_filepath_list = [
                        'Blocks_binwdfsthreads_1intertwine_6.csv',
                        'Blocks_binblockstatthreads_1intertwine_6.csv',
                        ]

paths = [os.path.join(head_dir, fpath) for fpath in layout_filepath_list]
csv_list = [np.genfromtxt(path, delimiter=',') for path in paths]
csv_list_clean = [item[~np.isnan(item)] for item in csv_list]
layout_names = ['bin weighted\n dfs', 'bin weighted\n blockwise\n dfs']

data_dict = {}
print(len(layout_names))
print(len(csv_list_clean))
for key, value in zip(layout_names, csv_list_clean):
    print(key)
    print(value)
    data_dict[key] = value

boxplot_df = pd.DataFrame(data_dict)

sns.set(font_scale=1.22)
sns.set(style="whitegrid")
ax = sns.violinplot(data=boxplot_df, split=True, inner='box')
ax.set(xlabel='layout', ylabel='Number of I/O Blocks(4K)')
plt.tight_layout()
plt.savefig(os.path.join(save_dir, 'Layout_vs_blocks_123.png'), dpi=150)
