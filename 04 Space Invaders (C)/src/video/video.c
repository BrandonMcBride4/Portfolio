#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <printf.h>
#include <stdio.h>
#include "video.h"

#define UIO_EXAMPLE_MMAP_SIZE   0x1000  //size of memory to allocate
#define UIO_EXAMPLE_ERROR       -1      //error return value
#define UIO_EXAMPLE_SUCCESS     0       //success return value

#define PIXEL_SIZE 3 // A pixel is 3 bytes
#define RED_OFFSET 16 // Offset value for shifting to the red value of an RGB value
#define GREEN_OFFSET 8 // Offset value for shifting to the green value of an RGB value
#define RGB_MASK 0xFF // Mask for selecting only the rightmost byte of an RGB value in order to get just one color value
#define GREEN_INCREMENT 1 // Value to increment from the red value of an array of packed rgb values in order to select green
#define BLUE_INCREMENT 2 // Value to increment from the red value of an array of packed rgb values in order to select blue
#define ORIGIN_VALUE_X 0 // Value representing the x origin of the display (left side)
#define ORIGIN_VALUE_Y 0 // Value representing the y origin of the display (top)
#define START_OF_BITMAP_MASK 1
#define INDEX_OFFSET 1         // Used to offset the bitmap ( the 32nd bit is indexed at 32-offset)
#define SIZE_OF_SCREEN PIXEL_SIZE * VIDEO_SCREEN_WIDTH * VIDEO_SCREEN_HEIGHT   // The total size of the screen found by calculating the total number of pixels and multiplying that by PIXEL_SIZE

#define READ_RED_INDEX   2 // For some reason the read function makes the buffer gbr instead of rgb, r is 2
#define READ_GREEN_INDEX 0 // For some reason the read function makes the buffer gbr instead of rgb, g is 0
#define READ_BLUE_INDEX  1 // For some reason the read function makes the buffer gbr instead of rgb, b is 1
#define SHIFT_RED 16        // Shift the red values
#define SHIFT_GREEN 8       // Shift the green values
#define SHIFT_BLUE 0        // Shift the blue number
#define RESET 0


/********************************** globals ***********************************/
static int f;        // this is a file descriptor that describes an open UIO device
static char *ptr;    // this is the virtual address of the UIO device registers


