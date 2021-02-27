{ pkgs ? import <nixpkgs> {} }:
let
  myStdenv = pkgs.gccStdenv;
  myMkShell = pkgs.mkShell.override {
    stdenv = myStdenv;
  };
  canCheck = myStdenv.hostPlatform == myStdenv.buildPlatform;
  cmakeTestFlag = if canCheck then "ON" else "OFF";
  catch2 = with pkgs; stdenv.mkDerivation rec {
    name = "catch-${version}";
    version = "2.4.1";
    src = fetchFromGitHub {
      owner = "catchorg";
      repo = "Catch2";
      rev = "v${version}";
      sha256 = "046sqsbiqhfyrvhi7pqacwl4np6p8z167c84wpalvwm6zfgpz9ya";
    };
  
    # Checking requires python
    nativeBuildInputs = [ cmake ] ++ (if canCheck then [python3] else []);
    cmakeFlags = ["-DCATCH_BUILD_TESTING=${cmakeTestFlag}"
                  "-DBUILD_TESTING=${cmakeTestFlag}" ];
    doCheck = canCheck;
  
    meta = with stdenv.lib; {
      description = " A modern, C++-native, header-only, test framework for unit-tests using C++11 and later";
      homepage = http://catch-lib.net;
      license = licenses.boost;
      maintainers = maintainers.jdehaas;
    };
  };
in
  myMkShell rec {
    buildInputs = with pkgs; [ catch2 clang-tools ninja cmake SDL2 SDL2_image gtest pkgconfig ];
  }
