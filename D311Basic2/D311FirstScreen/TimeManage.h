#include <Windows.h>


static double countsPerSecond = 0.0;
static __int64 CounterStart = 0;

static int frameCount = 0;
static int fps = 0;

static __int64 frameTimeOld = 0;
static double frameTime;

// Time 
void StartTimer();
double GetTime();
double GetFrameTime();
