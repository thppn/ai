#include "datasets.c"
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define d 2
#define K 4
#define H1 50
#define H2 100
#define H3 30
#define B 40
#define M 4000
#define F1 "log"
#define F2 "tanh"
#define F3 "tanh"
#define FO "linear"
#define EPOCHS_LIMIT 700

int epochs;
double learning_rate = 0.001;
double threshold = 0.1;
double total_error, prev_error, local_error;

// layer 1
double W1[d][H1];
double dE_dw1[d][H1];
double b1[H1];
double dE_db1[H1];
double u1[H1];
double o1[H1];
double delta1[H1];
// layer 2
double W2[H1][H2];
double dE_dw2[H1][H2];
double b2[H2];
double dE_db2[H2];
double u2[H2];
double o2[H2];
double delta2[H2];
// layer 3
double W3[H2][H3];
double dE_dw3[H2][H3];
double b3[H3];
double dE_db3[H3];
double u3[H3];
double o3[H3];
double delta3[H3];
// layer 4
double W4[H3][K];
double dE_dw4[H3][K];
double b4[K];
double dE_db4[K];
double u4[K];
double o4[K];
double delta4[K];
double error[K];

void print_network() {
  for (int i = 0; i < d; i++)
    for (int j = 0; j < H1; j++)
      printf("layer: %d weights: %d:%d : %f\n", 1, i, j, W1[i][j]);
  for (int i = 0; i < H1; i++)
    printf("bias: %d : %f\n", i, b1[i]);

  for (int i = 0; i < H1; i++)
    for (int j = 0; j < H2; j++)
      printf("layer: %d weights: %d:%d : %f\n", 2, i, j, W2[i][j]);
  for (int i = 0; i < H2; i++)
    printf("bias: %d : %f\n", i, b2[i]);

  for (int i = 0; i < H2; i++)
    for (int j = 0; j < H3; j++)
      printf("layer: %d weights: %d:%d : %f\n", 3, i, j, W3[i][j]);
  for (int i = 0; i < H3; i++)
    printf("bias: %d : %f\n", i, b3[i]);

  for (int i = 0; i < H3; i++)
    for (int j = 0; j < K; j++)
      printf("layer: %d weights: %d:%d : %f\n", 4, i, j, W4[i][j]);
  for (int i = 0; i < K; i++)
    printf("bias: %d : %f\n", i, b4[i]);
}
void write_network(FILE *file) {
  fprintf(file, "%d %d %d %d %d\n", d, H1, H2, H3, K);
  fprintf(file, "%s %s %s %s\n", F1, F2, F3, FO);
  fprintf(file, "%d %d %d %f %f\n", M, B, epochs, threshold, learning_rate);
  for (int i = 0; i < d; i++)
    for (int j = 0; j < H1; j++)
      fprintf(file, "layer: %d weights: %d:%d : %f\n", 1, i, j, W1[i][j]);
  for (int i = 0; i < H1; i++)
    fprintf(file, "bias: %d : %f\n", i, b1[i]);
  for (int i = 0; i < H1; i++)
    for (int j = 0; j < H2; j++)
      fprintf(file, "layer: %d weights: %d:%d : %f\n", 2, i, j, W2[i][j]);
  for (int i = 0; i < H2; i++)
    fprintf(file, "bias: %d : %f\n", i, b2[i]);
  for (int i = 0; i < H2; i++)
    for (int j = 0; j < H3; j++)
      fprintf(file, "layer: %d weights: %d:%d : %f\n", 3, i, j, W3[i][j]);
  for (int i = 0; i < H3; i++)
    fprintf(file, "bias: %d : %f\n", i, b3[i]);
  for (int i = 0; i < H3; i++)
    for (int j = 0; j < K; j++)
      fprintf(file, "layer: %d weights: %d:%d : %f\n", 4, i, j, W4[i][j]);
  for (int i = 0; i < K; i++)
    fprintf(file, "bias: %d : %f\n", i, b4[i]);
}

void print_de() {
  for (int i = 0; i < d; i++)
    for (int j = 0; j < H1; j++)
      printf("layer: %d dedw1: %d:%d : %f\n", 1, i, j, dE_dw1[i][j]);
  for (int i = 0; i < H1; i++)
    printf("bias: %d : %f\n", i, dE_db1[i]);
  for (int i = 0; i < H1; i++)
    for (int j = 0; j < H2; j++)
      printf("layer: %d dedw2: %d:%d : %f\n", 2, i, j, dE_dw2[i][j]);
  for (int i = 0; i < H2; i++)
    printf("bias: %d : %f\n", i, dE_db2[i]);
  for (int i = 0; i < H2; i++)
    for (int j = 0; j < H3; j++)
      printf("layer: %d dedw3: %d:%d : %f\n", 3, i, j, dE_dw3[i][j]);
  for (int i = 0; i < H3; i++)
    printf("bias: %d : %f\n", i, dE_db3[i]);
  for (int i = 0; i < H3; i++)
    for (int j = 0; j < K; j++)
      printf("layer: %d dedw4: %d:%d : %f\n", 4, i, j, dE_dw4[i][j]);
  for (int i = 0; i < K; i++)
    printf("bias: %d : %f\n", i, dE_db4[i]);
}
void print_delta() {
  for (int i = 0; i < K; i++)
    printf("layer 4: delta: %d : %f\n", i, delta4[i]);
  for (int i = 0; i < H3; i++)
    printf("layer 3: delta: %d : %f\n", i, delta3[i]);
  for (int i = 0; i < H2; i++)
    printf("layer 2: delta: %d : %f\n", i, delta2[i]);
  for (int i = 0; i < H1; i++)
    printf("layer 1: delta: %d : %f\n", i, delta1[i]);
}

