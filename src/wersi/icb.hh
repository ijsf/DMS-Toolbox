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
        /// WersiVoice mode
        enum class WvMode {
            RotorSlow       = 0,            ///< Rotor, slow rotation
            RotorFast       = 1,            ///< Rotor, fast rotation
            Flanger         = 2,            ///< Flanger
            Strings         = 3,            ///< Strings
            Chorus          = 4,            ///< Chorus
            Invalid         = 7             ///< Invalid
        };

        /**
          Create new ICB object from buffer.

          Creates a new ICB object with the given block number and associates the given buffer with it. During
          creation, the data from the buffer is parsed and copied to the object members. If an explicit update()
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
        void dissect();

        /**
          Update ICB raw data buffer.

          Writes back changes in the ICB object to the associated raw ICB data buffer.
         */
        void update();

        /**
          Get next ICB.

          Returns the next ICB value for layered voices or 0 if no next ICB is available.

          @return                   Next ICB number
         */
        uint8_t getNextIcb() const {
            return m_nextIcb;
        }

        /**
          Get VCF block.

          Returns the VCF block number.

          @return                   VCF block number
         */
        uint8_t getVcfBlock() const {
            return m_vcfBlock;
        }

        /**
          Get AMPL block.

          Returns the AMPL block number.

          @return                   AMPL block number
         */
        uint8_t getAmplBlock() const {
            return m_amplBlock;
        }

        /**
          Get FREQ block.

          Returns the FREQ block number.

          @return                   FREQ block number
         */
        uint8_t getFreqBlock() const {
            return m_freqBlock;
        }

        /**
          Get WAVE block.

          Returns the WAVE block number.

          @return                   WAVE block number
         */
        uint8_t getWaveBlock() const {
            return m_waveBlock;
        }

        /**
          Get left output enabled.

          Returns true if the left output is enabled.

          @return                   True if the left output is enabled
         */
        bool getLeft() const {
            return m_left;
        }

        /**
          Get right output enabled.

          Returns true if the right output is enabled.

          @return                   True if the right output is enabled
         */
        bool getRight() const {
            return m_right;
        }

        /**
          Get bright flag.

          Returns true if the bright flag is enabled.

          @return                   True if the bright flag is enabled
         */
        bool getBright() const {
            return m_bright;
        }

        /**
          Get VCF output enabled.

          Returns true if the VCF output is enabled.

          @return                   True if the VCF output is enabled
         */
        bool getVcf() const {
            return m_vcf;
        }

        /**
          Get WersiVoice output enabled.

          Returns true if the WersiVoice output is enabled.

          @return                   True if the WersiVoice output is enabled
         */
        bool getWersiVoice() const {
            return m_wv;
        }

        /**
          Get transpose value.

          Returns the transpose value.

          @return                   Transpose value
         */
        int8_t getTranspose() const {
            return m_transpose;
        }

        /**
          Get detune value.

          Returns the detune value.

          @return                   Detune value
         */
        int8_t getDetune() const {
            return m_detune;
        }

        /**
          Get WersiVoice mode.

          Returns the WersiVoice mode.

          @return                   WersiVoice mode
         */
        WvMode getWvMode() const {
            return m_wvMode;
        }

        /**
          Get WersiVoice left output enabled.

          Returns true if the WersiVoice left output is enabled.

          @return                   True if the WersiVoice left output is enabled
         */
        bool getWvLeft() const {
            return m_wvLeft;
        }

        /**
          Get WersiVoice right output enabled.

          Returns true if the WersiVoice right output is enabled.

          @return                   True if the WersiVoice right output is enabled
         */
        bool getWvRight() const {
            return m_wvRight;
        }

        /**
          Get WersiVoice flat feedback enabled.

          Returns true if the WersiVoice flat feedback is enabled.

          @return                   True if the WersiVoice flat feedback output is enabled
         */
        bool getWvFbFlat() const {
            return m_wvFbFlat;
        }

        /**
          Get WersiVoice deep feedback enabled.

          Returns true if the WersiVoice deep feedback is enabled.

          @return                   True if the WersiVoice deep feedback output is enabled
         */
        bool getWvFbDeep() const {
            return m_wvFbDeep;
        }

        /**
          Get ICB name.

          Returns the ICB name.

          @return                   ICB name
         */
        std::string getName() const {
            return m_name;
        }

        /**
          Get unknown bits.

          Returns all bits from raw data that are currently unknown.

          @return                   Unknown bits
         */
        uint16_t getUnknownBits() const {
            return m_unknownBits;
        }

        /**
          Get WersiVoice mode name.

          Returns the name for the given WersiVoice mode or an empty string if invalid.

          @param[in]    mode        WersiVoice mode to look up

          @return                   Name for given WersiVoice mode
         */
        static std::string getWvModeName(WvMode mode);

    private:
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

        uint16_t        m_unknownBits;      ///< Currently unknown bits
};

} // namespace Wersi
} // namespace DMSToolbox
