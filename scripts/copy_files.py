import shutil
import sys

num_copies = int(sys.argv[1])

for i in range(num_copies):
        shutil.copy2('/data4/packedmodelbinblock5.bin', '/data4/packedmodelbinblock5{}.bin'.format(i))
        shutil.copy2('/data4/packedmodelbinwdfs5.bin', '/data4/packedmodelbinwdfs5{}.bin'.format(i))
        shutil.copy2('/data4/packedmodelbinblock6.bin', '/data4/packedmodelbinblock6{}.bin'.format(i))
        shutil.copy2('/data4/packedmodelbinwdfs6.bin', '/data4/packedmodelbinwdfs6{}.bin'.format(i))
