#ifndef KP_UUID_H_INCLUDED
#define KP_UUID_H_INCLUDED

/* keepass includes */
#include "error.h"

#define KP_UUID_SIZE (16) /* 16 bytes == 128 bits */

char *kp_uuid_create (kp_error_t *);

#endif
