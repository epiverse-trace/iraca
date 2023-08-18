ABM_test <- iraca::setup(iraca::demographic_data_ibague,
  iraca::temperature_data_ibague,
  divipola_code = 73001
)
ABM_test_wrong <- unclass(ABM_test)


test_that("Simulation errors are thrown", {
  expect_error(simulation(ABM_test_wrong, 20))
  expect_error(simulation(ABM_test, 20.5))
})

simulated_output <- simulation(ABM_test, 20L)

test_that("Simulation works as expected", {
  expect_s3_class(simulated_output, "data.frame")
  expect_identical(ncol(simulated_output), 3L)
  expect_identical(nrow(simulated_output), 20L)
  expect_type(simulated_output$S, "integer")
  expect_type(simulated_output$I, "integer")
  expect_type(simulated_output$R, "integer")
})
