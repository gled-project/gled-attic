// Copyright (C) Matevz Tadel.
// This file is part of Gled.
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef TADemo_TA_Unit_GL_RNR_H
#define TADemo_TA_Unit_GL_RNR_H

#include <Glasses/TA_Unit.h>
#include <Rnr/GL/TA_SubUnit_GL_Rnr.h>

namespace gled {

class TA_Unit_GL_Rnr : public TA_SubUnit_GL_Rnr {
private:

protected:
  TA_Unit*	mTA_Unit;

public:
  TA_Unit_GL_Rnr(TA_Unit* idol) : TA_SubUnit_GL_Rnr(idol), mTA_Unit(idol) {}

  virtual void PreDraw(RnrDriver* rd);
  virtual void PostDraw(RnrDriver* rd);

}; // endclass TA_Unit_GL_Rnr

} // endnamespace gled

#endif
