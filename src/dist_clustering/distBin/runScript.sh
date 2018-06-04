num=1;
dt=$(date '+%d_%m_%Y_%H_%M_%S.txt');
fileName="output_runScript_$num_$dt"
echo $fileName
mpirun -np $num ./exemain -d 16points -c 4 -o 0 -r 30 -a /home/shared/distAlgData/ $@ 2>INFO.txt | tee "$fileName"
./pretty_tsv.sh $(ls -t all_* | head -1)
./pretty_tsv.sh $(ls -t avg_* | head -1)
echo $fileName
