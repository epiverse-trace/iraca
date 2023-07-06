// Copyright 2023 `iraca` authors.
#include <Rcpp.h>
#include "Simulation.h"
#include "Simulation.cpp"

//' @title run simulation
//' @param n_days number of days to simulate
//' @export
// [[Rcpp::export]]
void run_simulation(int n_days = 365)
{
  Simulation model(n_days);
  model.simulate();
}