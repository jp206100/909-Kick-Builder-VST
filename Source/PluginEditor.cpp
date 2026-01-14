/*
  ==============================================================================

    PluginEditor.cpp
    Main plugin editor implementation

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils/Constants.h"

//==============================================================================
NineZeroNineAudioProcessorEditor::NineZeroNineAudioProcessorEditor (NineZeroNineAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set custom LookAndFeel
    setLookAndFeel(&futuristicLookAndFeel);

    // Set plugin size
    setSize (Constants::UI::PLUGIN_WIDTH, Constants::UI::PLUGIN_HEIGHT);

    // Add waveform display
    addAndMakeVisible(waveformDisplay);

    // Add spectrum analyzer
    addAndMakeVisible(spectrumAnalyzer);

    // Add ADSR display
    addAndMakeVisible(adsrDisplay);

    // Engine Mode selector
    engineModeLabel.setText("Engine Mode", juce::dontSendNotification);
    engineModeLabel.setColour(juce::Label::textColourId, Constants::Colors::TEXT_PRIMARY);
    addAndMakeVisible(engineModeLabel);

    engineModeCombo.addItem("Sample", 1);
    engineModeCombo.addItem("Synthesis", 2);
    addAndMakeVisible(engineModeCombo);
    engineModeAttachment = std::make_unique<ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "engine_mode", engineModeCombo);

    // ADSR Controls
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setColour(juce::Label::textColourId, Constants::Colors::TEXT_PRIMARY);
    addAndMakeVisible(attackLabel);

    attackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(attackSlider);
    attackAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.getAPVTS(), "attack", attackSlider);

    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setColour(juce::Label::textColourId, Constants::Colors::TEXT_PRIMARY);
    addAndMakeVisible(decayLabel);

    decaySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(decaySlider);
    decayAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.getAPVTS(), "decay", decaySlider);

    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setColour(juce::Label::textColourId, Constants::Colors::TEXT_PRIMARY);
    addAndMakeVisible(sustainLabel);

    sustainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(sustainSlider);
    sustainAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.getAPVTS(), "sustain", sustainSlider);

    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setColour(juce::Label::textColourId, Constants::Colors::TEXT_PRIMARY);
    addAndMakeVisible(releaseLabel);

    releaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(releaseSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.getAPVTS(), "release", releaseSlider);

    // Clip to Zero controls
    addAndMakeVisible(clipEnabledButton);
    clipEnabledButton.setColour(juce::ToggleButton::textColourId, Constants::Colors::TEXT_PRIMARY);
    clipEnabledAttachment = std::make_unique<ButtonAttachment>(
        audioProcessor.getAPVTS(), "clip_enabled", clipEnabledButton);

    clipAmountLabel.setText("Clip Amount", juce::dontSendNotification);
    clipAmountLabel.setColour(juce::Label::textColourId, Constants::Colors::TEXT_PRIMARY);
    addAndMakeVisible(clipAmountLabel);

    clipAmountSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    clipAmountSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(clipAmountSlider);
    clipAmountAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.getAPVTS(), "clip_amount", clipAmountSlider);

    // Start timer for UI updates (60 fps)
    startTimerHz(Constants::UI::UI_REFRESH_RATE_HZ);
}

NineZeroNineAudioProcessorEditor::~NineZeroNineAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void NineZeroNineAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill background with dark color
    g.fillAll (Constants::Colors::BACKGROUND);

    // Draw title
    g.setColour (Constants::Colors::TEXT_PRIMARY);
    g.setFont (24.0f);
    g.drawFittedText ("909 Kick Builder", 0, 10, getWidth(), 30, juce::Justification::centred, 1);
}

void NineZeroNineAudioProcessorEditor::resized()
{
    const int margin = Constants::UI::MARGIN;
    const int knobSize = Constants::UI::KNOB_SIZE;

    auto area = getLocalBounds().reduced(margin);

    // Title area
    area.removeFromTop(40);

    // Waveform display (top section)
    waveformDisplay.setBounds(area.removeFromTop(Constants::UI::WAVEFORM_HEIGHT).reduced(margin / 2));

    area.removeFromTop(margin);

    // Engine mode selector
    auto engineArea = area.removeFromTop(40);
    engineModeLabel.setBounds(engineArea.removeFromLeft(100));
    engineModeCombo.setBounds(engineArea.removeFromLeft(150));

    area.removeFromTop(margin);

    // ADSR controls (row of 4 knobs)
    auto adsrArea = area.removeFromTop(knobSize + 40);
    const int knobSpacing = adsrArea.getWidth() / 4;

    auto attackArea = adsrArea.removeFromLeft(knobSpacing);
    attackLabel.setBounds(attackArea.removeFromTop(20));
    attackSlider.setBounds(attackArea.withSizeKeepingCentre(knobSize, knobSize));

    auto decayArea = adsrArea.removeFromLeft(knobSpacing);
    decayLabel.setBounds(decayArea.removeFromTop(20));
    decaySlider.setBounds(decayArea.withSizeKeepingCentre(knobSize, knobSize));

    auto sustainArea = adsrArea.removeFromLeft(knobSpacing);
    sustainLabel.setBounds(sustainArea.removeFromTop(20));
    sustainSlider.setBounds(sustainArea.withSizeKeepingCentre(knobSize, knobSize));

    auto releaseArea = adsrArea.removeFromLeft(knobSpacing);
    releaseLabel.setBounds(releaseArea.removeFromTop(20));
    releaseSlider.setBounds(releaseArea.withSizeKeepingCentre(knobSize, knobSize));

    area.removeFromTop(margin);

    // ADSR Display
    adsrDisplay.setBounds(area.removeFromTop(Constants::UI::ADSR_DISPLAY_HEIGHT).reduced(margin / 2));

    area.removeFromTop(margin);

    // Clip to Zero section
    auto clipArea = area.removeFromTop(knobSize + 40);
    clipEnabledButton.setBounds(clipArea.removeFromLeft(150).removeFromTop(30));
    clipAmountLabel.setBounds(clipArea.removeFromTop(20).removeFromLeft(150));
    clipAmountSlider.setBounds(clipArea.withSizeKeepingCentre(knobSize, knobSize));

    area.removeFromTop(margin);

    // Spectrum Analyzer (bottom)
    if (area.getHeight() > 0)
        spectrumAnalyzer.setBounds(area.reduced(margin / 2));
}

void NineZeroNineAudioProcessorEditor::timerCallback()
{
    // Update waveform display
    juce::AudioBuffer<float> tempBuffer;
    tempBuffer.setSize(2, 4096);
    audioProcessor.getLatestWaveform(tempBuffer);
    waveformDisplay.setBuffer(tempBuffer);

    // Update metering, etc.
    repaint();
}
