// Copyright (c) 2024, Christoforos Koutsianoudis
// All rights reserved.
//
// Licensed under the MIT License. See LICENSE file for details.
// Created: 2024-11-23
// Author: Christoforos Koutsianoudis <christoforos.koutsianoudis@gmail.com>
//
// This code follows the Google C++ Style Guide.
// Description:
/*
Declaration of csv handler class which is responsible 
for loading, parsing, writing csv files as per requirement
*/

#ifndef csv_handler_HPP
#define csv_handler_HPP

#include <string>
#include "../common.hpp"

class CsvHandler{
 public :
    SignalData parse(const std::string& in_filename);
    void write(const std::string& out_filename,const std::vector<Result>& results);
};
#endif
