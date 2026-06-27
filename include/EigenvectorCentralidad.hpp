#pragma once
#include "Grafo.hpp"
#include <unordered_map>
#include <string>

std::unordered_map<std::string, double> eigenvectorCentralidad(
    const Grafo& grafo,
    int    maxIteraciones = 100,
    double epsilon        = 1e-6
);