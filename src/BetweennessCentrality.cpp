#include "../include/BetweennessCentrality.hpp"
#include <queue>
#include <stack>
#include <limits>
#include <vector>


using namespace std;

// Estructura auxiliar para la Cola de Prioridad (Min-Heap)
struct NodoDistancia {
    string id;
    double distancia;
    // Sobrecarga del operador > para que priority_queue funcione como MIN-Heap
    bool operator>(const NodoDistancia& otro) const {
        return distancia > otro.distancia;
    }
};

unordered_map<string, double> BetweennessCentrality::calcular(const Grafo& grafo) {
    unordered_map<string, double> cb; // Mapa para guardar el Betweenness
    
    // Obtenemos todos los vértices usando el método real de tu Grafo.hpp
    vector<string> nodos = grafo.vertices(); 
    
    // Inicializar la centralidad en 0.0 para todos
    for (const string& v : nodos) {
        cb[v] = 0.0;
    }

    int total_nodos = nodos.size();
    int completados = 0;

    // El algoritmo de Brandes ejecuta un Dijkstra desde CADA nodo del grafo
    for (const string& s : nodos) {
        
        // --- CONTADOR DE PROGRESO ---
        //completados++;  //utilizado para sumar los nodos completados
        // Imprime cada 50 nodos o al terminar. \r sobrescribe la misma línea.
        // if (completados % 50 == 0 || completados == total_nodos) {
        //     cout << "\r  -> Progreso Betweenness: " << completados << " / " << total_nodos 
        //          << " nodos (" << (completados * 100 / total_nodos) << "%) calculados..." << flush;
        // }
        // ----------------------------

        stack<string> S; // Pila para el orden de recorrido
        
        // P[w] guarda una lista de los predecesores de w en los caminos más cortos
        unordered_map<string, vector<string>> P; 
        
        // sigma[w] guarda el NUMERO de caminos más cortos desde s hasta w
        unordered_map<string, double> sigma;
        
        // d[w] guarda la distancia mínima desde s hasta w
        unordered_map<string, double> d;

        for (const string& v : nodos) {
            sigma[v] = 0.0;
            d[v] = numeric_limits<double>::infinity();
        }
        
        sigma[s] = 1.0;
        d[s] = 0.0;

        // Cola de prioridad (Min-Heap) equivalente a la de tu clase
        priority_queue<NodoDistancia, vector<NodoDistancia>, greater<NodoDistancia>> Q;
        Q.push({s, 0.0});

        // 1. Fase de expansión (Dijkstra modificado para contar caminos)
        while (!Q.empty()) {
            NodoDistancia actual = Q.top();
            Q.pop();
            string v = actual.id;
            double dist_v = actual.distancia;

            // Si encontramos una distancia peor en la cola, la ignoramos
            if (dist_v > d[v]) continue;

            S.push(v);

            // Obtenemos vecinos usando el método real de tu Grafo.hpp
            auto lista_vecinos = grafo.vecinos(v); 

            for (const auto& vecino : lista_vecinos) {
                // Usamos los atributos de tu struct Arista
                string w = vecino.destino;
                double peso = vecino.peso; 

                // Camino más corto encontrado
                if (d[w] > d[v] + peso) {
                    d[w] = d[v] + peso;
                    Q.push({w, d[w]});
                    sigma[w] = 0.0;
                    P[w].clear();
                }
                
                // Si encontramos OTRO camino igual de corto, sumamos los caminos
                if (d[w] == d[v] + peso) {
                    sigma[w] += sigma[v];
                    P[w].push_back(v);
                }
            }
        }

        // 2. Fase de acumulación (Backtracking por la pila)
        unordered_map<string, double> delta;
        for (const string& v : nodos) {
            delta[v] = 0.0;
        }

        while (!S.empty()) {
            string w = S.top();
            S.pop();

            for (const string& v : P[w]) {
                // Ecuación de Brandes para acumular la dependencia
                delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            }
            if (w != s) {
                cb[w] += delta[w];
            }
        }
    }

    //cout << "\n"; // Salto de línea para que el Top-5 se imprima abajo del contador

    // IMPORTANTE: Si tu red es NO DIRIGIDA (como IMDb), cada camino se contó dos veces.
    bool red_dirigida = grafo.esDirigido(); 
    if (!red_dirigida) {
        for (auto& par : cb) {
            par.second /= 2.0;
        }
    }

    return cb;
}