/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : frag.glsl
 * PURPOSE     : Animation project.
 *               Sky shader program module.
 *               Fragment shader.
 * PROGRAMMER  : CGSG-SummerCamp'2025.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 04.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */


layout(binding = 0) uniform sampler2D Tex;

layout(location = 0) out vec4 OutColor;

/* The main program function */
void main( void )
{
  float 
    xp = CamRightWp.w * gl_FragCoord.x / CamLocFrameW.w - CamRightWp.w / 2,
    yp = CamUpHp.w * gl_FragCoord.y / CamAtFrameH.w - CamUpHp.w / 2;
  vec3 Dir = normalize(CamDirProjDist.xyz * CamDirProjDist.w + CamRightWp.xyz * xp + CamUpHp.xyz * yp);
  float
    pi = acos(-1),
    theta = -acos(-Dir.y),
    phi = atan(Dir.x, Dir.z);
  vec2 texcoord = vec2(phi / 2 / pi, theta / pi);

  vec4 tc = texture(Tex, texcoord);
  OutColor = vec4(tc.rgb, 1); 
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */

