#ifndef MAKISE_E_MESSAGE_WINDOW_H
#define MAKISE_E_MESSAGE_WINDOW_H

#include "makise_config.h"

#if ( MAKISE_E_MESSAGE_WINDOW > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

// This structure is populated by the user.
typedef struct {
	const MakiseFont*		const font;
	const uint32_t				font_line_spacing;

	const uint32_t				bg_color;
	const uint32_t				font_col;
	const uint32_t				border_color;
} MakiseStyle_SMessageWindow;


// Structure for internal use.
// May not be filled 0.
typedef struct {
	MakiseGUI*								gui;
	MElement								el;
	const char*						 		s;
	const MakiseStyle_SMessageWindow*		style;
} MMessageWindow;

void m_create_message_window (	MMessageWindow*						b,
								MContainer*							c,
								MPosition							pos,
								const char*							const s,
								const MakiseStyle_SMessageWindow*	style );


#ifdef __cplusplus
}
#endif

#endif

#endif
