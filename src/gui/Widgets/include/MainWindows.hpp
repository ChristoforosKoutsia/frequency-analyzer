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
#include <QGraphicsLineItem>
#include <QToolTip>
#include <vector>


/*****************************************/

/***********Macros Here*****************/


/*****************************************/

/***********Class Declaration Here*****************/

/* QLabel declaration*/

class DataLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DataLabel(QWidget *parent = nullptr)
        : QLabel(parent) {}

    // Set the data to display (default: x/y)
    void setData(double x, double y) {
        setText(QString("X: %1\nY: %2").arg(x).arg(y));
    }

    // Set custom text
    void setData(const QString& text) {
        setText(text);
    }

    // Clear the label
    void clearData() {
        setText("");
    }
};


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


/* class to create features on the plot like mouse Events 
 and cursor feature to capture data*/
class ChartView : public QChartView
{
    Q_OBJECT

public:
    explicit ChartView(QChart *chart, QWidget *parent = nullptr);
    void setDataLabel(DataLabel* label) { m_dataLabel = label; }

protected:
   /* This methods are overriden in order to develop the cursor feature*/
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

    /* We will override wheelEvent to implement zoom in/out with the scrollbar*/
    void wheelEvent(QWheelEvent *event) override;

    /* we can add some callback functions on keypressEvent*/
    void keyPressEvent(QKeyEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    QGraphicsLineItem *m_vLine = nullptr;

    /* create data label at the bottom to track the data value*/
    DataLabel* m_dataLabel = nullptr;

    /* lock functionality so the cursor can be locked to a specific point*/
    bool m_cursorLocked = false;
    QPoint m_lockedPos;

    void updateCursorAndLabel(QMouseEvent *event);


};



class GraphChart : public QWidget
{
    Q_OBJECT

public:
    explicit GraphChart(QWidget *parent = nullptr);

    void createChart(const QString &title = "Signals");
    QLineSeries* addSeries(const std::vector<double> &x, const std::vector<double> &y, const QString &seriesName = "Series");
    void clearChart();
    void setTitle(const QString &title);
    

    QChart* chart();
    QChartView* chartView();

private:
    QChart *m_chart;
    ChartView *m_chartView;
    DataLabel *m_dataLabel;

    
};











#endif // MAINWINDOWS_HPP
