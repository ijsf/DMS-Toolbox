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

#include <wersi/dx10device.hh>
#include <wersi/icb.hh>
#include <wersi/vcf.hh>
#include <wersi/envelope.hh>
#include <wersi/wave.hh>
#include <wersi/sysex.hh>
#include <exceptions.hh>
#include <cstring>
#include <unistd.h>

#ifdef HAVE_RTMIDI
#include <RtMidi.h>
#endif // HAVE_RTMIDI

namespace DMSToolbox {
namespace Wersi {

// Create new DX10/EX10R device object
Dx10Device::Dx10Device(void* buffer, size_t size)
    : InstrumentStore(buffer, size)
{
    // Initialize ICBs
    memset(buffer, 0, size);
    auto ptr = static_cast<uint8_t*>(buffer);
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        ptr[1] = i + 65;    // VCF
        ptr[2] = addr;      // AMPL
        ptr[3] = addr;      // FREQ
        ptr[4] = addr;      // WAVE
        ptr[10] = ' ';
        ptr[11] = ' ';
        ptr[12] = ' ';
        ptr[13] = ' ';
        ptr[14] = ' ';
        ptr[15] = ' ';
        ptr += 16;
    }

    dissect();
}

// Destroy DX10/DX5 cartridge object
Dx10Device::~Dx10Device()
{
}

#ifdef HAVE_RTMIDI
void readDevice(RtMidiIn* inPort, RtMidiOut* outPort,
                SysEx::BlockType type, uint8_t address, uint8_t* ptr, uint8_t length)
{
    // Generate request message
    SysEx::Message msg;
    msg.m_type = SysEx::BlockType::RequestBlock;
    msg.m_address = address;
    msg.m_length = 1;
    msg.m_data[0] = static_cast<uint8_t>(type);

    // Send request message
    std::vector<unsigned char> midi;
    midi.clear();
    auto buf = new unsigned char[1024];
    auto sem = reinterpret_cast<SysEx::SysExMessage*>(buf);
    try {
        size_t len = SysEx::toSysEx(2, msg, *sem);
        for (size_t i = 0; i < len; ++i) {
            midi.push_back(buf[i]);
        }
        outPort->sendMessage(&midi);

        // Try to receive response
        size_t retry = 0;
        bool done = false;
        while (retry < 20) {
            midi.clear();
            inPort->getMessage(&midi);
            if (midi.empty()) {
                usleep(100000);
            }
            else {
                if (midi[0] == 0xf0 && midi.size() < 1024) {
                    uint8_t* p = buf;
                    for (auto& i : midi) {
                        *p = i;
                        ++p;
                    }
                    auto out = reinterpret_cast<SysEx::Message*>(buf);
                    SysEx::fromSysEx(2, *sem, *out);
                    if (out->m_type == type && out->m_length == length) {
                        memcpy(ptr, out->m_data, length);
                        done = true;
                        break;
                    }
                }
                retry = 0;
            }
            ++retry;
        }
        if (!done) {
            throw MidiException("Did not receive expected data from device");
        }
    }
    catch (...) {
        delete[] buf;
        throw;
    }
    delete[] buf;
}

// Read instrument store contents from device
void Dx10Device::readFromDevice(RtMidiIn* inPort, RtMidiOut* outPort)
{
    uint8_t* ptr = m_buffer;
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 66;
        if (i >= 10) {
            ++addr;
        }
        readDevice(inPort, outPort, SysEx::BlockType::IcBlock, addr, ptr, 16);
        ptr += 16;
    }
    for (size_t i = 0; i < 10; ++i) {
        uint8_t addr = i + 65;
        readDevice(inPort, outPort, SysEx::BlockType::VcfBlock, addr, ptr, 10);
        ptr += 10;
    }
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        readDevice(inPort, outPort, SysEx::BlockType::AmplBlock, addr, ptr, 44);
        ptr += 44;
    }
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        readDevice(inPort, outPort, SysEx::BlockType::FreqBlock, addr, ptr, 32);
        ptr += 32;
    }
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        readDevice(inPort, outPort, SysEx::BlockType::FixWaveBlock, addr, ptr, 212);
        ptr += 212;
    }
    dissect();
}

#endif // HAVE_RTMIDI
// Dissect raw DX10/DX5 cartridge data
void Dx10Device::dissect()
{
    clearLists();

    try {
        // Extract ICBs
        size_t idx = 0;
        for (size_t i = 0; i < 20; ++i) {
            uint8_t addr = i + 66;
            if (i >= 10) {
                ++addr;
            }
            Icb icb(addr, &(m_buffer[idx]));
            m_icb.insert(std::pair<uint8_t, Icb>(addr, icb));
            idx += 16;
        }

        // Extract VCFs
        for (size_t i = 0; i < 10; ++i) {
            uint8_t addr = i + 65;
            Vcf vcf(addr, &(m_buffer[idx]));
            m_vcf.insert(std::pair<uint8_t, Vcf>(addr, vcf));
            idx += 10;
        }

        // Extract AMPLs
        for (size_t i = 0; i < 20; ++i) {
            uint8_t addr = i + 65;
            if (i >= 10) {
                ++addr;
            }
            Envelope ampl(addr, &(m_buffer[idx]), 44);
            m_ampl.insert(std::pair<uint8_t, Envelope>(addr, ampl));
            idx += 44;
        }

        // Extract FREQs
        for (size_t i = 0; i < 20; ++i) {
            uint8_t addr = i + 65;
            if (i >= 10) {
                ++addr;
            }
            Envelope freq(addr, &(m_buffer[idx]), 32);
            m_freq.insert(std::pair<uint8_t, Envelope>(addr, freq));
            idx += 32;
        }

        // Extract WAVEs
        for (size_t i = 0; i < 20; ++i) {
            uint8_t addr = i + 65;
            if (i >= 10) {
                ++addr;
            }
            Wave wave(addr, &(m_buffer[idx]), 212);
            m_wave.insert(std::pair<uint8_t, Wave>(addr, wave));
            idx += 212;
        }
    }
    catch (DataFormatException& e) {
        DataFormatException exc("Invalid DX10/EX10R buffer, ");
        exc << e.what();
        throw e;
    }
}

// Put together and update DX10/DX5 cartridge raw data
void Dx10Device::update()
{
}

} // namespace Wersi
} // namespace DMSToolbox
