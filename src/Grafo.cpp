#include "Grafo.hpp"
#include <algorithm>
#include <stdexcept>

//Constructor
Grafo::Grafo(bool dirigido)
    : _dirigido(dirigido), _numAristas(0) {}

//Agregar Vertice
void Grafo::agregarVertice(const std::string& vertice) {
    if (_adyacencia.count(vertice) == 0) {
        _adyacencia[vertice] = {};
        _gradoEntrada[vertice] = 0;
        _vertices.push_back(vertice);
    }
}

//Agregar Arista
void Grafo::agregarArista(const std::string& origen,
                           const std::string& destino,
                           double peso) {
    //Auto crear vértices por si estos no existen
    agregarVertice(origen);
    agregarVertice(destino);

    _adyacencia[origen].push_back({destino, peso});
    _gradoEntrada[destino]++;
    _numAristas++;

    if (!_dirigido) {
        _adyacencia[destino].push_back({origen, peso});
        _gradoEntrada[origen]++;
    }
}

//Eliminar Arista
void Grafo::eliminarArista(const std::string& origen,
                            const std::string& destino) {
    auto it = _adyacencia.find(origen);
    if (it == _adyacencia.end()) return;

    auto& lista = it->second;
    auto antes = lista.size();
    lista.erase(
        std::remove_if(lista.begin(), lista.end(),
            [&](const Arista& a){ return a.destino == destino; }),
        lista.end()
    );
    int eliminadas = static_cast<int>(antes - lista.size());
    _gradoEntrada[destino] -= eliminadas;
    _numAristas -= eliminadas;

    if (!_dirigido) {
        auto& listaInv = _adyacencia[destino];
        auto antesInv = listaInv.size();
        listaInv.erase(
            std::remove_if(listaInv.begin(), listaInv.end(),
                [&](const Arista& a){ return a.destino == origen; }),
            listaInv.end()
        );
        int elimInv = static_cast<int>(antesInv - listaInv.size());
        _gradoEntrada[origen] -= elimInv;
    }
}

//Existe Arista
bool Grafo::existeArista(const std::string& origen,
                          const std::string& destino) const {
    auto it = _adyacencia.find(origen);
    if (it == _adyacencia.end()) return false;
    for (const auto& a : it->second)
        if (a.destino == destino) return true;
    return false;
}

//Vecinos
const std::vector<Arista>& Grafo::vecinos(const std::string& vertice) const {
    auto it = _adyacencia.find(vertice);
    if (it == _adyacencia.end())
        throw std::invalid_argument("Vertice no existe: " + vertice);
    return it->second;
}

//Vertices
const std::vector<std::string>& Grafo::vertices() const {
    return _vertices;
}

//Consultas básicas
int Grafo::numVertices()  const { return static_cast<int>(_vertices.size()); }
int Grafo::numAristas()   const { return _numAristas; }
bool Grafo::esDirigido()  const { return _dirigido; }

int Grafo::gradoSalida(const std::string& vertice) const {
    auto it = _adyacencia.find(vertice);
    if (it == _adyacencia.end()) return 0;
    return static_cast<int>(it->second.size());
}

int Grafo::gradoEntrada(const std::string& vertice) const {
    auto it = _gradoEntrada.find(vertice);
    if (it == _gradoEntrada.end()) return 0;
    return it->second;
}