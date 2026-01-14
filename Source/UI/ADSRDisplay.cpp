/*
  ==============================================================================

    ADSRDisplay.cpp

  ==============================================================================
*/

#include "ADSRDisplay.h"
#include "../Utils/Constants.h"

ADSRDisplay::ADSRDisplay() {}
ADSRDisplay::~ADSRDisplay() {}

void ADSRDisplay::paint(juce::Graphics& g)
{
    g.fillAll(Constants::Colors::BACKGROUND);
    g.setColour(Constants::Colors::TEXT_SECONDARY);
    g.drawText("ADSR Display (TODO)", getLocalBounds(), juce::Justification::centred);
}
