/****************************
 * File: Actions.cpp
 * Author: Chris Koutsia
 * Date: 2025-05-15
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Implmentation of Platform SpectraForge specific actions
 ****************************/

/***********Includes Here*****************/
#include "Actions.hpp"
#include <QFileDialog>
#include <QMessageBox>


/******************************** to be removed in a while*************8*/
#include <iostream>
#include <vector>
#include <string>
#include "processing.hpp"
#include <filesystem>
#include "sysio/csv_handler.hpp"
#include "signal/processing.hpp"
#include "common.hpp"
#include <fstream>
/******************************************************* */

/***********Classes Here*****************/

/* call explicitely the Action constructor*/
SidebarHomeAction::SidebarHomeAction(QWidget* widget) : 
                    Action(QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon),
                          "Home",false),
                    m_widget(widget)
                    {}
 
void SidebarHomeAction::handler(void)
{
    if(m_widget)
    {
        /* Home Button shall show the provided widget*/
        m_widget->show();
    }
}

MenuBarAddSignal::MenuBarAddSignal(QWidget* widget, GraphChart* chartWidget,SignalsTableWidget* tableWidget) : 
                    Action(QApplication::style()->standardIcon(QStyle::SP_DirIcon),
                          "Signal",false),
                    m_widget(widget),
                    m_chartWidget(chartWidget),
                    m_tableWidget(tableWidget)
                    {}

void MenuBarAddSignal::handler(void)
{
    if (!m_widget || !m_chartWidget) return;
    QString fileName = QFileDialog::getOpenFileName(
        m_widget,
        "Open CSV File",
        QString(),
        "CSV Files (*.csv)"
    );
    if (!fileName.isEmpty()) {
        CsvHandler my_handler;
        const std::string loc_filename = fileName.toStdString();
        SignalData my_sig_data = my_handler.parse(loc_filename);

        // Plot the data
        if (!my_sig_data.time.empty() && !my_sig_data.volts.empty()) 
        {
            //m_chartWidget->clearChart();

            /* Extracting just the file name (without path) for the plot*/ 
            QFileInfo fileInfo(fileName);
            QString csvName = fileInfo.baseName();
           QLineSeries* current_series =  m_chartWidget->addSeries(my_sig_data.time, my_sig_data.volts, csvName);

            /*random color for now*/
            QColor color = current_series->pen().color();

            /* also the signal should be in the tablewidget*/
            m_tableWidget->AddRow(color,csvName);
        }
    }
}




/* call explicitely the Action constructor*/
SideBarFFTAction::SideBarFFTAction(QWidget* widget)
        : Action(QIcon("icons/fft_icon.png"), "FFT", false),
        m_widget(widget)
        {
        }
 
void SideBarFFTAction::handler(void)
{
    /* this handler shall calculate the FFT and probably create a second graph(?)
     * there shall be information on the selected signal(s) and shall
     * perform action(FFT) on the selected ones and keep them in a global buffer
     * Then for show signals in the frquency domain, shall be different action/button
    */

    QList<QTableWidgetSelectionRange> selectedRanges = m_signalsTable->selectedRanges();
    for (const auto& range : selectedRanges) 
    {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            QString signalName = m_signalsTable->item(row, 1)->text();
            // Do FFT on this signal
        }
    }

    QLineSeries* series = findSeriesByName(m_chart, signalName);
    std::vector<double> time_points;
    std::vector<double> ampl_points;
    if (series) 
    {
        const QList<QPointF>& points = series->points();
        for (const QPointF& pt : points) 
        {
            time_points.push_back(pt.x());
            ampl_points.push_back(pt.y());
        }

        SignalProcessing my_signal;
        std::vector<std::vector<double>> freq_mag = my_signal.calculate_frequency_FFT(time_points,ampl_points);
        const std::vector<double>& frequencies = freq_mag[0];
        const std::vector<double>& magnitudes  = freq_mag[1];

        /* now we have it -- store it in RAM so they can be plotted*/
    }




    return;
}




/***********Functions Here*****************/


