/*
  ==============================================================================

    SynthEngine.cpp
    909-style synthesis engine

  ==============================================================================
*/

#include "SynthEngine.h"

SynthEngine::SynthEngine()
{
}

SynthEngine::~SynthEngine()
{
}

void SynthEngine::prepare(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
}

void SynthEngine::trigger(int velocityValue)
{
    isPlaying = true;
    samplePosition = 0;
    bodyPhase = 0.0f;
    velocity = velocityValue / 127.0f;
}

void SynthEngine::setBodyPitch(float frequency)
{
    bodyPitch = frequency;
}

void SynthEngine::setClickPitch(float frequency)
{
    clickPitch = frequency;
}

void SynthEngine::setNoiseAmount(float amount)
{
    noiseAmount = juce::jlimit(0.0f, 1.0f, amount);
}

void SynthEngine::setPitchEnvDepth(float depth)
{
    pitchEnvDepth = depth;
}

void SynthEngine::setPitchEnvTime(float time)
{
    pitchEnvTime = time;
}

void SynthEngine::renderNextBlock(juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    if (!isPlaying)
        return;

    const float kickLength = 1.0f; // seconds
    const int maxSamples = static_cast<int>(kickLength * sampleRate);

    for (int i = 0; i < numSamples; ++i)
    {
        if (samplePosition >= maxSamples)
        {
            isPlaying = false;
            break;
        }

        const float t = samplePosition / static_cast<float>(sampleRate);
        const float pitchEnvDecayRate = 1000.0f / pitchEnvTime; // Convert ms to decay rate

        // Pitch envelope: exponential decay
        const float pitchEnvValue = std::exp(-pitchEnvDecayRate * t);
        const float pitchMultiplier = 1.0f + (pitchEnvDepth * pitchEnvValue);
        const float currentPitch = bodyPitch * pitchMultiplier;

        // Body: sine wave oscillator
        const float phaseDelta = currentPitch / static_cast<float>(sampleRate);
        bodyPhase += phaseDelta;
        if (bodyPhase >= 1.0f)
            bodyPhase -= 1.0f;

        const float bodySample = std::sin(bodyPhase * Constants::DSP::TWO_PI);

        // Click: band-limited noise
        const float noiseSample = random.nextFloat() * 2.0f - 1.0f;
        const float noiseEnv = std::exp(-50.0f * t); // Fast decay for click
        const float clickSample = noiseSample * noiseEnv * noiseAmount;

        // Amplitude envelope: fast attack, exponential decay
        const float ampEnv = std::exp(-5.0f * t);

        // Mix and apply amplitude envelope
        const float outputSample = (bodySample + clickSample) * ampEnv * velocity * 0.5f;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            buffer.addSample(ch, startSample + i, outputSample);
        }

        ++samplePosition;
    }
}
