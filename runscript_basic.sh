
./exe  --mode pack --modelfilename2 /data5/packedmodelbinblock6.bin --format binary --layout binblockstat --intertwine 6 --modelfilename /data5/mnist_manual_2048.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

mv metadata.txt /data5/metadatabinblock6.txt


./exe  --mode pack --format binary --layout binwdfs --modelfilename2 /data5/packedmodelbinwdfs6.bin --intertwine 6 --modelfilename /data5/mnist_manual_2048.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

mv metadata.txt /data5/metadatabinwdfs6.txt


./exe  --modelfilename2 /data5/packedmodelblock.bin --format binary --mode pack --layout blockstat --intertwine 1 --modelfilename /data5/mnist_manual_2048.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

mv metadata.txt /data5/metadatablock.txt

./exe  --mode pack --layout wdfs --intertwine 1 --format binary --modelfilename /data5/mnist_manual_2048.json --modelfilename2 /data5/packedmodelwdfs.bin --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
mv metadata.txt /data5/metadatawdfs.txt
#rm packedmodel.txt
#rm metadata.txt

#./exe  --mode both --layout binblock --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#rm packedmodel.txt
#rm metadata.txt

#./exe  --mode both --layout binclassblock --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#rm packedmodel.txt
#rm metadata.txt


#./exe  --mode both --layout bfs --intertwine 1 --modelfilename ~/pacset/models/cifar128.json --datafilename ~/pacset/data/cifar10.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#./exe  --mode both --layout bfs --intertwine 1 --modelfilename ~/pacset/models/cifar128.json --datafilename ~/pacset/data/cifar10.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

#./exe  --mode both --layout blockclass --intertwine 1 --modelfilename ~/pacset/models/iris10.json --datafilename ~/pacset/data/iris.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

