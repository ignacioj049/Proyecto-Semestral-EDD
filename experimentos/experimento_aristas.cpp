#include "Grafo.hpp"
#include "Lectordataset.hpp"
#include "Pagerank.hpp"
#include "EigenvectorCentralidad.hpp"
#include "Degreecentrality.hpp"
#include "Clusteringcoefficient.hpp"
#include "BetweennessCentrality.hpp"
#include "ClosenessCentrality.hpp"
#include "AverageShortestPath.hpp"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <unordered_map>


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

double promedioMapa(const std::unordered_map<std::string, double>& mapa) {
    if (mapa.empty()) return 0.0;
    double suma = 0.0;
    for (const auto& [k, v] : mapa) suma += v;
    return suma / mapa.size();
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

//Experimento 1 de tiempo de ejecución, además de la medicion de la memoria
void experimentoMemoria(const Grafo& grafo, const std::string& nombreDataset) {
    size_t bytes = grafo.memoriaBytes();
    double kb = bytes / 1024.0;
    double mb = kb / 1024.0;
    std::cout << "MEMORIA: " << nombreDataset << "\n";
    std::cout << "  Vertices:          " << grafo.numVertices() << "\n";
    std::cout << "  Aristas:           " << grafo.numAristas()  << "\n";
    std::cout << "  Memoria estimada:  " << bytes << " bytes";
    std::cout << "  (" << std::fixed << std::setprecision(2) << kb << " KB";
    std::cout << " / " << mb << " MB)\n";
    std::cout << "  Tipo:              " << (grafo.esDirigido() ? "Dirigido" : "No dirigido") << "\n";
}
void experimentoTiempos(const Grafo& grafo, const std::string& nombreDataset) {
    std::cout << "TIEMPOS: " << nombreDataset
              << "  (" << grafo.numVertices() << " vertices, "
              << grafo.numAristas() << " aristas)\n";
              std::cout << std::left
              << std::setw(30) << "Metrica"
              << std::setw(18) << "Promedio (ms)"
              << "Varianza (ms^2)\n";
    std::cout << std::string(66, '-') << "\n";
 
    auto imprimir = [](const std::string& nombre, double prom, double var) {
        std::cout << std::left  << std::setw(30) << nombre
                  << std::fixed << std::setprecision(4)
                  << std::setw(18) << prom << var << "\n";
    };
    //sobre las 7 métricas utilizadas
    auto [prP, prV] = medirTiempo([&]{ pageRank(grafo); });
    imprimir("PageRank", prP, prV);
 
    auto [dcP, dcV] = medirTiempo([&]{ degreeCentrality(grafo); });
    imprimir("Degree Centrality", dcP, dcV);
 
    auto [clP, clV] = medirTiempo([&]{ ClosenessCentrality::calcular(grafo); });
    imprimir("Closeness Centrality", clP, clV);
 
    auto [bcP, bcV] = medirTiempo([&]{ BetweennessCentrality::calcular(grafo); });
    imprimir("Betweenness Centrality", bcP, bcV);
 
    auto [apP, apV] = medirTiempo([&]{ AverageShortestPath::calcular(grafo); });
    imprimir("Avg Shortest Path", apP, apV);
 
    auto [ecP, ecV] = medirTiempo([&]{ eigenvectorCentralidad(grafo); });
    imprimir("Eigenvector Centrality", ecP, ecV);
 
    auto [ccP, ccV] = medirTiempo([&]{ clusteringCoefficient(grafo); });
    imprimir("Clustering Coefficient", ccP, ccV);
}


struct EstadoRed {  //struct que guarda el estado de todas las métricas en un cierto momento (para así comparar el antes y despues de modificar las aristas)
    double avgPageRank;
    double avgDegreeOut;
    double avgCloseness;
    double avgBetweenness;
    double avgShortestPath;
    double avgEigenvector;
    double clusteringGlobal;
};

EstadoRed calcularEstado(const Grafo& grafo) {
    EstadoRed e;
    e.avgPageRank     = promedioMapa(pageRank(grafo));
    e.avgDegreeOut    = promedioMapa(degreeCentrality(grafo).outDegree);
    e.avgCloseness    = promedioMapa(ClosenessCentrality::calcular(grafo));
    e.avgBetweenness  = promedioMapa(BetweennessCentrality::calcular(grafo));
    e.avgShortestPath = AverageShortestPath::calcular(grafo);
    e.avgEigenvector  = promedioMapa(eigenvectorCentralidad(grafo));
    e.clusteringGlobal= clusteringCoefficient(grafo).global;
    return e;
}
 
void imprimirEstado(const std::string& etiqueta, const EstadoRed& e) {
    std::cout << std::left << std::setw(38) << etiqueta
              << std::fixed << std::setprecision(5)
              << " PR="   << e.avgPageRank
              << " Deg="  << e.avgDegreeOut
              << " Cl="   << e.avgCloseness
              << " BC="   << e.avgBetweenness
              << " ASP="  << e.avgShortestPath
              << " EC="   << e.avgEigenvector
              << " CC="   << e.clusteringGlobal
              << "\n";
}


//Experimento 2, para ver el impacto de añadir y quitar aristas
void experimentoAristas(Grafo grafo, const std::string& nombreDataset) {
    std::cout << "Experimento: Impacto de colocar o quitar aristas\n";

    const auto& verts = grafo.vertices();
    int n = grafo.numVertices();
    if (n < 4) {
        std::cout << "Grafo demasiado pequeño para el experimento.\n";
        return;
    }
    //Estado base sin modificaciones
    EstadoRed base = calcularEstado(grafo);
    imprimirEstado("[BASE]", base);

    //Casos de prueba para añadir aristas 
    // Se prueban 3 posiciones distintas:
    //Entre los nodos centrales (que corresponderían a los primeros de la lista)
    //Entre nodos "periféricos" (que corresponderían a los últimos de la lista)
    //Entre un nodo central y uno periférico
    struct CasoArista { std::string etiqueta, origen, destino; };
    std::vector<CasoArista> casosAgregar = {
        {"Agregar arista Central-Central",     
        verts[0],   verts[1]  },
        {"Agegar arista Periferia-Periferia", 
        verts[n-1], verts[n-2]},
        {"Agregar Central-Periferia",   
        verts[0],   verts[n-1]},
    };

    std::cout << "\n---Agregar aristas---\n";
    for (const auto& caso : casosAgregar) {
        //Saltar si ya existe
        if (grafo.existeArista(caso.origen, caso.destino)) {
            std::cout << caso.etiqueta << ": ya existe, pues se omite.\n";
            continue;
        }
       grafo.agregarArista(caso.origen, caso.destino);
        EstadoRed res = calcularEstado(grafo);
        imprimirEstado(caso.etiqueta, res);
        grafo.eliminarArista(caso.origen, caso.destino); // revertir
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
        const auto& vec = grafo.vecinos(origen);
        if (vec.empty()) {
            std::cout << "Quitar " << etiqueta << ": nodo sin aristas, pues se omite.\n";
            continue;
        }
        std::string destino = vec[0].destino;
        double peso = vec[0].peso;

        grafo.eliminarArista(origen, destino);
        EstadoRed res = calcularEstado(grafo);
        imprimirEstado("Quitar " + etiqueta + " (" + origen + "-" + destino + ")", res);
        grafo.agregarArista(origen, destino, peso); // revertir
    }
}

//main que ejecuta ambos experimentos sobre los 3 dataset
int main(int argc, char* argv[]) {
    std::string base = (argc > 1) ? argv[1] : "./datasets";
 
    std::cout << "=== Proyecto Semestral: Metricas de Centralidad en Redes ===\n";
    std::cout << "Ruta de datasets: " << base << "\n";
    //Trade2000
    std::vector<std::string> anios = {"2000", "2005", "2010", "2015", "2018"};
    for (const auto &anio : anios)
    {
        try
        {
            Grafo trade = leerTrade(base + "/trade/" + anio + ".net", anio);
            experimentoMemoria(trade, "Trade " + anio);
            experimentoTiempos(trade, "Trade " + anio);
            experimentoAristas(trade, "Trade " + anio);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR] Trade " << anio << ": " << e.what() << "\n";
        }
    }

    //Yeast PPI
    try {
        Grafo yeast = leerYeast(base + "/yeast/yeast.edgelist");
        experimentoMemoria(yeast, "Yeast PPI");
        experimentoTiempos(yeast, "Yeast PPI");
        experimentoAristas(yeast, "Yeast PPI");
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Yeast: " << e.what() << "\n";
    }   

    //imdb
    try {
        Grafo imdb = leerIMDb(base + "/imdb/imdb_edgelist.csv");
        experimentoMemoria(imdb, "IMDb Actors");
        experimentoTiempos(imdb, "IMDb Actors");
        experimentoAristas(imdb, "IMDb Actors");
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] IMDb: " << e.what() << "\n";
    }
 
    return 0;
}