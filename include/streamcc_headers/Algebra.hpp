/*
  Original Author: Jakub Tabisz
  Description: Simple program for computing determinant of given matrix with two methods.
  LaplaceExpansion and GaussianElimination
  Modified by: Jiecao Chen (chenjiecao@gmail.com)
*/
			 

#ifndef __ALGEBRA_HPP__
#define __ALGEBRA_HPP__
#include <random>
#include <vector>
#include <algorithm>

namespace Algebra {

  //! helper type alias (c++11) for matrix type
  using Matrix = std::vector<std::vector<double>>;
  
  //!  returns matrix  

  Matrix makeMatrix(size_t rows, size_t cols)
  {
    Matrix tmp(rows);
    for (auto& row : tmp)
      {
	row.resize(cols);
      }

    return tmp;
  }



  /*!
    since we can't compare floating point numbers to 0 (zero),
    we'll check if they're in range close enough to zero
    (-epsilon, epsilon), where default epsilon=0,000000000001
  */

  bool isZero(double number, double epsilon = 1e-12)
  {
    return (number < epsilon) && (number > -epsilon);
  }


  /*!
    function for for swapping rows if given coefficient (matrix[row][col]) is equal to zero
    return true if there was non-zero coefficient below [row] in [column] and swap was succesful
    false otherwise
  */

  bool swapRows(Matrix& matrix, size_t row, size_t col)
  {
    for (size_t i = row + 1; i < matrix.size(); ++i)
      {
	if (!isZero(matrix[i][col]))
	  {
	    // found appropriate row -> swap rows and report success (true)
	    std::swap(matrix[row], matrix[i]);
	    return true;
	  }
      }

    return false; //failed to find appropriate row to swap with
  }

  /*!
    function for finding column where 'value' repeats the most
    By default it seeks the column with the most amount of zeros
  */

  size_t findBestColumn(Matrix& M, double value = 0)
  {
    // first = column index
    // second = number of 'value' occurrences
    std::pair<size_t, size_t> bestColumn{ 0, 0 };

    for (size_t i = 0; i < M.size(); ++i)
      {
	size_t count{};
	for (size_t j = 0; j < M.size(); ++j)
	  {
	    if (M[j][i] == value)
	      count++;
	  }

	if (count > bestColumn.second)
	  {
	    bestColumn.first = i;
	    bestColumn.second = count;
	  }
      }

    return bestColumn.first;
  }
  /*!
    deletes row [i] and column [j] from matrix M 	
  */

  Matrix deleteRowAndColumn(Matrix M, size_t i, size_t j)
  {
    // delete column j
    for (size_t k = 0; k < M.size(); ++k)
      {
	M[k].erase(M[k].begin() + j);
      }
    // delete row i
    M.erase(M.begin() + i);

    return M;
  }




  bool gaussianElimination(Matrix M, double& result)
  {
    size_t numberOfSwaps{};
    /*
      transform matrix M into upper triangular form
    */
    for (size_t i = 0; i < M.size(); ++i)
      {
  	// if the leading value (i-th column) is zero, try swaping its row with some row below it
  	if (isZero(M[i][i]))
  	  {			
  	    // if no appropriate row was found -> report failure (false)
  	    if (!swapRows(M, i, i))
  	      return false;
  	    // count number of row swaps
  	    else
  	      numberOfSwaps++;
  	  }
	
  	/*
  	  transform (through elementary matrix operations) every row below  i-th row,
  	  so that every value in i-th column is equal to zero
  	*/
  	for (size_t j = i + 1; j < M.size(); ++j)
  	  {
	
  	    double multiplyFactor = M[j][i] / M[i][i];
  	    size_t column = i;
	
  	    // subtract i-th row (multiplied by multiplyFactor) from j-th row
  	    std::transform
  	      (M[j].begin() + i, M[j].end(), // source
  	       M[j].begin() + i, // destination
  	       [&](double value) // operation
  	       {return value - multiplyFactor*M[i][column++]; });		
  	  }
      }

    // determinant of a matrix in triangular form is a product of every element on that matrix's diagonal
    double tmp {1};
    for (size_t i = 0; i < M.size(); ++i)
      {
  	tmp *= M[i][i];
      }
    // check how many times rows were swapped
    // odd - change determinant's sign
    // even - don't change determinant's sign
    if ((numberOfSwaps % 2) != 0) // odd number of times
      tmp *= -1;


    result = tmp;
    return true;
  }

};





#endif
