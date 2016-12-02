#include <iostream>
#include <string>
#include <limits>
using namespace std;

#include "base.h"
#include "graph.h"
#include "dijkstra.h"

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

  // Test case: Only one Dijkstra exploration on the ladder graph.
  // Sparsity is NOT checked.
  {
    Dijkstra dijkstra(&ladder_graph, &ladder_arc_lengths);
    vector<int> targets;
    targets.push_back(7);
    dijkstra.RunUntilAllTargetsAreReached(3, targets);
    CHECK_EQ(dijkstra.GetGraph().NumArcs(), kLadderNumNodes - 1);
    CHECK_EQ(dijkstra.Distances()[7], 4);
    cout << "Test #" << num_tests++ << " PASSED" << endl;
    CHECK_EQ(dijkstra.Distances()[3], 0);
    CHECK_EQ(dijkstra.Distances()[4], 1);
    CHECK_EQ(dijkstra.Distances()[5], 2);
    CHECK_EQ(dijkstra.Distances()[6], 3);
    cout << "Test #" << num_tests++ << " PASSED" << endl;
    CHECK_EQ(dijkstra.ParentArcs()[7], 6);
    CHECK_EQ(dijkstra.ParentArcs()[6], 5);
    CHECK_EQ(dijkstra.ParentArcs()[5], 4);
    CHECK_EQ(dijkstra.ParentArcs()[4], 3);
    CHECK_EQ(dijkstra.ParentArcs()[3], -1);
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

  // Test case: only one Dijkstra exploration. Sparsity not checked.
  {
    CHECK_EQ(kCycleNumNodes, 10);  // The numbers below only work for 10.
    Dijkstra dijkstra(&cycle_graph, &cycle_arc_lengths);
    vector<int> targets;
    targets.push_back(5);
    targets.push_back(6);
    dijkstra.RunUntilAllTargetsAreReached(9, targets);
    CHECK_EQ(dijkstra.GetGraph().NumArcs(), 20);
    CHECK_EQ(dijkstra.ReachedNodes().size(), 10);
    CHECK_EQ(dijkstra.Distances()[9], 0);
    CHECK_NEAR(dijkstra.Distances()[0], 0.1, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[1], 0.2, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[2], 0.3, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[3], 0.4, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[4], 0.5, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[5], 0.6, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[6], 0.6, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[7], 0.4, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[8], 0.2, 1e-9);
    CHECK_EQ(dijkstra.ParentArcs()[9], -1);
    CHECK_EQ(dijkstra.ParentArcs()[0], 18);
    CHECK_EQ(dijkstra.ParentArcs()[1], 0);
    CHECK_EQ(dijkstra.ParentArcs()[2], 2);
    CHECK_EQ(dijkstra.ParentArcs()[3], 4);
    CHECK_EQ(dijkstra.ParentArcs()[4], 6);
    CHECK_EQ(dijkstra.ParentArcs()[5], 8);
    CHECK_EQ(dijkstra.ParentArcs()[6], 13);
    CHECK_EQ(dijkstra.ParentArcs()[7], 15);
    CHECK_EQ(dijkstra.ParentArcs()[8], 17);
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
    Dijkstra dijkstra(&messy_graph, &messy_arc_lengths);
    const vector<int> empty;
    dijkstra.RunUntilAllTargetsAreReached(4, empty);

    CHECK_EQ(dijkstra.GetGraph().NumArcs(), 22);
    CHECK_NEAR(dijkstra.Distances()[0], 0.2, 1e-9);  // 4->5->0
    CHECK_NEAR(dijkstra.Distances()[1], 0.0, 1e-9);  // 4->6->1
    CHECK_EQ(dijkstra.Distances()[2], kInfinity);
    CHECK_NEAR(dijkstra.Distances()[3], 0.2, 1e-9);  // 4->6->1->3
    CHECK_EQ(dijkstra.Distances()[4], 0.0);
    CHECK_NEAR(dijkstra.Distances()[5], 0.0, 1e-9);  // 4->5
    CHECK_NEAR(dijkstra.Distances()[6], 0.0, 1e-9);  // 4->6
    CHECK_NEAR(dijkstra.Distances()[7], 0.3, 1e-9);  // 4->7
    CHECK_NEAR(dijkstra.Distances()[8], 0.4, 1e-9);  // 4->5->8
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(0), 4), "4, 5, 0");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(1), 4), "4, 6, 1");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(3), 4), "4, 6, 1, 3");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(4), 4), "4");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(5), 4), "4, 5");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(6), 4), "4, 6");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(7), 4), "4, 7");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(8), 4), "4, 5, 8");
    cout << "Test #" << num_tests++ << " PASSED" << endl;

    // Second run! Everything should start breaking now.
    dijkstra.RunUntilAllTargetsAreReached(8, empty);
    CHECK_EQ(dijkstra.ReachedNodes().size(), 1);
    CHECK_EQ(dijkstra.Distances()[0], kInfinity);
    CHECK_EQ(dijkstra.Distances()[1], kInfinity);
    CHECK_EQ(dijkstra.Distances()[2], kInfinity);
    CHECK_EQ(dijkstra.Distances()[3], kInfinity);
    CHECK_EQ(dijkstra.Distances()[4], kInfinity);
    CHECK_EQ(dijkstra.Distances()[5], kInfinity);
    CHECK_EQ(dijkstra.Distances()[6], kInfinity);
    CHECK_EQ(dijkstra.Distances()[7], kInfinity);
    CHECK_EQ(dijkstra.Distances()[8], 0);
    cout << "Test #" << num_tests++ << " PASSED" << endl;

    // Third run! If the second run passed, this should work okay.
    cout << "Test #" << num_tests++ << " PASSED" << endl;
    dijkstra.RunUntilAllTargetsAreReached(5, empty);
    CHECK_EQ(dijkstra.ReachedNodes().size(), 8);
    CHECK_NEAR(dijkstra.Distances()[0], 0.2, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[1], 0.3, 1e-9);
    CHECK_EQ(dijkstra.Distances()[2], kInfinity);
    CHECK_NEAR(dijkstra.Distances()[3], 0.49, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[4], 0.3, 1e-9);
    CHECK_EQ(dijkstra.Distances()[5], 0);
    CHECK_NEAR(dijkstra.Distances()[6], 0.3, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[7], 0.6, 1e-9);
    CHECK_NEAR(dijkstra.Distances()[8], 0.4, 1e-9);
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(0), 5), "5, 0");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(1), 5), "5, 0, 4, 6, 1");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(3), 5), "5, 0, 3");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(4), 5), "5, 0, 4");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(5), 5), "5");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(6), 5), "5, 0, 4, 6");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(7), 5), "5, 0, 4, 7");
    CHECK_EQ(NodePathOfArcPath(messy_graph, dijkstra.ArcPathFromSourceTo(8), 5), "5, 8");
    cout << "Test #" << num_tests++ << " PASSED" << endl;
  }

  // Performance / stress tests.
  cerr << "Building the big graph for the stress/performance test...";
  srandom(0);
  Graph grid_graph;
  vector<double> grid_arc_lengths;
  const int kSize = 1000;
  for (int i = 0; i < kSize; ++i) {
    for (int j = 1; j < kSize; ++j) {
      vector<pair<int, int> > arcs;
      arcs.push_back(make_pair(i * kSize + j - 1, i * kSize + j));
      arcs.push_back(make_pair((j - 1) * kSize + i, j * kSize + i));
      for (int k = 0; k < arcs.size(); ++k) {
        grid_graph.AddArc(arcs[k].first, arcs[k].second);
        grid_arc_lengths.push_back(double(random()) / RAND_MAX);
        grid_graph.AddArc(arcs[k].second, arcs[k].first);
        grid_arc_lengths.push_back(double(random()) / RAND_MAX);
      }
    }
  }
  cerr << "Done! Running the 'big' performance test" << endl;

  // Run 100 Dijkstra that should be "relatively short" because the
  // src/targets are closeby. Verify correctness for some.
  {
    Dijkstra dijkstra(&grid_graph, &grid_arc_lengths);

    // Run a Dijkstra from one end to the other. It should explore the entire graph.
    clock_t c0 = clock();
    vector<int> targets;
    dijkstra.RunUntilAllTargetsAreReached(0, targets);
    clock_t c1 = clock();
    CHECK_NEAR(dijkstra.Distances()[kSize * kSize - 1], 458.355733964, 1e-6);
    cout << "Test #" << num_tests++ << " PASSED! Performance: "
         << double(c1 - c0)/ CLOCKS_PER_SEC << " seconds for the big Dijkstra." << endl;

    c0 = clock();
    const int kNumDijkstras = 1000;
    double total_dist = 0.0;
    for (int k = 0; k < kNumDijkstras; ++k) {
      const int src_x = random() % kSize;
      const int src_y = random() % kSize;
      const int src = src_x * kSize + src_y;
      const int dst_x = std::max(0, std::min(kSize - 1, int(src_x + (random() % 7) - 3)));
      const int dst_y = std::max(0, std::min(kSize - 1, int(src_y + (random() % 7) - 3)));
      const int dst = dst_x * kSize + dst_y;
      vector<int> targets;
      targets.push_back(dst);
      dijkstra.RunUntilAllTargetsAreReached(src, targets);
      if (k == 27) {
        CHECK_EQ(src, 313438)
          << "IF THIS FAILED, YOUR CODE MAY BE FINE, BUT THE TEST DOESN'T WORK ON YOUR MACHINE -- GO TELL ME!";
        CHECK_EQ(dst, 312435)
          << "IF THIS FAILED, YOUR CODE MAY BE FINE, BUT THE TEST DOESN'T WORK ON YOUR MACHINE -- GO TELL ME!";
        CHECK_NEAR(dijkstra.Distances()[dst], 1.340870288825, 1e-9);
        CHECK_EQ(NodePathOfArcPath(grid_graph, dijkstra.ArcPathFromSourceTo(dst), src),
            "313438, 313437, 313436, 312436, 312435");
        cout << "Test #" << num_tests++ << " PASSED" << endl;
      }
      total_dist += dijkstra.Distances()[dst];
    }
    c1 = clock();
    CHECK_NEAR(total_dist, 1248.75449897, 1e-6);
    cout << "Test #" << num_tests++ << " PASSED! Performance: "
         << double(c1 - c0)/ CLOCKS_PER_SEC << " seconds for the 1000 dijkstras." << endl;
  }

  cout << "DONE for now -- BUT MORE TESTS ARE COMING SOON, TESTING MORE THINGS"
       << endl;
}
