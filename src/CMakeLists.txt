
add_executable(xy_square_lattice xy_square_lattice.cpp)
configure_file(xy_plot.py xy_plot.py COPYONLY)

target_compile_definitions(xy_square_lattice PUBLIC WOLFF_NO_FIELD WOLFF_BOND_DEPENDENCE)

target_link_libraries(xy_square_lattice onmodels)