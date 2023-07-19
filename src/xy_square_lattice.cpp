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

#include <wolff_models/vector.hpp>
#include <wolff_models/orthogonal.hpp>

#include "crosssection_measurement.hpp"
#include "xy_types.hpp"
#include "graphs.hpp"
#include "io.hpp"

typedef wolff::graph<std::tuple<>, double> G_t;
typedef wolff::system<onmodels::transformation_xy, onmodels::spin_xy, G_t> sys;

enum GraphType
{
    Grid,
    Hierarchical,
    Invalid,
};

GraphType resolveGraphType(const std::string type)
{
    if (type == "grid")
        return GraphType::Grid;
    else if (type == "hierarchical")
        return GraphType::Hierarchical;
    else
        return GraphType::Invalid;
}

int main(int argc, char *argv[])
{
    // set defaults
    unsigned N = (unsigned)1e3; // Number of iterations
    unsigned L = 16;            // Lattice side length
    double T = 10;              // Temperature
    std::string graphTypeStr = "hierarchical";
    std::string result_directory = "results";

    // take command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "N:L:T:G:")) != -1)
    {
        switch (opt)
        {
        case 'N':
            N = (unsigned)atof(optarg);
            break;
        case 'L':
            L = atoi(optarg);
            break;
        case 'T':
            T = atof(optarg);
            break;
        case 'G':
            graphTypeStr = optarg;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    GraphType graphType = resolveGraphType(graphTypeStr);

    G_t graph(2, L);
    std::string measurement_filename;
    switch (graphType)
    {
    case GraphType::Grid:
        measurement_filename = "xy_grid";
        graph = onmodels::grid(L);
        break;
    case GraphType::Hierarchical:
        measurement_filename = "xy_hierarchical";
        graph = onmodels::hierarchical(L, 3);
        break;
    default:
        exit(EXIT_FAILURE);
    }

    // define the spin-spin coupling
    std::function<double(const G_t::halfedge &, const onmodels::spin_xy &, const onmodels::spin_xy &)> spin_spin_interaction =
        [](const G_t::halfedge &e, const onmodels::spin_xy &s1, const onmodels::spin_xy &s2) -> double
    {
        double strenght = e.prop;
        return strenght * std::cos(s1.theta - s2.theta);
    };

    // initialize the system
    sys system(graph, T, spin_spin_interaction);

    std::function<onmodels::transformation_xy(std::mt19937 &, const sys &, const G_t::vertex)> transformation_generator = onmodels::generate_uniform_rotation<G_t>;

    // initailze the measurement object
    onmodels::crosssection_measurement measurement(system);

    // initialize the random number generator
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);

    // run wolff N times
    system.run_wolff(N, transformation_generator, measurement, rng);
    std::vector<double> crosssection = measurement.get_crossection();

    // Write the result of our measurements
    std::string measurement_result = onmodels::prettyPrintCrosssection(crosssection, L);
    if (!onmodels::write_txt(measurement_result, result_directory, measurement_filename))
        exit(EXIT_FAILURE);

    // Write adjacency matrix
    std::vector<std::vector<int>> adj_matrix = onmodels::compute_adjacency_table(graph);
    if (!onmodels::write_txt(onmodels::table2str(adj_matrix), result_directory, measurement_filename + "_adj"))
        exit(EXIT_FAILURE);

    return 0;
}