#ifndef BASE_2D_POSITION_H
#define BASE_2D_POSITION_H

#include "data/coordinate_frame.h"

namespace DataState {

class Base2DPosition
{

public:
    //!
    //! \brief StateGenericPosition
    //!
    Base2DPosition()
    {
        this->m_CoordinateFrame = Data::CoordinateFrameType::CF_GLOBAL_RELATIVE_ALT;
        this->x = 0.0;
        this->y = 0.0;
        this->posXFlag =false;
        this->posYFlag =false;
    }

    //!
    //! \brief StateGenericPosition
    //! \param coordinateFrame
    //!
    Base2DPosition(const Data::CoordinateFrameType &coordinateFrame)
    {
        this->m_CoordinateFrame = coordinateFrame;
        this->x = 0.0;
        this->y = 0.0;
        this->posXFlag =false;
        this->posYFlag =false;
    }

    //!
    //! \brief StateGenericPosition
    //! \param posX
    //! \param posY
    //! \param posZ
    //!
    Base2DPosition(const double &posX, const double &posY)
    {
        setPositionGeneric(posX,posY);
    }

    //!
    //! \brief StateGenericPosition
    //! \param coordinateFrame
    //! \param posX
    //! \param posY
    //! \param posZ
    //!
    Base2DPosition(const Data::CoordinateFrameType &coordinateFrame, const double &posX, const double &posY)
    {
        this->m_CoordinateFrame = coordinateFrame;
        this->setPositionGeneric(posX,posY);
    }

private:
    //!
    //! \brief setPositionGeneric
    //! \param posX
    //! \param posY
    //! \param posZ
    //!
    void setPositionGeneric(const double &posX, const double &posY)
    {
        this->setX(posX);
        this->setY(posY);
    }

public:
    //!
    //! \brief setX
    //! \param posX
    //!
    void setX(const double &posX)
    {
        this->x = posX;
        this->posXFlag = true;
    }

    //!
    //! \brief setY
    //! \param posY
    //!
    void setY(const double &posY)
    {
        this->y = posY;
        this->posYFlag = true;
    }

    //!
    //! \brief getX
    //! \return
    //!
    double getX() const
    {
        return this->x;
    }

    //!
    //! \brief getY
    //! \return
    //!
    double getY() const
    {
        return this->y;
    }

    //!
    //! \brief setCoordinateFrame
    //! \param coordinateFrame
    //!
    void setCoordinateFrame(const Data::CoordinateFrameType &coordinateFrame){
        this->m_CoordinateFrame = coordinateFrame;
    }

    //!
    //! \brief getCoordinateFrame
    //! \return
    //!
    Data::CoordinateFrameType getCoordinateFrame() const {
        return m_CoordinateFrame;
    }

public:
    //!
    //! \brief operator =
    //! \param rhs
    //!
    void operator = (const Base2DPosition &rhs)
    {
        this->m_CoordinateFrame = rhs.m_CoordinateFrame;
        this->x = rhs.x;
        this->y = rhs.y;
        this->posXFlag = rhs.posXFlag;
        this->posYFlag = rhs.posYFlag;
    }

    //!
    //! \brief operator ==
    //! \param rhs
    //! \return
    //!
    bool operator == (const Base2DPosition &rhs) {
        if(this->m_CoordinateFrame != rhs.m_CoordinateFrame){
            return false;
        }
        if(this->x != rhs.x){
            return false;
        }
        if(this->y != rhs.y){
            return false;
        }
        if(this->posXFlag != rhs.posXFlag){
            return false;
        }
        if(this->posYFlag != rhs.posYFlag){
            return false;
        }
        return true;
    }

    //!
    //! \brief operator !=
    //! \param rhs
    //! \return
    //!
    bool operator != (const Base2DPosition &rhs) {
        return !(*this == rhs);
    }

public:
    //!
    //! \brief getPosXFlag
    //! \return
    //!
    bool getPosXFlag() const
    {
        return this->posXFlag;
    }

    //!
    //! \brief getPosYFlag
    //! \return
    //!
    bool getPosYFlag() const
    {
        return this->posXFlag;
    }


    //!
    //! \brief has2DPositionSet
    //! \return
    //!
    bool has2DPositionSet() const
    {
        return this->posXFlag && this->posYFlag;
    }


protected:
    //!
    //! \brief m_CoordinateFrame
    //!
    Data::CoordinateFrameType m_CoordinateFrame;

    //!
    //! \brief x
    //!
    double x;

    //!
    //! \brief y
    //!
    double y;


    //!
    //! \brief posXFlag
    //!
    bool posXFlag;

    //!
    //! \brief posYFlag
    //!
    bool posYFlag;

};

} //end of namespace DataState

#endif // BASE_2D_POSITION_H
