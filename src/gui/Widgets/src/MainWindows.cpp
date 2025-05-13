/****************************
 * File: MainWindows.cpp
 * Author: Chris Koutsia
 * Date: 2025-05-09
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Description of the file
 ****************************/

/***********Includes Here*****************/
#include "../include/MainWindows.hpp"
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>


/*****************************************/

/***********Macros Here*****************/


/*****************************************/

/***********Classes Here*****************/
Toolbar::Toolbar(QWidget *parent, std::vector<Action> actions) 
    : QToolBar(parent){
      SetupActions(actions);  
    }
void Toolbar::_AddActionToToolbar(QAction *action, bool addSeparator) 
{
    addAction(action);
    if (addSeparator) 
    {
        this->addSeparator(); // Add a separator if requested
    }
}

QAction* Toolbar::_CreateAction(Action &action)
{
     // Create a new QAction with the icon and text from the Action object
     QAction *qAction = new QAction(action.GetQIcon(), action.GetQstring(), this);

    // Connect the QAction's triggered signal to the Action's handler
    connect(qAction, &QAction::triggered, [&action]() {
        action.handler(); // Call the handler method of the Action object
    });

    return qAction;
}

void Toolbar::SetupActions(std::vector<Action> actions) {
    for (auto &action : actions) {
        // Create a QAction from the Action object
        QAction *qAction = _CreateAction(action);

        // Add the QAction to the toolbar
        _AddActionToToolbar(qAction, action.AddSeparator());
    }
}

Action::Action(const QIcon &icon, const QString &text, bool addSeparator)
    : m_qicon(icon), m_qstring(text), m_add_separator(addSeparator) {}

void Action::handler() {
    // Default handler; override or extend as needed
}

QIcon Action::GetQIcon() {
    return m_qicon;
}

QString Action::GetQstring() {
    return m_qstring;
}

bool Action::AddSeparator() {
    return m_add_separator;
}

/***************** MainWindow Implementation **************************/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent){}

void MainWindow::AddMenuBar(QMenuBar *menu)
{
    if (menu != nullptr)
    {
        setMenuBar(menu);
    }
}

void MainWindow::AddToolbar(Toolbar *toolbar)
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

/************* Graph chart implementation ************************/
/* TODO : shall be revised*/
GraphChart::GraphChart(QWidget *parent)
    : QWidget(parent)
{
    m_chart = new QChart();
    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    setLayout(layout);

    m_chart->setTitle("Signal Plot");
}

void GraphChart::createChart(const QString &title)
{
    m_chart->removeAllSeries();
    m_chart->setTitle(title);
    m_chart->createDefaultAxes();
}


QLineSeries* GraphChart::addSeries(const std::vector<double> &x, const std::vector<double> &y, const QString &seriesName)
{
    if (x.size() != y.size() || x.empty())
        return nullptr;

    auto *series = new QLineSeries();
    series->setName(seriesName);

    for (size_t i = 0; i < x.size(); ++i)
        series->append(x[i], y[i]);

    m_chart->addSeries(series);
    m_chart->createDefaultAxes();
    return series;
}

void GraphChart::clearChart()
{
    m_chart->removeAllSeries();
}

void GraphChart::setTitle(const QString &title)
{
    m_chart->setTitle(title);
}

QChart* GraphChart::chart()
{
    return m_chart;
}

QChartView* GraphChart::chartView()
{
    return m_chartView;
}




/***********Functions Here*****************/


