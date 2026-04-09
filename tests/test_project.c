#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//structure de la date 

typedef struct 
{
int jour;
int mois;
int annee;
}Date;

//structure pour un lot de medicaments

typedef struct
{
char id_lot[20];  //identifiant unique du lot qui doit etre sous la forme (ex: Lot-2026-001)
Date date_fabrication;
Date date_expiration;   
int quantite; 
}Lot;


//Verification si l'année est bissextile

int est_bissextile(int annee){
    return(annee%4 == 0 && annee%100 !=0) || (annee % 400 == 0);
}

//Retourne le nombre de jours dans un mois donné

int jour_par_mois(int mois, int annee){
    if(mois == 2){
        return est_bissextile(annee)? 29:28; //Février a 29 jours si l'année est bissextile, sinon 28 jours
        }
    if(mois == 4 || mois == 6 || mois == 9 || mois == 11){
        return 30;
        }
    return 31;
    
}

//Valider la date 

int date_valide(Date d){  //Vérifie que l'année est entre 1900 et 2100, que le mois est entre 1 et 12, et que le jour est valide pour le mois et l'année donnés
    if(d.annee < 1900 || d.annee > 2100) return 0;
    if(d.mois < 1 || d.mois > 12) return 0;
    if(d.jour < 1 || d.jour > jour_par_mois(d.mois, d.annee)) return 0;
    return 1;
}

//affiche une date 

void afficher_date(Date d){
    printf("%02d/%02d/%04d", d.jour, d.mois, d.annee);
}

//Création d'un lot dynamique 

Lot* creer_lot(const char* id, Date fab, Date exp, int qte){ //crée un nouveau lot en allouant de la mémoire dynamiquement et en initialisant les champs avec les valeurs fournies. Si les dates ne sont pas valides ou si l'allocation mémoire échoue, la fonction retourne NULL.
    if(!date_valide(fab)||!date_valide(exp)){
        printf("Erreur : date invalide.\n");
        return NULL;
    }

    Lot*nouveau=(Lot*)malloc(sizeof(Lot));//Alloue de la mémoire pour un nouveau lot
    if(nouveau==NULL){
        printf("Erreur d'allocation mémoire.\n");
        return NULL;
    }

    strcpy(nouveau->id_lot,id);

    nouveau->date_fabrication=fab;
    nouveau->date_expiration=exp;
    nouveau->quantite=qte;

    return nouveau;
}

//Afiche les informations d'un lot

void afficher_lot(const Lot* lot){
    if(lot==NULL)return;
    printf("Lot ID : %s\n", lot->id_lot);
    printf("Fabriqué le : ");
    afficher_date(lot->date_fabrication);
    printf("\n Expire le : ");
    afficher_date(lot->date_expiration);
    printf("\n Quantité : %d unités\n", lot->quantite);
}

//libère la mémoire

void liberer_lot(Lot* lot){
    free(lot);
}

//Saisir une date depuis l'utilisateur

Date saisir_date(const char* message){
    Date d;
    int valide = 0;
    while(!valide){
        printf("%s (jj mm aaaa): ", message);
        scanf("%d %d %d", &d.jour, &d.mois, &d.annee);
        if(date_valide(d)){
            valide = 1;
        } else {
            printf("Erreur : date invalide. Réessayez.\n");
        }
    }
    return d;
}

int main(){
    int capacite = 10;
    int nb_lots = 0;
    Lot** lots = (Lot**)malloc(capacite * sizeof(Lot*));

    // Si nb_lots >= capacite, agrandir le tableau
    if(nb_lots >= capacite){
        capacite *= 2;
        lots = (Lot**)realloc(lots, capacite * sizeof(Lot*));
    }

    char continuer = 'o';

    printf("=== Gestion des Lots de Medicaments ===\n\n");

    while (continuer == 'o' && nb_lots < capacite) {
        printf("\n--- Ajouter un nouveau lot ---\n");
        
        char id[20];
        printf("Entrez l'ID du lot (ex: Lot-2026-001): ");
        scanf("%s", id);

        Date fab = saisir_date("Date de fabrication");
        Date exp = saisir_date("Date d'expiration");

        int qte;
        printf("Quantité: ");
        scanf("%d", &qte);

        Lot* lot = creer_lot(id, fab, exp, qte);
        if (lot != NULL) {
            lots[nb_lots++] = lot;
            printf("Lot ajouté avec succès!\n");
        }

        printf("Ajouter un autre lot? (o/n): ");
        scanf(" %c", &continuer);
    }

    printf("\n=== Affichage de tous les lots ===\n\n");
    for (int i = 0; i < nb_lots; i++) {
        printf("--- Lot %d ---\n", i+1);
        afficher_lot(lots[i]);
        printf("\n");
        liberer_lot(lots[i]);
    }

    printf("Total de lots gérés: %d\n", nb_lots);
    return 0;
}

