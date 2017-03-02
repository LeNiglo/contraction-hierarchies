// Copyright 2010-2014 Google
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <string>
#include <limits>
using namespace std;

#include "base.h"
#include "graph.h"
#include "bidi_dijkstra.h"

// Pseudo-random generator, shamelessly copied from
// https://github.com/google/or-tools/blob/master/src/base/random.h
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
class ACMRandom {
 public:
  explicit ACMRandom(int32 seed) : seed_(seed) {}

  int32 Next() {
    if (seed_ == 0) {
      seed_ = 0x14fd4603;  // Arbitrary random constant
    }
    const int32 M = 2147483647L;  // 2^31-1
    const int32 A = 16807;
    // In effect, we are computing seed_ = (seed_ * A) % M, where M = 2^31-1
    uint32 lo = A * static_cast<int32>(seed_ & 0xFFFF);
    uint32 hi = A * static_cast<int32>(static_cast<uint32>(seed_) >> 16);
    lo += (hi & 0x7FFF) << 16;
    if (lo > M) {
      lo &= M;
      ++lo;
    }
    lo += hi >> 15;
    if (lo > M) {
      lo &= M;
      ++lo;
    }
    return (seed_ = static_cast<int32>(lo));
  }

  // Returns a random value in [0..n-1]. If n == 0, always returns 0.
  int32 Uniform(int32 n) { return n == 0 ? 0 : Next() % n; }

  int64 Next64() {
    const int64 next = Next();
    return (next - 1) * 2147483646L + Next();
  }

  float RndFloat() {
    return Next() * 0.000000000465661273646;  // x: x * (M-1) = 1 - eps
  }

  // Returns a double in [0, 1).
  double RndDouble() {
    // Android does not provide ieee754.h and the associated types.
    union {
      double d;
      int64 i;
    } ieee_double;
    ieee_double.i = Next64();
    ieee_double.i &= ~(1LL << 63);  // Clear sign bit.
    // The returned number will be between 0 and 1. Take into account the
    // exponent offset.
    ieee_double.i |= (1023LL << 52);
    return ieee_double.d - static_cast<double>(1.0);
  }

  double RandDouble() { return RndDouble(); }

  double UniformDouble(double x) { return RandDouble() * x; }

  // Returns a double in [a, b). The distribution is uniform.
  double UniformDouble(double a, double b) { return a + (b - a) * RndDouble(); }

  // Returns true with probability 1/n. If n=0, always returns true.
  bool OneIn(int n) { return Uniform(n) == 0; }

  void Reset(int32 seed) { seed_ = seed; }
  static int32 DeterministicSeed() { return 0; }

// RandomNumberGenerator concept. Example:
//   ACMRandom rand(my_seed);
//   std::random_shuffle(myvec.begin(), myvec.end(), rand);
#if defined(_MSC_VER)
  typedef __int64 difference_type;  // NOLINT
#else
  typedef long long difference_type;  // NOLINT
#endif
  int64 operator()(int64 val_max) { return Next64() % val_max; }

 private:
  int32 seed_;
};

string NodePathOfArcPath(const Graph& graph, const vector<int>& arc_path, int src) {
  string path;
  path += std::to_string(src);
  for (int i = 0; i < arc_path.size(); ++i) {
    path += ", ";
    path += std::to_string(graph.Head(arc_path[i]));
  }
  return path;
}

