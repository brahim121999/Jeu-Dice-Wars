#include <iostream>
//#include <cstdlib>
#include <random>
#include <array>
#include "Map.h"
//#include "oklm.h"

//#define NBROW 28
//#define RBCOLUMN 32


void loadMap(Regions& regions)
{
	regions.clear();
	// on boucle des vecteurs de vecteurs de Cell
	
	// On remplis vecteur de Cell

	// On choisis aléatoirement des Cell
	Map map = { 0 };

	RemplirMap(map, regions);
}

//Fonction Cell
bool operator!=(pair<int, int> coord1, pair<int, int> coord2) {
	if (coord1.first != coord2.first || coord1.second != coord2.second) {
		return true;
	}
	else {
		return false;
	}
}

bool operator==(pair<int, int> coord1, pair<int, int> coord2) {
	if (coord1.first == coord2.first && coord1.second == coord2.second) {
		return true;
	}
	else {
		return false;
	}
}

Coord VoisinCell(Map& map, Coord coord, int dirCell) {
	if (coord.first % 2 == 0) {
		switch (dirCell)
		{
		case 1: // cellule en haut à gauche
			if (coord.first - 1 >= 0 && coord.second - 1 >= 0) {
				return Coord(coord.first - 1, coord.second - 1);
			}
			else { return coord; }
		case 2: // cellule en haut à droite
			if (coord.first - 1 >= 0) {
				return Coord(coord.first - 1, coord.second);
			}
			else { return coord; }
		case 3: // cellule à gauche
			if (coord.second - 1 >= 0) {
				return Coord(coord.first, coord.second - 1);
			}
			else { return coord; }
		case 4: // cellule à droite
			if (coord.second + 1 < map.size()) {
				return Coord(coord.first, coord.second + 1);
			}
			else { return coord; }
		case 5: // cellule en bas à gauche
			if (coord.first + 1 < map.size() && coord.second - 1 >= 0) {
				return Coord(coord.first + 1, coord.second - 1);
			}
			else { return coord; }
		case 6: // cellule en bas à droite
			if (coord.first + 1 < map.size()) {
				return Coord(coord.first + 1, coord.second);
			}
			else { return coord; }
		default: // cellule initial
			return coord;
		}
	}
	else {
		switch (dirCell)
		{
		case 1: // cellule en haut à gauche
			if (coord.first - 1 >= 0) {
				return Coord(coord.first - 1, coord.second);
			}
			else { return coord; }
		case 2: // cellule en haut à droite
			if (coord.first - 1 >= 0 && coord.second + 1 < map.size()) {
				return Coord(coord.first - 1, coord.second + 1);
			}
			else { return coord; }
		case 3: // cellule à gauche
			if (coord.second - 1 >= 0) {
				return Coord(coord.first, coord.second - 1);
			}
			else { return coord; }
		case 4: // cellule à droite
			if (coord.second + 1 < map.size()) {
				return Coord(coord.first, coord.second + 1);
			}
			else { return coord; }
		case 5: // cellule en bas à gauche
			if (coord.first + 1 < map.size()) {
				return Coord(coord.first + 1, coord.second);
			}
			else { return coord; }
		case 6: // cellule en bas à droite
			if (coord.first + 1 <= map.size() && coord.second + 1 <= map.size()) {
				return Coord(coord.first + 1, coord.second + 1);
			}
			else { return coord; }
		default: // cellule initial
			return coord;
		}
	}
}

bool AjouterCell(Map& map, Region& region, int numRegion) {
	srand(time(0));
	default_random_engine generator;
	uniform_int_distribution<int> distributionNumCell(1, static_cast<unsigned int>(region.size()));
	// numCell numéro de la cellule dans la region
	//int numCell = rand() % region.size();
	int numCell = distributionNumCell(generator);

	uniform_int_distribution<int> distributionDirCell(1, 6);

	// Ici, on voici un voisin au hasard de la cellule numCell
	//int dirCell = rand() % 6;
	int dirCell = distributionDirCell(generator);

	for (size_t i = numCell; i < region.size(); i++)
	{
		// On part tout d"abord de dirCell afin de créer de l'aléatoire
		for (size_t j = dirCell; j < 6; j++)
		{
			// region[i] est la coordonnée du numéro de cellule choisie au hasard
			Coord cellVoisine = VoisinCell(map, region[static_cast<int>(i)], static_cast<int>(j)); // Ici, on a les coordonées de son voisin choisi au hasard
			if (cellVoisine != region[static_cast<int>(i)] && map[cellVoisine.first][cellVoisine.second] == 0) {
				region.push_back(cellVoisine);
				map[cellVoisine.first][cellVoisine.second] = numRegion;
				return true;
			}
		}
		for (size_t j = 0; j < dirCell; j++)
		{
			Coord cellVoisine = VoisinCell(map, region[static_cast<int>(i)], static_cast<int>(j));
			if (cellVoisine != region[static_cast<int>(i)] && map[cellVoisine.first][cellVoisine.second] == 0) {
				region.push_back(cellVoisine);
				map[cellVoisine.first][cellVoisine.second] = numRegion;
				return true;
			}
		}
	}
	for (size_t i = 0; i < numCell; i++)
	{
		for (size_t j = dirCell; j < 6; j++)
		{
			Coord cellVoisine = VoisinCell(map, region[static_cast<int>(i)], static_cast<int>(j));
			if (cellVoisine != region[static_cast<int>(i)] && map[cellVoisine.first][cellVoisine.second] == 0) {
				region.push_back(cellVoisine);
				map[cellVoisine.first][cellVoisine.second] = numRegion;
				return true;
			}
		}
		for (size_t j = 0; j < dirCell; j++)
		{
			Coord cellVoisine = VoisinCell(map, region[static_cast<int>(i)], static_cast<int>(j));
			if (cellVoisine != region[static_cast<int>(i)] && map[cellVoisine.first][cellVoisine.second] == 0) {
				region.push_back(cellVoisine);
				map[cellVoisine.first][cellVoisine.second] = numRegion;
				return true;
			}
		}
	}
	return false;
}

// Fonction Map
void AfficherMap(const Map& map) {
	for (unsigned int i = 0; i < map.size(); i++)
	{
		for (unsigned int j = 0; j < map[0].size(); j++)
		{
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void AfficherRegion(const Region region) {
	for (size_t i = 0; i < region.size(); i++)
	{
		cout << "<" << region[i].first << "," << region[i].second << "> ";
	}
	cout << endl;
}

Coord RechercherMap(const Map& map/*array<array<pair<unsigned int, unsigned int>>>*/, int val, int ordre = 0 /* vaut 0 si recherche ligne par ligne */) {
	if (ordre == 0) {
		for (unsigned int i = 0; i < map.size(); i++)
		{
			for (unsigned int j = 0; j < map[i].size(); j++)
			{
				if (map[i][j] == val) { return Coord(i, j); }
			}
		}
	}
	else {
		for (unsigned int i = 0; i < map[0].size(); i++)
		{
			for (unsigned int j = 0; j < map.size(); j++)
			{
				if (map[i][j] == val) { return Coord(j, i); }
			}
		}
	}
	return Coord(-1, -1);
}

/*
using Region = std::vector<std::pair<unsigned int, unsigned int>>;  <=> Region->Cellule
using Regions = std::vector<Region>;
*/

void RemplirMap(Map& map, Regions& regions) {
	
	srand(time(0));
	
	Coord coord = RechercherMap(map, 0);

	while (coord.first >= 0) {
		// tant que le tableau n'est pas plein 
			// on crée une nouvelle région
		Region newRegion;
		regions.push_back(newRegion);
		unsigned int lastRegion = static_cast<unsigned int>(regions.size()) - 1;
		unsigned int numRegion = static_cast<unsigned int>(regions.size());
		regions[lastRegion].push_back(coord);
		map[coord.first][coord.second] = numRegion;  // "numRegion" : Nom qu'on donne à la region
		// on crée un bool qui détermine si la région a atteint sa taille maximum (elle est entourée de case occupée)
		bool regionTailleMax = false;
		while (!regionTailleMax && (regions[lastRegion].size() < TAILLEREGION))
			// on remplit les régions une par une
		{
			// Faux si on ne peut pas rajouter, vrai sinon
			regionTailleMax = !AjouterCell(map, regions[lastRegion], numRegion); // region.ajouterCell(map, cell, numregion)
		}
		coord = RechercherMap(map, 0);
		AfficherRegion(regions[lastRegion]);
	}
}

//déterminer taille de la région
//bool = true;
	//tant que taille pas bonne et bool == true:
		//sélectionne alea une des cell de la région (alea 1)
		//sélectionne alea une des pos adjacentes à la cellule (alea 2)
		//on boucle pour chaque cell (en partant de alea 1)
			//on boucle sur chaque pos (en partant de alea 2)
			//si on a fait le tour de toute cell, bool = false;