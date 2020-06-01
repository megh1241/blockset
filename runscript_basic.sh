
./exe  --mode both --layout classblockstat --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#rm packedmodel.txt
#rm metadata.txt
#./exe  --mode both --layout blockstat --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

#./exe  --mode both --layout classblock --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
rm packedmodel.txt
rm metadata.txt

#./exe  --mode both --layout binblock --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
rm packedmodel.txt
rm metadata.txt

#./exe  --mode both --layout binclassblock --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
rm packedmodel.txt
rm metadata.txt


#./exe  --mode both --layout bfs --intertwine 1 --modelfilename ~/pacset/models/cifar128.json --datafilename ~/pacset/data/cifar10.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#./exe  --mode both --layout bfs --intertwine 1 --modelfilename ~/pacset/models/cifar128.json --datafilename ~/pacset/data/cifar10.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

#./exe  --mode both --layout blockclass --intertwine 1 --modelfilename ~/pacset/models/iris10.json --datafilename ~/pacset/data/iris.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

