/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : win.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               WinAPI handle implementation.
 *               Window class instancing.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 31.07.2025.
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
tse::win::win( HINSTANCE hInst ) : hInstance(hInst),
  hWnd(nullptr), W(0), H(0), IsFullScreen(FALSE),
  FullScreenSaveRect{}, MouseWheel(0), IsActive(FALSE)
{
} /* End of 'tse::win::win' function */

/* Type destructor function */
tse::win::~win( VOID )
{
} /* End of 'tse::win::~win' function */

/* Create window function.
 * ARGUMENTS:
 *   - window label string:
 *       const std::string &WindowLabel;
 * RETURNS:
 *   (win &) window reference.
 */
tse::win & tse::win::Create( const std::string &WindowLabel )
{
  WNDCLASS wc;
  HWND hWnd;

  if (!GetClassInfo(hInstance, WND_CLASS_NAME, &wc))
  {
    /* Window class creation */
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.lpszMenuName = nullptr;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WinFunc;
    wc.lpszClassName = WND_CLASS_NAME;
    wc.cbWndExtra = sizeof(win *);
    /* Window class system registration */
    if (!RegisterClass(&wc))
    {
      MessageBox(nullptr, "Error while registering window class", "ERROR", MB_OK);
      return *this;
    }
  }
  /* Window creation */
  hWnd = CreateWindow(WND_CLASS_NAME, WindowLabel.c_str(),
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, reinterpret_cast<VOID *>(this));
  ShowWindow(hWnd, SW_SHOWNORMAL);
  /* UpdateWindow(hWnd); */
  return *this;
} /* End of 'Create' function */

/* Flip full screen function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID tse::win::FlipFullScreen( VOID )
{
  static bool IsFullScreen = false; // Store current mode
  static RECT SaveRC;               // Save old window size
 
  if (!IsFullScreen)
  {
    HMONITOR hMon;
    MONITORINFOEX moninfo;
    RECT rc;
 
    /* Save old window size and position */
    GetWindowRect(hWnd, &SaveRC);
    /* Get closest monitor */
    hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = true;
  }
  else
  {
    IsFullScreen = false;
    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_NOTOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
  }
} /* End of 'tse::win::FlipFullScreen' function */

/* Window message handle function (CALLBACK version).
 *   - window handle:
 *       HWND hWnd;
 *   - message identifier (see WM_***):
 *       UINT Msg;
 *   - message first parameter ('word' param):
 *       WPARAM wParam;
 *   - message second parameter ('long' param):
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message return code (depended to Msg type).
 */
LRESULT CALLBACK tse::win::WinFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  win *Win;
  HDC hDC;
  PAINTSTRUCT ps;
 
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.y =
      GetSystemMetrics(SM_CYCAPTION) * 8 +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.x += 300;
    return 0;
  case WM_CREATE:
    /* Attach 'this' pointer to window class to window */
    SetWindowLongPtr(hWnd, 0, (UINT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
  default:
    Win = reinterpret_cast<win *>(GetWindowLongPtr(hWnd, 0));
    if (Win != nullptr)
      switch (Msg)
      {
      case WM_CREATE:
        Win->hWnd = hWnd;
        return Win->OnCreate((CREATESTRUCT *)lParam) ? 0 : -1;
      case WM_SIZE:
        Win->W = (INT)(SHORT)LOWORD(lParam);
        Win->H = (INT)(SHORT)HIWORD(lParam);
        Win->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
        return 0;
      case WM_ACTIVATE:
        Win->IsActive = LOWORD(wParam) != WA_INACTIVE;
        Win->OnActivate((UINT)LOWORD(wParam), (HWND)(lParam),
                        (BOOL)HIWORD(wParam));
        return 0;
      case WM_ERASEBKGND:
        return (LRESULT)Win->OnEraseBkgnd((HDC)wParam);
      case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        Win->OnPaint(hDC, &ps);
        EndPaint(hWnd, &ps);
        return 0;
      case WM_DRAWITEM:
        Win->OnDrawItem((INT)wParam, (DRAWITEMSTRUCT *)lParam);
        return 0;
      case WM_TIMER:
        Win->OnTimer((UINT)wParam);
        return 0;
      case WM_MOUSEMOVE:
        Win->OnMouseMove((INT)(SHORT)LOWORD(lParam),
                         (INT)(SHORT)HIWORD(lParam),
                         (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_MOUSEWHEEL:
        Win->OnMouseWheel((INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(wParam),
                          (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONDBLCLK:
        Win->OnButtonDown(TRUE, (INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(lParam),
                          (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
        Win->OnButtonDown(FALSE, (INT)(SHORT)LOWORD(lParam),
                           (INT)(SHORT)HIWORD(lParam),
                           (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        Win->OnButtonUp((INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), 
                         (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_KEYDOWN:
        Win->OnKey((UINT)(SHORT)LOWORD(wParam), TRUE, (INT)(SHORT)LOWORD(lParam), 0);
        return 0;
      case WM_KEYUP:
        Win->OnKey((UINT)(SHORT)LOWORD(wParam), FALSE, (INT)(SHORT)LOWORD(lParam), 0);
        return 0;
      case WM_DROPFILES:
        Win->OnDropFiles((HDROP)wParam);
        return 0;
      case WM_NOTIFY:
        return Win->OnNotify((INT)wParam, (NMHDR *)lParam);
      case WM_COMMAND:
        Win->OnCommand((INT)LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam));
        return 0;
      case WM_MENUSELECT:
        Win->OnMenuSelect((HMENU)lParam,
          (HIWORD(wParam) & MF_POPUP) ? 0L : (INT)LOWORD(wParam),
          (HIWORD(wParam) & MF_POPUP) ? GetSubMenu((HMENU)lParam, LOWORD(wParam)):0L,
          (UINT)(((SHORT)HIWORD(wParam) == -1) ? 0xFFFFFFFF : HIWORD(wParam)));
        return 0;
      case WM_INITMENUPOPUP:
        Win->OnInitMenuPopup((HMENU)wParam, (UINT)
          LOWORD(lParam), (BOOL)HIWORD(lParam));
        return 0;
      case WM_HSCROLL:
        Win->OnHScroll((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
        return 0;
      case WM_VSCROLL:
        Win->OnVScroll((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
        return 0;
      case WM_ENTERSIZEMOVE:
        Win->OnEnterSizeMove();
        return 0;
      case WM_EXITSIZEMOVE:
        Win->OnExitSizeMove();
        return 0;
      case WM_DESTROY:
        Win->OnDestroy();
      default:
        return Win->OnMessage(Msg, wParam, lParam);
      }
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /** End of 'tse::win::WinFunc' function */

/* Start window message loop function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::win::Run( VOID )
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
      Idle();
    }
  }
} /* End of 'tse::win::Run' function */

/* Handle message loop idleness function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::win::Idle( VOID )
{
} /* End of 'tse::win::Idle' function */

/* END OF 'win.cpp' FILE */
