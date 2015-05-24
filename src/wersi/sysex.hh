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

  Wersi SysEx handler class.

  This class handles the encoding and decoding of SysEx messages in Wersi format.
 */
class SysEx {
    public:
        /// Message type
        enum class BlockType : uint8_t {
            RequestBlock    = 'r',          ///< Request block from instrument
            SwitchControl   = 's',          ///< Control instrument switches
            TransformBuffer = 't',          ///< Write to transform buffer (32 big endian 16bit words)
            IcBlock         = 'i',          ///< Instrument control block (ICB), 16 bytes
            VcfBlock        = 'v',          ///< VCF block, 10 bytes
            FreqBlock       = 'f',          ///< FREQ block, 32 bytes
            AmplBlock       = 'a',          ///< AMPL block, 44 bytes
            FixWaveBlock    = 'q',          ///< FIXWAVE block, 212 bytes
            RelWaveBlock    = 'w'           ///< RELWAVE block, 177 bytes
        };

        /// Raw message header
        struct Message {
            BlockType   m_type;             ///< Block type
            uint8_t     m_address;          ///< Address
            uint8_t     m_length;           ///< Message data length
            uint8_t     m_data[1];          ///< Message data, m_length bytes
        };

        /// Wersi SysEx message header
        struct SysExMessage {
            uint8_t     m_start;            ///< SysEx start byte (0xf0)
            uint8_t     m_vendor;           ///< SysEx vendor byte (0x25)
            uint8_t     m_device;           ///< Device, 0x01 for MK1, 0x02 for DX10
            uint8_t     m_typeLo;           ///< Type low nibble (plus 0x70)
            uint8_t     m_typeHi;           ///< Type high nibble (plus 0x60)
            uint8_t     m_addressLo;        ///< Address low nibble (plus 0x50)
            uint8_t     m_addressHi;        ///< Address high nibble (plus 0x40)
            uint8_t     m_lengthLo;         ///< Length low nibble (plus 0x30)
            uint8_t     m_lengthHi;         ///< Length high nibble (plus 0x20)
            uint8_t     m_data[1];          ///< Message data in nibbles, m_length * 2 bytes
        };

        /**
          Convert raw block to SysEx message.

          Converts the raw block to a SysEx message. The m_length field in the message gives the length of the
          m_data field. The output message must have enough room in the m_data field to hold twive this length plus
          the teminating SysEx byte. The length of the final message is returned.

          @param[in]        device      Device type to create message for
          @param[in]        message     Message data
          @param[in,out]    sysEx       Message data in SysEx format

          @return                       Length of the full message
         */
        static size_t toSysEx(uint8_t device, const Message& message, SysExMessage& sysEx);

        /**
          Convert SysEx message to raw block.

          Converts the SysEx message to raw data. The m_data field must be large enough to hold twice the amount
          given in the m_length field in the SysEx message, the m_data field in the raw message must have enough room
          to hold this amount of data.
          If an error occurs during parsing the message or if the device type doesn't match, a MidiException is thrown.

          @param[in]        device      Device type to check for
          @param[in]        sysEx       Message data in SysEx format
          @param[in,out]    message     Message data
         */
        static void fromSysEx(uint8_t device, const SysExMessage& sysEx, Message& message);
};

} // namespace Wersi
} // namespace DMSToolbox
