#ifndef XY_TYPES_H
#define XY_TYPES_H

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
        transformation_xy(){};

        spin_xy act(const spin_xy &x) const
        {
            return spin_xy();
        }

        spin_xy act_inverse(const spin_xy &x) const
        {
            return spin_xy();
        }

        transformation_xy act(const transformation_xy &t) const
        {
            return transformation_xy();
        }

        transformation_xy act_inverse(const transformation_xy &t) const
        {
            return transformation_xy();
        }
    };
}

#endif