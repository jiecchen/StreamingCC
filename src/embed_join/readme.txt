1. compile:  
git clone https://github.com/kedayuge/Embedjoin.git
cd Embedjoin
make

2. usage: ./embedjoin inputdata K r z m dictsize embedrange outputallresults shift T

inputdata: a set of strings in a file

K: edit distance threshold

r: number of CGK-embedding for each input string

z: number of hash functions for each embedded string 

m: number of bits in each hash function

dictsize: alpha beta size of input strings, could be 4 for DNA dataset (ACGT); 26 for UNIREF dataset (A~Z); 37 for TREC dataset (A~Z,0~9,' ') 

embedrange: the length of truncation, recommended to be the average length of strings (you could try smaller values to further save the embedding time)

outputallresults =1 if you want to output all the pairs of strings in results =0 just output the number of results and running time

shift: parameter about shift (\Delta in the paper)

T: Number of required matches (>T) for a pair of substrings to be considered as candidate

3. example input:
./embedjoin trec.txt 30 4 6 12 37 800 0 50 0 
./embedjoin trec.txt 50 5 8 6 37 600 0 50 0

./embedjoin uniref.txt 10 4 5 12 26 420 0 50 0
./embedjoin uniref.txt 20 5 5 6 26 420 0 50 0

./embedjoin dnadata50k.txt 100 5 12 13 4 5000 0 34 1
./embedjoin dnadata50k.txt 150 7 16 12 4 5000 0 50 1

Note that, all output pairs are distinct, and verified by exact edit distance filter. 
Thus, the number of output divided by number of ground truth (by exact algorithm such as Passjoin, Qchunck) is the accuracy.

4.Input format:
A collection of strings seperated by enter
For example:
ACGTTTTTTTAATTTTATAGGGGGC
ACGTAATTTAATTTTATAGGGGGC
ACGTTTTTTTATTTATAGGGTTGGC
...
