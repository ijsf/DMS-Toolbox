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
#include <map>

#ifdef HAVE_RTMIDI
// Forward declarations
class RtMidiIn;
class RtMidiOut;
#endif // HAVE_RTMIDI

namespace DMSToolbox {
namespace Wersi {

// Forward declarations
class Icb;
class Vcf;
class Envelope;
class Wave;

/**
  @ingroup wersi_group

  Wersi DMS-System instrument store.

  This is the general interface of an instrument store for the Wersi DMS-System.
 */
class InstrumentStore {
    public:
        /**
          Create new instrument store.

          Creates a new instrument store. If an explicit update() is called, the update() method of all contained
          objects is called to update their part of the buffer, then the store raw buffer is updated with this new
          information.

          @param[in]    buffer      Raw data buffer
          @param[in]    size        Raw data buffer size
         */
        InstrumentStore(void* buffer, size_t size);

        /**
          Destroy instrument store.

          Destroys the instrument store.
         */
        virtual ~InstrumentStore();

        /**
          Get buffer pointer.

          Return raw buffer pointer.

          @return                   Raw buffer pointer
         */
        virtual void* getBuffer() {
            return m_buffer;
        }

        /**
          Get buffer size.

          Return raw buffer size.

          @return                   Raw buffer size
         */
        virtual size_t getBufferSize() {
            return m_size;
        }

        /**
          Copy instrument store contents.

          Copies as much instrument data as possible from the source store.
          TODO this currently assumes source store is a cartridge, this store a device.

          @param[in]    source      Source instrument store to copy data from
         */
        void copyContents(const InstrumentStore& source);

#ifdef HAVE_RTMIDI
        /**
          Read instrument store contents from device.

          Reads the instrument store contents from the device using MIDI.

          @param[in]    inPort      MIDI input port
          @param[in]    outPort     MIDI output port
         */
        virtual void readFromDevice(RtMidiIn* inPort, RtMidiOut* outPort);
#endif // HAVE_RTMIDI

        /**
          Dissect instrument store raw data buffer.

          Parses the raw instrument store data buffer and updates object members.
         */
        virtual void dissect() = 0;

        /**
          Update instrument store raw data buffer.

          Writes back changes in the instrument store and children to the associated raw data buffer.
         */
        virtual void update() = 0;

        /**
          Get number of primary ICBs

          Returns the instrument store specific number of primary ICBs

          @return                   Number of primary ICBs
         */
        virtual size_t getNumIcbs() const = 0;

        /**
          Get iterator to beginning of ICB map.

          Returns an iterator to the beginning of the ICB map.

          @return                   Iterator to the beginning of the ICB map
         */
        std::map<uint8_t, Icb>::iterator begin();

        /**
          Get const iterator to beginning of ICB map.

          Returns an iterator to the beginning of the ICB map.

          @return                   Iterator to the beginning of the ICB map
         */
        std::map<uint8_t, Icb>::const_iterator begin() const;

        /**
          Get iterator to end of ICB map.

          Returns an iterator to the end of the ICB map.

          @return                   Iterator to the end of the ICB map
         */
        std::map<uint8_t, Icb>::iterator end();

        /**
          Get const iterator to end of ICB map.

          Returns an iterator to the end of the ICB map.

          @return                   Iterator to the end of the ICB map
         */
        std::map<uint8_t, Icb>::const_iterator end() const;

        /**
          Get ICB by block number.

          Returns the ICB for the given block number or nullptr if not found.

          @param[in]    block       Block number to look up ICB for

          @return                   Pointer to ICB or nullptr if not found
         */
        Icb* getIcb(uint8_t block);

        /**
          Get VCF by block number.

          Returns the VCF for the given block number or nullptr if not found.

          @param[in]    block       Block number to look up VCF for

          @return                   Pointer to VCF or nullptr if not found
         */
        Vcf* getVcf(uint8_t block);

        /**
          Get AMPL by block number.

          Returns the AMPL for the given block number or nullptr if not found.

          @param[in]    block       Block number to look up AMPL for

          @return                   Pointer to AMPL or nullptr if not found
         */
        Envelope* getAmpl(uint8_t block);

        /**
          Get FREQ by block number.

          Returns the FREQ for the given block number or nullptr if not found.

          @param[in]    block       Block number to look up FREQ for

          @return                   Pointer to FREQ or nullptr if not found
         */
        Envelope* getFreq(uint8_t block);

        /**
          Get WAVE by block number.

          Returns the WAVE for the given block number or nullptr if not found.

          @param[in]    block       Block number to look up WAVE for

          @return                   Pointer to WAVE or nullptr if not found
         */
        Wave* getWave(uint8_t block);

    protected:
        uint8_t*                    m_buffer;               ///< Raw data buffer
        size_t                      m_size;                 ///< Raw data buffer size

        std::map<uint8_t, Icb>      m_icb;                  ///< ICB data
        std::map<uint8_t, Vcf>      m_vcf;                  ///< VCF data
        std::map<uint8_t, Envelope> m_ampl;                 ///< AMPL data
        std::map<uint8_t, Envelope> m_freq;                 ///< FREQ data
        std::map<uint8_t, Wave>     m_wave;                 ///< WAVE data

        /**
          Clear all lists.

          Clears all lists, usually in preparation of a fresh dissect() call.
         */
        void clearLists();

    private:
        InstrumentStore(const InstrumentStore&);            ///< Inhibit copying objects
        InstrumentStore& operator=(const InstrumentStore&); ///< Inhibit copying objects
};

} // namespace Wersi
} // namespace DMSToolbox
