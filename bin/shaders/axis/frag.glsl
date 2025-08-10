/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : frag.glsl
 * PURPOSE     : Animation project.
 *               Axis shader program module.
 *               Fragment shader.
 * PROGRAMMER  : CGSG-SummerCamp'2025.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec3 DrawWPos;
in vec2 DrawTexCoord;

/* The main program function */
void main( void )
{
  OutColor = vec4(DrawColor.xyz, 1);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */

