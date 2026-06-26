# Proyecto Semestral — Métricas de Centralidad en Redes
**Estructura de Datos**  
Integrantes: Danitza Ávila, Vicente Ramírez e Ignacio Jara

---

## Estructura del repositorio

```
proyecto/
├── include/
│   ├── Grafo.h              
│   ├── PageRank.h           
│   ├── EigenvectorCentralidad.h  
│   ├── DegreeCentralidad.h   
│   ├── ClusteringCoefficient.h 
│   ├── LectorDataset.h       
│   ├── BetweennessCentralidad.h 
│   ├── ClosenessCentralidad.h   
│   └── AverageShortestPath.h
│
├── src/                      
│   ├── Grafo.cpp
│   ├── PageRank.cpp
│   ├── EigenvectorCentralidad.cpp
│   └── ...
│
├── experimentos/
│   ├── main_datasets.cpp 
│   └── experimento_aristas.cpp 
│
├── datasets/
│   ├── yeast/
│   │   └── yeast.edgelist
│   ├── imdb/
│   │   └── imdb_edgelist.csv
│   └── trade/
│       ├── 2000.net
│       ├── 2005.net
│       ├── 2010.net
│       ├── 2015.net
│       └── 2018.net
│
└── CMakeLists.txt 
```

---

## Compilar y ejecutar (Windows)
Para procesar los dataset:
```bash
g++ -std=c++17 -Wall -O2 -Iinclude `
  src/Grafo.cpp `
  src/LectorDataset.cpp `
  src/PageRank.cpp `
  src/EigenvectorCentralidad.cpp `
  src/DegreeCentralidad.cpp `
  src/ClusteringCoefficient.cpp `
  src/BetweennessCentralidad.cpp `
  src/ClosenessCentralidad.cpp `
  src/AverageShortestPath.cpp `
  experimentos/main_datasets.cpp `
  -o main_datasets.exe 
```
Para el experimento de aristas:
```bash
g++ -std=c++17 -Wall -O2 -Iinclude `
  src/Grafo.cpp `
  src/LectorDataset.cpp `
  src/PageRank.cpp `
  src/EigenvectorCentralidad.cpp `
  src/DegreeCentralidad.cpp `
  src/ClusteringCoefficient.cpp `
  src/BetweennessCentralidad.cpp `
  src/ClosenessCentralidad.cpp `
  src/AverageShortestPath.cpp `
  experimentos/experimento_aristas.cpp `
  -o experimento_aristas.exe

./experimento_aristas.exe
```

---

