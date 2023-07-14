#ifndef SIMPLE_MEASUREMENT_H
#define SIMPLE_MEASUREMENT_H

namespace onmodels
{
    template <class R_t, class X_t, class G_t>
    class crosssection_measurement : public wolff::measurement<R_t, X_t, G_t>
    {
    public:
        wolff::system<R_t, X_t, G_t> S;

        crosssection_measurement(const wolff::system<R_t, X_t, G_t> &S) : S(S)
        {
        }

        std::vector<double> get_crossection() const
        {
            unsigned L = S.G.L;
            std::vector<double> result(L * L);
            for (unsigned i = 0; i < L; i++)
            {
                for (unsigned j = 0; j < L; j++)
                {
                    result[i * L + j] = S.s[i * L + j].theta;
                }
            }
            return result;
        }
    };
}

#endif