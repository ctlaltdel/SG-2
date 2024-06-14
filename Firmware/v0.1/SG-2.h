#define PITCH_1             0
#define PITCH_2             1
#define ENV_1               2
#define ENV_2               3
#define ENV_THR_1           0.010f
#define ENV_THR_2           0.020f
#define B0                  3.6835
#define B1                  -6.9249
#define B2                  6.9501
#define B3                  -2.7343

#define FLT_TRACK_SLOPE     8.f
#define FLT_TRACK_OFFSET    200.f

struct Configuration
{
    int8_t  string_base_midi[2];
};

float GetPitch(int channel);
float GetEnv(int channel);
