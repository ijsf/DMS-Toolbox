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

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include <exceptions.hh>
#include <gui/mainframe.hh>

namespace DMSToolbox {
namespace Gui {

/**
  @ingroup gui_group

  DMS-Toolbox GUI application.

  This is the wxApp based application of the DMS toolbox GUI
*/
class DmsTb : public wxApp {
    public:
        /// Implements wxApp::OnInit()
        virtual bool OnInit();
};

// Initialize application
bool DmsTb::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    MainFrame* frame = new MainFrame(NULL);
    frame->Show(true);
    frame->applyConfiguration();
    return true;
}

} // namespace Gui
} // namespace DMSToolbox

#ifdef wxIMPLEMENT_APP // TODO wxWidgets version
wxIMPLEMENT_APP(DMSToolbox::Gui::DmsTb);
#else // wxIMPLEMENT_APP
IMPLEMENT_APP(DMSToolbox::Gui::DmsTb)
#endif // wxIMPLEMENT_APP
