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

#include <wersi/instrumentstore.hh>
#include <wersi/icb.hh>
#include <wersi/vcf.hh>
#include <wersi/envelope.hh>
#include <wersi/wave.hh>
#include <exceptions.hh>

#ifdef HAVE_RTMIDI
#include <RtMidi.h>
#endif // HAVE_RTMIDI

namespace DMSToolbox {
namespace Wersi {

// Create new instrument store
InstrumentStore::InstrumentStore(void* buffer, size_t size)
    : m_buffer(static_cast<uint8_t*>(buffer))
    , m_size(size)
    , m_icb()
    , m_vcf()
    , m_ampl()
    , m_freq()
    , m_wave()
{
}

// Destroy instrument store
InstrumentStore::~InstrumentStore()
{
}

// Copy instrument store contents
void InstrumentStore::copyContents(const InstrumentStore& source)
{
    // Copy ICBs
    for (auto& i : source) {
        Icb* icb = getIcb(i.first - 128);
        if (icb != nullptr) {
            icb->copy(i.second);
            if (icb->getNextIcb() != 0) {
                icb->setNextIcb(icb->getNextIcb() - 128);
            }
            icb->setVcfBlock(icb->getVcfBlock() - 128);
            icb->setAmplBlock(icb->getAmplBlock() - 128);
            icb->setFreqBlock(icb->getFreqBlock() - 128);
            icb->setWaveBlock(icb->getWaveBlock() - 128);
            icb->update();
        }
    }

    // Copy VCFs
    for (size_t i = 0; i < 10; ++i) {
        uint8_t addr = i + 65;
        auto src = const_cast<InstrumentStore*>(&source)->getVcf(addr + 128);
        auto dst = getVcf(addr);
        if (src != nullptr && dst != nullptr) {
            dst->copy(*src);
        }
    }

    // Copy AMPLs
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        auto src = const_cast<InstrumentStore*>(&source)->getAmpl(addr + 128);
        auto dst = getAmpl(addr);
        if (src != nullptr && dst != nullptr) {
            dst->copy(*src);
        }
    }

    // Copy FREQs
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        auto src = const_cast<InstrumentStore*>(&source)->getFreq(addr + 128);
        auto dst = getFreq(addr);
        if (src != nullptr && dst != nullptr) {
            dst->copy(*src);
        }
    }

    // Copy WAVEs
    for (size_t i = 0; i < 20; ++i) {
        uint8_t addr = i + 65;
        if (i >= 10) {
            ++addr;
        }
        auto src = const_cast<InstrumentStore*>(&source)->getWave(addr + 128);
        auto dst = getWave(addr);
        if (src != nullptr && dst != nullptr) {
            dst->copy(*src);
        }
    }
}

#ifdef HAVE_RTMIDI
// Read instrument store contents from device
void InstrumentStore::readFromDevice(RtMidiIn* /*inPort*/, RtMidiOut* /*outPort*/,
                                     bool(* /*callback*/)(void*, uint32_t, uint32_t), void* /*object*/)
{
    throw MidiException("Cannot read contents for this instrument store from device");
}

// SysEx receive callback
void InstrumentStore::receivedSysEx(std::vector<unsigned char>* /*message*/)
{
    throw MidiException("Cannot handle SysEx message in this instrument store");
}
#endif // HAVE_RTMIDI

// Return begin iterator to ICB map
std::map<uint8_t, Icb>::iterator InstrumentStore::begin()
{
    return m_icb.begin();
}

// Return const begin iterator to ICB map
std::map<uint8_t, Icb>::const_iterator InstrumentStore::begin() const
{
    return m_icb.begin();
}

// Return end iterator to ICB map
std::map<uint8_t, Icb>::iterator InstrumentStore::end()
{
    return m_icb.end();
}

// Return const end iterator to ICB map
std::map<uint8_t, Icb>::const_iterator InstrumentStore::end() const
{
    return m_icb.end();
}

// Return ICB for given block number
Icb* InstrumentStore::getIcb(uint8_t block)
{
    auto ret = m_icb.find(block);
    if (ret == m_icb.end()) {
        return nullptr;
    }
    else {
        return &(ret->second);
    }
}

// Return VCF for given block number
Vcf* InstrumentStore::getVcf(uint8_t block)
{
    auto ret = m_vcf.find(block);
    if (ret == m_vcf.end()) {
        return nullptr;
    }
    else {
        return &(ret->second);
    }
}

// Return AMPL for given block number
Envelope* InstrumentStore::getAmpl(uint8_t block)
{
    auto ret = m_ampl.find(block);
    if (ret == m_ampl.end()) {
        return nullptr;
    }
    else {
        return &(ret->second);
    }
}

// Return FREQ for given block number
Envelope* InstrumentStore::getFreq(uint8_t block)
{
    auto ret = m_freq.find(block);
    if (ret == m_freq.end()) {
        return nullptr;
    }
    else {
        return &(ret->second);
    }
}

// Return WAVE for given block number
Wave* InstrumentStore::getWave(uint8_t block)
{
    auto ret = m_wave.find(block);
    if (ret == m_wave.end()) {
        return nullptr;
    }
    else {
        return &(ret->second);
    }
}

// Clear all lists
void InstrumentStore::clearLists()
{
    m_icb.clear();
    m_vcf.clear();
    m_ampl.clear();
    m_freq.clear();
    m_wave.clear();
}

} // namespace Wersi
} // namespace DMSToolbox
