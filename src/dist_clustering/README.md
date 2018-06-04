# dist-clustering-outlier
bin/make
make local
make
pretty_tsv.sh
runscript.sh

-d datasetName
-o n_outliers
-c n_centers
-v :verbose on


mpirun -np 1 ./main -d 16points -c 3 -o 3 -v 2>INFO.txt
