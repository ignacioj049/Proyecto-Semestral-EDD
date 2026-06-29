#ifndef BETWEENNESSCENTRALITY_HPP
#define BETWEENNESSCENTRALITY_HPP

#include "Grafo.hpp"
#include <unordered_map>
#include <string>

/**
 * @class BetweennessCentrality
 * @brief Clase estática para calcular la Centralidad de Intermediación.
 * * Implementa el Algoritmo de Ulrik Brandes (2001) para encontrar los cuellos de botella 
 * de una red en tiempo O(V * E log V) utilizando Colas de Prioridad.
 */
class BetweennessCentrality {
public:
    /**
     * @brief Calcula el Betweenness Centrality para todos los vértices del grafo.
     * * @param grafo Referencia constante al TDA Grafo.
     * @return std::unordered_map<std::string, double> Mapa asociando el ID de cada nodo con su valor de intermediación.
     */
    static std::unordered_map<std::string, double> calcular(const Grafo& grafo);
};

#endif