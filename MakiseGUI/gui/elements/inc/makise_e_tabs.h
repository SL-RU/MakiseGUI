#ifndef MAKISE_E_TABS_H
#define MAKISE_E_TABS_H

#include "makise_config.h"

#if ( MAKISE_E_TABS > 0 )

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

//tabs
typedef struct _MTabs_Tab {
    char*           header;
    MContainer      cont;
} MTabs_Tab;

//type of header
typedef enum {
    MTabs_Type_Up,  //show tabs on the top
    MTabs_Type_Left,//show tabs on the left
    MTabs_Type_None //don't show tabs
}MTabs_Type;

typedef struct _MTabs MTabs;
typedef struct _MTabs {
    MakiseGUI*          gui;

    MElement            el;         //element of host
    MElement            tabs_el;    //element of tabs
    MElement            header_el;  //element of header
    MContainer          host;       //contains tabs & tab header

    MTabs_Tab*          tabs;       //tabs array
    uint32_t            len;        //count of tabs

    uint32_t            selected;   //index of selected tab

    MTabs_Type          type;       //type of tab's names to show
    uint32_t            size;       //height or width of tabs header

    MakiseStyle*        style;
    uint8_t             state;      //focus state
} MTabs;

void m_create_tabs( MTabs*          b,
                    MContainer*     c,
                    MPosition       pos,
                    MTabs_Tab*      tabs,
                    uint32_t        len,
                    MTabs_Type      type,
                    uint32_t        size,
                    MakiseStyle*    style );

void m_tabs_select_tab          ( MTabs *t, uint8_t tab );
void m_tabs_select_tab_back     ( MTabs *t );
void m_tabs_select_tab_next     ( MTabs *t );

#ifdef __cplusplus
}
#endif

#endif

#endif
