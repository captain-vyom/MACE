#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <mutex>
#include <iostream>
#include <thread>

#include <QSerialPortInfo>
#include <QSerialPort>

#include "serial_configuration.h"

#include "i_link.h"

namespace Comms
{



class SerialLink : public ILink
{
public:

    SerialLink(const SerialConfiguration &config) :
        _config(config)
    {
        m_bytesRead = 0;
        m_port     = NULL;
        m_stopp    = false;
        m_reqReset = false;


        std::cout << "Create SerialLink " << config.portName() << config.baud() << config.flowControl()
                 << config.parity() << config.dataBits() << config.stopBits() << std::endl;
        std::cout <<  "portName: " << config.portName() << std::endl;
    }

    ~SerialLink()
    {
        Disconnect();
        if(m_port) delete m_port;
        m_port = NULL;
    }


    virtual void RequestReset()
    {
        m_stoppMutex.lock();
        m_reqReset = true;
        m_stoppMutex.unlock();
    }

    virtual void WriteBytes(const char *bytes, int length) const
    {
        QByteArray data(bytes, length);
        if(m_port && m_port->isOpen()) {
            //_logOutputDataRate(data.size(), QDateTime::currentMSecsSinceEpoch());
            m_port->write(data);
        } else {
            // Error occured
            _emitLinkError("Could not send data - link " + getName() + " is disconnected!");
        }
    }

    //!
    //! \brief Determine the connection status
    //! \return True if the connection is established, false otherwise
    //!
    virtual bool isConnected() const
    {
        bool isConnected = false;

        if (m_port) {
            isConnected = m_port->isOpen();
        }

        return isConnected;
    }


    std::string getName() const
    {
        return _config.portName();
    }


    //!
    //! \brief Get the maximum connection speed for this interface.
    //! \return The nominal data rate of the interface in bit per second, 0 if unknown
    //!
    virtual u_int64_t getConnectionSpeed() const
    {
        int baudRate;
        if (m_port) {
            baudRate = m_port->baudRate();
        } else {
            baudRate = _config.baud();
        }
        u_int64_t dataRate;
        switch (baudRate)
        {
            case QSerialPort::Baud1200:
                dataRate = 1200;
                break;
            case QSerialPort::Baud2400:
                dataRate = 2400;
                break;
            case QSerialPort::Baud4800:
                dataRate = 4800;
                break;
            case QSerialPort::Baud9600:
                dataRate = 9600;
                break;
            case QSerialPort::Baud19200:
                dataRate = 19200;
                break;
            case QSerialPort::Baud38400:
                dataRate = 38400;
                break;
            case QSerialPort::Baud57600:
                dataRate = 57600;
                break;
            case QSerialPort::Baud115200:
                dataRate = 115200;
                break;
                // Otherwise do nothing.
            default:
                dataRate = -1;
                break;
        }
        return dataRate;
    }



    void _emitLinkError(const std::string& errorMsg) const
    {
        std::string msg = "Error on link " + getName() + ". " + errorMsg;
        EmitEvent([&](const ILinkEvents *ptr){ptr->CommunicationError("Link Error", msg);});
    }

    LinkConfiguration getLinkConfiguration()
    {
        return _config;
    }



    virtual bool Connect(void)
    {
        Disconnect();

        QSerialPort::SerialPortError    error;
        QString                         errorString;

        // Initialize the connection
        if (!_hardwareConnect(error, errorString)) {
            if (_config.isAutoConnect()) {
                // Be careful with spitting out open error related to trying to open a busy port using autoconnect
                if (error == QSerialPort::PermissionError) {
                    // Device already open, ignore and fail connect
                    return false;
                }
            }

            _emitLinkError("Error connecting: Could not create port. " + errorString.toStdString());
            return false;
        }
        return true;
    }

    virtual void Disconnect(void)
    {
        if (m_port) {
            m_port->close();
            delete m_port;
            m_port = NULL;
        }
    }

private:


