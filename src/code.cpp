#include <Rcpp.h>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace Rcpp;

//' @title Test fun
//' @param n Integer for the last element in the sequence
//' @export
// [[Rcpp::export]]
double get_numbers() {
  NumericVector v1 = {1,2,3,4};
  float out = v1[1];
  return out;
}
