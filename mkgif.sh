#!/bin/sh

ffmpeg -i %03d.png -vf palettegen -y pal.png
ffmpeg -f image2 -r 1000/$1 -i %03d.png -i pal.png -filter_complex paletteuse -y eyesquat.gif


