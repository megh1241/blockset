#./exe --mode both --layout binbfs --modelfilename /root/pacset/models/mnist128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binbfs --intertwine 1 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification
