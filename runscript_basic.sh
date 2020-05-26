
./exe  --mode both --layout binwdfs --intertwine 6 --modelfilename ~/pacset/models/cifar128.json --datafilename ~/pacset/data/cifar-10.csv --numthreads 4 --package sklearn --algorithm randomforest --task classification
cd lambda_exp
python3 read_and_predict.py > out
