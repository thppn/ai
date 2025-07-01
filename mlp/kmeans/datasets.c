#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define get_rand(min, max) ((rand() / (float) RAND_MAX) * (max-min)) + min

float sdo[1200][2];

void create_sdo() {
    time_t t;
    srand((unsigned) time(&t));
  
    for(int i=0;i < 150;i++) {
        //1
        sdo[i][0] = get_rand(0.8, 1.2);
        sdo[i][1] = get_rand(0.8, 1.2);
        //2
        sdo[i+150][0] = get_rand(0.0, 0.5);
        sdo[i+150][1] = get_rand(0.0, 0.5);
        //3
        sdo[i+300][0] = get_rand(0.0, 0.5);
        sdo[i+300][1] = get_rand(1.5, 2.0);
        //4
        sdo[i+450][0] = get_rand(1.5 ,2.0);
        sdo[i+450][1] = get_rand(0.0, 0.5);
        //5
        sdo[i+600][0] = get_rand(1.5, 2.0);
        sdo[i+600][1] = get_rand(1.5, 2.0);
        if(i < 75) {
            //6
            sdo[i+750][0] = get_rand(0.8, 1.2);
            sdo[i+750][1] = get_rand(0.0, 0.4);
            //7
            sdo[i+825][0] = get_rand(0.8, 1.2);
            sdo[i+825][1] = get_rand(1.6, 2.0);
            //8
            sdo[i+900][0] = get_rand(0.3, 0.7);
            sdo[i+900][1] = get_rand(0.8, 1.2);
            //9
            sdo[i+975][0] = get_rand(1.3, 1.7);
            sdo[i+975][1] = get_rand(0.8, 1.2);
        }
        //10
        sdo[i+1050][0] = get_rand(0.0 , 2.0);
        sdo[i+1050][1] = get_rand(0.0 , 2.0);
    }
}

void print_sdo() {
    for(int i=0;i<1200;i++) {
        printf("%f\t%f\n", sdo[i][0], sdo[i][1]);
    }
}

int write_sdo(FILE *file) {

    for(int i=0;i<1200;i++) {
        fprintf(file, "%f\t%f\n", sdo[i][0], sdo[i][1]);
    }
    return 0;
}

// main
int write_files() {

    time_t t;
    int i;
    FILE *fp_kmean;
    
    srand((unsigned) time(&t));
    create_sdo();
 

    fp_kmean = fopen("kmean.txt", "a");

    if(fp_kmean == NULL) {
        printf("file couldn't be opened\n");
        return 1;
    }

    for(i=0;i<1200;i++) {
        fprintf(fp_kmean, "%f\t%f\n", sdo[i][0], sdo[i][1]);
    }

    fclose(fp_kmean);

    return 0;
}


