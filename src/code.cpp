// Copyright 2023 'iraca' authors. See repository licence in LICENSE.md.
#include <Rcpp.h>

//' @title Test fun
//' @param n Integer for the last element in the sequence
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector get_numbers(const int n) {
  Rcpp::NumericVector vec(n);
  for (int i = 1; i <= n; i++) {
    vec[i - 1] = i;
  }

  return vec;
}
