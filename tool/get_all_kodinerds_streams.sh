#!/bin/bash

# clone the repository
git clone --depth 1 https://github.com/jnk22/kodinerds-iptv

# apply pull requests
cd kodinerds-iptv
for i in $(ls ../pull_requests); do
   patch -p1 < ../pull_requests/$i
done

scripts/yaml_to_m3u.py

cd ..

# rm unwanted
rm kodi_radio.m3u

# convert all kodi files
for i in $(ls kodinerds-iptv/iptv/kodi/*.m3u); do
    ../convm3u "$i" "$(basename "$i" ".m3u").config.json"
done

