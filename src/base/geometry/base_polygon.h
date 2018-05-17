#ifndef BASE_POLYGON_H
#define BASE_POLYGON_H

#include <vector>
#include <stdlib.h>

#include "geometry_helper.h"

#include "base/pose/cartesian_position_2D.h"
#include "base/pose/cartesian_position_3D.h"

namespace mace{
namespace geometry{

template <class T>
class PolygonBase
{
public:
    PolygonBase(const std::string &descriptor = "Polygon"):
        name(descriptor)
    {

    }

    PolygonBase(const std::vector<T> &vector, const std::string &descriptor = "Polygon"):
        name(descriptor)
    {
        //this->clearPolygon(); we should not have to call this case since this is in the constructer
        m_vertex = vector;
        updateBoundingBox();
    }

    PolygonBase(const PolygonBase &copy)
    {
        name = copy.name;
        this->replaceVector(copy.m_vertex);
    }

    void appendVertex(const T &vertex)
    {
        m_vertex.push_back(vertex);
        updateBoundingBox();
    }

    void removeVertex(const int &index);

    void replaceVector(const std::vector<T> &vector)
    {
        this->clearPolygon();
        m_vertex = vector;
        updateBoundingBox();
    }

    void clearPolygon()
    {
        m_vertex.clear();
        m_vertex.shrink_to_fit();
    }

    size_t polygonSize() const
    {
        return m_vertex.size();
    }

    //!
    //! \brief getVector
    //! \return
    //!
    std::vector<T> getVector() const
    {
        return m_vertex;
    }

    T at(const int &index)
    {
        return m_vertex[index];
    }

public:
    virtual T getTopLeft() const = 0;

    virtual T getTopRight() const = 0;

    virtual T getBottomLeft() const = 0;

    virtual T getBottomRight() const = 0;

    virtual void getCorners(T &topLeft, T &bottomRight) const = 0;

public:
    //!
    //! \brief operator ==
    //! \param rhs
    //! \return
    //!
    bool operator == (const PolygonBase &rhs) const
    {
        if(this->name != rhs.name)
        {
            return false;
        }
        if(this->m_vertex.size() != rhs.m_vertex.size())
        {
            return false;
        }

        for(int i = 0; i < this->m_vertex.size(); i++)
        {
            if(m_vertex.at(i) != rhs.m_vertex.at(i))
                return false;
        }
        return true;
    }

    //!
    //! \brief operator !=
    //! \param rhs
    //! \return
    //!
    bool operator != (const PolygonBase &rhs) const {
        return !(*this == rhs);
    }


protected:
    virtual void updateBoundingBox() = 0;

protected:
    std::string name;
    std::vector<T> m_vertex;
};

} //end of namepsace geometry
} //end of namespace mace
#endif // BASE_POLYGON_H
