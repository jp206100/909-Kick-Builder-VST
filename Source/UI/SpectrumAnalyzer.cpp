/*
  ==============================================================================

    SpectrumAnalyzer.cpp

  ==============================================================================
*/

#include "SpectrumAnalyzer.h"
#include "../Utils/Constants.h"

SpectrumAnalyzer::SpectrumAnalyzer() {}
SpectrumAnalyzer::~SpectrumAnalyzer() {}

void SpectrumAnalyzer::paint(juce::Graphics& g)
{
    g.fillAll(Constants::Colors::BACKGROUND);
    g.setColour(Constants::Colors::TEXT_SECONDARY);
    g.drawText("Spectrum Analyzer (TODO)", getLocalBounds(), juce::Justification::centred);
}
