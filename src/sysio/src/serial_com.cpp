#include "serial_com.hpp"
#include <memory.h>


SerialCom::SerialCom()
{
    /* this is the default values on startup*/
    m_serial.setPortName("COM3"); // harcoded for now
    m_serial.setBaudRate(QSerialPort::Baud115200);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    m_bufferTimer = new QTimer(this);
}
/* takes as argument a lamda function which does anything with the available data */
void SerialCom::SerialCom_Connect(std::function<void(const std::vector<uint8_t>&)> onDataCallback)
{
    /* just debug staff*/
    qint32 tmp_val = m_serial.baudRate();
    QString tmp_str = m_serial.portName();
    m_onDataCallback = std::move(onDataCallback);
    if (m_serial.open(QIODevice::ReadWrite)) 
    {
        /* this will implicitely called when new data will arrive*/
        /* We can try the following method. Stack data on a circular buffer
         and call callback function when there are no data for 5ms(configurable) */
        QObject::connect(&m_serial, &QSerialPort::readyRead, [&]() 
        {
             m_serialBuffer = m_serial.readAll();
             m_bufferTimer->start(5);

        });
        
        QObject::connect(m_bufferTimer, &QTimer::timeout, [&]() 
        {
        // Called when no new data came for 5ms
        /*decode first*/
        QList<SerialDataPacket> loc_packets = SerialCom_decodeMsg(m_serialBuffer);

        /* just single test with one packet (lets pretend is just uint8)
        */
        if (!loc_packets.isEmpty()) 
        {
            std::vector<uint8_t> loc_data = loc_packets[0].data;
            
            if (m_onDataCallback) {
                m_onDataCallback(loc_data);
            }
        }

        m_serialBuffer.clear();
        });
    } 
    else 
    {
        /* do nothing for now*/
    }
}

void SerialCom::SerialComSetCOMConfig(SerialComConfig& config)
{
    m_serial.setPortName(config.port_name); // harcoded for now
    m_serial.setBaudRate(config.baud_rate);
}


/* Eventually we need a decode function for the incoming messages.
 * This function should run on an event basis (e.g as soon as message received)
 * Will be the callback function entry*/
QList<SerialDataPacket> SerialCom_decodeMsg(QByteArray& rawData)
{
    QList<SerialDataPacket> packets;

    if (!rawData.isEmpty()) {
        SerialDataPacket decoded;
        decoded.type = 0; // or set as needed
        decoded.data = std::vector<uint8_t>(rawData.begin(), rawData.end());
        decoded.crc = 0; // not used
        packets.append(decoded);
        rawData.clear(); // clear buffer after processing
    }

    return packets;
}


void SerialCom_ProcessDecodedMsg()
{
    /*takes decoded msg, validate it, and extract data based on the type*/
}
