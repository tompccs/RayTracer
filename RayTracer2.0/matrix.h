#ifndef __RayTracer__matrix__
#define __RayTracer__matrix__

#include <stdio.h>
#include <vector>
#include <iostream>


using namespace std;

template <typename T> class matrix {
    
private:
    std::vector<std::vector<T> > mat;
    unsigned rows;
    unsigned cols;
    
public:
    matrix(unsigned _rows, unsigned _cols, const T& _initial);
    matrix(const matrix<T>& rhs);
    virtual ~matrix();
    
    // Operator overloading, for "standard" mathematical matrix operations
    matrix<T>& operator=(const matrix<T>& rhs);
    
    // Matrix mathematical operations
    matrix<T> operator+(const matrix<T>& rhs);
    matrix<T>& operator+=(const matrix<T>& rhs);
    matrix<T> operator-(const matrix<T>& rhs);
    matrix<T>& operator-=(const matrix<T>& rhs);
    matrix<T> operator*(const matrix<T>& rhs);
    matrix<T>& operator*=(const matrix<T>& rhs);
    matrix<T> transpose();
    
    // Matrix/scalar operations
    matrix<T> operator+(const T& rhs);
    matrix<T> operator-(const T& rhs);
    matrix<T> operator*(const T& rhs);
    matrix<T> operator/(const T& rhs);
    
    // Matrix/vector operations
    vector<T> operator*(const std::vector<T>& rhs);
    vector<T> diag_vec();
    
    // Access the individual elements
    T& operator()(const unsigned& row, const unsigned& col);
    const T& operator()(const unsigned& row, const unsigned& col) const;
    
    // Access the row and column sizes
    unsigned get_rows() const;
    unsigned get_cols() const;
    
    unsigned Print();
    
};

#include "matrix.cpp"


#endif