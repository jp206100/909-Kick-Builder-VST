/*
  ==============================================================================

    DSPHelpers.h
    Common DSP utility functions

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Constants.h"

namespace DSPHelpers
{
    /**
     * Convert decibels to linear gain
     */
    inline float dbToGain(float db)
    {
        return std::pow(10.0f, db / 20.0f);
    }

    /**
     * Convert linear gain to decibels
     */
    inline float gainToDb(float gain)
    {
        if (gain <= Constants::DSP::EPSILON)
            return Constants::DSP::DB_FLOOR;
        return 20.0f * std::log10(gain);
    }

    /**
     * Convert semitones to frequency ratio
     */
    inline float semitonesToRatio(float semitones)
    {
        return std::pow(2.0f, semitones / 12.0f);
    }

    /**
     * Hard clip a value between -1 and +1
     */
    inline float hardClip(float value)
    {
        return juce::jlimit(-1.0f, 1.0f, value);
    }

    /**
     * Soft clip using tanh saturation
     */
    inline float softClip(float value)
    {
        return std::tanh(value);
    }

    /**
     * Soft clip with adjustable threshold
     */
    inline float softClipWithThreshold(float value, float threshold)
    {
        if (threshold <= Constants::DSP::EPSILON)
            threshold = 1.0f;

        return threshold * std::tanh(value / threshold);
    }

    /**
     * Linear interpolation
     */
    inline float lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    /**
     * Map a value from one range to another
     */
    inline float mapRange(float value, float inMin, float inMax, float outMin, float outMax)
    {
        return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
    }

    /**
     * Get peak absolute value from a buffer
     */
    inline float getPeakLevel(const juce::AudioBuffer<float>& buffer, int channel = 0)
    {
        if (channel >= buffer.getNumChannels())
            return 0.0f;

        return buffer.getMagnitude(channel, 0, buffer.getNumSamples());
    }

    /**
     * Get RMS level from a buffer
     */
    inline float getRMSLevel(const juce::AudioBuffer<float>& buffer, int channel = 0)
    {
        if (channel >= buffer.getNumChannels())
            return 0.0f;

        return buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
    }

    /**
     * Apply fade in to prevent clicks
     */
    inline void applyFadeIn(juce::AudioBuffer<float>& buffer, int numSamplesToFade)
    {
        const int numSamples = juce::jmin(numSamplesToFade, buffer.getNumSamples());

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float* data = buffer.getWritePointer(channel);

            for (int i = 0; i < numSamples; ++i)
            {
                const float gain = static_cast<float>(i) / static_cast<float>(numSamples);
                data[i] *= gain;
            }
        }
    }

    /**
     * Apply fade out to prevent clicks
     */
    inline void applyFadeOut(juce::AudioBuffer<float>& buffer, int numSamplesToFade)
    {
        const int numSamples = buffer.getNumSamples();
        const int fadeStart = numSamples - numSamplesToFade;

        if (fadeStart < 0)
            return;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float* data = buffer.getWritePointer(channel);

            for (int i = fadeStart; i < numSamples; ++i)
            {
                const float gain = 1.0f - static_cast<float>(i - fadeStart) / static_cast<float>(numSamplesToFade);
                data[i] *= gain;
            }
        }
    }

    /**
     * Zero crossing detector
     */
    inline bool isZeroCrossing(float previousSample, float currentSample)
    {
        return (previousSample <= 0.0f && currentSample > 0.0f) ||
               (previousSample >= 0.0f && currentSample < 0.0f);
    }

    /**
     * Simple envelope follower
     */
    class EnvelopeFollower
    {
    public:
        EnvelopeFollower() = default;

        void setCoefficients(float attack, float release, double sampleRate)
        {
            attackCoeff = std::exp(-1.0f / (attack * static_cast<float>(sampleRate)));
            releaseCoeff = std::exp(-1.0f / (release * static_cast<float>(sampleRate)));
        }

        float process(float input)
        {
            const float rectified = std::abs(input);

            if (rectified > envelope)
                envelope = attackCoeff * envelope + (1.0f - attackCoeff) * rectified;
            else
                envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * rectified;

            return envelope;
        }

        void reset()
        {
            envelope = 0.0f;
        }

    private:
        float envelope = 0.0f;
        float attackCoeff = 0.0f;
        float releaseCoeff = 0.0f;
    };

    /**
     * One-pole lowpass filter (simple smoother)
     */
    class OnePoleFilter
    {
    public:
        OnePoleFilter() = default;

        void setCoefficient(float coeff)
        {
            a0 = coeff;
        }

        float process(float input)
        {
            z1 = input * (1.0f - a0) + z1 * a0;
            return z1;
        }

        void reset()
        {
            z1 = 0.0f;
        }

    private:
        float z1 = 0.0f;
        float a0 = 0.9f;
    };
}
