/****************************
 * File: MainWindows.hpp
 * Author: Chris Koutsia
 * Date: 2025-05-08
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Declaration of MainWindows such as Toolbars,Menus etc
 ****************************/

#ifndef MAINWINDOWS_HPP
#define MAINWINDOWS_HPP

#include <QToolBar>
#include <QAction>
#include <QObject>
#include <QMainWindow>
#include <QMenuBar>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QXYSeries>
#include <QGraphicsLineItem>
#include <QTableWidget>
#include <QToolTip>
#include <vector>


/**
* @brief Here an abstraction layer is implemented with general Widgets
* This abstraction layer shall be used eventually in Platform folder
* Where all the project-specific details and the supported functionality is there
* All the classes implemented here shall be abstract and no object of them shall be
* initialized.
* 
*/

/* Base class for DataLabel */
class BaseDataLabel : public QLabel
{
    Q_OBJECT
public:
    explicit BaseDataLabel(QWidget *parent = nullptr) : QLabel(parent) {}
    virtual void setData(double x, double y) = 0;
    virtual void setData(const QString& text) = 0;
    virtual void clearData() = 0;
};

class BaseAction : public QAction {
    Q_OBJECT
public:
    BaseAction(const QIcon &icon, const QString &text, bool addSeparator, QObject *parent = nullptr)
        : QAction(icon, text, parent), m_add_separator(addSeparator)
    {
        setToolTip(text);
        connect(this, &QAction::triggered, this, &BaseAction::handler);
    }

    virtual void handler() = 0;
    bool AddSeparator() const { return m_add_separator; }

private:
    bool m_add_separator;
};

/* Base class for MenuBar */
class BaseMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit BaseMenuBar(const std::vector<BaseAction*>& actions, QWidget *parent = nullptr);
    void SetupActions(const std::vector<BaseAction*>& actions);
};

/* Base class for Toolbar */
class BaseToolbar : public QToolBar
{
    Q_OBJECT
public:
    explicit BaseToolbar(QWidget *parent, const std::vector<BaseAction*>& actions);
    void SetupActions(const std::vector<BaseAction*>& actions);
};

/* Base class for MainWindow */
class BaseMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit BaseMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {}
    virtual void AddMenuBar(BaseMenuBar* menu) = 0;
    virtual void AddToolbar(BaseToolbar *toolbar) = 0;
    virtual void AddCentralWidget(QWidget *widget) = 0;
    virtual void AddTableWidget(QTableWidget *table) = 0;
};

/* Base class for ChartView */
class BaseChartView : public QChartView
{
    Q_OBJECT
public:
    explicit BaseChartView(QChart *chart, QWidget *parent = nullptr) : QChartView(chart, parent) {}
    virtual void setDataLabel(BaseDataLabel* label) = 0;
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override = 0;
    virtual void leaveEvent(QEvent *event) override = 0;
    virtual void wheelEvent(QWheelEvent *event) override = 0;
    virtual void keyPressEvent(QKeyEvent *event) override = 0;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override = 0;
    virtual void mousePressEvent(QMouseEvent *event) override = 0;
};

/* Base class for GraphChart */
class BaseGraphChart : public QWidget
{
    Q_OBJECT
public:
    explicit BaseGraphChart(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual void createChart(const QString &title = "Signals") = 0;
    virtual QLineSeries* addSeries(const std::vector<double> &x, 
                                const std::vector<double> &y,
                                const QString &seriesName = "Series",
                                const double& yoffset = 0u) = 0;
    virtual void clearChart() = 0;
    virtual void setTitle(const QString &title) = 0;
    virtual QChart* chart() = 0;
    virtual QChartView* chartView() = 0;
    virtual void setSeriesVisible(const QString& name, bool visible) = 0;
};

/* Base class for TableWidget */
class BaseTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit BaseTableWidget(
        int colCount,
        QWidget *parent = nullptr,
        int row = 0
    ) : QTableWidget(row,colCount,parent) {}
     virtual void AddRow() = 0;
};

#endif // MAINWINDOWS_HPP