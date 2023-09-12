#' @title Run simulation for a given number of days.
#'
#' @param ABM_data S3 class object containing the parameters for the
#' simulation.
#' @param time Integer for the amount of days to simulate.
#'
#' @return Data frame containing SIR simulation results per day.
#' @examples
#' \dontrun{
#' simulation(ABM_model, 100)
#' }
#' @export
simulation <- function(ABM_data, time) {
  checkmate::assert_class(ABM_data, "abm_data")
  checkmate::assert_integer(time)
  SIR_simulation <- internal_simulation_cpp(
    time,
    ABM_data$demographic_data,
    ABM_data$temperature_data,
    ABM_data$movement_data,
    ABM_data$incubation_period,
    ABM_data$infection_duration,
    ABM_data$init_infected_humans,
    ABM_data$init_infected_mosquito,
    ABM_data$geometry_text
  )

  return(SIR_simulation)
}
