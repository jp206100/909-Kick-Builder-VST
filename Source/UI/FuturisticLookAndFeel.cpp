/*
  ==============================================================================

    FuturisticLookAndFeel.cpp

  ==============================================================================
*/

#include "FuturisticLookAndFeel.h"

FuturisticLookAndFeel::FuturisticLookAndFeel()
{
    setColour(juce::ResizableWindow::backgroundColourId, Constants::Colors::BACKGROUND);
    setColour(juce::Label::textColourId, Constants::Colors::TEXT_PRIMARY);
    setColour(juce::Slider::thumbColourId, Constants::Colors::ACCENT_BLUE);
    setColour(juce::Slider::trackColourId, Constants::Colors::KNOB_TRACK);
}

FuturisticLookAndFeel::~FuturisticLookAndFeel()
{
}
