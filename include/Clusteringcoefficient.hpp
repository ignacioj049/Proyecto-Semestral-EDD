#pragma once
#include "Grafo.hpp"
#include <unordered_map>
#include <string>

// Coeficiente de clustering local por vértice y promedio global.
// Solo está definido de forma natural para grafos NO dirigidos;
// para dirigidos se calcula igual usando el grado de salida
struct ClusteringResult
{
    std::unordered_map<std::string, double> local; // C_i de cada vértice
    double global;                                 // promedio de C_i
};

ClusteringResult clusteringCoefficient(const Grafo &grafo);