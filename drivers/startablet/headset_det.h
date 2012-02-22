
#ifndef _STAR_HEADSET_DET_H
#define _STAR_HEADSET_DET_H

#include <sound/jack.h>
#include <sound/soc.h>
#include <sound/core.h>

typedef enum {
    STAR_NONE,
	STAR_HEADSET,
	STAR_HEADPHONE,

}headset_type_enum;

typedef enum {
    HOOK_PRESSED,
	HOOK_RELEASED
}hook_status_enum;

unsigned int get_headset_type(void);
int set_headset_jack(struct snd_soc_jack *j);

#endif /* _LINUX_SYNAPTICS_I2C_RMI_H */

