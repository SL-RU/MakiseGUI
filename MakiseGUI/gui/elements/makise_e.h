#ifndef MAKISE_E_H
#define MAKISE_E_H 1

#include "makise_gui_elements.h"
#include "makise_gui_container.h"

//Button - simple button. executes click() function when OK clicked.
typedef struct _MButton MButton;
typedef struct _MButton {
    MakiseGUI *gui;
    MElement el;

    char* text;
    MakiseStyle* style;

    void    (*click   )(MButton* b);                 //When OK button clicked
    uint8_t (*onkey   )(MButton* b, MInputData data);//Handle any key pressed on button. Return M_INPUT_HANDLED if was handled(then click method won't be called), and M_INPUT_NOT_HANDLED if not handled(click method will be called).
    void    (*onfocus )(MButton* b, MFocusEnum type);//type == M_G_FOCUS_GET when focus recieved and M_G_FOCUS_LEAVE when dropped.

    uint8_t state;    
} MButton;

void m_create_button(MButton* b, MContainer *c,
		     int32_t x, int32_t y, uint32_t w, uint32_t h,
		     char* text,
		     void    (*click   )(MButton* b),
		     uint8_t (*onkey   )(MButton* b, MInputData data),
		     void    (*onfocus )(MButton* b, MFocusEnum type),
		     MakiseStyle *style);
/* void m_button_set_click(MButton *b, void (*click   )(MButton* b)); */
/* void m_button_set_onkey(MButton *b, uint8_t (*onkey)(MButton* b, MInputData data)); */
/* void m_button_set_onfocus(MButton *b, void (*onfocus)(MButton* b, MFocusEnum type));*/

//Canvas - simple container. It's placing elements simply by their position wherever it needed
typedef struct {
    MakiseGUI *gui;
    MElement el;

    MContainer cont;
    
    MakiseStyle* style;
    
    uint8_t state;    
} MCanvas;

void m_create_canvas(MCanvas* b, MContainer *c,
		     int32_t x, int32_t y, uint32_t w, uint32_t h,
		     MakiseStyle *style);

//Lable. Simply draws required text. It can be only in one line. And it won't be cutting and folding text.
typedef struct {
    MakiseGUI *gui;
    MElement el;

    char *text;
    
    MakiseStyle* style;
} MLable;

void m_create_lable(MLable* b, MContainer *c,
		    int32_t x, int32_t y, uint32_t w, uint32_t h,
		    char* text,
		    MakiseStyle *style);

//This one will display multiline text with multiline, folding and cutting support.
typedef struct {
    MakiseGUI *gui;
    MElement el;

    char *text;
    
    MakiseStyle* style;
} MTextField;

void m_create_text_field(MTextField* b, MContainer *c,
			 int32_t x, int32_t y, uint32_t w, uint32_t h,
			 char* text,
			 MakiseStyle *style);
//Simple list element.
//Can display items. Supports scrolling.
//Modes: list, radio buttons, checkboxes.
typedef enum _MSList_Type
{
    MSList_List,
    MSList_RadioButton,
    MSList_Checkbox
} MSList_Type;
typedef struct _MSList_Item MSList_Item;
typedef struct _MSList MSList;
typedef struct _MSList_Item
{
    MSList_Item *prev;
    MSList_Item *next;

    uint32_t id;       //custom id, if NOT is_array, else - position in the array(will be setted automatically by MSList).
    char* text;        //text of item
    uint32_t value;    //value. It may be checkbox or radio button status
} MSList_Item;

typedef struct _MSList {
    MakiseGUI *gui;
    MElement el;

    char *text;

    MSList_Item *items;//item's array pointer or pointer to the first element of the list
    uint8_t is_array;  //if 1 then items will be
    uint32_t len;      //matters only if is_array

    MSList_Item *selected;

    void (*onselection)(MSList *l, MSList_Item *selected);//when selected item is changing
    void (*click)(MSList *l, MSList_Item *selected);      //when OK button clicked

    MSList_Type type;
    MakiseStyle* style;
    MakiseStyle *item_style;

    uint32_t state;
} MSList;

void m_create_slist(MSList* b, MContainer *c,
		    int32_t x, int32_t y, uint32_t w, uint32_t h,
		    char* text,
		    void (*onselection)(MSList *l, MSList_Item *selected),
		    void (*click)(MSList *l, MSList_Item *selected),
		    MSList_Type type,
		    MakiseStyle *style,
		    MakiseStyle *item_style);
void m_slist_add(MSList *l, MSList_Item *item); //add one item to the list at the end. Only if NOT is_array. 
void m_slist_clear(MSList *l); //clear all pointers
void m_slist_remove(MSList *l, MSList_Item *item); //remove item from linked list. Only if NOT is_array.
void m_slist_set_array(MSList *l, MSList_Item *array, uint32_t len); //set new data source. Simple array.
void m_slist_set_list(MSList *l, MSList_Item *first); //set linked list as new data source.
/* void m_slist_set_onselection(MSList *l, void (*onselection)(MSList *l, MSList_Item selected)); */
/* void m_slist_set_click(MSList *l, void (*click)(MSList *l, MSList_Item selected)); */


typedef struct _MSlider MSlider;
typedef struct _MSlider {
    MakiseGUI *gui;
    MElement el;

    char* text;
    int32_t value;
    int32_t value_max;
    int32_t value_min;

    uint8_t show_value; //is required to display value on the screen
    
    MakiseStyle* style;

    void    (*onchange   )(MSlider* b, uint32_t val);                 //When value changed by user
    void    (*onfocus )(MSlider* b, MFocusEnum type);//type == M_G_FOCUS_GET when focus recieved and M_G_FOCUS_LEAVE when dropped.

    uint8_t state;    
} MSlider;

void m_create_slider(MSlider* b, MContainer *c,
		     int32_t x, int32_t y, uint32_t w, uint32_t h,
		     char* text,
		     int32_t value,
		     int32_t value_max,
		     int32_t value_min,
		     uint8_t show_val,
		     void    (*onchange   )(MSlider* b, uint32_t val),
		     void    (*onfocus )(MSlider* b, MFocusEnum type),
		     MakiseStyle *style);




void _m_e_helper_draw_box(MakiseBuffer* b, MPosition *p, MakiseStyleTheme *th);

#endif
