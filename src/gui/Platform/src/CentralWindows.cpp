/****************************
 * File: CentralWindows.cpp
 * Author: Chris Koutsia
 * Date: 2025-05-27
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Concrete Implementation for Spectral project for
 * the Central Windows Widgets of the Gui.
 ****************************/

/***********Includes Here*****************/
#include "CentralWindows.hpp"
#include "MenuBar.hpp"

/*****************************************/


/***********Classes Here*****************/
/***************** MainWindow Implementation **************************/

void MainWindow::AddMenuBar(BaseMenuBar *menu)
{
    if (menu != nullptr)
    {
        setMenuBar(menu);
    }
}

void MainWindow::AddToolbar(BaseToolbar *toolbar)
{
    if (toolbar != nullptr)
    {
        addToolBar(toolbar);
    }
}

void MainWindow::AddCentralWidget(QWidget *widget)
{
    if (widget != nullptr)
    {
        setCentralWidget(widget);
    }
}

void MainWindow::AddTableWidget(QTableWidget* table)
{
    if (table) 
    {
        setCentralWidget(table);
    }
}

/***********Functions Here*****************/