double activation(double u, char *activation_function) {
  if (strcmp(activation_function, "log") == 0) {
    return 1.0 / (1.0 + exp(-u));
  } else if (strcmp(activation_function, "tanh") == 0) {
    return tanh(u);
  } else if (strcmp(activation_function, "relu") == 0) {
    return u > 0 ? u : 0;
  } else if (strcmp(activation_function, "linear") == 0) {
    return u;
  }
}
double activation_string(double u, char *activation_function) {
  if (strcmp(activation_function, "log") == 0) {
    return 1.0 / (1.0 + exp(-u));
  } else if (strcmp(activation_function, "tanh") == 0) {
    return tanh(u);
  } else if (strcmp(activation_function, "relu") == 0) {
    return u > 0 ? u : 0;
  } else if (strcmp(activation_function, "linear") == 0) {
    return u;
  }
}

double activation_derivative(double u, char *activation_function) {
  if (strcmp(activation_function, "log") == 0) {
    double f = activation(u, "log");
    return f * (1.0 - f);
  } else if (strcmp(activation_function, "tanh") == 0) {
    return 1.0 - pow(tanh(u), 2);
  } else if (strcmp(activation_function, "relu") == 0) {
    return u > 0 ? 1 : 0;
  } else if (strcmp(activation_function, "linear") == 0) {
    return 1;
  }
}

void init_network() {
  srand(time(NULL));
  int i, j;

  for (i = 0; i < d; i++)
    for (j = 0; j < H1; j++)
      W1[i][j] = 2 * (double)rand() / RAND_MAX - 1;
  for (i = 0; i < H1; i++)
    b1[i] = 2 * (double)rand() / RAND_MAX - 1;
  for (i = 0; i < H1; i++)
    for (j = 0; j < H2; j++)
      W2[i][j] = 2 * (double)rand() / RAND_MAX - 1;
  for (i = 0; i < H2; i++)
    b2[i] = 2 * (double)rand() / RAND_MAX - 1;
  for (i = 0; i < H2; i++)
    for (j = 0; j < H3; j++)
      W3[i][j] = 2 * (double)rand() / RAND_MAX - 1;
  for (i = 0; i < H3; i++)
    b3[i] = 2 * (double)rand() / RAND_MAX - 1;
  for (i = 0; i < H3; i++)
    for (j = 0; j < K; j++)
      W4[i][j] = 2 * (double)rand() / RAND_MAX - 1;
  for (i = 0; i < K; i++)
    b4[i] = 2 * (double)rand() / RAND_MAX - 1;
}

void init_de() {
  for (int i = 0; i < d; i++)
    for (int j = 0; j < H1; j++)
      dE_dw1[i][j] = 0;
  for (int i = 0; i < H1; i++)
    dE_db1[i] = 0;
  for (int i = 0; i < H1; i++)
    for (int j = 0; j < H2; j++)
      dE_dw2[i][j] = 0;
  for (int i = 0; i < H2; i++)
    dE_db2[i] = 0;
  for (int i = 0; i < H2; i++)
    for (int j = 0; j < H3; j++)
      dE_dw3[i][j] = 0;
  for (int i = 0; i < H3; i++)
    dE_db3[i] = 0;
  for (int i = 0; i < H3; i++)
    for (int j = 0; j < K; j++)
      dE_dw4[i][j] = 0;
  for (int i = 0; i < K; i++)
    dE_db4[i] = 0;
}

void forward_pass(double *x) {

  for (int i = 0; i < H1; i++) {
    u1[i] = b1[i];
    for (int j = 0; j < d; j++)
      u1[i] += x[j] * W1[j][i];
    o1[i] = activation(u1[i], F1);
  }

  for (int i = 0; i < H2; i++) {
    u2[i] = b2[i];
    for (int j = 0; j < H1; j++)
      u2[i] += o1[j] * W2[j][i];
    o2[i] = activation(u2[i], F2);
  }

  for (int i = 0; i < H3; i++) {
    u3[i] = b3[i];
    for (int j = 0; j < H2; j++)
      u3[i] += o2[j] * W3[j][i];
    o3[i] = activation(u3[i], F3);
  }

  for (int i = 0; i < K; i++) {
    u4[i] = b4[i];
    for (int j = 0; j < H3; j++)
      u4[i] += o3[j] * W4[j][i];
    o4[i] = activation(u4[i], FO);
  }
}

