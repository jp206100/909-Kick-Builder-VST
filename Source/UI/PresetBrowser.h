/*
  ==============================================================================

    PresetBrowser.h
    Preset selection interface

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PresetBrowser : public juce::Component
{
public:
    PresetBrowser();
    ~PresetBrowser() override;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBrowser)
};
