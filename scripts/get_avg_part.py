import numpy as np
import sys

path = sys.argv[1]
delim = sys.argv[2]

b0 = np.genfromtxt(path, delimiter=delim)

b1 = b0[~np.isnan(b0)][0:10]
#b2 = b0[~np.isnan(b0)][3001:6000]

print(np.mean(b1))
#print(np.mean(b2))
