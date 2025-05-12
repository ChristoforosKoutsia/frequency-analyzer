/****************************
 * File: MainWindows.hpp
 * Author: Chris Koutsia
 * Date: 2025-05-08
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Declaration of MainWindows such as Toolbars,Menus etc
 ****************************/

#ifndef MAINWINDOWS_HPP
#define MAINWINDOWS_HPP

/***********Includes Here*****************/
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
#include <vector>


/*****************************************/

/***********Macros Here*****************/


/*****************************************/

/***********Class Declaration Here*****************/

/* This class does not really belongs here. We should move it around*/
class Action
{
    /*class that provides all the information needed 
     for creating an Action. Provides a handler to handle
     and action as well but does not create an action itself*/
    public : 
        explicit Action(const QIcon & ,
                        const QString & ,
                        bool addSeparator
                    );

        /* handler for each specific action*/
        virtual void handler(void);
        QIcon GetQIcon();
        QString GetQstring();
        bool AddSeparator();        
    private :
       QIcon m_qicon;
       QString m_qstring;
       bool m_add_separator;
};


class Toolbar : public QToolBar 
{
    Q_OBJECT

public:
    explicit Toolbar(QWidget *parent,std::vector<Action> actions);

    /*Setup all actions existing in the buffer*/
    void SetupActions(std::vector<Action> actions);

private:

    /* Creating a new QAction*/
    QAction* _CreateAction(Action &action);
    
    void _AddActionToToolbar(QAction *action, bool addSeparator = false);

    /*Helper function to connect an action to a signal */
    void _ConnectAction(QAction *action, const QString &actionName, void (Action::*handler)());
    
};



/*****************************************/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public :
    explicit MainWindow(QWidget *parent = nullptr);
    void AddMenuBar(QMenuBar* menu);
    void AddToolbar(Toolbar *toolbar);
    /* central widget could be whatever widget*/
    void AddCentralWidget(QWidget *widget);
};

class GraphChart : public QWidget
{
    Q_OBJECT

public:
    explicit GraphChart(QWidget *parent = nullptr);

    void createChart(const QString &title = "Signal Plot");
    QLineSeries* addSeries(const std::vector<double> &x, const std::vector<double> &y, const QString &seriesName = "Series");
    void clearChart();
    void setTitle(const QString &title);
    

    QChart* chart();
    QChartView* chartView();

private:
    QChart *m_chart;
    QChartView *m_chartView;
};

#endif // MAINWINDOWS_HPP
