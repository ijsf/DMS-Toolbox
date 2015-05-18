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

#include <wersi/icb.hh>

namespace DMSToolbox {
namespace Wersi {

// Create new ICB object
Icb::Icb(uint8_t blockNum, void* buffer)
    : m_blockNum(blockNum)
    , m_buffer(static_cast<uint8_t*>(buffer))
    , m_nextIcb(0)
    , m_vcfBlock(0)
    , m_amplBlock(0)
    , m_freqBlock(0)
    , m_waveBlock(0)
    , m_left(false)
    , m_right(false)
    , m_bright(false)
    , m_vcf(false)
    , m_wv(false)
    , m_transpose(0)
    , m_detune(0)
    , m_wvMode(WvMode::Invalid)
    , m_wvLeft(false)
    , m_wvRight(false)
    , m_wvFbFlat(false)
    , m_wvFbDeep(false)
    , m_name()
    , m_unknownBits()
{
    dissect();
}

// Create ICB object by copying
Icb::Icb(const Icb& source)
    : m_blockNum(0)
    , m_buffer(nullptr)
    , m_nextIcb(0)
    , m_vcfBlock(0)
    , m_amplBlock(0)
    , m_freqBlock(0)
    , m_waveBlock(0)
    , m_left(false)
    , m_right(false)
    , m_bright(false)
    , m_vcf(false)
    , m_wv(false)
    , m_transpose(0)
    , m_detune(0)
    , m_wvMode(WvMode::Invalid)
    , m_wvLeft(false)
    , m_wvRight(false)
    , m_wvFbFlat(false)
    , m_wvFbDeep(false)
    , m_name()
    , m_unknownBits()
{
    *this = source;
}

// Destroy ICB object
Icb::~Icb()
{
}

// Copy ICB object members
Icb& Icb::operator=(const Icb& source)
{
    if (this != &source) {
        m_blockNum  = source.m_blockNum;
        m_buffer    = source.m_buffer;

        m_nextIcb   = source.m_nextIcb;
        m_vcfBlock  = source.m_vcfBlock;
        m_amplBlock = source.m_amplBlock;
        m_freqBlock = source.m_freqBlock;
        m_waveBlock = source.m_waveBlock;
        m_left      = source.m_left;
        m_right     = source.m_right;
        m_bright    = source.m_bright;
        m_vcf       = source.m_vcf;
        m_wv        = source.m_wv;
        m_transpose = source.m_transpose;
        m_detune    = source.m_detune;
        m_wvMode    = source.m_wvMode;
        m_wvLeft    = source.m_wvLeft;
        m_wvRight   = source.m_wvRight;
        m_wvFbFlat  = source.m_wvFbFlat;
        m_wvFbDeep  = source.m_wvFbDeep;
        m_name      = source.m_name;

        m_unknownBits = source.m_unknownBits;
    }
    return *this;
}

// Disssect ICB raw data
void Icb::dissect()
{
    m_nextIcb   = m_buffer[0];
    m_vcfBlock  = m_buffer[1];
    m_amplBlock = m_buffer[2];
    m_freqBlock = m_buffer[3];
    m_waveBlock = m_buffer[4];
    // Byte 5 unknown, bit 7 = fixed pitch?
    m_left      = (m_buffer[6] & 0x01) != 0;
    m_right     = (m_buffer[6] & 0x02) != 0;
    m_bright    = (m_buffer[6] & 0x04) != 0;
    m_vcf       = (m_buffer[6] & 0x08) != 0;
    m_wv        = (m_buffer[6] & 0x10) != 0;
    // Unknown bits 5-7
    m_transpose = int8_t(m_buffer[7]);
    m_detune    = int8_t(m_buffer[8]);
    m_wvMode    = static_cast<WvMode>(m_buffer[9] & 7);
    m_wvLeft    = (m_buffer[9] & 0x08) != 0;
    m_wvRight   = (m_buffer[9] & 0x10) != 0;
    // Unknown bit 5
    m_wvFbFlat  = (m_buffer[9] & 0x40) != 0;
    m_wvFbDeep  = (m_buffer[9] & 0x80) != 0;
    m_name      = std::string(reinterpret_cast<char*>(&(m_buffer[10])), 6);

    // Unknown bits
    m_unknownBits = m_buffer[5] | ((m_buffer[6] & 0xe0) << 3) | ((m_buffer[9] & 0x20) << 7);
}

// Put together and update ICB raw data
void Icb::update()
{
}

// Return WersiVoice mode name
std::string Icb::getWvModeName(WvMode mode)
{
    switch (mode) {
        case WvMode::RotorSlow:
            return std::string("Rotor slow");
            break;
        case WvMode::RotorFast:
            return std::string("Rotor fast");
            break;
        case WvMode::Flanger:
            return std::string("Flanger");
            break;
        case WvMode::Strings:
            return std::string("Strings");
            break;
        case WvMode::Chorus:
            return std::string("Chorus");
            break;
        default:
            return std::string();
            break;
    }
}

} // namespace Wersi
} // namespace DMSToolbox
