// Simple scene with two Total Annihilation units.
//
// The unit models and textures are game data and are not in the repository.
// Point ta_dir (or the environment variable TA_DATA) at a directory holding
//   textures.dir, textures_8bitRGB/*.rgb and t1hpi/objects3d/*.3do
//
// vars: ZQueen* g_queen
// libs: Geom1 TADemo

#include <glass_defines.h>
#include <gl_defines.h>

#include "sun_demos.C"
#include "eye.C"

using namespace gled;

TA_Unit* ta_unit(Scene* s, TA_TextureContainer* tc, const TString& file)
{
  TA_Unit* u = new TA_Unit(gSystem->BaseName(file));
  u->SetTexCont(tc);
  u->SetFile(file);
  s->GetQueen()->CheckIn(u);
  s->Add(u);
  u->Load();
  return u;
}

void ta_init(const Text_t* ta_dir = 0)
{
  ASSERT_MACRO(sun_demos);
  Gled::theOne->AssertLibSet("Geom1");
  Gled::theOne->AssertLibSet("TADemo");

  TString dir = ta_dir ? ta_dir : gSystem->Getenv("TA_DATA");
  if (dir.IsNull())
  {
    printf("ta_init: pass the TA data directory or set TA_DATA.\n");
    return;
  }

  Scene* ta_scene = new Scene("TADemo Scene");
  g_queen->CheckIn(ta_scene);
  g_queen->Add(ta_scene);
  g_scene = ta_scene;

  TA_TextureContainer* ta_cont = new TA_TextureContainer("Textures");
  g_queen->CheckIn(ta_cont);
  ta_scene->Add(ta_cont);
  ta_cont->SetDescDir(dir);
  ta_cont->ProcessDescFile();

  Lamp* ta_lamp = new Lamp("Lamp");
  ta_lamp->SetDiffuse(1, 1, 1);
  ta_lamp->MoveLF(3, 5);
  g_queen->CheckIn(ta_lamp);
  ta_scene->Add(ta_lamp);
  ta_scene->GetGlobLamps()->Add(ta_lamp);

  Rect* base_plane = new Rect("BasePlane");
  base_plane->SetUnitSquare(20);
  g_queen->CheckIn(base_plane);
  ta_scene->Add(base_plane);

  TA_Unit* ta_u1 = ta_unit(ta_scene, ta_cont, dir + "/t1hpi/objects3d/corgol.3do");
  ta_u1->MoveLF(2, -2);

  TA_Unit* ta_u2 = ta_unit(ta_scene, ta_cont, dir + "/t1hpi/objects3d/cortship.3do");
  ta_u2->MoveLF(2, 2);

  // Spawn GUI
  eye();
  setup_pupil_up_reference();
}