void update_weights() {

  for (int i = 0; i < d; i++)
    for (int j = 0; j < H1; j++)
      W1[i][j] -= learning_rate * dE_dw1[i][j];

  for (int i = 0; i < H1; i++)
    b1[i] -= learning_rate * dE_db1[i];

  for (int i = 0; i < H1; i++)
    for (int j = 0; j < H2; j++)
      W2[i][j] -= learning_rate * dE_dw2[i][j];

  for (int i = 0; i < H2; i++)
    b2[i] -= learning_rate * dE_db2[i];

  for (int i = 0; i < H2; i++)
    for (int j = 0; j < H3; j++)
      W3[i][j] -= learning_rate * dE_dw3[i][j];

  for (int i = 0; i < H3; i++)
    b3[i] -= learning_rate * dE_db3[i];

  for (int i = 0; i < H3; i++)
    for (int j = 0; j < K; j++)
      W4[i][j] -= learning_rate * dE_dw4[i][j];

  for (int i = 0; i < H3; i++)
    b4[i] -= learning_rate * dE_db4[i];

  init_de();
}

void backprop(double *x, double *t) {
  forward_pass(x);

  local_error = 0;
  for (int i = 0; i < K; i++) {
    error[i] = o4[i] - t[i];
    local_error += pow(error[i], 2);
  }
  local_error /= 2; // or K?
  total_error += local_error;

  int i, j;

  for (i = 0; i < K; i++) {
    delta4[i] = error[i] * activation_derivative(t[i], FO);
  }

  for (i = 0; i < H3; i++) {
    delta3[i] = 0;
    for (j = 0; j < K; j++)
      delta3[i] += delta4[j] * W4[i][j];
    delta3[i] *= activation_derivative(u3[i], F3);
  }

  for (i = 0; i < H2; i++) {
    delta2[i] = 0;
    for (j = 0; j < H3; j++)
      delta2[i] += delta3[j] * W3[i][j];
    delta2[i] *= activation_derivative(u2[i], F2);
  }

  for (i = 0; i < H1; i++) {
    delta1[i] = 0;
    for (j = 0; j < H2; j++)
      delta1[i] += delta2[j] * W2[i][j];
    delta1[i] *= activation_derivative(u1[i], F1);
  }

  for (i = 0; i < H1; i++)
    dE_db1[i] += delta1[i];

  for (i = 0; i < d; i++)
    for (j = 0; j < H1; j++)
      dE_dw1[i][j] += (delta1[j] * x[i]);

  for (i = 0; i < H2; i++)
    dE_db2[i] += delta2[i];

  for (i = 0; i < H1; i++)
    for (j = 0; j < H2; j++)
      dE_dw2[i][j] += (delta2[j] * o1[i]);

  for (i = 0; i < H3; i++)
    dE_db3[i] += delta3[i];

  for (i = 0; i < H2; i++)
    for (j = 0; j < H3; j++)
      dE_dw3[i][j] += (delta3[j] * o2[i]);

  for (i = 0; i < K; i++)
    dE_db4[i] += delta4[i];

  for (i = 0; i < H3; i++)
    for (j = 0; j < K; j++)
      dE_dw4[i][j] += (delta4[j] * o3[i]);
}

int decode(double *y) {
  int max_i = 0;
  double max = y[max_i];
  for (int i = 1; i < 4; i++)
    if (y[i] > max) {
      max = y[i];
      max_i = i;
    }
  return max_i;
}

double validate() {
  int valid = 0;
  for (int i = M; i < 2 * M; i++) {
    double x[] = {sdt[i][0], sdt[i][1]};
    double t[] = {0, 0, 0, 0};
    t[sdt_c[i]] = 1.0;

    forward_pass(x);
    if (decode(o4) == sdt_c[i])
      valid++;
  }
  printf("valid: %d / M: %d\n", valid, M);
  return (double)valid / M;
}

int main(int argc, char **argv) {

  init_network();
  init_de();
  create_sdt();

  epochs = 0;
  total_error = prev_error = 0;

  double start, finish;
  start = omp_get_wtime();

  while (epochs < EPOCHS_LIMIT || fabs(prev_error - total_error) > threshold) {

    prev_error = total_error;
    total_error = 0;

    for (int i = 0; i < M; i++) {
      double x[] = {sdt[i][0], sdt[i][1]};
      double t[] = {0, 0, 0, 0};
      t[sdt_c[i]] = 1.0;

      backprop(x, t);
      if (!(i % B))
        update_weights();
    }

    printf("epoch %d: total_error %f\n", epochs++, total_error);
  }
  finish = omp_get_wtime();
  printf("Time: %f\n", finish - start);
  printf("score: %f\n", validate());

  FILE *data_file = fopen(argv[1], "w");

  if (data_file == NULL) {
    fprintf(stderr, "Error opening the file.\n");
    return 1;
  }
  write_network(data_file);
  write_sdt(data_file);
  fclose(data_file);
  // print_network();

  return 0;
}