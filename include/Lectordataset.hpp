#pragma once
#include "Grafo.hpp"
#include <string>

Grafo leerIMDb(const std::string& rutaArchivo);

Grafo leerYeast(const std::string& rutaArchivo);

Grafo leerTrade(const std::string& rutaArchivo,
                const std::string& anio = "");