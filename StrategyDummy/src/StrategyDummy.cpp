#include <iostream>
#include <set>
#include <vector>
#include <array>
#include "StrategyDummy.h"

StrategyDummy::StrategyDummy(unsigned int id, unsigned int nbPlayer, const SMap* map) :
	Id(id),
	NbPlayer(nbPlayer)
{
	// faire une copie entière de la structure map localement dans l'objet Map
	//Map.cells = nullptr;
	//Map.nbCells = 0;

	//Map.cells = map->cells;
	//Map.nbCells = new unsigned int;
	Map.nbCells = map->nbCells;
	Map.cells = new SCell[Map.nbCells];
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		Map.cells[i] = map->cells[i];
	}
}

StrategyDummy::~StrategyDummy()
{
	// détruire proprement la structure Map
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		//delete& Map.cells[i];
	}
	//delete& Map;
}

double calculescore(std::vector<SCell&> cellules);
double calculescore(SCell cellule, SCell ennemie, bool reussir);
double proba(unsigned int nbDes, unsigned int nbDesVoisin);

bool StrategyDummy::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{
	// dérouler votre algorithme de choix
	//std::cout << gameTurn << std::endl;
	std::vector<SCell> me;
	
	for (unsigned int i = 0; i < this->Map.nbCells; i++) {
		// Pour chaque éléments de la Map
		SCell &thecell = Map.cells[i];
		SCellInfo &ofthecell = thecell.infos;
		
		// Si la Cellule est la mienne
		if (this->Id == ofthecell.owner) {
			// Je l'ajoute dans le vecteur concernant mes cellules
			me.push_back(Map.cells[i]);
		}
		
	}

	double meilleur_score = 0.0;//calculescore(me);
	std::pair<unsigned int, unsigned int> action = { 0, 0 };

	for (SCell cell : me) {
		pSCell* neighborsOfthecell = cell.neighbors;
		unsigned int nbNeighbors = cell.nbNeighbors;

		if (cell.infos.nbDices > 1) {
			for (unsigned int j = 0; j < nbNeighbors; j++) {
				
				if (((cell.neighbors[j])->infos.owner != this->Id) && ((cell.neighbors[j])->infos.nbDices) < cell.infos.nbDices) {
					// Calculer un score
					double s1 = calculescore(cell, *neighborsOfthecell[j], true);
					double s2 = calculescore(cell, *neighborsOfthecell[j], false);
					double psucces = proba((cell).infos.nbDices, (*neighborsOfthecell[j]).infos.nbDices);
					double s = s1 * psucces + s2 * (1 - psucces);
					/*if (s > meilleur_score) {
						meilleur_score = s;
						action = {cell.infos.id, neighborsOfthecell[j]->infos.id };
						turn->cellFrom = action.first;
						turn->cellTo = action.second;
						return(true);
					}*/
					action = { (cell).infos.id, neighborsOfthecell[j]->infos.id };
					turn->cellFrom = action.first;
					turn->cellTo = action.second;
					return(true);
				}
				else {
					return(false);
				}
			}
		}
	}

	// On attend tout d'abord pour avoir plus de dés

	// Quand on a beaucoup de dés, on attaque 
	return(false);
}

double calculescore(std::vector<SCell> cellules) {
	double toreturn;
	return 0.0;
}

double calculescore(SCell cellule, SCell ennemie, bool reussir) {
	return 0.0;
}

double proba(unsigned int nbDes, unsigned int nbDesVoisin) {
	return 0.0;
}