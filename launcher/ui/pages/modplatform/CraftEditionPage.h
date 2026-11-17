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

#include <QButtonGroup>
#include <QUrl>
#include <QWidget>

#include "ui/pages/BasePage.h"

class NewInstanceDialog;
class QLabel;

/**
 * "Craft Edition" source in the Add instance dialog.
 *
 * Offers the three Craft Edition presets and installs the chosen one from the latest
 * GitHub release of the modpack repository, through the same import task that the
 * Import page uses for .mrpack links.
 */
class CraftEditionPage : public QWidget, public BasePage {
    Q_OBJECT

   public:
    enum class Preset { Low, Balanced, High };

    explicit CraftEditionPage(NewInstanceDialog* dialog, QWidget* parent = nullptr);

    QString displayName() const override { return tr("Craft Edition"); }
    QIcon icon() const override { return QIcon::fromTheme("minecraft"); }
    QString id() const override { return "craft_edition"; }
    void openedImpl() override;
    void retranslate() override;

    /// Release asset for a preset. Asset names are stable across releases, so
    /// "releases/latest/download/<name>" always points at the newest pack.
    static QUrl packUrl(Preset preset);
    static QString presetKey(Preset preset);

   private:
    QWidget* makeCard(Preset preset);
    void updateState();

    NewInstanceDialog* m_dialog = nullptr;
    QButtonGroup m_presets;
    QLabel* m_title = nullptr;
    QLabel* m_intro = nullptr;
    QLabel* m_source = nullptr;
};
