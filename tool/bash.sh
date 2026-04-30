#!/bin/bash

# Cleans the repo of temporary files generated in development
clean(){
  find . -type f -name '*~' -delete
  rm -f *.obj
  rm -f *.exe
  rm -f debug.txt
}

# Finds a string in all files
search(){
  grep -r --exclude-dir=.git "$1"
}
