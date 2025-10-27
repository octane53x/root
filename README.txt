REPO README

Primarily C++ and perhaps some Python files

/Window: Windows window driver (Visual Studio)
/ai:     Artificial intelligence
/chef:   Codechef submissions
/core:   Universal resources
/gl:     Graphics library
/imp:    Impact game
/misc:   Miscellaneous
/mtg:    Magic deck simulator
/test:   Tests


TODO

! image rework
! object virtual fn: custom movement
! don't draw clear color
! const params
! ellipse.draw
! curve, shape
! image.rotate, image.flip
! point/uvec rotate
! orbit around uvec

! Comments
! Scene: Title Screen
! Cube raytracing
! Mining
! Planet gen
! Textures


FILE STRUCTURE

  CORE
incl <- def
def <- util
util <- num, graph

  GL/OBJ
uvec <- point
color <- image
point, image <- line, polygon, ellipse
point, color <- model
image, model <- object
object <- cube

  GL
image <- os
image, os <- font
object, font <- scene
image, font <- ui
scene <- env

  IMP
util <- recipe
point <- data, unit
graph, data, unit, recipe <- feature
feature <- planet
planet <- update
update <- game
scene, ui <- title
font, env, game, title <- impact

  DRIVER
os, impact <- Window


ACKNOWLEDGEMENTS

Fonts from www.dafont.com
