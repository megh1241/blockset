import shutil
import sys
import os

num_copies = int(sys.argv[1])
original_dir = sys.argv[2]
pack_filename = sys.argv[3]


for i in range(num_copies):
        shutil.copy2(original_dir + pack_filename + '.bin', original_dir+pack_filename + str(i) + '.bin' )
