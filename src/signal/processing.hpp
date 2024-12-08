// Copyright (c) 2024, Christoforos Koutsianoudis
// All rights reserved.
// Licensed under the MIT License. See LICENSE file for details.
// Created: 2024-11-23
// Author: Christoforos Koutsianoudis <christoforos.koutsianoudis@gmail.com>
// Description:
   /*
   processing class is responsible for analyzing the dominant 
   frequency of given data or any other signal processing action needed
   */
// This code follows the Google C++ Style Guide.

#ifndef processing_HPP
#define processing_HPP

#include "../common.hpp"

class SignalProcessing{
 public:
    double calculate_frequency_FFT(const std::vector<double>& time, std::vector<double>& amplitude);
};

#endif
