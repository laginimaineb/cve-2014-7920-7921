#ifndef __WRITE32_H__
#define __WRITE32_H__

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <media/AudioSystem.h>
#include <media/IAudioFlinger.h>
#include <media/IAudioPolicyService.h>


namespace android {

/**
 * Initializes the address of the audio_hw_device_t corresponding to the primary device, to the given address.
 * @parma address The absolute address of the audio_hw_device_t instance.
 */
void set_primary_device_address(uint32_t address);

/**
 * Sets the offset of the AudioOutputDescriptor.mRefCount to the audio_hw_device_t instance.
 * @param offset The offset, in DWORDs, between the two fields.
 */ 
void set_primary_device_offset(int offset);

/**
 * Writes the given 32-bit value to the given address.
 * @param af The handle the the AudioFlinger service used to perform the write.
 * @param aps The handle to the AudioPolicy service used to increment the global offset counter.
 * @param address The address to which the write will be performed.
 * @param value The value to write. 
 * @note Right now, only addresses after the address of the audio_hw_device_t can be written to.
 * @return Zero if successful, and a negative linux error code otherwise.
 */
int write32(const sp<IAudioFlinger>& af, const sp<IAudioPolicyService>& aps, uint32_t address, uint32_t value);

}

#endif
