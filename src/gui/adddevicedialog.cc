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

#include <gui/adddevicedialog.hh>
#include <gui/mainframe.hh>
#include <exceptions.hh>

namespace DMSToolbox {
namespace Gui {

// Helper to associate MIDI port index with choice item
class PortHelper : public wxClientData {
    public:
        PortHelper(unsigned int index)
            : wxClientData()
            , m_index(index) {
        }

        unsigned int getIndex() {
            return m_index;
        }

    private:
        unsigned int m_index;
};

// Create add device dialog
AddDeviceDialog::AddDeviceDialog(MainFrame* parent, const std::map<unsigned int, wxString>& inPorts,
                                 const std::map<unsigned int, wxString>& outPorts)
    : AddDeviceDialogBase(parent)
{
    // Preset a name
    m_nameInput->SetValue(_("new device"));

    // Fill MIDI port choices
    m_inPortChoice->Clear();
    for (auto& i : inPorts) {
        m_inPortChoice->Append(i.second, new PortHelper(i.first));
    }
    m_inPortChoice->SetSelection(0);
    m_outPortChoice->Clear();
    for (auto& i : outPorts) {
        m_outPortChoice->Append(i.second, new PortHelper(i.first));
    }
    m_outPortChoice->SetSelection(0);

    // Preset channel and type
    m_channelChoice->SetSelection(0);
    m_typeChoice->SetSelection(0);

    Fit();
}

// Get device name
wxString AddDeviceDialog::getName() const
{
    const wxString& str = m_nameInput->GetValue();
    if (str.IsEmpty()) {
        throw ConfigurationException("No name given");
    }
    return str;
}

// Get MIDI input port index
unsigned int AddDeviceDialog::getInPort() const
{
    int sel = m_inPortChoice->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto cd = static_cast<PortHelper*>(m_inPortChoice->GetClientObject(sel));
        if (cd != nullptr) {
            return cd->getIndex();
        }
    }
    throw ConfigurationException("No MIDI input port selected");
}

// Get MIDI output port index
unsigned int AddDeviceDialog::getOutPort() const
{
    int sel = m_outPortChoice->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto cd = static_cast<PortHelper*>(m_outPortChoice->GetClientObject(sel));
        if (cd != nullptr) {
            return cd->getIndex();
        }
    }
    throw ConfigurationException("No MIDI output port selected");
}

// Get MIDI channel
uint8_t AddDeviceDialog::getChannel() const
{
    int sel = m_channelChoice->GetSelection();
    if (sel == wxNOT_FOUND) {
        throw ConfigurationException("No MIDI channel selected");
    }

    return sel + 1;
}

// Get device type
uint8_t AddDeviceDialog::getType() const
{
    int sel = m_typeChoice->GetSelection();
    if (sel == wxNOT_FOUND) {
        throw ConfigurationException("No device type selected");
    }

    return sel + 1;
}

} // namespace Gui
} // namespace DMSToolbox
