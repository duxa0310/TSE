/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : logger.h
 * PURPOSE     : Tough Space Exploration project.
 *               Common utilities.
 *               Console handle module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7)
 * LAST UPDATE : 02.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __logger_h_
#define __logger_h_

/* Main program namespace */
namespace tse
{
  /* Logger type class representation */
  class logger
  {
  public:
    /* Default type constructor */
    logger( VOID )
    {
      AllocConsole();
      std::freopen("CONOUT$", "w", stdout);
      system("@chcp 1251 > nul");
    } /* End of 'logger' function */

    /* Type destructor function */
    ~logger( VOID )
    {
      std::fclose(stdout);
      FreeConsole();
    } /* End of '~logger' function */

    /* Log current time function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    static VOID CollectTimestamp( VOID )
    {
      std::time_t t_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::cout << std::put_time(std::localtime(&t_c), "[%H:%M:%S]");
    } /* End of 'CollectTimestamp' function */

    /* Get rainbow color by coefficient function.
     * ARGUMENTS:
     *   - interpolate coefficient:
     *       DBL T;
     * RETURNS:
     *   (DWORD) interpolated color (format 0x??RRGGBB).
     */
    static DWORD InterpolateRainbow( DBL T )
    {
      /* Clamp value to range [0;1] */
      if (T < 0)
        T = 0;
      else if (T > 1)
        T = 1;
      DBL R = 0, G = 0, B = 0;
      /* Interpolate */
      if (T <= 1.0 / 6)
      {
        /* Red (1, 0, 0) to yellow (1, 1, 0) */
        T *= 6;
        R = 1, G = T, B = 0;
      }
      else if (T <= 2.0 / 6)
      {
        /* Yellow (1, 1, 0) to green (0, 1, 0) */
        T -= 1.0 / 6;
        T *= 6;
        R = 1 - T; G = 1, B = 0;
      }
      else if (T <= 3.0 / 6)
      {
        /* Green (0, 1, 0) to cyan (0, 1, 1) */
        T -= 2.0 / 6;
        T *= 6;
        R = 0, G = 1, B = T;
      }
      else if (T <= 4.0 / 6)
      {
        /* Cyan (0, 1, 1) to blue (0, 0, 1) */
        T -= 3.0 / 6;
        T *= 6;
        R = 0, G = 1 - T, B = 1;
      }
      else if (T <= 5.0 / 6)
      {
        /* Blue (0, 0, 1) to purple (1, 0, 1) */
        T -= 4.0 / 6;
        T *= 6;
        R = T, G = 0, B = 1;
      }
      else
      {
        /* Purple (1, 0, 1) to red (1, 0, 0) */
        T -= 5.0 / 6;
        T *= 6;
        R = 1, G = 0, B = 1 - T;
      }
      return ((BYTE)(R * 255) << 16) | ((BYTE)(G * 255) << 8) | ((BYTE)(B * 255) << 0);
    } /* End of 'InterpolateRainbow' function */

    /* Log color into console function.
     * ARGUMENTS:
     *   - color to log:
     *       DWORD Color;
     * RETURNS: None.
     */
    static VOID LogColor( DWORD Color )
    {
      INT
        R = (Color >> 16) & 0xFF,
        G = (Color >> 8) & 0xFF,
        B = (Color >> 0) & 0xFF;
      std::cout << std::format("\x1b[38;2;{};{};{}m•\x1b[38;3;0;0;0m",
        static_cast<INT>(R),
        static_cast<INT>(G),
        static_cast<INT>(B));
    } /* End of 'LogColor' function */

    /* Log string into console function.
     * ARGUMENTS:
     *   - string to log:
     *       const std::string &Str;
     * RETURNS: None.
     */
    static VOID Log( const std::string &Str )
    {
      std::cout << " " << Str;
    } /* End of 'Log' function */

    /* Log string into console with new line function.
     * ARGUMENTS:
     *   - string to log:
     *       const std::string &Str;
     * RETURNS: None.
     */
    static VOID LogLn( const std::string &Str )
    {
      std::cout << Str << std::endl;
    } /* End of 'LogLn' function */

