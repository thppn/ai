#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define get_rand(min, max) ((rand() / (float) RAND_MAX) * (max-min)) + min

float sdt[8000][2];
int sdt_c[8000];

int categorize(int i) {

    float x_one_minus = pow(sdt[i][0]-0.5, 2);
    float x_two_minus = pow(sdt[i][1]-0.5, 2);
    float x_one_plus = pow(sdt[i][0]+0.5, 2);
    float x_two_plus = pow(sdt[i][1]+0.5, 2);

    if(x_one_minus + x_two_minus < 0.2 && sdt[i][1] > 0.5) {
        return 0;
    } else if(x_one_minus + x_two_minus < 0.2 && sdt[i][1] < 0.5) {
        return 1;
    } else if(x_one_plus + x_two_plus < 0.2 && sdt[i][1] > -0.5) {
        return 0;
    } else if(x_one_plus + x_two_plus < 0.2 && sdt[i][1] < -0.5) {
        return 1;
    } else if(x_one_minus + x_two_plus < 0.2 && sdt[i][1] > -0.5) {
        return 0;
    } else if(x_one_minus + x_two_plus < 0.2 && sdt[i][1] < -0.5) {
        return 1;
    } else if(x_one_plus + x_two_minus < 0.2 && sdt[i][1] > 0.5) {
        return 0;
    } else if(x_one_plus + x_two_minus < 0.2 && sdt[i][1] < 0.5) {
        return 1;
    } else {
        if(sdt_c[0] > 0) return 2;
        if(sdt_c[0] < 0) return 3;
    }
}

void create_sdt() {
    time_t t;
    srand((unsigned) time(&t));
    
    for(int i=0;i < 8000;i++) {
        sdt[i][0] = get_rand(-1 ,1);
        sdt[i][1] = get_rand(-1, 1);
        sdt_c[i] = categorize(i);
    }
}

void print_sdt() {
    for(int i=0;i<4000;i++) {
        printf("%f\t%f\t%d\n", sdt[i][0], sdt[i][1],  sdt_c[i]);
        printf("%f\t%f\t%d\n", sdt[4000+i][0], sdt[4000+i][1], sdt_c[i]);
    }
}

int write_sdt(FILE *file) {

    for(int i=0;i<4000;i++) {
        fprintf(file, "%f\t%f\t%d\n", sdt[i][0], sdt[i][1],  sdt_c[i]);
    }

    return 0;
}

// main
int write_files() {

    time_t t;
    int i;
    FILE *fp_train, *fp_valid;
    
    srand((unsigned) time(&t));
    create_sdt();
 
    fp_train = fopen("train.txt", "w");
    fp_valid = fopen("valid.txt", "w");

    if(fp_train == NULL || fp_valid == NULL) {
        printf("file couldn't be opened\n");
        return 1;
    }

    for(i=0;i<4000;i++) {
        fprintf(fp_train, "%f\t%f\t%d\n", sdt[i][0], sdt[i][1],  sdt_c[i]);
        fprintf(fp_valid, "%f\t%f\t%d\n", sdt[4000+i][0], sdt[4000+i][1], sdt_c[i]);
    }

    fclose(fp_train);
    fclose(fp_valid);

    return 0;
}


