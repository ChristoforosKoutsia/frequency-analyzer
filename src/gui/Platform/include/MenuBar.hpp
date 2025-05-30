/****************************
 * File: MenuBar.hpp
 * Author: Chris Koutsia
 * Date: 2025-05-27
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Description of the file
 ****************************/

#ifndef MENUBAR_HPP
#define MENUBAR_HPP

/***********Includes Here*****************/
#include "MainWindows.hpp"


/*****************************************/


/***********Class Declaration Here*****************/
class MenuBar : public BaseMenuBar
{
Q_OBJECT
public:
    explicit MenuBar(const std::vector<BaseAction*>& actions, 
                    QWidget *parent = nullptr):
    // just call the constructor of Base class
    BaseMenuBar(actions,parent){}
};

class ToolBar : public BaseToolbar
{
Q_OBJECT
public:
    explicit ToolBar(QWidget *parent,
                    const std::vector<BaseAction*>& actions 
                    ):
    // just call the constructor of Base class
    BaseToolbar(parent,actions){}
};

/*****************************************/

#endif // MENUBAR_HPP