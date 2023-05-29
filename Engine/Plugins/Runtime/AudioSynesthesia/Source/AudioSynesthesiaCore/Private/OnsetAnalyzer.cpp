// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnsetAnalyzer.h"
#include "PeakPicker.h"
#include "DSP/FloatArrayMath.h"

namespace Audio
{
	FOnsetStrengthAnalyzer::FOnsetStrengthAnalyzer(const FOnsetStrengthSettings& InSettings, float InSampleRate)
	:	Settings(InSettings)
	,	LagSpectraIndex(0)
	,	SlidingBuffer(InSettings.NumWindowFrames, InSettings.NumHopFrames)
	,	Window(Settings.WindowType, Settings.NumWindowFrames, 1, false)
	{
		// Check values legit.
		check(InSampleRate > 0.f);
		check(Settings.FFTSize > 0);
		check(Settings.NumHopFrames > 0);
		check(Settings.NumWindowFrames > 0);
		check(Settings.ComparisonLag > 0);
		check(Settings.MelSettings.NumBands > 0);
		check(Settings.NumWindowFrames <= Settings.FFTSize);

		// intialize buffers dealing with audio window
		WorkingBuffer.AddUninitialized(Settings.NumWindowFrames);

		// initialize buffers dealing with fft input / output
		WindowedSamples.AddZeroed(Settings.FFTSize);
		FFTOutputRealData.AddUninitialized(Settings.FFTSize);
		FFTOutputImagData.AddUninitialized(Settings.FFTSize);
		FFTSpectrumBuffer.AddUninitialized(Settings.FFTSize);
		
		// intialize buffers dealing with perceptual spectrum 
		PreviousSpectra.AddDefaulted(Settings.ComparisonLag);
		MelSpectrum.AddUninitialized(Settings.MelSettings.NumBands);
		SpectrumDifference.AddUninitialized(Settings.MelSettings.NumBands);

		// create mel spectrum transform
		MelTransform = NewMelSpectrumKernelTransform(Settings.MelSettings, Settings.FFTSize, InSampleRate);

		check(MelTransform.IsValid());
	}

	void FOnsetStrengthAnalyzer::CalculateOnsetStrengths(TArrayView<const float> InSamples, TArray<float>& OutEnvelopeStrengths)
	{
		const bool bDoFlush = false;
		AnalyzeAudio(InSamples, OutEnvelopeStrengths, bDoFlush);
	}

	void FOnsetStrengthAnalyzer::FlushAudio(TArray<float>& OutEnvelopeStrengths)
	{
		TArray<float> EmptyArray;

		// Force flushing of internal audio buffer
		const bool bDoFlush = true;

		AnalyzeAudio(EmptyArray, OutEnvelopeStrengths, bDoFlush);
	}
	

	void FOnsetStrengthAnalyzer::Reset()
	{
		// Reset internal memory
		for (TArray<float>& Spectrum : PreviousSpectra)
		{
			Spectrum.Reset();
		}
		SlidingBuffer.Reset();
	}

	float FOnsetStrengthAnalyzer::GetTimestampForIndex(const FOnsetStrengthSettings& InSettings, float InSampleRate, int32 InIndex)
	{
		check(InSampleRate > 0.f);

		// Getting the exact timestamp of an onset from the onset strength envelope is non-trivial because
		// the onset strength envelope changes depending on the attack, decay and sustain of the onset as well
		// as the choice of window, window size, etc. etc. 
		//
		// This approach does not attempt to account for all the subtle variability than can come up, but 
		// uses a generally sane approach of estimating the timestamp to be somewhere between the center of
		// the "loud" window and the end of the "quiet" window.
		const int32 WindowOffset = InSettings.NumWindowFrames / 2;

		const int32 LoudFrameIndex = (InIndex * InSettings.NumHopFrames) + WindowOffset;

		// Get the end of the window to which this was compared.
		const int32 QuietIndex = InIndex - InSettings.ComparisonLag;
		const int32 QuietFrameIndex = (QuietIndex * InSettings.NumHopFrames) + InSettings.NumWindowFrames;

		const int32 EstimatedOnsetFrameIndex = (LoudFrameIndex + QuietFrameIndex) / 2;

		return static_cast<float>(EstimatedOnsetFrameIndex) / InSampleRate;
	}

	void FOnsetStrengthAnalyzer::AnalyzeAudio(TArrayView<const float> InSamples, TArray<float>& OutEnvelopeStrengths, bool bDoFlush)
	{
		typedef TAutoSlidingWindow<float, FAudioBufferAlignedAllocator> FSlidingWindow;

		// Going to fill OutEnvelopeStrengths with envelope strength values generated during this call.
		OutEnvelopeStrengths.Reset();

		// Slide over audio 
		FSlidingWindow SlidingWindow(SlidingBuffer, InSamples, WorkingBuffer, bDoFlush);
		for (const AlignedFloatBuffer& WindowBuffer : SlidingWindow)
		{
			// Calculate onset strength per audio buffer.
			float OnsetStrength = GetNextOnsetStrength(WindowBuffer);

			OutEnvelopeStrengths.Add(OnsetStrength);
		}
	}

