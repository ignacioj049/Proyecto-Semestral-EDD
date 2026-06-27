#include "../include/AverageShortestPath.hpp"
#include <queue>
#include <limits>
#include <unordered_map>

using namespace std;

// Estructura auxiliar para la Cola de Prioridad (Min-Heap)
struct NodoDistASP {
    string id;
    double distancia;
    bool operator>(const NodoDistASP& otro) const {
        return distancia > otro.distancia;
    }
};

double AverageShortestPath::calcular(const Grafo& grafo) {
    vector<string> nodos = grafo.vertices();
    int n = nodos.size();

    // Si la red tiene 1 nodo o está vacía, no hay caminos
    if (n <= 1) return 0.0;

    double suma_total_distancias = 0.0;
    long long cantidad_caminos_validos = 0;
    int completados = 0;

    // Ejecutamos Dijkstra desde CADA nodo del grafo hacia todos los demás
    for (const string& origen : nodos) {
        
        // --- Contador de progreso en consola ---
        // completados++;
        // if (completados % 50 == 0 || completados == n) {
        //     cout << "\r  -> Progreso Average SP: " << completados << " / " << n 
        //          << " nodos (" << (completados * 100 / n) << "%) calculados..." << flush;
        // }
        // ---------------------------------------

        unordered_map<string, double> dist;
        for (const string& v : nodos) {
            dist[v] = numeric_limits<double>::infinity();
        }
        dist[origen] = 0.0;

        priority_queue<NodoDistASP, vector<NodoDistASP>, greater<NodoDistASP>> pq;
        pq.push({origen, 0.0});

        while (!pq.empty()) {
            NodoDistASP actual = pq.top();
            pq.pop();

            string u = actual.id;
            double d_u = actual.distancia;

            if (d_u > dist[u]) continue;

            auto lista_vecinos = grafo.vecinos(u);
            for (const auto& arista : lista_vecinos) {
                string v = arista.destino;
                double peso = arista.peso;

                if (dist[u] + peso < dist[v]) {
                    dist[v] = dist[u] + peso;
                    pq.push({v, dist[v]});
                }
            }
        }

        // Al terminar el Dijkstra para 'origen', sumamos todas sus distancias
        for (const auto& par : dist) {
            // Solo contamos los caminos hacia OTROS nodos que sí pudimos alcanzar
            if (par.first != origen && par.second != numeric_limits<double>::infinity()) {
                suma_total_distancias += par.second;
                cantidad_caminos_validos++;
            }
        }
    }

    // cout << "\n"; // Salto de línea al terminar el progreso

    // Si el grafo no tiene aristas, devolvemos 0 para evitar dividir por cero
    if (cantidad_caminos_validos == 0) {
        return 0.0;
    }

    // Retornamos el promedio: (suma de todas las distancias) / (cantidad de caminos posibles)
    return suma_total_distancias / cantidad_caminos_validos;
}