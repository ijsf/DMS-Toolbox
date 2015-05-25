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
#include <wersi/dx10device.hh>
#include <wersi/icb.hh>
#include <wersi/sysex.hh>

#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/msgdlg.h>

#ifdef HAVE_RTMIDI
#include <RtMidi.h>
#endif // HAVE_RTMIDI

#include <iostream>
#include <iomanip>
using namespace std;
#include <unistd.h>

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
    , m_dragStore(nullptr)
#ifdef HAVE_RTMIDI
    , m_midiIn(nullptr)
    , m_midiOut(nullptr)
#endif // HAVE_RTMIDI
    , m_midiInPorts()
    , m_midiOutPorts()
{
    // Add panels
    m_mainTabs->AddPage(m_instPanel, _("Basic"), true);
    m_mainTabs->AddPage(m_envelopePanel, _("Envelopes"), false);
    m_mainTabs->AddPage(m_wavePanel, _("Waves"), false);
    m_mainTabs->Fit();

    // Do the window layout
    Fit();
}

// Destroy main frame
MainFrame::~MainFrame()
{
    for (auto& i : m_instrumentStores) {
        if (i.second.m_store != nullptr) {
            auto buffer = static_cast<uint8_t*>(i.second.m_store->getBuffer());
            delete[] buffer;
            delete i.second.m_store;
        }
    }
#ifdef HAVE_RTMIDI
    if (m_midiIn != nullptr) {
        delete m_midiIn;
    }
    if (m_midiOut != nullptr) {
        delete m_midiOut;
    }
#endif // HAVE_RTMIDI
}

