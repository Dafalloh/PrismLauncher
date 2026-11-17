# Craft Edition Launcher: names and identifiers.
# Included from program_info/CMakeLists.txt right after Prism's own values, so it replaces them.
# Changing these separates our data folder, desktop entry and app ID from Prism Launcher,
# so both can be installed side by side.
set(Launcher_CommonName "CraftEditionLauncher")
set(Launcher_DisplayName "Craft Edition Launcher")
set(Launcher_AppID "io.github.dafalloh.CraftEditionLauncher")
set(Launcher_Domain "github.com/Dafalloh/PrismLauncher")
set(Launcher_Git "${CraftEdition_GITHUB_REPO}")
