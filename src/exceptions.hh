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

#pragma once

#include <common.hh>
#include <exception>
#include <sstream>

namespace DMSToolbox {

/**
  @ingroup common_group

  DMS-Toolbox exception base class.

  All Exceptions should inherit from this base exception.
 */
class Exception : public std::exception {
    public:
        /**
          Create new exception with error string.

          Creates a new exception and sets the given string as error string.

          @param[in]    string      Error string to use for the exception
         */
        Exception(const std::string& string) throw();

        /**
          Create exception by copying.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from
         */
        Exception(const Exception& source) throw();

        /**
          Destroy exception.

          Destroys the exception.
         */
        virtual ~Exception() throw();

        /**
          Copy exception.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from

          @return                   This object
         */
        Exception& operator=(const Exception& source) throw();

        /**
          Get exception error string.

          Returns the error string of the exception.

          @return                   Error string of the exception
         */
        virtual const char* what() const throw();

        /**
          Add exception message data.

          Adds more data to the exception dispatching the stream operator to the message stream.

          @param[in]    val         Data to append to exception message

          @return                   Reference to this object
         */
        template<typename T>
        Exception& operator<<(T val) {
            m_stream << val;
            return *this;
        }

    protected:
        std::ostringstream  m_stream;   ///< Stream to append contents to

    private:
        mutable const char* m_message;  ///< Exception message to be delivered by what()
};

/**
  @ingroup common_group

  Configuration exception.

  This exception is used in cases where an error occured due to a problem in the configuration.
 */
class ConfigurationException : public Exception {
    public:
        /**
          Create new configuration exception.

          Creates a new configuration exception with given error string.

          @param[in]    string      Error string to use for the exception
         */
        ConfigurationException(const std::string& string) throw();

        /**
          Create exception by copying.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from
         */
        ConfigurationException(const ConfigurationException& source) throw();

        /**
          Destroy exception.

          Destroys the exception.
         */
        virtual ~ConfigurationException() throw();

        /**
          Copy exception.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from

          @return                   This object
         */
        ConfigurationException& operator=(const ConfigurationException& source) throw();
};

/**
  @ingroup common_group

  System exception.

  This exception is used in cases where an error is reported from the operating system.
 */
class SystemException : public Exception {
    public:
        /**
          Create new system exception.

          Creates a new system exception with given error string.

          @param[in]    string      Error string to use for the exception
         */
        SystemException(const std::string& string) throw();

        /**
          Create exception by copying.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from
         */
        SystemException(const SystemException& source) throw();

        /**
          Destroy exception.

          Destroys the exception.
         */
        virtual ~SystemException() throw();

        /**
          Copy exception.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from

          @return                   This object
         */
        SystemException& operator=(const SystemException& source) throw();
};

/**
  @ingroup common_group

  MIDI exception.

  This exception is used in cases where an error is reported by the MIDI transport layer.
 */
class MidiException : public Exception {
    public:
        /**
          Create new MIDI exception.

          Creates a new MIDI exception with given error string.

          @param[in]    string      Error string to use for the exception
         */
        MidiException(const std::string& string) throw();

        /**
          Create exception by copying.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from
         */
        MidiException(const MidiException& source) throw();

        /**
          Destroy exception.

          Destroys the exception.
         */
        virtual ~MidiException() throw();

        /**
          Copy exception.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from

          @return                   This object
         */
        MidiException& operator=(const MidiException& source) throw();
};

/**
  @ingroup common_group

  Data format exception.

  This exception is used in cases where an error is reported by a programmer driver.
 */
class DataFormatException : public Exception {
    public:
        /**
          Create new data format exception.

          Creates a new data format exception with given error string.

          @param[in]    string      Error string to use for the exception
         */
        DataFormatException(const std::string& string) throw();

        /**
          Create exception by copying.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from
         */
        DataFormatException(const DataFormatException& source) throw();

        /**
          Destroy exception.

          Destroys the exception.
         */
        virtual ~DataFormatException() throw();

        /**
          Copy exception.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from

          @return                   This object
         */
        DataFormatException& operator=(const DataFormatException& source) throw();
};

/**
  @ingroup gui_group

  GUI exception.

  This exception is used in cases where an error is reported by the GUI modules.
 */
class GuiException : public Exception {
    public:
        /**
          Create new GUI exception.

          Creates a new GUI exception with given error string.

          @param[in]    string      Error string to use for the exception
         */
        GuiException(const std::string& string) throw();

        /**
          Create exception by copying.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from
         */
        GuiException(const GuiException& source) throw();

        /**
          Destroy exception.

          Destroys the exception.
         */
        virtual ~GuiException() throw();

        /**
          Copy exception.

          Copies the exception and it's current error string.

          @param[in]    source      Source exception to copy from

          @return                   This object
         */
        GuiException& operator=(const GuiException& source) throw();
};

} // DMSToolbox
