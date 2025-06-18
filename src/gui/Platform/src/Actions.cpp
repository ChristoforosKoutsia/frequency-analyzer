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

#include <iostream>
#include <vector>
#include <string>
#include "processing.hpp"
#include <filesystem>
#include "sysio/csv_handler.hpp"
#include "signal/processing.hpp"
#include "common.hpp"
#include <fstream>
#include "GraphChart.hpp"
#include "MenuBar.hpp"
#include <QElapsedTimer>
#include <QDateTime>
#include "ConfigDialog.hpp"
/******************************************************* */

static  std::vector<FFTResult> m_fftResults;

/***********Classes Here*****************/

/* call explicitely the Action constructor*/
SidebarHomeAction::SidebarHomeAction(QWidget* widget) : 
                    BaseAction(QIcon("icons/home_1.png"),
                          "Home",false,widget),
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
                    BaseAction(QIcon("icons/upload.svg"),
                          "Signal",false,widget),
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

static double offset = 0.0;
static double previousUpperLimit = 0.0; // To keep track of the upper limit of the previous sequence
for (const QString& fileName : fileNames) 
{
    if (!fileName.isEmpty()) {
        CsvHandler my_handler;
        const std::string loc_filename = fileName.toStdString();
        SignalData my_sig_data = my_handler.parse(loc_filename);
        std::vector<double> normalized_volts = normalizeToMinusOneOne(my_sig_data.volts);
        // Plot the data
        if (!my_sig_data.time.empty() && !normalized_volts.empty()) 
        {
            /* compute the offset so visually they are not all sequences together.
             * the point is that we keep the upper limit of the previous sequence
             * and we add on top*/
             double current_max_voltage = *std::max_element(normalized_volts.begin(), normalized_volts.end());
             double current_min_voltage = *std::min_element(normalized_volts.begin(), normalized_volts.end());
             offset += previousUpperLimit + std::abs(current_min_voltage);
            /* Extracting just the file name (without path) for the plot*/ 
            QFileInfo fileInfo(fileName);
            QString csvName = fileInfo.baseName();
           QLineSeries* current_series =  m_chartWidget->addSeries(my_sig_data.time, normalized_volts, csvName,offset);

            /*random color for now*/
            QColor color = current_series->pen().color();

            /* also the signal should be in the tablewidget*/
            m_tableWidget->AddRow(color,csvName);

            previousUpperLimit = current_max_voltage;
        }
    }

}
}


/* call explicitely the Action constructor*/
SideBarFFTAction::SideBarFFTAction(GraphChart* graph_widget,TableWidget* table)
        : BaseAction(QIcon("icons/fft_icon.svg"), "FFT", false,graph_widget),
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
                : BaseAction(QIcon("icons/toggle_on"), "Show FFT", false), 
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

ShowCursorAction :: ShowCursorAction(ChartView* chart_view)
                    : BaseAction(QIcon("icons/toggle_on"), "Show Cursor", true),
                    m_chart_view(chart_view)
                    {}
void ShowCursorAction::handler()
{
    if (m_chart_view) 
    {
        if (!m_chart_view->isCursorEnabled())
        {
            m_chart_view->setCursorEnabled(true);
        }
        else 
        {
            m_chart_view->setCursorEnabled(false);
        }
        m_chart_view->update(); // Refresh the view to show/hide the cursor
        
    }
}



LiveSectionAction::LiveSectionAction(QStackedWidget* stackedWidget)
: BaseAction(QIcon(""), "Show LiveSection", true),
  m_stackedWidget(stackedWidget)
  {
    /* do nothing*/
  }

void LiveSectionAction::handler()
{
    /* create new graph widget and just one series for now*/
    m_chart = new GraphChart(m_stackedWidget);
    m_stackedWidget->setCurrentIndex(2);
}

GraphChart* LiveSectionAction:: GetGraphChart(void)
{
    return m_chart;
}


LiveRecordingAction::LiveRecordingAction(LiveSectionAction* live_action,GraphChart* graph_chart)
: BaseAction(QIcon(""), "Live Recording", true,live_action),
  m_graph_widget(graph_chart),
  m_series(nullptr)
  {
  }

