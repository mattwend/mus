
#ifndef H_TYPES
#define H_TYPES

#ifndef M_PI
#define M_PI (3.14159265)
#define TABLE_SIZE (200)
#endif

//typedef float mus_audio_sample_t

typedef struct
{
	float sine[TABLE_SIZE];
	int left_phase;
	int right_phase;
}
paTestData;

#endif // H_TYPES
