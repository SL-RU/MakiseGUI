#ifndef _MAKISE_H_G_EVENTS
#define _MAKISE_H_G_EVENTS

#ifdef __cplusplus
extern "C" {
#endif

/*
Makise Event Manager - MEM

Logic:
1) When some element needs to be changed, accordant event  must be added by mem_add_event(...)
2) Right before any call need to be executed mem_execute_all(...). It will apply all required changes.

Buffer - buffer for dynamic allocation of events.
Queue  - queue of added events.
*/

#include "makise_config.h"    

#ifndef MAKISE_EVENT_BUFFER_LEN
#define MAKISE_EVENT_BUFFER_LEN 4096
#endif
#ifndef MAKISE_EVENT_QUEUE_LEN
#define MAKISE_EVENT_QUEUE_LEN 50
#endif    
    
// Types of targets
typedef enum {
    MEMType_Element,    //change element properties
    MEMType_ElementSet, //set element property
    MEMType_Container,  //change parents, children etc
    MEMType_Position,   //change MElement's position
} MEM_TargetType;

typedef struct _MEM_Event MEM_Event;    
// Structure with single event's data     
typedef struct _MEM_Event {
    MEM_TargetType type ; // type of the targer
    void * target       ; // element, container or etc
    void * field        ; // pointer to the field
    uint32_t action     ; // id of the action
    void * value        ; // new value(structure or etc)
    size_t value_len    ; // sizeof value
    void (*callback)(MEM_Event *ev);
} MEM_Event;

// structure in MHost with event's buffer and etc
typedef struct _MEM_Host {
    uint32_t events_i;
    MEM_Event events[MAKISE_EVENT_QUEUE_LEN];
    uint32_t buffer_i;
    uint8_t buffer[MAKISE_EVENT_BUFFER_LEN];   
} MEM_Host;

#include "makise_gui.h"

/**
 * Init event manager
 *
 * @param host Host
 * @return 
 */
void mem_init(MHost *host);
/**
 * Add new event to event buffer
 * 
 * @param host MHost of target element 
 * @param event event
 * @return void
 */
void mem_add_event(MHost *host, MEM_Event event);    
/**
 * Add new event to event buffer
 * 
 * @param host MHost of target element 
 * @param event event
 * @return void
 */
void mem_add_event(MHost *host, MEM_Event event);
/**
 * Allocate memory for buffer in MEM_Host's event buffer. It will exist only before mem_execute_all()
 * 
 * @param host MHost of target element 
 * @param len size of allocated buffer
 * @return -1 if error and pointer if OK
 */
void * mem_getbuffer(MHost *host, uint32_t len);
/**
 * Execute event
 *
 * @param host Host
 * @param ev Event
 * @return 
 */
void mem_execute(MHost *host, MEM_Event *ev);
/**
 * Execute all events and free MEM_Host's event buffer
 *
 * @param host Host
 * @return 
 */    
void mem_execute_all(MHost *host);
    
#ifdef __cplusplus
}
#endif
    
#endif
