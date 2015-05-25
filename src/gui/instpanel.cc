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
        // Set ICB name
        m_nameInput->ChangeValue(wxString::From8BitData(m_icb->getName().c_str()));

        // Get number of primary ICBs
        size_t numIcb = store->getNumIcbs();

        // Construct next ICB choice list
        m_nextIcbChoice->Clear();
        m_nextIcbChoice->Append(_("none"));
        m_nextIcbChoice->SetSelection(0);
        uint8_t tmp = m_icb->getNextIcb();
        uint8_t offset = 0;
        for (auto& i : *m_store) {
            if (offset == 0) {
                offset = i.first - 1;
            }
            wxString instName(wxT("("));
            instName << uint16_t(i.first) << wxT(") ");
            instName << wxString::From8BitData(i.second.getName().c_str());
            m_nextIcbChoice->Append(instName);
            if (i.first == tmp) {
                m_nextIcbChoice->SetSelection(m_nextIcbChoice->GetCount() - 1);
            }
        }

        // Construct VCF choice list
        m_vcfChoice->Clear();
        tmp = m_icb->getVcfBlock();
        for (size_t i = 0; i < 10; ++i) {
            uint16_t addr = i + offset;
            wxString name;
            name << addr;
            m_vcfChoice->Append(name);
            if (addr == tmp) {
                m_vcfChoice->SetSelection(m_vcfChoice->GetCount() - 1);
            }
        }

        // Construct AMPL choice list
        m_amplChoice->Clear();
        tmp = m_icb->getAmplBlock();
        for (size_t i = 0; i < 20; ++i) {
            uint16_t addr = i + offset;
            if (i >= numIcb) {
                ++addr;
            }
            wxString name;
            name << addr;
            m_amplChoice->Append(name);
            if (addr == tmp) {
                m_amplChoice->SetSelection(m_amplChoice->GetCount() - 1);
            }
        }

        // Construct FREQ choice list
        m_freqChoice->Clear();
        tmp = m_icb->getFreqBlock();
        for (size_t i = 0; i < 20; ++i) {
            uint16_t addr = i + offset;
            if (i >= numIcb) {
                ++addr;
            }
            wxString name;
            name << addr;
            m_freqChoice->Append(name);
            if (addr == tmp) {
                m_freqChoice->SetSelection(m_freqChoice->GetCount() - 1);
            }
        }

        // Construct WAVE choice list
        m_waveChoice->Clear();
        tmp = m_icb->getWaveBlock();
        for (size_t i = 0; i < 20; ++i) {
            uint16_t addr = i + offset;
            if (i >= numIcb) {
                ++addr;
            }
            wxString name;
            name << addr;
            m_waveChoice->Append(name);
            if (addr == tmp) {
                m_waveChoice->SetSelection(m_waveChoice->GetCount() - 1);
            }
        }

        // Configure dynamics inputs
        m_dynamicsChoice->SetSelection(m_icb->getDynamics());
        m_lowSelCheckBox->SetValue(m_icb->getLowSelect());
        m_highSelCheckBox->SetValue(m_icb->getHighSelect());

        // Configure output inputs
        m_leftCheckBox->SetValue(m_icb->getLeft());
        m_rightCheckBox->SetValue(m_icb->getRight());
        m_vcfCheckBox->SetValue(m_icb->getVcf());
        m_wvCheckBox->SetValue(m_icb->getWersiVoice());

        // Configure transpose/detune/bright
        m_transposeInput->Clear();
        *m_transposeInput << int16_t(m_icb->getTranspose());
        m_detuneInput->Clear();
        *m_detuneInput << int16_t(m_icb->getDetune());
        m_brightCheckBox->SetValue(m_icb->getBright());

        // Configure WersiVoice inputs
        m_wvLeftCheckBox->SetValue(m_icb->getWvLeft());
        m_wvRightCheckBox->SetValue(m_icb->getWvRight());
        m_wvFbFlatCheckBox->SetValue(m_icb->getWvFbFlat());
        m_wvFbDeepCheckBox->SetValue(m_icb->getWvFbDeep());
        m_wvModeChoice->SetSelection(static_cast<uint8_t>(m_icb->getWvMode()));
    }
    else {
        // TODO clear and disable inputs
    }

    // Handle VCF
    if (m_vcf != nullptr) {
        // Configure output inputs
        m_vcfLeftCheckBox->SetValue(m_vcf->getLeft());
        m_vcfRightCheckBox->SetValue(m_vcf->getRight());
        m_vcfWvCheckBox->SetValue(m_vcf->getWersiVoice());

        // Configure mode inputs
        m_lowPassCheckBox->SetValue(m_vcf->getLowPass());
        m_fourPolesCheckBox->SetValue(m_vcf->getFourPoles());
        m_distortionCheckBox->SetValue(m_vcf->getDistortion());

        // Configure cutoff/resonance inputs
        m_cutoffInput->Clear();
        *m_cutoffInput << int16_t(m_vcf->getFrequency());
        m_resonanceInput->Clear();
        *m_resonanceInput << uint16_t(m_vcf->getQuality());

        // Configure noise inputs
        m_noiseCheckBox->SetValue(m_vcf->getNoise());
        m_noiseTypeChoice->SetSelection(static_cast<uint8_t>(m_vcf->getNoiseType()));

        // Configure envelope inputs
        m_envModeChoice->SetSelection(static_cast<uint8_t>(m_vcf->getEnvelopeMode()));
        m_retriggerCheckBox->SetValue(m_vcf->getRetrigger());
        m_trackingCheckBox->SetValue(m_vcf->getTracking());

        // Configure envelope parameter inputs
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
