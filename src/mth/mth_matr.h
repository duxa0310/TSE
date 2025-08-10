/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_matr.h
 * PURPOSE     : Tough Space Exploration project.
 *               Mathematics library.
 *               Matrix handle module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 31.07.2025.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_matr_h_
#define __mth_matr_h_

#include "mth_vec3.h"

/* Math library namespace */
namespace mth
{
  /* Matrix bare data type */
  template<class type>
    struct matr_data
    {
    public:
      type M[4][4]; // Transformation matrix
 
      /* Class constructor */
      matr_data( VOID )
      {
      } /* End of 'matr_data' function */
 
      /* Class constructor.
        * ARGUMENTS:
        *   - source matrix:
        *       const type Mtr[4][4];
        */
      matr_data( const type Mtr[4][4] )
      {
        CopyMemory(M, Mtr, sizeof(type) * 16);
      } /* End of 'matr_data' function */

    }; /* End of 'matr_data' structure */
 
  /* Space transformation matrix handle class */
  template<class type>
    class matr : public matr_data<type>
    {
    private: 
      mutable type InverseM[4][4];   // Inverse matrix
      mutable BOOL IsInverseObtain;  // Inverse matrix presence flag
 
      /* Get 3x3 matrix determinant function.
       * ARGUMENTS:
       *   - elements of 3x3 matrix to get determinant of:
       *       type A11, A12, A13, A21, A22, A23, A31, A32, A33;
       * RETURNS:
       *   (type) 3x3 matrix determinant.
       */
      static type Determ3x3( type A11, type A12, type A13,
                             type A21, type A22, type A23,
                             type A31, type A32, type A33 )
      {
        return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
               A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
      } /* End of 'Determ3x3' function */

      /* Evaluate inverse matrix function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID EvaluateInverseMatrix( VOID ) const
      {
        if (IsInverseObtain)
          return;
        IsInverseObtain = TRUE;

        type det = Determ();
        if (det == 0)
        {
          InverseM[0][0] = 1, InverseM[0][1] = 0, InverseM[0][2] = 0, InverseM[0][3] = 0;
          InverseM[1][0] = 0, InverseM[1][1] = 1, InverseM[1][2] = 0, InverseM[1][3] = 0;
          InverseM[2][0] = 0, InverseM[2][1] = 0, InverseM[2][2] = 1, InverseM[2][3] = 0;
          InverseM[3][0] = 0, InverseM[3][1] = 0, InverseM[3][2] = 0, InverseM[3][3] = 1;
          return;
        }

        const type (*M)[4] = matr_data<type>::M;
        /* Build adjoint matrix */
        InverseM[0][0] =
          Determ3x3(M[1][1], M[1][2], M[1][3],
                    M[2][1], M[2][2], M[2][3],
                    M[3][1], M[3][2], M[3][3]) / det;
        InverseM[1][0] =
         -Determ3x3(M[1][0], M[1][2], M[1][3],
                    M[2][0], M[2][2], M[2][3],
                    M[3][0], M[3][2], M[3][3]) / det;
        InverseM[2][0] =
          Determ3x3(M[1][0], M[1][1], M[1][3],
                    M[2][0], M[2][1], M[2][3],
                    M[3][0], M[3][1], M[3][3]) / det;
        InverseM[3][0] =
         -Determ3x3(M[1][0], M[1][1], M[1][2],
                    M[2][0], M[2][1], M[2][2],
                    M[3][0], M[3][1], M[3][2]) / det;
        InverseM[0][1] =
         -Determ3x3(M[0][1], M[0][2], M[0][3],
                    M[2][1], M[2][2], M[2][3],
                    M[3][1], M[3][2], M[3][3]) / det;
        InverseM[1][1] =
          Determ3x3(M[0][0], M[0][2], M[0][3],
                    M[2][0], M[2][2], M[2][3],
                    M[3][0], M[3][2], M[3][3]) / det;
        InverseM[2][1] =
         -Determ3x3(M[0][0], M[0][1], M[0][3],
                    M[2][0], M[2][1], M[2][3],
                    M[3][0], M[3][1], M[3][3]) / det;
        InverseM[3][1] =
          Determ3x3(M[0][0], M[0][1], M[0][2],
                    M[2][0], M[2][1], M[2][2],
                    M[3][0], M[3][1], M[3][2]) / det;
        InverseM[0][2] =
          Determ3x3(M[0][1], M[0][2], M[0][3],
                    M[1][1], M[1][2], M[1][3],
                    M[3][1], M[3][2], M[3][3]) / det;
        InverseM[1][2] =
         -Determ3x3(M[0][0], M[0][2], M[0][3],
                    M[1][0], M[1][2], M[1][3],
                    M[3][0], M[3][2], M[3][3]) / det;
        InverseM[2][2] =
          Determ3x3(M[0][0], M[0][1], M[0][3],
                    M[1][0], M[1][1], M[1][3],
                    M[3][0], M[3][1], M[3][3]) / det;
        InverseM[3][2] =
         -Determ3x3(M[0][0], M[0][1], M[0][2],
                    M[1][0], M[1][1], M[1][2],
                    M[3][0], M[3][1], M[3][2]) / det;
        InverseM[0][3] =
         -Determ3x3(M[0][1], M[0][2], M[0][3],
                    M[1][1], M[1][2], M[1][3],
                    M[2][1], M[2][2], M[2][3]) / det;
        InverseM[1][3] =
          Determ3x3(M[0][0], M[0][2], M[0][3],
                    M[1][0], M[1][2], M[1][3],
                    M[2][0], M[2][2], M[2][3]) / det;
        InverseM[2][3] =
         -Determ3x3(M[0][0], M[0][1], M[0][3],
                    M[1][0], M[1][1], M[1][3],
                    M[2][0], M[2][1], M[2][3]) / det;
        InverseM[3][3] =
          Determ3x3(M[0][0], M[0][1], M[0][2],
                    M[1][0], M[1][1], M[1][2],
                    M[2][0], M[2][1], M[2][2]) / det;
      }; /* End of 'EvaluateInverseMatrix' function */

