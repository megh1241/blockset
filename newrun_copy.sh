declare -a layoutArray=("bfs" )
#declare -a layoutArray=("binbfs" "bindfs" "binstatdfs" "binstatblock")

for val in ${layoutArray[@]}; do
	python3 scripts/copy_files_single_dir.py 10 '/data5/' 'packedmodel'${val}
done
