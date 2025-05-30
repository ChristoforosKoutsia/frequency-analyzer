/****************************
 * File: GraphChart.hpp
 * Author: Chris Koutsia
 * Date: 2025-05-27
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Header file of the concrete GraphChart
 ****************************/

#ifndef GRAPHCHART_HPP
#define GRAPHCHART_HPP

/***********Includes Here*****************/
#include "MainWindows.hpp" 
#include "DataLabel.hpp"
/*****************************************/

/***********Class Declaration Here*****************/

class ChartView : public BaseChartView
{
    Q_OBJECT
public:
    explicit ChartView(QChart *chart, QWidget *parent = nullptr);
     void setDataLabel(BaseDataLabel* label) override;
protected:
     void mouseMoveEvent(QMouseEvent *event) override;
     void leaveEvent(QEvent *event) override;
     void wheelEvent(QWheelEvent *event) override;
     void keyPressEvent(QKeyEvent *event) override;
     void mouseDoubleClickEvent(QMouseEvent *event) override ;
     void mousePressEvent(QMouseEvent *event) override ;
private:
    DataLabel* m_dataLabel = nullptr;
    bool m_cursorLocked = false;
};

class GraphChart : public BaseGraphChart
{
    Q_OBJECT
public:
    explicit GraphChart(QWidget *parent = nullptr);

    void createChart(const QString &title = "Signals") override;
    QLineSeries* addSeries(const std::vector<double> &x, 
                           const std::vector<double> &y, 
                           const QString &seriesName = "Series") override;
    void clearChart() override;
    void setTitle(const QString &title) override;
    QChart* chart() override;
    QChartView* chartView() override;
    void setSeriesVisible(const QString& name, bool visible) override;

private:
    QChart *m_chart;
    ChartView *m_chartView;
    DataLabel* m_dataLabel = nullptr;
};




/*****************************************/

#endif // GRAPHCHART_HPP