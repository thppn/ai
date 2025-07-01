#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "datasets.c"

#define M 5

double centroids[M][2];
double prev_centroids[M][2];
double cluster_means[M][2];

int cluster[1200];
int cluster_population[M];

double error;

double distance(double x1, double y1, double x2, double y2) { 
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); 
} 

void kmeans() {
    int not_stable = 1;
    while(not_stable) {
        for(int i=0; i<1200; i++) {
            double min = 3;
            for(int j=0; j<M; j++) {
                double eucl = distance(sdo[i][0], sdo[i][1], centroids[j][0], centroids[j][1]);
                if (eucl < min) {
                    min = eucl;
                    cluster[i] = j;
                }
            }
        }

        for(int i=0; i < 1200; i++) {
            cluster_means[cluster[i]][0] += sdo[i][0];
            cluster_means[cluster[i]][1] += sdo[i][1];
            cluster_population[cluster[i]]++;
        }

        for(int i=0; i < M; i++) {
            cluster_means[i][0] /= cluster_population[i];
            cluster_means[i][1] /= cluster_population[i];
            cluster_population[i] = 0;


            prev_centroids[i][0] = centroids[i][0];
            prev_centroids[i][1] = centroids[i][1];
            
            centroids[i][0] = cluster_means[i][0];
            centroids[i][1] = cluster_means[i][1];
        }

        not_stable = 0;
        for(int i=0; i < M; i++) {
            if(centroids[i][0] != prev_centroids[i][0] || prev_centroids[i][1] != centroids[i][1]) {
                not_stable = 1;
                break;
            }
        }


    }
    error = 0;
    for (int i = 0; i < 1200; i++)
        error += distance(sdo[i][0], sdo[i][1], centroids[cluster[i]][0], centroids[cluster[i]][1]);
    
    printf("error: %f\n", error);
}

int main(int argc, char** argv) {

    create_sdo();
    srand(time(NULL));
    for(int i = 0; i < M; i++) {
        int centd_i = rand() % 1200;
        centroids[i][0] = sdo[centd_i][0];
        centroids[i][1] = sdo[centd_i][1];
    }
    kmeans();

    FILE* file = fopen(argv[1], "w");
    
    if (file == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        return 1;
    }
    
    // Write data to the file
    fprintf(file, "%d\n", M);
    for (int i = 0; i < M; ++i) 
        fprintf(file, "%f\t%f\n", centroids[i][0], centroids[i][1]);
    

    // Close the file
    write_sdo(file);
    fclose(file);
}