void LiveRecordingAction::ApplyCOMConfigSettings(SerialComConfig &config)
{
    m_serial_com.SerialComSetCOMConfig(config);
}
void LiveRecordingAction::handler()
{
        //auto* live_action = static_cast<LiveSectionAction*>(parent());
   // m_graph_widget = live_action->GetGraphChart();
        if (!m_series) 
        {
        m_series = new QLineSeries();
        m_series->setName("Signal1");
        m_graph_widget->chart()->addSeries(m_series);
        m_graph_widget->chart()->createDefaultAxes();
        m_series->attachAxis(m_graph_widget->chart()->axisX());
        m_series->attachAxis(m_graph_widget->chart()->axisY());
    }
    /* try to connect and estamblish a connection here*/
    /* first set the settings from the user's configuration*/
       
       m_serial_com.SerialCom_Connect([this](const std::vector<uint8_t>& data)
    {

        if (!m_graph_widget || !m_series)
            return;
        updatePlot(data);
    }
    );
    
}

void LiveRecordingAction::updatePlot(const std::vector<uint8_t>& data)
{
qint64 currentTimeMs = QDateTime::currentMSecsSinceEpoch();
    static qint64 startTimeMs = -1;
    if (startTimeMs < 0)
        startTimeMs = currentTimeMs;

    qint64 elapsedMs = currentTimeMs - startTimeMs;

    /* dummy test for now just take the first signal*/

    for (char byte : data)
    {
        qreal y = static_cast<qreal>(byte);

        // Use elapsed time for X, optionally add offset for each byte if needed
        m_series->append(elapsedMs, y);
    }
     m_graph_widget->chart()->axisX()->setRange(0, elapsedMs + 1000); // adjust as needed
    m_graph_widget->chart()->axisY()->setRange(-5, 5); // for uint8_t data
    m_graph_widget->chart()->update();
}
LiveRecordingAction::~LiveRecordingAction() {
    m_serial_com.SerialCom_Connect(nullptr); // or provide a disconnect method
}

// Actions.cpp
ConfigureSessionAction::ConfigureSessionAction(QWidget* parentWidget,LiveRecordingAction* live_rec_act)
    : BaseAction(QIcon("icons/settings.svg"), "Configure Session", false, parentWidget),
      m_parentWidget(parentWidget),
      m_live_rec(live_rec_act)
{}

void ConfigureSessionAction::handler()
{
    ConfigDialog dlg(m_parentWidget);
    if (dlg.exec() == QDialog::Accepted) 
    {
        m_protocol = dlg.protocol();
        m_baudrate = dlg.baudrate();
        m_port = dlg.port();

        SerialComConfig loc_config; /*local configuration from the user*/
        loc_config.port_name = m_port;
        uint32_t loc_baud_rate  = m_baudrate.toLong();
        loc_config.baud_rate = static_cast<QSerialPort::BaudRate>(loc_baud_rate);
        m_live_rec->ApplyCOMConfigSettings(loc_config);
        // Use these values to configure SerialCom or store for later use
        QMessageBox::information(m_parentWidget, "Session Configured",
            QString("Protocol: %1\nBaudrate: %2\nPort: %3")
            .arg(m_protocol, m_baudrate, m_port));
    }
}

/***********Functions Here*****************/

QLineSeries* findSeriesByName(GraphChart* graph_widget, QString signalName)
{
    // GraphChart inherits from QChart or has a method to get all series
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

std::vector<double> normalizeToMinusOneOne(const std::vector<double>& input) {
    if (input.empty()) return {};
    double minVal = *std::min_element(input.begin(), input.end());
    double maxVal = *std::max_element(input.begin(), input.end());
    std::vector<double> output;
    output.reserve(input.size());
    if (maxVal == minVal) {
        // All values are the same, map to 0
        output.assign(input.size(), 0.0);
    } else {
        for (double v : input) {
            double norm = 2.0 * (v - minVal) / (maxVal - minVal) - 1.0;
            output.push_back(norm);
        }
    }
    return output;
}