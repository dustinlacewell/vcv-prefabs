{
  description = "Quick patch prefabs and modules for VCV Rack.";

  inputs = {
    std.url = "github:divnix/std";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = {
    std,
    self,
    ...
  } @ inputs:
    std.growOn {
      inherit inputs;
      # Limited by the current means of acquiring the Rack SDK
      # Care to do better? DO IT!
      systems = ["x86_64-linux"];

      cellsFrom = ./nix;
      cellBlocks = with std.blockTypes; [
        (installables "packages")
        (devshells "devshells")
        (nixago "configs")
      ];
    }
    {
      packages = std.harvest self ["plugin" "packages"];
      devShells = std.harvest self ["_automation" "devshells"];
    };
}
