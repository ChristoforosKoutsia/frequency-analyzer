/****************************
 * File: CentralWindows.hpp
 * Author: Chris Koutsia
 * Date: 2025-05-28
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Description of the file
 ****************************/

#ifndef CENTRALWINDOWS_HPP
#define CENTRALWINDOWS_HPP

/***********Includes Here*****************/
#include "MainWindows.hpp"


/*****************************************/

/***********Class Declaration Here*****************/
/* Base class for MainWindow */
class MainWindow : public BaseMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr) : BaseMainWindow(parent) {}
     void AddMenuBar(BaseMenuBar* menu) override;
     void AddToolbar(BaseToolbar *toolbar) override;
     void AddCentralWidget(QWidget *widget) override;
     void AddTableWidget(QTableWidget *table) override;
};


/*****************************************/

#endif // CENTRALWINDOWS_HPP