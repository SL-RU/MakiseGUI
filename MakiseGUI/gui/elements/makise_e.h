#ifndef MAKISE_E_H
#define MAKISE_E_H      1

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_gui_elements.h"
#include "makise_gui_container.h"

#include "fonts.h"
#include "makise_e_buttons.h"
#include "makise_e_canvas.h"
#include "makise_e_lable.h"
#include "makise_e_progress_bar.h"
#include "makise_e_slider.h"
#include "makise_e_slist.h"
#include "makise_e_tabs.h"
#include "makise_e_text_field.h"
#include "makise_e_toggle.h"

void _m_e_helper_draw_box(MakiseBuffer* b, MPosition *p, MakiseStyleTheme *th );

#ifdef __cplusplus
}
#endif

#endif
