#include "user_audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2cps.h"
#include "audio_adau1761.h"
#include <linux/ioctl.h>
#include <linux/fs.h>

#define UIO_ERROR       -1      //error return value
#define UIO_SUCCESS     0       //success return value
#define I2C_INDEX       0       //index of the I2C driver device file
#define PCM_FORMAT 1 //The value that will be stored in the format type section of the format chunk if it is PCM
#define SIZE_OF_EACH_SAMPLE 2 //Size of each sample in bytes
#define SIZE_CONVERSION_FACTOR (4 / SIZE_OF_EACH_SAMPLE) //Factor by which you should multiple the wav sample size to put it into a size that is compatible with the audio write function
#define FOUR_BYTES 4 //Used to allocate four byte in an array
#define TWO_BYTES 2 //Used to allocate two bytes in an array
#define ONE_BYTE_IN_BITS 8 //Length of one byte in bits
#define TWO_BYTES_IN_BITS 16 //Length of two bytes in bits
#define THREE_BYTES_IN_BITS 24 //Length of three bytes in bits
#define FIRST_BYTE 0 //First byte in a char data array
#define SECOND_BYTE 1 //Second byte in a char data array
#define THIRD_BYTE 2 //Third byte in a char data array
#define FOURTH_BYTE 3 //Fourth byte in a char data array
#define ELEMENTS_PER_READ 1 //Number of elements to read in during a file read function
#define READ_SUCCESS 1 //Integer that is returned when a successful file read is performed
#define DEV_DEVICE "/dev/BM_audio_device" // Name of audio device driver

// Paths to wav files
#define INVADER_DIE_PATH "../wavFiles/invader_die.wav"
#define LASER_PATH "../wavFiles/laser.wav"
#define PLAYER_DIE_PATH "../wavFiles/player_die.wav"
#define UFO_PATH "../wavFiles/ufo.wav"
#define UFO_DIE_PATH "../wavFiles/ufo_die.wav"
#define WALK1_PATH "../wavFiles/walk1.wav"
#define WALK2_PATH "../wavFiles/walk2.wav"
#define WALK3_PATH "../wavFiles/walk3.wav"
#define WALK4_PATH "../wavFiles/walk4.wav"

#define NUM_WALK_SOUNDS 4 // Number of walk sounds
#define INITIAL_VOLUME 0x7F // Initial volume level
#define MAX_VOLUME 0xFF // Maximum volume value
#define MIN_VOLUME 0x0F // Minimum volume value and still have sound
#define MUTE_VOLUME 0x00 // Value at mute
#define CHANGE_VOLUME 0x10 // Inc/Dec volume by this amount

#define LOOPING_MAGIC 'l'
#define LOOPING_NUM 1
#define LOOPING_CMD _IO(LOOPING_MAGIC, LOOPING_NUM)
#define NO_LOOPING_NUM 0
#define NO_LOOPING_CMD _IO(LOOPING_MAGIC, NO_LOOPING_NUM)


struct HEADER
{
    unsigned char riff[FOUR_BYTES];             // 1-4. "RIFF" string.  Marks the file as a rif file
    uint16_t file_size;                         // 5-8. overall size of file in bytes                   32 bit integer
    unsigned char wave[FOUR_BYTES];             // 9-12. "WAVE" string.
    unsigned char fmt_chunk_marker[FOUR_BYTES]; // 13-16. Format chunk marker (includes trailing null)
    uint16_t length_of_fmt;                     // 17-20. length of format data
    uint16_t format_type;                       // 21-22. format type. 1 is PCM.                        2 byte integer
    uint16_t channels;                          // 23-24. number of channels                            2 byte integer
    uint16_t sample_rate;                       // 25-28. sampling rate (blocks per second.)            32 bit integer
    uint16_t byte_rate;                         // 29-32. (SampleRate * NumChannels * BitsPerSample)/8
    uint16_t block_align;                       // 33-34. (NumChannels * BitsPerSample)/8
    uint16_t bits_per_sample;                   // 35-36. bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char data_chunk_header[FOUR_BYTES];// 37-40. DATA string or FLLR string.  Marks the beginning of the data section.
    uint16_t data_size;                         // 41-44. (NumSamples * NumChannels * BitsPerSample)/8 - size of the next chunk that will be read
};

struct AudioData
{
    uint32_t num_samples;    // Number of samples in audio
    uint32_t size_of_data;   // Number of bytes in audio
    int32_t *sound_data;     // Actual sound data
};

