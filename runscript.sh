#./exe --mode both --layout binbfs --modelfilename /root/pacset/models/mnist128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout bfs --intertwine 1 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout dfs --intertwine 1 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout wdfs --intertwine 1 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binwdfs --intertwine 6 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout bindfs --intertwine 6 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binbfs --intertwine 6 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binwdfs --intertwine 7 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binbfs --intertwine 7 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout bindfs --intertwine 7 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binwdfs --intertwine 5 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binwdfs --intertwine 4 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binwdfs --intertwine 3 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binwdfs --intertwine 2 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe --mode both --layout binwdfs --intertwine 1 --modelfilename /root/pacset/models/cifar128.json --datafilename /root/pacset/data/cifar-10.csv --numthreads 8 --package sklearn --algorithm randomforest --task classification
