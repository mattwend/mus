
#ifndef H_TYPES
#define H_TYPES

#ifndef M_PI
#define M_PI (3.14159265)
#endif

#define TABLE_SIZE (400)

typedef float mus_audio_sample_t;

// temporary buffer type
typedef struct
{
	mus_audio_sample_t* out1;
	mus_audio_sample_t* out2;
	int nframes;
}
mus_audio_buffer_t;

#endif // H_TYPES
