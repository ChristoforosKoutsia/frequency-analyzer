#include "main_gui.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include "MainWindows.hpp"
#include "common.hpp"
#include <QDockWidget>
#include "Actions.hpp"
#include "Table.hpp"
#include "MenuBar.hpp"
#include "GraphChart.hpp"
#include "CentralWindows.hpp"

void runGui() {
    int argc = 0; // Dummy argument count
    char *argv[] = {nullptr}; // Dummy argument array
    QApplication app(argc, argv);
    //app.setWindowIcon(QIcon("icons/icon-wave.ico"));
    QIcon icon("icons/surfing.svg");

    app.setWindowIcon(icon);
    

    /* apply style to the app*/
    QFile file("styles/modern.qss");
    if (file.open(QFile::ReadOnly)) 
    {
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);
    }

    /* start with creating the main window - this will be the parent of all other widgets*/
    MainWindow* mainWindow = new MainWindow();

    /* create the Charts which will be the main display of MainWindow
     * THere will be eventually two charts, one for the time domain and one
     * for the frequency domain
     */
    GraphChart* chartWidget = new GraphChart(mainWindow);
    chartWidget->createChart("Signals Chart");

    /* create a new chartWidget for the Live plotting section*/
    GraphChart* liveRecordingChart = new GraphChart(mainWindow);
    liveRecordingChart->createChart("Live Recording");

     QToolBar* liveToolbar = new QToolBar("Live Controls", mainWindow);


    GraphChart* fftChartWidget = new GraphChart(mainWindow);   // Frequency-domain (FFT)
    /* the two new graphs will be part of stacked widgets so we can display one every time*/
    QStackedWidget* stackedWidget = new QStackedWidget(mainWindow);
    stackedWidget->addWidget(chartWidget);      // index 0: time domain
    stackedWidget->addWidget(fftChartWidget);   // index 1: frequency domain
    stackedWidget->addWidget(liveRecordingChart);   // index 1: frequency domain

    /* create the table that will display the signals along with their color and a checkbox 
     * so any signal can be displayed at a time*/
    SignalsTableWidget* signalsTable = new SignalsTableWidget(chartWidget,fftChartWidget,mainWindow);

    /* Assing all the Actions. The actions are the options that the user have (the "buttons")
     * each action has a specific handler that will be executed when they are pressed */
    ShowFFTAction* showFFTAction = new ShowFFTAction(stackedWidget,fftChartWidget);
    SidebarHomeAction* homeAction = new SidebarHomeAction(mainWindow); // pass your main window pointer
    SideBarFFTAction* fftAction = new SideBarFFTAction(chartWidget,signalsTable); // pass your main window pointer
    ShowTimeDomainAction* showTimeAction = new ShowTimeDomainAction(stackedWidget);
    ChartView* timeDomainChartView = static_cast<ChartView*>(chartWidget->chartView());
    ShowCursorAction* cursorAction = new ShowCursorAction(timeDomainChartView);
    LiveSectionAction* showlivesection = new LiveSectionAction(stackedWidget,liveToolbar);
    LiveRecordingAction* liveRecordingOption = new LiveRecordingAction(showlivesection,liveRecordingChart);
    ConfigureSessionAction* configSessionAction = new ConfigureSessionAction(mainWindow,liveRecordingOption);
    StopRecordingAction* stopAction = new StopRecordingAction(liveRecordingOption);


    //liveRecordingOption->setEnabled(false);

    /* add more actions here....*/
    std::vector<BaseAction*> actions = { 
                                        fftAction,
                                        showFFTAction,
                                        showTimeAction,
                                        cursorAction,
                                        showlivesection,
                                       };

    ToolBar* toolbar = new ToolBar(mainWindow, actions);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);   
    
   
    liveToolbar->addAction(liveRecordingOption); // Start
    liveToolbar->addAction(stopAction);          // Stop

    liveToolbar->setVisible(false); // Hide by default
    mainWindow->addToolBar(Qt::LeftToolBarArea, liveToolbar);

    // Add toolbar to the left
    /*When we use addToolBar and setCentralWidget, Qt automatically sets the parent of 
    toolbar and chartWidget to mainWindow. We have passed it explicitely
     as parent as the contructor as well*/
    mainWindow->addToolBar(Qt::TopToolBarArea, toolbar);

    /* add the menubar with its actions*/
    MenuBarAddSignal* addsignalAction = new MenuBarAddSignal(mainWindow,chartWidget,signalsTable);
    std::vector<BaseAction*> menubar_actions = {addsignalAction,
                                            configSessionAction
                                        };
    MenuBar* menubar = new MenuBar(menubar_actions);
    /* Add the menuBar */
    mainWindow->setMenuBar(menubar);


QSplitter* splitter = new QSplitter(Qt::Horizontal);
splitter->addWidget(signalsTable);   // Table on the left
splitter->addWidget(stackedWidget);    // Chart on the right

// Set initial sizes: table smaller, chart larger
QList<int> sizes;
sizes << 150 << 1000; // Adjust as needed (table width, chart width)
splitter->setSizes(sizes);

/* resize the Window for better overview*/
mainWindow->resize(1200, 800); // or any size you prefer
mainWindow->setCentralWidget(splitter);
mainWindow->show();

app.exec();
}