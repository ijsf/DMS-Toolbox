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
#include <wersi/dx10cartridge.hh>
#include <wersi/icb.hh>
#include <wersi/vcf.hh>
#include <exceptions.hh>
#include <iostream>
#include <iomanip>
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

    InstrumentStore* is = nullptr;
    try {
        is = new Mk1Cartridge(buf);
        cout << "Detected MK1 cartridge" << endl;
    }
    catch (DataFormatException& e) {
        string mk1Error = e.what();
        try {
            is = new Dx10Cartridge(buf, size);
            cout << "Detected DX10/DX5 cartridge" << endl;
        }
        catch (DataFormatException& e) {
            cerr << "Cartridge is neither MK1 nor DX10/DX5 format" << endl;
            cerr << "MK1 error: " << mk1Error << endl;
            cerr << "DX10/DX5 error: " << e.what() << endl;
        }
    }
    catch (Exception& e) {
        cerr << "Other exception: " << e.what() << endl;
    }
    if (is != nullptr) {
        for (auto& i : *is) {
            Icb& icb = i.second;
            cout << "ICB " << setw(3) << uint16_t(i.first)
                 << " (" << setw(6) << icb.getName() << ")"
                 << ": Next " << setw(3) << uint16_t(icb.getNextIcb())
                 << " V " << setw(3) << uint16_t(icb.getVcfBlock())
                 << " A " << setw(3) << uint16_t(icb.getAmplBlock())
                 << " F " << setw(3) << uint16_t(icb.getFreqBlock())
                 << " W " << setw(3) << uint16_t(icb.getWaveBlock())
                 << " D " << setw(1) << uint16_t(icb.getDynamics())
                 << (icb.getLowSelect() ? 'L' : '-')
                 << (icb.getHighSelect() ? 'H' : '-')
                 << " O " << (icb.getLeft() ? 'L' : '-')
                 << (icb.getRight() ? 'R' : '-')
                 << (icb.getVcf() ? 'V' : '-')
                 << (icb.getWersiVoice() ? 'W' : '-')
                 << (icb.getBright() ? 'B' : '-')
                 << " T " << setw(4) << int16_t(icb.getTranspose())
                 << " D " << setw(4) << int16_t(icb.getDetune())
                 << " WV " << setw(10) << icb.getWvModeName(icb.getWvMode())
                 << " " << (icb.getWvLeft() ? 'L' : '-')
                 << (icb.getWvRight() ? 'R' : '-')
                 << (icb.getWvFbFlat() ? 'F' : '-')
                 << (icb.getWvFbDeep() ? 'D' : '-')
                 << " U " << hex << setw(2) << uint16_t(icb.getUnknownBits()) << dec
                 << endl;
            Vcf* vcf = is->getVcf(icb.getVcfBlock());
            if (vcf != nullptr) {
                cout << "   VCF " << (vcf->getLeft() ? 'L' : '-')
                     << (vcf->getRight() ? 'R' : '-')
                     << (vcf->getWersiVoice() ? 'W' : '-')
                     << (vcf->getNoise() ? 'N' : '-')
                     << (vcf->getDistortion() ? 'D' : '-')
                     << (vcf->getLowPass() ? " LP" : " BP")
                     << (vcf->getFourPoles() ? '4' : '2')
                     << " F " << setw(4) << int16_t(vcf->getFrequency())
                     << " Q " << setw(3) << uint16_t(vcf->getQuality())
                     << " NT " << setw(5) << vcf->getNoiseTypeName(vcf->getNoiseType())
                     << " " << (vcf->getRetrigger() ? 'R' : '-')
                     << (vcf->getTracking() ? 'T' : '-')
                     << " ENV " << setw(15) << vcf->getEnvelopeModeName(vcf->getEnvelopeMode())
                     << " T1 T " << setw(3) << uint16_t(vcf->getT1Time())
                     << " I " << setw(4) << int16_t(vcf->getT1Intensity())
                     << " O " << setw(4) << int16_t(vcf->getT1Offset())
                     << " T2 T " << setw(3) << uint16_t(vcf->getT2Time())
                     << " I " << setw(4) << int16_t(vcf->getT2Intensity())
                     << " O " << setw(4) << int16_t(vcf->getT2Offset())
                     << " U " << hex << setw(2) << uint16_t(vcf->getUnknownBits()) << dec
                     << endl;
            }
        }

        delete is;
    }

    delete[] buf;

    return 0;
}
