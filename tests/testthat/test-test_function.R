test_that("Rcpp function works", {
  expect_identical(get_numbers(10), as.double(seq(1, 10)))
})
