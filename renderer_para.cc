#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "color.h"
#include "mandelbox_para.h"
#include "camera.h"
#include "vector3d.h"
#include "3d_para.h"

#define THREAD_COUNT 8

extern double getTime();
extern void   printProgress( double perc, double time );

extern void rayMarch (const RenderParams &render_params, const vec3 &from, const vec3  &to, pixelData &pix_data);
extern vec3 getColour(const pixelData &pixData, const RenderParams &render_params,
		      const vec3 &from, const vec3  &direction);

void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, 
		   unsigned char* image)
{
  printf("rendering fractal...\n");
  
  double farPoint[3];
  vec3 to, from;
  
  from.SetDoublePoint(camera_params.camPos);
  
  int height = renderer_params.height;
  int width  = renderer_params.width;
  int i, j, k;

  pixelData pix_data;
  
  double time = omp_get_wtime();

# pragma omp parallel num_threads(THREAD_COUNT)	\
  private(i, j, k, to, pix_data)
{
# pragma omp for schedule(dynamic)  
  for(j = 0; j < height; j++)
    {
      //for each column pixel in the row
      for(i = 0; i <width; i++)
	{
	  vec3 color;
	  if( renderer_params.super_sampling == 1 )
	    {
	      vec3 samples[9];
	      int idx = 0;
	      for(int ssj = -1; ssj < 2; ssj++){
		for(int ssi = -1; ssi< 2; ssi++){
		  UnProject(i+ssi*0.5, j+ssj*0.5, camera_params, farPoint);
		  
		  // to = farPoint - camera_params.camPos
		  to = SubtractDoubleDouble(farPoint,camera_params.camPos);
		  to.Normalize();
		  
		  //render the pixel
		  rayMarch(renderer_params, from, to, pix_data);
		  
		  //get the colour at this pixel
		  samples[idx] = getColour(pix_data, renderer_params, from, to);
		  idx++;
		}
	      }
	      color = (samples[0]*0.05 + samples[1]*0.1 + samples[2]*0.05 + 
		       samples[3]*0.1  + samples[4]*0.4 + samples[5]*0.1  + 
		       samples[6]*0.05 + samples[7]*0.1 + samples[8]*0.05);
	      
	    }
	  else
	  {
	      // get point on the 'far' plane
	      // since we render one frame only, we can use the more specialized method
	      UnProject(i, j, camera_params, farPoint);
	      
	      // to = farPoint - camera_params.camPos
	      to = SubtractDoubleDouble(farPoint,camera_params.camPos);
	      to.Normalize();
	      
	      //render the pixel
	      rayMarch(renderer_params, from, to, pix_data);
	      
	      //get the colour at this pixel
	      color = getColour(pix_data, renderer_params, from, to);
	  }
	  
	  //save colour into texture
	  k = (j * width + i)*3;
	  image[k+2] = (unsigned char)(color.x * 255);
	  image[k+1] = (unsigned char)(color.y * 255);
	  image[k]   = (unsigned char)(color.z * 255);
	}
      //printProgress((j+1)/(double)height,getTime()-time);
    }
}
  printf("\nrendering took %f seconds", omp_get_wtime()-time);
  printf("\nrendering done:\n");
}
