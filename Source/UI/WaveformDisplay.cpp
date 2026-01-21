/*
  ==============================================================================

    WaveformDisplay.cpp
    Vintage CRT oscilloscope-style waveform display with phosphor glow

  ==============================================================================
*/

#include "WaveformDisplay.h"
#include "../Utils/Constants.h"

WaveformDisplay::WaveformDisplay()
{
    displayBuffer.setSize(1, 1024);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Draw CRT bezel (inset frame)
    drawCRTBezel(g, bounds);

    // Get screen area (inside bezel)
    auto screenBounds = bounds.reduced(8.0f);

    // Draw CRT screen background
    g.setColour(Constants::Colors::CRT_BACKGROUND);
    g.fillRoundedRectangle(screenBounds, 4.0f);

    // Draw grid lines (oscilloscope style)
    drawGrid(g, screenBounds);

    // Draw the waveform with phosphor glow
    if (displayBuffer.getNumSamples() > 0)
    {
        drawWaveformWithGlow(g, screenBounds);
    }

    // Draw clip threshold if enabled
    if (clipEnabled && clipAmount > 0.0f)
    {
        drawClipThreshold(g, screenBounds);
    }

    // Draw CRT scanline effect
    drawScanlines(g, screenBounds);

    // Draw screen edge vignette
    drawVignette(g, screenBounds);
}

void WaveformDisplay::drawCRTBezel(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Draw outer bezel (metal frame)
    g.setColour(Constants::Colors::PANEL_DARK_GRAY);
    g.fillRoundedRectangle(bounds, 6.0f);

    // Draw inner shadow for depth
    g.setColour(Constants::Colors::METAL_SHADOW);
    g.drawRoundedRectangle(bounds.reduced(4.0f), 5.0f, 2.0f);

    // Draw highlight edge
    g.setColour(Constants::Colors::METAL_HIGHLIGHT.withAlpha(0.3f));
    juce::Path highlightPath;
    highlightPath.addRoundedRectangle(bounds.getX(), bounds.getY(),
                                     bounds.getWidth(), bounds.getHeight() * 0.3f, 6.0f, 6.0f, true, true, false, false);
    g.fillPath(highlightPath);
}

void WaveformDisplay::drawGrid(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    g.setColour(Constants::Colors::CRT_GRID);

    // Vertical grid lines
    int numVerticalLines = 10;
    for (int i = 1; i < numVerticalLines; ++i)
    {
        float x = bounds.getX() + (bounds.getWidth() * i / numVerticalLines);
        g.drawLine(x, bounds.getY(), x, bounds.getBottom(), 0.5f);
    }

    // Horizontal grid lines
    int numHorizontalLines = 6;
    for (int i = 1; i < numHorizontalLines; ++i)
    {
        float y = bounds.getY() + (bounds.getHeight() * i / numHorizontalLines);
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, 0.5f);
    }

    // Center line (brighter)
    float centerY = bounds.getCentreY();
    g.setColour(Constants::Colors::CRT_GRID.brighter(0.5f));
    g.drawLine(bounds.getX(), centerY, bounds.getRight(), centerY, 1.0f);
}

void WaveformDisplay::drawWaveformWithGlow(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    juce::Path waveformPath;
    const int width = (int)bounds.getWidth();
    const int height = (int)bounds.getHeight();
    const float midY = bounds.getCentreY();

    waveformPath.startNewSubPath(bounds.getX(), midY);

    // Build waveform path
    for (int x = 0; x < width; ++x)
    {
        const int sampleIndex = juce::jmap(x, 0, width, 0, displayBuffer.getNumSamples());
        if (sampleIndex < displayBuffer.getNumSamples())
        {
            const float sample = displayBuffer.getSample(0, sampleIndex);
            const float y = midY - (sample * height * 0.45f);
            waveformPath.lineTo(bounds.getX() + x, y);
        }
    }

    // Draw outer glow (phosphor bloom effect)
    for (int i = 3; i > 0; --i)
    {
        float alpha = 0.1f / i;
        float thickness = 6.0f * i;
        g.setColour(Constants::Colors::CRT_GLOW.withAlpha(alpha));
        g.strokePath(waveformPath, juce::PathStrokeType(thickness));
    }

    // Draw main phosphor trace (bright green)
    g.setColour(Constants::Colors::CRT_PHOSPHOR);
    g.strokePath(waveformPath, juce::PathStrokeType(2.0f));

    // Draw extra bright core
    g.setColour(Constants::Colors::CRT_PHOSPHOR.brighter(0.5f));
    g.strokePath(waveformPath, juce::PathStrokeType(1.0f));
}

