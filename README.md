# MIRANDA SANDBOX (A new start)

This is a sandbox. This repository can be used to start a new game with Miranda or to test some of the *amazing* features of my favourite library.

# Compile it

The sandbox has been sucessfully compiled on Linux (Ubuntu) and MacOS (Catalina)

### Dependencies
The game depends on the following libraries:
 * libmiranda (https://github.com/nardinan/miranda)
 * libSDL2
 * libSDL2_ttf
 * libSDL2_mixer
 * libSDL2_image
 * libSDL2_gfx

### Compilation on Ubuntu (and possibly any other _Debian-based_ distribution)

Open a terminal and install the SDL2 libraries using **apt**:
```bash
$ sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx
```

Now that all the required SDL2 packages have been installed, it's time for Miranda:
```bash
$ git clone https://github.com/nardinan/miranda
$ cd miranda
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```
Great! Miranda is now installed in your /usr/local/lib directory. **Be sure to add the corresponding entry "/usr/local/lib" to LD_LIBRARY_PATH (DYLD_LIBRARY_PATH on OSX).** After that, from your terminal, perform the following steps to build Pomodoro:
```bash
$ git clone https://github.com/nardinan/miranda_sandbox
$ cd miranda_sandbox
$ mkdir build
$ cmake ..
$ make
```

### Compilation on Mac OS X

On MacOS X, the installation procedure has been tested with Brew (https://brew.sh/). To install the SDL2 libraries via the *brew* command, type the following:
```bash
$ brew install sdl2 sdl2_image sdl2_ttf sdl2_gfx
$ brew install sdl2_mixer --with-flac --with-fluid-synth --with-libmikmod --with-mpg123 --HEAD
```
Once the libraries have been successfully installed, refer to the paragraph about the installation of Miranda and Linkino on the section 'Compilation on Ubuntu' above.

### Compilation on Windows

I don't remember anymore, but I did it once and wasn't that complicated.
