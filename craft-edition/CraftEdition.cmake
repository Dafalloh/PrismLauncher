# Craft Edition Launcher: build defaults for this fork of Prism Launcher.
#
# Included from the top-level CMakeLists.txt before Prism's own defaults. Every value is a
# CACHE entry, so Prism's `set(... CACHE ...)` lines that follow keep these values, and a
# `-D` on the command line still overrides them.
#
# Prism's policy for forks (README, "Forking/Redistributing/Custom builds policy"):
# make clear this is not Prism Launcher, and replace Prism's API keys with our own or "".

set(CraftEdition_GITHUB_REPO "https://github.com/Dafalloh/PrismLauncher" CACHE STRING "GitHub repository of the Craft Edition Launcher fork")
set(CraftEdition_PACKS_REPO "https://github.com/Dafalloh/Craft-Edition" CACHE STRING "GitHub repository that publishes the Craft Edition modpacks")

# API keys. Prism's keys were issued to Prism and must not be reused by derivative builds.
# Values come from the environment (CI sets them from repository secrets, see build.yml).
# Empty values disable the feature at build time; users can still enter their own
# Microsoft client ID and CurseForge key under Settings > APIs.
set(Launcher_MSA_CLIENT_ID "$ENV{CRAFT_EDITION_MSA_CLIENT_ID}" CACHE STRING "Microsoft Identity Platform client ID (register your own app)")
set(Launcher_CURSEFORGE_API_KEY "$ENV{CRAFT_EDITION_CURSEFORGE_API_KEY}" CACHE STRING "CurseForge API key issued to Craft Edition Launcher")
set(Launcher_IMGUR_CLIENT_ID "" CACHE STRING "Imgur client ID issued to Craft Edition Launcher")

# Updates come from this fork's GitHub releases, not Prism's.
set(Launcher_UPDATER_GITHUB_REPO "${CraftEdition_GITHUB_REPO}" CACHE STRING "Base github URL for the updater.")
# Sparkle (macOS) needs our own signing key and appcast; disabled until those exist.
set(MACOSX_SPARKLE_UPDATE_PUBLIC_KEY "" CACHE STRING "Public key for Sparkle update feed")
set(MACOSX_SPARKLE_UPDATE_FEED_URL "" CACHE STRING "URL for Sparkle update feed")

# Links shown in the launcher. The news panel reads the modpack releases (Atom feed).
set(Launcher_NEWS_RSS_URL "${CraftEdition_PACKS_REPO}/releases.atom" CACHE STRING "URL to fetch the launcher news feed from.")
set(Launcher_NEWS_OPEN_URL "${CraftEdition_PACKS_REPO}/releases" CACHE STRING "URL that gets opened when the user clicks 'More News'")
set(Launcher_BUG_TRACKER_URL "${CraftEdition_GITHUB_REPO}/issues" CACHE STRING "URL for the bug tracker.")
set(Launcher_DISCORD_URL "" CACHE STRING "URL for the Discord guild.")
set(Launcher_MATRIX_URL "" CACHE STRING "URL to the Matrix Space")
set(Launcher_SUBREDDIT_URL "" CACHE STRING "URL for the subreddit.")

set(Launcher_APP_BINARY_NAME "craftedition" CACHE STRING "Name of the Launcher binary")

# Read by launcher/ui/pages/modplatform/CraftEditionPage.cpp.
add_compile_definitions(CRAFT_EDITION_PACKS_REPO="${CraftEdition_PACKS_REPO}")
