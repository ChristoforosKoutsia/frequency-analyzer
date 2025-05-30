/****************************
 * File: MainWindows.cpp
 * Author: Chris Koutsia
 * Date: 2025-05-09
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Description of the file
 ****************************/

/***********Includes Here*****************/
#include "MainWindows.hpp"
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>




/*****************************************/

/***********Macros Here*****************/


/*****************************************/

/***********Classes Here*****************/

BaseMenuBar::BaseMenuBar(const std::vector<BaseAction*>& actions,QWidget *parent) 
    : QMenuBar(parent)
    {
      SetupActions(actions);  
    }


void BaseMenuBar::SetupActions(const std::vector<BaseAction*>& actions) 
{
    for (auto action : actions) 
    {

        // Add the QAction to the toolbar
        addAction(action);

        if(action->AddSeparator())
        {
            this->addSeparator(); // Add a separator if requested
        }
    }
}
BaseToolbar::BaseToolbar(QWidget *parent,const std::vector<BaseAction*>& actions) 
    : QToolBar(parent)
    {
      SetupActions(actions);  
    }

void BaseToolbar::SetupActions(const std::vector<BaseAction*>& actions) 
{
    for (auto action : actions) 
    {
        // Add the QAction to the toolbar
        addAction(action);

        if(action->AddSeparator())
        {
            this->addSeparator(); // Add a separator if requested
        }
    }
}


/***********Functions Here*****************/


