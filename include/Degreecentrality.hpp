#pragma once
#include "Grafo.hpp"
#include <unordered_map>
#include <string>

// Centralidad de grado normalizada: grado(v) / (n - 1)
// Para grafos dirigidos se calculan in-degree y out-degree por separado.
// Para grafos no dirigidos, inDegree == outDegree == degree.
struct DegreeCentralityResult {
    std::unordered_map<std::string, double> outDegree;
    std::unordered_map<std::string, double> inDegree;
};

DegreeCentralityResult degreeCentrality(const Grafo& grafo);
