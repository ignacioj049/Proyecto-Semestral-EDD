#ifndef AVERAGESHORTESTPATH_HPP
#define AVERAGESHORTESTPATH_HPP

#include "Grafo.hpp"

/**a
 * @class AverageShortestPath
 * @brief Clase estática para calcular la Longitud Promedio del Camino Más Corto.
 * * Medida de centralidad global que evalúa qué tan "compacta" (small-world) es la red,
 * promediando las distancias de todos los caminos válidos existentes.
 */
class AverageShortestPath {
public:
    /**
     * @brief Calcula el promedio global de caminos mínimos de la red.
     * * @param grafo Referencia constante al TDA Grafo.
     * @return double Promedio matemático de las distancias descubiertas.
     */
    static double calcular(const Grafo& grafo);
};

#endif