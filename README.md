# Gled attic

Parts of [Gled](https://github.com/gled-project/gled) that are kept but no
longer developed.

| path | what |
|---|---|
| `TADemo/` | libset that loads and renders the unit models of the game Total Annihilation: the `.3do` piece hierarchy and the game's textures, with `demos/ta_init.C` |
| `docs/gledarch.md` | *Architectural Elements of Gled*, a design document last edited around 2003, incomplete |
| `docs/gled-dev.md` | *Somewhat urgent Gled developments*, a list of planned work from 2008 and 2009 |
| `history/` | the ChangeLogs of the svn repository, and where the commit history is; see `history/README.md` |

## TADemo

TADemo builds like the libsets of gled-hep: put the attic on the libset
path when configuring Gled,

```sh
./configure ... --libsetpath ../libsets:../../gled-attic --libsets '<auto>'
```

The unit models and textures are game data and are not in the repository.
`ta_init.C` reads them from the directory given as its argument or in the
environment variable `TA_DATA`, which must hold `textures.dir`, `textures_8bitRGB/*.rgb` and
`t1hpi/objects3d/*.3do`. The converter that made the textures from the
game files is not in the repository either.

## License

The contents are free software under the GNU Lesser General Public License,
version 3 or later. See `LICENSE`, `COPYING` and `COPYING.LESSER`.
