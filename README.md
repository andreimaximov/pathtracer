# README

This is a [pathtracer](https://en.wikipedia.org/wiki/Path_tracing) I wrote as
my final project for [CSE 163 at UCSD](http://cseweb.ucsd.edu/~viscomp/classes/cse163/sp17/163.html).
Thank you to professor Ravi for allowing me to publish this publicly. A
detailed feature report with sample renders and binaries can be found
[here](https://github.com/andreimaximov/cse163-reports/tree/master/pa3).

## Dependencies

- [glm](https://github.com/g-truc/glm)
- [tinyobjloader](https://github.com/syoyo/tinyobjloader)
- [stb\_image\_write](https://github.com/nothings/stb/blob/master/stb_image_write.h)
- [json](https://github.com/nlohmann/json)
- [spdlog](https://github.com/gabime/spdlog)
- [args](https://github.com/Taywee/args)

All of the above libraries are included as headers in the `lib` folder so you
do not have to download them.

## Usage

The `pathtracer` executable has the following CLI interface. Docs for the
config file format can be found [here](include/config.hpp) and examples in the
[config](config) directory. Examples of CLI usage can be found in the
[scripts](scripts) directory.

```
./pathtracer {OPTIONS} [scene] [materials] [config] [output]

  pathtracer

OPTIONS:

    -h, --help                        display this help menu
    scene                             the scene file
    materials                         the materials directory
    config                            the config file
    output                            the output file
    "--" can be used to terminate flag options and force all following
    arguments to be treated as positional options
```

## Building

On OS X or Linux use the `cmake.sh` script to create a Makefile project in the
`build` directory. Execute `make` in the `build` directory to build the
`pathtracer` and `pathtracer_tests` executable. You will need **CMake 3.x+** to
execute the `cmake.sh` script. Follow [these directions for Ubuntu](http://askubuntu.com/questions/610291/how-to-install-cmake-3-2-on-ubuntu-14-04)
or use `sudo port install cmake` on macOS.

GCC 5.4+ is a compatible compiler although recent version of Clang and MSVC
should work as well.
