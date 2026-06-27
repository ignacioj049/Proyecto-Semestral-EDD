#include "../include/ClosenessCentrality.hpp"
#include <queue>
#include <limits>


using namespace std;

// Estructura auxiliar para la Cola de Prioridad (Min-Heap)
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

    // Si el grafo está vacío o tiene 1 nodo, la cercanía es 0
    if (n <= 1) {
        for (const string& v : nodos) closeness[v] = 0.0;
        return closeness;
    }

    int completados = 0;

    // Ejecutamos Dijkstra desde CADA nodo del grafo
    for (const string& origen : nodos) {
        
        // --- Contador de progreso en consola ---
        //completados++;
        // if (completados % 50 == 0 || completados == n) {
        //     cout << "\r  -> Progreso Closeness: " << completados << " / " << n 
        //          << " nodos (" << (completados * 100 / n) << "%) calculados..." << flush;
        // }
        // ---------------------------------------

        unordered_map<string, double> dist;
        for (const string& v : nodos) {
            dist[v] = numeric_limits<double>::infinity(); // Distancia inicial infinita
        }
        dist[origen] = 0.0;

        priority_queue<NodoDist, vector<NodoDist>, greater<NodoDist>> pq;
        pq.push({origen, 0.0});

        // Algoritmo de Dijkstra estándar
        while (!pq.empty()) {
            NodoDist actual = pq.top();
            pq.pop();

            string u = actual.id;
            double d_u = actual.distancia;

            // Si encontramos un camino peor guardado en la cola, lo ignoramos
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

        // Calcular la suma de las distancias hacia todos los demás nodos
        double suma_distancias = 0.0;
        int nodos_alcanzables = 0;
        
        for (const auto& par : dist) {
            if (par.first != origen && par.second != numeric_limits<double>::infinity()) {
                suma_distancias += par.second;
                nodos_alcanzables++;
            }
        }

        // Evitar división por cero. 
        // Se usa la fórmula de Wasserman y Faust adaptada para grafos que podrían ser disconexos.
        if (suma_distancias > 0) {
            closeness[origen] = (double)nodos_alcanzables / suma_distancias;
        } else {
            closeness[origen] = 0.0; // Nodo aislado
        }
    }
    
    // cout << "\n"; // Salto de línea al terminar el progreso
    return closeness;
}