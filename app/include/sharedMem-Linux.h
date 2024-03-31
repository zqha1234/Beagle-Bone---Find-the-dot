#ifndef _SHAREDMEM_H_
#define _SHAREDMEM_H_

// created the thread, and receive accelerometer info from accelerometer.c,
// and update the shared struct

// initiated the thread
void SharedMem_init(void);
// clean up the thread
void SharedMem_cleanup(void);

#endif