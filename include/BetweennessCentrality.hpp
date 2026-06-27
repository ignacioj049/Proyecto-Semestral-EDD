#ifndef BETWEENNESSCENTRALITY_HPP
#define BETWEENNESSCENTRALITY_HPP

#include "Grafo.hpp"
#include <unordered_map>
#include <string>

class BetweennessCentrality {
public:
    /**
     * Calcula la centralidad de intermediación (Betweenness Centrality) 
     * para todos los nodos del grafo utilizando el Algoritmo de Brandes.
     * * @param grafo Referencia al ADT Grafo del proyecto.
     * @return Un mapa donde la clave es el ID del nodo y el valor es su centralidad.
     */
    static std::unordered_map<std::string, double> calcular(const Grafo& grafo);
};

#endif