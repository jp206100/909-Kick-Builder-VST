/*
  ==============================================================================

    ADSRDisplay.h
    ADSR envelope visualizer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ADSRDisplay : public juce::Component
{
public:
    ADSRDisplay();
    ~ADSRDisplay() override;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRDisplay)
};
