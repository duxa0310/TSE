/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : def.h
 * PURPOSE     : Tough Space Exploration project.
 *               Common definitions module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7)
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __def_h_
#define __def_h_

/* Common definitions include */
#ifndef WIN32
#define WIN32
#include <commondf.h>
#undef WIN32
#else
#include <commondf.h>
#endif /* WIN32 */

/* Math library includes */
#include <cmath>
#include "mth/mth.h"

/* Debug memory allocation support */
#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
static struct __Dummy
 {
  /* Structure constructor */
  __Dummy( VOID )
  {
    SetDbgMemHooks();
  } /* End of '__Dummy' constructor */
} __oops;
#endif /* _DEBUG */

#ifdef _DEBUG
#  ifdef _CRTDBG_MAP_ALLOC
#    define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#  endif /* _CRTDBG_MAP_ALLOC */
#endif /* _DEBUG */

/* Standart includes */
#include <wincodec.h>

#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <span>

#include <functional>
#include <exception>
#include <thread>
#include <mutex>
#include <chrono>

#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>

/* Main program namespace */
namespace tse
{
  using vec2 = mth::vec2<FLT>;
  using vec3 = mth::vec3<FLT>;
  using vec4 = mth::vec4<FLT>;
  using matr = mth::matr<FLT>;
  using ray = mth::ray<FLT>;
  using camera = mth::camera<FLT>;

  /* Storage type class representation */
  template<typename T>
    class stock : public std::vector<T>
    {
    public:
      /* Append new element function.
       * ARGUMENTS:
       *   - element reference:
       *       const T &X;
       * RETURNS:
       *   (stock &) stock reference.
       */
      stock & operator<<( const T &X )
      {
        this->push_back(X);
        return *this;
      } /* End of 'operator<<' function */

      /* Walk through each element function.
       * ARGUMENTS:
       *   - walker function:
       *       WalkType W;
       * RETURNS: None.
       */
      template<class WalkType>
        VOID Walk( WalkType W )
        {
          for (auto &X : *this)
            W(X);
        } /* End of 'Walk' function */

    }; /* End of 'stock' class */

  /* Directory watcher class representation */
  class dir_watcher
  {
    OVERLAPPED Ovr {};       // Overlapped structure for changing hook
    HANDLE hDir {};          // Traching directory handle
    BYTE Buffer[30 * 47] {}; // Read directory changes data buffer
    std::string Path;        // Store path
 
  public:
    /* Start directory watching function.
     * ARGUMENTS:
     *   - directory path:
     *       const std::string &DirPath;
     * RETURNS: None.
     */
    VOID StartWatch( const std::string &DirPath )
    {
      StopWatch();
      ZeroMemory(&Ovr, sizeof(OVERLAPPED));
      Path = DirPath;
      /* Open directory */
      hDir =
        CreateFile(DirPath.c_str(),
          FILE_LIST_DIRECTORY,
          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
          nullptr, OPEN_EXISTING,
          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
          nullptr);
      /* Start monitoring */
      Ovr.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
      ReadDirectoryChangesW(hDir,
        Buffer, sizeof(Buffer),
        TRUE,
        FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |
          FILE_NOTIFY_CHANGE_FILE_NAME,
        nullptr,
        &Ovr,
        nullptr);
    } /* End of 'StartWatch' function */
 
    /* Stop directory watching function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID StopWatch( VOID )
    {
      if (Ovr.hEvent != nullptr)
        CloseHandle(Ovr.hEvent);
      if (hDir != nullptr)
        CloseHandle(hDir);
      ZeroMemory(&Ovr, sizeof(OVERLAPPED));
      hDir = nullptr;
      Path = "";
    } /* End of 'StopWatch' function */
 
    /* Check if directory is changed function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) TRUE if directory contents is changed (and subfolders).
     */
    BOOL IsChanged( VOID )
    {
      if (Ovr.hEvent == nullptr || hDir == nullptr)
        return FALSE;
      INT id = WaitForSingleObject(Ovr.hEvent, 0);
      if (id == WAIT_OBJECT_0)
      {
        /* Restart monitoring */
        ResetEvent(Ovr.hEvent);
        ReadDirectoryChangesW(hDir,
          Buffer, sizeof(Buffer),
          TRUE,
          FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |
            FILE_NOTIFY_CHANGE_FILE_NAME,
          nullptr,
          &Ovr,
          nullptr);
        return TRUE;
      }
      return FALSE;
    } /* End of 'IsChanged' function */

  }; /* End of 'dir_watcher' class */

   /* Unstructured data file reader class */
  class rdr
  {
  private:
    BYTE *&Ptr; // Memory pointer reference
 
  public:
    /* Class constructor.
      * ARGUMENTS:
      *   - reference to memory pointer:
      *       BYTE *&PtrRef;
      */
    rdr( BYTE *&PtrRef ) : Ptr(PtrRef)
    {
    } /* End of 'rdr' function */
 
    /* Read data function.
      * ARGUMENT:
      *   - read data pointer:
      *       type *Data;
      *   - read data count:
      *       INT Count;
      * RETURNS: None.
      */
    template<typename type>
      VOID operator()( type *Data, const INT Count = 1 )
      {
        if (Count == 1)
          *Data = *(type *)Ptr, Ptr += sizeof(type);
        else
        {
          memcpy(Data, Ptr, sizeof(type) * Count);
          Ptr += sizeof(type) * Count;
        }
      } /* End of 'operator()' function */
 
    /* Read data function.
      * ARGUMENT:
      *   - read data (skip with store pointer) pointer:
      *       type **Data;
      *   - read data count:
      *       INT Count;
      * RETURNS: None.
      */
    template<typename type>
      VOID operator()( type **Data, const INT Count = 1 )
      {
        *Data = (type *)Ptr;
        Ptr += sizeof(type) * Count;
      } /* End of 'operator()' function */
  }; /* End of 'rdr' class */

} /* end of 'tse' namespace */

#endif /* __def_h_ */

/* END OF 'def.h' FILE */
