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
    setLookAndFeel(&vintageLookAndFeel);

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
    auto bounds = getLocalBounds();

    // Fill background
    g.fillAll(Constants::Colors::BACKGROUND);

    // Draw main hardware panel
    drawHardwarePanel(g, bounds.toFloat());

    // Draw panel screws in corners
    drawPanelScrews(g, bounds.toFloat());

    // Draw title plate
    drawTitlePlate(g, bounds.removeFromTop(60).toFloat());
}

void NineZeroNineAudioProcessorEditor::drawHardwarePanel(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Main panel with brushed metal gradient
    juce::ColourGradient metalGradient(
        Constants::Colors::PANEL_MEDIUM_GRAY, bounds.getCentreX(), bounds.getY(),
        Constants::Colors::PANEL_DARK_GRAY, bounds.getCentreX(), bounds.getBottom(),
        false);
    metalGradient.addColour(0.3, Constants::Colors::PANEL_LIGHT_GRAY);

    g.setGradientFill(metalGradient);
    g.fillRect(bounds);

    // Add subtle noise texture for brushed metal effect
    juce::Random random(12345);
    for (int i = 0; i < 1000; ++i)
    {
        float x = random.nextFloat() * bounds.getWidth();
        float y = random.nextFloat() * bounds.getHeight();
        float brightness = random.nextFloat() * 0.1f - 0.05f;
        g.setColour(Constants::Colors::PANEL_MEDIUM_GRAY.brighter(brightness));
        g.fillRect(x, y, 2.0f, 1.0f);
    }
}

void NineZeroNineAudioProcessorEditor::drawPanelScrews(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    float screwSize = 12.0f;
    float margin = 12.0f;

    juce::Point<float> corners[] = {
        {bounds.getX() + margin, bounds.getY() + margin},
        {bounds.getRight() - margin - screwSize, bounds.getY() + margin},
        {bounds.getX() + margin, bounds.getBottom() - margin - screwSize},
        {bounds.getRight() - margin - screwSize, bounds.getBottom() - margin - screwSize}
    };

    for (auto corner : corners)
    {
        // Screw head shadow
        g.setColour(Constants::Colors::METAL_SHADOW);
        g.fillEllipse(corner.x + 1, corner.y + 1, screwSize, screwSize);

        // Screw head body
        juce::ColourGradient screwGradient(
            Constants::Colors::SCREW_HEAD.brighter(0.2f), corner.x + screwSize * 0.3f, corner.y + screwSize * 0.3f,
            Constants::Colors::SCREW_HEAD.darker(0.2f), corner.x + screwSize * 0.7f, corner.y + screwSize * 0.7f,
            true);
        g.setGradientFill(screwGradient);
        g.fillEllipse(corner.x, corner.y, screwSize, screwSize);

        // Phillips head cross
        g.setColour(Constants::Colors::METAL_SHADOW);
        float crossSize = screwSize * 0.6f;
        float crossThickness = 1.5f;
        juce::Point<float> center(corner.x + screwSize * 0.5f, corner.y + screwSize * 0.5f);
        g.drawLine(center.x - crossSize * 0.5f, center.y, center.x + crossSize * 0.5f, center.y, crossThickness);
        g.drawLine(center.x, center.y - crossSize * 0.5f, center.x, center.y + crossSize * 0.5f, crossThickness);
    }
}

void NineZeroNineAudioProcessorEditor::drawTitlePlate(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    bounds = bounds.reduced(16.0f, 8.0f);

    // Roland-style red accent bar
    auto accentBar = bounds.removeFromTop(6.0f);
    g.setColour(Constants::Colors::ROLAND_RED);
    g.fillRoundedRectangle(accentBar, 2.0f);

    // Add highlight to red bar
    g.setColour(Constants::Colors::ROLAND_RED_LIGHT);
    g.fillRoundedRectangle(accentBar.removeFromTop(2.0f), 1.0f);

    bounds.removeFromTop(4.0f);

    // Draw main title with engraved effect
    juce::String title = "TR-909 KICK BUILDER";

    // Shadow (engraved)
    g.setColour(Constants::Colors::TEXT_ENGRAVED);
    g.setFont(juce::Font("Arial", 28.0f, juce::Font::bold));
    g.drawText(title, bounds.translated(0, 2), juce::Justification::centred);

    // Main text
    g.setColour(Constants::Colors::TEXT_STENCIL);
    g.drawText(title, bounds, juce::Justification::centred);
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

    // Button on the left
    auto buttonArea = clipArea.removeFromLeft(150);
    clipEnabledButton.setBounds(buttonArea.removeFromTop(30));

    // Label and knob centered in remaining space
    auto knobArea = clipArea.withSizeKeepingCentre(knobSize + 40, knobSize + 40);
    clipAmountLabel.setBounds(knobArea.removeFromTop(20));
    clipAmountSlider.setBounds(knobArea.withSizeKeepingCentre(knobSize, knobSize));

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

    // Update waveform display clip visualization
    waveformDisplay.setClipEnabled(audioProcessor.getAPVTS().getRawParameterValue("clip_enabled")->load() > 0.5f);
    waveformDisplay.setClipAmount(audioProcessor.getAPVTS().getRawParameterValue("clip_amount")->load());
    waveformDisplay.setClipGainReduction(audioProcessor.getClipGainReduction());

    // Update ADSR display with current parameter values
    adsrDisplay.setAttack(audioProcessor.getAPVTS().getRawParameterValue("attack")->load());
    adsrDisplay.setDecay(audioProcessor.getAPVTS().getRawParameterValue("decay")->load());
    adsrDisplay.setSustain(audioProcessor.getAPVTS().getRawParameterValue("sustain")->load());
    adsrDisplay.setRelease(audioProcessor.getAPVTS().getRawParameterValue("release")->load());

    // Update metering, etc.
    repaint();
}