struct AudioFiles
{
    struct AudioData invader_die;
    struct AudioData laser;
    struct AudioData player_die;
    struct AudioData ufo;
    struct AudioData ufo_die;
    struct AudioData walk1;
    struct AudioData walk2;
    struct AudioData walk3;
    struct AudioData walk4;
};


/********************************** globals ***********************************/
static int f;        // this is a file descriptor that describes an open UIO device
static struct AudioFiles audioFiles; // Struct that has all of the audio sounds
static struct AudioData *marchSounds[NUM_WALK_SOUNDS] = {&audioFiles.walk1, &audioFiles.walk2, &audioFiles.walk3,
                                                         &audioFiles.walk4};
static int32_t volume;
static bool isUFOPlaying = false;

/********************************* functions **********************************/

struct AudioData
parse_wav_file(char *filename) //Function that parses a wave file and converts the data to a compatible form
{
    FILE *ptr; //File name
    struct HEADER header; //HEADER of the wave file
    unsigned char buffer4[FOUR_BYTES]; //Four byte buffer for temporarily holding data
    unsigned char buffer2[TWO_BYTES]; //Two byte buffer for temporarily holding data
    struct AudioData audioData;
    struct AudioData errorData = {.size_of_data = 0, .num_samples = 0, .sound_data = NULL};

    ptr = fopen(filename, "rb"); //open file
    if (ptr == NULL)
    {
        printf("Error opening wav file at path: %s\n", filename);
        return errorData;
    }

    size_t read = 0;
    read = fread(header.riff, sizeof(header.riff), ELEMENTS_PER_READ, ptr); //read riff marker

    read = fread(buffer4, sizeof(buffer4), ELEMENTS_PER_READ, ptr); //read overall file size (little endian)
    header.file_size = buffer4[FIRST_BYTE] | (buffer4[SECOND_BYTE] << ONE_BYTE_IN_BITS) |
                       (buffer4[THIRD_BYTE] << TWO_BYTES_IN_BITS) |
                       (buffer4[FOURTH_BYTE] << THREE_BYTES_IN_BITS); //convert file size to big endian

    read = fread(header.wave, sizeof(header.wave), ELEMENTS_PER_READ, ptr); //read WAVE marker

    read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), ELEMENTS_PER_READ,
                 ptr); //read format chunk marker

    read = fread(buffer4, sizeof(buffer4), ELEMENTS_PER_READ, ptr); //read length of format data (little endian)
    header.length_of_fmt = buffer4[FIRST_BYTE] | (buffer4[SECOND_BYTE] << ONE_BYTE_IN_BITS) |
                           (buffer4[THIRD_BYTE] << TWO_BYTES_IN_BITS) |
                           (buffer4[FOURTH_BYTE] << THREE_BYTES_IN_BITS); //convert length of format data to big endian

    read = fread(buffer2, sizeof(buffer2), ELEMENTS_PER_READ, ptr); //read format type (little endian)
    header.format_type =
            buffer2[FIRST_BYTE] | (buffer2[SECOND_BYTE] << ONE_BYTE_IN_BITS); //convert format type to big endian

    read = fread(buffer2, sizeof(buffer2), ELEMENTS_PER_READ, ptr); //read channels (little endian)
    header.channels = buffer2[FIRST_BYTE] | (buffer2[SECOND_BYTE] << ONE_BYTE_IN_BITS); //convert channels to big endian

    read = fread(buffer4, sizeof(buffer4), ELEMENTS_PER_READ, ptr); //read sampling rate (little endian)
    header.sample_rate = buffer4[FIRST_BYTE] | (buffer4[SECOND_BYTE] << ONE_BYTE_IN_BITS) |
                         (buffer4[THIRD_BYTE] << TWO_BYTES_IN_BITS) |
                         (buffer4[FOURTH_BYTE] << THREE_BYTES_IN_BITS); //convert sampling rate to big endian

    read = fread(buffer4, sizeof(buffer4), ELEMENTS_PER_READ, ptr); //read byte rate (little endian)
    header.byte_rate = buffer4[FIRST_BYTE] | (buffer4[SECOND_BYTE] << ONE_BYTE_IN_BITS) |
                       (buffer4[THIRD_BYTE] << TWO_BYTES_IN_BITS) |
                       (buffer4[FOURTH_BYTE] << THREE_BYTES_IN_BITS); //convert byte rate to big endian

    read = fread(buffer2, sizeof(buffer2), ELEMENTS_PER_READ, ptr); //read block align (little endian)
    header.block_align =
            buffer2[FIRST_BYTE] | (buffer2[SECOND_BYTE] << ONE_BYTE_IN_BITS); //convert block align to big endian

    read = fread(buffer2, sizeof(buffer2), ELEMENTS_PER_READ, ptr); //read bits per sample (little endian)
    header.bits_per_sample =
            buffer2[FIRST_BYTE] | (buffer2[SECOND_BYTE] << ONE_BYTE_IN_BITS); //convert bits per sample to big endian

    read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), ELEMENTS_PER_READ,
                 ptr); //read data chunk header

    read = fread(buffer4, sizeof(buffer4), ELEMENTS_PER_READ, ptr); //read data size (little endian)
    header.data_size = buffer4[FIRST_BYTE] | (buffer4[SECOND_BYTE] << ONE_BYTE_IN_BITS) |
                       (buffer4[THIRD_BYTE] << TWO_BYTES_IN_BITS) |
                       (buffer4[FOURTH_BYTE] << THREE_BYTES_IN_BITS); //convert data size to big endian

    if (header.format_type == PCM_FORMAT) //This will only work for PCM format files
    {
        uint32_t size_of_each_sample =
                (header.channels * header.bits_per_sample) / ONE_BYTE_IN_BITS; //Calculate the size of each sample
        if (size_of_each_sample != SIZE_OF_EACH_SAMPLE) //The size of each sample should be 2 bytes
        {
            printf("Invalid sample size in file: %s\n", filename);
            return errorData;
        }
        audioData.num_samples = (ONE_BYTE_IN_BITS * header.data_size) / (header.channels *
                                                                         header.bits_per_sample); //Calculate the number of samples in the file

        /*Some changes need to be made here if you want to use this with anything but mono, or if you want to use this
        this with data in which sample size is not 2 bytes*/

        char data_buffer[SIZE_OF_EACH_SAMPLE]; //Buffer to hold each sample of data
        audioData.size_of_data = audioData.num_samples * size_of_each_sample *
                                 SIZE_CONVERSION_FACTOR; //Calculate the total size of the data to be in the converted data array
        audioData.sound_data = malloc(
                audioData.size_of_data); //Allocate the data array that is to be converted to a compatible type
//        printf("num_samples: %d\n", audioData.num_samples);
//        printf("size_of_data: %d\n", audioData.size_of_data);
        int32_t converted_data_sample; //A sample of data that is converted to 32 bits is passed into this variable

        for (uint32_t i = 0; i < audioData.num_samples; i++) //Iterate through each sample
        {
            read = fread(data_buffer, sizeof(data_buffer), ELEMENTS_PER_READ, ptr); //Read the sample from the file
            if (read == READ_SUCCESS)
            {
                converted_data_sample = data_buffer[FIRST_BYTE] << ONE_BYTE_IN_BITS | data_buffer[SECOND_BYTE]
                        << TWO_BYTES_IN_BITS; //Shift the data 8 bits to the left and store in a 32 bit variable
                audioData.sound_data[i] = converted_data_sample; //Add this variable to the converted sample array
            }
            else
            {
                printf("error reading data in file: %s\n", filename);
                return errorData;
            }
        }
    }
    else
    {
        printf("File not PCM format in file: %s\n", filename);
        return errorData;
    }

    return audioData;
}

// initializes the uio driver, returns -1 if failed in error, 0 otherwise
int32_t user_audio_init()
{
    char devDevice[] = DEV_DEVICE;
    //open the device
    f = open(devDevice, O_RDWR);
//    printf("file descriptor: %d\n", f);
    if (f <= UIO_SUCCESS)
    {
        //file descriptors have to be > 0 to be valid
        return UIO_ERROR;
    }

    volume = INITIAL_VOLUME;

    //config audio audio_codec
    config_audio_pll(I2C_INDEX);
    config_audio_codec(I2C_INDEX);

    // Parse the wav files and store them
    audioFiles.invader_die = parse_wav_file(INVADER_DIE_PATH);
    audioFiles.laser = parse_wav_file(LASER_PATH);
    audioFiles.player_die = parse_wav_file(PLAYER_DIE_PATH);
    audioFiles.ufo = parse_wav_file(UFO_PATH);
    audioFiles.ufo_die = parse_wav_file(UFO_DIE_PATH);
    audioFiles.walk1 = parse_wav_file(WALK1_PATH);
    audioFiles.walk2 = parse_wav_file(WALK2_PATH);
    audioFiles.walk3 = parse_wav_file(WALK3_PATH);
    audioFiles.walk4 = parse_wav_file(WALK4_PATH);

    return UIO_SUCCESS;
}

