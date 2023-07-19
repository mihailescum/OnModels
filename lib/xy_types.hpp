#ifndef ONMODELS_XY_TYPES_H
#define ONMODELS_XY_TYPES_H

#include <wolff.hpp>

#define MATH_2PI 6.28318530718

namespace onmodels
{
    class spin_xy
    {
    public:
        double theta;

        spin_xy() : theta(0){};
        spin_xy(double theta) : theta(theta){};
    };

    class transformation_xy
    {
    public:
        double angle;

        transformation_xy() : angle(0){};
        transformation_xy(double angle) : angle(angle){};

        spin_xy act(const spin_xy &x) const
        {
            double theta = x.theta + angle;
            if (theta > MATH_2PI)
                theta -= MATH_2PI;
            return spin_xy(theta);
        }

        spin_xy act_inverse(const spin_xy &x) const
        {
            double theta = x.theta - angle;
            if (theta <= 0)
                theta += MATH_2PI;
            return spin_xy(theta);
        }

        transformation_xy act(const transformation_xy &t) const
        {
            double angle_new = t.angle + angle;
            if (angle_new > MATH_2PI)
                angle_new -= MATH_2PI;
            return transformation_xy(angle_new);
        }

        transformation_xy act_inverse(const transformation_xy &t) const
        {
            double angle_new = t.angle - angle;
            if (angle_new <= 0)
                angle_new += MATH_2PI;
            return transformation_xy(angle_new);
        }
    };

    template <class G_t>
    transformation_xy generate_uniform_rotation(std::mt19937 &r, const wolff::system<transformation_xy, spin_xy, G_t> &, const typename G_t::vertex &)
    {
        std::uniform_real_distribution<double> dist(0, MATH_2PI);
        double angle = dist(r);
        return transformation_xy(angle);
    };
}

#endif