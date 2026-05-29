# Étape du projet

## Version 01.00 : MVP

Pour le Minimum Viable Product, il faut que l'application soit capable de :
- Prendre en entré un fichier EXCEL avec une liste de voiture et de liens vers des fiches technique de l'Argus
- Scrapper ces fiches techniques et extraire des informations
- Trier les voitures selon des filtres
- Générer un fichier EXCEL de sortie avec les données filtrées

### Version 01.00 : détaillé
- 1 : La classe Entity
	1. [x] Faire une classe Entity avec UUID
	3. [x] Journalisation
	4. [x] Tests unitaires
- 2 : La classe Car
	1. [ ] Faire une classe Car pour le stockage des voitures
	2. [ ] Gestions des erreurs
	3. [ ] Journalisation
	4. [ ] Tests unitaires
- 3 : Le client HTTP
	1. [ ] Création d'un client HTTP capable de récupérer un HTML brut d'une page web
	2. [ ] Gestions des erreurs
	3. [ ] Journalisation
	4. [ ] Tests unitaires
- 4 : Le Scarpper de l'Argus
	1. [ ] Création d'un scrapper dédié au site de l'Argus
	2. [ ] Gestions des erreurs
	3. [ ] Journalisation
	4. [ ] Tests unitaires
- 5 : Le système de lecture d'EXCEL
	1. [ ] Création d'un système capable de lire un EXCEL et sortir une liste de liens hypertexte
	2. [ ] Gestions des erreurs
	3. [ ] Journalisation
	4. [ ] Tests unitaires
- 6 : Le système de filtre
	1. [ ] Création d'un système permettant de filtrer une liste de voiture selons des critères
	2. [ ] Gestions des erreurs
	3. [ ] Journalisation
	4. [ ] Tests unitaires
- 7 : Le système de génération d'EXCEL
	1. [ ] Création d'un système générant un EXCEL à partir d'une liste de voiture filtrée
	2. [ ] Gestions des erreurs
	3. [ ] Journalisation
	4. [ ] Tests unitaires


## Version 02.00 : Recherche avancée

Afin de compléter les données de l'Argus, le programme doit aller chercher des informations supplémentaires dans d'autres site comme Caradisiac ou alors récupérer des informations sur des contrats d'assurance...