void WaveformDisplay::drawScanlines(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Draw horizontal scanlines for CRT effect
    g.setColour(Constants::Colors::METAL_SHADOW.withAlpha(0.15f));

    for (float y = bounds.getY(); y < bounds.getBottom(); y += 3.0f)
    {
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, 1.0f);
    }
}

void WaveformDisplay::drawVignette(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Draw edge vignette for CRT curvature effect
    juce::ColourGradient vignette(
        juce::Colours::transparentBlack, bounds.getCentreX(), bounds.getCentreY(),
        Constants::Colors::METAL_SHADOW.withAlpha(0.4f), bounds.getX(), bounds.getY(),
        true);

    g.setGradientFill(vignette);
    g.fillRoundedRectangle(bounds, 4.0f);
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::setBuffer(const juce::AudioBuffer<float>& buffer)
{
    displayBuffer.makeCopyOf(buffer, true);
    repaint();
}

void WaveformDisplay::drawClipThreshold(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Calculate threshold level from clip amount (0-100% maps to 0dB to -12dB)
    const float thresholdDb = juce::jmap(clipAmount, 0.0f, 100.0f, 0.0f, -12.0f);
    const float threshold = juce::Decibels::decibelsToGain(thresholdDb);

    const float midY = bounds.getCentreY();
    const float height = bounds.getHeight();

    // Calculate Y positions for positive and negative threshold lines
    const float thresholdYPos = midY - (threshold * height * 0.45f);
    const float thresholdYNeg = midY + (threshold * height * 0.45f);

    // Choose color based on whether clipping is occurring
    juce::Colour thresholdColor = clipGainReduction > 0.001f
        ? Constants::Colors::ROLAND_RED         // Red when clipping
        : Constants::Colors::LED_AMBER;          // Amber when enabled but not clipping

    // Draw threshold lines with glow
    g.setColour(thresholdColor.withAlpha(0.3f));
    g.drawLine(bounds.getX(), thresholdYPos, bounds.getRight(), thresholdYPos, 3.0f);
    g.drawLine(bounds.getX(), thresholdYNeg, bounds.getRight(), thresholdYNeg, 3.0f);

    g.setColour(thresholdColor);
    g.drawLine(bounds.getX(), thresholdYPos, bounds.getRight(), thresholdYPos, 1.5f);
    g.drawLine(bounds.getX(), thresholdYNeg, bounds.getRight(), thresholdYNeg, 1.5f);

    // Draw "CLIP" label when clipping is occurring
    if (clipGainReduction > 0.001f)
    {
        g.setColour(Constants::Colors::ROLAND_RED);
        g.setFont(juce::FontOptions(10.0f, juce::Font::bold));
        g.drawText("CLIP", bounds.removeFromTop(15).reduced(4.0f), juce::Justification::topRight);

        // Draw gain reduction amount
        juce::String grText = juce::String(clipGainReduction * 100.0f, 1) + "%";
        g.setFont(juce::FontOptions(9.0f, juce::Font::plain));
        g.drawText(grText, bounds.removeFromTop(12).reduced(4.0f), juce::Justification::topRight);
    }
}

void WaveformDisplay::setClipEnabled(bool enabled)
{
    clipEnabled = enabled;
    repaint();
}

void WaveformDisplay::setClipAmount(float amount)
{
    clipAmount = amount;
    repaint();
}

void WaveformDisplay::setClipGainReduction(float gainReductionDb)
{
    clipGainReduction = gainReductionDb;
    repaint();
}
