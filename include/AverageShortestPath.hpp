#ifndef AVERAGESHORTESTPATH_HPP
#define AVERAGESHORTESTPATH_HPP

#include "Grafo.hpp"

class AverageShortestPath {
public:
    /**
     * Calcula la longitud promedio de los caminos más cortos de toda la red.
     * @param grafo Referencia al ADT Grafo.
     * @return Un número (double) que representa el promedio de la red.
     */
    static double calcular(const Grafo& grafo);
};

#endif