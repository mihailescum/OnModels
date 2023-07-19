#ifndef ONMODELS_EVENT_MEASUREMENT
#define ONMODELS_EVENT_MEASUREMENT

#include <wolff.hpp>

namespace onmodels
{
    template <typename... Types>
    struct Delegate
    {
        std::vector<std::function<void(Types...)>> funcs;
        template <class T>
        Delegate &operator+=(T mFunc)
        {
            funcs.push_back(mFunc);
            return *this;
        }
        void operator()(Types... args)
        {
            for (auto &f : funcs)
                f(args...);
        }
    };

    template <class R_t, class X_t, class G_t>
    class event_measurement : public wolff::measurement<R_t, X_t, G_t>
    {
    public:
        void add_pre_cluster_listener(std::function<void(unsigned, unsigned, const wolff::system<R_t, X_t, G_t> &, const typename G_t::vertex &, const R_t &)> &f)
        {
            pre_cluster_listeners += f;
        }
        void add_post_cluster_listener(std::function<void(unsigned, unsigned, const wolff::system<R_t, X_t, G_t> &)> &f)
        {
            post_cluster_listeners += f;
        }

        virtual void pre_cluster(unsigned n, unsigned N, const wolff::system<R_t, X_t, G_t> &S, const typename G_t::vertex &v, const R_t &r) override
        {
            pre_cluster_listeners(n, N, S, v, r);
        }

        virtual void post_cluster(unsigned n, unsigned N, const wolff::system<R_t, X_t, G_t> &S) override
        {
            post_cluster_listeners(n, N, S);
        }

    private:
        Delegate<unsigned, unsigned, const wolff::system<R_t, X_t, G_t> &, const typename G_t::vertex &, const R_t &> pre_cluster_listeners;
        Delegate<unsigned, unsigned, const wolff::system<R_t, X_t, G_t> &> post_cluster_listeners;
    };
}

#endif