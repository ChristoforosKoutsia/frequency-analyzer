#include "SignalsTableWidget.hpp"
#include <QCheckBox>
#include <QTableWidgetItem>

SignalsTableWidget::SignalsTableWidget(GraphChart* graph,QWidget* parent)
    : TableWidget(3, "Color,Signal,Show", true, parent),
      m_graphchart(graph)
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
QObject::connect(cb, &QCheckBox::toggled, this, [graphchart = m_graphchart, name](bool checked){
    graphchart->setSeriesVisible(name, checked);
});
}