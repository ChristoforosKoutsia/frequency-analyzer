/****************************
 * File: Actions.hpp
 * Author: Chris Koutsia
 * Date: 2025-05-15
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Specific Actions for this specific app/platform(SpectraForge)
 * We will define following Actions 
 * 
 * For SideBar
 * 1. Home
 * 2. Settings
 * 3. Help
 * 3. Exit
 * 
 * For Menubar
 * File
 * Signal
 * View
 ****************************/

#ifndef ACTIONS_HPP
#define ACTIONS_HPP

/***********Includes Here*****************/
#include "MainWindows.hpp"
#include "SignalsTableWidget.hpp"


/*****************************************/

/***********Structs Here*****************/

/* Store FFT Results*/
struct FFTResult 
{
    QString signalName;
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
};

/*****************************************/

/***********Class Declaration Here*****************/
class SidebarHomeAction : public Action
{
    public:
        explicit SidebarHomeAction(QWidget* widget);
        void handler(void) override;
    private:
        /* define hete the Icon*/
        QIcon m_homeIcon = QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon);
        QWidget* m_widget;
};

/*Add signal action. This action opens WindowExplorer*/
class MenuBarAddSignal : public Action
{
    public:
        explicit MenuBarAddSignal(QWidget* widget, GraphChart* chartWidget,SignalsTableWidget* tableWidget);
        void handler(void) override;
    private:
        /* Define hete the Icon*/
        QIcon m_addSignalIcon = QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon);

        /* Main Widget where the File Browser will open*/
        QWidget* m_widget;
        GraphChart* m_chartWidget;
        SignalsTableWidget* m_tableWidget;
};
/*****************************************/

/*create an action where for the selected signal performs FFT
 and plots the Signal in the frequency domain*/
class SideBarFFTAction : public Action
{
    public:
        explicit SideBarFFTAction(GraphChart* graph_widget);
        void handler(void) override;
    private:
        /* define hete the Icon*/
        std::vector<FFTResult> m_fftResults;
        GraphChart* m_graph_widget;
};
#endif // ACTIONS_HPP