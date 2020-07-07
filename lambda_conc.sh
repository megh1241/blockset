./exe  --mode both --layout binstatblock --intertwine 4 --blocksize 128 --modelfilename ~/pacset/models/cifarnew.json --packfilename ~/pacset/models/packedbinstatblock1.txt --datafilename ~/pacset/data/cifar-10.csv --metadatafilename ~/pacset/models/metabinstatblock1.txt --numthreads 1 --package sklearn --algorithm randomforest --task classification

./exe  --mode both --layout binstatblock --intertwine 4 --blocksize 64 --modelfilename ~/pacset/models/cifarnew.json --packfilename ~/pacset/models/packedbinstatblock2.txt --datafilename ~/pacset/data/cifar-10.csv --metadatafilename ~/pacset/models/metabinstatblock2.txt --numthreads 2 --package sklearn --algorithm randomforest --task classification

./exe  --mode both --layout binstatblock --intertwine 4 --blocksize 32 --modelfilename ~/pacset/models/cifarnew.json --packfilename ~/pacset/models/packedbinstatblock4.txt --datafilename ~/pacset/data/cifar-10.csv --metadatafilename ~/pacset/models/metabinstatblock4.txt --numthreads 4 --package sklearn --algorithm randomforest --task classification

./exe  --mode both --layout binstatblock --intertwine 4 --blocksize 16 --modelfilename ~/pacset/models/cifarnew.json --packfilename ~/pacset/models/packedbinstatblock8.txt --datafilename ~/pacset/data/cifar-10.csv --metadatafilename ~/pacset/models/metabinstatblock8.txt --numthreads 8 --package sklearn --algorithm randomforest --task classification

./exe  --mode both --layout binstatblock --intertwine 4 --blocksize 8 --modelfilename ~/pacset/models/cifarnew.json --packfilename ~/pacset/models/packedbinstatblock16.txt --datafilename ~/pacset/data/cifar-10.csv --metadatafilename ~/pacset/models/metabinstatblock16.txt --numthreads 16 --package sklearn --algorithm randomforest --task classification

./exe  --mode both --layout binstatblock --intertwine 4 --blocksize 4 --modelfilename ~/pacset/models/cifarnew.json --packfilename ~/pacset/models/packedbinstatblock32.txt --datafilename ~/pacset/data/cifar-10.csv --metadatafilename ~/pacset/models/metabinstatblock32.txt --numthreads 32 --package sklearn --algorithm randomforest --task classification

./exe  --mode both --layout binstatblock --intertwine 4 --blocksize 2 --modelfilename ~/pacset/models/cifarnew.json --packfilename ~/pacset/models/packedbinstatblock64.txt --datafilename ~/pacset/data/cifar-10.csv --metadatafilename ~/pacset/models/metabinstatblock64.txt --numthreads 64 --package sklearn --algorithm randomforest --task classification

./exe  --mode both --layout binstatblock --intertwine 4 --blocksize 1 --modelfilename ~/pacset/models/cifarnew.json --packfilename ~/pacset/models/packedbinstatblock128.txt --datafilename ~/pacset/data/cifar-10.csv --metadatafilename ~/pacset/models/metabinstatblock128.txt --numthreads 128 --package sklearn --algorithm randomforest --task classification