// Apply configuration
void MainFrame::applyConfiguration()
{
#ifdef HAVE_RTMIDI
    // Initialize MIDI
    bool midiOk = true;
    if (m_midiIn == nullptr) {
        try {
            m_midiIn = new RtMidiIn();
        }
        catch (RtMidiError&) {
            midiOk = false;
        }
    }
    if (m_midiOut == nullptr) {
        try {
            m_midiOut = new RtMidiOut();
        }
        catch (RtMidiError&) {
            midiOk = false;
        }
    }
    if (midiOk) {
        m_midiInPorts.clear();
        unsigned int numPorts = m_midiIn->getPortCount();
        for (unsigned int idx = 0; idx < numPorts; ++idx) {
            wxString name(wxString::FromUTF8(m_midiIn->getPortName(idx).c_str()));
            m_midiInPorts.insert(std::pair<unsigned int, wxString>(idx, name));
        }
        m_midiOutPorts.clear();
        numPorts = m_midiOut->getPortCount();
        for (unsigned int idx = 0; idx < numPorts; ++idx) {
            wxString name(wxString::FromUTF8(m_midiOut->getPortName(idx).c_str()));
            m_midiOutPorts.insert(std::pair<unsigned int, wxString>(idx, name));
        }
    }
    else {
        wxMessageDialog err(this, _("The MIDI subsystem reported an error, so MIDI is not available"),
                            _("Could not initialize MIDI"), wxOK | wxCENTRE | wxICON_ERROR);
        err.ShowModal();
    }

    // Read configured devices
    {
        m_config.SetPath(wxT("/Devices"));
        wxString name;
        long index;
        bool cont = m_config.GetFirstGroup(name, index);
        while (cont) {
            m_config.SetPath(name);
            try {
                InstStore is;
                is.m_store = nullptr;

                wxString inPortName = m_config.Read(wxT("InPort"));
                bool found = false;
                unsigned int numPorts = m_midiIn->getPortCount();
                for (unsigned int idx = 0; idx < numPorts; ++idx) {
                    wxString name(wxString::FromUTF8(m_midiIn->getPortName(idx).c_str()));
                    if (name == inPortName) {
                        is.m_inPort = idx;
                        found = true;
                    }
                }
                if (!found) {
                    throw ConfigurationException("MIDI input port not found");
                }

                wxString outPortName = m_config.Read(wxT("OutPort"));
                found = false;
                numPorts = m_midiOut->getPortCount();
                for (unsigned int idx = 0; idx < numPorts; ++idx) {
                    wxString name(wxString::FromUTF8(m_midiOut->getPortName(idx).c_str()));
                    if (name == outPortName) {
                        is.m_outPort = idx;
                        found = true;
                    }
                }
                if (!found) {
                    throw ConfigurationException("MIDI output port not found");
                }

                long tmp = 0;
                if (!m_config.Read(wxT("Channel"), &tmp) || tmp < 1 || tmp > 16) {
                    throw ConfigurationException("Invalid value for MIDI channel");
                }
                is.m_channel = uint8_t(tmp);
                if (!m_config.Read(wxT("Type"), &tmp) || tmp < 1 || tmp > 2) {
                    throw ConfigurationException("Invalid value for device type");
                }
                is.m_type = uint8_t(tmp);

                // Create instrument store
                is.m_store = new Dx10Device(new uint8_t[6180], 6180);

                auto id = m_instTree->AppendItem(m_devices, name, -1, -1, new InstrumentHelper(is, 0));
                for (auto& i : *(is.m_store)) {
                    wxString instName(wxT("("));
                    instName << uint16_t(i.first) << wxT(") ");
                    instName << wxString::From8BitData(i.second.getName().c_str());
                    m_instTree->AppendItem(id, instName, -1, -1, new InstrumentHelper(is, i.first));
                }
                m_instrumentStores.insert(std::pair<wxString, InstStore>(name, is));
            }
            catch (Exception& e) {
                wxString msg(_("Device '"));
                msg << name << _("' could not be created, reason: ");
                msg << wxString::FromUTF8(e.what());
                wxMessageDialog err(this, msg, _("Could not creatre device"),
                                    wxOK | wxCENTRE | wxICON_ERROR);
                err.ShowModal();
            }
            m_config.SetPath(wxT(".."));
            cont = m_config.GetNextGroup(name, index);
        }
    }
#endif // HAVE_RTMIDI

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

    // Expand top level trees
    m_instTree->Expand(m_devices);
    m_instTree->Expand(m_cartridges);
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
        if (store.m_store != nullptr && icbNum == 0) {
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
        if (store.m_store != nullptr && icbNum == 0) {
            // Scan through instrument stores and check for duplicate name
            auto newLabel = event.GetLabel();
            wxString oldLabel;
            bool veto = false;
            for (auto& i : m_instrumentStores) {
                if (i.second.m_store == store.m_store) {
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
                wxMessageDialog err(this, _("Device or cartridge with this name already exists"),
                                    _("Could not rename"), wxOK | wxCENTRE | wxICON_ERROR);
                err.ShowModal();
            }
            else {
                auto old = m_instrumentStores.find(oldLabel);
                if (old != m_instrumentStores.end()) {
                    m_instrumentStores.erase(old);
                }
                m_instrumentStores.insert(std::pair<wxString, InstStore>(newLabel, store));
            }
        }
    }
}

// Handle instrument selection
void MainFrame::onInstSelect(wxTreeEvent& event)
{
    // Check for MIDI device add
    if (event.GetItem() == m_devices) {
        addDevice();
        return;
    }

    //auto prevSel = m_instTree->GetItemData(event.GetOldItem());
    auto sel = m_instTree->GetItemData(event.GetItem());

    if (sel != nullptr) {
        auto inst = dynamic_cast<InstrumentHelper*>(sel);
        auto store = inst->getStore();
        auto icbNum = inst->getIcb();
        if (store.m_store != nullptr && icbNum != 0) {
            Icb* icb = store.m_store->getIcb(icbNum);
            if (icb != nullptr) {
                m_instPanel->setInstrument(store.m_store, icbNum);
                m_envelopePanel->setEnvelopes(store.m_store->getAmpl(icb->getAmplBlock()),
                                              store.m_store->getFreq(icb->getFreqBlock()));
                m_wavePanel->setWave(store.m_store->getWave(icb->getWaveBlock()));
            }
        }
        else if (store.m_store != nullptr && icbNum == 0 && store.m_type != 0) {
            writeDevice(store);
        }
    }
}

// Handle begin drag event
void MainFrame::onInstBeginDrag(wxTreeEvent& event)
{
    auto sel = m_instTree->GetItemData(event.GetItem());

    if (sel != nullptr) {
        auto inst = dynamic_cast<InstrumentHelper*>(sel);
        auto store = inst->getStore();
        auto icbNum = inst->getIcb();
        if (store.m_store != nullptr && store.m_type == 0 && icbNum == 0) {
            // Instrument store drag - allow it
            m_dragStore = store.m_store;
            event.Allow();
            return;
        }
    }
    event.Veto();
}

// Handle end drag event
void MainFrame::onInstEndDrag(wxTreeEvent& event)
{
    auto item = event.GetItem();
    auto sel = m_instTree->GetItemData(item);

    if (sel != nullptr) {
        auto inst = dynamic_cast<InstrumentHelper*>(sel);
        auto store = inst->getStore();
        auto icbNum = inst->getIcb();
        if (store.m_store != nullptr && m_dragStore != nullptr
                &&store.m_store != m_dragStore && store.m_type != 0 && icbNum == 0) {
            // Instrument store drag from cartridge to device - allow it
            store.m_store->copyContents(*m_dragStore);
            m_instTree->DeleteChildren(item);
            for (auto& i : *(store.m_store)) {
                wxString instName(wxT("("));
                instName << uint16_t(i.first) << wxT(") ");
                instName << wxString::From8BitData(i.second.getName().c_str());
                m_instTree->AppendItem(item, instName, -1, -1, new InstrumentHelper(store, i.first));
            }
            event.Allow();
            return;
        }
    }
    m_dragStore = nullptr;
    event.Veto();
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

            InstStore is;
            is.m_store = store;
            is.m_inPort = 0;
            is.m_outPort = 0;
            is.m_channel = 0;
            is.m_type = 0;
            auto id = m_instTree->AppendItem(m_cartridges, cartName, -1, -1, new InstrumentHelper(is, 0));
            for (auto& i : *store) {
                wxString instName(wxT("("));
                instName << uint16_t(i.first) << wxT(") ");
                instName << wxString::From8BitData(i.second.getName().c_str());
                m_instTree->AppendItem(id, instName, -1, -1, new InstrumentHelper(is, i.first));
            }
            m_instrumentStores.insert(std::pair<wxString, InstStore>(cartName, is));
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

// Add MIDI device
void MainFrame::addDevice()
{
    // Check if we have MIDI
    if (m_midiOutPorts.size() == 0 || m_midiInPorts.size() == 0) {
        wxMessageDialog err(this, _("There are no MIDI ports available to add a new device"),
                            _("Can not add new device"), wxOK | wxCENTRE | wxICON_ERROR);
        err.ShowModal();
        return;
    }

    // Show device add dialog
    AddDeviceDialog dlg(this);
    if (dlg.ShowModal() == wxID_OK) {
        // Get data from device dialog
        try {
            const wxString& name = dlg.getName();
            auto ent = m_instrumentStores.find(name);
            if (ent != m_instrumentStores.end()) {
                throw ConfigurationException("Device or cartridge with this name already exists");
            }
            InstStore is;
            is.m_store = nullptr;
            is.m_inPort = dlg.getInPort();
            is.m_outPort = dlg.getOutPort();
            is.m_channel = dlg.getChannel();
            is.m_type = dlg.getType();

            // Create instrument store
            is.m_store = new Dx10Device(new uint8_t[6180], 6180);

            auto id = m_instTree->AppendItem(m_devices, name, -1, -1, new InstrumentHelper(is, 0));
            for (auto& i : *(is.m_store)) {
                wxString instName(wxT("("));
                instName << uint16_t(i.first) << wxT(") ");
                instName << wxString::From8BitData(i.second.getName().c_str());
                m_instTree->AppendItem(id, instName, -1, -1, new InstrumentHelper(is, i.first));
            }
            m_instrumentStores.insert(std::pair<wxString, InstStore>(name, is));
            m_config.SetPath(wxT("/Devices"));
            m_config.SetPath(name);
#ifdef HAVE_RTMIDI
            m_config.Write(wxT("InPort"), wxString::FromUTF8(m_midiIn->getPortName(is.m_inPort).c_str()));
            m_config.Write(wxT("OutPort"), wxString::FromUTF8(m_midiOut->getPortName(is.m_outPort).c_str()));
            m_config.Write(wxT("Channel"), long(is.m_channel));
            m_config.Write(wxT("Type"), long(is.m_type));
#endif // HAVE_RTMIDI
            m_config.Flush();

            //readDevice(is);
        }
        catch (ConfigurationException& e) {
            wxString msg(_("Could not add device: "));
            msg << wxString::FromUTF8(e.what());
            wxMessageDialog err(this, msg, _("Could not add device"), wxOK | wxCENTRE | wxICON_ERROR);
            err.ShowModal();
        }
    }
}

// Read MIDI device
#ifdef HAVE_RTMIDI
void MainFrame::readDevice(const InstStore& store)
{
    m_midiIn->openPort(store.m_inPort);
    m_midiIn->ignoreTypes(false, true, true);
    m_midiOut->openPort(store.m_outPort);
    auto buf = new unsigned char[1024];
    auto sem = reinterpret_cast<SysEx::SysExMessage*>(buf);

    SysEx::Message msg;
    msg.m_type = SysEx::BlockType::RequestBlock;
    msg.m_address = 0x41;
    msg.m_length = 1;
    msg.m_data[0] = static_cast<uint8_t>(SysEx::BlockType::FixWaveBlock);
    std::vector<unsigned char> midi;

    midi.clear();
    size_t len = SysEx::toSysEx(store.m_type, msg, *sem);
    for (size_t i = 0; i < len; ++i) {
        midi.push_back(buf[i]);
    }
    cout << hex << setfill('0');
    for (auto& i : midi) {
        cout << " " << setw(2) << uint16_t(i);
    }
    cout << setfill(' ') << dec << endl;

    if (!midi.empty()) {
        m_midiOut->sendMessage(&midi);

        bool done = false;
        while (true) {
            midi.clear();
            m_midiIn->getMessage(&midi);
            if (midi.empty()) {
                if (!done) {
                    sleep(10);
                }
                m_midiIn->getMessage(&midi);
                if (midi.empty()) {
                    break;
                }
            }
            cout << midi.size() << endl;
            cout << hex << setfill('0');
            for (auto& i : midi) {
                cout << " " << setw(2) << uint16_t(i);
            }
            cout << setfill(' ') << dec << endl;
            if (midi[0] == 0xf0 || midi[0] == 0xc0) {
                done = true;
            }
        }
        cout << "Done" << endl;
    }

    delete[] buf;
    m_midiOut->closePort();
    m_midiIn->closePort();
}

// Write MIDI device
void MainFrame::writeDevice(const InstStore& store)
{
    m_midiOut->openPort(store.m_outPort);
    uint8_t offset = 0;

    // Send ICBs
    for (auto& i : *(store.m_store)) {
        SysEx::sendIcb(m_midiOut, store.m_type, i.first, i.second);
        if (offset == 0) {
            offset = i.first - 1;
        }
    }

    // Send VCFs
    for (size_t i = 0; i < 10; ++i) {
        uint8_t addr = i + offset;
        auto vcf = store.m_store->getVcf(addr);
        if (vcf != nullptr) {
            SysEx::sendVcf(m_midiOut, store.m_type, addr, *vcf);
        }
    }

    // Send AMPLs
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + offset;
        if (i >= 10) {
            ++addr;
        }
        auto ampl = store.m_store->getAmpl(addr);
        if (ampl != nullptr) {
            SysEx::sendAmpl(m_midiOut, store.m_type, addr, *ampl);
        }
    }

    // Send FREQs
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + offset;
        if (i >= 10) {
            ++addr;
        }
        auto freq = store.m_store->getFreq(addr);
        if (freq != nullptr) {
            SysEx::sendFreq(m_midiOut, store.m_type, addr, *freq);
        }
    }

    // Send WAVEs
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + offset;
        if (i >= 10) {
            ++addr;
        }
        auto wave = store.m_store->getWave(addr);
        if (wave != nullptr) {
            SysEx::sendWave(m_midiOut, store.m_type, addr, *wave);
        }
    }

    m_midiOut->closePort();
}
#else // HAVE_RTMIDI
void MainFrame::readDevice(const InstStore& /*store*/)
{
}
void MainFrame::writeDevice(const InstStore& /*store*/)
{
}
#endif // HAVE_RTMIDI

} // namespace Gui
} // namespace DMSToolbox
