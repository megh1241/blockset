
sudo echo 3 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --format binary  --metadatafilename /data5/metadatabinwdfs6.txt --layout binwdfs --intertwine 6  --modelfilename /data5/packedmodelbinwdfs6 --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

sudo echo 3 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --metadatafilename /data5/metadatabinblock6.txt --format binary  --layout binblockstat --intertwine 6  --modelfilename /data5/packedmodelbinblock6 --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification


sudo echo 3 > sudo /proc/sys/vm/drop_caches

./exe  --mode inference --metadatafilename /data5/metadatablock.txt --format binary  --layout blockstat --intertwine 5  --modelfilename /data5/packedmodelblock --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

sudo echo 3 > sudo /proc/sys/vm/drop_caches

./exe  --mode inference --format binary  --metadatafilename /data5/metadatawdfs.txt --layout wdfs --intertwine 5  --modelfilename /data5/packedmodelwdfs --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

sudo echo 3 > sudo /proc/sys/vm/drop_caches

./exe  --mode inference --format binary  --metadatafilename /data5/metadatabfs.txt --layout bfs --intertwine 5  --modelfilename /data5/packedmodelbfs --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

sudo echo 3 > sudo /proc/sys/vm/drop_caches

./exe  --mode inference --format binary  --metadatafilename /data5/metadatadfs.txt --layout dfs --intertwine 5  --modelfilename /data5/packedmodeldfs --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
