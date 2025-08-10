/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : frag.glsl
 * PURPOSE     : Animation project.
 *               Sky shader program module.
 *               Vertex shader.
 * PROGRAMMER  : CGSG-SummerCamp'2025.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 04.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

layout(location = 0) in vec3 InPosition;

/* The main program function */
void main( void )
{
  gl_Position = vec4(InPosition.xyz, 1);
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */

