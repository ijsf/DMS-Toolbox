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

#include <wersi/wave.hh>
#include <cstring>

#ifdef WIN32
#pragma warning ( disable : 4351 )
#endif // WIN32

namespace DMSToolbox {
namespace Wersi {

// Create new wave object
Wave::Wave(uint8_t blockNum, void* buffer, size_t size)
    : m_blockNum(blockNum)
    , m_buffer(static_cast<uint8_t*>(buffer))
    , m_size(size)
    , m_fixedFormants(false)
    , m_level(0)
    , m_bassWave()
    , m_tenorWave()
    , m_altoWave()
    , m_sopranoWave()
    , m_fixFormData()
{
}

// Create wave object by copying
Wave::Wave(const Wave& source)
    : m_blockNum(0)
    , m_buffer(nullptr)
    , m_size(0)
    , m_fixedFormants(false)
    , m_level(0)
    , m_bassWave()
    , m_tenorWave()
    , m_altoWave()
    , m_sopranoWave()
    , m_fixFormData()
{
    *this = source;
}

// Destroy wave object
Wave::~Wave()
{
}

// Copy wave object members
Wave& Wave::operator=(const Wave& source)
{
    if (this != &source) {
        m_blockNum      = source.m_blockNum;
        m_buffer        = source.m_buffer;
        m_size          = source.m_size;

        m_fixedFormants = source.m_fixedFormants;
        m_level         = source.m_level;

        memcpy(m_bassWave,    source.m_bassWave,    sizeof(m_bassWave));
        memcpy(m_tenorWave,   source.m_tenorWave,   sizeof(m_tenorWave));
        memcpy(m_altoWave,    source.m_altoWave,    sizeof(m_altoWave));
        memcpy(m_sopranoWave, source.m_sopranoWave, sizeof(m_sopranoWave));
        memcpy(m_fixFormData, source.m_fixFormData, sizeof(m_bassWave));
    }
    return *this;
}

// Dissect raw wave data
void Wave::Dissect()
{
    m_level         = m_buffer[0] & 0x7f;
    m_fixedFormants = (m_buffer[0] & 0x80) != 0;

    if (m_size > 64) {
        memcpy(m_bassWave, &(m_buffer[1]), sizeof(m_bassWave));
    }
    else {
        memset(m_bassWave, 0, sizeof(m_bassWave));
    }

    if (m_size > 128) {
        memcpy(m_tenorWave, &(m_buffer[65]), sizeof(m_tenorWave));
    }
    else {
        memset(m_tenorWave, 0, sizeof(m_tenorWave));
    }

    if (m_size > 160) {
        memcpy(m_altoWave, &(m_buffer[129]), sizeof(m_altoWave));
    }
    else {
        memset(m_altoWave, 0, sizeof(m_altoWave));
    }

    if (m_size > 176) {
        memcpy(m_sopranoWave, &(m_buffer[161]), sizeof(m_sopranoWave));
    }
    else {
        memset(m_sopranoWave, 0, sizeof(m_sopranoWave));
    }

    if (m_size > 211) {
        memcpy(m_fixFormData, &(m_buffer[177]), sizeof(m_fixFormData));
    }
    else {
        memset(m_fixFormData, 0, sizeof(m_fixFormData));
    }
}

// Put together and udate wave raw data
void Wave::Update()
{
}

} // namespace Wersi
} // namespace DMSToolbox
