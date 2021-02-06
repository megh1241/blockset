import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import matplotlib
import os, sys
from  matplotlib import cm


timeline_x = 'timeline_x'
timeline_y = 'timeline_y'
bin_num_clean = np.genfromtxt(timeline_x, delimiter=',')
lat_clean = np.genfromtxt(timeline_y, delimiter=',')

#bin_num_clean = [item[~np.isnan(item)] for item in bin_num]
#lat_clean = [item[~np.isnan(item)] for item in lat]
markerline, stemlines, baseline = plt.stem(bin_num_clean, lat_clean)

plt.tick_params(axis='both', labelsize='large')

plt.setp(markerline, marker='D', markersize=2.5, markeredgecolor="orange", markeredgewidth=0.5)
plt.setp(stemlines, linestyle="-", color="black", linewidth=1 )
plt.setp( markerline, 'linewidth', 1)
plt.setp(markerline, 'markerfacecolor', 'r')

plt.xlabel('Bin Number (Increasing order of launch time)', fontsize='x-large')
plt.ylabel('Latency(ms)', fontsize='x-large')

plt.savefig('/Users/admin/mlsys_2020_plot_scripts/lambda_plots_paper/Lambda_conc_stem.pdf', fbbox_inches='tight', bbox_inches='tight', format='pdf')
plt.show()

