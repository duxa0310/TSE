/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : anim.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Animation system module.
 *               Main implementation module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7)
 * LAST UPDATE : 01.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

/* Type constructor function.
 * ARGUMENTS:
 *   - application instance handle:
 *       HINSTANCE  hInst;
 */
tse::anim::anim( HINSTANCE hInst ) : win(hInst), input(win::hWnd, win::MouseWheel), render(win::hWnd)
{
} /* End of 'tse::anim::anim' function */

/* Type destructor function */
tse::anim::~anim( VOID )
{
  Units.Walk([]( unit *Uni )
    {
      delete Uni;
    });
} /* End of 'tse::anim::~anim' function */

/* Get animation path function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (std::string &) path string.
 */
std::string & tse::anim::Path( VOID )
{
  static std::string path = []()
    {
      std::filesystem::path P = std::filesystem::current_path();
      if (P.string().length() > 0 && P.string().back() != '\\' && P.string().back() != '/')
        P += "\\";
      return P.string();
    }();
  return path;
} /* End of 'tse::anim::Path' function */

/* Render frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::anim::Render( VOID )
{
  /* Response */
  if (IsActive)
    input::Response();
  timer::Response();
  Units.Walk([]( unit *Uni )
    {
      Uni->Response();
    });

  /* Render */
  render::FrameStart();
  Units.Walk([]( unit *Uni )
    {
      Uni->Render();
    });
  render::FrameEnd();
} /* End of 'tse::anim::Render' function */

/* Start window message loop function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::anim::Run( VOID )
{
  MSG msg;

  while (TRUE)
  {
    /* Check message at window message queue */
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      if (msg.message == WM_QUIT)
        break;
      else
      {
        /* Displatch message to window */
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    else
    {
      /* Idle... */
      Render();
      Idle();
    }
  }
} /* End of 'tse::anim::Run' function */

/* WM_CREATE window message handle function.
 * ARGUMENTS:
 *   - structure with creation data:
 *       CREATESTRUCT *CS;
 * RETURNS:
 *   (BOOL) TRUE to continue creation window, FALSE to terminate.
 */
BOOL tse::anim::OnCreate( CREATESTRUCT *CS )
{
  render::Init();
  SetTimer(hWnd, 47, 2, nullptr);
  return TRUE;
} /* End of 'tse::anim::OnCreate' function */

/* WM_SIZE window message handle function.
 * ARGUMENTS:
 *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
 *       UINT State;
 *   - new width and height of client area:
 *       INT W, H;
 * RETURNS: None.
 */
VOID tse::anim::OnSize( UINT State, INT W, INT H )
{
  render::Resize(W, H);
} /* End of 'tse::anim::OnSize' function */

/* WM_DESTROY window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::anim::OnDestroy( VOID )
{
  tse::logger::Aim("Finishing up...");
  render::Close();
  KillTimer(hWnd, 47);
  PostQuitMessage(30);
} /* End of 'tse::anim::OnDestroy' function */

/* WM_TIMER window message handle function.
 * ARGUMENTS:
 *   - specified the timer identifier.
 *       INT Id;
 * RETURNS: None.
 */
VOID tse::anim::OnTimer( INT Id )
{
  Render();
} /* End of 'tse::anim::OnTimer' function */

/* WM_ERASEBKGND window message handle function.
 * ARGUMENTS:
 *   - device context of client area:
 *       HDC hDC;
 * RETURNS:
 *   (BOOL) TRUE if background is erased, FALSE otherwise.
 */
BOOL tse::anim::OnEraseBkgnd( HDC hDC )
{
  return FALSE;
} /* End of 'tse::anim::OnEraseBkgnd' function */

/* WM_PAINT window message handle function.
 * ARGUMENTS:
 *   - window device context:
 *       HDC hDC;
 *   - paint message structure pointer:
 *       PAINTSTRUCT *PS;
 * RETURNS: None.
 */
VOID tse::anim::OnPaint( HDC hDC, PAINTSTRUCT *Ps )
{
} /* End of 'tse::anim::OnPaint' function */

/* WM_*BUTTONDOWN window message handle function.
 * ARGUMENTS:
 *   - double click flag:
 *       BOOL IsDoubleClick;
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tse::anim::OnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys )
{
} /* End of 'tse::anim::OnButtonDown' function */

/* WM_*BUTTONUP window message handle function.
 * ARGUMENTS:
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tse::anim::OnButtonUp( INT X, INT Y, UINT Keys )
{
} /* End of 'tse::anim::OnButtonUp' function */

/* WM_MOUSEWHEEL window message handle function.
 * ARGUMENTS:
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse wheel relative delta value:
 *       INT Z;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tse::anim::OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
{
  MouseWheel += Z;
} /* End of 'tse::anim::OnMouseWheel' function */

/* WM_EXITSIZEMOVE window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::anim::OnExitSizeMove( VOID )
{
} /* End of 'tse::anim::OnExitSizeMove' function */

/* WM_ENTERSIZEMOVE window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::anim::OnEnterSizeMove( VOID )
{
} /* End of 'tse::anim::OnEnterSizeMove' function */

/* WM_ACTIVATE window message handle function.
 * ARGUMENTS:
 *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
 *       UINT Reason;
 *   - handle of active window:
 *       HWND hWndActDeact;
 *   - minimized flag:
 *       BOOL IsMinimized;
 * RETURNS: None.
 */
VOID tse::anim::OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
{
  IsActive = Reason == WA_INACTIVE ? FALSE : TRUE;
} /* End of 'tse::anim::OnActivate' function */

/* END OF 'anim.cpp' FILE */
