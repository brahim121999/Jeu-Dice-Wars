#pragma once
#include <vector>	// vector
#include <utility>	// pair
#include <array>	// array
#include <cstdlib>	// rand
#include <time.h>	// time
#include <algorithm>// remove
#include <iostream>
#include "MapLoader.h" // Regions 

#define NBROW 28
#define NBCOLUMN 32
#define TAILLEREGIONMAX 16
#define TAILLEREGIONMIN 4
#define NBREGIONS 48

using namespace std;
using Coord = pair<unsigned int, unsigned int>;
using Map = array<array<int, NBCOLUMN>, NBROW>;

class RegionV
{
public:
	RegionV(int id_region);
	~RegionV() = default;
	RegionV(const RegionV&) = default;

	auto size() const { return region.size(); }
	auto operator[](int i) const { return region[i]; }

	void push_back(Coord cell) { return region.push_back(cell); }
	void push_back_voisine(int id) { return regions_voisines.push_back(id); }
	void remove_voisine(int id) {
		vector<int>::iterator val_a_supp;
		for (vector<int>::iterator ptr = regions_voisines.begin(); ptr < regions_voisines.end(); ptr++) { 
			if (*ptr == id) { 
				val_a_supp = ptr;
			} 
		}
		regions_voisines.erase(val_a_supp);
	}

	inline vector<int>& get_regions_voisine() { return regions_voisines; }
	inline int& get_id_region() { return id_region; }
	inline vector<Coord> regionVtoRegion() { return region; }

private:
	int id_region;
	vector<Coord> region;
	vector<int> regions_voisines; // contient les ids des régions => moyen d'optimiser en utilisant un arbre binaire de recherche...
};

using RegionsV = vector<RegionV>;

//Fonction Cell
bool operator!=(pair<int, int> coord1, pair<int, int> coord2);
bool operator==(pair<int, int> coord1, pair<int, int> coord2);
Coord VoisinCell(Map& map, Coord coord, int dirCell);
bool AjouterCell(Map& map, RegionsV& regions, RegionV& region, int numRegion);

//Fonction Map
void AfficherMap(const Map& map);
Coord RechercherMap(const Map& map, int val, int ordre);
void RemplirMap(Map& map, RegionsV& regions);
void TrouerMap(Map& map, RegionsV& regions, int nb_region_restante);
void LoadMapAlea(Regions &regions);

//Fonction Region
void AfficherRegion(const RegionV region);
void AfficherRegionVoisin(vector<int> listeIds);
void ActualiserRegionsVoisines(Map& map, RegionsV& regions, RegionV& region, int numRegion, Coord coord);

void SupprimerRegion(Map& map, RegionsV& regions, int id_region);
bool VerifConnexite(RegionsV& regions, int id_region_a_supp, int id_region_demarrage, int nb_region);
void RecuVerifConnexite(RegionsV& regions, RegionV& region, int id_region_a_supp, vector<int>& vec, int nb_region);
