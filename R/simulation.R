#' @title Run simulation for a given number of days.
#'
#' @param ABM_model S3 class object containing the parameters for the
#' simulation.
#' @param time Integer for the amount of days to simulate.
#'
#' @return Data frame containing SIR simulation results per day.
#' @examples
#' \dontrun{
#' simulation(ABM_model, 100)
#' }
#' @export
simulation <- function(ABM_model, time) {
  SIR_simulation <- simulate(
    time,
    ABM_model$demographic_data,
    ABM_model$temperature_data,
    ABM_model$movement_data,
    ABM_model$incubation_period,
    ABM_model$infection_duration,
    ABM_model$init_infected_humans,
    ABM_model$init_infected_mosquito
  )

  return(SIR_simulation)
}
