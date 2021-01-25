import numpy as np
import sys

path = sys.argv[1]
delim = sys.argv[2]

b0 = np.genfromtxt(path, delimiter=delim)

b0 = b0[~np.isnan(b0)]

print(np.mean(b0))