int main() {
  int num_tests = 0;

  // Ladder graph. 0->1->2->3->4.... Arcs all have cost 1.
  Graph ladder_graph;
  const int kLadderNumNodes = 10;
  vector<double> ladder_arc_lengths;
  for (int i = 1; i < kLadderNumNodes; ++i) {
    ladder_graph.AddArc(i - 1, i);
    ladder_arc_lengths.push_back(1);
  }

  // Test case: Only one BidirectionalDijkstra exploration on the ladder graph.
  // Sparsity is NOT checked.
  {
    BidirectionalDijkstra dijkstra(&ladder_graph, &ladder_arc_lengths);
    vector<int> targets;
    const vector<int> path = dijkstra.FindShortestPath(3, 7);
    CHECK_EQ(NodePathOfArcPath(ladder_graph, path, 3), "3, 4, 5, 6, 7");
    cout << "Test #" << num_tests++ << " PASSED" << endl;
    CHECK_EQ(dijkstra.FindShortestPath(7, 3).size(), 0);
    cout << "Test #" << num_tests++ << " PASSED" << endl;
  }

  // Bidirectional cycle:  ...<->N-1<->0<->1<->2<->3<->4<->...,
  // with arc costs: 0.1 from i to i+1 and 0.2 from i+1 to i.
  Graph cycle_graph;
  vector<double> cycle_arc_lengths;
  const int kCycleNumNodes = 10;
  for (int i = 0; i < kCycleNumNodes; ++i) {
    const int next = i == kCycleNumNodes - 1 ? 0 : i + 1;
    cycle_graph.AddArc(i, next);
    cycle_arc_lengths.push_back(0.1);
    cycle_graph.AddArc(next, i);
    cycle_arc_lengths.push_back(0.2);
  }

  // Test case: only one BidirectionalDijkstra exploration. Sparsity not checked.
  {
    CHECK_EQ(kCycleNumNodes, 10);  // The numbers below only work for 10.
    BidirectionalDijkstra dijkstra(&cycle_graph, &cycle_arc_lengths);
    CHECK_EQ(NodePathOfArcPath(cycle_graph, dijkstra.FindShortestPath(9, 5), 9),
             "9, 0, 1, 2, 3, 4, 5");
    CHECK_EQ(NodePathOfArcPath(cycle_graph, dijkstra.FindShortestPath(9, 6), 9),
             "9, 8, 7, 6");
    cout << "Test #" << num_tests++ << " PASSED" << endl;
  }

  // Messy graph.
  Graph messy_graph;
  vector<double> messy_arc_lengths;
  messy_graph.AddArc(0, 4);
  messy_arc_lengths.push_back(0.2);
  messy_graph.AddArc(0, 4);
  messy_arc_lengths.push_back(0.1);
  messy_graph.AddArc(0, 3);
  messy_arc_lengths.push_back(0.29);
  messy_graph.AddArc(0, 1);
  messy_arc_lengths.push_back(0.4);
  messy_graph.AddArc(5, 5);
  messy_arc_lengths.push_back(0.1);
  messy_graph.AddArc(5, 0);
  messy_arc_lengths.push_back(0.2);
  messy_graph.AddArc(5, 8);
  messy_arc_lengths.push_back(0.4);
  messy_graph.AddArc(3, 6);
  messy_arc_lengths.push_back(0.1);
  messy_graph.AddArc(3, 6);
  messy_arc_lengths.push_back(0.1);
  messy_graph.AddArc(4, 3);
  messy_arc_lengths.push_back(0.6);
  messy_graph.AddArc(4, 5);
  messy_arc_lengths.push_back(0.0);
  messy_graph.AddArc(4, 7);
  messy_arc_lengths.push_back(0.3);
  messy_graph.AddArc(4, 6);
  messy_arc_lengths.push_back(0.0);
  messy_graph.AddArc(8, 8);
  messy_arc_lengths.push_back(0.0);
  messy_graph.AddArc(1, 6);
  messy_arc_lengths.push_back(0.0);
  messy_graph.AddArc(1, 3);
  messy_arc_lengths.push_back(0.2);
  messy_graph.AddArc(7, 6);
  messy_arc_lengths.push_back(0.1);
  messy_graph.AddArc(7, 5);
  messy_arc_lengths.push_back(0.1);
  messy_graph.AddArc(7, 0);
  messy_arc_lengths.push_back(0.1);
  messy_graph.AddArc(6, 0);
  messy_arc_lengths.push_back(0.5);
  messy_graph.AddArc(6, 1);
  messy_arc_lengths.push_back(0.0);
  messy_graph.AddArc(6, 5);
  messy_arc_lengths.push_back(0.2);

  // One dijkstra. Empty target set = all targets.
  // Sparsity still not checked!
  const double kInfinity = std::numeric_limits<double>::infinity();
  {
    BidirectionalDijkstra dijkstra(&messy_graph, &messy_arc_lengths);
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(4, 0), 4), "4, 5, 0");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(4, 1), 4), "4, 6, 1");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(4, 3), 4), "4, 6, 1, 3");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(4, 4), 4), "4");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(4, 5), 4), "4, 5");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(4, 6), 4), "4, 6");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(4, 7), 4), "4, 7");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(4, 8), 4), "4, 5, 8");
    CHECK_EQ(dijkstra.FindShortestPath(2, 3).size(), 0);  // No path.
    CHECK_EQ(dijkstra.FindShortestPath(8, 3).size(), 0);  // No path.
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(5, 0), 5), "5, 0");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(5, 1), 5), "5, 0, 4, 6, 1");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(5, 3), 5), "5, 0, 3");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(5, 4), 5), "5, 0, 4");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(5, 5), 5), "5");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(5, 6), 5), "5, 0, 4, 6");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(5, 7), 5), "5, 0, 4, 7");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.FindShortestPath(5, 8), 5), "5, 8");
    cout << "Test #" << num_tests++ << " PASSED" << endl;
  }

  // Performance / stress tests.
  cerr << "Building the hypercube graph for the performance test.";
  // Build an hypercube of size 4, dimension 10. Each coordinate is in 0..3,
  // i.e. 2 bits: we pack an entire node with 2x10 bits.
  const int kDimension = 10;
  const int num_nodes = 1 << (2 * kDimension);
  Graph hc_graph;
  hc_graph.AddNode(num_nodes - 1);
  for (int node = 0; node < num_nodes; ++node) {
    for (int d = 0; d < kDimension; ++d) {
      int delta = 1 << (2 * d);
      const int x = (node >> (2 * d)) & 3;
      if (x > 0) {
        hc_graph.AddArc(node - delta, node);
        hc_graph.AddArc(node, node - delta);
      }
      if (x < 3) {
        hc_graph.AddArc(node + delta, node);
        hc_graph.AddArc(node, node + delta);
      }
    }
  }
  ACMRandom random(0);
  vector<double> hc_arc_lengths(hc_graph.NumArcs(), 0);
  for (int i = 0; i < hc_arc_lengths.size(); ++i) {
    hc_arc_lengths[i] = random.RandDouble();
  }
  cerr << "Done! Running the 'big' performance tests" << endl;

  // Run N BidirectionalDijkstra that should be "relatively short" because the
  // half-spheres are much smaller than the full one.
  {
    BidirectionalDijkstra dijkstra(&hc_graph, &hc_arc_lengths);
    Dijkstra std_dijkstra(&hc_graph, &hc_arc_lengths);

    // Run a BidirectionalDijkstra from one end to the other. It should explore the entire graph.
    clock_t c0 = clock();
    const int kNumDijkstras = 10;
    double sum_path_lengths = 0;
    for (int i = 0; i < kNumDijkstras; ++i) {
      const int src = random.Uniform(num_nodes);
      const int dst = random.Uniform(num_nodes);
      sum_path_lengths += dijkstra.FindShortestPath(src, dst).size();
    }
    clock_t c1 = clock();
    cout << "Performance: " << double(c1 - c0)/ CLOCKS_PER_SEC << " seconds." << endl;
    cout << "(the reference solution passes in about 0.4s on a 2-year old laptop)" << endl;
    CHECK_NEAR(sum_path_lengths / (kDimension * kNumDijkstras), 1.8, 0.2);
    cout << "Test #" << num_tests++ << " PASSED!" << endl;
  }
}
