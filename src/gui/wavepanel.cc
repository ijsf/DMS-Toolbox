// vim:set ts=4 sw=4 et cin:

/*
  DMS-Toolbox - an editor, librarian and converter for the Wersi DMS system
  (C) 2015 Michael Kukat <michael_AT_mik-music.org>

  This file is part of DMS-Toolbox.

  DMS-Toolbox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DMS-Toolbox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DMS-Toolbox.  If not, see <http://www.gnu.org/licenses/>.

  Diese Datei ist Teil von DMS-Toolbox.

  DMS-Toolbox ist Freie Software: Sie können es unter den Bedingungen
  der GNU General Public License, wie von der Free Software Foundation,
  Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
  veröffentlichten Version, weiterverbreiten und/oder modifizieren.

  DMS-Toolbox wird in der Hoffnung, dass es nützlich sein wird, aber
  OHNE JEDE GEWÄHELEISTUNG, bereitgestellt; sogar ohne die implizite
  Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
  Siehe die GNU General Public License für weitere Details.

  Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
  Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 */

#include <gui/wavepanel.hh>
#include <wersi/wave.hh>
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#include <memory>

using namespace DMSToolbox::Wersi;

namespace DMSToolbox {
namespace Gui {

// Create wave panel
WavePanel::WavePanel(wxWindow* parent)
    : WavePanelBase(parent)
    , m_bassPanel(new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(512, 256)))
    , m_tenorPanel(new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(512, 256)))
    , m_altoPanel(new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(512, 256)))
    , m_sopranoPanel(new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(512, 256)))
    , m_wave(nullptr)
{
    m_bassPanelSizer->Add(m_bassPanel, 1, wxALIGN_CENTER | wxALL, 10);
    m_bassPanel->Connect(wxEVT_PAINT, wxPaintEventHandler(WavePanel::onPaint), NULL, this);
    m_tenorPanelSizer->Add(m_tenorPanel, 1, wxALIGN_CENTER | wxALL, 10);
    m_tenorPanel->Connect(wxEVT_PAINT, wxPaintEventHandler(WavePanel::onPaint), NULL, this);
    m_altoPanelSizer->Add(m_altoPanel, 1, wxALIGN_CENTER | wxALL, 10);
    m_altoPanel->Connect(wxEVT_PAINT, wxPaintEventHandler(WavePanel::onPaint), NULL, this);
    m_sopranoPanelSizer->Add(m_sopranoPanel, 1, wxALIGN_CENTER | wxALL, 10);
    m_sopranoPanel->Connect(wxEVT_PAINT, wxPaintEventHandler(WavePanel::onPaint), NULL, this);
}

// Set wave to edit
void WavePanel::setWave(Wave* wave)
{
    m_wave = wave;
    if (m_wave != nullptr) {
        m_waveLevelSlider->SetValue(m_wave->getLevel());
        m_fixedWaveCheckBox->SetValue(m_wave->getFixedFormants());
    }

    Refresh();
    Update();
}

// Handle paint event
void WavePanel::onPaint(wxPaintEvent& event)
{
    wxObject* obj = event.GetEventObject();
    uint8_t* source = nullptr;
    size_t size = 0;
    wxPanel* target = nullptr;
    if (obj == m_bassPanel) {
        if (m_wave != nullptr) {
            source = m_wave->getBass();
            size = 64;
        }
        target = m_bassPanel;
    }
    else if (obj == m_tenorPanel) {
        if (m_wave != nullptr) {
            source = m_wave->getTenor();
            size = 64;
        }
        target = m_tenorPanel;
    }
    else if (obj == m_altoPanel) {
        if (m_wave != nullptr) {
            source = m_wave->getAlto();
            size = 32;
        }
        target = m_altoPanel;
    }
    else if (obj == m_sopranoPanel) {
        if (m_wave != nullptr) {
            source = m_wave->getSoprano();
            size = 16;
        }
        target = m_sopranoPanel;
    }

    if (target != nullptr) {
        wxPaintDC dc(target);
        dc.Clear();
        if (source != nullptr) {
            std::unique_ptr<wxPoint[]> points(new wxPoint[size]);
            size_t factor = 512 / size;
            for (size_t i = 0; i < size; ++i) {
                points[i] = wxPoint(i * factor, source[i]);
            }
            dc.DrawSpline(size, points.get());
        }
    }
}

} // namespace Gui
} // namespace DMSToolbox
