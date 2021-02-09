#ifndef WAV_PARSE
#define WAV_PARSE

#include <stdint.h>
#define BYTES_PER_S

struct HEADER
{
    unsigned char riff[4];                      // 1-4. "RIFF" string.  Marks the file as a rif file
    uint16_t file_size;                         // 5-8. overall size of file in bytes                   32 bit integer
    unsigned char wave[4];                      // 9-12. "WAVE" string.
    unsigned char fmt_chunk_marker[4];          // 13-16. Format chunk marker (includes trailing null)
    uint16_t length_of_fmt;                     // 17-20. length of format data
    uint16_t format_type;                       // 21-22. format type. 1 is PCM.                        2 byte integer
    uint16_t channels;                          // 23-24. number of channels                            2 byte integer
    uint16_t sample_rate;                       // 25-28. sampling rate (blocks per second.)            32 bit integer
    uint16_t byte_rate;                         // 29-32. (SampleRate * NumChannels * BitsPerSample)/8
    uint16_t block_align;                       // 33-34. (NumChannels * BitsPerSample)/8
    uint16_t bits_per_sample;                   // 35-36. bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char data_chunk_header[4];         // 37-40. DATA string or FLLR string.  Marks the beginning of the data section.
    uint16_t data_size;                         // 41-44. (NumSamples * NumChannels * BitsPerSample)/8 - size of the next chunk that will be read
};

uint32_t parse_wav_file(char* filename, int32_t* sound_data);

#endif