{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
  pname = "fmarch";
  version = "0.0.1";

  buildInputs = (with pkgs; [
    cmake
    gcc
    makeWrapper
    freetype
    glibc.out
    libGL
    libGLU
    glm
    glew
    glfw
    mesa
  ]) ++ (with pkgs.xorg; [
    libX11
    libXrandr
    libXinerama
    libXcursor
    libXi
  ]);

  src = ./.;

  configurePhase = ''
    export LD_LIBRARY_PATH="/run/opengl-driver/lib:/run/opengl-driver-32/lib"
    mkdir -p $TMPDIR/build
    cd $TMPDIR/build
    cmake $src
  '';

  buildPhase = ''
    export LD_LIBRARY_PATH="/run/opengl-driver/lib:/run/opengl-driver-32/lib"
    cd $TMPDIR/build
    make
  '';

  installPhase = ''
    export LD_LIBRARY_PATH="/run/opengl-driver/lib:/run/opengl-driver-32/lib"
    mkdir -p $out/bin
    cp $TMPDIR/build/fmarch $out/bin/fmarch
    wrapProgram $out/bin/fmarch \
      --prefix LD_LIBRARY_PATH : "${pkgs.libGL}/lib:${pkgs.libGLU}/lib:${pkgs.xorg.libX11}/lib:${pkgs.xorg.libXrandr}/lib:${pkgs.xorg.libXxf86vm}/lib:${pkgs.xorg.libXi}/lib"
  '';

  meta = with pkgs.lib; {
    description = "OpenGL ray marching fractal renderer";
    license = licenses.mit;
    platforms = platforms.unix;
  };

  shellHook = ''
    export LD_LIBRARY_PATH="/run/opengl-driver/lib:/run/opengl-driver-32/lib"
  '';
}
