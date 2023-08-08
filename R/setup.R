
#' @title Setup ABM data and basic parameters
#'
#' @param demographic_data sf object with the demographic data in the specified 
#' format. It's expected to be input by the user from the DANE's MGN (Marco 
#' Geoestadístico Nacional).
#' @param temperature_data Dataframe with the temperature historic. It's 
#' expected to be input from the historic data at IDEAM's historic temperature  
#' @param movement_data Dataframe with the movement flow between territories. 
#' If not provided it's autimatically calculated from the demographic data using
#' distance and population
#' @param divipola_code Divipola code of the municipality to be simulated 
#' @param disease Character string for the disease name. Can be either "dengue",
#' "zika" or "chikungunya".
#' @param incubation_period Incubation period of the disease
#' @param init_infected_humans Percentage of initially infected humans
#' @param init_infected_mosquito Percentage of initially infected mosquitoes
#'
#' @return S3 ABM_model object with the saved initial parameters
#'
#' @examples
#' \dontrun{
#' setup()
#' }
#' @export
setup <- function(demographic_data,
                  temperature_data,
                  movement_data = NULL,
                  demographic_data_MGN = TRUE,
                  divipola_code = NULL,
                  disease = "dengue",
                  incubation_period = 14,
                  init_infected_humans = 0.005,
                  init_infected_mosquito = 0.03)
{
  if(demographic_data_MGN && !is.null(divipola_code))
  {
    demographic_data <- filter_demographic_data(demographic_data)
  }
  
  if(is.null(movement_data))
  {
    movement_data <- flow_data(demographic_data)
  }
  
  model <- list(demographic_data = sf::st_drop_geometry(demographic_data),
                temperature_data = temperature_data,
                movement_data = movement_data,
                divipola_code = divipola_code,
                disease = disease,
                incubation_period = incubation_period,
                init_infected_humans = init_infected_humans,
                init_infected_mosquito = init_infected_mosquito)
  class(model) <- "ABM_model"
  
  return(model)
}

#' @title Filter demographic data when imported directly from the MGN 
#'
#' @param demographic_data sf object with demographic data directly imported 
#' from the MGN.
#'
#' @return sf object with filtered data, containing only the necessary columns 
#' for simulation
#' @examples
#' \dontrun{
#' filter_demographic_data(demographic_data)
#' }
filter_demographic_data <- function(demographic_data)
{
  # temporal test data
  #temp_data <- 
  #  sf::read_sf("C:/Users/Julia/Downloads/SHP_MGN2018_INTGRD_SECTU/MGN_ANM_SECTOR_URBANO.shp")
  filtered_data <- temp_data %>% 
    dplyr::filter(as.numeric(MPIO_CDPMP) == divipola_code,
                  as.numeric(SECR_CCDGO) == 0) %>% 
    dplyr::select(SETU_CCDGO, AREA, LATITUD, LONGITUD, STVIVIENDA, STP19_ALC1,
                  STP19_GAS1, STP27_PERS, STP32_1_SE, STP32_2_SE, STP34_1_ED,
                  STP34_2_ED, STP34_3_ED, STP34_4_ED, STP34_5_ED, STP34_6_ED,
                  STP34_7_ED, STP34_8_ED, STP34_9_ED, STP51_SUPE, 
                  STP51_POST) %>%
    dplyr::rename(territory = SETU_CCDGO,
                  area = AREA,
                  latitude = LATITUD, 
                  longitude = LONGITUD, 
                  houses = STVIVIENDA, 
                  sewage = STP19_ALC1,
                  gas = STP19_GAS1, 
                  population = STP27_PERS, 
                  men = STP32_1_SE, 
                  women = STP32_2_SE, 
                  age_0_9 = STP34_1_ED,
                  age_10_19 = STP34_2_ED, 
                  age_20_29 = STP34_3_ED, 
                  age_30_39 = STP34_4_ED, 
                  age_40_49 = STP34_5_ED, 
                  age_50_59 = STP34_6_ED,
                  age_60_69 = STP34_7_ED, 
                  age_70_79 = STP34_8_ED, 
                  age_80_up = STP34_9_ED,
                  sup_ed = STP51_SUPE, 
                  post_ed = STP51_POST) %>%
    dplyr::mutate(territory = as.numeric(territory),
                  sewage = sewage/houses,
                  gas = gas/houses,
                  density = population/area,
                  population = population,
                  men = men/population,
                  women = women/population,
                  age_0_9 = age_0_9/population,
                  age_10_19 = age_10_19/population, 
                  age_20_29 = age_20_29/population, 
                  age_30_39 = age_30_39/population, 
                  age_40_49 = age_40_49/population, 
                  age_50_59 = age_50_59/population,
                  age_60_69 = age_60_69/population, 
                  age_70_79 = age_70_79/population, 
                  age_80_up = age_80_up/population,
                  high_ed = (sup_ed + post_ed)/population, 
                  .keep = "unused") %>%
    dplyr::relocate(geometry, .after = last_col())
  return(filtered_data)
}

#' @title Calculate flow between locations according to population and distance 
#' between spatial structures
#'
#' @param geo_data sf object containing at least latitude, longitude, population
#'  and geometry for each spatial structure
#'
#' @return Dataframe with normalized flow probabilities between locations
#' 
#' @examples
#' \dontun{
#' flow_data(geo_data)
#' }
flow_data <- function(geo_data)
{
  geo_data$territory <- seq(1, nrow(geo_data))
  coordinates <- as.data.frame(cbind(geo_data$longitude, geo_data$latitude))
  coordinates <- sf::st_as_sf(coordinates, coords = c("V1", "V2"))
  coordinates <- sf::st_set_crs(coordinates, sf::st_crs(geo_data))
  
  distances <- sf::st_distance(coordinates, coordinates)
  distances_mat <- units::drop_units(distances)
  colnames(distances_mat) <- geo_data$territory
  row.names(distances_mat) <- geo_data$territory
  
  flow <- matrix(0, nrow(distances_mat), ncol(distances_mat))
  
  for(i in geo_data$territory){
    pop_i <- geo_data$population[geo_data$territory == i]
    for(j in geo_data$territory){
      if (i > j){
        pop_j <- geo_data$population[geo_data$territory == j]
        flow[i,j] <- pop_i * pop_j / (distances_mat[i,j] ^ 2)
      }
    }
  }
  flow_matrix <- flow + t(flow) - diag(flow)
  flow_matrix <- scale(flow_matrix, 
                       center = FALSE, 
                       scale = colSums(flow_matrix)) %>% as.data.frame()
  return (flow_matrix)
}