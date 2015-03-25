#include <assert.h>
#include <algorithm>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "color.h"
#include "renderer_para.h"

extern double DE(const vec3 &p);
void normal (const vec3 & p, vec3 & normal);

void rayMarch(const RenderParams &render_params, const vec3 &from, const vec3  &direction,
	      pixelData& pix_data)
{
  double dist = 0.0;
  double totalDist = 0.0;
  
  // We will adjust the minimum distance based on the current zoom
  double eps = pow(10.0, render_params.detail); 
  double epsModified = 0.0;
  
  int steps;
  vec3 p;
  for ( steps = 0; steps < render_params.maxRaySteps; steps++) 
    {      
      p = from + direction * totalDist;
      dist = DE(p);
      totalDist += 0.9*dist;
      
      epsModified = totalDist*eps;
      if (dist < epsModified || totalDist > render_params.maxDistance) 
	break;
    }
  
  vec3 hitNormal;
  if (dist < epsModified) 
    {
      //we didnt escape
      pix_data.escaped = false;
      
      // We hit something, or reached MaxRaySteps
      pix_data.hit = p;
      
      //figure out the normal of the surface at this point
      vec3 normPos = p - direction * epsModified;
      normal(normPos, pix_data.normal);
    }
  else 
    //we have the background colour
    pix_data.escaped = true;
}


void normal(const vec3 & p, vec3 & normal)
{
  // compute the normal at p
  const double sqrt_mach_eps = 1.4901e-08;

  double eps = std::max( p.Magnitude(), 1.0 )*sqrt_mach_eps;

  vec3 e1(eps, 0,   0);
  vec3 e2(0  , eps, 0);
  vec3 e3(0  , 0, eps);
  
  normal = vec3(DE(p+e1)-DE(p-e1), DE(p+e2)-DE(p-e2), DE(p+e3)-DE(p-e3));
  
  normal.Normalize();
}
