/**
 * @brief Test harness component for sending fuzzed inputs into a Simulink model.
 *
 * This file contains the implementation of a test harness that reads input values from
 * a text file and sends them as inputs to a Simulink model. The model processes the inputs
 * and generates outputs, which are printed to the console.
 *
 */


#include <stddef.h>
#include <stdio.h>            /* This example main program uses printf/fflush */
#include <stdlib.h>           /* For atof() */
#include "mayhemit.h"         /* Model header file */

/**
 * @brief Reads a file containing numerical values and stores them in an array.
 *
 * This function opens a file with the given filename, reads the numerical values
 * contained in the file, and stores them in an allocated array. The number of
 * values read is also returned.
 *
 * @param filename The name of the file containing the input values.
 * @param input_values A pointer to a pointer to an array of doubles. This function
 *                     allocates memory for the array and stores its address in
 *                     *input_values. The caller is responsible for freeing this
 *                     memory.
 * @param num_values A pointer to a size_t variable to store the number of values
 *                   read from the file.
 * @note The input file should have one numerical value per line. If the file
 *       cannot be opened, the function will print an error message and exit
 *       with a non-zero status code.
 */
void read_input_values(const char *filename, double **input_values, size_t *num_values) {
  FILE *file;
  size_t count = 0;
  double value;
  char line[256];

  file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }

  while (fgets(line, sizeof(line), file)) {
    count++;
  }

  fseek(file, 0, SEEK_SET);
  *input_values = (double *)malloc(count * sizeof(double));
  *num_values = count;

  count = 0;
  while (fgets(line, sizeof(line), file)) {
    value = atof(line);
    (*input_values)[count++] = value;
  }

  fclose(file);
}

void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(mayhemit_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  mayhemit_step();

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

/**
 * @brief The main function of a test harness for sending fuzzed inputs into a Simulink model.
 *
 * This function reads input values from a text file, initializes the Simulink model,
 * feeds the input values into the model, and prints the model's output for each input.
 * The model is terminated after processing all inputs, and allocated memory is freed.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments, where argv[1] should be the name of
 *             the input file containing the fuzzed input values.
 * @return Returns 0 on successful execution; returns 1 if the number of command-line
 *         arguments is incorrect.
 *
 * @note The input values file should have one numerical value per line.
 *       This function uses the read_input_values() function to read input values
 *       from the file and store them in an array.
 *       The Simulink model should be implemented with appropriate input and output
 *       variables (mayhemit_U.In1 and mayhemit_Y.Out1) and functions
 *       (mayhemit_initialize(), mayhemit_step(), and mayhemit_terminate()).
 */
int_T main(int_T argc, const char *argv[]) {
  double *input_values;
  size_t num_values, i;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s input_values.txt\n", argv[0]);
    return 1;
  }

  read_input_values(argv[1], &input_values, &num_values);

  /* Initialize model */
  mayhemit_initialize();

  for (i = 0; i < num_values; i++) {
    /* Set model input */
    mayhemit_U.In1 = input_values[i];

    /* Step the model */
    mayhemit_step();

    /* Get model output */
    printf("Input: %f, Output: %f\n", mayhemit_U.In1, mayhemit_Y.Out1);
  }

  /* Terminate model */
  mayhemit_terminate();

  free(input_values);
  return 0;

}
