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
#include <gui/adddevicedialog.hh>
#include <exceptions.hh>
#include <wersi/mk1cartridge.hh>
#include <wersi/dx10cartridge.hh>
#include <wersi/icb.hh>

#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/msgdlg.h>

using namespace DMSToolbox::Wersi;

namespace DMSToolbox {
namespace Gui {

// Create main frame
MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBase(parent)
    , m_config(wxT("DMS-Toolbox"), wxT("MusicMiK"))
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

    // Expand some trees
    m_instTree->Expand(m_devices);
    m_instTree->Expand(m_cartridges);

    // Do the window layout
    Fit();
}

// Destroy main frame
MainFrame::~MainFrame()
{
    for (auto& i : m_instrumentStores) {
        auto buffer = static_cast<uint8_t*>(i.second->getBuffer());
        delete[] buffer;
    }
}

// Apply configuration
void MainFrame::applyConfiguration()
{
    // Read cartridges opened last time
    m_config.SetPath(wxT("/Cartridges"));
    wxString name;
    long index;
    bool cont = m_config.GetFirstEntry(name, index);
    while (cont) {
        wxString path = m_config.Read(name);
        try {
            readCartridgeFile(path, name);
        }
        catch (Exception& e) {
            wxString msg(_("Cartridge file '"));
            msg << path << _("' could not be read, reason: ");
            msg << wxString::FromUTF8(e.what());
            wxMessageDialog err(this, msg, _("Could not load cartridge"),
                                wxOK | wxCENTRE | wxICON_ERROR);
            err.ShowModal();
        }
        cont = m_config.GetNextEntry(name, index);
    }
}

// Handle instrument deletion
void MainFrame::onInstDelete(wxTreeEvent& event)
{
    event.Veto();
}

// Handle instrument rename begin
void MainFrame::onInstRenameBegin(wxTreeEvent& event)
{
    // Check if a rename is allowed
    auto ren = m_instTree->GetItemData(event.GetItem());

    if (ren != nullptr) {
        auto inst = dynamic_cast<InstrumentHelper*>(ren);
        auto store = inst->getStore();
        auto icbNum = inst->getIcb();
        if (store != nullptr && icbNum == 0) {
            // Only instrument stores may be renamed
            return;
        }
    }

    event.Veto();
}

// Handle instrument rename
void MainFrame::onInstRename(wxTreeEvent& event)
{
    if (event.IsEditCancelled()) {
        return;
    }
    auto id = event.GetItem();
    auto ren = m_instTree->GetItemData(id);
    if (ren != nullptr) {
        auto inst = dynamic_cast<InstrumentHelper*>(ren);
        auto store = inst->getStore();
        auto icbNum = inst->getIcb();
        if (store != nullptr && icbNum == 0) {
            // Scan through instrument stores and check for duplicate name
            auto newLabel = event.GetLabel();
            wxString oldLabel;
            bool veto = false;
            for (auto& i : m_instrumentStores) {
                if (i.second == store) {
                    oldLabel = i.first;
                }
                if (i.first == newLabel) {
                    veto = true;
                    break;
                }
            }

            // Check if rename is allowed
            if (!veto) {
                m_config.SetPath(wxT("/Cartridges"));
                veto = !m_config.RenameEntry(oldLabel, newLabel);
            }
            if (veto) {
                event.Veto();
                wxMessageDialog err(this, _("Instrument store with this name already exists"),
                                    _("Could not rename"), wxOK | wxCENTRE | wxICON_ERROR);
                err.ShowModal();
            }
            else {
                auto old = m_instrumentStores.find(oldLabel);
                if (old != m_instrumentStores.end()) {
                    m_instrumentStores.erase(old);
                }
                m_instrumentStores.insert(std::pair<wxString, InstrumentStore*>(newLabel, store));
            }
        }
    }
}

// Handle instrument selection
void MainFrame::onInstSelect(wxTreeEvent& event)
{
    // Check for MIDI device add
    if (event.GetItem() == m_devices) {
        AddDeviceDialog dlg(this);
        dlg.ShowModal();
        return;
    }

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

// Handle file/open menu item
void MainFrame::onFileOpen(wxCommandEvent& /*event*/)
{
    // Create file dialog to open file
    wxFileDialog dlg(this, _("Select cartridge file to load"), wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_OPEN);
    if (dlg.ShowModal() == wxID_CANCEL) {
        return;
    }
    wxFileName fn(dlg.GetPath());
    try {
        readCartridgeFile(dlg.GetPath(), fn.GetFullName());
        m_config.SetPath(wxT("/Cartridges"));
        m_config.Write(fn.GetFullName(), dlg.GetPath());
        m_config.Flush();
    }
    catch (Exception& e) {
        wxMessageDialog err(this, wxString::FromUTF8(e.what()), _("Could not load cartridge"),
                            wxOK | wxCENTRE | wxICON_ERROR);
        err.ShowModal();
    }
}

// Handle edit/rename menu item
void MainFrame::onEditRename(wxCommandEvent& /*event*/)
{
}

// Read cartridge file and create instrument store from it.
void MainFrame::readCartridgeFile(const wxString& filePath, const wxString& cartName)
{
    // Open and check file
    if (!wxFile::Exists(filePath)) {
        throw SystemException("File does not exist");
    }
    wxFile file(filePath, wxFile::read);
    wxFileOffset size = file.Length();
    if (size != 8192 && size != 16384) {
        throw DataFormatException("Invalid file size (must be 8 or 16 KB)");
    }
    wxFileInputStream fileStream(file);
    if (fileStream.IsOk()) {
        InstrumentStore* store(nullptr);
        char* buffer(new char[size]);
        try {
            fileStream.Read(buffer, size);
            if (fileStream.LastRead() != size_t(size)) {
                throw DataFormatException("Could not read whole cartridge data");
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

            auto id = m_instTree->AppendItem(m_cartridges, cartName, -1, -1, new InstrumentHelper(store, 0));
            for (auto& i : *store) {
                wxString instName(wxT("("));
                instName << uint16_t(i.first) << wxT(") ");
                instName << wxString::From8BitData(i.second.getName().c_str());
                m_instTree->AppendItem(id, instName, -1, -1, new InstrumentHelper(store, i.first));
            }
            m_instrumentStores.insert(std::pair<wxString, InstrumentStore*>(cartName, store));
        }
        catch (...) {
            if (store != nullptr) {
                delete store;
            }
            if (buffer != nullptr) {
                delete[] buffer;
            }
            throw;
        }
    }
    else {
        throw SystemException("Unable to open file");
    }
}

} // namespace Gui
} // namespace DMSToolbox