    /* Log loading function.
     * ARGUMENTS:
     *   - prefix & postfix strings:
     *       const std::string &Prefix, &Postfix;
     *   - width in pixels:
     *       INT Width;
     *   - load completeness (range [0;1]):
     *       DBL Completeness;
     *   - color offset:
     *       INT Offset;
     * RETURNS: None.
     */
    static VOID Load( const std::string &Prefix, const std::string &Postfix, INT Width, DBL Completeness, INT Offset = 0 )
    {
      std::cout << Prefix << "[";
      for (INT i = 0; i < Width * Completeness; i++)
        LogColor(InterpolateRainbow(((Offset + i) % (3 * Width)) / (3.0 * Width)));
      for (INT i = Width * Completeness; i < Width; i++)
        std::cout << "•";
      std::cout << "] " << std::format("({}%)", static_cast<INT>(Completeness * 100)) << Postfix << "\r";
    } /* End of 'Load' function */

    /* Log important string into console function.
     * ARGUMENTS:
     *   - string to log:
     *       const std::string &Str;
     * RETURNS: None.
     */
    static VOID Aim( const std::string &Str )
    {
      std::cout << "\x1b[38;2;255;127;63m\x1b[48;2;0;0;0m" <<
        "-----===+ " << Str << " +===-----" << "\x1b[38;0;0;0;0m\x1b[48;0;0;0;0m" << std::endl;
    } /* End of 'Aim' function */

    /* Info string into console function.
     * ARGUMENTS:
     *   - string to log:
     *       const std::string &Str;
     * RETURNS: None.
     */
    static VOID Info( const std::string &Str )
    {
      std::cout << "\x1b[38;2;255;255;255m\x1b[48;2;0;127;190m" <<
        "[INFO]" << "\x1b[38;0;0;0;0m\x1b[48;0;0;0;0m" << " ";
      CollectTimestamp();
      std::cout << " \x1b[38;2;127;255;255m" << Str << "\x1b[38;0;0;0;0m" << std::endl;
    } /* End of 'Info' function */

    /* System string into console function.
     * ARGUMENTS:
     *   - string to log:
     *       const std::string &Str;
     * RETURNS: None.
     */
    static VOID Sys( const std::string &Str )
    {
      std::cout << "\x1b[38;2;255;255;255m\x1b[48;2;0;127;0m" <<
        "[SYS]" << "\x1b[38;0;0;0;0m\x1b[48;0;0;0;0m" << " ";
      CollectTimestamp();
      std::cout << " \x1b[38;2;127;255;127m" << Str << "\x1b[38;0;0;0;0m" << std::endl;
    } /* End of 'Sys' function */

    /* Warn string into console function.
     * ARGUMENTS:
     *   - string to log:
     *       const std::string &Str;
     * RETURNS: None.
     */
    static VOID Warn( const std::string &Str )
    {
      std::cout << "\x1b[38;2;255;255;255m\x1b[48;2;190;127;0m" <<
        "[WARN]" << "\x1b[38;0;0;0;0m\x1b[48;0;0;0;0m" << " ";
      CollectTimestamp();
      std::cout << " \x1b[38;2;255;190;63m" << Str << "\x1b[38;0;0;0;0m" << std::endl;
    } /* End of 'Warn' function */

    /* Error string into console function.
     * ARGUMENTS:
     *   - string to log:
     *       const std::string &Str;
     * RETURNS: None.
     */
    static VOID Err( const std::string &Str )
    {
      std::cout << "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m" <<
        "[ERROR]" << "\x1b[38;0;0;0;0m\x1b[48;0;0;0;0m" << " ";
      CollectTimestamp();
      std::cout << " \x1b[38;2;255;127;127m" << Str << "\x1b[38;0;0;0;0m" << std::endl;
    } /* End of 'Err' function */

  } _{}; /* End of 'logger' class */

} /* end of 'tse' namespace */

#endif /* __logger_h_ */

/* END OF 'logger.h' FILE */
