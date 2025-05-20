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

MenuBar::MenuBar(const std::vector<Action*>& actions,QWidget *parent) 
    : QMenuBar(parent)
    {
      SetupActions(actions);  
    }
void MenuBar::_AddActionToMenuBar(QAction *action, bool addSeparator) 
{
    addAction(action);
    if (addSeparator) 
    {
        this->addSeparator(); // Add a separator if requested
    }
}

QAction* MenuBar::_CreateAction(Action &action)
{
     // Create a new QAction with the icon and text from the Action object
     QAction *qAction = new QAction(action.GetQIcon(), action.GetQstring(), this);

    // Set the tooltip to the action's text
    qAction->setToolTip(action.GetQstring());

    // Connect the QAction's triggered signal to the Action's handler
    connect(qAction, &QAction::triggered, [&action]() {
        action.handler(); // Call the handler method of the Action object
    });

    return qAction;
}

void MenuBar::SetupActions(const std::vector<Action*>& actions) {
    for (auto action : actions) 
    {
        // Create a QAction from the Action object
        QAction *qAction = _CreateAction(*action);

        // Add the QAction to the toolbar
        _AddActionToMenuBar(qAction, action->AddSeparator());
    }
}

/* Toolbar implementation*/
Toolbar::Toolbar(QWidget *parent, const std::vector<Action*>& actions) 
    : QToolBar(parent)
    {
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

    // Set the tooltip to the action's text
    qAction->setToolTip(action.GetQstring());

    // Connect the QAction's triggered signal to the Action's handler
    connect(qAction, &QAction::triggered, [&action]() {
        action.handler(); // Call the handler method of the Action object
    });

    return qAction;
}

void Toolbar::SetupActions(const std::vector<Action*>& actions) {
    for (auto action : actions) 
    {
        // Create a QAction from the Action object
        QAction *qAction = _CreateAction(*action);

        // Add the QAction to the toolbar
        _AddActionToToolbar(qAction, action->AddSeparator());
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

void MainWindow::AddMenuBar(MenuBar *menu)
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
    /* A new QChart shall be created. QChart is a class that represent
     the chart itself(data axis etc but will not display anything)*/
    m_chart = new QChart();

    /* QChartView Provides the actual GUI component you can add to your layouts or set as a 
    central widget. Handles rendering, mouse events, zooming, etc.*/
    m_chartView = new ChartView(m_chart, this);
    
    /* always use antialiasing to smooth the edges*/
    m_chartView->setRenderHint(QPainter::Antialiasing);

    /* So we can zoom in the plot*/
    m_chartView->setRubberBand(QChartView::RectangleRubberBand);

    /* Add data label to printout the cursor values*/
    m_dataLabel = new DataLabel(this);
    m_dataLabel->setMinimumHeight(24);
    m_dataLabel->setStyleSheet("background: white; border: 1px solid gray; padding: 2px;");
    
    /* set the data to chartview*/
    m_chartView->setDataLabel(m_dataLabel);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    layout->addWidget(m_dataLabel, 0, Qt::AlignLeft); // label below, left-aligned

    setLayout(layout);

   // m_chart->setTitle("Signal Plot");
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

    /* This feature is to adjust the line width*/
    QPen pen = series->pen();
    pen.setWidthF(1.0); // Thinner line
    series->setPen(pen);    
    
    // Assign a unique color based on the number of series
    static const QVector<QColor> colors = {
        Qt::red, Qt::blue, Qt::green, Qt::magenta, Qt::cyan, Qt::darkYellow, Qt::darkRed, Qt::darkBlue
    };
    int colorIdx = m_chart->series().size() % colors.size();
    series->setColor(colors[colorIdx]);


    /* this is to make all the real points(data) visible*/
    series->setPointsVisible(true);
    m_chart->addSeries(series);
    m_chart->createDefaultAxes();
    /*hide the legend*/
    m_chart->legend()->hide();
    
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

/* making a plot visible/invisible*/
void GraphChart::setSeriesVisible(const QString& name, bool visible)
{
    for (auto* s : m_chart->series()) {
        if (s->name() == name) {
            s->setVisible(visible);
            break;
        }
    }
}


// ChartView constructor: sets up mouse tracking and the vertical line
ChartView::ChartView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent)
{
    setMouseTracking(true); // Enable mouse move events without clicking
    m_vLine = new QGraphicsLineItem();
    m_vLine->setPen(QPen(Qt::DashLine));
    chart->scene()->addItem(m_vLine);
    m_vLine->hide();
}

// Handles mouse movement over the chart view
void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_cursorLocked) 
    {
        // Use the locked position, not the current mouse position
        QMouseEvent lockedEvent(event->type(), m_lockedPos, event->button(), event->buttons(), event->modifiers());
        updateCursorAndLabel(&lockedEvent);
        return;
    }
    updateCursorAndLabel(event);
    QChartView::mouseMoveEvent(event);
}


// Handles the mouse leaving the chart view area
void ChartView::leaveEvent(QEvent *event)
{
    m_vLine->hide(); // Hide the vertical line
    QToolTip::hideText(); // Hide the tooltip
    QChartView::leaveEvent(event);
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    // Zoom in if scrolling up, zoom out if scrolling down
    if (event->angleDelta().y() > 0)
        chart()->zoomIn();
    else
        chart()->zoomOut();

    event->accept();
}

/* on F keypress Event the zoom is Reset*/
void ChartView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F) {
        chart()->zoomReset(); // Fit the plot
        event->accept();
    } else {
        QChartView::keyPressEvent(event); // Default handling for other keys
    }
}

/* added double click and right click functionality
 On doouble click the cursor is locked to a specific point
 On single click the cursor is movable again*/

 void ChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_cursorLocked = true;
        m_lockedPos = event->pos();
        // Optionally update the cursor immediately
        mouseMoveEvent(event);
        event->accept();
    } else {
        QChartView::mouseDoubleClickEvent(event);
    }
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_cursorLocked) {
        m_cursorLocked = false;
        event->accept();
    } else {
        QChartView::mousePressEvent(event);
    }
}

// Helper function to avoid code duplication
void ChartView::updateCursorAndLabel(QMouseEvent *event)
{
    QPointF chartPos = chart()->mapToValue(event->pos());
    QRectF plotArea = chart()->plotArea();

    m_vLine->setLine(event->pos().x(), plotArea.top(), event->pos().x(), plotArea.bottom());
    m_vLine->show();

    if (!chart()->series().isEmpty()) {
        auto *series = qobject_cast<QLineSeries*>(chart()->series().first());
        if (series) {
            qreal minDist = std::numeric_limits<qreal>::max();
            QPointF nearest;
            for (const QPointF &pt : series->points()) {
                qreal dist = std::abs(pt.x() - chartPos.x());
                if (dist < minDist) {
                    minDist = dist;
                    nearest = pt;
                }
            }
            if (m_dataLabel) {
                m_dataLabel->setData(nearest.x(), nearest.y());
            }
        }
    }
}

TableWidget::TableWidget(
    int colCount,
    std::string headerLabels,
    bool inDockWidget,
    QWidget *parent
) : QTableWidget(0, colCount, parent)
{
    // Set headers from comma-separated string
    QStringList headers = QString::fromStdString(headerLabels).split(',', Qt::SkipEmptyParts);
    setHorizontalHeaderLabels(headers);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setVisible(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Optionally, you can add this widget to a dock widget outside this class
    // if (inDockWidget) { ... }
}

void TableWidget::AddRow()
{
    int row = rowCount();
    insertRow(row);
}

/***********Functions Here*****************/


