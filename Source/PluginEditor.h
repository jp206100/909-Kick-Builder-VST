/*
  ==============================================================================

    PluginEditor.h
    Main plugin editor interface

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/FuturisticLookAndFeel.h"
#include "UI/WaveformDisplay.h"
#include "UI/SpectrumAnalyzer.h"
#include "UI/ADSRDisplay.h"

//==============================================================================
/**
 * Main editor component for 909 Kick Builder
 */
class NineZeroNineAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                           private juce::Timer
{
public:
    NineZeroNineAudioProcessorEditor (NineZeroNineAudioProcessor&);
    ~NineZeroNineAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Timer callback for UI updates
    void timerCallback() override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NineZeroNineAudioProcessor& audioProcessor;

    // Custom LookAndFeel
    FuturisticLookAndFeel futuristicLookAndFeel;

    // Visual components
    WaveformDisplay waveformDisplay;
    SpectrumAnalyzer spectrumAnalyzer;
    ADSRDisplay adsrDisplay;

    // Controls (using JUCE standard sliders for now)
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

    juce::Slider clipAmountSlider;
    juce::Label clipAmountLabel;
    juce::ToggleButton clipEnabledButton {"Clip to Zero"};

    juce::ComboBox engineModeCombo;
    juce::Label engineModeLabel;

    // Attachments for parameter binding
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    std::unique_ptr<SliderAttachment> clipAmountAttachment;
    std::unique_ptr<ButtonAttachment> clipEnabledAttachment;
    std::unique_ptr<ComboBoxAttachment> engineModeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NineZeroNineAudioProcessorEditor)
};
