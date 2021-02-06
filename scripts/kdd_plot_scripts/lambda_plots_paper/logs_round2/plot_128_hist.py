import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import matplotlib
import os, sys
from  matplotlib import cm


bin_num = 128
filename = str(bin_num) + '_bin'
values = np.genfromtxt(filename, delimiter=',')
values = list(values)
values.pop(-1)


cm = plt.cm.get_cmap('RdYlBu_r')
n, bins, patches = plt.hist(values, bins='auto')

col = (n-n.min())/(n.max()-n.min())
for c, p in zip(col, patches):
    plt.setp(p, 'facecolor', cm(c))


plt.xticks(fontsize='large')
plt.ylabel('Frequency (Number of Bins)', fontsize='x-large')
plt.xlabel('Latency (ms)', fontsize='x-large')
plt.tight_layout()
plt.savefig('../Lambda_128_hist.pdf', fbbox_inches='tight', bbox_inches='tight', format='pdf')
plt.show()
