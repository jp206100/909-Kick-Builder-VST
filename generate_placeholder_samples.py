#!/usr/bin/env python3
"""
Generate placeholder 909 kick drum samples for testing.
These are simple synthesized kicks that approximate the character of real 909 samples.
"""

import numpy as np
import wave
import struct

def generate_kick(filename, fundamental_freq, pitch_decay, noise_amount, length_ms, sample_rate=48000):
    """
    Generate a synthesized kick drum sample.

    Args:
        filename: Output WAV filename
        fundamental_freq: Starting frequency of the kick (Hz)
        pitch_decay: How fast the pitch drops (higher = faster)
        noise_amount: Amount of click/beater noise (0-1)
        length_ms: Length of sample in milliseconds
        sample_rate: Sample rate (default 48000)
    """
    num_samples = int((length_ms / 1000.0) * sample_rate)
    samples = np.zeros(num_samples)

    # Time array
    t = np.linspace(0, length_ms / 1000.0, num_samples)

    # Pitch envelope: exponentially decaying frequency
    pitch_envelope = fundamental_freq * np.exp(-pitch_decay * t)

    # Generate sine wave body with pitch envelope
    phase = np.cumsum(2 * np.pi * pitch_envelope / sample_rate)
    body = np.sin(phase)

    # Amplitude envelope: fast attack, exponential decay
    attack_samples = int(0.001 * sample_rate)  # 1ms attack
    decay_rate = 8.0  # Decay speed

    amplitude_env = np.zeros(num_samples)
    amplitude_env[:attack_samples] = np.linspace(0, 1, attack_samples)
    amplitude_env[attack_samples:] = np.exp(-decay_rate * t[attack_samples:])

    # Apply amplitude envelope to body
    samples = body * amplitude_env

    # Add click/beater noise if specified
    if noise_amount > 0:
        noise = np.random.uniform(-1, 1, num_samples)

        # Short noise burst at the beginning
        noise_env = np.zeros(num_samples)
        noise_length = int(0.01 * sample_rate)  # 10ms of noise
        noise_env[:noise_length] = np.exp(-50 * t[:noise_length])

        click = noise * noise_env * noise_amount
        samples += click

    # Normalize to -6dBFS (leave headroom)
    peak = np.max(np.abs(samples))
    if peak > 0:
        target_peak = 0.501  # -6dBFS ≈ 0.5
        samples = samples * (target_peak / peak)

    # Ensure we start at zero crossing
    if samples[0] != 0:
        # Fade in very quickly from zero
        fade_samples = min(10, len(samples))
        fade_in = np.linspace(0, 1, fade_samples)
        samples[:fade_samples] *= fade_in

    # Convert to 24-bit integer format
    samples_int = np.int32(samples * (2**23 - 1))

    # Write WAV file
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)  # Mono
        wav_file.setsampwidth(3)  # 24-bit (3 bytes)
        wav_file.setframerate(sample_rate)

        # Pack as 24-bit (3 bytes per sample)
        for sample in samples_int:
            # Convert numpy.int32 to Python int, then to 24-bit little-endian
            sample_int = int(sample)
            bytes_data = sample_int.to_bytes(4, byteorder='little', signed=True)[:3]
            wav_file.writeframes(bytes_data)

    print(f"Generated: {filename}")


def main():
    """Generate all 5 placeholder kick samples."""

    output_dir = "Assets/Samples"

    # 1. Classic 909 - balanced, iconic sound
    generate_kick(
        filename=f"{output_dir}/909_classic.wav",
        fundamental_freq=60,      # 60 Hz fundamental
        pitch_decay=15.0,          # Medium pitch drop
        noise_amount=0.3,          # Moderate click
        length_ms=600
    )

    # 2. Deep - lower tuning, more sub
    generate_kick(
        filename=f"{output_dir}/909_deep.wav",
        fundamental_freq=45,       # Lower fundamental
        pitch_decay=10.0,          # Slower pitch drop
        noise_amount=0.15,         # Less click
        length_ms=800              # Longer decay
    )

    # 3. Punchy - strong transient, tight
    generate_kick(
        filename=f"{output_dir}/909_punchy.wav",
        fundamental_freq=65,       # Slightly higher
        pitch_decay=25.0,          # Faster pitch drop
        noise_amount=0.5,          # More click
        length_ms=400              # Shorter, tighter
    )

    # 4. Vintage - analog character
    generate_kick(
        filename=f"{output_dir}/909_vintage.wav",
        fundamental_freq=55,       # Classic tuning
        pitch_decay=12.0,          # Moderate decay
        noise_amount=0.25,         # Moderate click
        length_ms=650
    )

    # 5. Modern - processed, club-ready
    generate_kick(
        filename=f"{output_dir}/909_modern.wav",
        fundamental_freq=58,       # Modern tuning
        pitch_decay=18.0,          # Snappy pitch drop
        noise_amount=0.4,          # Defined click
        length_ms=500
    )

    print("\n✓ All placeholder samples generated successfully!")
    print("These are temporary placeholders. Replace with real 909 samples later.")


if __name__ == "__main__":
    main()
