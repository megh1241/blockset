
./exe  --mode both --layout binwdfs --intertwine 4 --blocksize 2048 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
./exe  --mode both --layout binblockstat --intertwine 4 --blocksize 2048 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#./exe  --mode both --layout binclassblockstat --intertwine 5 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#./exe  --mode both --layout binbfs --intertwine 5 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

#./exe  --mode both --layout classblockstat --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#./exe  --mode both --layout blockstat --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#./exe  --mode both --layout wdfs --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
#./exe  --mode both --layout dfs --intertwine 1 --modelfilename ~/pacset/models/mnist.json --datafilename ~/pacset/data/mnist.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification


#mv logs logs_mnist
#mkdir logs


#./exe  --mode both --layout classblockstat --intertwine 1 --modelfilename ~/pacset/models/reg.json --datafilename ~/pacset/data/reg.csv --numthreads 1 --package sklearn --algorithm randomforest --task regression
#./exe  --mode both --layout blockstat --intertwine 1 --modelfilename ~/pacset/models/reg.json --datafilename ~/pacset/data/reg.csv --numthreads 1 --package sklearn --algorithm randomforest --task regression
#./exe  --mode both --layout wdfs --intertwine 1 --modelfilename ~/pacset/models/reg.json --datafilename ~/pacset/data/reg.csv --numthreads 1 --package sklearn --algorithm randomforest --task regression
#./exe  --mode both --layout dfs --intertwine 1 --modelfilename ~/pacset/models/reg.json --datafilename ~/pacset/data/reg.csv --numthreads 1 --package sklearn --algorithm randomforest --task regression
#./exe  --mode both --layout bfs --intertwine 1 --modelfilename ~/pacset/models/reg.json --datafilename ~/pacset/data/reg.csv --numthreads 1 --package sklearn --algorithm randomforest --task regression
