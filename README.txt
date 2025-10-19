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

! point/uvec rotate
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

  GL
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
