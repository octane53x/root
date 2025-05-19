REPO README

Everything is in this repo besides large data files
Hopefully large files can be generated or have links provided
Included should be primarily C++ and perhaps some Python files


FILE STRUCTURE

  CORE
incl <- def
def <- util
util <- thing
thing <- obj, num
obj <- const

  GL
thing <- gl_obj
gl_obj <- gl_const, cube
gl_const <- image, ui
image <- shape, os
ui <- scene
scene <- env

  IMP
gl_obj <- data, unit, recipe
data, unit, recipe <- feature
feature <- planet
planet <- game
shape, scene <- title
font, env, game, title <- impact
impact <- Window

  DRIVER
os, imp <- Window


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


TODO

! Fix load_bmp
! Scene: Start Menu


ACKNOWLEDGEMENTS

Fonts from www.dafont.com
