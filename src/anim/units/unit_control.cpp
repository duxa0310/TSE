/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : unit_control.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Animation unit samples module.
 *               Control handle unit.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7)
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

/* Main program namespace */
namespace tse
{
  /* Anonymous namespace for correct name mangling */
  namespace
  {
    /* Unit sample class representation */
    class unit_sample : public unit
    {
    private:
      anim *Ani; // Animation context pointer
      font *F;   // Font to draw

    public:
      /* Type constructor function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *NewAni;
       */
      unit_sample( anim *NewAni ) : Ani(NewAni)
      {
        Ani->Cam.SetLocAtUp({8, 8, 8}, {0, 0, 0}, {0, 1, 0});
        (F = Ani->FntCreate("Book"))->Load("bin/fonts/Book.g3df");
      } /* End of ''unit_sample' function */

      /* Type destructor function */
      ~unit_sample( VOID )
      {
      } /* End of ''~unit_sample' function */

      /* Unit response function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Response( VOID ) override
      {
        /* Common usage control */
        if (Ani->Keys[VK_SHIFT] && Ani->KeysClick['P'])
          Ani->IsPause = !Ani->IsPause;
        else if (Ani->KeysClick[VK_F11])
          Ani->FlipFullScreen();
        else if (Ani->KeysClick[VK_ESCAPE])
          DestroyWindow(Ani->hWnd);
        else if (Ani->Keys[VK_CONTROL] && Ani->KeysClick['F'])
          Ani->Cam.SetLocAtUp({8, 8, 8}, {0, 0, 0}, {0, 1, 0});
        /* Wireframe / solid render switch */
        else if (INT modes[2]; Ani->Keys[VK_SHIFT] && Ani->KeysClick['W'])
        {
          glGetIntegerv(GL_POLYGON_MODE, modes);
          if (modes[0] == GL_LINE)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        /* Rotate camera */
        else if (Ani->Keys[VK_LCONTROL])
        {
          /* Rotate handle */
          FLT
            Dist = !(Ani->Cam.At - Ani->Cam.Loc),
            cosT = (Ani->Cam.Loc.Y - Ani->Cam.At.Y) / Dist,
            sinT = sqrt(1 - cosT * cosT),
            plen = Dist * sinT,
            cosP = (Ani->Cam.Loc.Z - Ani->Cam.At.Z) / plen,
            sinP = (Ani->Cam.Loc.X - Ani->Cam.At.X) / plen,
            Azimuth = R2D(atan2(sinP, cosP)),
            Elevator = R2D(atan2(sinT, cosT));

          Azimuth += 
            (-1 * Ani->Keys[VK_LBUTTON] * Ani->Mdx +
             (47 << 1) * Ani->GlobalDeltaTime * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
          Elevator += 
            (-1 * Ani->Keys[VK_LBUTTON] * Ani->Mdy +
             (47 << 1) * Ani->GlobalDeltaTime * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));
          if (Elevator < 0.08)
            Elevator = 0.08;
          else if (Elevator > 178.90)
            Elevator = 178.90;
          Dist *= (1 + log(Dist * 18) * Ani->GlobalDeltaTime *
            (-0.08 * Ani->Mdz + (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR])));
          if (Dist < 0.1)
            Dist = 0.1;
          /* Handle camera position */
          if (Ani->Keys[VK_RBUTTON])
          {
            FLT Wp = Ani->Cam.Size;
            FLT Hp = Ani->Cam.Size;
            if (Ani->Cam.FrameW > Ani->Cam.FrameH)
              Wp *= (FLT)Ani->Cam.FrameW / Ani->Cam.FrameH;
            else
              Hp *= (FLT)Ani->Cam.FrameH / Ani->Cam.FrameW;
            FLT sx = -Ani->Mdx * Wp / Ani->Cam.FrameW * Dist / Ani->Cam.ProjDist / 4.7;
            FLT sy = Ani->Mdy * Hp / Ani->Cam.FrameH * Dist / Ani->Cam.ProjDist / 1.8;
            vec3 dv = Ani->Cam.Right * sx + Ani->Cam.Up * sy;
            Ani->Cam.At = Ani->Cam.At + dv;
            Ani->Cam.Loc = Ani->Cam.Loc + dv;
          }
          /* Setup result camera */
          Ani->Cam.SetLocAtUp((
              matr::RotateX(Elevator) * 
              matr::RotateY(Azimuth) * 
              matr::Translate(Ani->Cam.At)).TransformPoint(vec3(0, Dist, 0)),
            Ani->Cam.At, vec3(0, 1, 0));
        }
        else if (Ani->JZ != 0 || Ani->JX != 0 || Ani->JY != 0)
        {
          /* Joystick movement */
          if (Ani->JZ != 0)
            Ani->Cam.Loc +=
              Ani->Cam.Dir * -Ani->GlobalDeltaTime * 30 * 47 * Ani->JZ;
          if (Ani->JX != 0 || Ani->JY != 0)
            Ani->Cam.Loc =
              matr::Rotate(vec3(0, -Ani->JX, Ani->JY),
                Ani->GlobalDeltaTime * 18).TransformPoint(Ani->Cam.Loc);
          /* Camera reset to new position */
          Ani->Cam.SetLocAtUp(Ani->Cam.Loc, Ani->Cam.Loc + Ani->Cam.Dir, Ani->Cam.Up);
        }
      } /* End of 'Response' function */

      /* Unit render function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Render( VOID ) override
      {
        matr save_vp = Ani->Cam.VP;
        Ani->Cam.VP = matr::Ortho(0, Ani->W, -Ani->H, 0, -1, 1);
        F->Draw(std::format("CGSG SumCamp'2025 forever!\nFPS: {:3.6}", Ani->FPS), vec3(0, 3, 0), 64);
        Ani->Cam.VP = save_vp;
      } /* End of 'Render' function */

    }; /* End of 'unit_sample' class */

    /* Call register */
    static tse::anim::unit_register<unit_sample> _("Control");

  } /* end of anonymous namespace */

} /* end of 'tse' namespace */

/* END OF 'unit_control.cpp' FILE */
