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

#include <exceptions.hh>
#include <cstdlib>
#include <cstring>

using namespace std;

namespace DMSToolbox {

// Exception base class
Exception::Exception(const std::string& string) throw()
    : exception()
    , m_stream(string)
    , m_message(NULL)
{
    m_stream.seekp(0, ios_base::end);
}

Exception::Exception(const Exception& source) throw()
    : exception()
    , m_stream()
    , m_message(NULL)
{
    *this = source;
}

Exception::~Exception() throw()
{
    if (m_message != NULL) {
        free(const_cast<char*>(m_message));
    }
}

Exception& Exception::operator=(const Exception& source) throw()
{
    if (this != &source) {
        m_stream.str(source.m_stream.str());
        m_stream.seekp(0, ios_base::end);
    }
    return *this;
}

const char* Exception::what() const throw()
{
    string message(m_stream.str());
    if (m_message != NULL) {
        free(const_cast<char*>(m_message));
    }
#ifdef WIN32
    m_message = _strdup(message.c_str());
#else // WIN32
    m_message = strdup(message.c_str());
#endif // WIN32
    return m_message;
}

// Configuration exception
ConfigurationException::ConfigurationException(const std::string& string) throw()
    : Exception(string)
{
}

ConfigurationException::ConfigurationException(const ConfigurationException& source) throw()
    : Exception(source)
{
    *this = source;
}

ConfigurationException::~ConfigurationException() throw()
{
}

ConfigurationException& ConfigurationException::operator=(const ConfigurationException& source) throw()
{
    if (this != &source) {
        m_stream.str(source.m_stream.str());
        m_stream.seekp(0, ios_base::end);
    }
    return *this;
}

// System exception
SystemException::SystemException(const std::string& string) throw()
    : Exception(string)
{
}

SystemException::SystemException(const SystemException& source) throw()
    : Exception(source)
{
    *this = source;
}

SystemException::~SystemException() throw()
{
}

SystemException& SystemException::operator=(const SystemException& source) throw()
{
    if (this != &source) {
        m_stream.str(source.m_stream.str());
        m_stream.seekp(0, ios_base::end);
    }
    return *this;
}

// MIDI exception
MidiException::MidiException(const std::string& string) throw()
    : Exception(string)
{
}

MidiException::MidiException(const MidiException& source) throw()
    : Exception(source)
{
    *this = source;
}

MidiException::~MidiException() throw()
{
}

MidiException& MidiException::operator=(const MidiException& source) throw()
{
    if (this != &source) {
        m_stream.str(source.m_stream.str());
        m_stream.seekp(0, ios_base::end);
    }
    return *this;
}

// Data format exception
DataFormatException::DataFormatException(const std::string& string) throw()
    : Exception(string)
{
}

DataFormatException::DataFormatException(const DataFormatException& source) throw()
    : Exception(source)
{
    *this = source;
}

DataFormatException::~DataFormatException() throw()
{
}

DataFormatException& DataFormatException::operator=(const DataFormatException& source) throw()
{
    if (this != &source) {
        m_stream.str(source.m_stream.str());
        m_stream.seekp(0, ios_base::end);
    }
    return *this;
}

// GUI exception
GuiException::GuiException(const std::string& string) throw()
    : Exception(string)
{
}

GuiException::GuiException(const GuiException& source) throw()
    : Exception(source)
{
    *this = source;
}

GuiException::~GuiException() throw()
{
}

GuiException& GuiException::operator=(const GuiException& source) throw()
{
    if (this != &source) {
        m_stream.str(source.m_stream.str());
        m_stream.seekp(0, ios_base::end);
    }
    return *this;
}

} // DMSToolbox
