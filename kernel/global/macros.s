#include "kernel/global/definitions.s"

/* NASM:
 *   %define no_offset(a) a-offset
 *
 * GAS + CPP (for .S files):
 */
#define no_offset(a) ((a) - offset)
