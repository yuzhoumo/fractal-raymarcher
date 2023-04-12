# Fractal Raymarcher

[WIP] OpenGL real-time fractal renderer using raymarching.

## Building

External dependencies are included as git submodules in `lib`. Clone source
files and dependencies using:

```
git clone git@github.com:yuzhoumo/fractal-raymarcher.git --recursive
```

Build using CMake:

```
mkdir build && cd build
cmake ..
make
```

## Usage

```
./fmarch
```

## Dependencies

- [GLFW](https://github.com/glfw/glfw)
- [GLAD](https://github.com/Dav1dde/glad)
- [GLM](https://github.com/g-truc/glm)
- [CMake](http://www.cmake.org/)
