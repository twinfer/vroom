/*

This file is part of VROOM.

Copyright (c) 2015-2025, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include <algorithm>
#include <numeric>

#include "algorithms/kruskal.h"

namespace vroom::utils {

template <class T>
UndirectedGraph<T> minimum_spanning_tree(const UndirectedGraph<T>& graph) {
  // We just need the edges from original graph.
  std::vector<Edge<T>> edges = graph.get_edges();

  // First sorting edges by weight.
  std::ranges::sort(edges, [](const auto& a, const auto& b) {
    return a.get_weight() < b.get_weight();
  });

  // Storing the edges of the minimum spanning tree.
  std::vector<Edge<T>> mst;
  mst.reserve(graph.size() - 1);

  // Union-Find data structure with path compression and union by rank.
  std::vector<Index> parent(graph.size());
  std::iota(parent.begin(), parent.end(), 0);
  std::vector<Index> rank(graph.size(), 0);

  auto find = [&](Index i) {
    Index root = i;
    while (root != parent[root]) {
      root = parent[root];
    }
    while (i != root) {
      Index next = parent[i];
      parent[i] = root;
      i = next;
    }
    return root;
  };

  for (const auto& edge : edges) {
    const Index first_vertex = edge.get_first_vertex();
    const Index second_vertex = edge.get_second_vertex();

    const Index root_first = find(first_vertex);
    const Index root_second = find(second_vertex);

    if (root_first != root_second) {
      mst.push_back(edge);
      
      if (rank[root_first] < rank[root_second]) {
        parent[root_first] = root_second;
      } else {
        parent[root_second] = root_first;
        if (rank[root_first] == rank[root_second]) {
          rank[root_first]++;
        }
      }
    }
  }
  assert(mst.size() == graph.size() - 1);

  return UndirectedGraph<T>(std::move(mst));
}

template UndirectedGraph<UserCost>
minimum_spanning_tree(const UndirectedGraph<UserCost>& graph);

} // namespace vroom::utils
