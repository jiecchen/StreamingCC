#include "dataManager.h"

void DataSet::load() {  // this is here in header file because I couldn't make
                        // compiler read this from .cc file it was giving
                        // error!!
  // Once we figured that out we can move it.

  std::string fileName = folderName + name + fileExtention;
  bool header = false;
  arma::mat A;
  std::ifstream file(fileName);
  //  std::cout<<fileName<<std::endl;
  if (name == "kddcup") {
    //  if(std::find(DataSet::withHeader.begin(),
    //      DataSet::withHeader.end(),name)
    //      !=DataSet::withHeader.end()){
    std::string header;
    std::getline(file, header);
  }
  A.load(file, arma::csv_ascii);
  file.close();
  for (int i = 0; i < A.n_rows; i++) {
    data.push_back(A.row(i).t());
  }
}
