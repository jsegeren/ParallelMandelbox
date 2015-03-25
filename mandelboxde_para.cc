#include <cmath>
#include <cstdio>
#include <algorithm>
#include <assert.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "color.h"
#include "mandelbox_para.h"

using namespace std;

inline double SQR( double x) { return x*x; }

inline double component_fold(double x) 
{
  if (x>1.0) x = 2.0-x;
  else 
    if (x<-1.0) x = -2.0-x;
  return x;
}

static void boxFold(vec3 &v)
{
  v.x =  component_fold(v.x);
  v.y =  component_fold(v.y);
  v.z =  component_fold(v.z);
}

static void sphereFold(vec3 &v, double r2, double rMin2, double rFixed2)
{
  
  if (r2 < rMin2) 
    v = v*(rFixed2/rMin2);
  else
    if (r2 < rFixed2)
      v = v*(rFixed2/r2);
}


double MandelBoxDE(const vec3 &p0, const MandelBoxParams &params)
{
  vec3 p = p0;
  
  double rMin2   = SQR(params.rMin);
  double rFixed2 = SQR(params.rFixed);
  double escape  = SQR(params.escape_time);
  double scale   = params.scale;
  double dfactor = 1; 
  double r2=-1;
  
  double c1 = fabs(scale - 1.0);
  double c2 = pow( fabs(scale), 1 - params.num_iter);
  
  for (int i=0; i < params.num_iter; i++) 
    {
      // box fold
      boxFold(p);
      r2 = p.Dot(p);      

      // sphere fold
      sphereFold(p,r2,rMin2,rFixed2);
      
      // scale
      p = p*scale+p0;
      
      // estimate distance
      if ( r2 > escape ) break;
      if (r2 < rMin2)    dfactor *= (rFixed2/rMin2);
      else
	if (r2<rFixed2)  dfactor *= (rFixed2/r2);
      dfactor = dfactor*fabs(scale)+1.0;
      assert(dfactor>0);
      //if ( r2 > escape ) break;		
    }
  r2 = p.Magnitude();
  assert(r2>=0);
  double d = (r2 - c1) / dfactor - c2;
  return d;
}


