/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_camera.h
 * PURPOSE     : Tough Space Exploration project.
 *               Mathematics library.
 *               3D camera handle module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 04.08.2025.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_camera_h_
#define __mth_camera_h_

#include "mth_def.h"

/* Space math namespace */
namespace mth
{
  /* 3D camera representation type */
  template<typename type>
    class camera
    {
    public:
      mth::vec3<type>
        Loc,            // Camera location point
        Dir, Up, Right, // Basis direction vectors
        At;             // Camera look-at point

      type
        ProjDist, // Distance from eye to projection
        FarClip,  // Far clipping border
        Size,     // Projection plane size
        Wp, Hp;   // Projection cut size

      INT FrameW, FrameH; // Frame buffer size in pixels

      mth::matr<type>
        View, // View matrix
        Proj, // Projection matrix
        VP;   // Their product

      /* Default type constructor function */
      camera( VOID ) :
        Loc(0, 0, 5), Dir(0, 0, -1),
        Up(0, 1, 0), Right(1, 0, 0), At(0, 0, 0),
        ProjDist(0.1), FarClip(1847), Size(0.1), FrameW(30), FrameH(30)
      {
        UpdateProj();
      } /* End of 'camera' function */

      /* Get ray from frame by screen coordinates function.
       * ARGUMENTS:
       *   - screen coordinates:
       *       type Xs, Ys;
       * RETURNS:
       *   (ray<type>) frame ray.
       */
      ray<type> FrameRay( type Xs, type Ys ) const
      {
        vec3<type> Q = 
          Dir * ProjDist +
          Right * ((Xs - FrameW / 2.0) * Wp / FrameW) +
          Up * ((-Ys + FrameH / 2.0) * Hp / FrameH);
        return ray(Loc + Q, Q);
      } /* End of 'FrameRay' function */

      /* Set projection box parameters function.
       * ARGUMENTS:
       *   - new size:
       *       type NewSize;
       *   - new projection distance:
       *       type NewProjDist;
       *   - new far clipping:
       *       type NewFarClip;
       * RETURNS:
       *   (camera &) camera reference.
       */
      camera & SetProj( type NewSize, type NewProjDist, type NewFarClip )
      {
        Size = NewSize;
        ProjDist = NewProjDist;
        FarClip = NewFarClip;
        UpdateProj();
        return *this;
      } /* End of 'SetProj' function */

      /* Set projection size parameters function.
       * ARGUMENTS:
       *   - new frame size:
       *       type NewFameW, NewFrameH;
       * RETURNS:
       *   (camera &) camera reference.
       */
      camera & Resize( type NewFrameW, type NewFrameH )
      {
        FrameW = NewFrameW;
        FrameH = NewFrameH;
        UpdateProj();
        return *this;
      } /* End of 'Resize' function */

      /* Set projection view parameters function.
       * ARGUMENTS:
       *   - new camera location point reference:
       *       cont vec3<type> &L;
       *   - new view point reference:
       *       cont vec3<type> &A;
       *   - new up direction reference:
       *       cont vec3<type> &U;
       * RETURNS:
       *   (camera &) camera reference.
       */
      camera & SetLocAtUp( const vec3<type> &L, const vec3<type> &A, const vec3<type> &U = vec3<type>(0, 1, 0) )
      {
        Loc = L;
        At = A;
        Up = U;
        Dir = (At - Loc).Normalizing();
        Right = (Dir % Up).Normalizing();
        Up = Right % Dir;
        UpdateProj();
        return *this;
      } /* End of 'SetLocAtUp' function */

    private:
      /* Update projection parameters function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UpdateProj( VOID )
      {
        type
          rx = Size / 2,
          ry = Size / 2;
        if (FrameW >= FrameH)
          rx *= static_cast<type>(FrameW) / FrameH;
        else
          ry *= static_cast<type>(FrameH) / FrameW;
        Wp = rx * 2;
        Hp = ry * 2;
        Proj = mth::matr<type>::Frustum(-rx, rx, -ry, ry, ProjDist, FarClip);
        View = mth::matr<type>::View(Loc, At, Up);
        VP = View * Proj;
      } /* End of 'UpdateProj' function */

    }; /* End of 'camera' class */

} /* end of 'mth' namespace */

#endif /* __mth_camera_h_ */

/* END OF 'mth_camera.h' FILE */
