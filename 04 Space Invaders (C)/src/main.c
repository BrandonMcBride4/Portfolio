#include "btns/btns.h"
#include "fit/fit.h"
#include "switches/switches.h"
#include "intc/intc.h"
#include "video/video.h"
#include "tank/tank.h"
#include "tank_bullet/tank_bullet.h"
#include "alien_bullet/alien_bullet.h"
#include "image_render/image_render.h"
#include "saucer/saucer.h"
#include "globals/globals.h"
#include "aliens/aliens.h"
#include "control/control.h"
#include "user_audio/user_audio.h"
#include <stdio.h>
#include <stdbool.h>

#define BTNS_UIO_PATH      "/dev/uio1"  // Path to button's driver
#define SWITCHES_UIO_PATH  "/dev/uio2"  // Path to switches driver
#define USER_INTC_UIO_PATH "/dev/uio4"  // Path to interrupt controller driver
#define VIDEO_DRIVER_PATH  "/dev/ecen427_hdmi" // Path to video drvier

#define FIT_MASK  0x00000001            // Interrupt on bit 1 is the FIT
#define BTNS_MASK 0x00000002            // Interrupt on bit 2 is the buttons
#define SW_MASK   0x00000004            // Interrupt on bit 3 is the switches
#define PIT_MASK  0x00000008            // Interrupt on bit 4 is the PIT

#define BTN_0 1                // Mask that Corresponds to button zero
#define BTN_1 2                // Mask that Corresponds to button one
#define BTN_2 4                // Mask that Corresponds to button two

#define FIT_TICK_TIME 10                 // fit ticks every 10 ms
#define DEBOUNCE_TIME 50 / FIT_TICK_TIME // How long for button to be debounced

#define SW_0_MASK 1        // Mask to indicate the value of switch one
#define RESET 0            // Used to reset counter values
#define IS_MULTIPLE 0        // Indicates that a value is a multiple of a certain number when modulated with that number
#define NO_BUTTONS_PRESSED 0// When the button value is zero, no buttons are being pressed

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
static uint32_t btn_value = 0;            //Represents the buttons that are being pressed
static uint32_t switches_value = 0;        //Represents the switches that are being pressed

static bool is_btns_changed = false;        //Indicates that the value of the buttons has been changed within the debounce time
static bool is_switches_changed = false;    //Indicates that the value of the switches has been changed within the debounce time
static bool isVolumeButtonPressed = false;  // Indicates that the button used for volume is currently pressed
static bool isVolumeButtonReleased = false; // Indicates that the button used for volume is just been released

static uint32_t fit_counter = 0;            // Counter that is incremented each time the FIT ISR is accessed

void init_all()
{
    // Initialize interrupt controller driver
    intc_init(USER_INTC_UIO_PATH);

    // Initialize buttons
    btns_init(BTNS_UIO_PATH);

    //Initialize switches
    switches_init(SWITCHES_UIO_PATH);

    // Initialize audio
    user_audio_init();

    // Initialize the video
    video_init(VIDEO_DRIVER_PATH);

    // Initialize controller
    control_init();

}

// Checks if volume button was pressed and where volume switch is at
void changeVolume()
{
    // If switch 0 is up then volume up
    if(globals_getSwValue() & SW_0_MASK)
    {
        user_audio_volumeUp();
    }
    else // else volume down
    {
        user_audio_volumeDown();
    }
}

void debounceButtons()
{
    static uint32_t btns_debounce_counter = 0;   // Counter for indicating if enough time has passed for the buttons to be considered debounced
    static bool is_btn_debounced = false;        // Boolean variable indicating that the buttons have been debounced

    // Debounce buttons
    if (!is_btns_changed)
    {
        // Goes through this statement once when it is debounced
        if ((btns_debounce_counter >= DEBOUNCE_TIME) && !is_btn_debounced)
        {
            is_btn_debounced = true;
            globals_setBtnValue(btn_value);            //Set debounced_button_value to the current button balue that is being read
            //printf("Button value: %d\n", btn_value);
        }
            //If the debounce counter hasn't yet reached its threshold, continute incrementing the counter while button value is consistent
        else if (btns_debounce_counter < DEBOUNCE_TIME)
        {
            btns_debounce_counter++;
        }
    }
    else  //If the button value has changed since the last FIT isr
    {
        is_btns_changed = false;        //Reset the boolean indicating that the button value has changed
        is_btn_debounced = false;        //Button is not debounced if button value changes
        btns_debounce_counter = RESET;        //Reset the debounce counter because button value has changed
    }
}

void debounceSwitches()
{
    static uint32_t switches_debounce_counter = 0;    // Counter for indicating if enough time has passed for the switches to be considered debounced

    // Debounce switches
    if (!is_switches_changed) //Switch value is the same as it was in the last fit isr
    {
        //Switch is debounced when a consistent value is read for a certain amount of time
        if (switches_debounce_counter >= DEBOUNCE_TIME)
        {
            globals_setSwValue(switches_value);    //Set debounced_button_value to the current button balue that is being read
        }
        else
        {
            switches_debounce_counter++;    //Increment counter until it reaches the debounce threshhold
        }

    }
    else  //Switch value has changed since the last fit isr
    {
        is_switches_changed = false;
        switches_debounce_counter = RESET;    //Start the counter over again because the switch value has changed
    }
}

// Debounces buttons, and switches, as well as tick functions
// @param interruptMask: used to acknowledge the interrupt, it is either the mask for the FIT or PIT
void isr_timer(uint32_t interruptMask)
{
    debounceButtons();
    debounceSwitches();
    control_detectRelease(&isVolumeButtonPressed, &isVolumeButtonReleased, BTN_1); // BTN1 is used for volume
    if (isVolumeButtonReleased)
    {
        changeVolume();
    }

    //Run tick functions:
    tank_tick();
    tank_bullet_tick();
    saucer_tick();
    aliens_tick();
    control_tick();
    alien_bullet_tick();
    intc_ack_interrupt(interruptMask);        //Acknowledge interrupt
}


// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons()
{
    is_btns_changed = true;     // Used to debounce
    // Read the state of the buttons
    btn_value = btns_read();

    // Acknowledge the button interrupt
    btns_ack_interrupt();
    intc_ack_interrupt(BTNS_MASK);
}

// This is invoked each time there is a change in the switch state (result of a push or a bounce).
void isr_switches()
{
    is_switches_changed = true;
    // Read the state of the switches
    switches_value = switches_read();

    // Acknowledge the switches interrupt
    switches_ack_interrupt();
    intc_ack_interrupt(SW_MASK);
}

int main(void)
{
    init_all();

    while (btn_value != 10)
    {
        // Call interrupt controller function to wait for interrupt
        uint32_t interrupts = intc_wait_for_interrupt();
        // Check which interrupt lines are high and call the appropriate ISR functions
        if (interrupts & FIT_MASK)
            isr_timer(FIT_MASK);
        if (interrupts & BTNS_MASK)
            isr_buttons();
        if (interrupts & SW_MASK)
            isr_switches();
        if (interrupts & PIT_MASK)
        {
//            printf("PIT interrupt\n");
//            intc_ack_interrupt(PIT_MASK);
            isr_timer(PIT_MASK);
        }
//            isr_timer(PIT_MASK);
    }

    intc_exit();
    btns_exit();
    fit_exit();
    user_audio_exit();
}
