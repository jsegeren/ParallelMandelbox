#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT_FILE "cam_params.dat"
#define INPUT_FILE "cam_instructions.dat"

#define NUM_FRAMES 5


double camPos[3];
double camTarget[3];
double camPosDelta[3];
double camTargetDelta[3];

int num_frames = 0;

int main(int argc, char** argv)
{

  //Open files
  FILE *output_fp;
  output_fp = fopen(OUTPUT_FILE,"w");

  FILE *input_fp;
  input_fp = fopen(INPUT_FILE,"r");

  //Read in the initial params from the first line of the file
  for (int i=0; i<3; i++) fscanf(input_fp, " %lf", &camPos[i]);
  for (int i=0; i<3; i++) fscanf(input_fp, " %lf", &camTarget[i]);

  while (!feof(input_fp)) {

    //Read in the deltas and number of frames to apply them over
    for (int i=0; i<3; i++) fscanf(input_fp, " %lf", &camPosDelta[i]);
    for (int i=0; i<3; i++) fscanf(input_fp, " %lf", &camTargetDelta[i]);
    fscanf(input_fp, " %d", &num_frames);

    for (int i=0; i<num_frames; i++) {

      //Apply the deltas
      for (int i=0; i<3; i++) camPos[i] += camPosDelta[i];
      for (int i=0; i<3; i++) camTarget[i] += camTargetDelta[i];

      fprintf(
        output_fp, "%.1lf %.1lf %.1lf %.1lf %.1lf %.1lf\n",
        camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2]
      );

    }
  }

  fclose(output_fp);
  fclose(input_fp);

}
