/*
  ==============================================================================

    ClipToZero.cpp

  ==============================================================================
*/

#include "ClipToZero.h"
#include "../Utils/DSPHelpers.h"

ClipToZero::ClipToZero() {}
ClipToZero::~ClipToZero() {}

void ClipToZero::prepare(double sampleRate, int samplesPerBlock) {}

void ClipToZero::process(juce::AudioBuffer<float>& buffer)
{
    if (!isEnabled || clipAmount <= 0.0f)
    {
        gainReduction = 0.0f;
        return;
    }

    // Map amount (0-100%) to threshold (-12dB to 0dB)
    const float thresholdDb = juce::jmap(clipAmount, 0.0f, 100.0f, 0.0f, -12.0f);
    const float threshold = juce::Decibels::decibelsToGain(thresholdDb);

    float maxGainReduction = 0.0f;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        float* data = buffer.getWritePointer(ch);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const float inputLevel = std::abs(data[i]);

            if (clipMode == Constants::ClipMode::Hard)
            {
                data[i] = juce::jlimit(-threshold, threshold, data[i]);
            }
            else // Soft clip
            {
                data[i] = DSPHelpers::softClipWithThreshold(data[i], threshold);
            }

            // Measure gain reduction
            if (inputLevel > threshold)
            {
                const float reduction = inputLevel - threshold;
                maxGainReduction = std::max(maxGainReduction, reduction);
            }
        }
    }

    gainReduction = maxGainReduction;

    // Auto makeup gain
    if (autoGainEnabled && gainReduction > 0.0f)
    {
        const float makeupGain = 1.0f + gainReduction;
        buffer.applyGain(makeupGain);
    }
}

void ClipToZero::reset()
{
    gainReduction = 0.0f;
}

void ClipToZero::setEnabled(bool enabled)
{
    isEnabled = enabled;
}

void ClipToZero::setMode(Constants::ClipMode mode)
{
    clipMode = mode;
}

void ClipToZero::setAmount(float amount)
{
    clipAmount = juce::jlimit(0.0f, 100.0f, amount);
}

void ClipToZero::setAutoGain(bool enabled)
{
    autoGainEnabled = enabled;
}
