#include "wav_parser.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define PCM_FORMAT 1 //The value that will be stored in the format type section of the format chunk if it is PCM
#define SIZE_OF_EACH_SAMPLE 2 //Size of each sample in bytes
#define SIZE_CONVERSION_FACTOR 4 / SIZE_OF_EACH_SAMPLE //Factor by which you should multiple the wav sample size to put it into a size that is compatible with the audio write function


uint32_t parse_wav_file(char* filename, int32_t* sound_data)
{
    FILE* ptr;
    struct HEADER header;
    unsigned char buffer4[4];
    unsigned char buffer2[2];
    uint32_t size_of_data;

    ptr = fopen(filename, "rb"); //open file
    if (ptr == NULL) {
        printf("Error opening file\n");
    }

    size_t read = 0;
    read = fread(header.riff, sizeof(header.riff), 1, ptr); //read riff marker

    read = fread(buffer4, sizeof(buffer4), 1, ptr); //read overall file size (little endian)
    header.file_size = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24); //convert file size to big endian

    read = fread(header.wave, sizeof(header.wave), 1, ptr); //read WAVE marker

    read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr); //read format chunk marker

    read = fread(buffer4, sizeof(buffer4), 1, ptr); //read length of format data (little endian)
    header.length_of_fmt = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24); //convert length of format data to big endian

    read = fread(buffer2, sizeof(buffer2), 1, ptr); //read format type (little endian)
    header.format_type = buffer2[0] | (buffer2[1]<<8); //convert format type to big endian

    read = fread(buffer2, sizeof(buffer2), 1, ptr); //read channels (little endian)
    header.channels = buffer2[0] | (buffer2[1]<<8); //convert channels to big endian

    read = fread(buffer4, sizeof(buffer4), 1, ptr); //read sampling rate (little endian)
    header.sample_rate = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24); //convert sampling rate to big endian

    read = fread(buffer4, sizeof(buffer4), 1, ptr); //read byte rate (little endian)
    header.byte_rate = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24); //convert byte rate to big endian

    read = fread(buffer2, sizeof(buffer2), 1, ptr); //read block align (little endian)
    header.block_align = buffer2[0] | (buffer2[1]<<8); //convert block align to big endian

    read = fread(buffer2, sizeof(buffer2), 1, ptr); //read bits per sample (little endian)
    header.bits_per_sample = buffer2[0] | (buffer2[1]<<8); //convert bits per sample to big endian

    read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr); //read data chunk header

    read = fread(buffer4, sizeof(buffer4), 1, ptr); //read data size (little endian)
    header.data_size = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24); //convert data size to big endian

    if (header.format_type == PCM_FORMAT)
    {
        uint32_t size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
        if (size_of_each_sample != SIZE_OF_EACH_SAMPLE)
        {
            printf("Invalid sample size\n");
        }
        uint32_t num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);

        /*Some changes need to be made here if you want to use this with anything but mono, or if you want to use this
        this with data in which sample size is not 2 bytes*/

        char data_buffer[SIZE_OF_EACH_SAMPLE];
        size_of_data = num_samples * size_of_each_sample * SIZE_CONVERSION_FACTOR;
        sound_data = malloc(size_of_data);
        printf("Num_samples: %d\n", num_samples);
        printf("Size should be: %d, size is %d\n", size_of_data, sizeof(sound_data));
        int32_t converted_data_sample;

        for (uint32_t i = 0; i < num_samples; i++)
        {
            read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
            if (read == 1)
            {
                converted_data_sample = data_buffer[0]<<16 | data_buffer[1];
                sound_data[i] = converted_data_sample;
            }
            else
            {
                printf("error reading data\n");
            }
        }
    }
    else
    {
        printf("File not PCM format\n");
    }
    return size_of_data;
}
