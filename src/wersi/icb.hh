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
#include <string>

namespace DMSToolbox {
namespace Wersi {

/**
  @ingroup wersi_group

  Wersi DMS-System ICB (instrument control block) handler class.

  The instrument control block pulls together all elements of a sound by using pointers to VCF, FREQ, AMPL and WAVE
  blocks. Audio routing and general sound controls is also configured in the ICB.
 */
class Icb {
    public:
        /**
          Create new ICB object from buffer.

          Creates a new ICB object with the given block number and associates the given buffer with it. During
          creation, the data from the buffer is parsed and copied to the object members. If an explicit Update()
          is called, the buffer is written back with the updated ICB object data, for all other functions, it is
          left untouched.

          @param[in]    blockNum    Block number
          @param[in]    buffer      Raw data buffer
         */
        Icb(uint8_t blockNum, void* buffer);

        /**
          Create ICB object by copying.

          Copies the ICB object by duplicating all it's members.

          @param[in]    source      Source object to copy from
         */
        Icb(const Icb& source);

        /**
          Destroy ICB object.

          Destroys the ICB object.
         */
        ~Icb();

        /**
          Copy ICB object.

          Copies all members of the ICB object from the source object.

          @param[in]    source      Source object to copy from

          @return                   This object
         */
        Icb& operator=(const Icb& source);

        /**
          Dissect ICB raw data buffer.

          Parses the raw ICB data buffer and updates object members.
         */
        void Dissect();

        /**
          Update ICB raw data buffer.

          Writes back changes in the ICB object to the associated raw ICB data buffer.
         */
        void Update();

    private:
        /// WersiVoice mode
        enum class WvMode {
            RotorSlow       = 0,            ///< Rotor, slow rotation
            RotorFast       = 1,            ///< Rotor, fast rotation
            Flanger         = 2,            ///< Flanger
            Strings         = 3,            ///< Strings
            Chorus          = 4,            ///< Chorus
            Invalid         = 7             ///< Invalid
        };

        uint8_t         m_blockNum;         ///< Block number
        uint8_t*        m_buffer;           ///< Associated raw buffer

        uint8_t         m_nextIcb;          ///< Next ICB pointer (for layering), 0 on last one
        uint8_t         m_vcfBlock;         ///< VCF block pointer
        uint8_t         m_amplBlock;        ///< AMPL block pointer
        uint8_t         m_freqBlock;        ///< FREQ block pointer
        uint8_t         m_waveBlock;        ///< WAVE block pointer
        bool            m_left;             ///< Left slave output enabled
        bool            m_right;            ///< Right slave output enabled
        bool            m_bright;           ///< Disable slave low pass on true
        bool            m_vcf;              ///< VCF slave output enabled
        bool            m_wv;               ///< WV slave output enabled
        int8_t          m_transpose;        ///< Transpose in semitones
        int8_t          m_detune;           ///< Detune (inverted)
        WvMode          m_wvMode;           ///< WersiVoice mode
        bool            m_wvLeft;           ///< Left WV output enabled
        bool            m_wvRight;          ///< Right WV output enabled
        bool            m_wvFbFlat;         ///< Feedback flat
        bool            m_wvFbDeep;         ///< Feedback deep
        std::string     m_name;             ///< Voice name
};

} // namespace Wersi
} // namespace DMSToolbox
