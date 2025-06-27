REPO README

Everything is in this repo besides large data files
Hopefully large files can be generated or have links provided
Included should be primarily C++ and perhaps some Python files


TODO

! Remove "new" everywhere
! Comments
! Fix load_bmp
! Scene: Title Screen
! Cube


SCHEDULE

MAY 2: 2D support
JUNE 1: Ray tracing with Cube
JUNE 2: 3D support
JULY 1: Planet generation
JULY 2: Procedural art for buildings
AUG: BREAK
SEPT: Features, Units, Recipes
OCT: Logistics
NOV: Mechanics
DEC: BREAK
JAN-MAR: IMP 1.5 Design
APRIL: BREAK


FILE STRUCTURE

  GL
incl <- def
def <- util
util <- num, graph
num, graph <- obj
obj <- image, shape, cube, ui, os, scene
image, os <- font
scene <- move, env

  IMP
obj <- data, unit, recipe
data, unit, recipe <- feature
feature <- planet
planet <- update
update <- game
shape, move, scene, ui <- title
font, env, game, title <- impact

  DRIVER
os, impact <- Window


ACKNOWLEDGEMENTS

Fonts from www.dafont.com
