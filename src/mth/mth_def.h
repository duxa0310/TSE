/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_def.h
 * PURPOSE     : Tough Space Exploration project.
 *               Mathematics library.
 *               Common definitions module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 31.07.2025.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_def_h_
#define __mth_def_h_

/* Define numeric types for common usage */
typedef float FLT;
typedef double DBL;
typedef long long INT64;
typedef unsigned long long UINT64;

/* Pi math constant definition */
#define PI 3.1415926535897932384626433832795

/* Degrees to radians conversion macro definition */
#define D2R(A) ((A) * (PI / 180.0))
#define R2D(A) ((A) * (180.0 / PI))
#define DegreeToRadian(A) D2R(A)
#define RadianToDegree(A) R2D(A)

#endif /* __mth_def_h_ */

/* END OF 'mth_def.h' FILE */
