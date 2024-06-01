#define PITCH_1             0
#define PITCH_2             1
#define ENV_1               2
#define ENV_2               3
#define MIN_ENV             0.009f
#define MIN_POS             0.01f  
#define PITCH_COEFF_A       0.91040486
#define PITCH_COEFF_B       2.64052850
#define PITCH_GATE_DELAY    4800
#define B0                  3.6835
#define B1                  -6.9249
#define B2                  6.9501
#define B3                  -2.7343
struct Configuration
{
    int8_t  string_base_midi[2];
};

float GetPitch(int channel);
float GetEnv(int channel);
