# mlc2
This is the code of the second generation controller for my MIDI based keyboard LED strip, which makes different LEDs
shine based on the notes I play. It is currently work in progress.

## History
I started the redesign because I found that the first version was not extensible enough anymore, both in terms of
hardware and software. The first version has some light effect "patches" (presets with different colors) hard-coded into
the firmware, together with the MIDI program numbers they respond to. This means that adding or changing a patch means
C coding, recompiling and flashing the chip.
The plan is a fully configurable version, so I can make and edit light effect patches from my smartphone or notebook,
even last-minute when everything is already setup on stage and the show is about to begin.

I'm an embedded software engineer, and like to make the most out of limited hardware (like the AVR microcontroller which
runs the previous version). However, my time to hobby around is also limited, and I prefer C++ for embedded now, because
of the object-oriented nature of the language. There's enough cheap hardware around which has enough memory and is
powerful enough to make use of these advantages.
