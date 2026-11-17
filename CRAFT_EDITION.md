# Craft Edition Launcher

Craft Edition Launcher is a fork of [Prism Launcher](https://github.com/PrismLauncher/PrismLauncher).
**It is not Prism Launcher and is not endorsed by or affiliated with the Prism Launcher project.**
It is licensed under GPL-3.0, like Prism Launcher.

## What this fork changes

All fork changes are kept in new files where possible, so upstream merges rarely conflict.
Edits to upstream files are one-line hooks marked `Craft Edition fork`.

| Change | Where |
| --- | --- |
| Build defaults: API keys, updater repository, links, binary name | `craft-edition/CraftEdition.cmake` (included from `CMakeLists.txt`) |
| Name, app ID, data folder | `craft-edition/CraftEdition-program-info.cmake` (included from `program_info/CMakeLists.txt`) |
| Logo, desktop entry, metainfo, Windows and macOS icons | `program_info/io.github.dafalloh.CraftEditionLauncher.*`, `program_info/craftedition.*`, `program_info/CraftEditionLauncher.icon` |
| Craft Edition theme, icons and background (default for new installs) | `launcher/resources/craft_edition/`, `launcher/ui/themes/CraftEditionTheme.*`, hooks in `ThemeManager` and `Application.cpp` |
| "Craft Edition" page in Add instance (Low / Balanced / High) | `launcher/ui/pages/modplatform/CraftEditionPage.*`, hook in `NewInstanceDialog.cpp` |
| Weekly upstream sync | `.github/workflows/craft-edition-sync.yml` |
| Release artifact names | `.github/actions/package/*`, `.github/workflows/release.yml` |
| Winget publishing limited to upstream | `.github/workflows/publish.yml` |

The launcher uses its own data folder (`CraftEditionLauncher`), so it can be installed next to Prism Launcher.

## One-time setup (repository owner)

1. **Enable Actions**: repository *Actions* tab, enable workflows (GitHub disables them on new forks).
2. **Microsoft sign-in**: register an app in [Microsoft Entra ID](https://entra.microsoft.com) (App registrations > New registration;
   supported accounts: *Personal Microsoft accounts only*; platform *Mobile and desktop applications* with the redirect URIs
   `craftedition://oauth/microsoft` and `http://localhost`; under *Authentication* enable *Allow public client flows* for device-code sign-in). Then request Minecraft API access for the app ID through
   [Mojang's form](https://aka.ms/mce-reviewappid) — new app IDs are blocked from the Minecraft API until Mojang approves them.
   Save the Application (client) ID as the repository secret `CRAFT_EDITION_MSA_CLIENT_ID`.
3. **CurseForge** (optional): apply for a key at the [CurseForge console](https://console.curseforge.com) and save it as
   `CRAFT_EDITION_CURSEFORGE_API_KEY`. Without it, CurseForge browsing is disabled; Modrinth works.
4. **Upstream sync** (recommended): create a fine-grained token with *Contents*, *Pull requests* and *Workflows* write access to
   this repository and save it as `SYNC_TOKEN`. Without it, syncs that touch workflow files cannot be pushed and sync pull requests do not start CI.

## Getting Prism Launcher updates

Every Monday `Sync with Prism Launcher` merges `PrismLauncher/PrismLauncher@develop` into a `sync/prism-*` branch and opens a pull request.
CI builds it on Linux, Windows and macOS. Merge it with a merge commit (not squash). If the merge conflicts, the workflow opens an issue listing the files.
Run it any time from the Actions tab (*Run workflow*).

## Releasing

Push a tag to build and publish a GitHub release: `git tag 12.0.0 && git push origin 12.0.0`.
Use Prism's version from `CMakeLists.txt` (`Launcher_VERSION_*`). Installed launchers on Windows and Linux find the new release through
the built-in updater, which reads this repository's releases. macOS auto-update (Sparkle) is off until we have our own signing key and appcast.

## Craft Edition packs

The Craft Edition page downloads `CraftEdition-Java-<low|balanced|high>.mrpack` from the **latest release** of
[Dafalloh/Craft-Edition](https://github.com/Dafalloh/Craft-Edition). Each pack release must include those three files with exactly these names
(no version in the name). Change the repository with `-DCraftEdition_PACKS_REPO=...`.

## Building locally

Same as Prism Launcher ([build instructions](https://prismlauncher.org/wiki/development/build-instructions/)). Optional:
`CRAFT_EDITION_MSA_CLIENT_ID=<id> cmake --preset linux` to build with Microsoft sign-in.
