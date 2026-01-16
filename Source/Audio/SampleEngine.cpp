/*
  ==============================================================================

    SampleEngine.cpp
    Sample playback engine for 909 kicks

  ==============================================================================
*/

#include "SampleEngine.h"

SampleEngine::SampleEngine()
{
    loadSamples();
}

SampleEngine::~SampleEngine()
{
}

void SampleEngine::prepare(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
}

void SampleEngine::loadSamples()
{
    // Load samples from BinaryData (embedded in the plugin)
    // For now, this is a placeholder - samples will be loaded via JUCE's BinaryData
    // which is generated from the Assets folder

    // Note: The actual sample loading will be implemented once we integrate BinaryData
    // For testing, we'll generate simple sine wave kicks

    const int sampleLength = static_cast<int>(48000 * 0.6); // 600ms at 48kHz

    for (size_t i = 0; i < static_cast<size_t>(Constants::SampleID::NumSamples); ++i)
    {
        samples[i].setSize(1, sampleLength);
        samples[i].clear();

        // Generate a simple kick sound (sine wave with pitch decay)
        float* data = samples[i].getWritePointer(0);
        float baseFreq = 50.0f + (i * 5.0f); // Vary frequency slightly per sample

        for (int s = 0; s < sampleLength; ++s)
        {
            float t = static_cast<float>(s) / 48000.0f;
            float freq = baseFreq + (200.0f * std::exp(-15.0f * t));
            float phase = freq * t * Constants::DSP::TWO_PI;
            float env = std::exp(-6.0f * t);

            data[s] = 0.5f * std::sin(phase) * env;
        }
    }
}

void SampleEngine::trigger(int velocityValue)
{
    isPlaying = true;
    playbackPosition = 0;
    velocity = velocityValue / 127.0f;
}

void SampleEngine::setCurrentSample(Constants::SampleID sampleID)
{
    currentSampleID = sampleID;
}

void SampleEngine::renderNextBlock(juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    if (!isPlaying)
        return;

    const auto& currentSample = samples[static_cast<size_t>(currentSampleID)];
    const int sampleLength = currentSample.getNumSamples();
    const int fadeInSamples = 64; // Short fade-in to prevent clicks

    for (int i = 0; i < numSamples; ++i)
    {
        if (playbackPosition >= sampleLength)
        {
            isPlaying = false;
            break;
        }

        float sample = currentSample.getSample(0, playbackPosition) * velocity;

        // Apply short fade-in to prevent clicks at the start
        if (playbackPosition < fadeInSamples)
        {
            const float fadeGain = static_cast<float>(playbackPosition) / static_cast<float>(fadeInSamples);
            sample *= fadeGain;
        }

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            buffer.addSample(ch, startSample + i, sample);
        }

        ++playbackPosition;
    }
}
