#ifndef KP_ERROR_H_INCLUDED
#define KP_ERROR_H_INCLUDED

/* NOTE: This is not the definitive implementation, it's an idea. */

typedef struct {
  char *file;
  char *func;
  unsigned int line;
  unsigned int code;
  char *message;
} kp_error_t;

#define kp_set_error_sys(err) \
  (kp_set_error (err, __FILE__, __func__, __LINE__, errno, strerror (errno)))

#endif
