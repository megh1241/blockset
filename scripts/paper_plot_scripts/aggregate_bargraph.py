import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os

save_dir = '/home/ubuntu/pacset/plots'

params = {'legend.fontsize': 'x-large',
        'figure.figsize': (20, 8),
        'axes.labelsize': 'x-large',
        'axes.titlesize':'x-large',
        'xtick.labelsize':'x-large',
        'ytick.labelsize':'x-large'}

plt.rcParams.update(params)

df = pd.read_csv('aggregate.csv', index_col=0, delimiter=',', skipinitialspace=True)
ax = df.plot( kind= 'bar' , secondary_y='BIN+Blockwise weighted DFS latency(ms)' , rot= 0 )
plt.ylabel('Latency(ms)')
plt.savefig('aggregate.png', format='png')
plt.show()
