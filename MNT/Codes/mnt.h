#ifndef MNT_H
#define MNT_H
#include <stdlib.h>
const int no_dir_value = -9; // choisi pour initialiser l'absence de données dans la direction
const int no_acc_value = -1;

void lecture(std::string filename, int *nb_lignes, int *nb_cols, float *no_value, float** terrain);
void affichageTerrain(int n, int m, float* t);
int* initialisationDirection(int nb_lignes, int nb_cols);
void affichageDirection(int n, int m, int* t);
void calculDirection(int n, int m, float no_value, float* t, int* d);
int* initialisationAccumulation(int nb_lignes, int nb_cols);
void affichageAccumulation(int nb_lignes, int nb_cols, int* accumulation);
void calculAccumulation(int nb_lignes, int nb_cols, int* direction, int* accumulation);

#endif //MNT_H
