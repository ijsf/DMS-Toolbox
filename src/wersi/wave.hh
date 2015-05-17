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

  Wersi DMS-System wave handler class.

  Wersi wave data can have two different types - relative formants and fixed formants. At the moment, only relative
  formants are completely understood, they consists of four simple PCM waves for different note ranges. Assuming that
  this behavior is similar for fixed format waves, the class provides access to those waves.
 */
class Wave {
    public:
        /**
          Create new wave object from buffer.

          Creates a new wave object with the given block number and associates the given buffer with it. During
          creation, the data from the buffer is parsed and copied to the object members. If an explicit update()
          is called, the buffer is written back with the updated wave object data, for all other functions, it is
          left untouched.

          @param[in]    blockNum    Block number
          @param[in]    buffer      Raw data buffer
          @param[in]    size        Size of raw data buffer
         */
        Wave(uint8_t blockNum, void* buffer, size_t size);

        /**
          Create wave object by copying.

          Copies the wave object by duplicating all it's members.

          @param[in]    source      Source object to copy from
         */
        Wave(const Wave& source);

        /**
          Destroy wave object.

          Destroys the wave object.
         */
        ~Wave();

        /**
          Copy wave object.

          Copies all members of the wave object from the source object.

          @param[in]    source      Source object to copy from

          @return                   This object
         */
        Wave& operator=(const Wave& source);

        /**
          Dissect wave raw data buffer.

          Parses the raw wave data buffer and updates object members.
         */
        void dissect();

        /**
          Update wave raw data buffer.

          Writes back changes in the wave object to the associated raw wave data buffer.
         */
        void update();

    private:
        uint8_t         m_blockNum;         ///< Block number
        uint8_t*        m_buffer;           ///< Associated raw buffer
        size_t          m_size;             ///< Size of associated raw buffer

        bool            m_fixedFormants;    ///< True if wave is using fixed formants
        uint8_t         m_level;            ///< Wave level
        uint8_t         m_bassWave[64];     ///< Bass wave
        uint8_t         m_tenorWave[64];    ///< Tenor wave
        uint8_t         m_altoWave[32];     ///< Alto wave
        uint8_t         m_sopranoWave[16];  ///< Soprano wave
        uint8_t         m_fixFormData[35];  ///< Fixed formant data
};

} // namespace Wersi
} // namespace DMSToolbox
