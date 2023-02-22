{
  inputs,
  cell,
}: let
  inherit (inputs) nixpkgs std;
in {
  editorconfig = std.lib.cfg.editorconfig {
    hook.mode = "copy";
    data = {
      root = true;

      "*" = {
        charset = "utf-8";
        end_of_line = "lf";
        indent_size = "unset";
        indent_style = "space";
        insert_final_newline = true;
        max_line_length = "unset";
        trim_trailing_whitespace = true;
      };

      "*.nix" = {
        indent_size = 2;
        max_line_length = "120";
      };

      "*.{c,cpp,h,hpp}" = {
        indent_size = 4;
        max_line_length = "120";
      };

      "*.{diff,patch}" = {
        end_of_line = "unset";
        insert_final_newline = "unset";
        trim_trailing_whitespace = "unset";
      };

      "{LICENSES/**,LICENSE,*.lock,package-lock.json}" = {
        charset = "unset";
        end_of_line = "unset";
        indent_size = "unset";
        indent_style = "unset";
        insert_final_newline = "unset";
        trim_trailing_whitespace = "unset";
      };
    };
  };

  treefmt = std.lib.cfg.treefmt {
    packages = [
      nixpkgs.alejandra
      nixpkgs.nodePackages.prettier
      nixpkgs.clang-tools
    ];
    data.formatter = {
      nix = {
        command = "alejandra";
        includes = ["*.nix"];
      };
      prettier = {
        command = "prettier";
        includes = ["*.md" "*.json"];
      };
      cc = {
        command = "clang-format";
        includes = ["*.c" "*.cpp" "*.h" "*.hpp"];
      };
    };
  };

  lefthook = std.lib.cfg.lefthook {
    data = {
      pre-commit.commands = {
        treefmt = {
          run = "${nixpkgs.treefmt}/bin/treefmt --fail-on-change {staged_files}";
          skip = ["merge" "rebase"];
        };
        editorconfig-checker = {
          run = "${nixpkgs.editorconfig-checker}/bin/editorconfig-checker {staged_files}";
          skip = ["merge" "rebase"];
        };
      };
    };
  };
}
