// Copyright 2023 `iraca` authors.

// [[Rcpp::depends(BH)]]
#include <Rcpp.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <iostream>
#include <string>
#include <vector>

// typedef boost::geometry::model::d2::point_xy<double> point_xy;
//typedef boost::geometry::geometries::point_xy<double> point_t;
//typedef boost::geometry::geometries::polygon<point_t> polygon_t;
//typedef boost::geometry::geometries::multi_polygon<polygon_t> multi_polygon_t;

//namespace bg = boost::geometry;

// [[Rcpp::export]]
void testGeometry(const std::string& wkt_t2) {
  boost::geometry::model::multi_polygon<boost::geometry::model::polygon<
    boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>>> 
    pointt;
  std::string wkt_t = "MULTIPOLYGON (((30 20, 45 40, 10 40, 30 20)),((15 5, 40 10, 10 20, 5 10, 15 5)))";
  boost::geometry::read_wkt(wkt_t, pointt);
  std::cout << "done" << std::endl;
}
