/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : anim.h
 * PURPOSE     : Tough Space Exploration project.
 *               Animation system module.
 *               Common definitions module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 01.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __anim_h_
#define __anim_h_

#include "input/input.h"
#include "input/timer.h"
#include "rnd/render.h"

/* Main program namespace */
namespace tse
{
  /* Active animation unit class representation */
  class unit
  {
  public:
    /* Class destructor function */
    virtual ~unit( VOID )
    {
    } /* End of '~unit' function */

    /* Response unit function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Response( VOID )
    {
    } /* End of 'Response' function */

    /* Render unit function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Render( VOID )
    {
    } /* Enf of 'Render' function */

  }; /* End of 'unit' class */

  /* Animation representation class */
  class anim : public win, public input, public timer, public render
  {
    stock<unit *> Units; // Unit stock

    /* Type constructor function.
     * ARGUMENTS:
     *   - application instance handle:
     *       HINSTANCE  hInst;
     */
    anim( HINSTANCE hInst = GetModuleHandle(nullptr) );

  public:
    /* Get animation path function */
    static std::string & Path( VOID );

    template<typename unit_type>
      /* Unit register representation class */
      class unit_register
      {
      private:
        /* Create unit function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (unit *) unit pointer;
         */
        static unit * Create( VOID )
        {
          return new unit_type(anim::GetPtr());
        } /* End of 'Create' function */

      public:
        /* Type constructor function.
         * ARGUMENTS:
         *   - unit to register name reference:
         *       const std::string &UnitName;
         */
        unit_register( const std::string &UnitName )
        {
          anim::Get().UnitNames[UnitName] = Create;
        } /* End of 'unit_register' function */

      }; /* End of 'unit_register' class */

    std::map<std::string, unit * (*)( VOID )> UnitNames; // Map of unit registers

    /* Add unit by name function.
     * ARGUMENTS:
     *   - unit name:
     *       const std::string &UnitName;
     * RETURNS:
     *   (anim &) self reference.
     */
    anim & operator<<( const std::string &UnitName )
    {
      if (UnitNames.find(UnitName) != UnitNames.end())
        *this << UnitNames[UnitName](), tse::logger::Info("UNIT added: " + UnitName);;
      return *this;
    } /* End of 'operator<<' function */

    /* Add unit by pointer function.
     * ARGUMENTS:
     *   - unit reference:
     *       unit *Uni;
     * RETURNS:
     *   (anim &) self reference.
     */
    anim & operator<<( unit *Uni )
    {
      if (Uni != nullptr)
        Units << Uni;
      return *this;
    } /* End of 'operator<<' function */

    /* Type destructor function */
    virtual ~anim( VOID ) override;

    /* Get animation singleton function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (anim &) singleton reference.
     */
    static anim & Get( VOID )
    {
      static anim singleton;
      return singleton;
    } /* End of 'Get' function */

    /* Get animation singleton pointer function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (anim *) singleton pointer.
     */
    static anim * GetPtr( VOID )
    {
      return &Get();
    } /* End of 'GetPtr' function */

    /* Render frame function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID );

    /* Start window message loop function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Run( VOID ) override;

    /* WM_CREATE window message handle function.
     * ARGUMENTS:
     *   - structure with creation data:
     *       CREATESTRUCT *CS;
     * RETURNS:
     *   (BOOL) TRUE to continue creation window, FALSE to terminate.
     */
    virtual BOOL OnCreate( CREATESTRUCT *CS ) override;

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT W, H;
     * RETURNS: None.
     */
    virtual VOID OnSize( UINT State, INT W, INT H ) override;

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID OnDestroy( VOID ) override;

    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */
    virtual VOID OnTimer( INT Id ) override;

    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
     virtual BOOL OnEraseBkgnd( HDC hDC ) override;

    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - window device context:
     *       HDC hDC;
     *   - paint message structure pointer:
     *       PAINTSTRUCT *PS;
     * RETURNS: None.
     */
    virtual VOID OnPaint( HDC hDC, PAINTSTRUCT *Ps ) override;

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
    virtual VOID OnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys ) override;

    /* WM_*BUTTONUP window message handle function.
     * ARGUMENTS:
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    virtual VOID OnButtonUp( INT X, INT Y, UINT Keys ) override;

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
    virtual VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys ) override;

    /* WM_EXITSIZEMOVE window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID OnExitSizeMove( VOID );

    /* WM_ENTERSIZEMOVE window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID OnEnterSizeMove( VOID );

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
    virtual VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized );

  }; /* End of 'anim' class */

} /* end of 'tse' namespace */

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */
