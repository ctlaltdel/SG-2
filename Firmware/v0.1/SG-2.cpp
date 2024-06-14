#include "daisy_seed.h"
#include "daisysp.h"
#include "SG-2.h"
#include <cmath>

using namespace daisy;
using namespace daisysp;

static DaisySeed hw;
Chorus chorus;
static MoogLadder flt1, flt2;
static VariableSawOscillator voice1, voice2;
ReverbSc verb;
Configuration config =
{
	{56, 61}	
};
float string_base_freq[2];
bool env_hyst_flag[2];
GPIO pitch_gate[2]; 

float flt1_cutoff, flt2_cutoff;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float sig1=0.f, sig2=0.f, string_env_1, string_env_2, pitch_1, pitch_2;

	for (size_t i = 0; i < size; i++)
	{
		pitch_1 = GetPitch(PITCH_1);
		if(pitch_1 != 0.f)
		{
			string_env_1 = GetEnv(ENV_1); 
		    voice1.SetFreq(pitch_1);
			flt1_cutoff = pitch_1 * FLT_TRACK_SLOPE + FLT_TRACK_OFFSET;
			sig1 = voice1.Process() * string_env_1;
			flt1.SetFreq(flt1_cutoff);
			sig1 = flt1.Process(sig1);
		}

		pitch_2 = GetPitch(PITCH_2);
		if(pitch_2 != 0.f)
		{
			string_env_2 = GetEnv(ENV_2); 
		    voice2.SetFreq(pitch_2);
			flt2_cutoff = pitch_2 * FLT_TRACK_SLOPE + FLT_TRACK_OFFSET;
			sig2 = voice2.Process() * string_env_2;
			flt2.SetFreq(flt2_cutoff);
			sig2 = flt2.Process(sig2);
		}

		float sig = chorus.Process(sig1+sig2);
//		out[0][i] = out[1][i] = sig;
		verb.Process(sig, sig, &out[0][i], &out[1][i]);
	}
}

int main(void)
{ 
	env_hyst_flag[0] = env_hyst_flag[1] = false;

	string_base_freq[0] = mtof((float)config.string_base_midi[0]);
	string_base_freq[1] = mtof((float)config.string_base_midi[1]);

	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	float samplerate = hw.AudioSampleRate();

	pitch_gate[0].Init(seed::D13, GPIO::Mode::INPUT, GPIO::Pull::NOPULL);
	pitch_gate[1].Init(seed::D14, GPIO::Mode::INPUT, GPIO::Pull::NOPULL);

    AdcChannelConfig adc_cfg[4];
    adc_cfg[0].InitSingle(seed::A0);
    adc_cfg[1].InitSingle(seed::A1);
    adc_cfg[2].InitSingle(seed::A2);
    adc_cfg[3].InitSingle(seed::A3);
	hw.adc.Init(adc_cfg, 4);
    hw.adc.Start();

    chorus.Init(samplerate);
    chorus.SetLfoFreq(.33f);
    chorus.SetLfoDepth(1.f);
    chorus.SetDelay(0.9f);
	chorus.SetFeedback(0.3);

    flt1.Init(samplerate);
    flt1.SetRes(0.9);
    flt2.Init(samplerate);
    flt2.SetRes(0.9);
	
    verb.Init(samplerate);
    verb.SetFeedback(0.9f);
    verb.SetLpFreq(2000.0f);

	voice1.Init(samplerate);
	voice2.Init(samplerate);
	voice1.SetWaveshape(0.5f);
	voice2.SetWaveshape(0.5f);
 
	hw.StartAudio(AudioCallback);
	while(1) {}
}

float GetPitch(int channel)
{
	float pos, pitch;

	pitch = 0.f;
	if(pitch_gate[channel].Read())
	{
		pos = hw.adc.GetFloat(channel);
		pitch = string_base_freq[channel] * ((B3 * pow(pos, 3)) + (B2 * pow(pos, 2)) + (B1 * pos) + B0);
	}
	return pitch;
}

float GetEnv(int channel)
{
	float env;

	env = hw.adc.GetFloat(channel); 
	if(env_hyst_flag[channel-2])
	{
		if(env >= ENV_THR_2)
		{
			env_hyst_flag[channel-2] = false;
		}
		else env = 0.f;
	}
	else
	{
		if(env <= ENV_THR_1)
		{
			env_hyst_flag[channel-2] = true;
			env = 0.f;
		}
	}
	
	return env;
}