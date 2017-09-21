#include <QCoreApplication>

#include "base/geometry/cell_2DC.h"
#include "planners/tsp_2opt.h"
#include "maps/bounded_2d_grid.h"
#include "base/geometry/polygon_2dc.h"

#include "base/state_space/cartesian_2D_space.h"

#include "flann.hpp"

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QStringList>

class StateData{
public:

    StateData():
        value(4.5)
    {

    }
    StateData(const StateData &copy)
    {
        this->value = copy.value;
    }

    StateData& operator =(const StateData &rhs)
    {
        this->value = rhs.value;
        return *this;
    }

    double getValue() const {
      this->value;
    }

    void setValue(const double &newValue){
        this->value = newValue;
    }

private:
    double value;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    using namespace mace::state_space;

    Cartesian2DSpaceBounds bounds(-10,10,-10,10);
    Cartesian2DSpace space;
    space.setBounds(bounds);
    Cartesian2DSpace_Sampler sampler(&space);
    mace::pose::CartesianPosition_2D state;
    sampler.sampleUniform(&state);

//    char* MACEPath = getenv("MACE_ROOT");
//    if(MACEPath){
//        std::string rootPath(MACEPath);
//        QFile inputFile(QString::fromStdString(rootPath + "/US48.txt"));
//        if (inputFile.open(QIODevice::ReadOnly))
//        {
//           QTextStream in(&inputFile);
//           while (!in.atEnd())
//           {

//              QString line = in.readLine();
//              QStringList list = line.split(" ");
//              std::string name = "point_" + std::to_string(counter);
//              double x = list.at(1).toDouble();
//              double y = list.at(2).toDouble();
//              mace::pose::Position<mace::pose::CartesianPosition_2D> point(name.c_str(), x, y);
//              vector.push_back(point);
//              counter++;
//           }
//           inputFile.close();
//        }
//    }


    return a.exec();
}
