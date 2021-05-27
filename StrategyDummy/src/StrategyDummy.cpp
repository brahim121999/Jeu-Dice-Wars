#include <iostream>
#include <set>
#include <vector>
#include <array>
#include "StrategyDummy.h"
#include <utility>

StrategyDummy::StrategyDummy(unsigned int id, unsigned int nbPlayer, const SMap* map) :
	Id(id),
	NbPlayer(nbPlayer)
{
	// faire une copie entière de la structure map localement dans l'objet Map
	// on copie les cellules
	Map.nbCells = map->nbCells;
	Map.cells = new SCell[Map.nbCells];
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		Map.cells[i] = map->cells[i];
		Map.cells[i].neighbors = new SCell* [Map.cells[i].nbNeighbors];
	}
	// on remplis les tableau de voisins avec les adresses des voisins
	for (unsigned int i = 0; i < Map.nbCells; ++i) {
		for (unsigned int j = 0; j < Map.cells[i].nbNeighbors; ++j) {
			for (unsigned int k = 0; k < Map.nbCells; ++k) {
				if (((map->cells[i].neighbors[j])->infos).id == Map.cells[k].infos.id) {
					Map.cells[i].neighbors[j] = &Map.cells[k];
				}
			}
		}
	}
}

StrategyDummy::~StrategyDummy()
{
	// détruire proprement la structure Map
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		delete& Map.cells[i];
	}
	delete& Map;
}

bool StrategyDummy::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{
	// on met à jour nos infos sur la map
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		Map.cells[i].infos = state->cells[i];
	}

	// par défaut, la meilleure action est de ne rien faire
	double meilleur_score = 0;
	std::pair<unsigned int, unsigned int> meilleure_action (NULL,NULL);

	for (unsigned int i = 0; i < Map.nbCells; i++) {
		if (Map.cells[i].infos.owner == Id && Map.cells[i].infos.nbDices > 1) {
		// si la cellule est la notre et qu'elle possède plus d'un dé (donc qu'elle peux attaquer)
			for (unsigned int j = 0; j < Map.cells[i].nbNeighbors; j++) {
			// pour chacune de ses cellules voisines
				if (Map.cells[i].neighbors[j]->infos.owner != Id && Map.cells[i].infos.nbDices > Map.cells[i].neighbors[j]->infos.nbDices) {
				// si la cellule voisine est une cellule adverse et qu'elle possède moins de dés que notre cellule
					double score_atq_reussi = atqCalculScore(Id, Map.cells[i], *Map.cells[i].neighbors[j], true);
					double score_atq_echoue = atqCalculScore(Id, Map.cells[i], *Map.cells[i].neighbors[j], false);
					double proba_reussite = proba(Map.cells[i].infos.nbDices, Map.cells[i].neighbors[j]->infos.nbDices);
					// on calcul l'espérance du score obtenu après atq
					double score = (score_atq_reussi * proba_reussite) + (score_atq_echoue * (1 - proba_reussite));
					if (score > meilleur_score) { 
					// si elle est meilleure que la meilleure espérance trouvée jusque-là, on la retient et l'action jouée ce tour sera celle-ci
					// à moins que l'on trouve mieux
						meilleur_score = score;
						meilleure_action.first = Map.cells[i].infos.id;
						meilleure_action.second = Map.cells[i].neighbors[j]->infos.id;
					}
				}
			}
		}
	}
	if (meilleur_score != 0) {
		turn->cellFrom = meilleure_action.first;
		turn->cellTo = meilleure_action.second;
		return true;
	}
	else {
		return false;
	}
}

double atqCalculScore(int id, SCell& cellule, SCell& ennemie, bool reussir) {
	if (reussir == true) {
		double score = 0;

		// pour la cellule que l'on vient de prendre
		double nb_voisins_ennemies = 0;
		double nb_des_voisins = 0;
		for (unsigned int i = 0; i < ennemie.nbNeighbors; i++) {
			if (ennemie.neighbors[i]->infos.owner != id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += ennemie.neighbors[i]->infos.nbDices;
			}
		}
		if (nb_voisins_ennemies != 0) {
			score += (nb_des_voisins / nb_voisins_ennemies) - (cellule.infos.nbDices - 1);
		}
		else {
			score += cellule.infos.nbDices - 1;
		}

		// pour la cellule avec laquelle on prend
		// score avant atq
		nb_voisins_ennemies = 0;
		nb_des_voisins = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_avant = (nb_des_voisins / nb_voisins_ennemies) - (cellule.infos.nbDices);
		// score après atq
		nb_voisins_ennemies = 0;
		nb_des_voisins = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id && cellule.neighbors[i]->infos.id != ennemie.infos.id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_apres = 1;
		if (nb_voisins_ennemies != 0) {
			score_apres = (nb_des_voisins / nb_voisins_ennemies) - 1;
		}
		// score total
		score += (score_apres - score_avant);

		return score;
	}
	else {
		double score = 0;

		// pour la cellule avec laquelle on prend
		// score avant atq
		double nb_voisins_ennemies = 0;
		double nb_des_voisins = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_avant = (nb_des_voisins / nb_voisins_ennemies) - (cellule.infos.nbDices);
		// score après atq
		nb_voisins_ennemies = 0;
		nb_des_voisins = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_apres = 1;
		if (nb_voisins_ennemies != 0) {
			score_apres = (nb_des_voisins / nb_voisins_ennemies) - 1;
		}
		// score total
		score += (score_apres - score_avant);

		return score;
	}
}

double proba(unsigned int nbDes, unsigned int nbDesVoisin) {
	return (nbDes - nbDesVoisin + 1) / nbDes;
}