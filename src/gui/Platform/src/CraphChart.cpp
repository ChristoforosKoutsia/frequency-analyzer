/****************************
 * File: CraphChart.cpp
 * Author: Chris Koutsia
 * Date: 2025-05-27
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Concrete implmentation of GraphChart for Spectral
 * project
 ****************************/

/***********Includes Here*****************/
#include "GraphChart.hpp"
#include "DataLabel.hpp"

/*****************************************/


/***********Classes Here*****************/



ChartView::ChartView(QChart *chart, QWidget *parent)
    : BaseChartView(chart, parent), m_dataLabel(nullptr), m_cursorLocked(false)
{
    setMouseTracking(true);
}

void ChartView::setDataLabel(BaseDataLabel* label)
{
     m_dataLabel = static_cast<DataLabel*>(label);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_cursorLocked && m_dataLabel) {
        QPointF chartPos = chart()->mapToValue(event->pos());
        m_dataLabel->setData(chartPos.x(), chartPos.y());
    }
    QChartView::mouseMoveEvent(event);
}

void ChartView::leaveEvent(QEvent *event)
{
    if (m_dataLabel) m_dataLabel->clearData();
    QChartView::leaveEvent(event);
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    QChartView::wheelEvent(event);
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    QChartView::keyPressEvent(event);
}

void ChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QChartView::mouseDoubleClickEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_cursorLocked = !m_cursorLocked;
        event->accept();
    } 
    else 
    {
        QChartView::mousePressEvent(event);
    }
}


GraphChart::GraphChart(QWidget *parent)
    : BaseGraphChart(parent)
{
    /* A new QChart shall be created. QChart is a class that represent
     the chart itself(data axis etc but will not display anything)*/
    m_chart = new QChart();

    /* QChartView Provides the actual GUI component you can add to your layouts or set as a 
    central widget. Handles rendering, mouse events, zooming, etc.
     Here we have implemented a custom implmentation of QChartView*/
    m_chartView = new ChartView(m_chart, this);
    
    /* always use antialiasing to smooth the edges*/
    m_chartView->setRenderHint(QPainter::Antialiasing);

    /* So we can zoom in the plot*/
    m_chartView->setRubberBand(QChartView::RectangleRubberBand);

    /* Add data label to printout the cursor values*/
    m_dataLabel = new DataLabel(this);
    m_dataLabel->setMinimumHeight(24);

    /* this datalabel property shall be used in the stylesheet for styling*/
    m_dataLabel->setObjectName("dataLabel");    
    /* set the data to chartview*/
    m_chartView->setDataLabel(m_dataLabel);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    layout->addWidget(m_dataLabel, 0, Qt::AlignLeft); // label below, left-aligned

    setLayout(layout);
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



/***********Functions Here*****************/


