#include <vector>

#include <wolff.hpp>

namespace onmodels
{
    unsigned to_1D(unsigned x, unsigned y, unsigned z, unsigned L)
    {
        return x + L * y + L * L * z;
    }

    std::tuple<unsigned, unsigned, unsigned> to_3D(unsigned idx, unsigned L)
    {
        unsigned z = idx / (L * L);
        idx -= z * L * L;
        unsigned y = idx / L;
        unsigned x = idx % L;
        return std::tuple<unsigned, unsigned, unsigned>(x, y, z);
    }

    class grid : public wolff::graph<>
    {
    public:
        grid(unsigned L) : graph()
        {
            D = 3;
            this->L = L;
            nv = pow(L, D);

            vertices.resize(nv);

            unsigned j = -1;
            for (unsigned i = 0; i < nv; i++)
            {
                vertices[i].ind = i;
                auto [x, y, z] = to_3D(i, L);
                if (x > 0)
                {
                    j = to_1D(x - 1, y, z, L);
                    add_edge(i, j);
                }
                if (x < L - 1)
                {
                    j = to_1D(x + 1, y, z, L);
                    add_edge(i, j);
                }

                if (y > 0)
                {
                    j = to_1D(x, y - 1, z, L);
                    add_edge(i, j);
                }
                if (y < L - 1)
                {
                    j = to_1D(x, y + 1, z, L);
                    add_edge(i, j);
                }

                if (z > 0)
                {
                    j = to_1D(x, y, z - 1, L);
                    add_edge(i, j);
                }
                if (z < L - 1)
                {
                    j = to_1D(x, y, z + 1, L);
                    add_edge(i, j);
                }
            }
        }

    private:
        void add_edge(unsigned i, unsigned j)
        {
            halfedge f(vertices[i], vertices[j]);
            vertices[i].edges.push_back(f);
            ne++;
        }
    };

    class hierarchical : public wolff::graph<>
    {
    public:
        hierarchical(unsigned L, unsigned levels) : graph()
        {
            if (L % 2 != 0)
                throw std::invalid_argument("L must be a power of two");

            this->L = L;
            D = 3;
            nv = pow(L, D);
            vertices.resize(nv);

            for (unsigned x = 0; x < L; x++)
                for (unsigned y = 0; y < L; y++)
                    for (unsigned z = 0; z < L; z++)
                        vertices[to_1D(x, y, z, L)].ind = to_1D(x, y, z, L);
            ne = generate_level(0, 0, 0, L, levels - 1);
        }

    private:
        void add_edge(unsigned i, unsigned j)
        {
            halfedge f(vertices[i], vertices[j]);
            vertices[i].edges.push_back(f);
            ne++;
        }

        unsigned generate_level(unsigned x, unsigned y, unsigned z, unsigned L_level, unsigned level)
        {
            unsigned ne = 0;
            if (level > 0)
            {
                for (unsigned i = 0; i < 8; i++)
                {
                    unsigned L_local_level = L_level / 2;
                    auto [x_local, y_local, z_local] = to_3D(i, 2);
                    ne += generate_level(x + L_local_level * x_local, y + L_local_level * y_local, z + L_local_level * z_local, L_local_level, level - 1);

                    unsigned x_center = x + L_local_level * x_local + L_local_level / 2;
                    unsigned y_center = y + L_local_level * y_local + L_local_level / 2;
                    unsigned z_center = z + L_local_level * z_local; // + L_local_level / 2;
                    unsigned ind_center = to_1D(x_center, y_center, z_center, L);

                    if (x_local > 0)
                    {
                        unsigned ind_neighbor = to_1D(x + L_local_level * (x_local - 1) + L_local_level / 2, y_center, z_center, L);
                        halfedge e(vertices[ind_center], vertices[ind_neighbor]);
                        vertices[ind_center].edges.push_back(e);
                        ne++;
                    }
                    if (x_local < 1)
                    {
                        unsigned ind_neighbor = to_1D(x + L_local_level * (x_local + 1) + L_local_level / 2, y_center, z_center, L);
                        halfedge e(vertices[ind_center], vertices[ind_neighbor]);
                        vertices[ind_center].edges.push_back(e);
                        ne++;
                    }

                    if (y_local > 0)
                    {
                        unsigned ind_neighbor = to_1D(x_center, y + L_local_level * (y_local - 1) + L_local_level / 2, z_center, L);
                        halfedge e(vertices[ind_center], vertices[ind_neighbor]);
                        vertices[ind_center].edges.push_back(e);
                        ne++;
                    }
                    if (y_local < 1)
                    {
                        unsigned ind_neighbor = to_1D(x_center, y + L_local_level * (y_local + 1) + L_local_level / 2, z_center, L);
                        halfedge e(vertices[ind_center], vertices[ind_neighbor]);
                        vertices[ind_center].edges.push_back(e);
                        ne++;
                    }

                    if (z_local > 0)
                    {
                        unsigned ind_neighbor = to_1D(x_center, y_center, z + L_local_level * (z_local - 1), L);
                        halfedge e(vertices[ind_center], vertices[ind_neighbor]);
                        vertices[ind_center].edges.push_back(e);
                        ne++;
                    }
                    if (z_local < 1)
                    {
                        unsigned ind_neighbor = to_1D(x_center, y_center, z + L_local_level * (z_local + 1), L);
                        halfedge e(vertices[ind_center], vertices[ind_neighbor]);
                        vertices[ind_center].edges.push_back(e);
                        ne++;
                    }
                }
            }
            else
            {
                grid grid(L_level);
                for (wolff::graph<>::vertex &v : grid.vertices)
                {
                    auto [x_local, y_local, z_local] = to_3D(v.ind, L_level);
                    unsigned ind = to_1D(x + x_local, y + y_local, z + z_local, L);

                    for (wolff::graph<>::halfedge &e : v.edges)
                    {
                        auto [x_neighbor_local, y_neighbor_local, z_neighbor_local] = to_3D(e.neighbor.ind, L_level);
                        unsigned ind_neighbor = to_1D(x + x_neighbor_local, y + y_neighbor_local, z + z_neighbor_local, L);

                        halfedge f(vertices[ind], vertices[ind_neighbor]);
                        vertices[ind].edges.push_back(f);
                    }
                }
                ne = grid.ne;
            }
            return ne;
        }
    };

    std::vector<std::vector<int>>
    compute_adjacency_table(const wolff::graph<> &graph)
    {
        std::vector<std::vector<int>> result;
        for (unsigned i = 0; i < graph.nv; i++)
        {
            std::vector<int> neighbors;
            for (const wolff::graph<>::halfedge &edge : graph.vertices[i].edges)
                neighbors.push_back(edge.neighbor.ind);
            result.push_back(neighbors);
        }
        return result;
    }
}