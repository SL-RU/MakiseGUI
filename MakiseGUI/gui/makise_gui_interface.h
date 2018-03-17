#ifndef _MAKISE_H_INTERFACE
#define _MAKISE_H_INTERFACE

#ifdef __cplusplus
//extern "C" {
#endif


#include "makise_gui.h"

typedef enum {
    MEM_IA_ADD        ,
    MEM_IA_REM        ,
    MEM_IA_CLEAR      ,
    MEM_IA_INSERT     ,
    MEM_IA_REPLACE    ,
    MEM_IA_FOCUS      ,
    MEM_IA_FOCUS_NEXT ,
    MEM_IA_FOXUS_PREV ,
} MEM_INTERFACE_ACTION;

typedef struct _MEM_INTERFACE_DATA {
    MEM_INTERFACE_ACTION action;
    MHost                *host ;
    MContainer           *cont ;
    MElement             *e1   ;
    MElement             *e2   ;
} MEM_INTERFACE_DATA;


/**
 * add element to new container
 *
 * @param cont container
 * @param el element
 * @return 
 */
void mi_cont_add(MContainer *container, MElement *el);
/**
 * remove element from it's parent container
 *
 * @param el element
 * @return 
 */
void mi_cont_rem(MElement *el);
/**
 * Remove all children from container
 *
 * @param cont container
 * @return 
 */
void mi_cont_clear(MContainer *cont);
 /**
 * insert element in exact place of container children's list
 *
 * @param cont container
 * @param el element
 * @param index required index in the list
 * @return new index or -1 if error
 */
int32_t mi_cont_insert(MContainer * cont, MElement *el, uint32_t index);
/**
 * replace two elements in container's lists even they have different parents
 *
 * @param e1 
 * @param e2 
 * @return 
 */
void mi_cont_replace(MElement *e1, MElement *e2);
/**
 * This method will focus required element and all it's parents. Other already focused elements will be leaved by focus
 *
 * @param el 
 * @param event 
 * @return 
 */
MFocusEnum mi_focus(MElement *el, MFocusEnum event);
/**
 * Change focus to next element in the host's focus queue
 *
 * @param host host
 * @return MFocusEnum
 */
MFocusEnum mi_focus_next(MHost *host);
/**
 * Change focus to previous element in the host's focus queue
 *
 * @param host host
 * @return MFocusEnum
 */
MFocusEnum mi_focus_prev(MHost *host);

void mi_execute(MEM_Event event);

#ifdef __cplusplus
//}
#endif
#endif
