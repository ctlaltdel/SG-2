#include "daisy_seed.h"
#include "daisysp.h"
#include "SG-2.h"
#include <cmath>

using namespace daisy;
using namespace daisysp;

static DaisySeed hw;
Fm2 voice1, voice2;
ReverbSc verb;
Configuration config =
{
	{68, 73}	
};
float string_base_freq[2];

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float string_env_1, string_env_2, pitch_1, pitch_2;

	for (size_t i = 0; i < size; i++)
	{
		string_env_1 = GetEnv(ENV_1); 
		string_env_2 = GetEnv(ENV_2); 
		pitch_1 = GetPitch(PITCH_1);
		pitch_2 = GetPitch(PITCH_2);

	    if(pitch_1==0.f) voice1.Reset(); else voice1.SetFrequency(pitch_1);
	    if(pitch_2==0.f) voice2.Reset(); else voice2.SetFrequency(pitch_2);

		float sig = (voice1.Process() * string_env_1) + (voice2.Process() * string_env_2);
//		out[0][i] = out[1][i] = sig;
		verb.Process(sig, sig, &out[0][i], &out[1][i]);
	}
}

int main(void)
{
	string_base_freq[0] = mtof((float)config.string_base_midi[0]);
	string_base_freq[1] = mtof((float)config.string_base_midi[1]);

	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	float samplerate = hw.AudioSampleRate();

    AdcChannelConfig adc_cfg[4];
    adc_cfg[0].InitSingle(seed::A0);
    adc_cfg[1].InitSingle(seed::A1);
    adc_cfg[2].InitSingle(seed::A2);
    adc_cfg[3].InitSingle(seed::A3);
	hw.adc.Init(adc_cfg, 4);
    hw.adc.Start();
	
    verb.Init(samplerate);
    verb.SetFeedback(0.9f);
    verb.SetLpFreq(1000.0f);

	voice1.Init(samplerate);
	voice2.Init(samplerate);
    voice1.SetIndex(0.3);
    voice1.SetRatio(5);
    voice2.SetIndex(0.3);
    voice2.SetRatio(5);

	hw.StartAudio(AudioCallback);
	while(1) {}
}

float GetPitch(int channel)
{
	float pos, pitch;

	pos = hw.adc.GetFloat(channel);
	if(pos < MIN_POS) pitch = 0.f; else pitch = string_base_freq[channel] * ((B3 * pow(pos, 3)) + (B2 * pow(pos, 2)) + (B1 * pos) + B0);
	return pitch;
}

float GetEnv(int channel)
{
	float env;

	env = hw.adc.GetFloat(channel); 
	if (env < MIN_ENV) return 0.f; else return env;
}