/****************************
 * File: Table.hpp
 * Author: Chris Koutsia
 * Date: 2025-05-28
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Description of the file
 ****************************/

#ifndef TABLE_HPP
#define TABLE_HPP

/***********Includes Here*****************/
#include "MainWindows.hpp"
#include "GraphChart.hpp"


/*****************************************/

/***********Class Declaration Here*****************/
class TableWidget : public BaseTableWidget
{
    Q_OBJECT
public:
    explicit TableWidget(
        int colCount,
        std::string headerLabels,
        bool inDockWIdget = false,
        QWidget *parent = nullptr
    );
     void AddRow() override;
};

class SignalsTableWidget : public TableWidget
{
    Q_OBJECT
public:
    explicit SignalsTableWidget(GraphChart* graph,GraphChart* fft_graph,QWidget* parent = nullptr);
    void AddRow(const QColor& color, const QString& name, bool checked = true);

private:
    /* pass a pointer to a custom implementation graph chart
     so we can connect tickbox with the plot visibility*/
    GraphChart* m_graphchart;
    GraphChart* m_fftGraphChart;
};
/*****************************************/

#endif // TABLE_HPP