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

#include <wersi/dx10cartridge.hh>
#include <wersi/icb.hh>
#include <wersi/vcf.hh>
#include <wersi/envelope.hh>
#include <wersi/wave.hh>
#include <exceptions.hh>

namespace DMSToolbox {
namespace Wersi {

// Create new DX10/DX5 cartridge object
Dx10Cartridge::Dx10Cartridge(void* buffer, size_t size, bool /*initialize*/)
    : InstrumentStore()
    , m_buffer(static_cast<uint8_t*>(buffer))
    , m_size(size)
{
    dissect();
}

// Destroy DX10/DX5 cartridge object
Dx10Cartridge::~Dx10Cartridge()
{
}

// Dissect raw DX10/DX5 cartridge data
void Dx10Cartridge::dissect()
{
    try {
        // Check size
        if (m_size != 8192 && m_size != 16384) {
            throw DataFormatException("invalid raw data size");
        }

        // Verify presets/instruments checksum
        uint16_t check = 0x3131;
        for (size_t i = 0; i < 0x0f64; ++i) {
            check += m_buffer[i];
        }
        uint16_t dummy = (m_buffer[0x0f64] << 8) | m_buffer[0x0f65];
        dummy += check;
        if (dummy != 0) {
            throw DataFormatException("checksum verification for presets/instruments failed");
        }

        // Verify rhythms/sequences checksum
        if (m_size > 8192) {
            check = 0;
            for (size_t i = 0x2000; i < 0x3ffe; ++i) {
                check += m_buffer[i];
            }
            dummy = (m_buffer[0x3ffe] << 8) | m_buffer[0x3fff];
            dummy += check;
            if (dummy != 0) {
                throw DataFormatException("checksum verification for rhythms/sequences failed");
            }
        }

        // Skip presets
        size_t idx = 0;
        for (size_t i = 0; i < 8; ++i) {
            idx += 250;
        }

        // Extract ICBs
        for (size_t i = 0; i < 20; ++i) {
            uint8_t addr = i + 194;
            if (i >= 10) {
                ++addr;
            }
            Icb icb(addr, &(m_buffer[idx]));
            m_icb.insert(std::pair<uint8_t, Icb>(addr, icb));
            idx += 16;
        }

        // Extract VCFs
        for (size_t i = 0; i < 10; ++i) {
            uint8_t addr = i + 193;
            Vcf vcf(addr, &(m_buffer[idx]));
            m_vcf.insert(std::pair<uint8_t, Vcf>(addr, vcf));
            idx += 10;
        }

        // Extract AMPLs
        for (size_t i = 0; i < 20; ++i) {
            uint8_t addr = i + 193;
            if (i >= 10) {
                ++addr;
            }
            Envelope ampl(addr, &(m_buffer[idx]), 44);
            m_ampl.insert(std::pair<uint8_t, Envelope>(addr, ampl));
            idx += 44;
        }

        // Extract FREQs
        for (size_t i = 0; i < 20; ++i) {
            uint8_t addr = i + 193;
            if (i >= 10) {
                ++addr;
            }
            Envelope freq(addr, &(m_buffer[idx]), 32);
            m_freq.insert(std::pair<uint8_t, Envelope>(addr, freq));
            idx += 32;
        }

        // Check pointer correctness and skip checksum
        if (idx != 0x0f64) {
            throw DataFormatException("something went wrong extracting ICB, VCF, AMPL and FREQ");
        }
        idx += 2;

        // Extract WAVEs
        for (size_t i = 0; i < 20; ++i) {
            uint8_t addr = i + 193;
            if (i >= 10) {
                ++addr;
            }
            Wave wave(addr, &(m_buffer[idx]), 212);
            m_wave.insert(std::pair<uint8_t, Wave>(addr, wave));
            idx += 212;
        }

        // Check pointer correctness
        if (idx != 0x1ff6) {
            throw DataFormatException("something went wrong extracting WAVE");
        }
    }
    catch (DataFormatException& e) {
        DataFormatException exc("Invalid DX10/DX5 cartridge, ");
        exc << e.what();
        throw e;
    }
}

// Put together and update DX10/DX5 cartridge raw data
void Dx10Cartridge::update()
{
}

} // namespace Wersi
} // namespace DMSToolbox
