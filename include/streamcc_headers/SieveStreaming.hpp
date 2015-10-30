/**
 @file SieveStreaming.hpp
 @author Jiecao Chen <chenjiecao@gmail.com>

 This file implements a framework that do optimiation for
 monotone submodular function over data stream.
 
 See [Streaming Submodular Maximization:
Massive Data Summarization on the Fly](http://las.ethz.ch/files/badanidiyuru14streaming.pdf) (KDD'14).
*/

#ifndef __SIEVE_STREAMING_HPP__
#define __SIEVE_STREAMING_HPP__
#include <cmath>
#include <vector>
#include <map>
#include <deque>
#include "utils.hpp"
#include <cstdlib>
//#include "CVector.hpp"
#include <armadillo>








// Comments:
//    + oldSamplesPtr private while samplesPtr can be shared among
//      multiple SieveKnowOpt instances.  When samplesPtr is updated by
//      other instance, oldSamplesPtr give a way to refer its own "latest version"
//      of samplesPtr




///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// abstract class for SieveKnowOpt ///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
namespace Scc {
  
  typedef std::vector<arma::vec> Vectors;

  //! Abstract class for SieveStreaming Algorithm with known optimal value

  /**
     + f() : the monotone submordular to be optimized
     + marginal_gain(const arma::vec& item) : the marginal gain when a new item added 
  */
  class CSieveKnowOpt {
  protected:
    double opt_est; //!< the estimated optimal maximum 
    int k; //!< Upper bound of number of centers
    Vectors S; //!< to keep the selected centers
  public:
    //! Constructor
    CSieveKnowOpt(int _k, //!< Upper bound of number of centers
		  double _opt_est //!< the estimated optimal maximum
		  ): opt_est(_opt_est), k(_k) {};
    //! monotone submordular to be optimized
    virtual double f() = 0; 
    //! marginal gain when a new item added 
    virtual double marginal_gain(const arma::vec& item) = 0;
    //! update internal states when a new item comes
    void update(const arma::vec& item) {
      if ((int) S.size() < k 
	  && marginal_gain(item) >= (opt_est / 2. - f()) / (k - S.size()))
	appendNewCenter(item);
    }

    //! return the centers selected
    Vectors getCenters() {
      return this->S;
    }

    //! append a new center to quene, may need to update intern states
    virtual void appendNewCenter(const arma::vec& item) {
      // S.push_back(item);
    }

  
    //! return estimated optimal maximum
    double getOptEst() {
      return this->opt_est;
    }
    virtual ~CSieveKnowOpt() {}
  };


  //! Gaussion kernel function, can be override to support other kernel
  double K(const arma::vec &v1, const arma::vec &v2, double h = 100.) {
    double d = arma::norm(v1 - v2, 2);
    return std::exp(- d * d / h);
  }

  
  //! Informative Vector Machine: log-det of I + sigma * K(S)

  /**
     k(S) is the kernal matrix of set S
  */
  double log_det_IVM(const Vectors &S, double sigma=1.) {
    arma::mat tmp = arma::eye<arma::mat>(S.size(), S.size());
    for (size_t i = 0; i < S.size(); ++i)
      for (size_t j = 0; j < S.size(); ++j)
	tmp(i, j) += sigma * K(S[i], S[j]);
    double val, sign;
    arma::log_det(val, sign, tmp);
    return val;
  }





  //! An implementation of CSieveKnowOpt: Informative Vector Machine (IVM)
  /**
     One can implement their own CSieveKnowOpt to support
     other monotone submordular function
   */
  class SieveKnowOptIVM: public CSieveKnowOpt {
  private:
    double log_det_val;    // to keep the current log_det(I + sigma* K(S))
    double penalty;
    double thresh;
    double cur_marginal_gain;
  public:
    //! Constructor
    SieveKnowOptIVM(int _k, //!< Upper bound of number of centers
		    double _opt_est, //!< the estimated optimal maximum
		    double _penalty //!< add penalty to control the growth of centers
		    ): CSieveKnowOpt(_k, _opt_est), log_det_val(0.), penalty(_penalty), thresh(1e-1) {};
    
    //! monotone submordular function to be optimized
    double f() {    
      return log_det_val - penalty * S.size();
    };

    void appendNewCenter(const arma::vec& item) {
      S.push_back(item);
      log_det_val += cur_marginal_gain;
    }

  
    double marginal_gain(const arma::vec& item) {
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



  //! Class template for monotone submodular function optimization in data stream

  /**
     T must be an implementation of the abstract class CSieveKnowOpt.
     This class then processes item by item in the data stream,
     and return a set of centers (selected from data streams)
     that approximately maximizes the function f()
  */
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
    //! Constructor
    CSieveStreaming(int _k, //!< Upper bound of number of centers
		    double _eps, //!< Parameter to control the approximation ratio
		    double _penalty=0 //!< add penalty to control the growth of centers
		    ): 
      k(_k), eps(_eps), m(0.), penalty(_penalty) {
      this->svPtr = new T(0, 100, penalty);    
    };


    //! update internal states when a new item comes
    void update(arma::vec& item) {
      // estimate f({item})
      double f_item = svPtr->marginal_gain(item);
    
      // when m needs to be updated
      if (this->m < f_item) {
	this->m = f_item;
	double c = utils::find_closest(1 + this->eps, this->m);
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


    //! return the set of centers maximize the objective function f()
    Vectors getCenters() {
      int max_i = 0;
      double max_f = O[max_i].f();
      for (unsigned int i = 1; i < O.size(); ++i) {
	double tmp_f = O[i].f();
	if (tmp_f > max_f) {
	  max_f = tmp_f;
	  max_i = i;
	}
      }
      // std::cerr << "max_f(S) = " << max_f << std::endl;
      return O[max_i].getCenters();
    }

    ~CSieveStreaming() {
      delete svPtr;
    };  
  };

};

#endif

















