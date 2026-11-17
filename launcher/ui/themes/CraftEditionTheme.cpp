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

#include "CraftEditionTheme.h"

#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>

#include "ui/themes/ThemeManager.h"

namespace CraftEdition {

namespace {

const QString s_bundledThemeRoot = QStringLiteral(":/craft_edition/themes/") + ThemeId;

bool sameContent(const QString& path, const QByteArray& data)
{
    QFile existing(path);
    if (!existing.exists() || existing.size() != data.size()) {
        return false;
    }
    return existing.open(QIODevice::ReadOnly) && existing.readAll() == data;
}

}  // namespace

void installBundledTheme(const QDir& themeFolder)
{
    const QDir source(s_bundledThemeRoot);
    if (!source.exists()) {
        themeWarningLog() << "Bundled Craft Edition theme is missing from resources";
        return;
    }

    const QDir target(themeFolder.absoluteFilePath(ThemeId));
    int written = 0;

    QDirIterator it(s_bundledThemeRoot, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString sourcePath = it.next();
        const QString relativePath = source.relativeFilePath(sourcePath);
        const QString targetPath = target.absoluteFilePath(relativePath);

        QFile in(sourcePath);
        if (!in.open(QIODevice::ReadOnly)) {
            themeWarningLog() << "Couldn't read bundled theme file" << sourcePath;
            continue;
        }
        const QByteArray data = in.readAll();
        if (sameContent(targetPath, data)) {
            continue;
        }

        if (!QDir().mkpath(QFileInfo(targetPath).absolutePath())) {
            themeWarningLog() << "Couldn't create folder for" << targetPath;
            continue;
        }
        // QSaveFile replaces the file atomically, so a crash never leaves a half-written theme.
        QSaveFile out(targetPath);
        if (!out.open(QIODevice::WriteOnly) || out.write(data) != data.size() || !out.commit()) {
            themeWarningLog() << "Couldn't write bundled theme file" << targetPath << out.errorString();
            continue;
        }
        ++written;
    }

    themeDebugLog() << "Craft Edition theme installed to" << target.absolutePath() << "(" << written << "files updated)";
}

}  // namespace CraftEdition
