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
#include <exceptions.hh>
#include <wersi/mk1cartridge.hh>
#include <wersi/dx10cartridge.hh>
#include <wersi/icb.hh>

#include <fstream>
using namespace std;

using namespace DMSToolbox::Wersi;

namespace DMSToolbox {
namespace Gui {

// Create main frame
MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBase(parent)
    , m_instPanel(new InstPanel(m_mainTabs))
    , m_envelopePanel(new EnvelopePanel(m_mainTabs))
    , m_wavePanel(new WavePanel(m_mainTabs))
    , m_root(m_instTree->AddRoot(_("Instruments")))
    , m_devices(m_instTree->AppendItem(m_root, _("Devices")))
    , m_cartridges(m_instTree->AppendItem(m_root, _("Cartridges")))
{
    // Add panels
    m_mainTabs->AddPage(m_instPanel, _("Basic"), true);
    m_mainTabs->AddPage(m_envelopePanel, _("Envelopes"), false);
    m_mainTabs->AddPage(m_wavePanel, _("Waves"), false);
    m_mainTabs->Fit();

    // TODO dummy data
    m_instTree->AppendItem(m_devices, wxT("MIDI 1 / MK1 / Bank 1"));
    addCartridge("630813_J_ROM1_MK1.bin", "MK1 ROM1");
    addCartridge("4201073_630822_U_LAMBADA_DX5.bin", "DX5 Lambada");
    addCartridge("4201013_ROM1_DX10V2.bin", "DX10V2 ROM1");

    // Expand some trees
    m_instTree->Expand(m_root);
    m_instTree->Expand(m_devices);
    m_instTree->Expand(m_cartridges);

    // Do the window layout
    Fit();
}

// Handle instrument selection
void MainFrame::onInstSelect(wxTreeEvent& event)
{
    //auto prevSel = m_instTree->GetItemData(event.GetOldItem());
    auto sel = m_instTree->GetItemData(event.GetItem());

    if (sel != nullptr) {
        auto inst = dynamic_cast<InstrumentHelper*>(sel);
        auto store = inst->getStore();
        auto icbNum = inst->getIcb();
        if (store != nullptr && icbNum != 0) {
            Icb* icb = store->getIcb(icbNum);
            if (icb != nullptr) {
                m_instPanel->setInstrument(store, icbNum);
                m_envelopePanel->setEnvelopes(store->getAmpl(icb->getAmplBlock()),
                                              store->getFreq(icb->getFreqBlock()));
                m_wavePanel->setWave(store->getWave(icb->getWaveBlock()));
            }
        }
    }
}

// TODO temporary - add cartridge from file
void MainFrame::addCartridge(const std::string& fileName, const std::string& cartName)
{
    string path("/home/michael/Documents/NoBackup/EPROMs/Wersi/Cartridges/");
    path.append(fileName);
    ifstream f(path, ios::binary);
    if (f) {
        f.seekg(0, ios::end);
        size_t size = f.tellg();
        f.seekg(0, ios::beg);
        if (size == 16384) {
            char* buffer(nullptr);
            InstrumentStore* store(nullptr);
            try {
                buffer = new char[size];
                f.read(buffer, size);
                if (f.gcount() != 16384) {
                    throw DataFormatException("Could not read cartridge data");
                }
                if (store == nullptr) {
                    try {
                        store = new Mk1Cartridge(buffer);
                    }
                    catch (DataFormatException&) {
                        // Ignore data format errors, try next type
                    }
                }
                if (store == nullptr) {
                    try {
                        store = new Dx10Cartridge(buffer, size);
                    }
                    catch (DataFormatException&) {
                        // Ignore data format errors, try next type
                    }
                }
                if (store == nullptr) {
                    throw DataFormatException("Unknown cartridge format");
                }

                auto id = m_instTree->AppendItem(m_cartridges, wxString::From8BitData(cartName.c_str()),
                                                 -1, -1, new InstrumentHelper(store, 0));
                for (auto& i : *store) {
                    Icb& icb = i.second;
                    m_instTree->AppendItem(id, wxString::From8BitData(icb.getName().c_str()),
                                           -1, -1, new InstrumentHelper(store, i.first));
                }
            }
            catch (Exception&) {
                if (store != nullptr) {
                    delete store;
                }
                if (buffer != nullptr) {
                    delete[] buffer;
                }
                // Ignore errors - cartridge not set
            }
        }
    }
}

} // namespace Gui
} // namespace DMSToolbox
