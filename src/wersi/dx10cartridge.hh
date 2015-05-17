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

  Wersi DMS-System DX10/DX5 cartridge handler class.

  There are two basic image formats for cartridges, the MK1 and the DX10/DX5 format. This class handles the DX10/DX5
  format, storing 8 presets and 10 instruments and optionally 24 rhythms and 8 sequences
 */
class Dx10Cartridge : public InstrumentStore {
    public:
        /**
          Create new DX10/DX5 cartridge object from buffer.

          Creates a new DX10/DX5 cartridge object and associates the given buffer with it. During creation, the data
          from the buffer is parsed and all contained objects are created for simple data access. If an explicit
          update() is called, the update() method of all contained objects is called to update their part of the
          buffer, then the cartridge raw buffer is updated with this new information.

          @todo implement double buffering for this

          @param[in]    buffer      Raw data buffer
          @param[in]    size        Size of data buffer
          @param[in]    initialize  If true, a blank DX10/DX5 cartridge is created
         */
        Dx10Cartridge(void* buffer, size_t size, bool initialize = false);

        /**
          Destroy DX10/DX5 cartridge object.

          Destroys the DX10/DX5 cartridge object.
         */
        virtual ~Dx10Cartridge();

        /// Implements InstrumentStore::dissect()
        virtual void dissect();

        /// Implements InstrumentStore::update()
        virtual void update();

    private:
        uint8_t*                    m_buffer;           ///< Associated raw buffer
        size_t                      m_size;

        Dx10Cartridge(const Dx10Cartridge&);            ///< Inhibit copying objects
        Dx10Cartridge& operator=(const Dx10Cartridge&); ///< Inhibit copying objects
};

} // namespace Wersi
} // namespace DMSToolbox
