{
  inputs,
  cell,
}: let
  inherit (inputs) nixpkgs std self;
in {
  default = cell.packages.vcv-prefabs;
  vcv-prefabs = nixpkgs.stdenv.mkDerivation {
    pname = "vcv-prefabs";
    version = "2.0.5";

    src = std.incl self [
      "CMakeLists.txt"
      "RackSDK.cmake"
      "plugin.json"
      "src"
      "efsw"
      "res"
      "README.md"
    ];

    hardeningDisable = ["format"];

    prePatch = let
      efsw = nixpkgs.fetchFromGitHub {
        owner = "SpartanJ";
        repo = "efsw";
        rev = "3fd18f71b50befb8b1b3e221eceefc55500a2e75";
        sha256 = "r2shLCb3puVjIy20qruu6bDZHNX/PE7Ztn1EdTGou2Y=";
      };
    in ''
      mkdir -p efsw
      cp -r ${efsw}/* efsw
    '';

    cmakeFlags = let
      # rack-sdk = nixpkgs.fetchzip {
      #   name = "Rack-SDK-2.git.b04e4117.zip";
      #   url = "https://vcvrack.com/downloads/Rack-SDK-2.git.b04e4117.zip";
      #   sha256 = "mXH0deiy7OPmLx+XVMZd+KEzbVj5Vnr2G1niWcTHo/8=";
      # };
      rack-sdk = nixpkgs.fetchzip {
        name = "Rack-SDK-2.2.3-lin-x64.zip";
        url = "https://vcvrack.com/downloads/Rack-SDK-2.2.3-lin-x64.zip";
        sha256 = "4p1q34R0DXPmjqF2lJX0NIPr5TR16MEJHWWM3/vAhPY=";
      };
    in [
      "-DRACK_SDK_DIR=${rack-sdk}"
    ];

    buildInputs = [
      nixpkgs.cmake
    ];
  };
}
