import shutil
import sys

num_copies = int(sys.argv[1])

for i in range(num_copies):
        shutil.copy2('/data4/packedmodelstat2048correct2.bin', '/dat' + str(i) + '/packedmodelstat2048correct2.bin')
        #shutil.copy2('/data4/metadatablock2048correct0.txt', '/dat' + str(i) + '/metadatablock2048correct0.txt')
        #shutil.copy2('/data4/metadatastat2048correct2.txt', '/dat' + str(i) + '/metadatastat2048correct2.txt')
