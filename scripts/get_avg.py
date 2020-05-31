import numpy as np
import sys

path = sys.argv[1]

b0 = np.genfromtxt(path, delimiter='\n')


#b0 = b0[~np.isnan(b0)]
b0 = b0[~np.isnan(b0)][0:4]

print(np.mean(b0))
