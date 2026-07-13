{
  description = "Axiom development environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { nixpkgs, ... }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      riscv = pkgs.pkgsCross.riscv64-embedded;
    in
    {
      devShells.x86_64-linux.default = pkgs.mkShell {
        packages = [
          riscv.stdenv.cc
          riscv.binutils
          pkgs.qemu
          pkgs.gnumake
        ];

        CROSS_COMPILE = "riscv64-none-elf-";

        hardeningDisable = [ "all" ];
      };
    };
}