    /// Performs the actual hardware port connection.
    ///     @param[out] error if failed
    ///     @param[out] error string if failed
    /// @return success/fail
    bool _hardwareConnect(QSerialPort::SerialPortError& error, QString& errorString)
    {
        if (m_port) {
            std::cout << "SerialLink:" << QString::number((long)this, 16).toStdString() << "closing port" << std::endl;
            m_port->close();
            std::this_thread::sleep_for(std::chrono::microseconds(50000));
            delete m_port;
            m_port = NULL;
        }

        std::cout << "SerialLink: hardwareConnect to " << _config.portName() << std::endl;

        // If we are in the Pixhawk bootloader code wait for it to timeout
        if (_isBootloader()) {
            std::cout << "Not connecting to a bootloader, waiting for 2nd chance" << std::endl;
            const unsigned retry_limit = 12;
            unsigned retries;
            for (retries = 0; retries < retry_limit; retries++) {
                if (!_isBootloader()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            // Check limit
            if (retries == retry_limit) {
                // bail out
                std::cerr << "Timeout waiting for something other than booloader" << std::endl;
                return false;
            }
        }

        m_port = new QSerialPort(QString::fromStdString(_config.portName()).trimmed());


        //  port->setCommTimeouts(QSerialPort::CtScheme_NonBlockingRead);

        // TODO This needs a bit of TLC still...


        for (int openRetries = 0; openRetries < 4; openRetries++) {
            if (!m_port->open(QIODevice::ReadWrite)) {
                //std::cout << "Port open failed, retrying" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } else {
                break;
            }
        }

        if (!m_port->isOpen() ) {
            //std::cerr << "open failed" << m_port->errorString().toStdString() << m_port->error() << getName() << _config.isAutoConnect() << std::endl;
            error = m_port->error();
            errorString = m_port->errorString();
            EmitEvent([&](const ILinkEvents *ptr){ptr->CommunicationUpdate(getName(), "Error opening port: " + errorString.toStdString());});
            m_port->close();
            delete m_port;
            m_port = NULL;
            return false; // couldn't open serial port
        }

        std::cout << "Configuring port" << std::endl;

        m_port->setBaudRate     (_config.baud());
        m_port->setDataBits     (static_cast<QSerialPort::DataBits>     (_config.dataBits()));
        m_port->setFlowControl  (static_cast<QSerialPort::FlowControl>  (_config.flowControl()));
        m_port->setStopBits     (static_cast<QSerialPort::StopBits>     (_config.stopBits()));
        m_port->setParity       (static_cast<QSerialPort::Parity>       (_config.parity()));

        EmitEvent([this](const ILinkEvents *ptr){ptr->CommunicationUpdate(getName(), "Opened port!");});
        EmitEvent([](const ILinkEvents *ptr){ptr->Connected();});

        std::cout << "Connection SeriaLink: " << "with settings " << _config.portName() << " "
                 << _config.baud() << " " << _config.dataBits() << " " << _config.parity() << " " << _config.stopBits() << std::endl;

        //start port's event loop
        m_CommsThread = new std::thread([this](){this->PortEventLoop();});

        return true; // successful connection
    }


    bool _isBootloader()
    {
        QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
        if( portList.count() == 0){
            return false;
        }
        std::cout << "Listing available Ports" << std::endl;
        foreach (const QSerialPortInfo &info, portList)
        {
            std::cout << "  PortName    : " << info.portName().toStdString() << std::endl << "  Description : " << info.description().toStdString() << std::endl;
            std::cout << "  Manufacturer: " << info.manufacturer().toStdString() << std::endl;
            if (info.portName().trimmed() == QString::fromStdString(_config.portName()).trimmed() &&
                    (info.description().toLower().contains("bootloader") ||
                     info.description().toLower().contains("px4 bl") ||
                     info.description().toLower().contains("px4 fmu v1.6"))) {
                std::cout << "BOOTLOADER FOUND" << std::endl;
                return true;
           }
        }
        // Not found
        return false;
    }


private:


    void _readBytes(void)
    {
        qint64 byteCount = m_port->bytesAvailable();
        if (byteCount) {
            QByteArray buffer;
            buffer.resize(byteCount);
            m_port->read(buffer.data(), buffer.size());

            std::vector<u_int8_t> vec_buffer = std::vector<u_int8_t>(buffer.begin(), buffer.end());

            EmitEvent([this,&vec_buffer](const ILinkEvents *ptr){ptr->ReceiveData(this, vec_buffer);});
        }
    }

    void linkError(QSerialPort::SerialPortError error)
    {
        switch (error) {
        case QSerialPort::NoError:
            break;
        case QSerialPort::ResourceError:
            EmitEvent([this](const ILinkEvents *ptr){ptr->ConnectionRemoved(this);});
            break;
        default:
            // You can use the following qDebug output as needed during development. Make sure to comment it back out
            // when you are done. The reason for this is that this signal is very noisy. For example if you try to
            // connect to a PixHawk before it is ready to accept the connection it will output a continuous stream
            // of errors until the Pixhawk responds.
            //std::cout << "SerialLink::linkError" << error;
            break;
        }
    }

private:

    void PortEventLoop()
    {
        while(true)
        {
            if(m_port->waitForReadyRead(50))
                this->_readBytes();

            if(m_port->errorString() != "")
            {
                linkError(m_port->error());
            }
        }
    }


private:

    QSerialPort* m_port;
    quint64 m_bytesRead;
    int     m_timeout;
    std::thread *m_CommsThread;
    std::mutex  m_dataMutex;       // Mutex for reading data from _port
    std::mutex  m_writeMutex;      // Mutex for accessing the _transmitBuffer.


    volatile bool        m_stopp;
    volatile bool        m_reqReset;
    std::mutex           m_stoppMutex;      // Mutex for accessing _stopp
    SerialConfiguration _config;
};

} //END MAVLINKComms

#endif // SERIALLINK_H
