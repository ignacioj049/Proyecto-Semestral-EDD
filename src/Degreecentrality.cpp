#include "Degreecentrality.hpp"

// Degree Centrality (normalizada)

DegreeCentralityResult degreeCentrality(const Grafo& grafo) {
    DegreeCentralityResult resultado;

    const auto& listaVertices = grafo.vertices();
    int n = grafo.numVertices();

    // Con n <= 1 no tiene sentido normalizar (división por cero)
    if (n <= 1) {
        for (const auto& v : listaVertices) {
            resultado.outDegree[v] = 0.0;
            resultado.inDegree[v]  = 0.0;
        }
        return resultado;
    }

    double norm = static_cast<double>(n - 1);

    for (const auto& v : listaVertices) {
        resultado.outDegree[v] = grafo.gradoSalida(v)  / norm;
        resultado.inDegree[v]  = grafo.gradoEntrada(v) / norm;
    }

    return resultado;
}