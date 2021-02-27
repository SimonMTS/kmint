{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell rec {
  buildInputs = with pkgs; [ cmake SDL2 SDL2_image gtest doxygen ninja pkg-config ];
}
