#ifndef HLSL_TRIANGLE
#define HLSL_TRIANGLE

static const float4 clip_fullscreen_quad[6] = {
    {-1.0f, -1.0f, 0.0f, 1.0f},
    {-1.0f,  1.0f, 0.0f, 1.0f},
    { 1.0f, -1.0f, 0.0f, 1.0f},
	{-1.0f,  1.0f, 0.0f, 1.0f},
	{ 1.0f,  1.0f, 0.0f, 1.0f},
	{ 1.0f, -1.0f, 0.0f, 1.0f}
};

#endif //HLSL_TRIANGLE