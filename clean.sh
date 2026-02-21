#!/bin/bash
# Cleans the repo of temporary files generated in development

find . -type f -name '*~' -delete
rm *.obj
rm *.exe
rm debug.txt
