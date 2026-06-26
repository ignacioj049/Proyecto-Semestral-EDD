#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

struct Arista {
    std::string destino;
    double peso;
};

class Grafo {
public:
    //Constructor
    explicit Grafo(bool dirigido = false);

    //Modificación
    void agregarVertice(const std::string& vertice);
    void agregarArista(const std::string& origen,
                       const std::string& destino,
                       double peso = 1.0);
    void eliminarArista(const std::string& origen,
                        const std::string& destino);

    //Consulta
    bool existeArista(const std::string& origen,
                      const std::string& destino) const;
    const std::vector<Arista>& vecinos(const std::string& vertice) const;
    const std::vector<std::string>& vertices() const;

    int numVertices()  const;
    int numAristas()   const;
    bool esDirigido()  const;

    // grado de salida 
    int gradoSalida(const std::string& vertice) const;
    // grado de entrada
    int gradoEntrada(const std::string& vertice) const;

private:
    bool _dirigido;
    int  _numAristas;

    //lista de adyacencia
    std::unordered_map<std::string, std::vector<Arista>> _adyacencia;
    //orden de inserción de vértices
    std::vector<std::string> _vertices;
    //grados de entrada (para grafos dirigidos)
    std::unordered_map<std::string, int> _gradoEntrada;
};