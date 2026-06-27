#ifndef CLOSENESSCENTRALITY_HPP
#define CLOSENESSCENTRALITY_HPP

#include "Grafo.hpp"
#include <unordered_map>
#include <string>

class ClosenessCentrality {
public:
    /**
     * Calcula la centralidad de cercanía (Closeness Centrality).
     * @param grafo Referencia al ADT Grafo.
     * @return Mapa con el ID del nodo y su centralidad de cercanía.
     */
    static std::unordered_map<std::string, double> calcular(const Grafo& grafo);
};

#endif