/****************************
 * File: DataLabel.cpp
 * Author: Chris Koutsia
 * Date: 2025-05-27
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Description of the file
 ****************************/

/***********Includes Here*****************/
#include "DataLabel.hpp"

/*****************************************/

/***********Classes Here*****************/
DataLabel::DataLabel(QWidget *parent)
    : BaseDataLabel(parent)
{
    setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setText("X: ---, Y: ---");
}

void DataLabel::setData(double x, double y)
{
    setText(QString("X: %1, Y: %2").arg(x, 0, 'g', 6).arg(y, 0, 'g', 6));
}

void DataLabel::setData(const QString& text)
{
    setText(text);
}

void DataLabel::clearData()
{
    setText("X: ---, Y: ---");
}


/***********Functions Here*****************/


