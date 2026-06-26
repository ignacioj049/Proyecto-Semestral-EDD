#include "Grafo.hpp"
#include "PageRank.hpp"
#include "EigenvectorCentralidad.hpp"


#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <chrono>


//Calcula promedio de un vector
double promedio(const std::vector<double>& v) {
    if (v.empty()) return 0.0;
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

//Calcula varianza de un vector
double varianza(const std::vector<double>& v) {
    if (v.size() < 2) return 0.0;
    double prom = promedio(v);
    double suma = 0.0;
    for (double x : v) suma += (x - prom) * (x - prom);
    return suma / (v.size() - 1); // varianza muestral
}

//Imprime el top N de un mapa de centralidad
void imprimirTop(
    const std::unordered_map<std::string, double>& centralidad,
    const std::string& nombreMetrica,
    int topN = 5)
{
    std::vector<std::pair<std::string, double>> lista(
        centralidad.begin(), centralidad.end());
    std::sort(lista.begin(), lista.end(),
        [](const auto& a, const auto& b){ return a.second > b.second; });

    std::cout << "\n  Top-" << topN << " " << nombreMetrica << ":\n";
    for (int i = 0; i < std::min(topN, (int)lista.size()); ++i) {
        std::cout << "    " << std::setw(3) << i+1 << ". "
                  << std::left << std::setw(20) << lista[i].first
                  << std::fixed << std::setprecision(6) << lista[i].second
                  << "\n";
    }
}

//Se mide el tiempo de ejecución de una métrica N veces
//Retorna el promedio y variansa en ms
template<typename Func>
std::pair<double,double> medirTiempo(Func f, int repeticiones = 10) {
    std::vector<double> tiempos;
    tiempos.reserve(repeticiones);

    for (int i = 0; i < repeticiones; ++i) {
        auto inicio = std::chrono::high_resolution_clock::now();
        f();
        auto fin = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(fin - inicio).count();
        tiempos.push_back(ms);
    }
    return { promedio(tiempos), varianza(tiempos) };
}

//Experimento para ver el impacto de colocar o quitar una arista

struct ResultadoMetrica {
    double promedioPageRank;      // promedio de PR de todos los nodos
    double promedioEigenvector;   // promedio de EC de todos los nodos
    //Luego seguirían las demás métricas 
    // double avgShortestPath;
    // double avgDegree;
    // double avgCloseness;
    // double avgBetweenness;
    // double avgClustering;
};

double promedioMapa(const std::unordered_map<std::string, double>& mapa) {
    if (mapa.empty()) return 0.0;
    double suma = 0.0;
    for (const auto& [k, v] : mapa) suma += v;
    return suma / mapa.size();
}

ResultadoMetrica calcularMetricas(const Grafo& grafo) {
    ResultadoMetrica res;
    res.promedioPageRank    = promedioMapa(pageRank(grafo));
    res.promedioEigenvector = promedioMapa(eigenvectorCentralidad(grafo));
    return res;
}

void imprimirResultado(const std::string& etiqueta,
                       const ResultadoMetrica& res) {
    std::cout << std::left << std::setw(40) << etiqueta
              << "PR_avg=" << std::fixed << std::setprecision(6)
              << res.promedioPageRank
              << "  EC_avg=" << res.promedioEigenvector
              << "\n";
}

void experimentoAristas(Grafo grafo) {
    std::cout << "Experimento: Impacto de colocar o quitar aristas\n";

    //Estado base
    ResultadoMetrica base = calcularMetricas(grafo);
    imprimirResultado("Estado base", base);

    const auto& verts = grafo.vertices();
    int n = grafo.numVertices();
    if (n < 4) {
        std::cout << "Grafo demasiado pequeño para el experimento.\n";
        return;
    }

    //Casos de prueba para añadir aristas 
    // Se prueban 3 posiciones distintas:
    //Entre los nodos centrales (que corresponderían a los primeros de la lista)
    //Entre nodos "periféricos" (que corresponderían a los últimos de la lista)
    //Entre un nodo central y uno periférico
    struct CasoArista {
        std::string etiqueta;
        std::string origen;
        std::string destino;
    };

    std::vector<CasoArista> casosAgregar = {
        {"Agregar arista central-central",
         verts[0], verts[1]},
        {"Agregar arista periferia-periferia",
         verts[n-1], verts[n-2]},
        {"Agregar arista central-periferia",
         verts[0], verts[n-1]},
    };

    std::cout << "\n---Colocar aristas---\n";
    for (const auto& caso : casosAgregar) {
        //Saltar si ya existe
        if (grafo.existeArista(caso.origen, caso.destino)) {
            std::cout << caso.etiqueta << ": ya existe, pues se omite.\n";
            continue;
        }
        grafo.agregarArista(caso.origen, caso.destino);
        ResultadoMetrica res = calcularMetricas(grafo);
        imprimirResultado(caso.etiqueta, res);
        grafo.eliminarArista(caso.origen, caso.destino); //revertir
    }

    //Casos de prueba para quiqtar aristas
    // Buscar 3 aristas existentes en distintas zonas del grafo
    std::cout << "\n---Quitar aristas---\n";

    //Arista 1: Primera arista del primer vértice
    //Arista 2: Primera arista del vértice del medio
    //Arista 3: Primera arista del último vértice
    std::vector<std::pair<int,std::string>> candidatos = {
        {0,         "primera arista (zona central)"},
        {n/2,       "arista del medio"},
        {n-1,       "ultima arista (periferia)"},
    };

    for (const auto& [idx, etiqueta] : candidatos) {
        const std::string& origen = verts[idx];
        const auto& vecinos = grafo.vecinos(origen);
        if (vecinos.empty()) {
            std::cout << "Quitar " << etiqueta << ": nodo sin aristas, pues se omite.\n";
            continue;
        }
        std::string destino = vecinos[0].destino;
        double peso = vecinos[0].peso;

        grafo.eliminarArista(origen, destino);
        ResultadoMetrica res = calcularMetricas(grafo);
        imprimirResultado("Quitar " + etiqueta + " (" + origen + "-" + destino + ")", res);
        grafo.agregarArista(origen, destino, peso); // revertir
    }
}

// Experimento de tiempo (con 10 repeticiones por métrica)
void experimentoTiempos(const Grafo& grafo) {
    std::cout << "Experimento: Tiempos de ejecucion (con 10 repeticiones)\n";
    std::cout << std::left
              << std::setw(30) << "Metrica"
              << std::setw(15) << "Promedio"
              << std::setw(15) << "Varianza"
              << "\n";
    std::cout << std::string(60, '-') << "\n";

    auto [prProm, prVar] = medirTiempo([&]{ pageRank(grafo); });
    std::cout << std::setw(30) << "PageRank"
              << std::setw(15) << std::fixed << std::setprecision(4) << prProm
              << std::setw(15) << prVar << "\n";

    auto [ecProm, ecVar] = medirTiempo([&]{ eigenvectorCentralidad(grafo); });
    std::cout << std::setw(30) << "Eigenvector Centrality"
              << std::setw(15) << ecProm
              << std::setw(15) << ecVar << "\n";

    //Luego agregar aquí las métricas restantes
}
