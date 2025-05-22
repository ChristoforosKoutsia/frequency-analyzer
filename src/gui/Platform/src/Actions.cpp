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

static  std::vector<FFTResult> m_fftResults;

/***********Classes Here*****************/

/* call explicitely the Action constructor*/
SidebarHomeAction::SidebarHomeAction(QWidget* widget) : 
                    Action(QIcon("icons/home.svg"),
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
                    Action(QIcon("icons/upload.svg"),
                          "Signal",false),
                    m_widget(widget),
                    m_chartWidget(chartWidget),
                    m_tableWidget(tableWidget)
                    {}

void MenuBarAddSignal::handler(void)
{
    if (!m_widget || !m_chartWidget) return;
QStringList fileNames = QFileDialog::getOpenFileNames(
    m_widget,
    "Open CSV Files",
    QString(),
    "CSV Files (*.csv)"
);
for (const QString& fileName : fileNames) 
{
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
}




/* call explicitely the Action constructor*/
SideBarFFTAction::SideBarFFTAction(GraphChart* graph_widget,TableWidget* table)
        : Action(QIcon("icons/fft_icon.svg"), "FFT", false),
        m_graph_widget(graph_widget),
        m_signals_table(table)
        {
        }
 
void SideBarFFTAction::handler(void)
{
    /* this handler shall calculate the FFT and probably create a second graph(?)
     * there shall be information on the selected signal(s) and shall
     * perform action(FFT) on the selected ones and keep them in a global buffer
     * Then for show signals in the frquency domain, shall be different action/button
    */

    QList<QTableWidgetSelectionRange> selectedRanges = m_signals_table->selectedRanges();
    for (const auto& range : selectedRanges) 
    {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) 
        {
            QString signalName = m_signals_table->item(row, 1)->text();

            /* perform FFT and store the results*/
            FFTCalcAndStore(signalName);
        }
    }
    return;
}


void SideBarFFTAction::FFTCalcAndStore(const QString& signalName)
{
    QLineSeries* series = findSeriesByName(m_graph_widget, signalName);
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

        /* store the Color*/
        QColor color = series->color();
        /* now we have it -- store it in RAM so they can be plotted later*/
            FFTResult result;
            result.signalName = signalName;
            result.frequencies = frequencies;
            result.magnitudes = magnitudes;
            result.color = color;
            m_fftResults.push_back(result);
    }
}

ShowFFTAction::ShowFFTAction(QStackedWidget* stackedWidget, GraphChart* fft_chart)
                : Action(QIcon("icons/toggle_on"), "Show FFT", false), 
                m_stackedWidget(stackedWidget),
                m_fft_graph(fft_chart)
                {}

void ShowFFTAction::handler()
{
    m_fft_graph->clearChart();
    for (const FFTResult& result : m_fftResults) 
    {
        QLineSeries* fft_series = m_fft_graph->addSeries(result.frequencies, result.magnitudes, result.signalName);
        fft_series->setColor(result.color);
    }
    m_fft_graph->setTitle("Frequency Domain (FFT)");
    m_stackedWidget->setCurrentIndex(1); // Show the FFT chart
}


/***********Functions Here*****************/

QLineSeries* findSeriesByName(GraphChart* graph_widget, QString signalName)
{
    // Assuming GraphChart inherits from QChart or has a method to get all series
    const auto seriesList = graph_widget->chart()->series(); //
    for (auto* s : seriesList) 
    {
        if (s->name() == signalName) 
        {
            return qobject_cast<QLineSeries*>(s);
        }
    }
    return nullptr;
}
