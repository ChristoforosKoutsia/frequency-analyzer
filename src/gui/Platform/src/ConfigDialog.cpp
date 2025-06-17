/* Just Dialog Box for configure communication session*/
#include "ConfigDialog.hpp"

ConfigDialog::ConfigDialog(QWidget* parent)
    : QDialog(parent)
{
    auto* layout = new QFormLayout(this);

    m_protocolBox = new QComboBox();
    m_protocolBox->addItems({"UART"});
    layout->addRow("Protocol:", m_protocolBox);

    m_baudrateBox = new QComboBox();
    m_baudrateBox->addItems({"9600", "19200", "38400", "57600", "115200"});
    layout->addRow("Baudrate:", m_baudrateBox);

    m_portEdit = new QLineEdit();
    layout->addRow("Port:", m_portEdit);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString ConfigDialog::protocol() const
{
    return m_protocolBox->currentText();
}

QString ConfigDialog::baudrate() const
{
    return m_baudrateBox->currentText();
}

QString ConfigDialog::port() const
{
    return m_portEdit->text();
}