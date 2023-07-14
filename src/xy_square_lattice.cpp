/*
This file is derived work from the original `Wolff` implementation of Jaron Kent-Dobias, published under the MIT license.
Published article: Cluster representations and the Wolff algorithm in arbitrary external fields, Jaron Kent-Dobias & James P Sethna, Physical Review E 98, 063306 (2018).
Code: https://git.kent-dobias.com/wolff/.
*/

#include <getopt.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#include <wolff_models/vector.hpp>
#include <wolff_models/orthogonal.hpp>

#include "crosssection_measurement.hpp"
#include "xy_types.hpp"

typedef wolff::graph<> G_t;
typedef wolff::system<onmodels::transformation_xy, onmodels::spin_xy, G_t> sys;

std::string pretty_print_crosssection(const std::vector<double> &crosssection, unsigned L)
{
    std::stringstream ss;
    for (unsigned i = 0; i < L; i++)
    {
        for (unsigned j = 0; j < L; j++)
        {
            ss << std::fixed << std::setprecision(2) << crosssection[i * L + j] << " ";
        }
        ss << std::endl;
    }
    return ss.str();
}

int main(int argc, char *argv[])
{
    // set defaults
    unsigned N = (unsigned)1e3; // Number of iterations
    unsigned D = 2;             // Dimension
    unsigned L = 16;            // Lattice side length
    double T = 0.8;             // Temperature

    // take command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "N:D:L:T:")) != -1)
    {
        switch (opt)
        {
        case 'N':
            N = (unsigned)atof(optarg);
            break;
        case 'D':
            D = atoi(optarg);
            break;
        case 'L':
            L = atoi(optarg);
            break;
        case 'T':
            T = atof(optarg);
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    // define the spin-spin coupling
    std::function<double(const onmodels::spin_xy &, const onmodels::spin_xy &)> spin_spin_interaction =
        [](const onmodels::spin_xy &s1, const onmodels::spin_xy &s2) -> double
    {
        return std::cos(s1.theta - s2.theta);
    };

    // initialize the lattice
    G_t graph(D, L);

    // initialize the system
    wolff::system<onmodels::transformation_xy, onmodels::spin_xy> system(graph, T, spin_spin_interaction);

    std::function<onmodels::transformation_xy(std::mt19937 &, const sys &, const G_t::vertex)> transformation_generator = onmodels::generate_uniform_rotation<G_t>;

    // initailze the measurement object
    onmodels::crosssection_measurement measurement(system);

    // initialize the random number generator
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);

    // run wolff N times
    system.run_wolff(N, transformation_generator, measurement, rng);
    std::vector<double> crosssection = measurement.get_crossection();

    // print the result of our measurements
    std::cout << pretty_print_crosssection(crosssection, L) << std::endl;

    return 0;
}