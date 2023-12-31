% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/setup.R
\name{flow_data}
\alias{flow_data}
\title{Calculate flow between locations according to population and distance
between spatial structures}
\usage{
flow_data(geo_data)
}
\arguments{
\item{geo_data}{sf object containing at least latitude, longitude, population
and geometry for each spatial structure}
}
\value{
Dataframe with normalized flow probabilities between locations
}
\description{
Calculate flow between locations according to population and distance
between spatial structures
}
\examples{
\dontrun{
flow_data(geo_data)
}
}
