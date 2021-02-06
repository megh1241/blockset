import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

params = {'legend.fontsize': 'large',
          'figure.figsize': (20, 8),
         'axes.labelsize': 'x-large',
         'axes.titlesize':'x-large',
         'xtick.labelsize':'x-large',
         'ytick.labelsize':'x-large'}
plt.rcParams.update(params)

df = pd.read_csv('redis_blocksize.csv', index_col=0, delimiter=',', skipinitialspace=True)
ax = df.plot( kind= 'bar' , secondary_y= 'Latency per read(ms)', rot= 0 )
plt.show()