    public:
      /* Default class constructor */
      matr( VOID ) : IsInverseObtain(FALSE)
      {
      } /* End of 'matr' function */
 
      /* Matrix constructor.
       * ARGUMENTS:
       *   - matrix 4x4 values:
       *       type
       *         A00, A01, A02, A03,
       *         A10, A11, A12, A13,
       *         A20, A21, A22, A23,
       *         A30, A31, A32, A33;
       */
      matr( type A00, type A01, type A02, type A03,
            type A10, type A11, type A12, type A13,
            type A20, type A21, type A22, type A23,
            type A30, type A31, type A32, type A33 ) : IsInverseObtain(FALSE)
      {
        type (*M)[4] = matr_data<type>::M;
 
        M[0][0] = A00, M[0][1] = A01, M[0][2] = A02, M[0][3] = A03;
        M[1][0] = A10, M[1][1] = A11, M[1][2] = A12, M[1][3] = A13;
        M[2][0] = A20, M[2][1] = A21, M[2][2] = A22, M[2][3] = A23;
        M[3][0] = A30, M[3][1] = A31, M[3][2] = A32, M[3][3] = A33;
      } /* End of 'matr' function */
 
      /* Class constructor.
       * ARGUMENTS:
       *   - matrix elements:
       *       type R[4][4];
       */
      matr( type R[4][4] ) : IsInverseObtain(FALSE)
      {
        CopyMemory(matr_data<type>::M, R, sizeof(matr_data<type>::M));
      } /* End of 'matr' function */

      /* Obtain matrix first element pointer function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type *) pointer to A00 element.
       */
      operator type *( VOID )
      {
        return &matr_data<type>::M[0][0];
      } /* End of 'operator type *' function */
 
      /* Obtain matrix first element pointer function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const type *) pointer to A00 element.
       */
      operator const type *( VOID ) const
      {
        return &matr_data<type>::M[0][0];
      } /* End of 'operator const type *' function */
 
