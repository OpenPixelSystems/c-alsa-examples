# ALSA examples

## Intro

This repository contains some recent ALSA examples showing the basic functionalities of the ALSA Framework. They've been made in such a way that each example can be copy pasted into another project and *should* just work. However nothing is ever simple with ALSA.

## Contents

- Playback: A generic simple playback of a WAV file
- Capture: Just capture the Microphone
- Utils:
	- List-devices: Use ALSA to display the current PCM devices available
	- Update-volume: Connect to the Master mixer and sweep the volume between 0 and 50%

## Building

The project uses the Meson build system so you'll need the following dependancies:
- alsa-devel / alsa-dev / libasound-devel, w/e your distro's name is for ALSA's userspace development lib
- meson
- ninja
- gcc (ofcourse...)

then run the following commands:
```
$ meson build
$ ninja -C build
$ ./build/<example_name>
```

## Troubleshooting ALSA errors

> Good luck... It's never easy with ALSA!..

If ALSA complains about card not found, validate the name of the card being openend. At the moment this is set to `default`. If your setup doesn't have a default for some **magic** reason, use the `build/utils/alsa-list-devices` example to see which PCM devices are available and change the `snd_pcm_open` function accordingly.
