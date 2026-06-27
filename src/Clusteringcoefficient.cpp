#include "Clusteringcoefficient.hpp"
#include <unordered_set>

// Coeficiente de Clustering Local

ClusteringResult clusteringCoefficient(const Grafo& grafo) {
    ClusteringResult resultado;
    resultado.global = 0.0;

    const auto& listaVertices = grafo.vertices();
    int n = grafo.numVertices();

    if (n == 0) return resultado;

    for (const auto& v : listaVertices) {
        const auto& vecinosV = grafo.vecinos(v);
        int k = static_cast<int>(vecinosV.size()); // grado de salida

        if (k < 2) {
            resultado.local[v] = 0.0;
            continue;
        }

        // Construimos un set con los vecinos de v para búsqueda O(1)
        std::unordered_set<std::string> setVecinos;
        setVecinos.reserve(k);
        for (const auto& arista : vecinosV)
            setVecinos.insert(arista.destino);

        // Contamos cuántas aristas existen entre los vecinos de v
        int aristasEntreVecinos = 0;
        for (const auto& arista : vecinosV) {
            const std::string& u = arista.destino;
            // Revisamos cada vecino de u para ver si también es vecino de v
            for (const auto& aristaU : grafo.vecinos(u)) {
                if (setVecinos.count(aristaU.destino))
                    aristasEntreVecinos++;
            }
        }

        // En un grafo no dirigido cada arista se cuenta dos veces;
        // en uno dirigido se cuentan los arcos, por lo que el denominador cambia.
        double posibles;
        if (grafo.esDirigido()) {
            // Pares ordenados: k*(k-1)
            posibles = static_cast<double>(k) * (k - 1);
        } else {
            // aristasEntreVecinos ya se contó dos veces (u->w y w->u)
            posibles = static_cast<double>(k) * (k - 1);
            // se divide igual: numerador doble / denominador doble -> correcto
        }

        resultado.local[v] = static_cast<double>(aristasEntreVecinos) / posibles;
        resultado.global   += resultado.local[v];
    }

    resultado.global /= static_cast<double>(n);
    return resultado;
}