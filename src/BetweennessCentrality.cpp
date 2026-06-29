#include "../include/BetweennessCentrality.hpp"
#include <queue>
#include <stack>
#include <limits>
#include <vector>
#include <iostream>

using namespace std;

/**
 * @struct NodoDistancia
 * @brief Estructura auxiliar para gestionar la Cola de Prioridad (Min-Heap) en Dijkstra.
 */
struct NodoDistancia {
    string id;
    double distancia;
    
    /**
     * @brief Sobrecarga del operador > para ordenar el Min-Heap de menor a mayor distancia.
     */
    bool operator>(const NodoDistancia& otro) const {
        return distancia > otro.distancia;
    }
};

unordered_map<string, double> BetweennessCentrality::calcular(const Grafo& grafo) {
    unordered_map<string, double> cb; // Mapa para almacenar los resultados (C_B)
    vector<string> nodos = grafo.vertices(); 
    
    // Inicialización de la centralidad en 0.0
    for (const string& v : nodos) {
        cb[v] = 0.0;
    }

    int total_nodos = nodos.size();
    // int completados = 0; // Desactivado para no ensuciar la terminal en los experimentos

    // Algoritmo de Brandes: Se ejecuta la expansión desde cada vértice 's'
    for (const string& s : nodos) {
        
        // --- CONTADOR DE PROGRESO (Comentado para experimentos limpios) ---
        /*
        completados++;
        if (completados % 50 == 0 || completados == total_nodos) {
            cout << "\r  -> Progreso Betweenness: " << completados << " / " << total_nodos 
                 << " nodos (" << (completados * 100 / total_nodos) << "%) calculados..." << flush;
        }
        */
        // ------------------------------------------------------------------

        stack<string> S; // Pila para almacenar el orden de descubrimiento (usado en backtracking)
        
        // P[w]: Lista de predecesores de 'w' en los caminos más cortos desde 's'
        unordered_map<string, vector<string>> P; 
        
        // sigma[w]: Cantidad total de caminos más cortos desde 's' hasta 'w'
        unordered_map<string, double> sigma;
        
        // d[w]: Distancia mínima descubierta desde 's' hasta 'w'
        unordered_map<string, double> d;

        // Inicialización de estructuras para la iteración actual
        for (const string& v : nodos) {
            sigma[v] = 0.0;
            d[v] = numeric_limits<double>::infinity();
        }
        
        sigma[s] = 1.0;
        d[s] = 0.0;

        // Cola de prioridad (Min-Heap) para el algoritmo de Dijkstra
        priority_queue<NodoDistancia, vector<NodoDistancia>, greater<NodoDistancia>> Q;
        Q.push({s, 0.0});

        // ==========================================
        // FASE 1: Expansión (Dijkstra Modificado)
        // ==========================================
        while (!Q.empty()) {
            NodoDistancia actual = Q.top();
            Q.pop();
            string v = actual.id;
            double dist_v = actual.distancia;

            // Optimización: Ignorar caminos subóptimos que quedaron rezagados en el Heap
            if (dist_v > d[v]) continue;

            S.push(v); // Guardar en la pila para la fase de acumulación

            auto lista_vecinos = grafo.vecinos(v); 

            for (const auto& vecino : lista_vecinos) {
                string w = vecino.destino;
                double peso = vecino.peso; 

                // Caso 1: Se descubre un camino estrictamente más corto hacia 'w'
                if (d[w] > d[v] + peso) {
                    d[w] = d[v] + peso;
                    Q.push({w, d[w]});
                    sigma[w] = 0.0; // Reiniciar contador de caminos
                    P[w].clear();   // Reiniciar predecesores
                }
                
                // Caso 2: Se descubre un camino alternativo con la misma distancia mínima
                if (d[w] == d[v] + peso) {
                    sigma[w] += sigma[v]; // Acumular la cantidad de caminos
                    P[w].push_back(v);    // Registrar a 'v' como predecesor válido
                }
            }
        }

        // ==========================================
        // FASE 2: Acumulación (Backtracking)
        // ==========================================
        unordered_map<string, double> delta; // Dependencias fraccionales
        for (const string& v : nodos) {
            delta[v] = 0.0;
        }

        // Desapilar en orden inverso al descubrimiento
        while (!S.empty()) {
            string w = S.top();
            S.pop();

            for (const string& v : P[w]) {
                // Fórmula de Brandes para propagar la dependencia hacia los predecesores
                delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            }
            // Sumar al Betweenness global (excluyendo al propio nodo origen)
            if (w != s) {
                cb[w] += delta[w];
            }
        }
    }

    // cout << "\n"; // Salto de línea para consola (Comentado)

    // Ajuste final: Si el grafo es no dirigido, cada camino se contó dos veces (ida y vuelta)
    bool red_dirigida = grafo.esDirigido(); 
    if (!red_dirigida) {
        for (auto& par : cb) {
            par.second /= 2.0;
        }
    }

    return cb;
}