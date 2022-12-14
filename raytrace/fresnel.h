// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#ifndef FRESNEL_H
#define FRESNEL_H

inline float fresnel_r_s(float cos_theta1, float cos_theta2, float ior1, float ior2)
{
  // Compute the perpendicularly polarized component of the Fresnel reflectance
  float t1 = ior1 * cos_theta1;
  float t2 = ior2 * cos_theta2;
  return (t1 - t2)/(t1 + t2);
}

inline float fresnel_r_p(float cos_theta1, float cos_theta2, float ior1, float ior2)
{
  // Compute the parallelly polarized component of the Fresnel reflectance
  float t1 = ior2 * cos_theta1;
  float t2 = ior1 * cos_theta2;
  return (t1 - t2)/(t1 + t2);
}

inline float fresnel_R(float cos_theta1, float cos_theta2, float ior1, float ior2)
{
  // Compute the Fresnel reflectance using fresnel_r_s(...) and fresnel_r_p(...)
  return 0.5 * (fresnel_r_s(cos_theta1, cos_theta2, ior1, ior2)*fresnel_r_s(cos_theta1, cos_theta2, ior1, ior2)
              + fresnel_r_p(cos_theta1, cos_theta2, ior1, ior2)*fresnel_r_p(cos_theta1, cos_theta2, ior1, ior2));
}

#endif // FRESNEL_H
