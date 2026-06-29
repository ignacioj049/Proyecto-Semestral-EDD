#ifndef CLOSENESSCENTRALITY_HPP
#define CLOSENESSCENTRALITY_HPP

#include "Grafo.hpp"
#include <unordered_map>
#include <string>

/**
 * @class ClosenessCentrality
 * @brief Clase estática para calcular la Centralidad de Cercanía.
 * * Utiliza el Algoritmo de Dijkstra iterativo. Incorpora la variante de Wasserman y Faust 
 * para garantizar resultados matemáticamente válidos en grafos disconexos.
 */
class ClosenessCentrality {
public:
    /**
     * @brief Calcula la eficiencia de comunicación de cada nodo hacia el resto de la red.
     * * @param grafo Referencia constante al TDA Grafo.
     * @return std::unordered_map<std::string, double> Mapa con el ID del nodo y su valor de cercanía.
     */
    static std::unordered_map<std::string, double> calcular(const Grafo& grafo);
};

#endif