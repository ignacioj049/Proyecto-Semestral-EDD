#pragma once
#include "Grafo.hpp"
#include <unordered_map>
#include <string>

std::unordered_map<std::string, double> pageRank(
    const Grafo& grafo,
    double factorAmortiguamiento = 0.85,
    int    maxIteraciones        = 100,
    double epsilon               = 1e-6
);