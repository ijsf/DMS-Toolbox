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
#include <gui/generated.h>
#include <map>
#include <string>

namespace DMSToolbox {

namespace Wersi {
// Forward declarations
class InstrumentStore;
} // namespace Wersi

namespace Gui {

// Forward declarations
class InstPanel;
class EnvelopePanel;
class WavePanel;

/**
  @ingroup gui_group

  Main frame implementation.

  This class implements the parts of the main frame that are not included in the generated MainFrameBase class.
 */
class MainFrame : public MainFrameBase {
    public:
        /**
          Create main frame.

          Creates the main frame of DMS-Toolbox.

          @param[in]    parent      Parent window this frame belongs to
         */
        MainFrame(wxWindow* parent);

    protected:
        /**
          Instrument selection event handler.

          This handler is called when an instrument (or folder) has been selected in the instrument tree.

          @param[in]    event       Event for instrument selection
         */
        virtual void onInstSelect(wxTreeEvent& event);

    private:
        /// Helper class to store instrument data with choice
        class InstrumentHelper : public wxTreeItemData {
            public:
                /// Create instrument helper
                InstrumentHelper(Wersi::InstrumentStore* store, uint8_t icb)
                    : wxTreeItemData()
                    , m_store(store)
                    , m_icb(icb) {
                }

                /// Get instrument store from instrument helper
                Wersi::InstrumentStore* getStore() const {
                    return m_store;
                }

                /// Get ICB number from instrument helper
                uint8_t getIcb() const {
                    return m_icb;
                }

            private:
                Wersi::InstrumentStore* m_store;    ///< Instrument store
                uint8_t                 m_icb;      ///< ICB number of 0 if store root
        };

        InstPanel*      m_instPanel;        ///< Instrument panel
        EnvelopePanel*  m_envelopePanel;    ///< Envelope panel
        WavePanel*      m_wavePanel;        ///< Wave panel
        wxTreeItemId    m_root;             ///< Root node of the instrument tree
        wxTreeItemId    m_devices;          ///< Devices node of the instrument tree
        wxTreeItemId    m_cartridges;       ///< Cartridges node of the instrument tree

        /// Instrument stores, mapped to tree item IDs
        std::map<wxTreeItemId, Wersi::InstrumentStore*>    m_instrumentStores;

        /// TODO temporary
        void addCartridge(const std::string& fileName, const std::string& cartName);
};

} // namespace Gui
} // namespace DMSToolbox
