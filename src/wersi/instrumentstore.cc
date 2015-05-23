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

using namespace std;

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

// Return begin iterator to ICB map
std::map<uint8_t, Icb>::iterator InstrumentStore::begin()
{
    return m_icb.begin();
}

// Return end iterator to ICB map
std::map<uint8_t, Icb>::iterator InstrumentStore::end()
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

} // namespace Wersi
} // namespace DMSToolbox
