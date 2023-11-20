#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Graph {
public:
  std::unordered_map<std::string, std::vector<std::string>> adjList;

  void addEdge(const std::string &from, const std::string &to) {
    adjList[from].push_back(to);
  }

  std::unordered_map<std::string, int> cal_fanouts() const {
    std::unordered_map<std::string, int> fanouts;
    for (const auto &node : adjList) {
      fanouts[node.first] = node.second.size();
    }
    return fanouts;
  }

  void printGraph() const {
    for (const auto &node : adjList) {
      std::cout << node.first << " -> ";
      for (const auto &edge : node.second) {
        std::cout << edge << ", ";
      }
      std::cout << std::endl;
    }
  }
};

Graph parseBenchFile(const std::string &filename) {
  Graph graph;
  std::ifstream file(filename);
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Wrong file name" << std::endl;
    exit(EXIT_FAILURE);
  }

  while (getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue;

    std::istringstream iss(line);
    std::string token, gate, type;
    std::vector<std::string> tokens;

    while (getline(iss, token, '(')) {
      tokens.push_back(token);
    }

    if (tokens.size() >= 2) {
      std::istringstream inputs(tokens[1]);
      gate = tokens[0].substr(0, tokens[0].find('=') - 1);
      while (getline(inputs, token, ',')) {
        token.erase(std::remove(token.begin(), token.end(), ')'), token.end());
        token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
        graph.addEdge(token, gate);
      }
    }
  }

  file.close();
  return graph;
}

std::pair<std::unordered_map<std::string, int>,
          std::unordered_map<std::string, std::string>>
dijkstra(const Graph &graph, const std::string &source) {
  auto fanouts = graph.cal_fanouts();
  std::unordered_map<std::string, int> distances;
  std::unordered_map<std::string, std::string> predecessors;
  for (const auto &node : graph.adjList) {
    distances[node.first] = std::numeric_limits<int>::max();
  }
  distances[source] = 0;

  auto cmp = [](const std::pair<std::string, int> &left,
                const std::pair<std::string, int> &right) {
    return left.second > right.second;
  };
  std::priority_queue<std::pair<std::string, int>,
                      std::vector<std::pair<std::string, int>>, decltype(cmp)>
      pq(cmp);

  pq.push({source, 0});

  while (!pq.empty()) {
    std::string u = pq.top().first;
    pq.pop();

    if (graph.adjList.find(u) == graph.adjList.end())
      continue;

    for (const std::string &v : graph.adjList.at(u)) {
      int weight = fanouts[u];
      int distanceThroughU = distances[u] + weight;
      if (distanceThroughU < distances[v]) {
        distances[v] = distanceThroughU;
        predecessors[v] = u;
        pq.push({v, distanceThroughU});
      }
    }
  }

  return {distances, predecessors};
}

std::vector<std::string> reconstructPath(
    const std::unordered_map<std::string, std::string> &predecessors,
    const std::string &source, const std::string &destination) {
  std::vector<std::string> path;
  for (std::string at = destination; at != source; at = predecessors.at(at)) {
    path.push_back(at);
  }
  path.push_back(source);
  std::reverse(path.begin(), path.end());
  return path;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Incorrect number of arguments" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  std::string source = argv[2];
  std::string destination = argv[3];

  Graph graph = parseBenchFile(filename);
  auto fanouts = graph.cal_fanouts();
  auto results = dijkstra(graph, source);
  auto distances = results.first;
  auto predecessors = results.second;

  if (graph.adjList.find(source) == graph.adjList.end()) {
    std::cerr << "Signal " << source << " not found in file " << filename
              << std::endl;
    return 1;
  }

  else if (graph.adjList.find(destination) == graph.adjList.end()) {
    std::cerr << "Signal " << destination << " not found in file " << filename
              << std::endl;
    return 1;
  }

  if (distances[destination] == std::numeric_limits<int>::max()) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("INPUT") == std::string::npos) {
            size_t pos = line.find("(");
            size_t endPos = line.find(")");
            std::string inputName = line.substr(pos + 1, endPos - pos - 1);
            if (inputName == source) {
                std::cout << source << " is not an input pin" << std::endl;
                return 0;
            }
        }
        else if (line.find("OUTPUT") == std::string::npos) {
            size_t pos = line.find("(");
            size_t endPos = line.find(")");
            std::string outputName = line.substr(pos + 1, endPos - pos - 1);
            if (outputName == destination) {
                std::cout << destination << " is not an output pin" << std::endl;
                return 1;
            }
        }
    }
    file.close();
    return 1;
  } else {
    std::vector<std::string> path =
        reconstructPath(predecessors, source, destination);
    std::cout << source << " --> " << destination << ": shortest path is ";
    for (size_t i = 0; i < path.size(); ++i) {
      std::cout << path[i];
      if (i < path.size() - 1) {
        std::cout << " --> ";
      }
    }
    if (distances[destination]>1) {
      std::cout << " = " << distances[destination] - 1 << std::endl;
    } else if(distances[destination]==1){
      std::cout << " = " << distances[destination] << std::endl;
    }
  }
  return 0;
}
