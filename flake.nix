{
  description = "A trivial lisp interpreter";

  inputs = {
    nixpkgs.url = "github:Nixos/nixpkgs/nixos-26.05";
    flake-utils.url = "github:/numtide/flake-utils";
  };

  outputs = inputs @ { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
      inherit (pkgs.stdenv) mkDerivation;
    in {
      packages.default = mkDerivation {
        name = "Trivial Lisp";
	version = "1.0";
	src = ./.;
	makeFlags = [ "PREFIX=$(out)" ];
      };
      devShells.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
	  gnumake
	  gcc
          gdb
	];
      };
    });
}
