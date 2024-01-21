#!/bin/bash

pd -open /mnt/data/Internet_of_plants/software/server/pd_patches/receiveTCP.pd -nogui -r 44100 -alsa  -noadc -audioaddoutdev "sof-hda-dsp (hardware)"  -audiooutdev 1 -outchannels 2 -listdev -stderr &

python3 /mnt/data/Internet_of_plants/software/server/main.py
