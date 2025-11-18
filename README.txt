REPO README

Primarily C++ and perhaps some Python files


DIRECTORIES

/Window: Windows window driver (Visual Studio)
/ai:     Artificial intelligence
/chef:   Codechef submissions
/core:   Universal resources
/gl:     Graphics library
/imp:    Impact game
/misc:   Miscellaneous
/mtg:    Magic deck simulator
/net:    Network operations
/test:   Tests


COMMENTS

Dynamically allocated:
  gl/button, gl/object
  imp/feature, imp/unit

These structures require formal deletion to prevent memory leaks because
they need to be stored as pointers to a base class. This both allows usage of
virtual functions and prevents needing a unique container for every derivation.

Note many structures inherit from these and are not listed here.


TODO

! imp console
! demo square rotate
! clear out data.hh

! unit.target
! unit.path
! unit.activity

! const params & funcs
! ellipse.draw
! curve, shape
! image.rotate, image.flip
! point/uvec rotate
! orbit around uvec
! button click manager?
! fix image.scale
! resize window
! delete objects properly
! call validates
! scene draw time

! Comments
! Scene: Title Screen
! Multithread
! Cube raytracing
! Mining
! Planet gen
! Textures


ACKNOWLEDGEMENTS

Fonts from www.dafont.com
