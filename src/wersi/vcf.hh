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

  Wersi DMS-System VCF (voltage controlled filter control block) handler class

  The VCF block keeps configuration of the VCF mode, audio routing, noise and envelope.
 */
class Vcf {
    public:
        /// Noise type
        enum class NoiseType {
            Wind    = 0,                    ///< Wind
            Patch   = 1,                    ///< Patch
            Flute   = 2,                    ///< Flute
            Invalid = 3                     ///y Invalid
        };

        /// Envelope mode
        enum class EnvelopeMode {
            T1      = 0,                    ///< T1
            T1T2    = 1,                    ///< T1->T2
            T1RT2   = 2,                    ///< T1->Release->T2
            Rotor   = 3,                    ///< Rotor
        };

        /**
          Create new VCF object from buffer.

          Creates a new VCF object with the given block number and associates the given buffer with it. During
          creation, the data from the buffer is parsed and copied to the object members. If an explicit update()
          is called, the buffer is written back with the updated VCF object data, for all other functions, it is
          left untouched.

          @param[in]    blockNum    Block number
          @param[in]    buffer      Raw data buffer
         */
        Vcf(uint8_t blockNum, void* buffer);

        /**
          Create VCF object by copying.

          Copies the VCF object by duplicating all it's members.

          @param[in]    source      Source object to copy from
         */
        Vcf(const Vcf& source);

        /**
          Destroy VCF object.

          Destroys the VCF object.
         */
        ~Vcf();

        /**
          Get raw buffer.

          Returns a const pointer to the raw buffer.

          @return                   Raw buffer const pointer
         */
        const void* getBuffer() const {
            return m_buffer;
        }

        /**
          Copy VCF object.

          Copies all members of the VCF object from the source object.

          @param[in]    source      Source object to copy from

          @return                   This object
         */
        Vcf& operator=(const Vcf& source);

        /**
          Copy VCF data.

          Copies all members except the block number and buffer from source object. This is intended to duplicate
          the VCF's contents to another existing VCF.

          @param[in]    source      Source object to copy from
         */
        void copy(const Vcf& source);

        /**
          Dissect VCF raw data buffer.

          Parses the raw VCF data buffer and updates object members.
         */
        void dissect();

        /**
          Update VCF raw data buffer.

          Writes back changes in the VCF object to the associated raw VCF data buffer.
         */
        void update();

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
          Get low pass enabled.

          Returns true if the low pass is enabled.

          @return                   True if low pass, false if band pass
         */
        bool getLowPass() const {
            return m_lowPass;
        }

        /**
          Get four poles enabled.

          Returns true if four poles are enabled.

          @return                   True if four poles, false if two poles
         */
        bool getFourPoles() const {
            return m_fourPoles;
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
          Get noise enabled.

          Returns true if noise is enabled.

          @return                   True if noise is enabled
         */
        bool getNoise() const {
            return m_noise;
        }

        /**
          Get distortion enabled.

          Returns true if distortion is enabled.

          @return                   True if distortion is enabled
         */
        bool getDistortion() const {
            return m_distortion;
        }

        /**
          Get frequency value.

          Returns the frequency value.

          @return                   Frequency value
         */
        int8_t getFrequency() const {
            return m_frequency;
        }

        /**
          Get quality value.

          Returns the quality value.

          @return                   Quality value
         */
        uint8_t getQuality() const {
            return m_quality;
        }

        /**
          Get noise type.

          Returns the noise type.

          @return                   Noise type
         */
        NoiseType getNoiseType() const {
            return m_noiseType;
        }

        /**
          Get retrigger enabled.

          Returns true if retrigger is enabled.

          @return                   True if retrigger is enabled
         */
        bool getRetrigger() const {
            return m_retrigger;
        }

        /**
          Get envelope mode.

          Returns the envelope mode.

          @return                   Envelope mode
         */
        EnvelopeMode getEnvelopeMode() const {
            return m_envMode;
        }

        /**
          Get tracking enabled.

          Returns true if tracking is enabled.

          @return                   True if tracking is enabled
         */
        bool getTracking() const {
            return m_tracking;
        }

        /**
          Get T1 time.

          Returns the T1 time.

          @return                   T1 time
         */
        uint8_t getT1Time() const {
            return m_t1Time;
        }

        /**
          Get T2 time.

          Returns the T2 time.

          @return                   T2 time
         */
        uint8_t getT2Time() const {
            return m_t2Time;
        }

        /**
          Get T1 intensity.

          Returns the T1 intensity.

          @return                   T1 intensity
         */
        int8_t getT1Intensity() const {
            return m_t1Intensity;
        }

        /**
          Get T1 offset.

          Returns the T1 offest.

          @return                   T1 offset
         */
        int8_t getT1Offset() const {
            return m_t1Offset;
        }

        /**
          Get T2 intensity.

          Returns the T2 intensity.

          @return                   T2 intensity
         */
        int8_t getT2Intensity() const {
            return m_t2Intensity;
        }

        /**
          Get T2 offset.

          Returns the T2 offest.

          @return                   T2 offset
         */
        int8_t getT2Offset() const {
            return m_t2Offset;
        }

        /**
          Get unknown bits.

          Returns all bits from raw data that are currently unknown.

          @return                   Unknown bits
         */
        uint8_t getUnknownBits() const {
            return m_unknownBits;
        }

        /**
          Get noise type name.

          Returns the name for the given noise type or an empty string if invalid.

          @param[in]    type        Noise type to look up

          @return                   Name for given noise type
         */
        static std::string getNoiseTypeName(NoiseType type);

        /**
          Get envelope mode name.

          Returns the name for the given envelope mode or an empty string if invalid.

          @param[in]    mode        Envelope mode to look up

          @return                   Name for given envelope mode
         */
        static std::string getEnvelopeModeName(EnvelopeMode mode);

    private:
        uint8_t         m_blockNum;         ///< Block number
        uint8_t*        m_buffer;           ///< Associated raw buffer

        bool            m_left;             ///< Left VCF output enabled
        bool            m_right;            ///< Right VCF output enabled
        bool            m_lowPass;          ///< VCF mode, false = BP, true = LP
        bool            m_fourPoles;        ///< True for 4-pole VCF, false for 2-pole
        bool            m_wv;               ///< WV VCF output enabled
        bool            m_noise;            ///< Noise off(false)/on(true)
        bool            m_distortion;       ///< Distortion off(false)/on(true)
        int8_t          m_frequency;        ///< Filter cutoff frequency
        uint8_t         m_quality;          ///< Filter quality (resonance)
        NoiseType       m_noiseType;        ///< Noise type WIND(0)/PATC(1)/FLUT(2)
        bool            m_retrigger;        ///< If true, retrigger filter envelopes on new note
        EnvelopeMode    m_envMode;          ///< VCF envelope mode T1(0)/T1->T2(1)/T1-Release-T2(2)/Rotor(3)
        bool            m_tracking;         ///< If true, filter frequency tracks played notes
        uint8_t         m_t1Time;           ///< T1 envelope time
        uint8_t         m_t2Time;           ///< T2 envelope time
        int8_t          m_t1Intensity;      ///< T1 envelope intensity
        int8_t          m_t1Offset;         ///< T1 envelope offset
        int8_t          m_t2Intensity;      ///< T2 envelope intensity
        int8_t          m_t2Offset;         ///< T2 envelope offset

        uint8_t         m_unknownBits;      ///< Currently unknown bits
};

} // namespace Wersi
} // namespace DMSToolbox
