// Copyright (c) 2024, Christoforos Koutsianoudis
// All rights reserved.
//
// Licensed under the MIT License. See LICENSE file for details.
// Created: 2024-11-23
// Author: Christoforos Koutsianoudis <christoforos.koutsianoudis@gmail.com>

// This code follows the Google C++ Style Guide.

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "processing.hpp"
#include "../3rdparty/fftw-3.3.5-dll64/fftw3.h"
#include <complex>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::vector<std::vector<double>> SignalProcessing::calculate_frequency_FFT(const std::vector<double>& time, std::vector<double>& amplitude) {
    size_t n = amplitude.size();

    double fs = 1.0 / (time[1] - time[0]); // getting the sampling rate assuming that is uniform

    // removing dc offset to
    double mean = std::accumulate(amplitude.begin(), amplitude.end(), 0.0) / n;
    for (auto& value : amplitude) {
        value -= mean;
    }

    // apply Hanning window to reduce spectral leakage
    for (size_t i = 0; i < n; ++i) {
        amplitude[i] *= 0.5 * (1 - cos(2 * M_PI * i / (n - 1)));
    }

    std::vector<std::complex<double>> fft_output(n);
    fftw_plan plan = fftw_plan_dft_r2c_1d(n, amplitude.data(), reinterpret_cast<fftw_complex*>(fft_output.data()), FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    std::vector<double> frequencies(n / 2), magnitude(n / 2);
    for (size_t i = 0; i < n / 2; ++i) 
    {
        frequencies[i] = i * fs / n;
        magnitude[i] = std::abs(fft_output[i]);
    }

    std::vector<std::vector<double>> res(2);
    res[0] = frequencies;
    res[1] = magnitude;

    /* return a vecotr of two vectors with frequencies and magnitudes*/
    return res;
}

double SignalProcessing::FindMaxFreq(std::vector<double> magnitude,std::vector<double> freq)
{
    
    // calculate the frequency with the highest amplitude
    auto max_it = std::max_element(magnitude.begin(), magnitude.end());
    size_t peak_idx = std::distance(magnitude.begin(), max_it);

    return freq[peak_idx];
}