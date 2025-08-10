/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : prim.cpp
 * PURPOSE     : Tough Space Exploration project.
 *               Render resources module.
 *               Primitives implementation module.
 * PROGRAMMER  : CGSG-Jr'2024-25.
 *               Belykh Andrey (AB7).
 * LAST UPDATE : 03.08.2025.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tse.h"

/***
 * PRIMITIVE FUNCTIONS
 ***/

/* Class constructor */
tse::prim::prim( VOID )
{
} /* End of 'tse::prim::prim' function */

/* Class destructor */
tse::prim::~prim( VOID )
{
} /* End of 'tse::prim::~prim' function */

/* Primitive destroy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tse::prim::Free( VOID )
{
  VertexMap.clear();
  if (VA != 0)
  {
    glBindVertexArray(VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (VBuf != 0)
      glDeleteBuffers(1, &VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VA);
    VA = 0;
    VBuf = 0;
  }
  if (IBuf != 0)
    glDeleteBuffers(1, &IBuf), IBuf = 0;
  NumOfElements = 0;
  MinBB = MaxBB = {};
} /* End of 'tse::prim::Free' function */

/* Update primitive vertex array function.
 * ARGUMENTS: None.
 * RETURNS: Non.
 */
VOID tse::prim::UpdateVA( VOID ) const
{
  if (Mtl == nullptr || Mtl->Shd == nullptr)
    return;
  if (IsVAUpdated)
    return;
  IsVAUpdated = TRUE;
 
  /* Activate vertex array */
  glBindVertexArray(VA);
 
  /* Activate vertex buffer */
  glBindBuffer(GL_ARRAY_BUFFER, VBuf);
 
  /* Setup data order due to vertex map */
  for (auto &a : VertexMap)
    if (auto attr = Mtl->Shd->Attributes.find(a.first);
        attr != Mtl->Shd->Attributes.end())
    {
      glEnableVertexAttribArray(attr->second.Loc);
      glVertexAttribPointer(attr->second.Loc,          // Layout
        attr->second.Components,                       // Components count
        attr->second.IsFloat ? GL_FLOAT : GL_INT,      // Component type
        FALSE,                                         // Normalize flag
        VertexStride,                                  // Stride
        reinterpret_cast<VOID *>((UINT_PTR)a.second)); // Offset
    }
  /* Disable vertex array */
  glBindVertexArray(0);
} /* End of 'tse::prim::UpdateVA' function */
 
/* Primitive with empty vertex stream creation function.
 * ARGUMENTS:
 *   - primitive type:
 *       prim_type Type;
 *   - number of emmitted vertexes:
 *       INT NumOfV;
 * RETURNS:
 *   (prim &) self reference.
 */
tse::prim & tse::prim::Create( tse::prim_type Type, INT NumOfV )
{
  Free();
  Type = Type;
  Transform = matr::Identity();
  NumOfElements = NumOfV;
  VertexStride = 0;
  MinBB = MaxBB = vec3(0);
 
  /* Create OpenGL vertex array */
  glGenVertexArrays(1, &VA);
  return *this;
} /* End of 'tse::prim::Create' function */

/* Load primitive from .OBJ function.
 * ARGUMENTS:
 *   - file name (*.OBJ model) to be load:
 *       const std::string &FileName;
 *   - transform matrix:
 *       const matr &Transform;
 *   - fit size flag vector:
 *       const vec3 &FitSize;
 * RETURNS:
 *   (prim &) self reference.
 */
tse::prim & tse::prim::Load( const std::string &FileName,
                               const matr &Transform, const vec3 &FitSize )
{
  std::ifstream f(anim::Path() + FileName);
  std::string Src(std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>()));

  if (Src.length() == 0)
    return *this;

  /* Count vertices and indices */
  SIZE_T nv = 0, nf = 0, nn = 0, nt = 0;
  for (INT c = 0; c < Src.length(); c++)
  {
    if (c > 0 && Src[c - 1] != '\n')
      continue;
    if (Src[c] == 'v' && Src[c + 1] == ' ')
      nv++;
    else if (INT n = 0; Src[c++] == 'f' && Src[c++] == ' ')
    {
      while (Src[c] != '\n')
        n += !isspace(Src[c]) && isspace(Src[c - 1]), c++;
      nf += n - 2;
    }
  }

  std::vector<vertex_std4> V;
  std::vector<INT> Ind;
  V.resize(nv);
  Ind.resize(nf * 3);
  nv = 0, nf = 0;
  INT min_ind = -1;
  for (INT c = 0; c < Src.length(); c++)
  {
    if (c > 0 && Src[c - 1] != '\n')
      continue;
    if (Src[c] == 'v' && Src[c + 1] == ' ')
    {
      sscanf(&Src[c + 2], "%f %f %f", &V[nv].P.X, &V[nv].P.Y, &V[nv].P.Z);
      V[nv].P = Transform.TransformPoint(V[nv].P);
      nv++;
    }
    else if (Src[c] == 'v' && Src[c + 1] == 't' && Src[c + 2] == ' ')
    {
      sscanf(&Src[c + 3], "%f %f", &V[nt].T.X, &V[nt].T.Y);
      nt++;
    }
    else if (Src[c] == 'v' && Src[c + 1] == 'n' && Src[c + 2] == ' ')
    {
      sscanf(&Src[c + 3], "%f %f %f", &V[nn].N.X, &V[nn].N.Y, &V[nn].N.Z);
      V[nn].N = Transform.TransformNormal(V[nn].N);
      nn++;
    }
    else if (INT n = 0; Src[c++] == 'f' && Src[c++] == ' ')
    {
      INT i = 0, i0 = 0, i1 = 0;

      while (Src[c] != '\n')
      {
        if (!isspace(Src[c]) && isspace(Src[c - 1]))
        {
          sscanf(&Src[c], "%d", &i);
          if (i < 0)
            i = nv + i;
          else
            i--;
          if (n == 0)
            i0 = i;
          else if (n == 1)
            i1 = i;
          else
          {
            if (min_ind == -1)
              min_ind = i0;
            /* Add a new triangle */
            Ind[nf++] = i0;
            Ind[nf++] = i1;
            Ind[nf++] = i;
            if (i0 < min_ind)
              min_ind = i0;
            if (i1 < min_ind)
              min_ind = i1;
            if (i < min_ind)
              min_ind = i;
            i1 = i;
          }
          n++;
        }
        c++;
      }
    }
  }
  if (min_ind > 0)
    for (INT i = 0; i < Ind.size(); i++)
      Ind[i] -= min_ind;
  Create(anim::Get().MtlCreate(FileName), prim_type::TRIMESH, std::span(V), Ind);
  tse::logger::Info("PRIMITIVE loaded: " + FileName);
  return *this;
} /* End of 'tse::prim::Load' function */

/***
 * MODEL FUNCTIONS
 ***/

/* Default constructor function */
tse::model::model( VOID ) : Transform(matr::Identity())
{
} /* End of 'tse::model::model' function */

/* Destructor function */
tse::model::~model( VOID )
{
} /* End of 'tse::model::~model' function */

/* Add new primitive to set function.
 * ARGUMENTS:
 *   - primitive to be add:
 *       primt *Pr;
 * RETURNS:
 *   (model &) self reference.
 */
tse::model & tse::model::operator<<( prim *Pr )
{
  Prims << Pr;
  return *this;
} /* End of 'tse::model::operator<<' function */

/* Load model from *.G3DM file function.
 * ARGUMENTS:
 *   - file name (*.G3DM model) to be load:
 *       const std::string &FileName;
 *   - transform matrix:
 *       const matr &Transform;
 *   - fit size vector flag:
 *       const vec3 &FitSize;
 * RETURNS:
 *   (model &) self reference.
 */
tse::model & tse::model::Load( const std::string &FileName,
                                 const matr &Transform, const vec3 &FitSize )
{
  for (auto pr : Prims)
    tse::anim::Get().render::PrimFree(pr);
  Prims.clear();
  this->Transform = matr::Identity();
  MinBB = MaxBB = vec3(0);

  std::fstream f(FileName, std::fstream::in | std::fstream::binary);
  if (!f.is_open())
    return *this;

  f.seekg(0, std::fstream::end);
  SIZE_T flen = f.tellg();
  f.seekg(0, std::fstream::beg);

  std::vector<BYTE> mem;
  mem.resize(flen);
  f.read((CHAR *)mem.data(), flen);
  BYTE *ptr = mem.data();

  rdr rd(ptr);

  /* Load header */
  DWORD sign;
  rd(&sign);
  if (sign != *(DWORD *)"G3DM")
    return *this;

  /* Get number of primitives, materials and textures */
  INT NoofP = 0, NoofM = 0, NoofT = 0;
  rd(&NoofP);
  rd(&NoofM);
  rd(&NoofT);

  /* Obtain min-max info */
  BYTE *save_ptr = ptr;
  BOOL IsFirstVertex = TRUE;
  for (INT i = 0; i < NoofP; i++)
  {
    UINT nv = 0, ni = 0, mtl_no;
    tse::vertex_std4 *V;
    INT *Ind;

    rd(&nv);
    rd(&ni);
    rd(&mtl_no);
    rd(&V, nv);
    rd(&Ind, ni);

    /* Collect bound box */
    if (nv > 0)
    {
      if (IsFirstVertex)
      {
        IsFirstVertex = FALSE;
        MinBB = MaxBB = V[0].P;
      }
      for (INT j = 0; j < nv; j++)
      {
        MinBB = MinBB.Min(V[j].P);
        MaxBB = MaxBB.Max(V[j].P);
      }
    }
  }

  /* Prepare transform matrix */
  vec3 BB = MaxBB - MinBB, scv = vec3(1);
  matr M(matr::Identity());

  if (FitSize.X != 0)
  {
    scv = vec3(FitSize.X / BB.X);
    M = matr::Translate(-MinBB) * matr::Scale(scv);
  }
  else if (FitSize.Y != 0)
  {
    scv = vec3(FitSize.Y / BB.Y);
    M = matr::Translate(-MinBB) * matr::Scale(scv);
  }
  else if (FitSize.Z != 0)
  {
    scv = vec3(FitSize.Z / BB.Z);
    M = matr::Translate(-MinBB) * matr::Scale(scv);
  }

  matr Trans = M * Transform;
  MinBB = Trans.TransformPoint(MinBB);
  MaxBB = Trans.TransformPoint(MaxBB);
  ptr = save_ptr;

  /* Load primitives */
  Prims.resize(NoofP);
  std::vector<INT> prims_mtl;
  prims_mtl.resize(NoofP);
  for (INT i = 0; i < NoofP; i++)
  {
    UINT nv = 0, ni = 0;
    tse::vertex_std4 *V;
    INT *Ind;

    rd(&nv);
    rd(&ni);
    rd(&prims_mtl[i]);
    rd(&V, nv);
    rd(&Ind, ni);

    for (INT j = 0; j < nv; j++)
    {
      V[j].P = Trans.TransformPoint(V[j].P);
      V[j].N = Trans.TransformNormal(V[j].N);
    }
    Prims[i] = tse::anim::Get().PrimCreate(nullptr,
      prim_type::TRIMESH, std::span(V, nv), std::span(Ind, ni));
  }

  /* Load materials */

  /* Stored material structure */
  struct STORE_MATERIAL
  {
    CHAR Name[300]; // Material name
 
    /* Illumination coefficients */ 
    mth::vec3<FLT> Ka, Kd, Ks; // Ambient, diffuse, specular coefficients
    FLT Ph;                    // Phong power coefficient
    FLT Trans;                 // Transparency factor
    INT Tex[8];                // Texture references
 
    CHAR ShaderStr[300];       // Shader string
    INT Shader;                // Shader index (not used)
  }; /* End of 'STORE_MATERIAL' struct */

  std::vector<material *> mtls;
  STORE_MATERIAL *store_mtls;
  rd(&store_mtls, NoofM);
  mtls.resize(NoofM);
  if (NoofM > 0)
  {
    for (INT i = 0; i < NoofM; i++)
    {
      mtls[i] = anim::Get().MtlCreate(
        FileName + "::" + store_mtls[i].Name);
      mtls[i]->Ka = store_mtls[i].Ka;
      mtls[i]->Kd = store_mtls[i].Kd;
      mtls[i]->Ks = store_mtls[i].Ks;
      mtls[i]->Ph = store_mtls[i].Ph;
      mtls[i]->Trans = store_mtls[i].Trans;
    }
  }

  /* Load textures */
  std::vector<texture *> texs;
  texs.resize(NoofT);
  for (INT i = 0; i < NoofT; i++)
  {
    CHAR Name[300];
    INT W, H, C;

    rd(Name, 300);
    rd(&W);
    rd(&H);
    rd(&C);
    SIZE_T last_byte = (ptr - mem.data()) + W * H * C;
    if (last_byte <= mem.size())
      texs[i] = anim::Get().TexCreate(FileName + "::" + Name, W, H, C, ptr);
    else
      break;
    ptr += W * H * C;
  }

  /* Correct material texture references */
  for (INT i = 0; i < NoofM; i++)
    for (INT t = 0; t < 8; t++)
      if (store_mtls[i].Tex[t] != -1)
        mtls[i]->Tex[t] = texs[store_mtls[i].Tex[t]];

  /* Update material buffers */
  if (NoofM > 0)
    for (INT i = 0; i < NoofM; i++)
      mtls[i]->Update();

  /* Correct primitive material references */
  for (INT i = 0; i < NoofP; i++)
    if (prims_mtl[i] != -1)
      if (prims_mtl[i] >= 0 && prims_mtl[i] < mtls.size())
        Prims[i]->Mtl = mtls[prims_mtl[i]];
      else
        Prims[i]->Mtl = anim::Get().material_manager::Find("default");

  Prims.resize(NoofP);
  tse::logger::Info("MODEL created: " + FileName);
  return *this;
} /* End of 'tse::model::Load' function */

/***
 * PRIMITIVE MANAGER FUNCTIONS
 ***/

/* Default type constructor function */
tse::primitive_manager::primitive_manager( render &Rnd ) : resource_manager<prim>(Rnd)
{
} /* End of 'tse::primitive_manager::primitive_manager' function */

/* Create primitive with empty vertex stream function.
 * ARGUMENTS:
 *   - primitive type:
 *       prim_type Type;
 *   - number of emmitted vertexes:
 *       INT NumOfV;
 * RETURNS:
 *   (prim *) created primitive interface.
 */
tse::prim * tse::primitive_manager::PrimCreate( prim_type Type, INT NumOfV )
{
  return resource_manager::Add(prim().Create(Type, NumOfV));
} /* End of 'tse::primitive_manager::PrimCreate' function */

/* Create primitive function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       prim *Pr;
 * RETURNS: None.
 *   (prim *) created primitive interface.
 */
VOID tse::primitive_manager::PrimFree( prim *Pr )
{
  resource_manager::Delete(Pr);
} /* End of 'tse::primitive_manager::PrimFree' function */

/* END OF 'prim.cpp' FILE */
