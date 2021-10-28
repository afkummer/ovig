# Realistic instance generator for the Home Health Care Routing and Scheduling Problem (HHCRSP)

This repository contains the source code for generating realistic instances for the home health care problem introduced by [Mankowska et al. (2014)](https://doi.org/10.1007/s10729-013-9243-1). The software uses the [OVIG tool](https://github.com/cssartori/ovig) as codebase, and allows generating instances based on realistic addresses and travel times.

The project uses Open Street Maps (OSM) from [Geofabrik](http://download.geofabrik.de/) and the [Open Source Routing Machine](https://github.com/Project-OSRM/osrm-backend) (OSRM) tool to compute realistic travel times. Addresses are currently obtained from the [OpenAddresses](https://openaddresses.io/) database, but any data on geographic locations can be used to feed the OVIG tool as long as it is valid.

## Compiling the code

Compiling the project is somewhat a straightforward process for those who have been working in UNIX/Linux environments for a while. As this fork of the original repository only adds some particular changes to the OVIG source code, we suggest the user to follow OVIG's original instructions for building the project.

## Running

Different than OVIG, we use a shell script as front-end to the OVIG main executable. To run this script, you need to change the [template-sed.conf](template-sed.conf) configuration file with the path to your copy of OSRM dataset in line `OSRM-FILE:`. After that, you can generate new instances though the [generate.sh](generate.sh) script.

