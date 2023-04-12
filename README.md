# Fractal Raymarcher

[WIP] OpenGL real-time fractal renderer using raymarching.

## Cloning

External dependencies are included as git submodules in `lib`. Clone source
files and dependencies using:

```
git clone git@github.com:yuzhoumo/fractal-raymarcher.git --recursive
```

Note: If using NixOS, there is no need for the `--recursive` flag since
dependencies are pulled from nix-store.

### Build using CMake:

```
mkdir build && cd build
cmake ..
make
```

### Building on NixOS

On NixOS, you can build by running `nix-build` or entering the build
environment using `nix-shell` and following the CMake build instructions.
When building using `nix-build`, the resulting binary can be run using
`./result/bin/fmarch`.

## Usage

```
./fmarch
```

## Dependencies

- [GLFW](https://github.com/glfw/glfw)
- [GLAD](https://github.com/Dav1dde/glad)
- [GLM](https://github.com/g-truc/glm)
- [CMake](http://www.cmake.org/)
