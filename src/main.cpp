/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : main.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Main entry point module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 01.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  tse::logger::Aim("Starting up...");

  tse::anim &my_anim = tse::anim::Get();
  my_anim.Create("CGSG SumCamp'2025 AB7 Animation Window");
  my_anim << "Sky" << "Axis" << "X6" << "Control";
  my_anim.Run();

  return 30;
} /* End of 'WinMain' function */

/* END OF 'main.cpp' FILE */
