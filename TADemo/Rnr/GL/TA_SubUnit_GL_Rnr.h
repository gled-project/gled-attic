// Copyright (C) Matevz Tadel.
// This file is part of Gled.
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef TADemo_TA_SubUnit_GL_RNR_H
#define TADemo_TA_SubUnit_GL_RNR_H

#include <Glasses/TA_SubUnit.h>
#include <Rnr/GL/ZNode_GL_Rnr.h>

namespace gled {

class TA_SubUnit_GL_Rnr : public ZNode_GL_Rnr {
private:

protected:
  TA_SubUnit*	mTA_SubUnit;

public:
  TA_SubUnit_GL_Rnr(TA_SubUnit* idol) : ZNode_GL_Rnr(idol), mTA_SubUnit(idol) {}

  virtual void Draw(RnrDriver* rd);

}; // endclass TA_SubUnit_GL_Rnr

} // endnamespace gled

#endif
