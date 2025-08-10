/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : frag.glsl
 * PURPOSE     : Animation project.
 *               Default shader program module.
 *               Vertex shader.
 * PROGRAMMER  : CGSG-SummerCamp'2025.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPos;
out vec3 DrawWPos;
out vec2 DrawTexCoord;

/* The main program function */
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition.xyz, 1);
  
  DrawColor = InColor;
  DrawNormal = mat3(MatrWInvTrans) * InNormal;
  DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  DrawWPos = InPosition;
  DrawTexCoord = InTexCoord;
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */

