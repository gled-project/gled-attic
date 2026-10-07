// Copyright (C) Matevz Tadel.
// This file is part of Gled.
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "TA_Unit_GL_Rnr.h"
#include <GL/glew.h>

using namespace gled;

/**************************************************************************/

void TA_Unit_GL_Rnr::PreDraw(RnrDriver* rd)
{
  TA_SubUnit_GL_Rnr::PreDraw(rd);
  glPushMatrix(); glScalef(mTA_Unit->mS, mTA_Unit->mS, mTA_Unit->mS);
  glColor4fv(mTA_Unit->mColor());
}

void TA_Unit_GL_Rnr::PostDraw(RnrDriver* rd)
{
  glPopMatrix();
  TA_SubUnit_GL_Rnr::PostDraw(rd);
}
