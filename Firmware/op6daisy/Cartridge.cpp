#include "daisy_seed.h"
#include "Cartridge.h"
#include "SyxVoiceParam.h"

using namespace daisy;
using namespace daisy::seed;

void CartridgeMonitor::Init(Instrument *instr, CartStatusLED *led) {
    mInstrument=instr;
    
    cart_status = led;
    cart_status->Init();

	i2c_conf.periph = I2CHandle::Config::Peripheral::I2C_1;
	i2c_conf.speed  = I2CHandle::Config::Speed::I2C_100KHZ;
	i2c_conf.mode   = I2CHandle::Config::Mode::I2C_MASTER;
	i2c_conf.pin_config.scl  = SCL;
	i2c_conf.pin_config.sda  = SDA;
	i2c.Init(i2c_conf);

	cart_detect.Init(GPIO_CART_DETECT, GPIO::Mode::INPUT, GPIO::Pull::NOPULL);

    state = STATE_IDLE;
	t1 = System::GetNow();
}

void CartridgeMonitor::Process() {
	t2 = System::GetNow();
	if((t2 - t1) >= CART_CHECK_INTERVAL)
	{
		t1 = t2;
	
		switch(state)
		{
			case STATE_IDLE:
				if(CartridgePresent()) state = STATE_PAUSE;
				break;
			case STATE_PAUSE:
				if(CartridgePresent()) state = STATE_LOAD; else state = STATE_IDLE;
				break;
			case STATE_LOAD:
				CartridgeLoad();
				state = STATE_VERIFY;
				break;
			case STATE_VERIFY:
				state = STATE_DONE;
				cart_status->On();
				break;
			case STATE_DONE:
				if(!CartridgePresent()) state = STATE_IDLE;
				break;
			case STATE_ERROR:
				cart_status->Toggle();
				break;
			default:
				state = STATE_IDLE;
				break;
		} 
	}  
}

void CartridgeMonitor::CartridgeLoad()
{
	i2c.ReadDataAtAddress(CART_ADDR, 0x0, 0x02, (uint8_t *)&voiceParam, VOICE_SIZE, 100);
	mInstrument->programLoad(voiceParam);
}

bool CartridgeMonitor::CartridgePresent()
{
    return !cart_detect.Read();
}

void CartStatusLED::Init()
{
   cart_status.Init(GPIO_CART_STATUS, GPIO::Mode::OUTPUT);
   Off();
}

void CartStatusLED::Off()
{
    state = false;
    cart_status.Write(state);
}

void CartStatusLED::On()
{
    state = true;
    cart_status.Write(state);    
}

void CartStatusLED::Toggle()
{
    state ^= true;
    cart_status.Write(state);        
}



