#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT_FILE "cam_params.dat"
#define INPUT_FILE "cam_instructions.dat"

#define NUM_FIELDS 10

FILE *output_fp;
FILE *input_fp;

double camPos[3];
double camTarget[3];
double camUp[3];

double camPosDelta[3];
double camTargetDelta[3];
double camUpDelta[3];

int num_frames = 0;

int readLine (){
  int numFields = 0;

  //Read in the deltas and number of frames to apply them over
  for (int i=0; i<3; i++) numFields += fscanf(input_fp, " %lf", &camPosDelta[i]);
  for (int i=0; i<3; i++) numFields += fscanf(input_fp, " %lf", &camTargetDelta[i]);
  for (int i=0; i<3; i++) numFields += fscanf(input_fp, " %lf", &camUpDelta[i]);
  numFields += fscanf(input_fp, " %d", &num_frames);
  return numFields;
}

int main(int argc, char** argv)
{

  //Open files
  output_fp = fopen(OUTPUT_FILE,"w");
  input_fp = fopen(INPUT_FILE,"r");

  //Read in the initial params from the first line of the file
  for (int i=0; i<3; i++) fscanf(input_fp, " %lf", &camPos[i]);
  for (int i=0; i<3; i++) fscanf(input_fp, " %lf", &camTarget[i]);
  for (int i=0; i<3; i++) fscanf(input_fp, " %lf", &camUp[i]);

  while (readLine() == NUM_FIELDS) {

    for (int i=0; i<num_frames; i++) {

      //Apply the deltas
      for (int j=0; j<3; j++) camPos[j] += camPosDelta[j];
      for (int j=0; j<3; j++) camTarget[j] += camTargetDelta[j];
      for (int j=0; j<3; j++) camUp[j] += camUpDelta[j];

      fprintf(
        output_fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
        camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], camUp[0], camUp[1], camUp[2]
      );

    }
  }

  fclose(output_fp);
  fclose(input_fp);

}
