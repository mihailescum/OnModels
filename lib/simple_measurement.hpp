#ifndef SIMPLE_MEASUREMENT_H
#define SIMPLE_MEASUREMENT_H

namespace onmodels
{
    template <class R_t, class X_t, class G_t>
    class simple_measurement : public wolff::measurement<R_t, X_t, G_t>
    {
    public:
        simple_measurement(const wolff::system<R_t, X_t, G_t> &S)
        {
        }
    };
}

#endif