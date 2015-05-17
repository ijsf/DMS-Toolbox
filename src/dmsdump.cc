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

#include <wersi/mk1cartridge.hh>
#include <exceptions.hh>
#include <iostream>
#include <fstream>

using namespace std;
using namespace DMSToolbox;
using namespace DMSToolbox::Wersi;

int main(int argc, char** argv)
{
    // Check arguments
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Open and check input file
    ifstream f(argv[1], ios::binary);
    if (!f) {
        cerr << "Cannot open input file" << endl;
        return 2;
    }
    f.seekg(0, ios::end);
    size_t size = f.tellg();
    f.seekg(0, ios::beg);
    if (size > 1024 * 1024) {
        cerr << "Input file too large" << endl;
        return 3;
    }

    char* buf = new char[size];
    f.read(buf, size);

    try {
        Mk1Cartridge c(buf);
    }
    catch (DataFormatException& e) {
        cerr << "Data format exception: " << e.what() << endl;
    }
    catch (Exception& e) {
        cerr << "Other exception: " << e.what() << endl;
    }

    delete[] buf;

    return 0;
}
