/* system includes */
#include <assert.h>
#include <endian.h>
#include <errno.h>
#include <openssl/rand.h>

/* keepass includes */
#include "uuid.h"

void
kp_bytes_reverse (unsigned char *bytes, unsigned int offset, unsigned int len)
{
  unsigned char byte;
  unsigned int i, j, n;
  int uneven;

  n = len / 2;
  if ((float)len % 2)
    uneven = 1;
  else
    uneven = 0;

  for (i = offset; i < n; i++) {
    j = (i + uneven + (n * 2)) - 1;
    byte = bytes[i];
    bytes[i] = bytes[j];
    bytes[j] = byte;
  }
}

/* Used the implementation from KeePass 2.x and sources from Mono as a
   reference. NOTE: this code is not tested yet, it might not produce the
   same results as the original sources (using the same input of course ;)) */
char *
kp_uuid_create (kp_error_t *err)
{
  unsigned char *uuid, zero[KP_UUID_SIZE];

  memset (zero, '\0', KP_UUID_SIZE);
  if (! (uuid = calloc (KP_UUID_SIZE, sizeof (char)))) {
    kp_set_error_sys (err);
    goto error;
  }

  do {
    if (! RAND_pseudo_bytes (uuid, KP_UUID_SIZE)) {
      kp_set_error_crypto (err);
      goto error;
    }
  /* Zero is a reserved value -- do not generate Zero */
  } while (memcmp (uuid, zero, KP_UUID_SIZE) == 0);

#ifndef __BIG_ENDIAN
  kp_bytes_reverse (uuid, 0, 4);
  kp_bytes_reverse (uuid, 4, 2);
  kp_bytes_reverse (uuid, 6, 2);
#endif

  /* Mask in Variant 1-0 in Bit[7..6] */
  uuid[8] = ((uuid[8] & 0x3f) | 0x80);
  /* Mask in Version 4 (random based Guid) in Bits[15..13] */
  uuid[6] = ((uuid[6] & 0x0f) | 0x40);
  uuid[7] = ((uuid[7] & 0xff) | 0x00);

  return uuid;
error:
  if (uuid)
    free (uuid);
  return NULL;
}

