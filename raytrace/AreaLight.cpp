// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "IndexedFaceSet.h"
#include "ObjMaterial.h"
#include "mt_random.h"
#include "cdf_bsearch.h"
#include "HitInfo.h"
#include "AreaLight.h"

using namespace optix;

bool AreaLight::sample(const float3& pos, float3& dir, float3& L) const

{
  const IndexedFaceSet& normals = mesh->normals;
  L = make_float3(0.0f);

  // Compute output and return value given the following information.
  //
  // Input:  pos  (the position of the geometry in the scene)
  //
  // Output: dir  (the direction toward the light)
  //         L    (the radiance received from the direction dir)
  //
  // Return: true if not in shadow
  //
  // Relevant data fields that are available (see Light.h and above):
  // shadows             (on/off flag for shadows)
  // tracer              (pointer to ray tracer)
  // normals             (indexed face set of vertex normals)
  // mesh->face_areas    (array of face areas in the light source)
  //
  // Hints: (a) Use mesh->compute_bbox().center() to get the center of 
  //        the light source bounding box.
  //        (b) Use the function get_emission(...) to get the radiance
  //        emitted by a triangle in the mesh.

  // Old worksheet 4 area light
  if(0) {
    float3 t = mesh->compute_bbox().center() - pos;
    dir = normalize(t);

    // Calculate L
    float3 sum = make_float3(0.0f);
    for (int i = 0; i < mesh->face_areas.size(); i++) {
      uint3 face = normals.face(i);
      float3 n1 = normals.vertex(face.x);
      float3 n2 = normals.vertex(face.y);
      float3 n3 = normals.vertex(face.z);
      float costheta = fmaxf(0, dot(-dir, normalize(n1 + n2 + n3)));
      sum += costheta * get_emission(i) * mesh->face_areas.at(i);
    }

    L = (1 / (length(t) * length(t))) * sum;

    if (shadows) {
      float epsilon = 10e-4;
      Ray r(pos, dir, 0, epsilon, length(mesh->compute_bbox().center() - pos) - epsilon);
      HitInfo hit;
      return !tracer->trace_to_closest(r, hit);
    }
  } else {

    // Worksheet 7 area light with true sampling

    // sample a triangle (1 out of n triangles in mesh)
    int rand_index = round(mt_random() * (mesh->face_areas.size() - 1));

    uint3 triangle = mesh->geometry.face(rand_index);

    // Sample position on the triangle
    // Get random numbers
    float rand1 = mt_random_half_open();
    float rand2 = mt_random_half_open();
    //Sample barycentric coords
    float u = 1.0 - sqrtf(rand1);
    float v = (1.0 - rand2) * sqrtf(rand1);
    float w = rand2 * sqrtf(rand1);

    float3 x = u * mesh->geometry.vertex(triangle.x) + v * mesh->geometry.vertex(triangle.y) +
               w * mesh->geometry.vertex(triangle.z);
    float3 t = x - pos;
    dir = normalize(t);

    // Calculate L
    float3 normal = u * normals.vertex(triangle.x) + v * normals.vertex(triangle.y) + w * normals.vertex(triangle.z);
    normal = normalize(normal);
    int n = mesh->face_areas.size();
    float A = mesh->face_areas.at(rand_index);
    float term = (1 / (length(t) * length(t))) * fmaxf(0, dot(-dir, normal));
    L = get_emission(rand_index) * term * n * A;

    if (shadows) {
      float epsilon = 10e-4;
      Ray r(pos, dir, 0, epsilon, length(t) - epsilon);
      HitInfo hit;
      return !tracer->trace_to_closest(r, hit);
    }

    return true;
  }
}

bool AreaLight::emit(Ray& r, HitInfo& hit, float3& Phi) const
{
  // Generate and trace a ray carrying radiance emitted from this area light.
  //
  // Output: r    (the new ray)
  //         hit  (info about the ray-surface intersection)
  //         Phi  (the flux carried by the emitted ray)
  //
  // Return: true if the ray hits anything when traced
  //
  // Relevant data fields that are available (see Light.h and Ray.h):
  // tracer              (pointer to ray tracer)
  // geometry            (indexed face set of triangle vertices)
  // normals             (indexed face set of vertex normals)
  // no_of_faces         (number of faces in triangle mesh for light source)
  // mesh->surface_area  (total surface area of the light source)
  // r.origin            (starting position of ray)
  // r.direction         (direction of ray)

  // Get geometry info
  const IndexedFaceSet& geometry = mesh->geometry;
	const IndexedFaceSet& normals = mesh->normals;
	const float no_of_faces = static_cast<float>(geometry.no_faces());

  // Sample ray origin and direction
 
  // Trace ray
  
  // If a surface was hit, compute Phi and return true

  return false;
}

float3 AreaLight::get_emission(unsigned int triangle_id) const
{
  const ObjMaterial& mat = mesh->materials[mesh->mat_idx[triangle_id]];
  return make_float3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
}
