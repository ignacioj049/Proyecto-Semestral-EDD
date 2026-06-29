#include "../include/AverageShortestPath.hpp"
#include <queue>
#include <limits>
#include <iostream>
#include <unordered_map>

using namespace std;

/**
 * @struct NodoDistASP
 * @brief Estructura de soporte para instanciar la Cola de Prioridad en Dijkstra.
 */
struct NodoDistASP {
    string id;
    double distancia;
    
    /**
     * @brief Sobrecarga del operador > para ordenar el Min-Heap de menor a mayor distancia.
     */
    bool operator>(const NodoDistASP& otro) const {
        return distancia > otro.distancia;
    }
};

double AverageShortestPath::calcular(const Grafo& grafo) {
    vector<string> nodos = grafo.vertices();
    int n = nodos.size();

    // Validación estructural: Redes triviales no poseen longitud de camino
    if (n <= 1) return 0.0;

    double suma_total_distancias = 0.0;
    long long cantidad_caminos_validos = 0;
    
    // int completados = 0; // Desactivado para no ensuciar el output de varianza en la terminal

    // Iteración global: Construir el mapa de distancias desde la totalidad de los vértices
    for (const string& origen : nodos) {
        
        // --- Contador de progreso en consola (Comentado) ---
        /*
        completados++;
        if (completados % 50 == 0 || completados == n) {
            cout << "\r  -> Progreso Average SP: " << completados << " / " << n 
                 << " nodos (" << (completados * 100 / n) << "%) calculados..." << flush;
        }
        */
        // ---------------------------------------------------

        unordered_map<string, double> dist;
        for (const string& v : nodos) {
            dist[v] = numeric_limits<double>::infinity();
        }
        dist[origen] = 0.0;

        priority_queue<NodoDistASP, vector<NodoDistASP>, greater<NodoDistASP>> pq;
        pq.push({origen, 0.0});

        
        // Algoritmo de Dijkstra para Caminos Mínimos
        
        while (!pq.empty()) {
            NodoDistASP actual = pq.top();
            pq.pop();

            string u = actual.id;
            double d_u = actual.distancia;

            // Filtrado de caminos subóptimos guardados en la cola
            if (d_u > dist[u]) continue;

            auto lista_vecinos = grafo.vecinos(u);
            for (const auto& arista : lista_vecinos) {
                string v = arista.destino;
                double peso = arista.peso;

                // Relajación de la arista
                if (dist[u] + peso < dist[v]) {
                    dist[v] = dist[u] + peso;
                    pq.push({v, dist[v]});
                }
            }
        }

        
        // Sumarización Global (Ignorando disconexiones)
       
        for (const auto& par : dist) {
            // Restringimos la métrica únicamente a los pares que poseen un camino matemáticamente válido.
            // Esto evita que componentes aislados (distancia infinita) corrompan el promedio global.
            if (par.first != origen && par.second != numeric_limits<double>::infinity()) {
                suma_total_distancias += par.second;
                cantidad_caminos_validos++;
            }
        }
    }

    // cout << "\n"; // Salto de línea (Comentado)

    // Prevención de división por cero en grafos completamente vacíos de aristas
    if (cantidad_caminos_validos == 0) {
        return 0.0;
    }

    // Retorno de la métrica L(G)
    return suma_total_distancias / cantidad_caminos_validos;
}