{
  description = "A trivial lisp interpreter";

  inputs = {
    nixpkgs.url = "github:Nixos/nixpkgs/nixos-25.11";
    flake-utils.url = "github:/numtide/flake-utils";
  };

  outputs = inputs @ { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
      inherit (pkgs.stdenv) mkDerivation;
    in {
      devShells.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
	  gnumake
	  gcc
          gdb
	];
      };
    });
}
