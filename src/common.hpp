// common data that may used from all the submodules
// Copyright (c) 2024, Christoforos Koutsianoudis
// All rights reserved.
//
// Licensed under the MIT License. See LICENSE file for details.
// Created: 2024-11-23
// Author: Christoforos Koutsianoudis <christoforos.koutsianoudis@gmail.com>
//
// This code follows the Google C++ Style Guide.
// Description:
/**/

#ifndef common_HPP
#define common_HPP

#include <string>
#include <vector>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <iostream>

struct SignalData{
    std::string filename;
    std::vector<double> time;
    std::vector<double> volts;
};

struct Result{
std::string filename;
double freq;
};

/*public variable*/
extern SignalData my_sig_data;
#endif
