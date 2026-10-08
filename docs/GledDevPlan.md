# STATUS

From the gled.org wiki (Foswiki), converted to markdown in 2026. Last edited in March 2010. The mailing lists it mentions are gone.

# Gled release and development plan

There are two main documents to look through (other than info on the web
pages): [gled-dev](gled-dev.md)
and
[greedworld-prototype-tasks](https://github.com/gled-project/greed/blob/main/docs/greedworld-prototype-tasks.md).

Get in touch with us either via [gled](mailto:gled-bootstrap@gled.org) or
[greed](mailto:greed-proto@gled.org) mailing list if you would like to get
involved!

## Release 1.3.1

Planned for early 2010. Release highlights:

- OSX port - done.
- Direct steering of units in greed-world demo - done.
- Reasonable dynamics of units on terrain - done.
- Laser cannon shooting - done.

## Release 1.3.2

Planned for late 2010. Main targets:

- First networked version of greed-world demo.
- Things from the list below - but they are not required for the release.

## Possible / optional tasks for 1.3.2 related to greed-demo

### Simple tasks (suitable for new members)

- Change rendering of laser-trace from line to a pair of triangles. Add
  divergence parameter to laser-gun properties.
- Implement key-binding editor for `Spiritio` (knowing Fltk would help).
- Improve overlay look-and-feel for first-person driving of `Crawlers` in
  class `CrawlerSpiritio`. E.g. add turret steering overlay bars.
- Improve dynamics of `Crawlers` -- requires reasonable knowledge of physics
  and 3D geometry needed.
  1.  Use forces instead of fixed velocities.
  2.  Use gravity information, air-drag, friction and static friction.
  3.  Make friction depend on levitation-height of the Crawler (also add
      controls into `CrawlerSpiritio`).
  4.  Allow side-slides when driving across laterally inclined terrain.
  5.  Do more realistic collision handling (now momentum and energy are
      conserved).
- Design and implement basic first-person steering of aeroplanes and
  helicopters (suitable for pilots and people who understand flight
  dynamics).

### Complex tasks

- Extend dynamic variable handling to also support exponential/sigmoid
  approach/decay of values to desired/ideal value.
  1.  Use these in `TringuCamSpiritio` to replace semi-hard-coded key
      handling in class `TringuCam`. See key handling code in
      `CrawlerSpiritio`.
  2.  Extract common key-handling functions from these two examples and put
      them into the base-class `Spiritio`.
- Design infrastructure for weapon classification -- basically a class
  structure that can represent various weapons, their properties and
  projectiles. Consider how weapons can be attached to `Extendios` and how
  they relate to energy consumption.
- Implement shooting of projectiles (from easy to hard: laser ray (done),
  bullet, cannon grenade, rocket, bomb).
- Extend damage / health handling of `Extendios`. Add damage types and
  armour / shield.
- Improve rendering of explosions for units and buildings.
- Terrain coloration in `TringuRep` and related classes `TriMesh` and
  `TriMeshField`.
  1.  Add suport for terrain with single color.
  2.  Make sure re-coloration functions run in detached threads with minimal
      locking.
  3.  See if better place can be found for `TriMesh::ColorByXxyz()`
      functions (one option could be `TriMeshField`). Should they run in
      detached-threads?
  4.  Consider how shaders could be used for terrain rendering.
- Use FTGL for text rendering. FTGL is already used in ROOT so using it in
  Gled should not be extra hard.

Partially done: FTGL is used, but only texture fonts are properly supported
(the old font rendering was only using texture fonts). The next step would
be to add support for bitmap/pixmap fonts and then for the more exotic ones
(outline, polygon, extruded).

### Open tasks that require more thinking

- Generalise management of unit selection in class `TSPupilInfo`. What
  happens when a first-person `Spiritio` is activated (clear selection,
  allow furhter selections)? What happens with menus (hide them or allow
  further interaction)?
- Implement path-finding and path-steering for `Crawlers`. Implement simple
  "go-to" command. Think how to implement a task list / stack for
  `Extendios` in general.
- `Tringula` now emits `Rays` about new explosions etc. This should really
  be done by `TringuRep` → `Tringula` should notify its representations in
  some other (more direct) way. This is already related to the networked
  version.
- Overlay menus are constructed explicitly for the default `Pupil` /
  `TringuRep` -- thus they can not be reused for additional views. They also
  could not be reused for `Extendio` menus.
  1.  Think how to solve this -- it seems that menu cloning on: a) top-menu
      creation and on b) extendio menu assignment would be sufficient.
  2.  How should they be cloned? Via serialization, direct cloning method,
      ...?
  3.  Implement additional parsing of alpha/beta tags for menu entries (see
      `tringula.C`, they are already listed, but not used: `TringuCamAlpha`,
      `PupilInfoAlpha`). This requires some more menu handling context.
      Thus, the natural place for this is `TSPupilInfo`.

-- Main.MatevzTadel - 14 Dec 2009
