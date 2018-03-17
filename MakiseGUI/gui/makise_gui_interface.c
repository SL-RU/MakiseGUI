#include "makise_gui_interface.h"


static void mi_add_event(MHost *host, MEM_INTERFACE_DATA data)
{
    MEM_INTERFACE_DATA *d =
	mem_getbuffer(host, sizeof(MEM_INTERFACE_DATA));
    *d = data;
    
    MEM_Event ev = (MEM_Event){
	.type = MEMType_MI,
	.target = host,
	.field = 0,
	.action = data.action,
	.value = d,
	.value_len = sizeof(MEM_INTERFACE_DATA)
    };

    mem_add_event(host, ev);
}
