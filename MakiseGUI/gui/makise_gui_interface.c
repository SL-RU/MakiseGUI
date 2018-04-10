#include "makise_gui_interface.h"

void mi_cont_add(MContainer *cont, MElement *el)
{
    if(cont == 0 || cont->host == 0)
	return;
    MHost *h = cont->host;
    MAKISE_MUTEX_REQUEST(&h->mutex);

    makise_g_cont_add(cont, el);
    MAKISE_MUTEX_RELEASE(&h->mutex);
}
void mi_cont_rem(MElement *el)
{
    if(el == 0 || el->host == 0)
	return;
    MHost *h = el->host;
    MAKISE_MUTEX_REQUEST(&h->mutex);
    makise_g_cont_rem(el);
    MAKISE_MUTEX_RELEASE(&h->mutex);
}
void mi_cont_clear(MContainer *cont)
{
    if(cont == 0 || cont->host == 0)
	return;
    MHost *h = cont->host;
    MAKISE_MUTEX_REQUEST(&h->mutex);

    makise_g_cont_clear(cont);
    MAKISE_MUTEX_RELEASE(&h->mutex);
}
int32_t mi_cont_insert(MContainer * cont, MElement *el, uint32_t index)
{
    if(cont == 0 || cont->host == 0)
	return -1;
    MHost *h = cont->host;
    MAKISE_MUTEX_REQUEST(&h->mutex);

    int32_t e = makise_g_cont_insert(cont, el, index);
    MAKISE_MUTEX_RELEASE(&h->mutex);
    return e;
}
void mi_cont_replace(MElement *e1, MElement *e2)
{
    if(e1 == 0 || e1->host == 0)
	return;
    MHost *h = e1->host;
    MAKISE_MUTEX_REQUEST(&h->mutex);

    makise_g_cont_replace(e1, e2);
    MAKISE_MUTEX_RELEASE(&h->mutex);
}
MFocusEnum mi_focus(MElement *el, MFocusEnum event)
{
    if(el == 0 || el->host == 0)
	return M_G_FOCUS_ERROR;
    MHost *h = el->host;
    MAKISE_MUTEX_REQUEST(&h->mutex);

    MFocusEnum e = makise_g_focus(el, event);
    MAKISE_MUTEX_RELEASE(&h->mutex);
    return e;
}
MFocusEnum mi_focus_next(MHost *host)
{
    if(host == 0)
	return M_G_FOCUS_ERROR;
    MHost *h = host;
    MAKISE_MUTEX_REQUEST(&h->mutex);

    MFocusEnum e = makise_g_host_focus_next(host);
    MAKISE_MUTEX_RELEASE(&h->mutex);
    return e;
}
MFocusEnum mi_focus_prev(MHost *host)
{
    if(host == 0)
	return M_G_FOCUS_ERROR;
    MHost *h = host;
    MAKISE_MUTEX_REQUEST(&h->mutex);

    MFocusEnum e = makise_g_host_focus_prev(host);
    MAKISE_MUTEX_RELEASE(&h->mutex);
    return e;
}