/********************************* functions **********************************/
// initializes the uio driver, returns -1 if failed in error, 0 otherwise
int32_t video_init(char devDevice[])
{

    //open the device
    f = open(devDevice, O_RDWR);
    if (f <= UIO_EXAMPLE_SUCCESS)
    {
        //file descriptors have to be > 0 to be valid
        return UIO_EXAMPLE_ERROR;
    }

    //memory map the physical address of the hardware into virtual address space
    ptr = mmap(0, UIO_EXAMPLE_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    if (ptr == MAP_FAILED)
    {
        return UIO_EXAMPLE_ERROR;
    }


    return UIO_EXAMPLE_SUCCESS;
}

// Sets the offset marker of the HDMI display
void setOffset(uint32_t x, uint32_t y)
{
    uint32_t offset = (PIXEL_SIZE * y * VIDEO_SCREEN_WIDTH + PIXEL_SIZE *
                                                             x); // Convert 2D values to 1D value and multiply by the size of each pixel to get total offset
    lseek(f, offset, SEEK_SET); // Set the offset marker to the offset value that was calculated above
}

// Takes an rgb value and packs the colors into the array passed in
void colorPacking(char *packed_colors, uint32_t rgbValue, uint32_t size)
{
    char r = (char) ((rgbValue >> RED_OFFSET) & RGB_MASK); // Get the red portion of an rgbvalue
    char g = (char) ((rgbValue >> GREEN_OFFSET) & RGB_MASK); // Get the green portion of an rgbvalue
    char b = (char) (rgbValue & RGB_MASK); // Get the blue portion of an rgbvalue

    for (int i = 0;
         i < size; i += PIXEL_SIZE) //Iterate through each pixel for the commanded length to set it to the rgb value
    {
        packed_colors[i] = r; // Set the red value of this pixel
        packed_colors[i + GREEN_INCREMENT] = g; // Set the green value of this pixel
        packed_colors[i + BLUE_INCREMENT] = b; // Set the blue value of this pixel
    }
}

// Width max is 32
// Will set a certain length of display to a certain color
void colorPackingMasked(char *packed_colors, uint32_t rgbValue, uint32_t width, uint32_t mask, uint32_t mag)
{
    char r = (char) ((rgbValue >> RED_OFFSET) & RGB_MASK); // Get the red portion of an rgbvalue
    char g = (char) ((rgbValue >> GREEN_OFFSET) & RGB_MASK); // Get the green portion of an rgbvalue
    char b = (char) (rgbValue & RGB_MASK); // Get the blue portion of an rgbvalue

    char isColor = 0;

    // Iterate through each pixel for the commanded length to set it to the rgb value
    for (uint32_t i = 0; i < width; i++)
    {
        isColor = RESET;

        // Reads the bits in the bitmap
        if (mask & (START_OF_BITMAP_MASK << (width - INDEX_OFFSET - i)))
        {
            isColor = RGB_MASK;
        }

        // Magnifying means that we add extra pixels in that position
        for (int j = 0; j < mag; j++)
        {
            uint32_t pixel_index = (i * mag + j) * PIXEL_SIZE;

            packed_colors[pixel_index] = r & isColor; // Set the red value of this pixel
            packed_colors[pixel_index + GREEN_INCREMENT] = g & isColor; // Set the green value of this pixel
            packed_colors[pixel_index + BLUE_INCREMENT] = b & isColor; // Set the blue value of this pixel
        }
    }
}

// Write a pixel at a coorinate with a certain color
void video_writePixel(uint32_t x, uint32_t y, uint32_t rgbValue)
{
    setOffset(x, y); // Set the offset to the passed in coordinates
    char pixel[PIXEL_SIZE]; // Create an array representing the pixel
    colorPacking(pixel, rgbValue, PIXEL_SIZE); // Set the pixel to the correct color
    write(f, pixel, PIXEL_SIZE); // Write the pixel to the display
}

// Writes a line with a bitmask at a x, y
// 0 is background, 1 is color given
void video_writeMaskedLine(uint32_t x, uint32_t y, uint32_t rgbValue, uint32_t width, uint32_t mask, uint32_t mag)
{
    setOffset(x, y); // Set the offset to the passed in coordinates
    uint32_t line_size = PIXEL_SIZE * width * mag; // Calculate the total size of the line
    char line[line_size]; // Create an array that represents the line to be drawn
    colorPackingMasked(line, rgbValue, width, mask, mag); // Assign the line to an rgb value
    write(f, line, line_size); // Write the line to the display
}

// Writes a line at given location
void video_writeHLine(uint32_t x, uint32_t y, uint32_t rgbValue, uint32_t width)
{
    setOffset(x, y); // Set the offset to the passed in coordinates
    uint32_t line_size = PIXEL_SIZE * width; // Calculate the total size of the line
    char h_line[line_size]; // Create an array that represents the line to be drawn
    colorPacking(h_line, rgbValue, line_size); //Assign the line to an rgb value

    write(f, h_line, line_size); // Write the line to the display
}

// Writes the screen one color
void video_writeScreen(uint32_t rgbValue)
{
    setOffset(ORIGIN_VALUE_X, ORIGIN_VALUE_Y); // Set the offset to the passed in coordinates
    uint32_t size_of_screen = PIXEL_SIZE * VIDEO_SCREEN_WIDTH *
                              VIDEO_SCREEN_HEIGHT; // Find the total size of the screen by calculating the total number of pixels and multiplying that by PIXEL_SIZE
    char screen[size_of_screen]; // Create an array of chars representing the screen
    colorPacking(screen, rgbValue, size_of_screen); // Set the screen array to the appropriate color

    write(f, screen, size_of_screen); // Set the entire screen to the specified color
}

// Reads the screen at a location and reads the pixel color
uint32_t video_readPixel(uint32_t x, uint32_t y)
{
    setOffset(x, y); // Set the offset to the passed in coordinates
    char buffer[PIXEL_SIZE];    // Buffer used to read the pixel colors
    uint32_t rgbValue;          // Value of color returned
    read(f, buffer, PIXEL_SIZE);// Read the location and return the color through the buffer

    // For some reason the buffer returns the value as GBR
    // This turns it from GBR to RGB
    rgbValue = (uint32_t) (buffer[READ_BLUE_INDEX] << SHIFT_BLUE) +
               (uint32_t) (buffer[READ_GREEN_INDEX] << SHIFT_GREEN) +
               (uint32_t) (buffer[READ_RED_INDEX] << SHIFT_RED);
    return rgbValue;
}

// Closes the video driver
void video_exit()
{
    close(f);
}