      /* Get product of two matrices function.
       * ARGUMENTS:
       *   - second matrix to multiply reference:
       *       const matr<type> & R;
       * RETURNS:
       *   (matr<type>) matrix product.
       */
      matr<type> operator*( const matr<type> &R ) const
      {
        const type (*M)[4] = matr_data<type>::M;

        return matr(M[0][0] * R.M[0][0] + M[0][1] * R.M[1][0] + M[0][2] * R.M[2][0] + M[0][3] * R.M[3][0],
                    M[0][0] * R.M[0][1] + M[0][1] * R.M[1][1] + M[0][2] * R.M[2][1] + M[0][3] * R.M[3][1],
                    M[0][0] * R.M[0][2] + M[0][1] * R.M[1][2] + M[0][2] * R.M[2][2] + M[0][3] * R.M[3][2],
                    M[0][0] * R.M[0][3] + M[0][1] * R.M[1][3] + M[0][2] * R.M[2][3] + M[0][3] * R.M[3][3],
                    M[1][0] * R.M[0][0] + M[1][1] * R.M[1][0] + M[1][2] * R.M[2][0] + M[1][3] * R.M[3][0],
                    M[1][0] * R.M[0][1] + M[1][1] * R.M[1][1] + M[1][2] * R.M[2][1] + M[1][3] * R.M[3][1],
                    M[1][0] * R.M[0][2] + M[1][1] * R.M[1][2] + M[1][2] * R.M[2][2] + M[1][3] * R.M[3][2],
                    M[1][0] * R.M[0][3] + M[1][1] * R.M[1][3] + M[1][2] * R.M[2][3] + M[1][3] * R.M[3][3],
                    M[2][0] * R.M[0][0] + M[2][1] * R.M[1][0] + M[2][2] * R.M[2][0] + M[2][3] * R.M[3][0],
                    M[2][0] * R.M[0][1] + M[2][1] * R.M[1][1] + M[2][2] * R.M[2][1] + M[2][3] * R.M[3][1],
                    M[2][0] * R.M[0][2] + M[2][1] * R.M[1][2] + M[2][2] * R.M[2][2] + M[2][3] * R.M[3][2],
                    M[2][0] * R.M[0][3] + M[2][1] * R.M[1][3] + M[2][2] * R.M[2][3] + M[2][3] * R.M[3][3],
                    M[3][0] * R.M[0][0] + M[3][1] * R.M[1][0] + M[3][2] * R.M[2][0] + M[3][3] * R.M[3][0],
                    M[3][0] * R.M[0][1] + M[3][1] * R.M[1][1] + M[3][2] * R.M[2][1] + M[3][3] * R.M[3][1],
                    M[3][0] * R.M[0][2] + M[3][1] * R.M[1][2] + M[3][2] * R.M[2][2] + M[3][3] * R.M[3][2],
                    M[3][0] * R.M[0][3] + M[3][1] * R.M[1][3] + M[3][2] * R.M[2][3] + M[3][3] * R.M[3][3]);
      } /* End of 'operator*' function */

      /* Obtain transposed matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) result matrix.
       */
      matr<type> Transpose( VOID ) const
      {
         const type (*M)[4] = matr_data<type>::M;

        return matr<type>(M[0][0], M[1][0], M[2][0], M[3][0],
                          M[0][1], M[1][1], M[2][1], M[3][1],
                          M[0][2], M[1][2], M[2][2], M[3][2],
                          M[0][3], M[1][3], M[2][3], M[3][3]);
      } /* End of 'Transpose' function */

      /* Obtain inverse matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) result matrix.
       */
      matr<type> Inverse( VOID ) const
      {
        EvaluateInverseMatrix();
        return matr<type>(InverseM);
      } /* End of 'Inverse' function */

