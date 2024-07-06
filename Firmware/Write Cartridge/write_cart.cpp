#include "daisy_seed.h"
#include "daisysp.h"
#include "voice_data.h"

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

static DaisySeed hw;
GPIO cart_status, cart_detect;
enum{STATE_IDLE, STATE_PAUSE, STATE_LOAD1, STATE_LOAD2, STATE_VERIFY, STATE_DONE, STATE_ERROR};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
	}
}

int main(void)
{ 
	uint8_t state, led_state=0, i2c_addr = 0xa0, temp[VOICE_SIZE];

	hw.Init();
	hw.StartLog(false);
//	hw.SetAudioBlockSize(4); // number of samples handled per callback
//	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
//	float samplerate = hw.AudioSampleRate();

	cart_status.Init(D25, GPIO::Mode::OUTPUT);
	cart_detect.Init(D24, GPIO::Mode::INPUT, GPIO::Pull::NOPULL);

	I2CHandle::Config i2c_conf;
	i2c_conf.periph = I2CHandle::Config::Peripheral::I2C_1;
	i2c_conf.speed  = I2CHandle::Config::Speed::I2C_100KHZ;
	i2c_conf.mode   = I2CHandle::Config::Mode::I2C_MASTER;
	i2c_conf.pin_config.scl  = {DSY_GPIOB, 8};
	i2c_conf.pin_config.sda  = {DSY_GPIOB, 9};

	I2CHandle i2c;
	i2c.Init(i2c_conf);

	state = STATE_IDLE;
//	hw.StartAudio(AudioCallback);
	while(1) 
	{
		switch(state)
		{
			case STATE_IDLE:
				hw.PrintLine("State: IDLE");
				led_state = 0;
				cart_status.Write(led_state);
				if(!cart_detect.Read()) state = STATE_PAUSE;
				break;
			case STATE_PAUSE:
				hw.PrintLine("State: PAUSE");
				if(!cart_detect.Read()) state = STATE_LOAD1; else state = STATE_IDLE;
				break;
			case STATE_LOAD1:
				hw.PrintLine("State: LOAD 1");
				i2c.WriteDataAtAddress(i2c_addr, 0x0, 0x02, voice_data, VOICE_SIZE >> 1, 100);
				state = STATE_LOAD2;
				break;
			case STATE_LOAD2:
				hw.PrintLine("State: LOAD 2");
				i2c.WriteDataAtAddress(i2c_addr, VOICE_SIZE >> 1, 0x02, voice_data + (VOICE_SIZE >> 1), VOICE_SIZE >> 1, 100);
				state = STATE_VERIFY;
				break;
			case STATE_VERIFY:
				hw.PrintLine("State: VERIFY");
				state = STATE_DONE;
				i2c.ReadDataAtAddress(i2c_addr, 0x0, 0x02, temp, VOICE_SIZE, 100);
				for(uint i=0; i<VOICE_SIZE; i++)
				{
					if(voice_data[i] != temp[i]) 
					{
						state = STATE_ERROR;
						led_state = 0;
						hw.PrintLine("Cartridge data at location %u should be %02X but was read as %02X", i, voice_data[i], temp[i]);
					}
				}				
				cart_status.Write(led_state);
				break;
			case STATE_DONE:
				hw.PrintLine("State: DONE");
				led_state = 1;
				cart_status.Write(led_state);
				if(cart_detect.Read()) state = STATE_IDLE;
				break;
			case STATE_ERROR:
				hw.PrintLine("State: ERROR");
				led_state ^= 1;
				cart_status.Write(led_state);
				break;
			default:
				state = STATE_IDLE;
				break;
		}
		System::Delay(500);
	}
}