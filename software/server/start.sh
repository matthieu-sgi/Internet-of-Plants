#!/bin/bash

pd -open <path>/pd_patches/receiveTCP.pd -nogui -r 44100 -alsa  -noadc -audioaddoutdev "sof-hda-dsp (hardware)"  -audiooutdev 1 -outchannels 2 -listdev -stderr &

python3 <path>/main.py
