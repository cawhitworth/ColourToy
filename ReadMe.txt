Messing around with pixels.

Inspired largely by https://medium.com/@kapuramax/procedural-image-generation-in-go-7a57ff2e2e90

(the VersionTwo algorithm is essentially the same as his, but uses cubes
rather than spheres when searching the colourspace, and always picks the
middle value of the candidate set of colours. Also, at present, it uses a
single queue of pixels - C++ lacks the nice concurrency primitives of Go)

Please note:

This application includes a modified version of LodePNG. The modifications are
not functional changes but are necessary for building on Windows: a pch
include, and changing fopen() to fopen_s().
