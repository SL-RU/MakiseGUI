#include "makise_gui_interface.h"

#if MAKISE_MUTEX
#define MM_REQUEST(e) MAKISE_MUTEX_t e_mutex = e->host->mutex; MAKISE_MUTEX_REQUEST(e_mutex)
#define MM_RELEASE(e) MAKISE_MUTEX_RELEASE(e_mutex)
#else
#define MM_REQUEST(...)
#define MM_RELEASE(...)
#endif

void mi_cont_add(MContainer *cont, MElement *el)
{
    if(cont == 0 || cont->host == 0)
	return;
    MM_REQUEST(cont);

    makise_g_cont_add(cont, el);
    MM_RELEASE(cont);
}
void mi_cont_rem(MElement *el)
{
    if(el == 0 || el->host == 0)
	return;
    MM_REQUEST(el);
    makise_g_cont_rem(el);
    MM_RELEASE(el);
}
void mi_cont_clear(MContainer *cont)
{
    if(cont == 0 || cont->host == 0)
	return;
    MM_REQUEST(cont);

    makise_g_cont_clear(cont);
    MM_RELEASE(cont);
}
int32_t mi_cont_insert(MContainer * cont, MElement *el, uint32_t index)
{
    if(cont == 0 || cont->host == 0)
	return -1;
    MM_REQUEST(cont);

    int32_t e = makise_g_cont_insert(cont, el, index);
    MM_RELEASE(cont);
    return e;
}
void mi_cont_replace(MElement *e1, MElement *e2)
{
    if(e1 == 0 || e1->host == 0)
	return;
    MM_REQUEST(e1);
#if MAKISE_MUTEX
    MAKISE_MUTEX_t m2 = 0;
    if( e1->host != e2->host && e2->host != 0) {
        m2 = e2->host->mutex;
        MAKISE_MUTEX_REQUEST(m2);
    }
#endif
    makise_g_cont_replace(e1, e2);
    MM_RELEASE(e1);
#if MAKISE_MUTEX
    if(m2 != 0)
        MAKISE_MUTEX_REQUEST(m2);
#endif
}
MFocusEnum mi_focus(MElement *el, MFocusEnum event)
{
    if(el == 0 || el->host == 0)
	return M_G_FOCUS_ERROR;
    MM_REQUEST(el);
    
    MFocusEnum e = makise_g_focus(el, event);
    
    MM_RELEASE(el);
    return e;
}
MFocusEnum mi_focus_next(MHost *host)
{
    if(host == 0)
	return M_G_FOCUS_ERROR;
    MAKISE_MUTEX_REQUEST(&host->mutex);

    MFocusEnum e = makise_g_host_focus_next(host);
    MAKISE_MUTEX_RELEASE(&host->mutex);
    return e;
}
MFocusEnum mi_focus_prev(MHost *host)
{
    if(host == 0)
	return M_G_FOCUS_ERROR;
    MAKISE_MUTEX_REQUEST(&host->mutex);

    MFocusEnum e = makise_g_host_focus_prev(host);
    MAKISE_MUTEX_RELEASE(&host->mutex);
    return e;
}
