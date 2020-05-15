
./exe  --mode both --layout bfs --intertwine 1 --modelfilename ~/pacset/models/cifar128.json --datafilename ~/pacset/data/cifar10.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
cd lambda_exp
python3 read_and_predict.py
