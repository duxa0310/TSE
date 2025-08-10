/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_ray.h
 * PURPOSE     : Tough Space Exploration project.
 *               Mathematics library.
 *               3D ray handle module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 31.07.2025.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_ray_h_
#define __mth_ray_h_

#include "mth_def.h"

/* Space math namespace */
namespace mth
{
  /* 3D ray representation type */
  template<typename type>
    class ray
    {
    public:
      vec3<type>
        Org, // Base origin point
        Dir; // Normalized direction vector

      /* Default type constructor function */
      ray( VOID )
      {
      } /* End of 'ray' function */

      /* Type constructor function.
       * ARGUMENTS:
       *   - base point & normalized direction vector references:
       *       const vec3<type> &O, &D;
       */
      ray( const vec3<type> &O, const vec3<type> &D ) :
        Org(O), Dir(D.Normalizing())
      {
      } /* End of 'ray' function */

      /* Get ray point by distance from base function.
       * ARGUMENTS:
       *   - distance from base:
       *       type T;
       * RETURNS:
       *   (vec3<type>) ray point.
       */
      vec3<type> operator()( type T ) const
      {
        return Org + Dir * T;
      } /* End of 'operator()' function */

      /* Check intersection with AABB boundaries function.
       * ARGUMENTS:
       *   - AABB boundary vector references:
       *       const vec3 &MinBVB, &MaxBB;
       * RETURNS:
       *   (BOOL) TRUE, if ray intersects with AABB, otherwise FALSE.
       */
      BOOL IsBoxIntersect( const vec3<type> &MinBB, const vec3<type> &MaxBB ) const
      {
        static DBL Threshold = 1E-8;
        DBL tnear = 0, tfar = 0, t = 0;
        INT tnear_no = -1, tfar_no = -1;
 
        for (INT i = 0; i < 3; i++)
        {
          if (abs(Dir[i]) < Threshold)
          {
            if (Org[i] - MinBB[i] < -Threshold || Org[i] - MaxBB[i] > Threshold)
              return FALSE;
          }
          else if (Dir[i] > Threshold)
          {
            /* Ray starts outside and go away */
            if (Org[i] - MaxBB[i] > Threshold)
              return FALSE;
            /* Check if ray starts inside */
            if (Org[i] - MinBB[i] < -Threshold)
            {
              /* Outside: tnear is present */
              t = (MinBB[i] - Org[i]) / Dir[i];
              if (tnear_no == -1 || tnear - t < -Threshold)
                tnear_no = i * 2 + 0, tnear = t;
            }
            t = (MaxBB[i] - Org[i]) / Dir[i];
            if (tfar_no == -1 || tfar - t > Threshold)
              tfar_no = i * 2 + 1, tfar = t;
          }
          else
          {
            /* Ray starts outside and go away */
            if (Org[i] - MinBB[i] < -Threshold)
              return FALSE;
            /* Check if ray starts inside */
            if (Org[i] - MaxBB[i] > Threshold)
            {
              /* Outside: tnear is present */
              t = (MaxBB[i] - Org[i]) / Dir[i];
              if (tnear_no == -1 || tnear - t < -Threshold)
                tnear_no = i * 2 + 0, tnear = t;
            }
            t = (MinBB[i] - Org[i]) / Dir[i];
            if (tfar_no == -1 || tfar - t > Threshold)
              tfar_no = i * 2 + 1, tfar = t;
          }
        }
        if (tnear_no != -1 && tnear - tfar > Threshold)
          return FALSE;
        return TRUE;
      } /* End of 'IsBoxIntersect' function */

    }; /* End of 'ray' class */

} /* end of 'mth' namespace */

#endif /* __mth_ray_h_ */

/* END OF 'mth_ray.h' FILE */
