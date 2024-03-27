#!/bin/bash

# pd -open /mnt/data/Internet_of_plants/software/server/pd_patches/receiveTCP.pd -nogui -r 44100 -alsa  -noadc  -audiooutdev 1 -outchannels 2 -listdev -stderr &

# python3 /mnt/data/Internet_of_plants/software/server/main.py



pd -open /home/auriane/Documents/IoP/Internet-of-Plants/software/server/pd_patches/receiveTCP.pd -nogui -pa -audiooutdev 12 -noadc &

python3 /home/auriane/Documents/IoP/Internet-of-Plants/software/server/main.py