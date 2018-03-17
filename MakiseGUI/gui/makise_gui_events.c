#include "makise_gui_events.h"

void mem_init(MHost *host)
{
    MEM_Host *h = &host->mem;
    h->buffer_i = 0;
    h->events_i = 0;
}
void mem_add_event(MHost *host, MEM_Event event)
{
    MAKISE_MUTEX_REQUEST(host->mutex);
    MEM_Host *h = &host->mem;
    if(MAKISE_EVENT_QUEUE_LEN <= h->events_i) {
	// If queue is full
	// then we'll execute all stored events and free the queue
	MAKISE_MUTEX_RELEASE(host->mutex);
	mem_execute_all(host);
	MAKISE_MUTEX_REQUEST(host->mutex);
	//Now queue and buffer are freed
    }
    h->events[h->events_i] = event;
    h->events_i ++;
    
    MAKISE_MUTEX_RELEASE(host->mutex);
}
void * mem_getbuffer(MHost *host, uint32_t len)
{
    MAKISE_MUTEX_REQUEST(host->mutex);
    if(len + host->mem.buffer_i > MAKISE_EVENT_BUFFER_LEN)
    {
	MAKISE_MUTEX_RELEASE(host->mutex);
	//if no free space in buffer left
	return (void*)(-1);
    }
    void *p = &host->mem.buffer[host->mem.buffer_i];
    host->mem.buffer_i += len;
    
    MAKISE_MUTEX_RELEASE(host->mutex);
    
    return p;
}
void mem_execute(MHost *host, MEM_Event *ev)
{
    MAKISE_MUTEX_REQUEST(host->mutex);
    switch (ev->type) {
    case MEMType_Position: {
	MElement *el = (MElement*)(ev->target);
	el->position = *(MPosition*)ev->value;
	break;
    }
    case MEMType_Element: {
	MElement *el = (MElement*)(ev->target);
	el->exec_event(el, ev);
	break;
    }
    case MEMType_ElementSet: {
	//MElement *el = (MElement*)(ev->target);
	memcpy(ev->field, &ev->value, ev->value_len);
	break;
    }

    case MEMType_MI: {
	//MContainer *c = (MContainer*)(ev->target);
	
	break;
    }	
       
    default:
	break;
    }
    MAKISE_MUTEX_RELEASE(host->mutex);
}
void mem_execute_all(MHost *host)
{
    MAKISE_MUTEX_REQUEST(host->mutex);
    MEM_Host *h = &host->mem;
    if(h->events_i != 0)
    {
	mem_execute(host, &h->events[0]);
    }
    h->events_i = 0;
    h->buffer_i = 0;
    
    MAKISE_MUTEX_RELEASE(host->mutex);
}
