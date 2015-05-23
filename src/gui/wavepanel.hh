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
class Wave;
} // namespace Wersi

namespace Gui {

/**
  @ingroup gui_group

  Wave panel implementation.

  This class implements the wave panel allowing viewing and editing WAVE blocks.
 */
class WavePanel : public WavePanelBase {
    public:
        /**
          Create wave panel.

          Creates the wave panel allowing viewing and editing WAVE blocks.

          @param[in]    parent      Parent window this panel belongs to
         */
        WavePanel(wxWindow* parent);

        /**
          Set wave data to edit.

          Sets the wave data to edit, updating all fields and associating the wave data with the panel to apply changes
          in the GUI to it.

          @param[in]    wave        Wave data
         */
        void setWave(Wersi::Wave* wave);

    private:
        wxPanel*        m_bassPanel;    ///< Bass wave drawing panel
        wxPanel*        m_tenorPanel;   ///< Bass wave drawing panel
        wxPanel*        m_altoPanel;    ///< Bass wave drawing panel
        wxPanel*        m_sopranoPanel; ///< Bass wave drawing panel

        Wersi::Wave*    m_wave;         ///< Wave data being edited

        /**
          Handle paint event.

          Handles a paint event and redraws a wave.

          @param[in]    event       wxWidgets paint event
         */
        void onPaint(wxPaintEvent& event);
};

} // namespage Gui
} // namespace DMSToolbox
