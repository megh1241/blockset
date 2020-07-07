
#cd scripts
#python3 copy_files2.py 4 4
#cd ..
: '
sudo echo 3 > sudo /proc/sys/vm/drop_caches
sudo echo 2 > sudo /proc/sys/vm/drop_caches
sudo echo 1 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --format binary  --metadatafilename /data8/metadatablock2048correct0.txt --layout blockstat --intertwine 0  --modelfilename /data8/packedmodelbinblockstat2048correct0 --datafilename /data4/foo2.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

rm /data8/packedmodel*

sudo echo 3 > sudo /proc/sys/vm/drop_caches
sudo echo 2 > sudo /proc/sys/vm/drop_caches
sudo echo 1 > sudo /proc/sys/vm/drop_caches
'
cd scripts
python3 copy_files_new.py packedmodelbfscorrect0.bin metadatabfscorrect0.txt
cd ..
sudo echo 3 > sudo /proc/sys/vm/drop_caches
sudo echo 2 > sudo /proc/sys/vm/drop_caches
sudo echo 1 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --format binary  --metadatafilename /dat0/metadatastat2048correct2.txt --layout binstatdfs --intertwine 2  --modelfilename packedmodelstat2048correct2.bin --datafilename /data4/foo2.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

: '
rm /data4/packedmodelwdfs*.bin
cd scripts
python3 copy_files.py 9 2
cd ..

sudo echo 3 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --format binary  --metadatafilename /data4/metadatawdfs2048correct2.txt --layout binwdfs --intertwine 2  --modelfilename /data4/packedmodelwdfs2048correct2 --datafilename /data4/foo2.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

rm /data4/packedmodelwdfs*.bin
cd scripts
python3 copy_files.py 9 3
cd ..

sudo echo 3 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --format binary  --metadatafilename /data4/metadatawdfs2048correct3.txt --layout binwdfs --intertwine 3  --modelfilename /data4/packedmodelwdfs2048correct3 --datafilename /data4/foo2.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
rm /data4/packedmodelwdfs*.bin

cd scripts
python3 copy_files.py 9 7 
cd ..

sudo echo 3 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --format binary  --metadatafilename /data4/metadatawdfs2048correct7.txt --layout binwdfs --intertwine 7  --modelfilename /data4/packedmodelwdfs2048correct7 --datafilename /data4/foo2.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

rm /data4/packedmodelwdfs*.bin
cd scripts
python3 copy_files.py 9 8
cd ..

sudo echo 3 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --format binary  --metadatafilename /data4/metadatawdfs2048correct8.txt --layout binwdfs --intertwine 8  --modelfilename /data4/packedmodelwdfs2048correct8 --datafilename /data4/foo2.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification

rm /data4/packedmodelwdfs*.bin
cd scripts
python3 copy_files.py 9 9
cd ..

sudo echo 3 > sudo /proc/sys/vm/drop_caches
./exe  --mode inference --format binary  --metadatafilename /data4/metadatawdfs2048correct9.txt --layout binwdfs --intertwine 9  --modelfilename /data4/packedmodelwdfs2048correct9 --datafilename /data4/foo2.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
rm /data4/packedmodelwdfs*.bin
'
