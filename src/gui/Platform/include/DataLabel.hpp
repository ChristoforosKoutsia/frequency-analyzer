/****************************
 * File: DataLabel.hpp
 * Author: Chris Koutsia
 * Date: 2025-05-27
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Concrete implementation of Datalabel
 ****************************/

#ifndef DATALABEL_HPP
#define DATALABEL_HPP

/***********Includes Here*****************/
#include "MainWindows.hpp"


/*****************************************/

/***********Macros Here*****************/


/*****************************************/

/***********Class Declaration Here*****************/
class DataLabel : public BaseDataLabel
{
    Q_OBJECT
public:
    explicit DataLabel(QWidget *parent = nullptr);

    void setData(double x, double y) override;
    void setData(const QString& text) override;
    void clearData() override;
};


/*****************************************/

#endif // DATALABEL_HPP