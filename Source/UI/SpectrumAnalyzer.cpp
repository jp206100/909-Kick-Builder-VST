/*
  ==============================================================================

    SpectrumAnalyzer.cpp
    Vintage VU meter-style spectrum analyzer

  ==============================================================================
*/

#include "SpectrumAnalyzer.h"
#include "../Utils/Constants.h"

SpectrumAnalyzer::SpectrumAnalyzer()
{
    // Initialize spectrum data
    for (int i = 0; i < numBands; ++i)
    {
        bandLevels[i] = 0.0f;
        peakHoldLevels[i] = 0.0f;
        peakHoldTimers[i] = 0;
    }
}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
}

void SpectrumAnalyzer::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Draw VU meter bezel
    drawMeterBezel(g, bounds);

    // Get meter area (inside bezel)
    auto meterBounds = bounds.reduced(8.0f);

    // Draw meter background
    g.setColour(Constants::Colors::METER_BACKGROUND);
    g.fillRoundedRectangle(meterBounds, 3.0f);

    // Draw frequency bands
    drawFrequencyBands(g, meterBounds);

    // Draw scale markings
    drawScaleMarkings(g, meterBounds);
}

void SpectrumAnalyzer::drawMeterBezel(juce::Graphics& g, juce::Rectangle<float> bounds)
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
                                     bounds.getWidth(), bounds.getHeight() * 0.3f,
                                     6.0f, 6.0f, true, true, false, false);
    g.fillPath(highlightPath);
}

void SpectrumAnalyzer::drawFrequencyBands(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    float bandWidth = bounds.getWidth() / numBands;
    float spacing = 2.0f;

    for (int i = 0; i < numBands; ++i)
    {
        float x = bounds.getX() + (i * bandWidth);
        float barWidth = bandWidth - spacing;

        // Calculate bar height based on level
        float level = bandLevels[i];
        float barHeight = level * bounds.getHeight();

        // Draw band background (slot)
        g.setColour(Constants::Colors::METAL_SHADOW.withAlpha(0.3f));
        g.fillRoundedRectangle(x, bounds.getY(), barWidth, bounds.getHeight(), 2.0f);

        // Draw level bar with color gradient
        if (barHeight > 0.0f)
        {
            float barY = bounds.getBottom() - barHeight;

            // Determine color based on level
            juce::Colour barColour;
            if (level < 0.7f)
                barColour = Constants::Colors::METER_GREEN;
            else if (level < 0.9f)
                barColour = Constants::Colors::METER_YELLOW;
            else
                barColour = Constants::Colors::METER_RED;

            // Draw bar with gradient
            juce::ColourGradient gradient(
                barColour.brighter(0.3f), x + barWidth * 0.5f, barY,
                barColour.darker(0.2f), x + barWidth * 0.5f, bounds.getBottom(),
                false);

            g.setGradientFill(gradient);
            g.fillRoundedRectangle(x, barY, barWidth, barHeight, 2.0f);

            // Draw LED segments
            drawLEDSegments(g, juce::Rectangle<float>(x, barY, barWidth, barHeight), barColour);
        }

        // Draw peak hold indicator
        float peakLevel = peakHoldLevels[i];
        if (peakLevel > 0.0f)
        {
            float peakY = bounds.getBottom() - (peakLevel * bounds.getHeight());

            juce::Colour peakColour = peakLevel > 0.9f ? Constants::Colors::METER_RED
                                                        : Constants::Colors::METER_YELLOW;
            g.setColour(peakColour);
            g.fillRect(x, peakY - 2.0f, barWidth, 3.0f);
        }
    }
}

void SpectrumAnalyzer::drawLEDSegments(juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour colour)
{
    // Draw horizontal lines to simulate LED segments
    int numSegments = 12;
    float segmentHeight = bounds.getHeight() / numSegments;

    g.setColour(Constants::Colors::METAL_SHADOW.withAlpha(0.5f));

    for (int i = 1; i < numSegments; ++i)
    {
        float y = bounds.getBottom() - (i * segmentHeight);
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, 1.0f);
    }
}

void SpectrumAnalyzer::drawScaleMarkings(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Draw dB scale on the right side
    auto scaleBounds = bounds.removeFromRight(30.0f);

    g.setColour(Constants::Colors::TEXT_SECONDARY);
    g.setFont(juce::Font("Arial", 9.0f, juce::Font::plain));

    // Draw scale markers
    const char* labels[] = {"0", "-6", "-12", "-18", "-24", "-inf"};
    float positions[] = {0.0f, 0.3f, 0.5f, 0.7f, 0.85f, 1.0f};

    for (int i = 0; i < 6; ++i)
    {
        float y = scaleBounds.getY() + (positions[i] * scaleBounds.getHeight());
        g.drawText(labels[i], scaleBounds.getX(), y - 6.0f,
                  scaleBounds.getWidth(), 12.0f, juce::Justification::centredLeft);

        // Draw tick mark
        g.drawLine(bounds.getRight() - 5.0f, y, bounds.getRight(), y, 1.0f);
    }
}

void SpectrumAnalyzer::resized()
{
}

void SpectrumAnalyzer::updateSpectrum(const float* magnitudes, int numMagnitudes)
{
    // Map FFT bins to frequency bands
    for (int i = 0; i < numBands; ++i)
    {
        int binStart = (i * numMagnitudes) / numBands;
        int binEnd = ((i + 1) * numMagnitudes) / numBands;

        float maxMag = 0.0f;
        for (int bin = binStart; bin < binEnd && bin < numMagnitudes; ++bin)
        {
            maxMag = juce::jmax(maxMag, magnitudes[bin]);
        }

        // Smooth the level changes
        float targetLevel = juce::jlimit(0.0f, 1.0f, maxMag);
        bandLevels[i] = bandLevels[i] * 0.7f + targetLevel * 0.3f;

        // Update peak hold
        if (bandLevels[i] > peakHoldLevels[i])
        {
            peakHoldLevels[i] = bandLevels[i];
            peakHoldTimers[i] = 60; // Hold for 60 frames (~1 second at 60fps)
        }
        else if (peakHoldTimers[i] > 0)
        {
            peakHoldTimers[i]--;
        }
        else
        {
            peakHoldLevels[i] *= 0.95f; // Slowly decay peak hold
        }
    }

    repaint();
}
