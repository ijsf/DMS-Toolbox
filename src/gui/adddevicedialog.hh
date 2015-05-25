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

#pragma once

#include <gui/gui.hh>

namespace DMSToolbox {
namespace Gui {

// Forward declarations
class MainFrame;

/**
  @ingroup gui_group

  Add device dialog implementation.

  This class implements the dialog to add MIDI devices.
 */
class AddDeviceDialog : public AddDeviceDialogBase {
    public:
        /**
          Create add device dialog.

          Creates an add device dialog, letting the user choose the MIDI devices, channel and type of a new Wersi
          device.

          @param[in]    parent      Main frame this dialog belongs to. Used as wxWindow and for MIDI port information
         */
        AddDeviceDialog(MainFrame* parent);

        /**
          Get device name.

          Returns the name for the new device.

          @return                   Name for the new device
         */
        wxString getName() const;

        /**
          Get MIDI input port index.

          Returns the MIDI input port index.

          @return                   MIDI input port index
         */
        unsigned int getInPort() const;

        /**
          Get MIDI output port index.

          Returns the MIDI output port index.

          @return                   MIDI output port index
         */
        unsigned int getOutPort() const;

        /**
          Get MIDI channel.

          Returns the MIDI channel.

          @return                   MIDI channel
         */
        uint8_t getChannel() const;

        /**
          Get device type.

          Returns the device type, 1 for MK1/EX20, 2 for DX10/EX10R.

          @return                   Device type
         */
        uint8_t getType() const;
};

} // namespace Gui
} // namespace DMSToolbox
