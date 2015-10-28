#ifndef __SIEVE_STREAMING_HPP__
#define __SIEVE_STREAMING_HPP__
#include <cmath>
#include <vector>
#include <map>
#include <deque>
#include "utils.hpp"
#include <cstdlib>
#include "CVector.hpp"
#include <armadillo>





/////////////////////////////////////////////////////////
/////////////  Defintion for CVector ////////////////////
/////////////////////////////////////////////////////////


// Comments:
//    + oldSamplesPtr private while samplesPtr can be shared among
//      multiple SieveKnowOpt instances.  When samplesPtr is updated by
//      other instance, oldSamplesPtr give a way to refer its own "latest version"
//      of samplesPtr




///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// abstract class for SieveKnowOpt ///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
namespace Scc {

  class CSieveKnowOpt {
  protected:
    double opt_est;
    int k;
    Vectors S; // to keep the centers
  public:
    CSieveKnowOpt(int _k, double _opt_est): opt_est(_opt_est), k(_k) {};
    virtual double f() = 0;
    virtual double marginal_gain(const CVector& item) = 0;
    void update(const CVector& item) {
      if ((int) S.size() < k 
	  && marginal_gain(item) >= (opt_est / 2. - f()) / (k - S.size()))
	appendNewCenter(item);
    }

    Vectors getCenters() {
      return this->S;
    }

    virtual void appendNewCenter(const CVector& item) {
      // S.push_back(item);
    }

  
    double getOptEst() {
      return this->opt_est;
    }
    virtual ~CSieveKnowOpt() {}
  };


  // kernel function
  double K(const CVector &v1, const CVector &v2, double h = 100.) {
    return std::exp(- dist(v1, v2) / h);
  }


  double log_det_IVM(const Vectors &S, double sigma=1.) {
    arma::mat tmp = arma::eye<arma::mat>(S.size(), S.size());
    for (size_t i = 0; i < S.size(); ++i)
      for (size_t j = 0; j < S.size(); ++j)
	tmp(i, j) += sigma * K(S[i], S[j]);
    double val, sign;
    arma::log_det(val, sign, tmp);
    return val;
  }



  // for IVM (informative Vector Machine)
  // TODO: estimate log-det of (I + sigma^{-2} \Sigma)
  // check: + http://www.shogun-toolbox.org/static/notebook/current/logdet.html
  //        + http://arxiv.org/pdf/1105.5256v1.pdf
  class SieveKnowOptIVM: public CSieveKnowOpt {
  private:
    double log_det_val;    // to keep the current log_det(I + sigma* K(S))
    double penalty;
    double thresh;
    double cur_marginal_gain;
  public:
    SieveKnowOptIVM(int _k, double _opt_est, double _penalty): CSieveKnowOpt(_k, _opt_est), log_det_val(0.), penalty(_penalty), thresh(1e-1) {};
    double f() {    
      return log_det_val - penalty * S.size();
    };

    void appendNewCenter(const CVector& item) {
      S.push_back(item);
      log_det_val += cur_marginal_gain;
    }

  
    double marginal_gain(const CVector& item) {
      Vectors block;
      for (auto &itm: this->S)
	if (K(item, itm) > thresh)
	  block.push_back(itm);
      double f_val = log_det_IVM(block);
      block.push_back(item);
      double f_val_new = log_det_IVM(block);
      cur_marginal_gain = f_val_new - f_val;
      return cur_marginal_gain;
    };

    ~SieveKnowOptIVM() {};
  };



  ////////////////////////////////////////////////////////////////////////////
  ///////////////// class template for SieveStreaming ////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  // one requirement to use this
  // template is that
  // f() in T only depends on S.



  template <class T>
  class CSieveStreaming {
  private:
    int k;
    double eps;
    double m;
    T *svPtr;
    double penalty;
    std::deque<T> O;
  public:
    CSieveStreaming(int _k, double _eps, double _penalty): 
      k(_k), eps(_eps), m(0.), penalty(_penalty) {
      this->svPtr = new T(0, 100, penalty);    
    };


    void update(CVector& item) {
      // estimate f({item})
      double f_item = svPtr->marginal_gain(item);
    
      // when m needs to be updated
      if (this->m < f_item) {
	this->m = f_item;
	double c = find_closest(1 + this->eps, this->m);
	// drop all instances with opt_est < c
	while (this->O.size() > 0 && O[0].getOptEst() < c) 
	  O.pop_front();      
	// append new SieveKnowOpt instances
	while (O.size() > 0 && O.back().getOptEst() >= c)
	  c *= (1 + this->eps);
	while (c <= m * 2 * k) {
	  O.push_back(T(k, c, penalty));
	  c *= (1 + this->eps);
	}
      }

      // now update each instance in deque O
      for (auto it = O.begin(); it != O.end(); ++it)
	it->update(item);
    };


    Vectors getOptMedoids() {
      int max_i = 0;
      double max_f = O[max_i].f();
      for (unsigned int i = 1; i < O.size(); ++i) {
	double tmp_f = O[i].f();
	if (tmp_f > max_f) {
	  max_f = tmp_f;
	  max_i = i;
	}
      }
      std::cerr << "max_f(S) = " << max_f << std::endl;
      return O[max_i].getCenters();
    }

    ~CSieveStreaming() {
      delete svPtr;
    };  
  };

};

#endif

















