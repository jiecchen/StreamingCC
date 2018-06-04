#include "../algorithms/util.h"
#include "../experiments/dataManager.h"


#include <iostream>
#include <cstdlib>
#include <fstream>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"


using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 4) {
    cerr << "usage: ./rm_column input_file which_column output_file" << endl;
    return 0;
  }
  ifstream fin(argv[1], fstream::in);
  arma::mat old;
  old.load(fin, arma::csv_ascii);
  old.shed_col(atoi(argv[2]));
  fin.close();
  ofstream fout(argv[3], fstream::out);
  old.save(fout, arma::csv_ascii);
  fout.close();
  return 0;
}
