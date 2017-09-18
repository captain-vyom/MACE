#ifndef CARTESIAN_POSITION_2D_H
#define CARTESIAN_POSITION_2D_H

#include "base_position.h"
#include "state_space/state_space.h"

namespace mace{
namespace pose {

class CartesianPosition_2D : public AbstractPosition<CartesianPosition_2D, misc::Data2D>, public CartesianPosition
{
public:
    CartesianPosition_2D():
        AbstractPosition(AbstractPosition::PositionType::CARTESIAN, CoordinateFrame::CF_LOCAL_ENU)
    {

    }

    ~CartesianPosition_2D() = default;

    CartesianPosition_2D(const CartesianPosition_2D &copy):
        AbstractPosition(copy)
    {

    }

    CartesianPosition_2D(const double x, const double &y):
        AbstractPosition(AbstractPosition::PositionType::CARTESIAN, CoordinateFrame::CF_LOCAL_ENU)
    {
        this->data.setData(x,y);
    }

public:
    void updatePosition(const double &x, const double &y)
    {
        this->data.setData(x,y);
    }

    void setXPosition(const double &x)
    {
        this->data.setX(x);
    }

    void setYPosition(const double &y)
    {
        this->data.setY(y);
    }

    double getXPosition() const
    {
        return this->data.getX();
    }

    double getYPosition() const
    {
        return this->data.getY();
    }

    Eigen::Vector2d getAsVector()
    {
        Eigen::Vector2d vec(this->data.getX(), this->data.getY());
        return vec;
    }

    bool hasXBeenSet() const
    {
        return this->data.getDataXFlag();
    }

    bool hasYBeenSet() const
    {
        return this->data.getDataXFlag();
    }
public:
    double deltaX(const CartesianPosition_2D &that) const;
    double deltaY(const CartesianPosition_2D &that) const;
public:
    void setCoordinateFrame(const LocalFrameType &desiredFrame)
    {
        this->frame = mace::pose::getCoordinateFrame(desiredFrame);
    }

    /** Arithmetic Operators */
public:

    //!
    //! \brief operator +
    //! \param that
    //! \return
    //!
    CartesianPosition_2D operator + (const CartesianPosition_2D &that) const
    {
        CartesianPosition_2D newPoint(*this);
        newPoint.data = this->data + that.data;
        return newPoint;
    }

    //!
    //! \brief operator -
    //! \param that
    //! \return
    //!
    CartesianPosition_2D operator - (const CartesianPosition_2D &that) const
    {
        CartesianPosition_2D newPoint(*this);
        newPoint.data = this->data - that.data;
        return newPoint;
    }


public:
    //!
    //! \brief distanceBetween2D
    //! \param position
    //! \return
    //!
    double distanceBetween2D(const CartesianPosition_2D &pos) const override;

    //!
    //! \brief distanceTo
    //! \param position
    //! \return
    //!
    double distanceTo(const CartesianPosition_2D &pos) const override;

    //!
    //! \brief polarBearingTo
    //! \param position
    //! \return
    //!
    double polarBearingTo(const CartesianPosition_2D &pos) const override;

    //!
    //! \brief polarBearingTo
    //! \param position
    //! \return
    //!
    double compassBearingTo(const CartesianPosition_2D &pos) const override;

    //!
    //! \brief newPositionFromPolar
    //! \param distance
    //! \param compassBearing
    //! \return
    //!
    CartesianPosition_2D newPositionFromPolar(const double &distance, const double &bearing) const override;

    //!
    //! \brief newPositionFromPolar
    //! \param distance
    //! \param compassBearing
    //! \return
    //!
    CartesianPosition_2D newPositionFromCompass(const double &distance, const double &bearing) const override;

public:
    friend std::ostream& operator<<(std::ostream& os, const CartesianPosition_2D& t);

};

} //end of namespace pose
} //end of namespace mace

#endif // CARTESIAN_POSITION_2D_H
