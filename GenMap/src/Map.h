#pragma once

#include <vector>	// vector
#include <utility>	// pair
#include <array>	// array
#include <cstdlib>	// rand
#include <time.h>	// time
#include <iostream>
#include "MapLoader.h"


#define NBROW 28
#define NBCOLUMN 32
#define TAILLEREGION 16

using namespace std;
using Region = std::vector<std::pair<unsigned int, unsigned int>>;
using Regions = std::vector<Region>;
using Map = array<array<int, NBCOLUMN>, NBROW>;
using Coord = pair<int, int>;

//Fonction Cell
bool operator!=(pair<int, int> coord1, pair<int, int> coord2);
Coord VoisinCell(Map& map, Coord coord, int dirCell);
bool AjouterCell(Map& map, Region& region, int numRegion);

//Fonction Map
void AfficherMap(const Map& map);
Coord RechercherMap(const Map& map, int val, int ordre);
void RemplirMap(Map& map, Regions& regions);


// Load la Map
void loadMap(Regions& regions);