      /* Get matrix determinant function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) matrix determinant.
       */
      type Determ( VOID ) const
      {
        const type (*M)[4] = matr_data<type>::M;

        return
          M[0][0] * Determ3x3(M[1][1], M[1][2], M[1][3],
                                  M[2][1], M[2][2], M[2][3],
                                  M[3][1], M[3][2], M[3][3]) +
          -M[0][1] * Determ3x3(M[1][0], M[1][2], M[1][3],
                                   M[2][0], M[2][2], M[2][3],
                                   M[3][0], M[3][2], M[3][3]) +
          M[0][2] * Determ3x3(M[1][0], M[1][1], M[1][3],
                                  M[2][0], M[2][1], M[2][3],
                                  M[3][0], M[3][1], M[3][3]) +
          -M[0][3] * Determ3x3(M[1][0], M[1][1], M[1][2],
                                   M[2][0], M[2][1], M[2][2],
                                   M[3][0], M[3][1], M[3][2]);
      } /* End of 'Determ' function */
 
      /* Get identity matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) identity matrix;
       */
      static matr Identity( VOID )
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      } /* End of 'Identity' function */

      /* Obtain translation matrix function.
       * ARGUMENTS:
       *   - coordinates delta values:
       *       type Dx, Dy, Dz;
       * RETURNS:
       *   (matr) translate transformatrion matrix.
       */
      static matr Translate( type Dx, type Dy, type Dz )
      {
        return matr(1,  0,  0,  0,
                    0,  1,  0,  0,
                    0,  0,  1,  0,
                    Dx, Dy, Dz, 1);
      } /* End of 'Translate' function */
 
      /* Obtain translation matrix function.
       * ARGUMENTS:
       *   - vector delta value:
       *       const vec3<type> &D;
       * RETURNS:
       *   (matr) translate transformatrion matrix.
       */
      static matr Translate( const vec3<type> &D )
      {
        return matr(1,  0,  0,  0,
                    0,  1,  0,  0,
                    0,  0,  1,  0,
                    D.X, D.Y, D.Z, 1);
      } /* End of 'Translate' function */

      /* Obtain scale matrix function.
       * ARGUMENTS:
       *   - coordinates scale values:
       *       type Sx, Sy, sz;
       * RETURNS:
       *   (matr) scale transformatrion matrix.
       */
      static matr Scale( type Sx, type Sy, type Sz )
      {
        return matr(Sx, 0, 0, 0,
                    0, Sy, 0, 0,
                    0, 0, Sz, 0,
                    0, 0, 0, 1);
      } /* End of 'Scale' function */

      /* Obtain scale matrix function.
       * ARGUMENTS:
       *   - vector scale value reference:
       *       const vec3<type> &S;
       * RETURNS:
       *   (matr) scale transformation matrix.
       */
      static matr Scale( const vec3<type> &S )
      {
        return matr(S.X, 0, 0, 0,
                    0, S.Y, 0, 0,
                    0, 0, S.Z, 0,
                    0, 0, 0, 1);
      } /* End of 'Scale' function */

      /* Obtain rotate matrix function.
       * ARGUMENTS:
       *   - rotate vector coordinates values:
       *       type Rx, Ry, Rz;
       *   - angle in degrees:
       *       DBL AngleInDegrees;
       * RETURNS:
       *   (matr) rotate transformation matrix.
       */
      static matr Rotate( type Rx, type Ry, type Rz, DBL AngleInDegrees )
      {
        type 
          s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees));

        return matr(Rx * Rx * (1 - c) + c, Rx * Ry * (1 - c) + Rz * s, Rx * Rz * (1 - c) - Ry * s, 0,
                    Rx * Ry * (1 - c) - Rz * s, Ry * Ry * (1 - c) + c, Ry * Rz * (1 - c) + Rx * s, 0,
                    Rx * Rz * (1 - c) + Ry * s, Ry * Rz * (1 - c) - Rx * s, Rz * Rz * (1 - c) + c, 0,
                    0, 0, 0, 1);
      } /* End of 'Rotate' function */

      /* Obtain rotation matrix function.
       * ARGUMENTS:
       *   - rotate vector value:
       *       const vec3<type> &R;
       *   - angle in degrees:
       *       DBL AngleInDegrees;
       * RETURNS:
       *   (matr) rotation transformatrion matrix.
       */
      static matr Rotate( const vec3<type> &R, DBL AngleInDegrees )
      {
        type
          s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees)),
          x = R.X, y = R.Y, z = R.Z;

        return matr(x * x * (1 - c) + c, x * y * (1 - c) + z * s, x * z * (1 - c) - y * s, 0,
                    x * y * (1 - c) - z * s, y * y * (1 - c) + c, y * z * (1 - c) + x * s, 0,
                    x * z * (1 - c) + y * s, y * z * (1 - c) - x * s, z * z * (1 - c) + c, 0,
                    0, 0, 0, 1);
      } /* End of 'Rotate' function */

      /* Obtain rotate 'X' axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       DBL AngleInDegrees;
       * RETURNS:
       *   (matr) rotate 'X' axis transformation matrix.
       */
      static matr RotateX( DBL AngleInDegrees )
      {
        type 
          s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees));

        return matr(1, 0, 0, 0,
                    0, c, s, 0,
                    0, -s, c, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateX' function */

      /* Obtain rotate 'Y' axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       DBL AngleInDegrees;
       * RETURNS:
       *   (matr) rotate 'Y' axis transformation matrix.
       */
      static matr RotateY( DBL AngleInDegrees )
      {
        type 
          s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees));

        return matr(c, 0, -s, 0,
                    0, 1, 0, 0,
                    s, 0, c, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateY' function */

      /* Obtain rotate 'Z' axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       DBL AngleInDegrees;
       * RETURNS:
       *   (matr) rotate 'Z' axis transformation matrix.
       */
      static matr RotateZ( DBL AngleInDegrees )
      {
        type 
          s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees));

        return matr(c, s, 0, 0,
                    -s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateZ' function */
 
      /* Matrix look-at viewer setup function.
       * ARGUMENTS:
       *   - viewer position, look-at point, approximate up direction references:
       *       const vec3<vtype> &Loc, &At, &Up1;
       * RETURNS:
       *   (matr<vtype>) result matrix.
       */
      template<typename vtype>
        static matr<vtype> View( const vec3<vtype> &Loc, const vec3<vtype> &At, const vec3<vtype> &Up1 )
        {
          vec3<type>
            Dir = (At - Loc).Normalize(),
            Right = (Dir % Up1).Normalize(),
            Up = (Right % Dir).Normalize();

          return matr(Right.X, Up.X, -Dir.X, 0,
                      Right.Y, Up.Y, -Dir.Y, 0,
                      Right.Z, Up.Z, -Dir.Z, 0,
                      -Loc & Right, -Loc & Up, Loc & Dir, 1);
        } /* End of 'View' function */

      /* Matrix orthographic projection function.
       * ARGUMENTS:
       *   - view parameters:
       *       type Left, Right, Bottom, Top, Near, Far;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr Ortho( type Left, type Right, type Bottom, type Top, type Near, type Far )
      {
        return matr(2 / (Right - Left), 0, 0, 0,
                    0, 2 / (Top - Bottom), 0, 0,
                    0, 0, -2 / (Far - Near), 0,
                    -(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1);
      } /* End of 'Ortho' function */

      /* Matrix frustum projection function.
       * ARGUMENTS:
       *   - view parameters:
       *       type Left, Right, Bottom, Top, Near, Far;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr Frustum( type Left, type Right, type Bottom, type Top, type Near, type Far )
      {
        return matr(2 * Near / (Right - Left), 0, 0, 0,
                    0, 2 * Near / (Top - Bottom), 0, 0,
                    (Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),  -(Far + Near) / (Far - Near), -1,
                    0, 0, -2 * Near * Far / (Far - Near), 0);
      } /* End of 'Frustum' function */

      /* Transformation point by matrix function.
       * ARGUMENTS:
       *   - vector to be tranform:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> TransformPoint( const vec3<vtype> &V ) const
        {
          const type (*M)[4] = matr_data<type>::M;
          vtype w = V.X * M[0][3] + V.Y * M[1][3] + V.Z * M[2][3] + M[3][3];
 
          return vec3<vtype>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0]) / w,
                             (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1]) / w,
                             (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]) / w);
        } /* End of 'TransformPoint' function */
 
      /* Transformation vector by matrix function.
       * ARGUMENTS:
       *   - vector to be tranform:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> TransformVector( const vec3<vtype> &V ) const
        {
          const type (*M)[4] = matr_data<type>::M;

          return vec3<vtype>(V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0],
                             V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1],
                             V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2]);
        } /* End of 'TransformVector' function */
 
      /* Transformation normal vector by matrix function.
       * ARGUMENTS:
       *   - normal vector to be tranform:
       *       const vec3<type> &N;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> TransformNormal( const vec3<vtype> &N ) const
        {
          EvaluateInverseMatrix();
          return vec3<vtype>(N.X * InverseM[0][0] + N.Y * InverseM[0][1] + N.Z * InverseM[0][2],
                             N.X * InverseM[1][0] + N.Y * InverseM[1][1] + N.Z * InverseM[1][2],
                             N.X * InverseM[2][0] + N.Y * InverseM[2][1] + N.Z * InverseM[2][2]);
        } /* End of 'TransformNormal' function */

      /* Transformation by vector-matrix product function.
       * ARGUMENTS:
       *   - vector to be tranform:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> Transform4x4( const vec3<vtype> &V ) const
        {
          const type (*M)[4] = matr_data<type>::M;

          return vec3<vtype>(V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0],
                             V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1],
                             V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]);
        } /* End of 'Transform4x4' function */

      /* Transformation point by inversed matrix function.
       * ARGUMENTS:
       *   - vector to be tranform:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> InvTransformPoint( const vec3<vtype> &V ) const
        {
          EvaluateInverseMatrix();
          vtype w = V.X * InverseM[0][3] + V.Y * InverseM[1][3] + V.Z * InverseM[2][3] + InverseM[3][3];
 
          return vec3<vtype>((V.X * InverseM[0][0] + V.Y * InverseM[1][0] + V.Z * InverseM[2][0] + InverseM[3][0]) / w,
                             (V.X * InverseM[0][1] + V.Y * InverseM[1][1] + V.Z * InverseM[2][1] + InverseM[3][1]) / w,
                             (V.X * InverseM[0][2] + V.Y * InverseM[1][2] + V.Z * InverseM[2][2] + InverseM[3][2]) / w);
        } /* End of 'InvTransformPoint' function */
 
      /* Transformation vector by inversed matrix function.
       * ARGUMENTS:
       *   - vector to be tranform:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> InvTransformVector( const vec3<vtype> &V ) const
        {
          EvaluateInverseMatrix();
          matr InvTrans = matr(InverseM).Transpose();
          return vec3<vtype>(V.X * InvTrans.M[0][0] + V.Y * InvTrans.M[1][0] + V.Z * InvTrans.M[2][0],
                             V.X * InvTrans.M[0][1] + V.Y * InvTrans.M[1][1] + V.Z * InvTrans.M[2][1],
                             V.X * InvTrans.M[0][2] + V.Y * InvTrans.M[1][2] + V.Z * InvTrans.M[2][2]);
        } /* End of 'InvTransformVector' function */
 
      /* Transformation normal vector by inversed matrix function.
       * ARGUMENTS:
       *   - normal vector to be tranform:
       *       const vec3<type> &N;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> InvTransformNormal( const vec3<vtype> &N ) const
        {
          const type (*M)[4] = matr_data<type>::M;

          return vec3<vtype>(N.X * M[0][0] + N.Y * M[0][1] + N.Z * M[0][2],
                             N.X * M[1][0] + N.Y * M[1][1] + N.Z * M[1][2],
                             N.X * M[2][0] + N.Y * M[2][1] + N.Z * M[2][2]);
        } /* End of 'InvTransformNormal' function */

    }; /* End of 'matr' class */

} /* end of 'mth' namespace */
 
#endif /* __mth_matr_h_ */

/* END OF 'mth_matr.h' FILE */
