#include "EigenvectorCentralidad.hpp"
#include <cmath>
#include <algorithm>
#include <numeric>


std::unordered_map<std::string, double> eigenvectorCentralidad(
    const Grafo& grafo,
    int    maxIteraciones,
    double epsilon)
{
    const auto& listaVertices = grafo.vertices();
    int n = grafo.numVertices();

    if (n == 0) return {};

    //Inicialización uniforme
    std::unordered_map<std::string, double> x, xNuevo;
    double valorInicial = 1.0 / n;
    for (const auto& v : listaVertices) {
        x[v]      = valorInicial;
        xNuevo[v] = 0.0;
    }

    //Iteraciones
    for (int iter = 0; iter < maxIteraciones; ++iter) {

        for (const auto& v : listaVertices)
            xNuevo[v] = 0.0;

        for (const auto& u : listaVertices) {
            for (const auto& arista : grafo.vecinos(u)) {
                xNuevo[arista.destino] += x[u];
                //Para grafos ponderados, debería multiplicar por arista.peso
                // xNuevo[arista.destino] += x[u] * arista.peso;
            }
        }

        //Se normaliza por la norma 
        double norma = 0.0;
        for (const auto& v : listaVertices)
            norma += xNuevo[v] * xNuevo[v];
        norma = std::sqrt(norma);

        if (norma < 1e-12) break; 

        for (const auto& v : listaVertices)
            xNuevo[v] /= norma;

        //Verificar convergencia
        double diferencia = 0.0;
        for (const auto& v : listaVertices)
            diferencia += std::abs(xNuevo[v] - x[v]);

        x = xNuevo;

        if (diferencia < epsilon) break;
    }

    return x;
}