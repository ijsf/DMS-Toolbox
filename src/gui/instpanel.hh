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

namespace DMSToolbox {

namespace Wersi {
// Forward declarations
class InstrumentStore;
class Icb;
class Vcf;
} // namespace Wersi

namespace Gui {

/**
  @ingroup gui_group

  Instrument panel implementation.

  This class implements the instrument panel providing editors for ICB and VCF data.
 */
class InstPanel : public InstPanelBase {
    public:
        /**
          Create instrument panel.

          Creates the instrument panel providing editors for ICB and VCF data.

          @param[in]    parent      Parent window this frame belongs to
         */
        InstPanel(wxWindow* parent);

        /**
          Set instrument to edit.

          Sets the instrument to edit, updating all fields and associating the ICB and VCF blocks with the panel
          to apply changes in the GUI to them.

          @param[in]    store       Instrument store to get data from
          @param[in]    icbNum      ICB number
         */
        void setInstrument(Wersi::InstrumentStore* store, uint8_t icbNum);

    protected:
        Wersi::InstrumentStore*     m_store;        ///< Instrument store data belongs to
        uint8_t                     m_icbNum;       ///< Block number of ICB being edited
        Wersi::Icb*                 m_icb;          ///< Pointer to ICB being edited
        Wersi::Vcf*                 m_vcf;          ///< Pointer to VCF being edited

        void onIcbChoice(wxCommandEvent& event);
        void onVcfChoice(wxCommandEvent& event);
        void onAmplChoice(wxCommandEvent& event);
        void onFreqChoice(wxCommandEvent& event);
        void onWaveChoice(wxCommandEvent& event);
};

} // namespace Gui
} // namespace DMSToolbox
