#include "main_gui.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include "MainWindows.hpp"
#include "common.hpp"
#include <QDockWidget>
#include "Actions.hpp"
#include "SignalsTableWidget.hpp"

void runGui() {
    int argc = 0; // Dummy argument count
    char *argv[] = {nullptr}; // Dummy argument array
    QApplication app(argc, argv);


    // Action homeAction(QIcon(), "Home", false);
    // Action optionsAction(QIcon(), "Options", false);
    // Action exitAction(QIcon(), "Exit", true);

    



    // Create main window
    MainWindow* mainWindow = new MainWindow();
    
    SidebarHomeAction* homeAction = new SidebarHomeAction(mainWindow); // pass your main window pointer
    SideBarFFTAction* fftAction = new SideBarFFTAction(mainWindow); // pass your main window pointer
    std::vector<Action*> actions = { homeAction,fftAction};
        // Create toolbar
    Toolbar* toolbar = new Toolbar(nullptr, actions);


    // Add toolbar to the left
    /*When we use addToolBar and setCentralWidget, Qt automatically sets the parent of 
    toolbar and chartWidget to mainWindow. So we do not need to
    pass mainWindow as the parent in the constructors in this case, 
    because these methods will set the parent for use.*/
    mainWindow->addToolBar(Qt::LeftToolBarArea, toolbar);


    // Create chart widget and plot the first signal
    GraphChart* chartWidget = new GraphChart();
    chartWidget->createChart("First Signal");

    /* copy the data to show e.g first signal for now*/
    const SignalData loc_sig = my_sig_data;

    if (!loc_sig.time.empty() && !loc_sig.volts.empty()) {
        chartWidget->addSeries(my_sig_data.time, my_sig_data.volts, "Signal 1");
    }

    // /* make main Signal Plot to be floatable*/
    // QDockWidget* graphDock = new QDockWidget("Signal Plot", mainWindow);
    // graphDock->setWidget(chartWidget);
    // graphDock->setFloating(false); // Start docked, but user can float it
    // graphDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    // mainWindow->addDockWidget(Qt::RightDockWidgetArea, graphDock);



    mainWindow->resize(1200, 800); // or any size you prefer

    SignalsTableWidget* signalsTable = new SignalsTableWidget(chartWidget,mainWindow);

    MenuBarAddSignal* addsignalAction = new MenuBarAddSignal(mainWindow,chartWidget,signalsTable); // pass your main window pointer
    std::vector<Action*> menubar_actions = {addsignalAction};
    MenuBar* menubar = new MenuBar(menubar_actions);
    /* Add the menuBar */
    mainWindow->setMenuBar(menubar);


QSplitter* splitter = new QSplitter(Qt::Horizontal);
splitter->addWidget(signalsTable);   // Table on the left
splitter->addWidget(chartWidget);    // Chart on the right

// Set initial sizes: table smaller, chart larger
QList<int> sizes;
sizes << 150 << 1000; // Adjust as needed (table width, chart width)
splitter->setSizes(sizes);

mainWindow->setCentralWidget(splitter);
mainWindow->show();

    app.exec();
}