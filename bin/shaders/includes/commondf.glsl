/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : commondf.glsl
 * PURPOSE     : Animation project.
 *               Shader program module.
 *               Common definitions shader.
 * PROGRAMMER  : CGSG-SummerCamp'2025.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 02.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

layout(std140, binding = 0) buffer Camera
{
  mat4 MatrView;
  mat4 MatrProj;
  mat4 MatrVP;
  vec4 CamLocFrameW;
  vec4 CamDirProjDist;
  vec4 CamRightWp;
  vec4 CamUpHp;
  vec4 CamAtFrameH;
};

layout(std140, binding = 1) buffer Syncronization
{
  vec4 SyncGlobalTimeGlobalDeltaTimeTimeDeltaTime;
};

layout(std140, binding = 2) buffer Primitive
{
  mat4 MatrWVP;
  mat4 MatrW;
  mat4 MatrWInvTrans;
  vec4 RndIsWireIsAny;
};

layout(std140, binding = 3) buffer Material
{
  vec4 Ka;
  vec4 KdTrans;
  vec4 KsPh;
  bool TextureFlags[8];
};

/* Shade point function */
vec3 Shade( vec3 P, vec3 N, vec3 Kd, vec3 Ks, float Ph, vec3 L, vec3 LC )
{
  vec3
    color = vec3(0),
    V = normalize(P - CamLocFrameW.xyz);
 
  N = faceforward(N, V, N);

  /* Diffuse color */
  color += max(0, dot(N, L)) * Kd * LC;

  /* Specular color */
  vec3 R = reflect(V, N);
  color += pow(max(0, dot(R, L)), Ph) * Ks * LC;
  
  return color;
} /* End of 'Shade' function */

/* END OF 'commondf.glsl' FILE */
