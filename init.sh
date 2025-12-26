#!/bin/bash

# clears allolib and al_ext folders if they exist
rm -rf allolib
rm -rf al_ext

# get submodules
git submodule update --init --recursive

# grab bass model
curl -o nam_models/BassModel.nam https://raw.githubusercontent.com/joeljaffesd/WorldsBestBassTone/main/BassModel.nam

# convert bass model to .h format
python ./RTNeural/modules/rt-nam/nam_to_header.py nam_models/BassModel.nam nam_models/BassModel.h