#include "../include/ClosenessCentrality.hpp"
#include <queue>
#include <limits>
#include <iostream>

using namespace std;

/**
 * @struct NodoDist
 * @brief Estructura auxiliar para manejar el Min-Heap en la búsqueda de caminos mínimos.
 */
struct NodoDist {
    string id;
    double distancia;
    bool operator>(const NodoDist& otro) const {
        return distancia > otro.distancia;
    }
};

unordered_map<string, double> ClosenessCentrality::calcular(const Grafo& grafo) {
    unordered_map<string, double> closeness;
    vector<string> nodos = grafo.vertices();
    int n = nodos.size();

    // Caso base: Si el grafo carece de estructura suficiente, la cercanía es nula.
    if (n <= 1) {
        for (const string& v : nodos) closeness[v] = 0.0;
        return closeness;
    }

    // int completados = 0; // Desactivado para limpieza de terminal

    // Se calcula el árbol de caminos mínimos (SSSP) desde cada nodo del grafo
    for (const string& origen : nodos) {
        
        // --- Contador de progreso en consola (Comentado) ---
        /*
        completados++;
        if (completados % 50 == 0 || completados == n) {
            cout << "\r  -> Progreso Closeness: " << completados << " / " << n 
                 << " nodos (" << (completados * 100 / n) << "%) calculados..." << flush;
        }
        */
        // ---------------------------------------------------

        unordered_map<string, double> dist;
        for (const string& v : nodos) {
            dist[v] = numeric_limits<double>::infinity(); // Distancia inicial infinita
        }
        dist[origen] = 0.0;

        priority_queue<NodoDist, vector<NodoDist>, greater<NodoDist>> pq;
        pq.push({origen, 0.0});

        // ==========================================
        // Algoritmo de Dijkstra Estándar
        // ==========================================
        while (!pq.empty()) {
            NodoDist actual = pq.top();
            pq.pop();

            string u = actual.id;
            double d_u = actual.distancia;

            // Ignorar nodos desactualizados en la cola
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

        // ==========================================
        // Cálculo de Métrica (Wasserman y Faust)
        // ==========================================
        double suma_distancias = 0.0;
        int nodos_alcanzables = 0;
        
        // Sumarizar las distancias descubiertas
        for (const auto& par : dist) {
            // Se excluye al propio origen y a los nodos inalcanzables (infinito)
            if (par.first != origen && par.second != numeric_limits<double>::infinity()) {
                suma_distancias += par.second;
                nodos_alcanzables++;
            }
        }

        // Aplicación de la variante para grafos disconexos.
        // Si nodos_alcanzables es mayor a 0, se pondera por la fracción de la red alcanzada.
        if (suma_distancias > 0) {
            closeness[origen] = (double)nodos_alcanzables / suma_distancias;
        } else {
            closeness[origen] = 0.0; // El nodo está completamente aislado
        }
    }
    
    // cout << "\n"; // Salto de línea (Comentado)
    return closeness;
}