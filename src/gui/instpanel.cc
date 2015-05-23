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

#include <gui/instpanel.hh>
#include <wersi/instrumentstore.hh>
#include <wersi/icb.hh>
#include <wersi/vcf.hh>

using namespace DMSToolbox::Wersi;

namespace DMSToolbox {
namespace Gui {

// Create instrument panel
InstPanel::InstPanel(wxWindow* parent)
    : InstPanelBase(parent)
    , m_store(nullptr)
    , m_icbNum(0)
    , m_icb(nullptr)
    , m_vcf(nullptr)
{
}

// Load instrument data into panel
void InstPanel::setInstrument(Wersi::InstrumentStore* store, uint8_t icbNum)
{
    // Update local pointers
    m_store = store;
    m_icbNum = icbNum;
    m_icb = nullptr;
    m_vcf = nullptr;
    if (m_store != nullptr) {
        m_icb = m_store->getIcb(m_icbNum);
        if (m_icb != nullptr) {
            m_vcf = m_store->getVcf(m_icb->getVcfBlock());
        }
    }

    // Handle ICB
    if (m_icb != nullptr) {
        m_nextIcbChoice->Clear();
        m_nextIcbChoice->Append(_("none"));
        m_nextIcbChoice->SetSelection(0);
        uint8_t tmp = m_icb->getNextIcb();
        for (auto& i : *m_store) {
            m_nextIcbChoice->Append(wxString::From8BitData(i.second.getName().c_str()));
            if (i.first == tmp) {
                m_nextIcbChoice->SetSelection(m_nextIcbChoice->GetCount() - 1);
            }
        }
        m_vcfChoice->Clear();
        m_amplChoice->Clear();
        m_freqChoice->Clear();
        m_waveChoice->Clear();
        m_leftCheckBox->SetValue(m_icb->getLeft());
        m_rightCheckBox->SetValue(m_icb->getRight());
        m_brightCheckBox->SetValue(m_icb->getBright());
        m_vcfCheckBox->SetValue(m_icb->getVcf());
        m_wvCheckBox->SetValue(m_icb->getWersiVoice());
        m_unk1CheckBox->SetValue(false);
        m_transposeInput->Clear();
        *m_transposeInput << int16_t(m_icb->getTranspose());
        m_detuneInput->Clear();
        *m_detuneInput << int16_t(m_icb->getDetune());
        m_wvModeChoice->Clear();
        m_wvLeftCheckBox->SetValue(m_icb->getWvLeft());
        m_wvRightCheckBox->SetValue(m_icb->getWvRight());
        m_wvFbFlatCheckBox->SetValue(m_icb->getWvFbFlat());
        m_wvFbDeepCheckBox->SetValue(m_icb->getWvFbDeep());
        m_nameInput->ChangeValue(wxString::From8BitData(m_icb->getName().c_str()));
    }
    else {
        // TODO clear and disable inputs
    }

    // Handle VCF
    if (m_vcf != nullptr) {
        m_vcfLeftCheckBox->SetValue(m_vcf->getLeft());
        m_vcfRightCheckBox->SetValue(m_vcf->getRight());
        m_lowPassCheckBox->SetValue(m_vcf->getLowPass());
        m_fourPolesCheckBox->SetValue(m_vcf->getFourPoles());
        m_vcfWvCheckBox->SetValue(m_vcf->getWersiVoice());
        m_noiseCheckBox->SetValue(m_vcf->getNoise());
        m_distortionCheckBox->SetValue(m_vcf->getDistortion());
        m_unk2CheckBox->SetValue(false);
        m_cutoffInput->Clear();
        *m_cutoffInput << int16_t(m_vcf->getFrequency());
        m_resonanceInput->Clear();
        *m_resonanceInput << uint16_t(m_vcf->getQuality());
        m_noiseTypeChoice->Clear();
        m_envModeChoice->Clear();
        m_retriggerCheckBox->SetValue(m_vcf->getRetrigger());
        m_trackingCheckBox->SetValue(m_vcf->getTracking());
        m_t1TimeInput->Clear();
        *m_t1TimeInput << uint16_t(m_vcf->getT1Time());
        m_t1IntensityInput->Clear();
        *m_t1IntensityInput << int16_t(m_vcf->getT1Intensity());
        m_t1OffsetInput->Clear();
        *m_t1OffsetInput << int16_t(m_vcf->getT1Offset());
        m_t2TimeInput->Clear();
        *m_t2TimeInput << uint16_t(m_vcf->getT2Time());
        m_t2IntensityInput->Clear();
        *m_t2IntensityInput << int16_t(m_vcf->getT2Intensity());
        m_t2OffsetInput->Clear();
        *m_t2OffsetInput << int16_t(m_vcf->getT2Offset());
    }
    else {
        // TODO clear and disable inputs
    }
}

void InstPanel::onIcbChoice(wxCommandEvent& /*event*/)
{
    // TODO: Implement onIcbChoice
}

void InstPanel::onVcfChoice(wxCommandEvent& /*event*/)
{
    // TODO: Implement onVcfChoice
}

void InstPanel::onAmplChoice(wxCommandEvent& /*event*/)
{
    // TODO: Implement onAmplChoice
}

void InstPanel::onFreqChoice(wxCommandEvent& /*event*/)
{
    // TODO: Implement onFreqChoice
}

void InstPanel::onWaveChoice(wxCommandEvent& /*event*/)
{
    // TODO: Implement onWaveChoice
}

} // namespace Gui
} // namespace DMSToolbox
