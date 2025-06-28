#!/bin/sh

# clears allolib and al_ext folders if they exist
rm -rf allolib
rm -rf al_ext

# get submodules
git submodule update --init --recursive