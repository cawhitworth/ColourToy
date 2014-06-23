# ColourToy

Messing around with pixels. Experiments on theme of images with unique pixel
colours - no two pixels the same, programmatically generated prettiness.

Inspired largely by [@kapuramax's experiments](https://medium.com/@kapuramax/procedural-image-generation-in-go-7a57ff2e2e90)

(the VersionTwo algorithm is essentially the same as his, but uses cubes
rather than spheres when searching the colourspace, and always picks the
middle value of the candidate set of colours. Also, at present, it uses a
single queue of pixels - C++ lacks the nice concurrency primitives of Go)

## Building

There's a .sln / .vcxproj for building on Visual Studio for Windows, and
a SConstruct for compiling on OS X/Linux. The code should be broadly
portable, and with a couple of ifdefs scattered around for the Windows build.

## LodePNG licensing note

This project includes a modified version of LodePNG. The modifications are
not functional changes but are necessary for building on Windows: a stdafx.h
include, and changing fopen() to fopen_s(). There's also a back-mapping of
fopen_s() to fopen() for OS X/Linux.

LodePNG can be found [here](http://lodev.org/lodepng/) - it's great and
ridiculously easy to use.
