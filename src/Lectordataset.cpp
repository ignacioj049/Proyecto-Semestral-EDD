#include "Lectordataset.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <string>

//Quitar los espacios y \r al inicio y fin 
static std::string trim(const std::string& s) {
    size_t ini = s.find_first_not_of(" \t\r\n");
    if (ini == std::string::npos) return "";
    size_t fin = s.find_last_not_of(" \t\r\n");
    return s.substr(ini, fin - ini + 1);
}

//leerIMDb
//Formato: CSV con encabezado "From,To,Strength"
//Un grafo no dirigido, peso = Strength
Grafo leerIMDb(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open())
        throw std::runtime_error("No se pudo abrir: " + rutaArchivo);

    Grafo grafo(false); //no dirigido
    std::string linea;

    //Saltar encabezado
    std::getline(archivo, linea);

    int aristasLeidas = 0;
    while (std::getline(archivo, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string origen, destino, pesoStr;

        if (!std::getline(ss, origen,   ',')) continue;
        if (!std::getline(ss, destino,  ',')) continue;
        if (!std::getline(ss, pesoStr,  ',')) pesoStr = "1";

        origen   = trim(origen);
        destino  = trim(destino);
        pesoStr  = trim(pesoStr);

        if (origen.empty() || destino.empty()) continue;

        double peso = 1.0;
        try { peso = std::stod(pesoStr); } catch (...) { peso = 1.0; }

        //Evitar auto bucles
        if (origen == destino) continue;

        //En grafo no dirigido, evitar duplicados (como A-B y B-A)
        if (!grafo.existeArista(origen, destino))
            grafo.agregarArista(origen, destino, peso);

        aristasLeidas++;
    }

    std::cout << "[IMDb] Vertices: " << grafo.numVertices()
              << "  Aristas: "       << grafo.numAristas()
              << "  (lineas leidas: " << aristasLeidas << ")\n";
    return grafo;
}

// leerYeast
// Formato: TSV sin encabezado
// El archivo tiene cada arista dos veces
// Grafo no dirigido, por lo que el peso = 1
Grafo leerYeast(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open())
        throw std::runtime_error("No se pudo abrir: " + rutaArchivo);

    Grafo grafo(false); //no dirigido
    std::string linea;
    int aristasLeidas = 0;

    while (std::getline(archivo, linea)) {
        linea = trim(linea);
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string origen, destino;

        //Separador que puede ser tab  o espacio
        if (!(ss >> origen >> destino)) continue;

        origen  = trim(origen);
        destino = trim(destino);
        if (origen.empty() || destino.empty()) continue;
        if (origen == destino) continue; // auto-bucle

        //El dataset tiene duplicados (A B y B A), por lo que solo se agrega una vez
        if (!grafo.existeArista(origen, destino))
            grafo.agregarArista(origen, destino, 1.0);

        aristasLeidas++;
    }

    std::cout << "[Yeast] Vertices: " << grafo.numVertices()
              << "  Aristas: "        << grafo.numAristas()
              << "  (lineas leidas: " << aristasLeidas << ")\n";
    return grafo;
}

//leerTrade
//Es un grafo dirigido, con peso = volumen de comercio
//Los IDs numéricos se mapean a nombres de un país (de 3 letras)
Grafo leerTrade(const std::string& rutaArchivo, const std::string& anio) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open())
        throw std::runtime_error("No se pudo abrir: " + rutaArchivo);

    Grafo grafo(true); //dirigido
    std::string linea;

    //Mapa
    std::unordered_map<int, std::string> idANombre;

    enum class Seccion { NINGUNA, VERTICES, ARCOS };
    Seccion seccion = Seccion::NINGUNA;

    int aristasLeidas = 0;

    while (std::getline(archivo, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;

        //Detectar sección
        std::string lineaLower = linea;
        for (auto& c : lineaLower) c = static_cast<char>(tolower(c));

        if (lineaLower.find("*vertices") != std::string::npos) {
            seccion = Seccion::VERTICES;
            continue;
        }
        if (lineaLower.find("*arcs") != std::string::npos ||
            lineaLower.find("*edges") != std::string::npos) {
            seccion = Seccion::ARCOS;
            continue;
        }

        if (seccion == Seccion::VERTICES) {
            std::stringstream ss(linea);
            int id;
            if (!(ss >> id)) continue;

            //Buscar nombre entre comillas
            size_t ini = linea.find('"');
            size_t fin = linea.find('"', ini + 1);
            if (ini == std::string::npos || fin == std::string::npos) continue;

            std::string nombre = linea.substr(ini + 1, fin - ini - 1);
            nombre = trim(nombre);
            idANombre[id] = nombre;
            grafo.agregarVertice(nombre);
        }
        else if (seccion == Seccion::ARCOS) {
            std::stringstream ss(linea);
            int idOrigen, idDestino;
            double peso;

            if (!(ss >> idOrigen >> idDestino >> peso)) continue;

            auto itO = idANombre.find(idOrigen);
            auto itD = idANombre.find(idDestino);
            if (itO == idANombre.end() || itD == idANombre.end()) continue;

            const std::string& origen  = itO->second;
            const std::string& destino = itD->second;

            if (origen == destino) continue; //auto bucle
            grafo.agregarArista(origen, destino, peso);
            aristasLeidas++;
        }
    }

    std::string etiqueta = anio.empty() ? "Trade" : "Trade " + anio;
    std::cout << "[" << etiqueta << "] Vertices: " << grafo.numVertices()
              << "  Aristas: "                      << grafo.numAristas()
              << "  (lineas leidas: "               << aristasLeidas << ")\n";
    return grafo;
}