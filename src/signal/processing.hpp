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
   /*method that computes and return the spectrum of a time based signal*/
    std::vector<std::vector<double>> calculate_frequency_FFT(const std::vector<double>& time, std::vector<double>& amplitude);

    /* function that finds the maximum frequency*/
    double FindMaxFreq(std::vector<double> magnitude,std::vector<double> freq);
};

#endif
