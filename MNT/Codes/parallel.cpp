#include <iostream>
#include <mpi.h>
#include "mnt.h"

int main(int argc, char **argv) {
    int pid, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    char* filename = argv[1];
    int root = atoi(argv[2]);

    int nb_lignes, nb_cols;
    float no_value;

    float* terrain;

    if (pid==root) {
        lecture(filename, &nb_lignes, &nb_cols, &no_value, &terrain);
        std::cout << "le terrain" << std::endl;
        affichageTerrain(nb_lignes, nb_cols, terrain);
    }

    /* A compléter */
    //broadcast des dimensions et de la valeur no_data
    MPI_Bcast(&nb_lignes, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&nb_cols, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&no_value, 1, MPI_FLOAT, root, MPI_COMM_WORLD);
    
    //affichage des infos recues
    std::cout << "pid: " << pid << " nb_lignes: " << nb_lignes << " nb_cols: " << nb_cols << " no_value: " << no_value << std::endl;

    //distribution du terrain en partant u principe que nb_lignes est un multiple de nprocs
    int ligne_proc = nb_lignes / nprocs;
    float* terrain_par_proc = new float[(ligne_proc * nb_cols)+2*nb_cols]; 
    
    MPI_Scatter(terrain+nb_cols, ligne_proc*nb_cols, MPI_FLOAT, terrain_par_proc+nb_cols, ligne_proc*nb_cols, MPI_FLOAT, root, MPI_COMM_WORLD);
    
    //affichage du terrain partiel
    std::cout << "pid: " << pid << " terrain partiel" << std::endl;
    affichageTerrain(ligne_proc+2, nb_cols, terrain_par_proc);
    

    //envoie chaque proc de la ligne du dessus et du dessous
   if (pid==0) {
        MPI_Send(terrain+(ligne_proc*nb_cols)+nb_cols, nb_cols, MPI FLOAT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(terrain_par_proc+(ligne_proc+1)*nb_cols, nb_cols, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //affichage apres reception
        std::cout << "pid: " << pid << " terrain partiel apres reception" << std::endl;
        affichageTerrain(ligne_proc+2, nb_cols, terrain_par_proc);  
        
    } else if (pid==nprocs-1) {
        MPI_Recv(terrain_par_proc, nb_cols, MPI_FLOAT, pid-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //affichage apres reception
        std::cout << "pid: " << pid << " terrain partiel apres reception" << std::endl;
        affichageTerrain(ligne_proc+2, nb_cols, terrain_par_proc);
        MPI_Send(terrain+(pid*ligne_proc*nb_cols), nb_cols, MPI_FLOAT, pid-1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(terrain_par_proc, nb_cols, MPI_FLOAT, pid-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //affichage apres reception
        std::cout << "pid: " << pid << " terrain partiel apres reception" << std::endl;
        affichageTerrain(ligne_proc+2, nb_cols, terrain_par_proc);
        MPI_Send(terrain+(pid*ligne_proc*nb_cols)+nb_cols, nb_cols, MPI_FLOAT, pid+1, 0, MPI_COMM_WORLD);
        MPI_Recv(terrain_par_proc+(ligne_proc+1)*nb_cols, nb_cols, MPI_FLOAT, pid+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //affichage apres reception
        std::cout << "pid: " << pid << " terrain partiel apres reception" << std::endl;
        affichageTerrain(ligne_proc+2, nb_cols, terrain_par_proc);
        MPI_Send(terrain+(pid*ligne_proc*nb_cols), nb_cols, MPI_FLOAT, pid-1, 0, MPI_COMM_WORLD);
    }
   /*
    int dest_up = pid - 1;
    int dest_down = pid + 1;

    // Envoi de la ligne du dessus et réception de la ligne du dessous
    if (pid > 0) {
        MPI_Sendrecv(terrain_par_proc + nb_cols, nb_cols, MPI_FLOAT, dest_up, 0,
                    terrain_par_proc, nb_cols, MPI_FLOAT, dest_up, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if (pid < nprocs - 1) {
        MPI_Sendrecv(terrain_par_proc + ligne_proc * nb_cols, nb_cols, MPI_FLOAT, dest_down, 0,
                    terrain_par_proc + (ligne_proc + 1) * nb_cols, nb_cols, MPI_FLOAT, dest_down, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    */


   /*
    MPI_Request requests[4];// 
    MPI_Request reqs[4];
    int req_count = 0;

    // Envoi/Reception vers le voisin du dessus
        if (pid > 0) {
            MPI_Irecv(terrain_par_proc, nb_cols, MPI_FLOAT, pid - 1, 0, MPI_COMM_WORLD, &reqs[req_count++]);
            MPI_Isend(terrain_par_proc + nb_cols, nb_cols, MPI_FLOAT, pid - 1, 0, MPI_COMM_WORLD, &reqs[req_count++]);
        }

        // Envoi/Reception vers le voisin du dessous
        if (pid < nprocs - 1) {
            MPI_Irecv(terrain_par_proc + (ligne_proc + 1) * nb_cols, nb_cols, MPI_FLOAT, pid + 1, 0, MPI_COMM_WORLD, &reqs[req_count++]);
            MPI_Isend(terrain_par_proc + ligne_proc * nb_cols, nb_cols, MPI_FLOAT, pid + 1, 0, MPI_COMM_WORLD, &reqs[req_count++]);
        }

        // Attente de la fin des communications
        MPI_Waitall(req_count, reqs, MPI_STATUSES_IGNORE);
    */




    MPI_Finalize();
    return 0;
}