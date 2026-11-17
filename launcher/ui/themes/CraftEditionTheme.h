// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Craft Edition Launcher - a fork of Prism Launcher
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QDir>
#include <QString>

/**
 * The Craft Edition look ships inside the binary (resources/craft_edition).
 *
 * The icon theme is served straight from resources. The application theme cannot be,
 * because CustomTheme reads "themes/<id>" from the data folder, so it is copied there
 * on start. The copy is refreshed when a launcher update changes it; users who want to
 * customise it should copy the folder under another name.
 */
namespace CraftEdition {

inline const QString ThemeId = QStringLiteral("CraftEdition");
inline const QString IconThemeId = QStringLiteral("craft_edition");
inline const QString CatPackId = QStringLiteral("craft_edition");
inline const QString CatPackManifest = QStringLiteral(":/craft_edition/catpacks/craft_edition/catpack.json");

/// Registers the Craft Edition resources (theme, icons, cat pack). The launcher logic is a
/// static library, so its .qrc is only linked in when something references it. Must run
/// before the theme manager reads any of them; safe to call more than once.
void initResources();

/// Copies the bundled application theme into @p themeFolder/CraftEdition.
/// Only files whose content differs are written. Failures are logged, never fatal:
/// the launcher then falls back to its normal theme selection.
void installBundledTheme(const QDir& themeFolder);

}  // namespace CraftEdition
