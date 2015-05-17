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

#include <wersi/envelope.hh>

namespace DMSToolbox {
namespace Wersi {

// Create new envelope object
Envelope::Envelope(uint8_t blockNum, void* buffer, size_t size)
    : m_blockNum(blockNum)
    , m_buffer(static_cast<uint8_t*>(buffer))
    , m_size(size)
{
}

// Create new envelope object by copying
Envelope::Envelope(const Envelope& source)
    : m_blockNum(0)
    , m_buffer(nullptr)
    , m_size(0)
{
    *this = source;
}

// Destroy envelope object
Envelope::~Envelope()
{
}

// Copy envelope object members
Envelope& Envelope::operator=(const Envelope& source)
{
    if (this != &source) {
        m_blockNum = source.m_blockNum;
        m_buffer = source.m_buffer;
        m_size = source.m_size;
    }
    return *this;
}

// Dissect envelope raw data
void Envelope::Dissect()
{
}

// Put together and update envelope raw data
void Envelope::Update()
{
}

} // namespace Wersi
} // namespace DMSToolbox