	float FOnsetStrengthAnalyzer::GetNextOnsetStrength(TArrayView<const float> InSamples)
	{
		float OnsetStrength = 0.f;

		// Copy input samples and apply window
		FMemory::Memcpy(WindowedSamples.GetData(), InSamples.GetData(), sizeof(float) * Settings.NumWindowFrames);
		Window.ApplyToBuffer(WindowedSamples.GetData());

		// Take FFT of windowed samples
		const FFTTimeDomainData TimeData = {WindowedSamples.GetData(), Settings.FFTSize};
		FFTFreqDomainData FreqData = {FFTOutputRealData.GetData(), FFTOutputImagData.GetData()};

		PerformFFT(TimeData, FreqData);

		// Calculate spectrum from FFT output
		ComputePowerSpectrum(FreqData, Settings.FFTSize, FFTSpectrumBuffer);
		
		// Convert spectrum to CQT
		MelTransform->TransformArray(FFTSpectrumBuffer, MelSpectrum);

		// Apply decibel scaling 
		ArrayPowerToDecibelInPlace(MelSpectrum, -90.f);

		// Clamp to noise floor
		ArrayClampMinInPlace(MelSpectrum, Settings.NoiseFloorDb);

		// Onset strength is mean(abs(diff(spectrum - previous_spectrum)))
		TArray<float>& PreviousSpectrum = PreviousSpectra[LagSpectraIndex];
		if (PreviousSpectrum.Num() > 0)
		{
			ArraySubtract(MelSpectrum, PreviousSpectrum, SpectrumDifference);

			// Half wave rectify
			ArrayClampMinInPlace(SpectrumDifference, 0.f);

			ArrayMean(SpectrumDifference, OnsetStrength);
		}

		// Save this spectrum for later use
		PreviousSpectrum.Reset(Settings.MelSettings.NumBands);
		PreviousSpectrum.AddUninitialized(Settings.MelSettings.NumBands);
		FMemory::Memcpy(PreviousSpectrum.GetData(), MelSpectrum.GetData(), sizeof(float) * Settings.MelSettings.NumBands);

		// Increment index for comparison spectra
		LagSpectraIndex = (LagSpectraIndex + 1) % Settings.ComparisonLag;

		return OnsetStrength;
	}


	/*******************************************************************************/
	/*******************************************************************************/
	/*******************************************************************************/

	void OnsetExtractIndices(const FPeakPickerSettings& InSettings, TArrayView<const float> InOnsetEnvelope, TArray<int32>& OutOnsetIndices)
	{
		const int32 Num = InOnsetEnvelope.Num();
		OutOnsetIndices.Reset();

		if (Num < 1)
		{
			return;
		}
		
		TArray<float> NormOnsets;

		// Onset strength is normalized between [0, 1.0] before picking peaks.
		ArrayMinMaxNormalize(InOnsetEnvelope, NormOnsets);

		FPeakPicker PeakPicker(InSettings);
		PeakPicker.PickPeaks(NormOnsets, OutOnsetIndices);
	}

	/*******************************************************************************/
	/*******************************************************************************/
	/*******************************************************************************/

	void OnsetBacktrackIndices(TArrayView<const float> InOnsetStrength, TArrayView<const int32> InOnsetIndices, TArray<int32>& OutBacktrackedOnsetIndices)
	{
		const int32 NumOnsets = InOnsetIndices.Num();
		const int32 NumOnsetStrength = InOnsetStrength.Num();

		OutBacktrackedOnsetIndices.Reset();
		
		if (NumOnsets < 1)
		{
			return;
		}

		OutBacktrackedOnsetIndices.AddUninitialized(NumOnsets);

		const int32* InOnsetIndicesData = InOnsetIndices.GetData();
		int32* OutOnsetIndicesData = OutBacktrackedOnsetIndices.GetData();
		const float* InOnsetStrengthData = InOnsetStrength.GetData();

		for (int32 i = 0; i < NumOnsets; i++)
		{

			int32 OnsetStartIndex = InOnsetIndicesData[i];
			check(OnsetStartIndex < NumOnsetStrength);

			while (OnsetStartIndex > 0)
			{
				if (InOnsetStrengthData[OnsetStartIndex] <= 0.f)
				{
					// No onset strength means the audio is steady state or silent.
					break;
				}
				else if (InOnsetStrengthData[OnsetStartIndex - 1] <= InOnsetStrengthData[OnsetStartIndex])
				{
					// The onest strength is still holding steady or decreasing.
					OnsetStartIndex--;
				}
				else
				{
					// Onset strength is no longer decreasing.
					break;
				}
			}

			OutOnsetIndicesData[i] = OnsetStartIndex;
		}
	}
}
