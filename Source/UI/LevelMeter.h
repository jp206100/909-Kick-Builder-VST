/*
  ==============================================================================

    LevelMeter.h
    Output level meter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class LevelMeter : public juce::Component
{
public:
    LevelMeter();
    ~LevelMeter() override;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
};
