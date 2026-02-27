#!/bin/bash
# Cleans the repo of temporary files generated in development

find . -type f -name '*~' -delete
rm -f *.obj
rm -f *.exe
rm -f debug.txt
