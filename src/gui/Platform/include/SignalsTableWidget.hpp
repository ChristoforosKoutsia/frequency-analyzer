#pragma once
#include "MainWindows.hpp"
#include <QColor>
#include <QString>

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