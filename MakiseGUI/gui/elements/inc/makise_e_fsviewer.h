#ifndef MAKISE_E_FSVIEWER_H
#define MAKISE_E_FSVIEWER_H

#define MAKISE_E_FSVIEWER_FATFS 2
#define MAKISE_E_FSVIEWER_STDIO 1

#include "makise_config.h"

#if ( MAKISE_E_FSVIEWER > 0 )
/*
If MAKISE_E_FSVIEWER == 1 then it'll use STDIO for files
If MAKISE_E_FSVIEWER == 2 then it'll use Chan's FATFS for files

Compatible Chan FatFS version is >=R0.12 
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"
#if MAKISE_E_FSVIEWER == MAKISE_E_FSVIEWER_FATFS
#include "ffconf.h"
#include "ff.h"
#else //STDIO
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>    
#include <unistd.h>
#endif //END std or fat
#include <string.h>

#define FM_BUFFERED 16 //how many lines are buffered

typedef struct {
    uint32_t bg_color;
    uint32_t font_col;
    uint32_t border_c;
    uint32_t icon_col;

    uint16_t double_border;
} MakiseStyleTheme_FSViewer_Item;


typedef struct
{
    const MakiseFont*       font;
    uint32_t                font_line_spacing;

    const MakiseBitmap*     bitmap_folder;
    
    //spacing set
    uint8_t                 left_margin;
    uint8_t                 item_margin; //vertical

    //scroll
    uint8_t                 scroll_width;
    uint32_t                scroll_bg_color; //may be 0
    uint32_t                scroll_color;

    MakiseStyleTheme  normal;
    MakiseStyleTheme  focused;
    MakiseStyleTheme  active;    
} MakiseStyle_FSViewer;

typedef struct
{
    const MakiseFont*       font;
    uint32_t                font_line_spacing;

    MakiseStyleTheme_FSViewer_Item  normal;
    MakiseStyleTheme_FSViewer_Item  focused;
    MakiseStyleTheme_FSViewer_Item  active;
} MakiseStyle_FSViewer_Item;


//Simple list element.
//Can display items. Supports scrolling.
//Modes: list, radio buttons, checkboxes.
typedef enum _MFSViewer_Type
{
    MFSViewer_Viewer,
    MFSViewer_SingleSelect,
//    MFSViewer_MultiSelect
} MFSViewer_Type;
typedef struct _MFSViewer_Item MFSViewer_Item;
typedef struct _MFSViewer MFSViewer;
typedef struct _MFSViewer_Item
{
#if MAKISE_E_FSVIEWER == MAKISE_E_FSVIEWER_FATFS    
    char name[_MAX_LFN + 2];        //text of item
    char fname[13];                 //short file name
    DWORD sclust;                 //start of clust
#else //STDIO
    char name[256];                 //text of item
    uint32_t inode;                 //start of clust
#endif //END std or fat   
    uint8_t am_dir;                 //is dir 
    uint32_t size;                  //file's size
    //uint8_t selected;               //is selected
    
    MFSViewer_Item *prev;
    MFSViewer_Item *next;

    uint32_t id;       //custom id, if NOT is_array, else - position in the array(will be computed automatically by MMFSViewer).
} MFSViewer_Item;

typedef struct _MFSViewer {
    MakiseGUI *gui;
    MElement el;

    char *header; //header. if ==0, then it won't show
    char *path;   //current folder's path

    MFSViewer_Item buffer[FM_BUFFERED]; //buffer for the linked list


    //for displaying great amount of files in folder. Caching
    uint32_t displayed_count; //how many lines are on the screen
    uint32_t files_count; //all files in directory
    uint32_t current_chunk_position; //current chunk. Chunk contains FM_BUFFERED lines

    //current position
    uint32_t current_position; //current position in folder
#if MAKISE_E_FSVIEWER == MAKISE_E_FSVIEWER_FATFS    
    DWORD current_folder; //current folder's sclust
#else //STDIO
    ino_t current_folder; //current folder's inode
#endif //END std or fat   
    //selected file
    uint8_t was_selected; //Flag
    uint32_t selected_file_id; //selected file's id
#if MAKISE_E_FSVIEWER == MAKISE_E_FSVIEWER_FATFS        
    char selected_file[14]; //selected file's name
    DWORD selected_folder; //folder which contains selected file. It's sclust
#else //STDIO
    char selected_file[256]; //selected file's name
    ino_t selected_folder; //folder which contains selected. inode
#endif //END std or fat
    
    uint8_t (*onselection)(MFSViewer *l, MFSViewer_Item *selected); //when selected item is changing. return 1 for accepting and 0 to not
    void (*click)(MFSViewer *l, MFSViewer_Item *selected);      //when OK button clicked

    void (*done)(MFSViewer *l, MFSViewer_Item *list);      //when all required elements are selected

    MFSViewer_Type type;
    MakiseStyle_FSViewer* style;
    MakiseStyle_FSViewer_Item *item_style;

    uint32_t state; //focus state
} MFSViewer;

void m_create_fsviewer(MFSViewer* b, MContainer *c,
		       MPosition pos,
		       char* header,
		       uint8_t (*onselection)(MFSViewer *l, MFSViewer_Item *selected),
		       void (*click)(MFSViewer *l, MFSViewer_Item *selected),
		       MFSViewer_Type type,
		       MakiseStyle_FSViewer *style,
		       MakiseStyle_FSViewer_Item *item_style);
void m_fsviewer_deselect(MFSViewer *l); //deselect all
void m_fsviewer_refresh(MFSViewer *l); //set linked list as new data source.
void m_fsviewer_loadchunk(MFSViewer *l, uint32_t required_id); //load chunk with required position

void fsviewer_open(MFSViewer *l, char *path);

#ifdef __cplusplus
}
#endif //cpp

#endif

#endif

