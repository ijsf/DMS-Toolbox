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
class Envelope;
} // namespace Wersi

namespace Gui {

/**
  @ingroup gui_group

  Envelope panel implementation.

  This class implements the envelope panel logic.
 */
class EnvelopePanel : public EnvelopePanelBase {
    public:
        /**
          Create envelope panel.

          Creates the envelope panel allowing editing the AMPL and FREQ envelopes.

          @param[in]    parent      Parent window this panel belongs to
         */
        EnvelopePanel(wxWindow* parent);

        /**
          Set envelopes to edit.

          Sets the envelopes to edit, updating all fields and associating the AMPL and FREQ envelopes with the
          panel to apply changes in the GUI to them.

          @param[in]    ampl        AMPL envelope
          @param[in]    freq        FREQ envelope
         */
        void setEnvelopes(Wersi::Envelope* ampl, Wersi::Envelope* freq);

    private:
        Wersi::Envelope*    m_ampl;     ///< AMPL envelope being edited
        Wersi::Envelope*    m_freq;     ///< FREQ envelope being edited
};

} // namespace Gui
} // namespace DMSToolbox
