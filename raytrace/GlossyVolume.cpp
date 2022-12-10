// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "int_pow.h"
#include "GlossyVolume.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 GlossyVolume::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  // Compute the specular part of the glossy shader and attenuate it
  // by the transmittance of the material if the ray is inside (as in
  // the volume shader).

  float3 rho_s = get_specular(hit);
  float s = get_shininess(hit);
  float3 result = make_float3(0.0f);

  for(int i = 0; i < lights.size(); i++){
    float3 accum = make_float3(0.0f);
    for(int j = 0; j < lights.at(i)->get_no_of_samples(); j++){
      float3 dir, L;
      if(lights.at(i)->sample(hit.position, dir, L)) {
        float costheta = dot(dir, hit.shading_normal);
        float3 wr = optix::reflect(-dir, hit.shading_normal);
        if (costheta > 0) {
          accum += L * costheta * (rho_s*((s+2)/(2*M_PIf))*pow(fmax(0.0f,dot(-r.direction,wr)),s));
        }
      }
    }
    result += accum / lights.at(i)->get_no_of_samples();
  }

  float dot_prod = dot(r.direction, hit.shading_normal);
  if(dot_prod > 0.0){ // inside
    result *= Volume::get_transmittance(hit);
  }

  return Volume::shade(r, hit, emit) + result;
}
