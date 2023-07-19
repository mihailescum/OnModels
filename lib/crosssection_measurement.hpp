#ifndef ONMODELS_CROSSECTION_MEASUREMENT_H
#define ONMODELS_CROSSECTION_MEASUREMENT_H

#include "event_measurement.hpp"

namespace onmodels
{
    template <class R_t, class X_t, class G_t>
    class crosssection_measurement : public event_measurement<R_t, X_t, G_t>
    {
    public:
        const wolff::system<R_t, X_t, G_t> &S;

        crosssection_measurement(const wolff::system<R_t, X_t, G_t> &S) : S(S)
        {
        }

        std::vector<double> get_crossection() const
        {
            unsigned L = S.G.L;
            std::vector<double> result(L * L);
            for (unsigned x = 0; x < L; x++)
            {
                for (unsigned y = 0; y < L; y++)
                {
                    unsigned i = x + y * L;
                    result[i] = S.s[i].theta;
                }
            }
            return result;
        }
    };
}

#endif