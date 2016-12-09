#include "module_ground_station.h"

#include <iostream>
#include <functional>

#include <QApplication>
#include <QString>
#include <QDataStream>


#include "mace_core/module_factory.h"

class ServerThread : public QThread
{
public:
    ServerThread(const std::function<void(void)> &func):
        m_func(func)
    {
        if(QCoreApplication::instance() == NULL)
        {
            int argc = 0;
            char * argv[] = {(char *)"sharedlib.app"};
            pApp = new QCoreApplication(argc, argv);
        }
    }

    virtual void run()
    {
        while(true)
        {
            QCoreApplication::processEvents();
            m_func();
        }
    }

private:

    std::function<void(void)> m_func;
    QCoreApplication *pApp;
};



ModuleGroundStation::ModuleGroundStation() :
    MaceCore::IModuleCommandGroundStation(),
    m_ListenThread(NULL),
    m_TcpServer(NULL)
{


}

ModuleGroundStation::~ModuleGroundStation()
{
    if(m_ListenThread != NULL)
    {
        delete m_ListenThread;
    }
}


void ModuleGroundStation::on_newConnection()
{
    std::cout << "New connection..." << std::endl;

    while (m_TcpServer->hasPendingConnections())
    {
        QTcpSocket *socket = m_TcpServer->nextPendingConnection();
        while (socket->waitForReadyRead())
        {
            QByteArray data = socket->readAll();
            QByteArray returnData;

            std::cout << "Incoming data: " << data.toStdString() << std::endl;

            QJsonObject jsonObj;
            QJsonDocument doc = QJsonDocument::fromJson(data);

            // check validity of the document
            if(!doc.isNull())
            {
                if(doc.isObject())
                {
                    jsonObj = doc.object();
                    parseTCPRequest(jsonObj, returnData);
                }
                else
                {
                    std::cout << "Command is not a valid JSON object." << std::endl;
                    return;
                }
            }
            else
            {
                std::cout << "Invalid JSON..." << std::endl;
                return;
            }

            socket->write(returnData);
            socket->flush();
            socket->waitForBytesWritten(3000);
        }

        socket->close();
    }
}


void ModuleGroundStation::parseTCPRequest(QJsonObject jsonObj, QByteArray &returnData)
{
    QString command = jsonObj["command"].toString();
    int vehicleID = jsonObj["vehicleID"].toInt();
    QByteArray data;
    if(command == "GET_POSITION")
    {
        std::cout << "TCP: Get vehicle position" << std::endl;
        std::string vehiclePosition;
//        getVehiclePosition(vehicleID, data);
    }
    else if (command == "GET_ATTITUDE")
    {
        std::cout << "TCP: Get vehicle attitude" << std::endl;
        std::string vehicleAttitude;
        getVehicleAttitude(vehicleID, data);
    }
    else
    {
        std::cout << "Command " << command.toStdString() << " not recognized." << std::endl;
        data = "command_not_recognized";
        return;
    }

    returnData = data;
}


//!
//! \brief Describes the strucure of the parameters for this module
//! \return Strucure
//!
std::shared_ptr<MaceCore::ModuleParameterStructure> ModuleGroundStation::ModuleConfigurationStructure() const
{
    MaceCore::ModuleParameterStructure structure;
    return std::make_shared<MaceCore::ModuleParameterStructure>(structure);
}


//!
//! \brief Provides object contains parameters values to configure module with
//! \param params Parameters to configure
//!
void ModuleGroundStation::ConfigureModule(const std::shared_ptr<MaceCore::ModuleParameterValue> &params)
{

}

bool ModuleGroundStation::StartTCPServer()
{
    m_TcpServer = new QTcpServer();
    m_ListenThread = new ServerThread([&](){
        if(m_TcpServer->hasPendingConnections())
            this->on_newConnection();
    });

//    while (!m_TcpServer->isListening())
//    {
        m_TcpServer->listen(QHostAddress::LocalHost, 1234);
//    }

    m_TcpServer->moveToThread(m_ListenThread);
    m_ListenThread->start();


    if(!m_TcpServer->isListening())
    {
        std::cout << "Server could not start..." << std::endl;
    }
    else
    {
        std::cout << "Server started" << std::endl;
    }

    return m_TcpServer->isListening();
}

void ModuleGroundStation::UpdatedVehicleMap(const std::string &vehicleID)
{
    //This is a sample of how to get data from the map containing vehicle information
    std::shared_ptr<const MaceCore::MaceData> data = this->getDataObject();
//    std::map<int, std::shared_ptr<VehicleObject>> vehicleDataMap;
    data->GetVehicleMap(m_VehicleMap);
}


void ModuleGroundStation::UpdatedVehicleLife(const std::string &vehicleID)
{
    UpdatedVehicleMap(vehicleID);
}


void ModuleGroundStation::UpdatedPositionDynamics(const std::string &vehicleID)
{
    UpdatedVehicleMap(vehicleID);
}


void ModuleGroundStation::UpdateAttitudeDynamics(const std::string &vehicleID)
{
    UpdatedVehicleMap(vehicleID);
}


void ModuleGroundStation::getVehiclePosition(const int &vehicleID, QByteArray &vehiclePosition)
{
    Eigen::Vector3d positionVector(10.0,10.0,10.0);
    if(m_VehicleMap.find(vehicleID) == m_VehicleMap.cend())
    {
        std::cout << "The vehicle with that ID is not there." << std::endl;
    }else{        
        m_VehicleMap.at(vehicleID)->getVehiclePosition(positionVector);

        QJsonObject json;
        json["x"] = positionVector(0);
        json["y"] = positionVector(1);
        json["z"] = positionVector(2);

        QJsonDocument doc(json);
        vehiclePosition = doc.toJson();
    }
}

void ModuleGroundStation::getVehicleAttitude(const int &vehicleID, QByteArray &vehicleAttitude)
{
    Eigen::Vector3d attitudeVector(10.0,10.0,10.0);
    if(m_VehicleMap.find(vehicleID) == m_VehicleMap.cend())
    {
        std::cout << "The vehicle with that ID is not there." << std::endl;
    }else{
        m_VehicleMap.at(vehicleID)->getVehicleAttitude(attitudeVector);

        QJsonObject json;
        json["roll"] = attitudeVector(0);
        json["pitch"] = attitudeVector(1);
        json["yaw"] = attitudeVector(2);

        QJsonDocument doc(json);
        vehicleAttitude = doc.toJson();
    }
}