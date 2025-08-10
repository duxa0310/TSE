/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : unit_x6.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Animation unit samples module.
 *               'X6' G3DM model test unit.
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
      model Btr; // Car model

    public:
      /* Type constructor function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *NewAni;
       */
      unit_sample( anim *NewAni ) : Ani(NewAni)
      {
        Btr.Load("bin/models/X6.g3dm", matr::RotateX(-90));
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
      } /* End of 'Response' function */

      /* Unit render function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Render( VOID ) override
      {
        Ani->Draw(&Btr, matr::Translate(-(Btr.MinBB + Btr.MaxBB) / 2) * 
                        matr::RotateY(30.0 * Ani->Time));
      } /* End of 'Render' function */

    }; /* End of 'unit_sample' class */

    /* Call register */
    static tse::anim::unit_register<unit_sample> _("X6");

  } /* end of anonymous namespace */

} /* end of 'tse' namespace */

/* END OF 'unit_x6.cpp' FILE */
