#include <Rcpp.h>
using namespace Rcpp;

//' @title Test fun
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector get_numbers(const int n) {
  Rcpp::NumericVector vec (n);
  for(int i = 1; i <= n; i++) {
    vec[i - 1] = i;
  }
  
  return vec;
}
