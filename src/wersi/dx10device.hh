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

#include <wersi/instrumentstore.hh>

namespace DMSToolbox {
namespace Wersi {

/**
  @ingroup wersi_group

  Wersi DMS-System DX10/EX10R device handler class.

  This class implements an instrument store for the DX10/EX10R internal RAM. The binary representation isn't exactly
  as in the device's RAM.
 */
class Dx10Device : public InstrumentStore {
    public:
        /**
          Create new DX10/EX10R device object from buffer.

          Creates a new DX10/EX10R device object and associates the given buffer with it. During creation, all data
          items are initialized with zeroes but with the typical links between them.

          @param[in]    buffer      Raw data buffer
          @param[in]    size        Size of data buffer
         */
        Dx10Device(void* buffer, size_t size);

        /**
          Destroy DX10/EX10R device object.

          Destroys the DX10/EX10R device object.
         */
        virtual ~Dx10Device();

#ifdef HAVE_RTMIDI
        /// Implements InstrumentStore::readFromDevice()
        virtual void readFromDevice(RtMidiIn* inPort, RtMidiOut* outPort);
#endif // HAVE_RTMIDI

        /// Implements InstrumentStore::dissect()
        virtual void dissect();

        /// Implements InstrumentStore::update()
        virtual void update();

        /// Implements InstrumentStore::getNumIcbs()
        virtual size_t getNumIcbs() const {
            return 10;
        }

    private:
        Dx10Device(const Dx10Device&);              ///< Inhibit copying objects
        Dx10Device& operator=(const Dx10Device&);   ///< Inhibit copying objects
};

} // namespace Wersi
} // namespace DMSToolbox
