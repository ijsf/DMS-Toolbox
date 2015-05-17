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

#include <common.hh>

namespace DMSToolbox {
namespace Wersi {

/**
  @ingroup wersi_group

  Wersi DMS-System envelope handler class.

  Wersi envelopes are pretty complex, the data structures represent small programs that are executed by the slaves.
  The first two bytes of an envelope are always pointers for the release and the attack phase, the rest is envelope
  program data. This class provides an assembler and disassembler for this envelope data, it also allows inserting
  and deleting instructions in the envelope program with updating all the necessary pointers. So the envelope program
  can be edited and is assembled back into the raw data structure on an Update() call.
 */
class Envelope {
    public:
        /**
          Create new envelope object from buffer.

          Creates a new VCF object with the given block number and associates the given buffer with it. During
          creation, the data from the buffer is parsed and copied to the object members. If an explicit Update()
          is called, the buffer is written back with the updated envelope object data, for all other functions, it is
          left untouched.

          @param[in]    blockNum    Block number
          @param[in]    buffer      Raw data buffer
          @param[in]    size        Size of raw data buffer
         */
        Envelope(uint8_t blockNum, void* buffer, size_t size);

        /**
          Create envelope object by copying.

          Copies the evelope object by duplicating all it's members.

          @param[in]    source      Source object to copy from
         */
        Envelope(const Envelope& source);

        /**
          Destroy envelope object.

          Destroys the envelope object.
         */
        ~Envelope();

        /**
          Copy envelope object.

          Copies all members of the envelope object from the source object.

          @param[in]    source      Source object to copy from

          @return                   This object
         */
        Envelope& operator=(const Envelope& source);

        /**
          Dissect envelope raw data buffer.

          Parses the raw envelope data buffer and updates object members.
         */
        void Dissect();

        /**
          Update envelope raw data buffer.

          Writes back changes in the envelope object to the associated raw envelope data buffer.
         */
        void Update();

    private:
        uint8_t         m_blockNum;         ///< Block number
        uint8_t*        m_buffer;           ///< Associated raw buffer
        size_t          m_size;             ///< Size of associated raw buffer
};

} // namespace Wersi
} // namespace DMSToolbox
