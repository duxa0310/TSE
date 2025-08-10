/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : images.h
 * PURPOSE     : Tough Space Exploration project.
 *               Common utilities.
 *               Images handle module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7)
 * LAST UPDATE : 31.07.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __images_h_
#define __images_h_

/* Main program namespace */
namespace tse
{
  /* Image representation class */
  class image
  {
  private:
    INT Width = 0, Height = 0; // Image size in pixels
    std::vector<BYTE> Pixels;  // Image pixel data

    // Windows Imaging Component factory
    inline static IWICImagingFactory2 *WicFactory = nullptr;

  public:
    std::vector<DWORD *> RowsD;     // Rows access pointer by DWORD
    std::vector<BYTE (*)[4]> RowsB; // Rows access pointer by BYTE quads
 
    const INT &W = Width, &H = Height; // Image size references
 
    /* Default class construtor function */
    image( VOID )
    {
    } /* End of 'image' function */
 
    /* Class construtor function.
     * ARGUMENTS:
     *   - image file name:
     *       const std::string &FileName;
     */
    image( const std::string &FileName )
    {
      BOOL is_ok = FALSE;
      if (WicFactory == nullptr)
      {
        CoInitialize(nullptr);
        CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_ALL,
          IID_PPV_ARGS(&WicFactory));
      }
      /* Load through Window Image Component */
      if (WicFactory != nullptr)
      {
        std::wstring FileNameW {FileName.begin(), FileName.end()};
        IWICBitmapDecoder *Decoder {};
 
        /* Create decoder (loader) */
        WicFactory->CreateDecoderFromFilename(FileNameW.c_str(), nullptr,
          GENERIC_READ, WICDecodeMetadataCacheOnDemand, &Decoder);
        if (Decoder != nullptr)
        {
          IWICBitmapFrameDecode *Frame {};
          Decoder->GetFrame(0, &Frame);
          UINT fw = 0, fh = 0;
          if (Frame != nullptr)
          {
            Frame->GetSize(&fw, &fh);
            if (fw != 0 && fh != 0)
            {
              /* Converting image format to standard - RGBA with 8-bits per pixel */
              IWICFormatConverter *Convert {};
              WicFactory->CreateFormatConverter(&Convert);
              if (Convert != nullptr)
              {
                /* Converter initialization */
                Convert->Initialize(Frame, GUID_WICPixelFormat32bppBGRA,
                  WICBitmapDitherTypeNone, nullptr, 0.0,
                  WICBitmapPaletteTypeCustom);
                /* Size checking */
                Convert->GetSize(&fw, &fh);
                Pixels.resize(fw * fh * 4);
                if (fw != 0 && fh != 0)
                {
                  /* Copying image pixels to container pixel array */
                  WICRect Rect {0, 0, static_cast<INT>(fw), static_cast<INT>(fh)};
                  Convert->CopyPixels(&Rect, fw * 4, fw * fh * 4,
                    (BYTE *)Pixels.data());
                  Width = fw;
                  Height = fh;
                  is_ok = TRUE;
                }
                Convert->Release();
              }
            }
            Frame->Release();
          }
          Decoder->Release();
        }
      }
      if (!is_ok)
      {
        if (HBITMAP hBm;
            (hBm = (HBITMAP)LoadImage(nullptr, FileName.c_str(),
                                      IMAGE_BITMAP, 0, 0,
                                      LR_LOADFROMFILE)) != nullptr)
        {
          /* Case of *.BMP file */
          HDC hDC = GetDC(nullptr);
          HDC hMemDC = CreateCompatibleDC(hDC);
          HDC hMemDC1 = CreateCompatibleDC(hDC);
          ReleaseDC(nullptr, hDC);
          SelectObject(hMemDC, hBm);
          BITMAP bm;
          GetObject(hBm, sizeof(bm), &bm);
          BITMAPINFOHEADER bmih {};
          bmih.biSize = sizeof(BITMAPINFOHEADER);
          bmih.biBitCount = 32;
          bmih.biPlanes = 1;
          bmih.biCompression = BI_RGB;
          bmih.biWidth = bm.bmWidth;
          bmih.biHeight = -bm.bmHeight;
          bmih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;
          VOID *Bits;
          HBITMAP hImage = CreateDIBSection(nullptr, (BITMAPINFO *)&bmih,
            DIB_RGB_COLORS, &Bits, nullptr, 0);
          SelectObject(hMemDC1, hImage); 
          /* Draw load image to DIB */
          BitBlt(hMemDC1, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
          DeleteDC(hMemDC);
          DeleteDC(hMemDC1);
          Pixels.resize(bmih.biSizeImage);
          CopyMemory(&Pixels[0], Bits, bmih.biSizeImage);
          DeleteObject(hImage);
          DeleteObject(hBm); 
          /* Make alpha channel */
          for (INT i = 3; i < Width * Height * 4; i += 4)
            Pixels[i] = 0xFF;
        }
        else
        {
          std::fstream f(FileName, std::fstream::in | std::fstream::binary);
          if (f.is_open())
          {
            /* Case of *.G24 / *.G32 */
            INT fw = 0, fh = 0;
            f.read((CHAR *)&fw, 2);
            f.read((CHAR *)&fh, 2);
            f.seekg(0, std::fstream::end);
            INT flen = f.tellg();
            if (flen == 4 + fw * fh * 3)
            {
              /* *.G24 */
              f.seekg(4, std::fstream::beg);
              Pixels.resize(fw * fh * 4);
              for (INT r = 0, p = 0; r < fh; r++)
                for (INT c = 0; c < fw; c++)
                {
                  BYTE rgb[3];
                  f.read((CHAR *)rgb, 3);
                  Pixels[p++] = rgb[0];
                  Pixels[p++] = rgb[1];
                  Pixels[p++] = rgb[2];
                  Pixels[p++] = 255;
                }
            }
            else if (flen == 4 + fw * fh * 4)
            {
              /* *.G32 */
              f.seekg(4, std::fstream::beg);
              Pixels.resize(fw * fh * 4);
              Width = fw;
              Height = fh;
              f.read((CHAR *)Pixels.data(), 4 * fw * fh);
            }
            f.close();
          }
        }
      }
      /* Setup row pointers */
      INT i;
      RowsD.resize(Height);
      i = 0;
      for (auto &r : RowsD)
        r = (DWORD *)&Pixels[i++ * Width * 4];
      RowsB.resize(Height);
      i = 0;
      for (auto &r : RowsB)
        r = (BYTE (*)[4])&Pixels[i++ * Width * 4];
    } /* End of 'image' function */
 
    /* Class copying construtor.
     * ARGUMENTS:
     *   - const reference to copy object:
     *       const image &Img;
     */
    image( const image &Img ) :
      Width(Img.Width), Height(Img.Height), 
      W(Width), H(Height), Pixels(Img.Pixels)
    {
      /* Setup row pointers */
      RowsD.resize(Height);
      INT i = 0;
      for (auto &r : RowsD)
        r = (DWORD *)&Pixels[i++ * Width * 4];
      RowsB.resize(Height);
      i = 0;
      for (auto &r : RowsB)
        r = (BYTE (*)[4])&Pixels[i++ * Width * 4];
    } /* End of 'image' function *
 
    /* Class move construtor.
     * ARGUMENTS:
     *   - move reference to copy object:
     *       image &&Img;
     */
    image( image &&Img ) :
      Width(Img.Width), Height(Img.Height), 
      W(Width), H(Height), Pixels(std::move(Img.Pixels))
    {
      /* Setup row pointers */
      INT i;
      RowsD.resize(Height);
      i = 0;
      for (auto &r : RowsD)
        r = (DWORD *)&Pixels[i++ * Width * 4];
      RowsB.resize(Height);
      i = 0;
      for (auto &r : RowsB)
        r = (BYTE (*)[4])&Pixels[i++ * Width * 4];
      Img.Width = Img.Height = 0;
    } /* End of 'image' function */

    /* Obtain float point RGB color function.
     * ARGUMENTS:
     *   - sample pixel coordinate (0..1):
     *       const vec2 &T;
     * RETURNS:
     *   (vec3) sample color.
     */
    vec3 Sample( const vec2 &T )
    {
      INT
        tx = fmod(T.X, 1.0) * Width,
        ty = fmod(T.Y, 1.0) * Height;
      if (tx >= Width)
        tx = Width - 1;
      if (ty >= Height)
        ty = Height - 1;
      if (tx < 0)
        tx = 0;
      if (ty < 0)
        ty = 0;
      return vec3(RowsB[ty][tx][2] / 255.0,
                  RowsB[ty][tx][1] / 255.0,
                  RowsB[ty][tx][0] / 255.0);
    } /* End of 'Sample' function */

  }; /* End of 'image' class */

} /* end of 'tse' namespace */

#endif /* __images_h_ */

/* END OF 'images.h' FILE */
