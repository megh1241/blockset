import sys
import os
import subprocess

num_dirs = int(sys.argv[1])
delete_dir = '/dat'

for i in range(num_dirs):
    command_str = "rm " + delete_dir + str(i) + "/*.bin"
    command_str_2 = "rm " + delete_dir + str(i) + "/*.txt"
    print(command_str)
    print(command_str_2)
    subprocess.run(command_str, shell=True, check=False)
    subprocess.run(command_str_2, shell=True, check=False)
