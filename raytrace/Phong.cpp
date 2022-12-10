// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Phong.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 Phong::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  float3 rho_d = get_diffuse(hit);
  float3 rho_s = get_specular(hit);
  float s = get_shininess(hit);
  float3 result = make_float3(0.0f);

  // Implement Phong reflection here.
  //
  // Input:  r          (the ray that hit the material)
  //         hit        (info about the ray-surface intersection)
  //         emit       (passed on to Emission::shade)
  //
  // Return: radiance reflected to where the ray was coming from
  //
  // Relevant data fields that are available (see Lambertian.h, Ray.h, and above):
  // lights             (vector of pointers to the lights in the scene)
  // hit.position       (position where the ray hit the material)
  // hit.shading_normal (surface normal where the ray hit the material)
  // rho_d              (difuse reflectance of the material)
  // rho_s              (specular reflectance of the material)
  // s                  (shininess or Phong exponent of the material)
  //
  // Hint: Call the sample function associated with each light in the scene.

  for(int i = 0; i < lights.size(); i++){
    float3 accum = make_float3(0.0f);
    for(int j = 0; j < lights.at(i)->get_no_of_samples(); j++){
      float3 dir, L;
      if(lights.at(i)->sample(hit.position, dir, L)) {
        float costheta = dot(dir, hit.shading_normal);
        float3 wr = optix::reflect(-dir, hit.shading_normal);
        if (costheta > 0) {
          accum += L * costheta * (rho_d/M_PIf + rho_s*((s+2)/(2*M_PIf))*pow(fmax(0.0f,dot(-r.direction,wr)),s));
        }
      }
    }
    result += accum / lights.at(i)->get_no_of_samples();
  }

  float3 wo = make_float3(1.0f);
  return result + Emission::shade(r, hit, emit);
}
