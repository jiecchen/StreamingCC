#ifndef __CVECTOR_H__
#define __CVECTOR_H__
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
// multi-dimentinal vector
// it can be initialized using both std::vector and normal array
namespace Scc {

  //! a data point (aka item in data stream)
  typedef std::vector<double> Vdouble;

  typedef Vdouble CVector;

  //! vector of items
  typedef std::vector<Vdouble> Vectors;

  std::string to_string(const Vdouble& data) { 
    std::string outstring = "(" + std::to_string(data[0]);
    for (auto it = data.begin() + 1; it != data.end(); ++it)
      outstring += (", " + std::to_string(*it));
    outstring += ")";
    return outstring;
  }



  // size = 1  index[0] = 0. CVector is considered as all zero vector
  double dist(const Vdouble& v1, const Vdouble& v2) {
    // v1 is all 0 vector
    if (v1.size() == 1 && v1[0] == 0.) {
      double tot_sq = 0.;
      for (size_t i = 0; i < v2.size(); ++i)
	tot_sq += std::pow(v2[i], 2);
      return std::sqrt(tot_sq);
    }

    // v2 is all 0 vector
    if (v2.size() == 1 && v2[0] == 0.) {
      double tot_sq = 0.;
      for (size_t i = 0; i < v1.size(); ++i)
	tot_sq += std::pow(v1[i], 2);
      return std::sqrt(tot_sq);
    }

    if (v1.size() != v2.size()) {    
      throw "dist: Two vectors must have same dimention!";
    }

    double tot_sq = 0.;
    for (size_t i = 0; i < v1.size(); ++i)
      tot_sq += std::pow(v1[i] - v2[i], 2);
    return std::sqrt(tot_sq);
  }


  Vdouble operator-(const Vdouble& v1, const Vdouble& v2) {
    if (v1.size() == 1 && v1[0] == 0.) {
      Vdouble data(v2.size());
      for (size_t i = 1; i < v2.size(); ++i)
	data[i] = - v2[i];
      return data;    
    }

    if (v2.size() == 1 && v2[0] == 0.) {
      return v1;
    }


    if (v1.size() != v2.size())
      throw "operator-: Two vectors must have same dimention!";

    Vdouble data(v1.size());
    for (size_t i = 1; i < v1.size(); ++i)
      data[i] = v1[i] - v2[i];
    return data;
  }

  CVector zero_vector(size_t sz) {
    return CVector(sz);
  }

};

#endif













