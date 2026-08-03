# Étape du projet

## Version 01.00 : MVP

Pour le Minimum Viable Product, il faut que l'application soit capable de :
- Prendre en entrée une marque et un modèle ainsi qu'un intervalle d'année de production
- Scrapper les fiches techniques des sites de l'Argus et Caradisiac
- Extraire et croiser les informations des fiches technique des deux sites
- Trier les voitures selon des filtres
- Générer un fichier EXCEL de sortie avec les données filtrées
- Envoyer une notification mail de fin d'exécution avec le fichier Excel en PJ

## Version 01.00 : Feuille de route :

Ce qui a été réalisé et testé :
1. Mise en place d'un Logger (import de spdlog et wraper autour de la lib).
2. Classe Entity, classe de base permettant de faire du tracking des objet avec un préfix, un UUID et un full-ID (préfix-UUID) pour le logging.
3. Classe Car, représentant une voiture avec des caractéristique technique et commericales.
4. Fonctions de validation (de string, int et double) et constantes sentinelles pour le projet.
5. Classe HttpClient représentant un client HTTP, équipé de structure représentant des politique anti-blocage pour le scraping.
6. Classe HtmlSaver représentant un objet capable de sauvegarder un HTML dans un fichier texte et de lire / mettre à jour un cache contenant les liens déjà visité et sauvegardé.
7. Classe ExcelReader représentant un objet capable de lire et d'extraire des informations d'un fichier Excel.
8. Classe HtmlParser représentant un parser HTML.
9. Classe GenericCarScraper représentant la base d'un Scraper pour des données de voitures.
10. Classe ArgusScraper et CaradisiacScraper représentant des scraper spécifique pour les site de largus.fr et caradisiac.com. Il sont capables d'extraire et sauvegarder des fiches technique de voitures d'une marque donnée en entrée, d'un modèle, et d'un interval de dates.
11. Class GenericCarMapper représentant la base d'un Mapper pour des données de voitures.
12. Class ArgusMapper et CaradisiacMapper capable de créer un objet Car à partir d'un HTML donné en entrée.
13. Class CrossMapper capable de compléter un objet Car partiel en utilisant deux objets des deux sources largus.fr et caradisiac.com.

&#8303;

Ce qui reste à faire :
1. Créer le service de scraping complet, qui télécharge les fiches technique depuis largus.fr et caradisiac.com en parrallèle, créer des objets Car, les combines et les sauvegardes.
2. Créer un service de suppression des doublons des Car sauvegardée
3. Créer le service de filtrage, prenant en entrée une liste de Car et une série de filtre
4. Créer la classe ExcelWritter capable de créer un fichier Excel affichant les infos d'une liste d'objet Car.
5. Créer un service de mail automatique
6. Créer un runner :
    a. Prend en entrée des paramètres utilisateur (re-scraping forcé, marque, modele, filtres)
    b. Scraping des fiches technique ciblées
    c. Suppression des doublons
    c. Filtrage des voitures
    d. Export Excel
    e. Notification email

&#8303;
___


## Version 02.00 : Recherche avancée

Afin de compléter les données déjà obtenues, récupérer des informations d'autres sources. Idéalement, le but serait d'obtenir des informations sur la fiabilité d'un modèle. Une tentative d'extraction d'information pour la simulation d'un contrat d'assurance pourrait être souhaitable, mais difficilement réalisable.
