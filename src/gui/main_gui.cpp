#include "main_gui.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include "MainWindows.hpp"
#include "common.hpp"


void runGui() {
    int argc = 0; // Dummy argument count
    char *argv[] = {nullptr}; // Dummy argument array
    QApplication app(argc, argv);
    
    Action homeAction(QIcon(), "Home", false);
    Action optionsAction(QIcon(), "Options", false);
    Action exitAction(QIcon(), "Exit", false);

    std::vector<Action> actions = { homeAction, optionsAction, exitAction };

    // Create toolbar
    Toolbar* toolbar = new Toolbar(nullptr, actions);

    // Create main window
    MainWindow* mainWindow = new MainWindow();

    // Add toolbar to the left
    mainWindow->addToolBar(Qt::LeftToolBarArea, toolbar);

    // Create chart widget and plot the first signal
    GraphChart* chartWidget = new GraphChart();
    chartWidget->createChart("First Signal");

    /* copy the data to show e.g first signal for now*/
    const SignalData loc_sig = my_sig_data;

    if (!loc_sig.time.empty() && !loc_sig.volts.empty()) {
        chartWidget->addSeries(my_sig_data.time, my_sig_data.volts, "Signal 1");
    }

    mainWindow->setCentralWidget(chartWidget);

    mainWindow->resize(1200, 800); // or any size you prefer
    mainWindow->show();

    app.exec();
}