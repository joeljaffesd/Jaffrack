#!/bin/bash

cd ../..

# get allolib-kickstart
git clone https://github.com/joeljaffesd/allolib-kickstart

cd allolib-kickstart

mv ../Jaffrack .

git submodule update --recursive --init

cd Jaffrack

git submodule update --recursive --init

# get AlloLib's dependencies (Partially tested)
# "$(sed -n '61s/^[ \t]*//p' ../allolib/readme.md)"

# more dependencies (UNTESTED!!!!)
# sudo apt-get install mesa-utils libglu1-mesa-dev xorg openbox