// Copyright (c) 2024, Christoforos Koutsianoudis
// All rights reserved.
//
// Licensed under the MIT License. See LICENSE file for details.
// Created: 2024-11-23
// Author: Christoforos Koutsianoudis <christoforos.koutsianoudis@gmail.com>

// This code follows the Google C++ Style Guide.

#include <vector>
#include <string>
#include "csv_handler.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

void CsvHandler::write(const std::string& outputFile, const std::vector<Result>& results) {
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << outputFile << " for writing.\n";
        return;
    }
    // Write the header
    outFile << "Filename,Frequency (Hz)\n";

    // Sort the results by frequency
    std::vector<Result> sorted_res = results;
    std::sort(sorted_res.begin(), sorted_res.end(),
              [](const Result& a, const Result& b) { return a.freq < b.freq; });

    // Write each result
    for (const auto& result : sorted_res) {
        outFile << result.filename << "," << result.freq << "\n";
    }

    std::cout << "Results successfully written to " << outputFile << "\n";
    outFile.close();
}


SignalData CsvHandler::parse(const std::string& in_filename){
// function that takes as input a filename (csv) and stores the signal data in correspodnding datatype
// Assumes that csvs are two directories back
SignalData sig = SignalData();
sig.filename = in_filename;
std::string full_path = "../../" + in_filename;
std::ifstream inFile(full_path);
 if (!inFile.is_open()) {
    throw std::runtime_error("Error: Could not open file  for reading.\n");
        }
        std::string line;
        bool isHeader = true;  // To skip the header row
        while (std::getline(inFile, line)) {
            // Skip the header row
            if (isHeader) {
                isHeader = false;
                continue;
            }

            // Parse the line (assuming the format is "Seconds, Volts")
            std::stringstream ss(line);
            std::string seconds_str;
            std::string volts_str;
            std::getline(ss, seconds_str, ',');
            
            std::getline(ss, volts_str);

            // Convert the strings to doubles
            double seconds = std::stod(seconds_str);
            double volts = std::stod(volts_str);

            // Store the data
            sig.time.push_back(seconds);
            sig.volts.push_back(volts);
        }
        inFile.close();
        return sig;
    }
