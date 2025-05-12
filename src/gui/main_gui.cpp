#include "main_gui.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QLabel>

void runGui() {
    int argc = 0; // Dummy argument count
    char *argv[] = {nullptr}; // Dummy argument array
    QApplication app(argc, argv);
    
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Simple Qt Application");

    QLabel label("Hello, Qt!");
    label.setAlignment(Qt::AlignCenter);
    mainWindow.setCentralWidget(&label);

    mainWindow.resize(400, 300);
    mainWindow.show();

    app.exec();
}