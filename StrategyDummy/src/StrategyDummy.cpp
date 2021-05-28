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
		// si la cellule est la notre et qu'elle possde plus d'un dès (donc qu'elle peux attaquer)
			for (unsigned int j = 0; j < Map.cells[i].nbNeighbors; j++) {
			// pour chacune de ses cellules voisines
				if (Map.cells[i].neighbors[j]->infos.owner != Id && Map.cells[i].infos.nbDices > Map.cells[i].neighbors[j]->infos.nbDices) {
				// si la cellule voisine est une cellule adverse et qu'elle possde moins de dès que notre cellule
				// on calcule grâce à la fonction atqCalculScore(), le score obtenu si l'attaque réussi, 
				// celui obtenu si l'attaque échoue, ainsi que la probabilité de réussite de notre attaque
					double score_atq_reussi = atqCalculScore(Id, Map.cells[i], *Map.cells[i].neighbors[j], true);
					double score_atq_echoue = atqCalculScore(Id, Map.cells[i], *Map.cells[i].neighbors[j], false);
					double proba_reussite = proba(Map.cells[i].infos.nbDices, Map.cells[i].neighbors[j]->infos.nbDices);
					// on calcul l'esprance du score obtenu après attaque en fonction des 2 scores 
					// obtenus et de la proba de réussite
					double score = (score_atq_reussi * proba_reussite) + (score_atq_echoue * (1 - proba_reussite)); 
					// varie entre -13 et 21
					if (score > meilleur_score) { 
					// si elle est meilleure que la meilleure esprance trouvée jusque-là, 
					//on la retient et l'action joué ce tour sera celle-ci à moins que l'on trouve mieux
						meilleur_score = score;
						meilleure_action.first = Map.cells[i].infos.id;
						meilleure_action.second = Map.cells[i].neighbors[j]->infos.id;
					}
				}
			}
		}
	}
	if (meilleur_score > 0) {
		turn->cellFrom = meilleure_action.first;
		turn->cellTo = meilleure_action.second;
		return true;
	}
	else {
		return false;
	}
}

// fonction de calcul de score d'une attaque, basée sur le nombre de voisins amis de la cellule qui attaque, 
// son nombre de dés ainsi que le nombre de voisins ennemis qu'elle a et leur nombre de dés
double atqCalculScore(int id, SCell& cellule, SCell& ennemie, bool reussir) {
	if (reussir == true) { // calcul du score si l'attaque réussie
		double score = 0;

		// on calcule le score de la cellule que l'on vient de prendre
		double nb_voisins_ennemies = 0;
		double nb_des_voisins = 0;
		for (unsigned int i = 0; i < ennemie.nbNeighbors; i++) {
			if (ennemie.neighbors[i]->infos.owner != id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += ennemie.neighbors[i]->infos.nbDices;
			}
		}
		if (nb_voisins_ennemies != 0) { // si la cellule que l'on vient de prendre a des voisins ennemis
			score += (cellule.infos.nbDices - 1) - (nb_des_voisins / nb_voisins_ennemies); // varie entre -7 et 6
		}
		else { // si elle n'a pas de voisins ennemies, son score est son nombre de dés
			score += cellule.infos.nbDices - 1; // varie entre 1 et 7
		}

		// pour la cellule avec laquelle on attaque
		// score de la cellule avant attaque
		nb_voisins_ennemies = 0;
		nb_des_voisins = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_avant = (nb_des_voisins / nb_voisins_ennemies) - (cellule.infos.nbDices); // varie entre -7 et 6

		// score de la cellule après attaque
		nb_voisins_ennemies = 0;
		nb_des_voisins = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id && cellule.neighbors[i]->infos.id != ennemie.infos.id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_apres = 1; // après avoir attaquer il nous reste forcément 1 dès sur la cellule
		if (nb_voisins_ennemies != 0) {
			score_apres = (nb_des_voisins / nb_voisins_ennemies) - 1; // varie entre 0 et 7
		}

		// score total de l'attaque : addition du score de la cellule à attaquer 
		// + la différence de score de la cellule qui attaque, entre avant et après avoir attqué
		score += (score_apres - score_avant); // varie entre -13 et 21

		return score;
	}
	else { // calcul du score si l'attaque échoue
		double score = 0;

		// pour la cellule avec laquelle on attaque
		// score avant attaque
		double nb_voisins_ennemies = 0;
		double nb_des_voisins = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_avant = (nb_des_voisins / nb_voisins_ennemies) - (cellule.infos.nbDices); // varie entre -7 et 6

		// score après attaque
		nb_voisins_ennemies = 0;
		nb_des_voisins = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_voisins_ennemies += 1;
				nb_des_voisins += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_apres = 1; // après avoir attaqué il nous reste forcément 1 dès sur la cellule
		if (nb_voisins_ennemies != 0) {
			score_apres = (nb_des_voisins / nb_voisins_ennemies) - 1; // varie entre 0 et 7
		}
		
		// score total de l'attaque 
		score += (score_apres - score_avant); // varie entre -6 et 14

		return score;
	}
}

// fonction qui retourne un nombre qui nous servira de probabilité (bien que ce ne soit pas la vraie probabilité) 
// de réussite d'une attaque en fonction du nombre de dés 
// de la cellule avec laquelle on attaque, et le nombre de dès de la cellule qu'on attaque. 
double proba(unsigned int nbDes, unsigned int nbDesVoisin) {
	return (nbDes - nbDesVoisin + 1) / nbDes; // varie entre 0.125 et 1
}