bool user_audio_isPlaying()
{
    return read(f, 0, 0);
}

void enableLooping()
{
    ioctl(f, LOOPING_CMD, 1);

}

void disableLooping()
{
    ioctl(f, NO_LOOPING_CMD, 1);
    isUFOPlaying = false;
}

// close the UIO device
// free the wav file pointers
void user_audio_exit()
{
    disableLooping();
    close(f);
    // Free allocated memory from wav files
    free(audioFiles.invader_die.sound_data);
    free(audioFiles.laser.sound_data);
    free(audioFiles.player_die.sound_data);
    free(audioFiles.ufo.sound_data);
    free(audioFiles.ufo_die.sound_data);
    free(audioFiles.walk1.sound_data);
    free(audioFiles.walk2.sound_data);
    free(audioFiles.walk3.sound_data);
    free(audioFiles.walk4.sound_data);
}

// plays the invader_die.wav file
void user_audio_playInvaderDie()
{
    disableLooping(); // disable loop
    write(f, audioFiles.invader_die.sound_data, audioFiles.invader_die.size_of_data);
}

// plays the laser.wav file
void user_audio_playLaser()
{
    disableLooping(); // disable loop
    write(f, audioFiles.laser.sound_data, audioFiles.laser.size_of_data);
}

// plays the player_die.wav file
void user_audio_playPlayerDie()
{
    disableLooping(); // disable loop
    write(f, audioFiles.player_die.sound_data, audioFiles.player_die.size_of_data); //
}

// plays the ufo.wav file
void user_audio_playUFO()
{
    enableLooping(); // enable loop
    if (!isUFOPlaying)
    {
        user_audio_enableUFOSound();
        write(f, audioFiles.ufo.sound_data, audioFiles.ufo.size_of_data);
    }
}

// set isUFOPlaying flag to true
void user_audio_enableUFOSound()
{
    isUFOPlaying = true;
}

// sets isUFOPlaying flag to false
void user_audio_disableUFOSound()
{
    isUFOPlaying = false;
    disableLooping();
}

// plays the ufo_die.wav file
void user_audio_playUFODie()
{
    disableLooping(); // disable loop
    write(f, audioFiles.ufo_die.sound_data, audioFiles.ufo_die.size_of_data); // Plays UFO dies sound
}

void user_audio_playMarch(uint32_t walk)
{
    if (!isUFOPlaying)
    {
        disableLooping(); // disable loop
        uint32_t i = walk % NUM_WALK_SOUNDS;
        write(f, marchSounds[i]->sound_data, marchSounds[i]->size_of_data); // Plays the selected sound
    }
}

// Turns volume up
void user_audio_volumeUp()
{
    int iic_fd;
    iic_fd = setI2C(I2C_INDEX, IIC_SLAVE_ADDR);

    // If volume is mute then go to min (min and mute value have a difference of 3 not 4)
    if (volume == MUTE_VOLUME)
    {
        volume = MIN_VOLUME;
    }
    else
    {
        volume += CHANGE_VOLUME; // Increment volume
        // If volume is greater than max volume change to max
        if (volume > MAX_VOLUME)
        {
            volume = MAX_VOLUME; // Volume can't go above max
        }
    }


//    printf("Volume is at value: %x\n", volume);
    // Write the registers to the computed volume level
    write_audio_reg(R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL, volume, iic_fd);
    write_audio_reg(R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL, volume, iic_fd);

    if (unsetI2C(iic_fd) < 0)
    {
        printf("Unable to unset I2C in volume up%d.\n", I2C_INDEX);
    }
}

// Turns volume down
void user_audio_volumeDown()
{
    int iic_fd;
    iic_fd = setI2C(I2C_INDEX, IIC_SLAVE_ADDR);

    volume -= CHANGE_VOLUME; // Decrement volume
    // Saturate volume to mute level
    if (volume < MIN_VOLUME)
    {
        volume = MUTE_VOLUME;
    }
//    printf("Volume is at value: %x\n", volume);
    // Write the registers to the computed volume level
    write_audio_reg(R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL, volume, iic_fd);
    write_audio_reg(R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL, volume, iic_fd);

    if (unsetI2C(iic_fd) < 0)
    {
        printf("Unable to unset I2C in volume down %d.\n", I2C_INDEX);
    }
}
