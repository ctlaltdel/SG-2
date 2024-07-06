#include "daisy_seed.h"
#include "Instrument.h"
#include "Program.h"

using namespace daisy;
using namespace daisy::seed;

#define VOICE_SIZE 128
#define CART_ADDR 0xA0
#define SCL {DSY_GPIOB, 8}
#define SDA {DSY_GPIOB, 9}
#define GPIO_CART_STATUS D25
#define GPIO_CART_DETECT D24

class CartStatusLED
{
    public:
        void Init();
        void Off();
        void On();
        void Toggle();

    private:
        bool state;
        GPIO cart_status;
};

class CartridgeMonitor
{
    enum{STATE_IDLE, STATE_PAUSE, STATE_LOAD, STATE_VERIFY, STATE_DONE, STATE_ERROR};

    public:
        void Init(Instrument *instr, CartStatusLED *led);
        bool CartridgePresent();
        void CartridgeLoad();
        void Process();

    private:
        uint8_t state;
	    I2CHandle::Config i2c_conf;
	    I2CHandle i2c;
        GPIO cart_detect;
        CartStatusLED *cart_status;
        Instrument *mInstrument;
        struct SyxVoiceParam voiceParam;
        uint32_t t1, t2;
};

