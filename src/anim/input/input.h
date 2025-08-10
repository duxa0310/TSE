  /*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : input.h
 * PURPOSE     : Tough Space Exploration project.
 *               Animation input handle module.
 *               Common definitions module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 02.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __input_h_
#define __input_h_

#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Joystick each axis position getter macro definition */
#define GET_JOYSTIC_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

/* Main program namespace */
namespace tse
{
  /* Keyboard handle class representation */
  class keyboard
  {
  public:
    BYTE Keys[256];      // Current key states
    BYTE KeysOld[256];   // Old key states
    BYTE KeysClick[256]; // Click key states

    /* Default type constructor function */
    keyboard( VOID )
    {
      Reset();
    } /* End of 'keyboard' function */

    /* Reset keyboard function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
      FillMemory(Keys, 256, 0);
      FillMemory(KeysOld, 256, 0);
      FillMemory(KeysClick, 256, 0);
    } /* End of 'Reset' function */

    /* Perform keyboard response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      GetKeyboardState(Keys);
      for (INT i = 0; i < 256; i++)
      {
        Keys[i] >>= 7;
        KeysClick[i] = Keys[i] && !KeysOld[i];
      }
      CopyMemory(KeysOld, Keys, 256);
    } /* End of 'Response' function */

  }; /* End of 'keyboard' class */

  /* Mouse handle class representation */
  class mouse
  {
  private:
    HWND &hWndRef;   // Window handle reference
    INT &Wheel;      // Mouse wheel state
  public:
    INT
      Mx, My, Mz,    // Mouse position parameters
      Mdx, Mdy, Mdz; // Mouse delta parameters

    /* Type constructor function.
     * ARGUMENTS:
     *   - window handle reference:
     *       HWND &hNewWndRef;
     *   - mouse wheel reference:
     *       INT &NewWheel;
     */
    mouse( HWND &hNewWndRef, INT &NewWheel ) : hWndRef(hNewWndRef), Wheel(NewWheel)
    {
      Reset();
    } /* End of 'mouse' function */

    /* Reset mouse function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
      Mx = My = Mz = Mdx = Mdy = Mdz = 0;
    } /* End of 'Reset' function */

    /* Perform mouse response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      POINT pt;
      GetCursorPos(&pt);
      ScreenToClient(hWndRef, &pt);
      Mdx = pt.x - Mx;
      Mdy = pt.y - My;
      Mx = pt.x;
      My = pt.y;
      Mdz = Wheel - Mz;
      Mz = Wheel;
    } /* End of 'Response' function */

  }; /* End of 'mouse' class */

  /* Joystick handle class representation */
  class joystick
  {
  public:
    BYTE
      JBut[32], JButOld[32], JButClick[32]; // Joystick button states
    INT JPov;                               // Joystick point-of-view control [-1,0..7]
    DBL JX, JY, JZ, JR;                     // Joystick axes

    /* Default type constructor function */
    joystick( VOID )
    {
      Reset();
    } /* End of 'joystick' function */

    /* Reset joystick function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
      FillMemory(JBut, 32, 0);
      FillMemory(JButOld, 32, 0);
      FillMemory(JButClick, 32, 0);
      JPov = 0;
      JX = JY = JZ = JR = 0;
    } /* End of 'Reset' function */

    /* Perform joystick response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      static BOOL is_active = joyGetNumDevs() > 0;
      if (!is_active)
        return;
      
      JOYCAPS jc;
      if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
      {
        JOYINFOEX ji;

        ji.dwSize = sizeof(JOYINFOEX);
        ji.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
        {
          for (INT i = 0; i < 32; i++)
          {
            JBut[i] = (ji.dwButtons >> i) & 1;
            JButClick[i] = JBut[i] && !JButOld[i];
            JButOld[i] = JBut[i];
          }
          JX = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
          JX = GET_JOYSTIC_AXIS(X);
          JY = GET_JOYSTIC_AXIS(Y);
          JZ = GET_JOYSTIC_AXIS(Z);
          JR = GET_JOYSTIC_AXIS(R);
          JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
        }
      }
    } /* End of 'Response' function */

  }; /* End of 'joystick' class */

  /* Input handle class representation */
  class input : public keyboard, public mouse, public joystick
  {
  public:
    /* Type constructor function.
     * ARGUMENTS:
     *   - window handle reference:
     *       HWND &hWndRef;
     *   - mouse wheel reference:
     *       INT &MouseWheel;
     */
    input( HWND &hWndRef, INT &MouseWheel ) : keyboard(), mouse(hWndRef, MouseWheel), joystick()
    {
      Reset();
    } /* End of 'input' function */

    /* Reset input function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
      keyboard::Reset();
      mouse::Reset();
      joystick::Reset();
    } /* End of 'Reset' function */

    /* Perform input response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      keyboard::Response();
      mouse::Response();
      joystick::Response();
    } /* End of 'Response' function */

  }; /* End of 'input' class */

} /* end of 'tse' namepspace */

#endif /* __input_h_ */

/* END OF 'input.h' FILE */
