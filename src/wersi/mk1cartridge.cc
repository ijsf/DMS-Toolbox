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

#include <wersi/mk1cartridge.hh>
#include <wersi/icb.hh>
#include <wersi/vcf.hh>
#include <wersi/envelope.hh>
#include <wersi/wave.hh>
#include <exceptions.hh>

using namespace std;

namespace DMSToolbox {
namespace Wersi {

// Create new MK1 cartridge object
Mk1Cartridge::Mk1Cartridge(void* buffer, bool /*initialize*/)
    : InstrumentStore()
    , m_buffer(static_cast<uint8_t*>(buffer))
{
    dissect();
}

// Destroy MK1 cartridge object
Mk1Cartridge::~Mk1Cartridge()
{
}

// Dissect raw MK1 cartridge data
void Mk1Cartridge::dissect()
{
    try {
        // Check header bytes
        uint16_t dummy = (m_buffer[0] << 8) | m_buffer[1];
        if (dummy != 0xffff) {
            throw DataFormatException("first two bytes need to be 0xff");
        }

        // Verify checksum
        uint16_t check = 0;
        for (size_t i = 0; i < 0x3ffe; ++i) {
            check += m_buffer[i];
        }
        dummy = (m_buffer[0x3ffe] << 8) | m_buffer[0x3fff];
        dummy += check;
        if (dummy != 0) {
            throw DataFormatException("checksum verification failed");
        }

        // Extract pointer table pointers
        uint16_t icbPtr = (m_buffer[2] << 8) | m_buffer[3];
        if (icbPtr >= 0x3ffe) {
            throw DataFormatException("invalid ICB table pointer");
        }
        uint16_t vcfPtr = (m_buffer[4] << 8) | m_buffer[5];
        if (vcfPtr >= 0x3ffe) {
            throw DataFormatException("invalid VCF table pointer");
        }
        uint16_t amplPtr = (m_buffer[6] << 8) | m_buffer[7];
        if (amplPtr >= 0x3ffe) {
            throw DataFormatException("invalid AMPL table pointer");
        }
        uint16_t freqPtr = (m_buffer[8] << 8) | m_buffer[9];
        if (freqPtr >= 0x3ffe) {
            throw DataFormatException("invalid FREQ table pointer");
        }
        uint16_t wavePtr = (m_buffer[10] << 8) | m_buffer[11];
        if (wavePtr >= 0x3ffe) {
            throw DataFormatException("invalid WAVE table pointer");
        }

        // Initialize extraction
        size_t current = 129; // ICBs start counting at 1, bit 7 is for cartridge
        size_t maxIcb = current + 19; // Dynamic number of ICBs, init with 20 instruments that are always there
        size_t maxVcf = 0;
        size_t maxAmpl = 0;
        size_t maxFreq = 0;
        size_t maxWave = 0;

        // Extract ICBs
        while (current <= maxIcb) {
            uint16_t idx = (current - 129) * 2 + icbPtr;
            idx = (m_buffer[idx] << 8) | m_buffer[idx + 1];
            if (idx >= 0x3ffe) {
                throw DataFormatException("invalid ICB pointer");
            }
            Icb icb(current, &(m_buffer[idx]));
            m_icb.insert(pair<uint8_t, Icb>(current, icb));
            uint8_t tmp = icb.getNextIcb();
            if (tmp > maxIcb) {
                maxIcb = tmp;
            }
            tmp = icb.getVcf();
            if (tmp > maxVcf) {
                maxVcf = tmp;
            }
            tmp = icb.getAmpl();
            if (tmp > maxAmpl) {
                maxAmpl = tmp;
            }
            tmp = icb.getFreq();
            if (tmp > maxFreq) {
                maxFreq = tmp;
            }
            tmp = icb.getWave();
            if (tmp > maxWave) {
                maxWave = tmp;
            }
            ++current;
        }

        // Extract VCFs
        current = 128;
        while (current < maxVcf) {
            uint16_t idx = (current - 128) * 2 + vcfPtr;
            idx = (m_buffer[idx] << 8) | m_buffer[idx + 1];
            if (idx >= 0x3ffe) {
                throw DataFormatException("invalid VCF pointer");
            }
            Vcf vcf(current, &(m_buffer[idx]));
            m_vcf.insert(pair<uint8_t, Vcf>(current, vcf));
            ++current;
        }

        // Extract AMPLs
        current = 128;
        while (current < maxAmpl) {
            uint16_t idx = (current - 128) * 2 + amplPtr;
            idx = (m_buffer[idx] << 8) | m_buffer[idx + 1];
            if (idx >= 0x3ffe) {
                throw DataFormatException("invalid AMPL pointer");
            }
            Envelope ampl(current, &(m_buffer[idx]), 44);
            m_ampl.insert(pair<uint8_t, Envelope>(current, ampl));
            ++current;
        }

        // Extract FREQs
        current = 128;
        while (current < maxFreq) {
            uint16_t idx = (current - 128) * 2 + freqPtr;
            idx = (m_buffer[idx] << 8) | m_buffer[idx + 1];
            if (idx >= 0x3ffe) {
                throw DataFormatException("invalid FREQ pointer");
            }
            Envelope freq(current, &(m_buffer[idx]), 32);
            m_freq.insert(pair<uint8_t, Envelope>(current, freq));
            ++current;
        }

        // Extract WAVEs
        current = 128;
        while (current < maxWave) {
            uint16_t idx = (current - 128) * 2 + wavePtr;
            idx = (m_buffer[idx] << 8) | m_buffer[idx + 1];
            if (idx >= 0x3ffe) {
                throw DataFormatException("invalid WAVE pointer");
            }
            Wave wave(current, &(m_buffer[idx]), (m_buffer[idx] & 0x80) == 0 ? 177 : 212);
            m_wave.insert(pair<uint8_t, Wave>(current, wave));
            ++current;
        }
    }
    catch (DataFormatException& e) {
        DataFormatException exc("Invalid MK1 cartridge, ");
        exc << e.what();
        throw exc;
    }
}

// Put together and update MK1 cartridge raw data
void Mk1Cartridge::update()
{
}

} // namespace Wersi
} // namespace DMSToolbox
