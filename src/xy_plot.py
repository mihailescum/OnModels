from typing import Tuple, Literal

import argparse
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable

import numpy as np
import networkx as nx

results_directory = "results"


def plot_configuration(data: np.ndarray):
    fig, ax = plt.subplots()
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)

    im = ax.imshow(data, vmin=0.0, vmax=2 * np.pi, cmap="twilight")
    fig.colorbar(im, cax=cax, orientation="vertical")

    plt.show()


def plot_graph(graph: nx.Graph):
    positions = {n: (graph.nodes[n]["i"], graph.nodes[n]["j"]) for n in graph.nodes}
    nx.draw(graph, positions, node_size=10)
    plt.show()


def to_3D(i: int, L: int) -> Tuple[int, int, int]:
    z = i // (L * L)
    i -= z
    y = i // L
    x = i % L
    return x, y, z


def to_1D(x: int, y: int, z: int, L: int) -> int:
    return x + y * L + z * L * L


def load_crosssection_graph(
    filename: str, L, dimension: Literal["x", "y", "z"] = "z"
) -> nx.Graph:
    result = nx.Graph()

    with open(filename) as f:
        lines = f.read().splitlines()

    adjacency_table = [[int(v) for v in line.split()] for line in lines]

    for i in range(L):
        for j in range(L):
            if dimension == "x":
                ind = to_1D(0, i, j, L)
            elif dimension == "y":
                ind = to_1D(i, 0, j, L)
            elif dimension == "z":
                ind = to_1D(i, j, 0, L)
            else:
                raise ValueError(f"Unknown dimension {dimension}.")

            result.add_node(ind)
            result.nodes[ind]["i"] = i
            result.nodes[ind]["j"] = j

            for neighbor in adjacency_table[ind]:
                x, y, z = to_3D(neighbor, L)
                if (
                    (dimension == "x" and not x == 0)
                    or (dimension == "y" and not y == 0)
                    or (dimension == "z" and not z == 0)
                ):
                    continue

                result.add_edge(ind, neighbor)

    return result


def main(args):
    graph_type = args.graph
    if graph_type == "grid":
        filename = "xy_grid"
    elif graph_type == "hierarchical":
        filename = "xy_hierarchical"
    else:
        print("Unknown graph type")
        exit()

    data = np.loadtxt(results_directory + "/" + filename + ".txt")
    L = data.shape[0]

    graph_crosssection_z = load_crosssection_graph(
        results_directory + "/" + filename + "_adj.txt", L, dimension="z"
    )

    plot_configuration(data)
    plot_graph(graph_crosssection_z)


def configure_args() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Plot an XY-model configuration")
    parser.add_argument(
        "-g",
        "--graph",
        metavar="graph_type",
        help="The graph type. (can be: `grid`, `hierarchical`)",
        default="grid",
    )

    return parser


if __name__ == "__main__":
    parser = configure_args()
    main(parser.parse_args())
