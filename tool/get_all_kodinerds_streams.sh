#!/bin/bash

# clone the repository
git clone --depth 1 https://github.com/jnk22/kodinerds-iptv

# rm unwanted
rm kodi_radio.m3u

# convert all kodi files
for i in $(ls kodinerds-iptv/iptv/kodi/*.m3u); do
    ../convm3u "$i" "$(basename "$i" ".m3u").config.json"
done

