import shutil
import sys
import os

num_copies = int(sys.argv[1])
original_dir = sys.argv[2]
pack_filename = sys.argv[3]
meta_filename = sys.argv[4]

#directory to copy into
new_dir = '/dat'

for i in range(num_copies):
        shutil.copy2(os.path.join(original_dir, pack_filename), new_dir + str(i) + "/" + pack_filename)
        shutil.copy2(os.path.join(original_dir, meta_filename), new_dir + str(i) + "/" + meta_filename)
