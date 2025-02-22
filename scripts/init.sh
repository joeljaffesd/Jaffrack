#!/bin/bash

cd ../..

# get allolib_playground
git clone https://github.com/allosphere-research-group/allolib_playground

cd allolib_playground 

mv ../Jaffrack .

git submodule update --recursive --init

cd Jaffrack

git submodule update --recursive --init

# get AlloLib's dependencies (Partially tested)
# "$(sed -n '61s/^[ \t]*//p' ../allolib/readme.md)"

# more dependencies (UNTESTED!!!!)
# sudo apt-get install mesa-utils libglu1-mesa-dev xorg openbox