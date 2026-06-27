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
#include <algorithm>

//Imprime el top-N de un mapa de centralidad
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
                  << std::left << std::setw(25) << lista[i].first
                  << std::fixed << std::setprecision(6)
                  << lista[i].second << "\n";
    }
}

void analizarGrafo(const Grafo& grafo, const std::string& nombre) {
    std::cout << "Dataset: " << nombre << "\n";
    std::cout << "  Vertices: " << grafo.numVertices()
              << "  Aristas: "  << grafo.numAristas()
              << "  Dirigido: " << (grafo.esDirigido() ? "Si" : "No") << "\n";

    auto pr = pageRank(grafo);
    imprimirTop(pr, "PageRank");

    auto ec = eigenvectorCentralidad(grafo);
    imprimirTop(ec, "Eigenvector Centrality");

    auto dc = degreeCentrality(grafo);
    imprimirTop(dc.outDegree, "Degree Centrality (out)");

    auto cc = clusteringCoefficient(grafo);
    std::cout << "Clustering global: " << cc.global << "\n";
    imprimirTop(cc.local, "Clustering Coefficient");

    std::cout << "Calculando Betweenness Centrality (esto puede tomar un momento)...\n";
    auto bc = BetweennessCentrality::calcular(grafo);
    imprimirTop(bc, "Betweenness Centrality");

    std::cout << "Calculando Closeness Centrality...\n";
    auto clc = ClosenessCentrality::calcular(grafo);
    imprimirTop(clc, "Closeness Centrality");

    std::cout << "Calculando Average Shortest Path...\n";
    double asp = AverageShortestPath::calcular(grafo);
    std::cout << "  -> Average Shortest Path Global: " << asp << "\n";
}

int main(int argc, char* argv[]) {
    //Ruta base de datasets 
    //Por defecto busca en ../datasets/ 
    std::string base = (argc > 1) ? argv[1] : "../datasets";

    std::cout << "===Metricas de Centralidad en Redes===\n";
    std::cout << "Ruta de datasets: " << base << "\n";

    //Yeast
    try {
        Grafo yeast = leerYeast(base + "/yeast/yeast.edgelist");
        analizarGrafo(yeast, "Yeast PPI");
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Yeast: " << e.what() << "\n";
    }

    //IMDb
    try {
        Grafo imdb = leerIMDb(base + "/imdb/imdb_edgelist.csv");
        analizarGrafo(imdb, "IMDb Actors");
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] IMDb: " << e.what() << "\n";
    }

    //Trade (todos los años)
    std::vector<std::string> anios = {"2000","2005","2010","2015","2018"};
    for (const auto& anio : anios) {
        try {
            Grafo trade = leerTrade(
                base + "/trade/" + anio + ".net", anio);
            analizarGrafo(trade, "Trade " + anio);
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Trade " << anio << ": " << e.what() << "\n";
        }
    }

    return 0;
}