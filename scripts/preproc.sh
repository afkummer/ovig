#!/bin/bash

## script to perform the pre-processing step in OSM files using OSRM

## IMPORTANT: must change the profile for car use so that the metric
## used when computing edge values is distance/duration, in which
## case the triangle inequality will hold.

## first extract the data from OSM file using the profile for cars
../../osrm-backend/build/osrm-extract -p ../../osrm-backend/profiles/car.lua spain-latest.osm.pbf

## partition and customize (whatever this means)
../../osrm-backend/build/osrm-partition spain-latest.osrm
../../osrm-backend/build/osrm-customize spain-latest.osrm

## execute the routing algorithm MLD
##../../osrm-backend/build/osrm-routed --algorithm mld spain-latest.osrm
