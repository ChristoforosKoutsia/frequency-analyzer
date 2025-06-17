#ifndef CONFIG_DIALOG_HPP
#define CONFIG_DIALOG_HPP

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget* parent = nullptr);

    QString protocol() const;
    QString baudrate() const;
    QString port() const;

private:
    QComboBox* m_protocolBox;
    QComboBox* m_baudrateBox;
    QLineEdit* m_portEdit;
};

#endif