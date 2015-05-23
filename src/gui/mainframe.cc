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

#include <gui/mainframe.hh>
#include <gui/instpanel.hh>
#include <gui/envelopepanel.hh>
#include <gui/wavepanel.hh>

#include <iostream>
using namespace std;

namespace DMSToolbox {
namespace Gui {

// Create main frame
MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBase(parent)
    , m_instPanel(new InstPanel(m_mainTabs))
    , m_envelopePanel(new EnvelopePanel(m_mainTabs))
    , m_wavePanel(new WavePanel(m_mainTabs))
{
    // Add panels
    m_mainTabs->AddPage(m_instPanel, _("Basic"), true);
    m_mainTabs->AddPage(m_envelopePanel, _("Envelopes"), false);
    m_mainTabs->AddPage(m_wavePanel, _("Waves"), false);
    m_mainTabs->Fit();
    Fit();

    // TODO dummy data
    m_root = m_instTree->AddRoot(_("Instruments"));
    m_instrument = m_instTree->AppendItem(m_root, wxT("MK1"));
    m_cartridges = m_instTree->AppendItem(m_root, wxT("Cartridges"));

    m_instTree->AppendItem(m_instrument, wxT("Bank 1"));

    m_instTree->AppendItem(m_cartridges, wxT("DX10V1 ROM 1"));
    m_instTree->AppendItem(m_cartridges, wxT("MK1 ROM 1"));
    m_instTree->AppendItem(m_cartridges, wxT("MK1 ROM 2"));
    m_instTree->AppendItem(m_cartridges, wxT("MK1 ROM 3"));
}

// Handle instrument selection
void MainFrame::onInstSelect(wxTreeEvent& /*event*/)
{
    cout << "Instrument selected" << endl;
}

} // namespace Gui
} // namespace DMSToolbox
