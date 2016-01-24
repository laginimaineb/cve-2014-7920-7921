#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <media/AudioSystem.h>
#include <media/IAudioFlinger.h>
#include <media/IAudioPolicyService.h>

#include <stdlib.h>

//The value used to indicate that a DWORD is not zero
#define NOT_ZERO (1)

//The value used to indicate that a DWORD is zero
#define ZERO     (0)

//The length of a row of the memory map, when printed using print_memory_map.
#define MEMORY_MAP_ROW_LEN (10)

namespace android {

/**
 * Searches for the given template memory map from the given start offset, until the given end offset.
 * @param service The handle to the AudioPolicyService, using which the binder call is made.
 * @param template_map The memory map to search for (comprised of ZERO/NOT_ZERO values).
 * @param template_map_size The size, in bytes, of hte template_map for which to search.
 * @param start_offset The starting offset from the mRefCount field, from which to start searching.
 * @param end_offset The end offset from the mRefCount field, until which to search.
 * @param match_offset The resulting match offset (valid only if the return value is zero!)
 * @return Zero if successful, and a negative linux error code otherwise.
 * @note This function assumes the start_offset and end_offset arguments are *negative*!
 */
int find_memory_map(const sp<IAudioPolicyService>& service, const unsigned char* template_map, uint32_t template_map_size,
                    int start_offset, int end_offset, int& match_offset);


/**
 * Outputs the given memory map to the standard output.
 * @param map The pointer to the memory map.
 * @param size The size, in bytes, of the memory map.
 */
void print_memory_map(const unsigned char* map, int size);

}


#endif
