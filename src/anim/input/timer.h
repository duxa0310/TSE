/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : timer.h
 * PURPOSE     : Tough Space Exploration project.
 *               Animation input handle module.
 *               Timer handle.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 31.07.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __timer_h_
#define __timer_h_

/* Main program namespace */
namespace tse
{
  /* Timer type class representation */
  class timer
  {
  private:
    UINT64
      StartTime,    // Start program time
      OldTime,      // Previous frame time
      OldTimeFPS,   // Old time FPS measurement
      PauseTime,    // Time during pause period
      TimePerSec,   // Timer resolution
      FrameCounter; // Frames counted from last measure

  public:
    BOOL IsPause;      // Pause flag
    DBL
      GlobalTime,      // Global time
      GlobalDeltaTime, // Global time difference
      Time,            // Unpaused time
      DeltaTime,       // Unpaused time difference
      FPS;             // Frames per second

    /* Default type constructor.
     * ARGUMENTS: None.
     */
    timer( VOID ) : StartTime(0), OldTime(0), OldTimeFPS(0),
      PauseTime(0), TimePerSec(0), FrameCounter(0),
      IsPause(FALSE), GlobalTime(0), GlobalDeltaTime(30),
      Time(0), DeltaTime(30), FPS(102.30)
    {
      LARGE_INTEGER t {};
      QueryPerformanceFrequency(&t);
      TimePerSec = t.QuadPart;
      QueryPerformanceCounter(&t);
      StartTime = OldTime = OldTimeFPS = t.QuadPart;
    } /* End of 'timer' function */

    /* Perform response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      LARGE_INTEGER t {};
      QueryPerformanceCounter(&t);

      /* Global time */
      GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
      GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;
      /* Unpaused time */
      if (IsPause)
        DeltaTime = 0, PauseTime += t.QuadPart - OldTime;
      else
      {
        DeltaTime = GlobalDeltaTime;
        Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
      }
      /* Frames per second */
      FrameCounter++;
      if (t.QuadPart - OldTimeFPS > TimePerSec)
      {
        FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
        OldTimeFPS = t.QuadPart;
        FrameCounter = 0;
      }
      OldTime = t.QuadPart;
    } /* End of 'Response' function */

  }; /* End of 'timer' class */

} /* end of 'tse' namespace */

#endif /* __timer_h_ */

/* END OF 'timer.h' FILE */
