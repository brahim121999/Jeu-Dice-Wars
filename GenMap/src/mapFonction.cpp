#include "mapFonction.h"

// Constructeur(s) Region

RegionV::RegionV(int id_region) : id_region(id_region) {}

// Fonction Cell
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
		case 1: // cellule en haut � gauche
			if (coord.first >= 1 && coord.second >= 1) {
				return Coord(coord.first - 1, coord.second - 1);
			}
			else { return coord; }
		case 2: // cellule en haut � droite
			if (coord.first >= 1) {
				return Coord(coord.first - 1, coord.second);
			}
			else { return coord; }
		case 3: // cellule � gauche
			if (coord.second >= 1) {
				return Coord(coord.first, coord.second - 1);
			}
			else { return coord; }
		case 4: // cellule � droite
			if (coord.second + 1 < map[0].size()) {
				return Coord(coord.first, coord.second + 1);
			}
			else { return coord; }
		case 5: // cellule en bas � gauche
			if (coord.first + 1 < map.size() && coord.second >= 1) {
				return Coord(coord.first + 1, coord.second - 1);
			}
			else { return coord; }
		case 6: // cellule en bas � droite
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
		case 1: // cellule en haut � gauche
			if (coord.first >= 1) {
				return Coord(coord.first - 1, coord.second);
			}
			else { return coord; }
		case 2: // cellule en haut � droite
			if (coord.first >= 1 && coord.second + 1 < map[0].size()) {
				return Coord(coord.first - 1, coord.second + 1);
			}
			else { return coord; }
		case 3: // cellule � gauche
			if (coord.second >= 1) {
				return Coord(coord.first, coord.second - 1);
			}
			else { return coord; }
		case 4: // cellule � droite
			if (coord.second + 1 < map[0].size()) {
				return Coord(coord.first, coord.second + 1);
			}
			else { return coord; }
		case 5: // cellule en bas � gauche
			if (coord.first + 1 < map.size()) {
				return Coord(coord.first + 1, coord.second);
			}
			else { return coord; }
		case 6: // cellule en bas � droite
			if (coord.first + 1 < map.size() && coord.second + 1 < map[0].size()) {
				return Coord(coord.first + 1, coord.second + 1);
			}
			else { return coord; }
		default: // cellule initial
			return coord;
		}
	}
}

bool AjouterCell(Map& map, RegionsV& regions, RegionV& region, int numRegion) {
	// numCell num�ro de la cellule dans la region
	int numCell = rand() % region.size();
	// ici, on choisi un voisin au hasard de la cellule numCell
	int dirCell = rand() % 6;
	// on regarde pour le voisin tir� au hasard de la cellule tir�e au hasard si on peux l'ajouter � notre r�gion
	// sinon on prend le voisin suivant de la cellule tir�e au hasard
	// si aucun des voisins n'est dispo on prend la cellule suivante (dans la liste de cellule de notre r�gion)
	// si apr�s avoir test� tout les voisins de toutes les cellules aucun n'est dispo, alors on renvoi la cellule elle-m�me
	// proc�der comme ceci conserve l'al�atoire et �vite de tester plusieurs fois certaines cellule (alors m�me qu'il est possible qu'aucune ne soit dispo)
	for (size_t i = numCell; i < region.size(); i++)
	{
		for (size_t j = dirCell; j < 6; j++)
		{
			Coord cellVoisine = VoisinCell(map, region[i], j);
			if (cellVoisine != region[i] && map[cellVoisine.first][cellVoisine.second] == 0) {
				region.push_back(cellVoisine);
				map[cellVoisine.first][cellVoisine.second] = numRegion;
				ActualiserRegionsVoisines(map, regions, region, numRegion, cellVoisine);
				return true;
			}
		}
		for (size_t j = 0; j < dirCell; j++)
		{
			Coord cellVoisine = VoisinCell(map, region[i], j);
			if (cellVoisine != region[i] && map[cellVoisine.first][cellVoisine.second] == 0) {
				region.push_back(cellVoisine);
				map[cellVoisine.first][cellVoisine.second] = numRegion;
				ActualiserRegionsVoisines(map, regions, region, numRegion, cellVoisine);
				return true;
			}
		}
	}
	for (size_t i = 0; i < numCell; i++)
	{
		for (size_t j = dirCell; j < 6; j++)
		{
			Coord cellVoisine = VoisinCell(map, region[i], j);
			if (cellVoisine != region[i] && map[cellVoisine.first][cellVoisine.second] == 0) {
				region.push_back(cellVoisine);
				map[cellVoisine.first][cellVoisine.second] = numRegion;
				ActualiserRegionsVoisines(map, regions, region, numRegion, cellVoisine);
				return true;
			}
		}
		for (size_t j = 0; j < dirCell; j++)
		{
			Coord cellVoisine = VoisinCell(map, region[i], j);
			if (cellVoisine != region[i] && map[cellVoisine.first][cellVoisine.second] == 0) {
				region.push_back(cellVoisine);
				map[cellVoisine.first][cellVoisine.second] = numRegion;
				ActualiserRegionsVoisines(map, regions, region, numRegion, cellVoisine);
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
		if (i % 2 == 0) { cout << "  "; }
		for (unsigned int j = 0; j < map[0].size(); j++)
		{
			if (map[i][j] < 10) { cout << " "; }
			cout << map[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;
}

Coord RechercherMap(const Map& map, int val, int ordre = 0 /* vaut 0 si recherche ligne par ligne */) {
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
	return Coord(1000, 1000);
}

void RemplirMap(Map& map, RegionsV& regions) {
	srand(time(NULL));
	Coord coord = RechercherMap(map, 0);

	while (coord.first != 1000) {
		// tant que le tableau n'est pas plein 
			// on cr�e une nouvelle r�gion
		RegionV newRegion(regions.size() + 1);
		regions.push_back(newRegion);
		unsigned int lastRegion = regions.size() - 1;
		unsigned int numRegion = regions.size();
		regions[lastRegion].push_back(coord);
		ActualiserRegionsVoisines(map, regions, regions[lastRegion], numRegion, coord);

		map[coord.first][coord.second] = numRegion;  // "numRegion" : Nom qu'on donne � la region
		// on cr�e un bool qui d�termine si la r�gion a atteint sa taille maximum (elle est entour�e de case occup�e)
		bool regionTailleMax = false;
		while (!regionTailleMax && (regions[lastRegion].size() < TAILLEREGIONMAX))
			// on remplit les r�gions une par une
		{
			// Faux si on ne peut pas rajouter, vrai sinon
			regionTailleMax = !AjouterCell(map, regions, regions[lastRegion], numRegion);
		}
		coord = RechercherMap(map, 0);
		//AfficherRegion(regions[lastRegion]);
	}
}

void TrouerMap(Map& map, RegionsV& regions, int nb_region_restante) {

	int nb_region = regions.size();
	// on supprime toutes les r�gions trop petite :
	// on commence par d�terminer une region qui sera la region de demarrage de la r�cursivit� : elle doit �tre de taille > � la taille mini pour pas avoir � en trouver une autre
	// au cas o� on la supprimerais. Elle est choisi al�atoirement. Aussi on ne la supprimera pas pour ne pas avoir � en trouver une autre.
	int id_region_demarrage;
	int alea = rand() % nb_region;
	vector<RegionV>::iterator ptr = regions.begin() + alea;
	while (((*ptr).get_id_region() == 0 || (*ptr).size() < TAILLEREGIONMIN) && ptr < regions.end()) { ptr++; }
	if (ptr != regions.end()) { id_region_demarrage = (*ptr).get_id_region(); }
	else {
		ptr = regions.begin();
		while (((*ptr).get_id_region() == 0 || (*ptr).size() < TAILLEREGIONMIN) && ptr < regions.end()) { ptr++; }
		id_region_demarrage = (*ptr).get_id_region();
	}
	// maintenant qu'on a d�terminer notre region de demarrage de toutes nos fonction, on supprime si possible les r�gions trop petites
	for (vector<RegionV>::iterator ptr = regions.begin(); ptr < regions.end(); ptr++) {
		if ((*ptr).size() < TAILLEREGIONMIN														// si la region est trop petite
			&& VerifConnexite(regions, (*ptr).get_id_region(), id_region_demarrage, nb_region)	// si la supprimer ne nous fait pas perdre la connexit�
			&& nb_region > nb_region_restante)													// si apr�s sa suppression, il ne nous resterait pas moins de regions que le nb_region_restante
		{
			SupprimerRegion(map, regions, (*ptr).get_id_region());								// alors on la supprime
			nb_region--;
		}
	}
	// maintenant on supprime al�atoirement des r�gions
	while (nb_region > nb_region_restante) {
		alea = rand() % nb_region;
		if (regions[alea].get_id_region() != id_region_demarrage								// si ce n'est pas notre r�gion de d�marrage
			&& regions[alea].get_id_region() != 0												// si elle n'a pas d�j� �t� supprim�e
			&& VerifConnexite(regions, alea + 1, id_region_demarrage, nb_region))				// si la supprimer ne nous fait pas perdre la connexit�
		{
			SupprimerRegion(map, regions, regions[alea].get_id_region());						// alors on la supprime
			nb_region--;
		}
	}
}

void LoadMapAlea(Regions &regions) {
	Map map = { 0 };
	RegionsV regionsV;
	RemplirMap(map, regionsV);
	TrouerMap(map, regionsV, NBREGIONS);
	for (RegionsV::iterator ptr = regionsV.begin(); ptr < regionsV.end(); ptr++) {
		if ((*ptr).get_id_region() != 0) {
			regions.push_back((*ptr).regionVtoRegion());
		}
	}
}

// Fonction Region

void AfficherRegion(const RegionV region) {
	for (size_t i = 0; i < region.size(); i++)
	{
		cout << "<" << region[i].first << "," << region[i].second << "> ";
	}
	cout << endl;
}

void AfficherRegionVoisin(vector<int> listeIds) {
	for (vector<int>::iterator ptr = listeIds.begin(); ptr < listeIds.end(); ptr++) {
		cout << *ptr << " ";
	}
	cout << endl;
}

void ActualiserRegionsVoisines(Map& map, RegionsV& regions, RegionV& region, int numRegion, Coord coord) {
	// algo naif pas tr�s opti mais suffisant car op�ration peu couteuse (liste des r�gions voisines jamais tr�s grande)
	// on recherche pour chaque cellule voisine de la cellule ajout�e, si l'une d'elle appartient � une r�gion qui n'est pas dans notre liste de r�gions voisines
	// si c'est le cas on l'ajoute
	for (size_t i = 1; i < 7; i++)
	{
		Coord cellVoisine = VoisinCell(map, coord, i);
		int idRegionVoisine = map[cellVoisine.first][cellVoisine.second];
		if (idRegionVoisine != 0 && idRegionVoisine != numRegion &&
			find(region.get_regions_voisine().begin(), region.get_regions_voisine().end(), idRegionVoisine) == region.get_regions_voisine().end())
		{
			region.push_back_voisine(idRegionVoisine);
			regions[idRegionVoisine - 1].push_back_voisine(numRegion);
		}
	}
}

void SupprimerRegion(Map& map, RegionsV& regions, int id_region) {
	RegionV& region = regions[id_region - 1];
	// on parcourt la liste des r�gions voisines
	for (vector<int>::iterator ptr = region.get_regions_voisine().begin(); ptr < region.get_regions_voisine().end(); ptr++) {
		// on supprime de la liste des r�gions voisines de chaque r�gion voisine l'id de la r�gion � supprimer
		regions[*ptr - 1].remove_voisine(id_region);
	}
	region.get_id_region() = 0;
	//juste pour l'affichage, � supprimer pour le rendu final :
	for (size_t i = 0; i < regions[id_region - 1].size(); i++)
	{
		map[region[i].first][region[i].second] = 0;
	}
}

bool VerifConnexite(RegionsV& regions, int id_region_a_supp, int id_region_demarrage, int nb_region) {
	// on cr�e un vecteur : il contient tout les noeuds auxquelles on a acc�s
	vector<int> vec;
	// on ajoute � notre vecteur l'id de la r�gion
	vec.push_back(id_region_demarrage);
	RecuVerifConnexite(regions, regions[id_region_demarrage - 1], id_region_a_supp, vec, nb_region);
	// on v�rifie que la taille de notre vecteur est �gal � notre nombre de r�gions (et donc qu'on a acc�s � toutes les r�gions � partir de notre r�gion de d�marrage)
	// c'est ce qui garanti la connexit�
	//return vec.size() == (nb_region - 1) ? true : false;
	if (vec.size() == (nb_region - 1)) { return true; }
	else { return false; }
}

void RecuVerifConnexite(RegionsV& regions, RegionV& region, int id_region_a_supp, vector<int>& vec, int nb_region) {
	// on regarde pour chaque voisin de notre r�gion si son id n'est pas d�j� dans notre vecteur et si ce n'est pas l'id de la region a supprimer
	// si ce n'est pas le cas on appelle la r�cursivit� sur cette r�gion
	if (vec.size() == nb_region - 1) { return; }

	for (vector<int>::iterator ptr = region.get_regions_voisine().begin(); ptr < region.get_regions_voisine().end(); ptr++) {
		if (find(vec.begin(), vec.end(), *ptr) == vec.end() && *ptr != id_region_a_supp) {
			vec.push_back(*ptr);
			RecuVerifConnexite(regions, regions[*ptr - 1], id_region_a_supp, vec, nb_region);
		}
	}
}

/* 
bool VerifConnexite2(Regions& regions, int id_region_a_supp, int id_region_demarrage, int nb_region) {
	// on cr�e un vecteur : il contient tout les noeuds auxquelles on a acc�s
	vector<int> vec;
	// on ajoute � notre vecteur l'id de la r�gion
	vec.push_back(id_region_demarrage);

	vector<int> vec2;
	vec.push_back(id_region_demarrage);

	vector<int> vec3;

	while (vec2.size() != 0)
	{
		for (vector<int>::iterator ptr = vec2.begin(); ptr < vec2.end(); ptr++) {
			Region region = regions[*ptr];

			for (vector<int>::iterator ptr2 = region.get_regions_voisine().begin(); ptr2 < region.get_regions_voisine().end(); ptr2++) {
				if (find(vec.begin(), vec.end(), *ptr2) == vec.end() && *ptr2 != id_region_a_supp) {
					vec3.push_back(*ptr2);
				}
			}
		}
		vec.reserve(vec.size() + vec3.size());
		vec.insert(vec.end(), vec3.begin(), vec3.end());

		vec2.clear();
		vec2.reserve(vec3.size());
		vec2.insert(vec2.begin(), vec3.begin(), vec3.end());

		vec3.clear();
	}

	// on v�rifie que la taille de notre vecteur est �gal � notre nombre de r�gions (et donc qu'on a acc�s � toutes les r�gions � partir de notre r�gion de d�marrage)
	// c'est ce qui garanti la connexit�
	return vec.size() == (nb_region - 1) ? true : false;
}
*/