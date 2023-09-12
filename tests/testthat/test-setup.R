incomplete_dataframe <- sf::st_drop_geometry(iraca::demographic_data_ibague)

test_that("Setup errors are thrown", {
  expect_error(setup(
    demographic_data = incomplete_dataframe,
    temperature_data = iraca::temperature_data_ibague,
    divipola_code = 73001
  ))
  expect_error(setup(
    demographic_data = iraca::demographic_data_ibague,
    temperature_data = c(4, 5, 6, 7),
    divipola_code = 73001
  ))
  expect_error(setup(
    demographic_data = iraca::demographic_data_ibague,
    temperature_data = iraca::temperature_data_ibague,
    movement_data = matrix(rep(0, 99 * 99), ncol = 99)
  ))
  expect_error(setup(
    demographic_data = iraca::demographic_data_ibague,
    temperature_data = iraca::temperature_data_ibague,
    movement_data = iraca::movement_data_ibague,
    divipola_code = "73001"
  ))
  expect_error(setup(
    demographic_data = iraca::demographic_data_ibague,
    temperature_data = iraca::temperature_data_ibague,
    divipola_code = 5001
  ))
  expect_error(setup(
    demographic_data = iraca::demographic_data_ibague[0, ],
    temperature_data = iraca::temperature_data_ibague
  ))
  expect_error(setup(
    demographic_data = iraca::demographic_data_ibague[1, ],
    temperature_data = iraca::temperature_data_ibague,
    movement_data = iraca::movement_data_ibague
  ))
})

test_that("Seup throws expected object", {
  expect_s3_class(setup(
    demographic_data = iraca::demographic_data_ibague,
    temperature_data = iraca::temperature_data_ibague,
    divipola_code = 73001
  ), "abm_data")
  expect_length(setup(
    demographic_data = iraca::demographic_data_ibague,
    temperature_data = iraca::temperature_data_ibague,
    divipola_code = 73001
  ), 10L)

  ABM_test_model <- setup(
    demographic_data = iraca::demographic_data_ibague,
    temperature_data = iraca::temperature_data_ibague,
    divipola_code = 73001
  )
  expect_s3_class(ABM_test_model$demographic_data, "data.frame")
  expect_s3_class(ABM_test_model$temperature_data, "data.frame")
  expect_s3_class(ABM_test_model$movement_data, "data.frame")
  expect_type(ABM_test_model$disease, "character")
  expect_type(ABM_test_model$incubation_period, "integer")
  expect_type(ABM_test_model$infection_duration, "integer")
  expect_type(ABM_test_model$init_infected_humans, "double")
  expect_type(ABM_test_model$init_infected_mosquitoes, "double")
  expect_type(ABM_test_model$geometry_text, "character")
})
