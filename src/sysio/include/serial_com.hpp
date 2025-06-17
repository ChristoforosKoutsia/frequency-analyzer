/* Driver for Serial Uart Communication an Input from the user 
 * for the serial port etc shall be done here. The communication is 
 * done through QSerial crossplatform module */

#include <QSerialPort>
#include <QSerialPortInfo>
#include <functional>
#include <QByteArray>
#include <qtimer.h>
struct SerialComConfig
{
    QString port_name;
    QSerialPort::BaudRate baud_rate;
};

/**
 * @brief Serial Com class implements the communication between external
 * Device and host device through the serial port. The user shall indicate 
 * the details of the UART communication such as baudrate , parity bit etc
 * otherwise default values might be used
*/
class SerialCom : public QObject 
{
    Q_OBJECT
public:
    /* class to implement the UART serial communication*/
    SerialCom(); /* this interface apparently will change since we */
    void SerialCom_Connect(std::function<void(const std::vector<uint8_t>&)> onDataCallback);
    void SerialComSetCOMConfig(SerialComConfig& config);
private :
        QSerialPort::BaudRate m_baudrate;
        QSerialPort::Parity m_parity;
        QSerialPort::StopBits m_stop_bits;
        QSerialPort::FlowControl m_flow_control;
        QSerialPort m_serial;
        QByteArray m_serialBuffer;
        QTimer* m_bufferTimer;
        std::function<void(const std::vector<uint8_t>&)> m_onDataCallback;
};


/*Start Byte 
| Packet Type 
| Payload Length 
| Data | CRC | End Byte*/

/* based on the packet type we know how to cast the bytes*/
struct SerialDataPacket
{
    uint8_t startByte;
    uint8_t sig_id; /* this id will be mapped in json file so the correct sig name is printed*/
    uint8_t type;
    uint8_t payloadLength;
    std::vector<uint8_t> data;
    uint8_t crc;
    uint8_t endByte;
};

/* this shall be probably be part of the class (?)*/
QList<SerialDataPacket> SerialCom_decodeMsg(QByteArray& rawData);


