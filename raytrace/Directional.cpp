// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <sstream>
#include <string>
#include <optix_world.h>
#include "HitInfo.h"
#include "Directional.h"

using namespace std;
using namespace optix;

bool Directional::sample(const float3& pos, float3& dir, float3& L) const
{
  // Compute output and return value given the following information.
  //
  // Input:  pos (the position of the geometry in the scene)
  //
  // Output: dir (the direction toward the light)
  //         L   (the radiance received from the direction dir)
  //
  // Return: true if not in shadow
  //
  // Relevant data fields that are available (see Directional.h and Light.h):
  // shadows    (on/off flag for shadows)
  // tracer     (pointer to ray tracer)
  // light_dir  (direction of the emitted light)
  // emission   (radiance of the emitted light)

  float3 t = -light_dir;
  dir = normalize(t);

  // Calculate L
  L = emission;

  if(shadows) {
    float epsilon = 10e-4;
    Ray r(pos, dir, 0, epsilon, RT_DEFAULT_MAX);
    HitInfo hit;
    return !tracer->trace_to_closest(r, hit);
  }

  return true;
}

string Directional::describe() const
{
  ostringstream ostr;
  ostr << "Directional light (emitted radiance " << emission << ", direction " << light_dir << ").";
  return ostr.str();
}
