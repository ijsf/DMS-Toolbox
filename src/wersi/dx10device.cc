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
    , m_awaitType(0)
    , m_awaitAddress(0)
    , m_awaitLength(0)
    , m_awaitDestination(nullptr)
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
void Dx10Device::readBlock(RtMidiOut* outPort, SysEx::BlockType type, uint8_t address, uint8_t* ptr, uint8_t length)
{
    // Generate request message
    SysEx::Message msg;
    msg.m_type = SysEx::BlockType::RequestBlock;
    msg.m_address = address;
    msg.m_length = 1;
    msg.m_data[0] = static_cast<uint8_t>(type);
    unsigned char buf[sizeof(SysEx::SysExMessage) + 2];
    auto sem = reinterpret_cast<SysEx::SysExMessage*>(buf);
    size_t len = SysEx::toSysEx(1, msg, *sem);

    // Set awaiting information
    m_awaitType = msg.m_data[0];
    m_awaitAddress = address;
    m_awaitLength = length;
    m_awaitDestination = ptr;
    
    printf("INFO: Awaiting message, (type %u, addr %u, len %u)\n",
        (uint8_t)m_awaitType,
        m_awaitAddress,
        m_awaitLength
    );

    // Send request message
    std::vector<unsigned char> midi;
    for (size_t i = 0; i < len; ++i) {
        midi.push_back(buf[i]);
    }

    // Wait for response
    size_t retry = 0;
    while (retry < 200) {
        usleep(50000);
        if (m_awaitDestination == nullptr) {
            break;
        } else {
            // Resend
            printf("WARNING: Resending\n");
            outPort->sendMessage(&midi);
        }
    }
    if (m_awaitDestination != nullptr) {
        m_awaitDestination = nullptr;
        throw MidiException("Did not receive expected data from device");
    }
}

// SysEx message callback
void Dx10Device::receivedSysEx(std::vector<unsigned char>* message)
{
    // Check if we are awaiting a message
    if (m_awaitDestination == nullptr) {
        return;
    }

    // If message is not too large, take it apart
    if (message->size() <= 512) {
        auto buf = new unsigned char[512];
        uint8_t* p = buf;
        for (auto& i : *message) {
            *p = i;
            ++p;
        }
        try {
            auto sem = reinterpret_cast<SysEx::SysExMessage*>(buf);
            auto out = reinterpret_cast<SysEx::Message*>(buf);
            SysEx::fromSysEx(1, *sem, *out);

            // If this is what we expect, use it
            if (static_cast<uint8_t>(out->m_type) == m_awaitType &&
                    out->m_address == m_awaitAddress && out->m_length == m_awaitLength) {
                printf("INFO: Got message\n");
                memcpy(m_awaitDestination, out->m_data, m_awaitLength);
                m_awaitDestination = nullptr;
            } else {
                printf("WARNING: Invalid message, got (type %u, addr %u, len %u) expected (type %u, addr %u, len %u)\n",
                  (uint8_t)out->m_type,
                  out->m_address,
                  out->m_length,
                  (uint8_t)m_awaitType,
                  m_awaitAddress,
                  m_awaitLength
                );
            }
            delete[] buf;
        }
        catch (...) {
            delete[] buf;
            throw;
        }
    }
}

// Read instrument store contents from device
void Dx10Device::readFromDevice(RtMidiIn* /*inPort*/, RtMidiOut* outPort,
                                bool(*callback)(void* object, uint32_t current, uint32_t max), void* object)
{
    uint8_t* ptr = m_buffer;
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 66;
        if (i >= 10) {
            ++addr;
        }
        if (callback != nullptr) {
            callback(object, ptr - m_buffer, m_size);
        }
        readBlock(outPort, SysEx::BlockType::IcBlock, addr, ptr, 16);
        ptr += 16;
    }
    for (size_t i = 0; i < 10; ++i) {
        uint8_t addr = i + 65;
        if (callback != nullptr) {
            callback(object, ptr - m_buffer, m_size);
        }
        readBlock(outPort, SysEx::BlockType::VcfBlock, addr, ptr, 10);
        ptr += 10;
    }
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        if (callback != nullptr) {
            callback(object, ptr - m_buffer, m_size);
        }
        readBlock(outPort, SysEx::BlockType::AmplBlock, addr, ptr, 44);
        ptr += 44;
    }
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        if (callback != nullptr) {
            callback(object, ptr - m_buffer, m_size);
        }
        readBlock(outPort, SysEx::BlockType::FreqBlock, addr, ptr, 32);
        ptr += 32;
    }
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        if (callback != nullptr) {
            callback(object, ptr - m_buffer, m_size);
        }
        readBlock(outPort, SysEx::BlockType::FixWaveBlock, addr, ptr, 212);
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
