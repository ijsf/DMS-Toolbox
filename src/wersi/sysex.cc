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

#include <wersi/sysex.hh>
#include <wersi/icb.hh>
#include <wersi/vcf.hh>
#include <wersi/envelope.hh>
#include <wersi/wave.hh>
#include <wersi/instrumentstore.hh>
#include <exceptions.hh>
#include <cstring>

namespace DMSToolbox {
namespace Wersi {

// Convert byte to two SysEx bytes
inline void byteToSysEx(uint8_t type, uint8_t byte, uint8_t& lo, uint8_t& hi)
{
    lo = (type << 5) | 0x10 | (byte & 0x0f);
    hi = (type << 5) | ((byte >> 4) & 0x0f);
}

// Convert two SysEx bytes to raw byte
inline uint8_t byteFromSysEx(uint8_t type, uint8_t lo, uint8_t hi)
{
    if ((lo & 0xf0) != ((type << 5) | 0x10) || (hi & 0xf0) != (type << 5)) {
        throw MidiException("Invalid Wersi SysEx data");
    }
    return (lo & 0x0f) | ((hi & 0x0f) << 4);
}

// Create SysEx message from raw message data
size_t SysEx::toSysEx(uint8_t device, const Message& message, SysExMessage& sysEx)
{
    sysEx.m_start = 0xf0;
    sysEx.m_vendor = 0x25;
    sysEx.m_device = device;
    byteToSysEx(3, static_cast<uint8_t>(message.m_type), sysEx.m_typeLo, sysEx.m_typeHi);
    byteToSysEx(2, message.m_address, sysEx.m_addressLo, sysEx.m_addressHi);
    byteToSysEx(1, message.m_length, sysEx.m_lengthLo, sysEx.m_lengthHi);
    const uint8_t* s = message.m_data;
    uint8_t* d = sysEx.m_data;
    for (size_t i = 0; i < message.m_length; ++i) {
        byteToSysEx(0, *s, d[0], d[1]);
        ++s;
        d += 2;
    }
    *d = 0xf7;

    return sizeof(SysExMessage) + (2 * message.m_length);
}

// Convert SysEx message to raw message data
void SysEx::fromSysEx(uint8_t device, const SysExMessage& sysEx, Message& message)
{
    if (sysEx.m_start != 0xf0 || (sysEx.m_vendor != 0x25 && sysEx.m_vendor != 0x3b) || sysEx.m_device != device) {
        throw MidiException("Invalid Wersi SysEx message");
    }
    message.m_type = static_cast<BlockType>(byteFromSysEx(3, sysEx.m_typeLo, sysEx.m_typeHi));
    message.m_address = byteFromSysEx(2, sysEx.m_addressLo, sysEx.m_addressHi);
    message.m_length = byteFromSysEx(1, sysEx.m_lengthLo, sysEx.m_lengthHi);
    const uint8_t* s = sysEx.m_data;
    uint8_t* d = message.m_data;
    for (size_t i = 0; i < message.m_length; ++i) {
        *d = byteFromSysEx(0, s[0], s[1]);
        s += 2;
        ++d;
    }
}

#ifdef HAVE_RTMIDI
// Send ICB to device
void SysEx::sendIcb(RtMidiOut* midi, uint8_t type, uint8_t blockNum, const Icb& icb)
{
    // Construct raw message
    uint8_t buf[sizeof(Message) + 15];
    auto msg = reinterpret_cast<Message*>(buf);
    msg->m_type = BlockType::IcBlock;
    msg->m_address = blockNum;
    msg->m_length = 16;
    memcpy(msg->m_data, icb.getBuffer(), 16);

    // Convert to SysEx message
    uint8_t out[sizeof(SysExMessage) + 32];
    auto omsg = reinterpret_cast<SysExMessage*>(out);
    size_t length = toSysEx(type, *msg, *omsg);

    // Create MIDI message vector and send it
    std::vector<unsigned char> data;
    for (size_t i = 0; i < length; ++i) {
        data.push_back(out[i]);
    }
    midi->sendMessage(&data);
}

// Send VCF to device
void SysEx::sendVcf(RtMidiOut* midi, uint8_t type, uint8_t blockNum, const Vcf& vcf)
{
    // Construct raw message
    uint8_t buf[sizeof(Message) + 9];
    auto msg = reinterpret_cast<Message*>(buf);
    msg->m_type = BlockType::VcfBlock;
    msg->m_address = blockNum;
    msg->m_length = 10;
    memcpy(msg->m_data, vcf.getBuffer(), 10);

    // Convert to SysEx message
    uint8_t out[sizeof(SysExMessage) + 20];
    auto omsg = reinterpret_cast<SysExMessage*>(out);
    size_t length = toSysEx(type, *msg, *omsg);

    // Create MIDI message vector and send it
    std::vector<unsigned char> data;
    for (size_t i = 0; i < length; ++i) {
        data.push_back(out[i]);
    }
    midi->sendMessage(&data);
}

// Send AMPL to device
void SysEx::sendAmpl(RtMidiOut* midi, uint8_t type, uint8_t blockNum, const Envelope& ampl)
{
    // Construct raw message
    uint8_t buf[sizeof(Message) + 43];
    auto msg = reinterpret_cast<Message*>(buf);
    msg->m_type = BlockType::AmplBlock;
    msg->m_address = blockNum;
    msg->m_length = ampl.getBufferSize();
    memcpy(msg->m_data, ampl.getBuffer(), msg->m_length);

    // Convert to SysEx message
    uint8_t out[sizeof(SysExMessage) + 88];
    auto omsg = reinterpret_cast<SysExMessage*>(out);
    size_t length = toSysEx(type, *msg, *omsg);

    // Create MIDI message vector and send it
    std::vector<unsigned char> data;
    for (size_t i = 0; i < length; ++i) {
        data.push_back(out[i]);
    }
    midi->sendMessage(&data);
}

// Send FREQ to device
void SysEx::sendFreq(RtMidiOut* midi, uint8_t type, uint8_t blockNum, const Envelope& freq)
{
    // Construct raw message
    uint8_t buf[sizeof(Message) + 31];
    auto msg = reinterpret_cast<Message*>(buf);
    msg->m_type = BlockType::FreqBlock;
    msg->m_address = blockNum;
    msg->m_length = freq.getBufferSize();
    memcpy(msg->m_data, freq.getBuffer(), msg->m_length);

    // Convert to SysEx message
    uint8_t out[sizeof(SysExMessage) + 64];
    auto omsg = reinterpret_cast<SysExMessage*>(out);
    size_t length = toSysEx(type, *msg, *omsg);

    // Create MIDI message vector and send it
    std::vector<unsigned char> data;
    for (size_t i = 0; i < length; ++i) {
        data.push_back(out[i]);
    }
    midi->sendMessage(&data);
}

// Send WAVE to device
void SysEx::sendWave(RtMidiOut* midi, uint8_t type, uint8_t blockNum, const Wave& wave)
{
    // Construct raw message
    uint8_t buf[sizeof(Message) + 211];
    auto msg = reinterpret_cast<Message*>(buf);
    msg->m_type = BlockType::FixWaveBlock;
    msg->m_address = blockNum;
    msg->m_length = wave.getBufferSize();
    if (msg->m_length < 212) {
        msg->m_type = BlockType::RelWaveBlock;
    }
    memcpy(msg->m_data, wave.getBuffer(), msg->m_length);

    // Convert to SysEx message
    uint8_t out[sizeof(SysExMessage) + 424];
    auto omsg = reinterpret_cast<SysExMessage*>(out);
    size_t length = toSysEx(type, *msg, *omsg);

    // Create MIDI message vector and send it
    std::vector<unsigned char> data;
    for (size_t i = 0; i < length; ++i) {
        data.push_back(out[i]);
    }
    midi->sendMessage(&data);
}

// MIDI receive callback
void SysEx::rtMidiCallback(double /*timestamp*/, std::vector<unsigned char>* message, void* userData)
{
    if (userData != nullptr && message->size() >= 8 &&
            message->at(0) == 0xf0 && (message->at(1) == 0x25 || message->at(1) == 0x3b)) {
        auto is = static_cast<InstrumentStore*>(userData);
        is->receivedSysEx(message);
    }
}
#endif // HAVE_RTMIDI

} // namespace Wersi
} // namespace DMSToolbox
