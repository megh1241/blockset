import shutil
import sys

num_copies = int(sys.argv[1])

for i in range(num_copies):
        #shutil.copy2('/data5/packedmodelbinblock5.bin', '/data5/packedmodelbinblock5{}.bin'.format(i))
        #shutil.copy2('/data5/packedmodelbinwdfs5.bin', '/data5/packedmodelbinwdfs5{}.bin'.format(i))
        shutil.copy2('/data5/packedmodelbinblock6.bin', '/data5/packedmodelbinblock6{}.bin'.format(i))
        #shutil.copy2('/data5/packedmodelblock.bin', '/data5/packedmodelblock{}.bin'.format(i))
        shutil.copy2('/data5/packedmodelbinwdfs6.bin', '/data5/packedmodelbinwdfs6{}.bin'.format(i))
        #shutil.copy2('/data5/packedmodelwdfs.bin', '/data5/packedmodelwdfs{}.bin'.format(i))
