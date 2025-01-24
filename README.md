
<!-- README.md is generated from README.Rmd. Please edit that file. -->
<!-- The code to render this README is stored in .github/workflows/render-readme.yaml -->
<!-- Variables marked with double curly braces will be transformed beforehand: -->
<!-- `packagename` is extracted from the DESCRIPTION file -->
<!-- `gh_repo` is extracted via a special environment variable in GitHub Actions -->

# iraca

<!-- badges: start -->

[![License:
MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/license/mit/)
[![R-CMD-check](https://github.com/epiverse-trace/iraca/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/epiverse-trace/iraca/actions/workflows/R-CMD-check.yaml)
[![Codecov test
coverage](https://codecov.io/gh/epiverse-trace/iraca/branch/main/graph/badge.svg)](https://app.codecov.io/gh/epiverse-trace/iraca?branch=main)
[![lifecycle-concept](https://raw.githubusercontent.com/reconverse/reconverse.github.io/master/images/badge-concept.svg)](https://www.reconverse.org/lifecycle.html#concept)
<!-- badges: end -->

iraca is focused on the simulation of contagion scenarios given by the
interaction between mosquitoes and humans at individual levels, while
allowing to include possible interventions and their aftermath. The
package

iraca is a package built to model vector-borne diseases based on
human-mosquito interaction. It relies on Agent-Based models to simulate
individual behavior in the Colombian context. The library can be used
to:

- Simulate mosquito-human interaction and possible aftermath given
  different contagion parameters and scenarios.
- Calibrate and obtain current parameters for simulation and analysis.
- Integrate policies and local/individual interventions to simulate
  their impact on vector-borne diseases.
- Support the validation process of intervention policies.

The current model is only fit for Dengue Virus. In addition, it is very
important to note that the approach given by the package will be purely
theoretical and should not be used for inference or immediate decision
making without proper analysis.

<!-- This sentence is optional and can be removed -->

iraca is developed at [Universidad de Los
Andes](https://uniandes.edu.co/) as part of the [Epiverse-TRACE
program](https://data.org/initiatives/epiverse/).

## Installation

You can install the development version of iraca from
[GitHub](https://github.com/) with:

``` r
# install.packages("pak")
pak::pak("epiverse-trace/iraca")
```

## Motivation

Modern tools like Agent-Based Modeling (ABM) have been lately used to
create robust simulations for epidemiological analysis; however, they
are usually built for specific scenarios rather than a general approach.
When analyzing vector-borne diseases like dengue, zika or chikungunya,
part the process can be standardized since the vector is (usually) the
same mosquito. Some available tools and libraries include a general ABM
approach; however, they are still limited when modelling big volumes of
data or very specific simulations. iraca solves the need for an ABM
focused on Human-Mosquito interaction that’s specific enough to capture
the behavior of a disease, but at the same time flexible to consider
multiple scenarios.

This library is aimed at epidemiologists, researchers, local public
health decision-makers, and anyone interested in the dynamics of
vector-borne diseases.

## Workflow

The user will interact with the library using a `model` object with the
required information. Users will input direct information from the
National Geostatistical Framework provided by the National Department of
Statistics (DANE). This dataset should include the socioeconomic and
demographic details for all municipalities of the country. In addition,
climate data from the Institute of Hydrology, Meteorology and
Environmental Studies should be included to simulate weather patterns.
If looking for a fitted calibration, observed cases for the period of
interest are required, which will be used to calculate the set of
parameters that best fit the real data by minimizing RMSE. Finally,
simulation can be run with or without interventions, either to recreate
past events or evaluate future scenarios.

The simulation allows the differentiation of three types of agents:
humans, mosquitoes and territories. All three are created from the input
data stated above, and assigned different movement and behavioral
patterns. Interventions can also be included in the simulation,
including use of mosquito nets, insecticides and container cleaning.

To set up the model, we can use the function `setup`. Users may also
indicate different incubation and infection periods, as well as initial
parameters regarding infected population (humans and mosquitoes).

``` r
library(iraca)

# load example data, which is from the city of Ibague, Colombia
data(demographic_data_ibague)
data(temperature_data_ibague)

# Setup model
model <- setup(
  demographic_data = demographic_data_ibague,
  temperature_data = temperature_data_ibague,
  movement_data = NULL,
  demographic_data_MGN = TRUE,
  divipola_code = NULL,
  disease = "dengue",
  incubation_period = 4,
  infection_duration = 14,
  init_infected_humans = 0.005,
  init_infected_mosquitoes = 0.03
)
```

After setting up the model, we can start the model to make a simulation
for a given amount of days (steps).

``` r
simulation(ABM_data = model,
           time = 100)
```

To allow larger and faster simulations, the model implementation relies
on C++ and is liked to R through ´Rcpp´. The library uses only
`S3`object to make data input and output as simple as possible for the
user.

### Lifecycle

This package is currently a *concept*, as defined by the [RECON software
lifecycle](https://www.reconverse.org/lifecycle.html). This means that
essential features and mechanisms are still being developed, and the
package is not ready for use outside of the development team.

After further consideration, this package development is temporarily
halted. Usability issues were contemplated regarding the actual use in
the local context given the lack of human resources and time
constraints; in addition, ethical concerns were also taken into account,
given that the open release of very sensitive tools which relies on fine
tuning and calibration could easily lead to poor evidence production for
decision-making.

### Contributions

Contributions are welcome via [pull
requests](https://github.com/epiverse-trace/iraca/pulls).

### Code of Conduct

Please note that the iraca project is released with a [Contributor Code
of
Conduct](https://github.com/epiverse-trace/.github/blob/main/CODE_OF_CONDUCT.md).
By contributing to this project, you agree to abide by its terms.
