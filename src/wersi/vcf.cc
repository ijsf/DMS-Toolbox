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

#include <wersi/vcf.hh>

namespace DMSToolbox {
namespace Wersi {

// Create new VCF object
Vcf::Vcf(uint8_t blockNum, void* buffer)
    : m_blockNum(blockNum)
    , m_buffer(static_cast<uint8_t*>(buffer))
    , m_left(false)
    , m_right(false)
    , m_lowPass(false)
    , m_fourPoles(false)
    , m_wv(false)
    , m_noise(false)
    , m_distortion(false)
    , m_frequency(0)
    , m_quality(0)
    , m_noiseType(NoiseType::Invalid)
    , m_retrigger(false)
    , m_envMode(EnvelopeMode::Rotor)
    , m_tracking(false)
    , m_t1Time(0)
    , m_t2Time(0)
    , m_t1Intensity(0)
    , m_t1Offset(0)
    , m_t2Intensity(0)
    , m_t2Offset(0)
    , m_unknownBits(0)
{
    dissect();
}

// Create VCF object by copying
Vcf::Vcf(const Vcf& source)
    : m_blockNum(0)
    , m_buffer(nullptr)
    , m_left(false)
    , m_right(false)
    , m_lowPass(false)
    , m_fourPoles(false)
    , m_wv(false)
    , m_noise(false)
    , m_distortion(false)
    , m_frequency(0)
    , m_quality(0)
    , m_noiseType(NoiseType::Invalid)
    , m_retrigger(false)
    , m_envMode(EnvelopeMode::Rotor)
    , m_tracking(false)
    , m_t1Time(0)
    , m_t2Time(0)
    , m_t1Intensity(0)
    , m_t1Offset(0)
    , m_t2Intensity(0)
    , m_t2Offset(0)
    , m_unknownBits(0)
{
    *this = source;
}

// Destroy VCF object
Vcf::~Vcf()
{
}

// Copy VCF object members
Vcf& Vcf::operator=(const Vcf& source)
{
    if (this != &source) {
        m_blockNum      = source.m_blockNum;
        m_buffer        = source.m_buffer;

        m_left          = source.m_left;
        m_right         = source.m_right;
        m_lowPass       = source.m_lowPass;
        m_fourPoles     = source.m_fourPoles;
        m_wv            = source.m_wv;
        m_noise         = source.m_noise;
        m_distortion    = source.m_distortion;
        m_frequency     = source.m_frequency;
        m_quality       = source.m_quality;
        m_noiseType     = source.m_noiseType;
        m_retrigger     = source.m_retrigger;
        m_envMode       = source.m_envMode;
        m_tracking      = source.m_tracking;
        m_t1Time        = source.m_t1Time;
        m_t2Time        = source.m_t2Time;
        m_t1Intensity   = source.m_t1Intensity;
        m_t1Offset      = source.m_t1Offset;
        m_t2Intensity   = source.m_t2Intensity;
        m_t2Offset      = source.m_t2Offset;

        m_unknownBits   = source.m_unknownBits;
    }
    return *this;
}

// Dissect VCF raw data
void Vcf::dissect()
{
    m_left          = (m_buffer[0] & 0x01) != 0;
    m_right         = (m_buffer[0] & 0x02) != 0;
    m_lowPass       = (m_buffer[0] & 0x04) != 0;
    m_fourPoles     = (m_buffer[0] & 0x08) != 0;
    m_wv            = (m_buffer[0] & 0x10) != 0;
    m_noise         = (m_buffer[0] & 0x20) != 0;
    m_distortion    = (m_buffer[0] & 0x40) != 0;
    // Bit 7 unknown
    m_frequency     = int8_t(m_buffer[1]);
    m_quality       = m_buffer[2];
    // Bits 0-1 unknown
    m_noiseType     = static_cast<NoiseType>((m_buffer[3] & 0x0c) >> 2);
    m_retrigger     = (m_buffer[3] & 0x10) != 0;
    m_envMode       = static_cast<EnvelopeMode>((m_buffer[3] & 0x60) >> 5);
    m_tracking      = (m_buffer[3] & 0x80) != 0;
    m_t1Time        = m_buffer[4];
    m_t2Time        = m_buffer[5];
    m_t1Intensity   = int8_t(m_buffer[6]);
    m_t1Offset      = int8_t(m_buffer[7]);
    m_t2Intensity   = int8_t(m_buffer[8]);
    m_t2Offset      = int8_t(m_buffer[9]);

    m_unknownBits   = (m_buffer[3] & 0x03) | (m_buffer[0] & 0x80);
}

// Put together and update VCF raw data
void Vcf::update()
{
}

// Return noise type name
std::string Vcf::getNoiseTypeName(NoiseType type)
{
    switch (type) {
        case NoiseType::Wind:
            return std::string("Wind");
            break;
        case NoiseType::Patch:
            return std::string("Patch");
            break;
        case NoiseType::Flute:
            return std::string("Flute");
            break;
        default:
            return std::string();
            break;
    }
}

// Return envelope mode name
std::string Vcf::getEnvelopeModeName(EnvelopeMode mode)
{
    switch (mode) {
        case EnvelopeMode::T1:
            return std::string("T1 only");
            break;
        case EnvelopeMode::T1T2:
            return std::string("T1->T2");
            break;
        case EnvelopeMode::T1RT2:
            return std::string("T1->Release->T2");
            break;
        case EnvelopeMode::Rotor:
            return std::string("Rotor");
            break;
        default:
            return std::string();
            break;
    }
}

} // namespace Wersi
} // namespace DMSToolbox
