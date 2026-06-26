#include "PageRank.hpp"
#include <cmath>
#include <algorithm>


std::unordered_map<std::string, double> pageRank(
    const Grafo& grafo,
    double factorAmortiguamiento,
    int    maxIteraciones,
    double epsilon)
{
    const auto& listaVertices = grafo.vertices();
    int n = grafo.numVertices();

    if (n == 0) return {};

    //Inicialización
    std::unordered_map<std::string, double> pr, prNuevo;
    double valorInicial = 1.0 / n;
    for (const auto& v : listaVertices) {
        pr[v]      = valorInicial;
        prNuevo[v] = 0.0;
    }

    double teleporte = (1.0 - factorAmortiguamiento) / n;

    //Iteraciones
    for (int iter = 0; iter < maxIteraciones; ++iter) {

        //Se reinician los valores nuevos con el término de teletransportación
        for (const auto& v : listaVertices)
            prNuevo[v] = teleporte;

        //Para cada nodo u, se distribuye su PR entre sus vecinos de salida
        for (const auto& u : listaVertices) {
            int gradoSal = grafo.gradoSalida(u);
            if (gradoSal == 0) {
                //Se reparte su PR uniformemente a todos 
                double contribucion = factorAmortiguamiento * pr[u] / n;
                for (const auto& v : listaVertices)
                    prNuevo[v] += contribucion;
            } else {
                double contribucion = factorAmortiguamiento * pr[u] / gradoSal;
                for (const auto& arista : grafo.vecinos(u))
                    prNuevo[arista.destino] += contribucion;
            }
        }

        //Se verifica la convergencia
        double diferencia = 0.0;
        for (const auto& v : listaVertices)
            diferencia += std::abs(prNuevo[v] - pr[v]);

        pr = prNuevo;

        if (diferencia < epsilon) break;
    }

    return pr;
}