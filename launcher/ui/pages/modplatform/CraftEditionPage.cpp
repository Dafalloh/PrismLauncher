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

#include "CraftEditionPage.h"

#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

#include "InstanceImportTask.h"
#include "ui/dialogs/NewInstanceDialog.h"

// Set by craft-edition/CraftEdition.cmake.
#ifndef CRAFT_EDITION_PACKS_REPO
#define CRAFT_EDITION_PACKS_REPO "https://github.com/Dafalloh/Craft-Edition"
#endif

namespace {

struct PresetText {
    QString name;   // used in the instance name
    QString title;  // shown on the card
    QString summary;
    QString details;
};

PresetText presetText(CraftEditionPage::Preset preset)
{
    switch (preset) {
        case CraftEditionPage::Preset::Low:
            return { CraftEditionPage::tr("Low"), CraftEditionPage::tr("Low"),
                     CraftEditionPage::tr("For older laptops and integrated graphics."),
                     CraftEditionPage::tr("Performance mods only (Sodium, Lithium, C2ME, FerriteCore). No shaders or visual extras.") };
        case CraftEditionPage::Preset::Balanced:
            return { CraftEditionPage::tr("Balanced"), CraftEditionPage::tr("Balanced (recommended)"),
                     CraftEditionPage::tr("For most gaming PCs."),
                     CraftEditionPage::tr("Iris ready, Fresh Animations, Better Clouds, Falling Leaves and 3D skin layers.") };
        case CraftEditionPage::Preset::High:
            return { CraftEditionPage::tr("High"), CraftEditionPage::tr("High"), CraftEditionPage::tr("For strong dedicated GPUs."),
                     CraftEditionPage::tr("Complementary Reimagined shaders, Distant Horizons, Sound Physics and Wakes.") };
    }
    return {};
}

}  // namespace

CraftEditionPage::CraftEditionPage(NewInstanceDialog* dialog, QWidget* parent) : QWidget(parent), m_dialog(dialog)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    m_title = new QLabel(this);
    m_title->setObjectName("craftEditionTitle");
    m_intro = new QLabel(this);
    m_intro->setWordWrap(true);
    layout->addWidget(m_title);
    layout->addWidget(m_intro);

    auto* cards = new QGridLayout();
    cards->setSpacing(12);
    const Preset presets[] = { Preset::Low, Preset::Balanced, Preset::High };
    int column = 0;
    for (auto preset : presets) {
        cards->addWidget(makeCard(preset), 0, column);
        cards->setColumnStretch(column, 1);
        ++column;
    }
    layout->addLayout(cards);

    m_source = new QLabel(this);
    m_source->setWordWrap(true);
    m_source->setOpenExternalLinks(true);
    m_source->setTextFormat(Qt::RichText);
    layout->addWidget(m_source);
    layout->addStretch(1);

    m_presets.setExclusive(true);
    m_presets.button(static_cast<int>(Preset::Balanced))->setChecked(true);
    connect(&m_presets, &QButtonGroup::idToggled, this, [this](int, bool checked) {
        if (checked) {
            updateState();
        }
    });

    retranslate();
}

QWidget* CraftEditionPage::makeCard(Preset preset)
{
    auto* card = new QWidget(this);
    card->setObjectName("craftEditionCard");
    card->setAttribute(Qt::WA_StyledBackground, true);
    auto* layout = new QVBoxLayout(card);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(6);

    // The radio button is the control; the labels describe it and are linked for screen readers.
    auto* radio = new QRadioButton(card);
    radio->setObjectName("craftEditionPreset");
    auto* summary = new QLabel(card);
    summary->setWordWrap(true);
    summary->setObjectName("craftEditionSummary");
    auto* details = new QLabel(card);
    details->setWordWrap(true);
    details->setObjectName("craftEditionDetails");
    summary->setBuddy(radio);

    layout->addWidget(radio);
    layout->addWidget(summary);
    layout->addWidget(details);
    layout->addStretch(1);

    m_presets.addButton(radio, static_cast<int>(preset));
    // Text is set in retranslate(); keep the widgets reachable from there.
    radio->setProperty("craftEditionPreset", static_cast<int>(preset));
    summary->setProperty("craftEditionPreset", static_cast<int>(preset));
    details->setProperty("craftEditionPreset", static_cast<int>(preset));
    return card;
}

void CraftEditionPage::retranslate()
{
    m_title->setText(QString("<h2>%1</h2>").arg(tr("Pick a preset for your PC")));
    m_intro->setText(tr(
        "All three presets use the same world generation, so worlds and servers stay compatible. You can install another preset later."));
    m_source->setText(tr("Packs are downloaded from the latest release on <a href=\"%1/releases\">GitHub</a>.")
                          .arg(QStringLiteral(CRAFT_EDITION_PACKS_REPO)));

    for (auto* radio : findChildren<QRadioButton*>("craftEditionPreset")) {
        radio->setText(presetText(static_cast<Preset>(radio->property("craftEditionPreset").toInt())).title);
    }
    for (auto* label : findChildren<QLabel*>("craftEditionSummary")) {
        label->setText(presetText(static_cast<Preset>(label->property("craftEditionPreset").toInt())).summary);
    }
    for (auto* label : findChildren<QLabel*>("craftEditionDetails")) {
        label->setText(presetText(static_cast<Preset>(label->property("craftEditionPreset").toInt())).details);
    }
}

void CraftEditionPage::openedImpl()
{
    updateState();
}

QString CraftEditionPage::presetKey(Preset preset)
{
    switch (preset) {
        case Preset::Low:
            return QStringLiteral("low");
        case Preset::Balanced:
            return QStringLiteral("balanced");
        case Preset::High:
            return QStringLiteral("high");
    }
    return QStringLiteral("balanced");
}

QUrl CraftEditionPage::packUrl(Preset preset)
{
    return QUrl(QStringLiteral(CRAFT_EDITION_PACKS_REPO "/releases/latest/download/CraftEdition-Java-%1.mrpack").arg(presetKey(preset)));
}

void CraftEditionPage::updateState()
{
    if (!isOpened) {
        return;
    }
    const int checked = m_presets.checkedId();
    if (checked < 0) {
        m_dialog->setSuggestedPack();
        return;
    }
    const auto preset = static_cast<Preset>(checked);
    const auto name = tr("Craft Edition %1").arg(presetText(preset).name);
    // Not a trusted source: the import task still asks before downloading files from hosts
    // outside the Modrinth allow-list, exactly as for any other .mrpack link.
    m_dialog->setSuggestedPack(name, new InstanceImportTask(packUrl(preset), false, this));
    m_dialog->setSuggestedIcon("default");
}
