/****************************
 * File: Table.cpp
 * Author: Chris Koutsia
 * Date: 2025-05-28
 * Licensed under the MIT License. See LICENSE file for details.
 * Description: Description of the file
 ****************************/

/***********Includes Here*****************/
#include "Table.hpp"
#include <QCheckBox>
#include <QTableWidgetItem>

/*****************************************/


/***********Classes Here*****************/
TableWidget::TableWidget(
    int colCount,
    std::string headerLabels,
    bool inDockWidget,
    QWidget *parent
) : BaseTableWidget(colCount, parent)
{
    // Set headers from comma-separated string
    QStringList headers = QString::fromStdString(headerLabels).split(',', Qt::SkipEmptyParts);
    setHorizontalHeaderLabels(headers);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setVisible(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void TableWidget::AddRow()
{
    int row = rowCount();
    insertRow(row);
}


SignalsTableWidget::SignalsTableWidget(GraphChart* graph,GraphChart* fft_graph,QWidget* parent)
    : TableWidget(3, "Color,Signal,Show", true, parent),
      m_graphchart(graph),
      m_fftGraphChart(fft_graph)
{
    // Hide the horizontal header
    horizontalHeader()->setVisible(false);

// Set color column to fixed and small
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    horizontalHeader()->setMinimumSectionSize(0); // Allow zero minimum

    setColumnWidth(0,14); // Try 16-24 for a small color box
    
    // Let the other columns stretch
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}

void SignalsTableWidget::AddRow(const QColor& color, const QString& name, bool checked)
{
    int row = rowCount();
    insertRow(row);
    // Set the row height (e.g., 18 pixels)
    setRowHeight(row, 18);

    // Color cell -> We want this cell to be small enough
    QTableWidgetItem* colorItem = new QTableWidgetItem();
    colorItem->setBackground(color);
    setItem(row, 0, colorItem);

    // Name cell
    setItem(row, 1, new QTableWidgetItem(name));

    // Checkbox cell
    QCheckBox* cb = new QCheckBox();
    cb->setChecked(checked);
    setCellWidget(row, 2, cb);

    // Enforce small color column again
    setColumnWidth(0, 14);

    /*lambda function slot*/
QObject::connect(cb, &QCheckBox::toggled, this, [this, name](bool checked)
{
    /* the checkbox shall control visibility for both time and frequency domain*/
    // Set visibility in time domain
    if (m_graphchart) 
    {

        for (auto* s : m_graphchart->chart()->series()) 
        {
            if (s->name() == name) s->setVisible(checked);
        }
    }
    // Set visibility in frequency domain
    if (m_fftGraphChart) 
    {
        for (auto* s : m_fftGraphChart->chart()->series()) 
        {
            if (s->name() == name) s->setVisible(checked);
        }
    }
});

}

/***********Functions Here*****************/


