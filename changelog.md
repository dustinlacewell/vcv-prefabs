# Prefabs Changelog

## 2.0.3 - 2/1/2023

- Multiple Prefab module are no longer a problem, they all share the single global widget.
- All Prefab modules share a single set of settings. So changing it here changes it everywhere.
- Once loaded the Prefabs Widget is now permanent and persists across patch loads.
- Prefabs and patches with missing modules can't be clicked and show tooltip with missing modules.
- V1 patches now supported.
- Some reliability fixes.

## 2.0.2 - 1/31/2023

Ongoing menu refactors and improvements...

- Unorganized prefabs (those not in a sub-directory) now show in the "untagged" menu.
- Toggling state favorites is less finicky.
- Ctrl must be held when opening the widget to see the full state library. The previous behavior of being able to
  toggle it while the menu was open was janky and uncomfortable to maintain.
- Module library and search results now sorted.
- Select how modules are sorted in state library using settings context-menu.
- Toggle which tags are shown in state library using settings context-menu.

## 2.0.1 - 1/30/2023

Initial pre-release