// Copyright (c) 2024, Christoforos Koutsianoudis
// All rights reserved.
//
// Licensed under the MIT License. See LICENSE file for details.
// Created: 2024-11-23
// Author: Christoforos Koutsianoudis <christoforos.koutsianoudis@gmail.com>
// This code follows the Google C++ Style Guide.

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "sysio/csv_handler.hpp"
#include "signal/processing.hpp"
#include "common.hpp"
#include <fstream>
#include "gui/main_gui.hpp"


namespace fs = std::filesystem;
std::vector<std::string> load_csv_filenames(const std::string& directory) {
    std::vector<std::string> filenames;

        // Start from the current directory
    fs::path target_dir = fs::path(directory);

    // go to directories back
    for (int i = 0; i < 2; ++i) {
        target_dir = target_dir.parent_path();
    }

    // Iterate over files in the given directory
    for (const auto& entry : std::filesystem::directory_iterator(target_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            filenames.push_back(entry.path().filename().string());
        }
    }

    // Check if no CSV files are found
    if (filenames.empty()) {
        std::cerr << "No CSV files found in the directory!" << std::endl;
        throw std::runtime_error("No CSV files found.");
    }

    return filenames;
}

SignalData my_sig_data;

int main(){
    // std::vector <Result> results;
    // std::string current_directory = std::filesystem::current_path().string();
    // CsvHandler my_handler = CsvHandler();
    // SignalProcessing my_signal;
    
    // std::vector <std::string> filenames = load_csv_filenames(current_directory);
    // Result result;
    // for (const auto filename : filenames)
    // {
    //     my_sig_data = my_handler.parse(filename);
    // double freq = my_signal.calculate_frequency_FFT(my_sig_data.time,my_sig_data.volts);
    //     std::cout<<"Frequency " << freq << " and filename "<< filename <<std::endl;
    //     result.filename = filename;
    //     result.freq = freq;
    //     results.push_back(result);
    // }
    // my_handler.write("out.csv",results);

    runGui();
}