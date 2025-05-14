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
image <- os
ui <- scene
scene <- env

  IMP
gl_obj <- data, unit, recipe
data, unit, recipe <- feature
feature <- planet
planet <- game
scene <- title
font, env, game, title <- impact
impact <- Window

  DRIVER
os, imp <- Window


TODO

! Inherit imp objs by thing
! Label text/font support
! Scene: Start Menu
! Recipes


ACKNOWLEDGEMENTS

Fonts from www.dafont.com
