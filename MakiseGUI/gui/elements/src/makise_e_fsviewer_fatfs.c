#include "makise_e_fsviewer.h"
#if MAKISE_E_FSVIEWER == MAKISE_E_FSVIEWER_FATFS


uint32_t fsviewer_count_files ( TCHAR* path )
{
    uint32_t count = 0;
    
    DIR dir;
    static FILINFO fno;
    FRESULT res;

    if(path == 0)
	res = f_opendir(&dir, (TCHAR*)"");
    else
	res = f_opendir(&dir, path);
    if (res == FR_OK) {
	res = f_readdir( &dir, NULL );
	for (;;) {
	    res = f_readdir(&dir, &fno);                   /* Read a directory item */
	    MAKISE_DEBUG_OUTPUT("%d %d %s\n", count, res, fno.fname);
	    
	    if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
	    count ++;
	}
	f_closedir(&dir);
    }
    else
    {
	MAKISE_DEBUG_OUTPUT("opendir err %d\n", res);
    }
    return count;
}

void m_fsviewer_loadchunk(MFSViewer *l, uint32_t required_id)
{
    TCHAR bu[5] = {0};
    //do we need to show [..](go back)
    uint8_t isroot = f_getcwd(bu, 5);
    isroot = (bu[0] == '/') && (bu[1] == 0);
    
    l->files_count = fsviewer_count_files(0);
    FRESULT res;
    DIR dir;
    static FILINFO fno;

    uint32_t ci = !isroot, //file's ID
	bi = 0; //id in the buffer

    //add go back items
    if(!isroot)
    {
	l->files_count ++;
    }

    MAKISE_DEBUG_OUTPUT("files coint: %d\n", l->files_count);
    
    //calculate chunk's start position
    if ( required_id >= FM_BUFFERED / 2 ) {
        l->current_chunk_position = required_id - FM_BUFFERED / 2;
        if ( l->current_chunk_position != 0 &&
             l->current_chunk_position + FM_BUFFERED/2 - 1 >= l->files_count ) {
            uint32_t d = l->current_chunk_position + FM_BUFFERED/2 - 1 - l->files_count;
            if ( d > l->current_chunk_position ) {
                l->current_chunk_position = 0;
            } else {
                l->current_chunk_position -= d;
            }
        }
    } else {
        l->current_chunk_position = 0;
    }

    if(l->current_chunk_position == 0 && !isroot)
    {
	strcpy(l->buffer[0].name, "..");
#if FF_LFN_UNICODE && FF_USE_LFN	
	memcpy(l->buffer[0].fname, (TCHAR[3]){'.', '.', 0}, 6);
#else
	strcpy((char*)l->buffer[0].fname, "..");
#endif
	l->buffer[0].am_dir = 1;
	l->buffer[0].text_width = 2;
	l->buffer[0].scroll_x = 0;
    }
    
    res = f_opendir(&dir, (TCHAR*)"");
    if (res == FR_OK) {
	l->current_folder = dir.obj.sclust; //set current dir
	//sel_dir = l->current_folder == l->selected_folder; //is current dir the selected
	
	MAKISE_DEBUG_OUTPUT("sclust %d\n", dir.obj.sclust);
        for (;;) {
	    if(ci >= l->current_chunk_position)
	    {
		bi = ci - l->current_chunk_position;
		//fno.lfname = l->buffer[bi].name;
		//fno.lfsize = _MAX_LFN + 1;
	    }
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
	    MAKISE_DEBUG_OUTPUT("cl %d %s ", ci, fno.fname);
	    if(ci >= l->current_chunk_position)
	    {
		MAKISE_DEBUG_OUTPUT("buf %d\n", bi);
		//if(fno.lfname[0] == 0)
#if FF_LFN_UNICODE && FF_USE_LFN
		makise_d_utf16_string_decode((uint8_t*)fno.fname, 
		 			     (uint8_t*)l->buffer[bi].name, FF_MAX_LFN-1); 
		//memcpy(l->buffer[bi].name, fno.fname, 256);
		if(fno.altname[0] != 0)
		    memcpy(l->buffer[bi].fname, fno.altname, sizeof(TCHAR)*13);
		else
		    //if altname is 0
		    memcpy(l->buffer[bi].fname, fno.fname, sizeof(TCHAR)*13);
#elif FF_USE_LFN
		// Usual LFN name
		memcpy(l->buffer[bi].name, fno.fname, 256);
		memcpy(l->buffer[bi].fname, fno.altname, sizeof(TCHAR)*13);

#else
		//Without LFN
		strncpy(l->buffer[bi].name, fno.fname, 13);
#endif
		l->buffer[bi].text_width = INT16_MAX;
		l->buffer[bi].scroll_x = 0;

		//l->buffer[bi].sclust = 

		l->buffer[bi].am_dir = fno.fattrib & AM_DIR;

		l->buffer[bi].id = ci;
		bi ++;
	    }
	    else {
		MAKISE_DEBUG_OUTPUT("\n");
	    }
	    ci ++;
	    if(bi >= FM_BUFFERED)
		break;
        }
        f_closedir(&dir);

    }
    else
    {
	MAKISE_ERROR_OUTPUT("ERROR while oppenning directory! code: %d\n\r", res);
    }    
}

void m_fsviewer_deselect(MFSViewer *l)
{
    m_element_mutex_request(&l->el);
    l->was_selected = 0;
    m_element_mutex_release(&l->el);
}

void _fsviewer_open( MFSViewer *l, TCHAR* path )
{
    //set path
    l->path = path;
    //go to dir
    f_chdir(path);
    
    //reset selection and etc
    //l->selected_file[0] = 0;
    l->current_position = 0;
    l->files_count = 0;
    l->current_folder = 0;

    //load first chunk
    m_fsviewer_loadchunk(l, 0);   
}

void fsviewer_open(MFSViewer *l, TCHAR *path)
{
    m_element_mutex_request(&l->el);
    _fsviewer_open(l, path);
    m_element_mutex_release(&l->el);
}

void m_fsviewer_set_onselection(
    MFSViewer *l,
    uint8_t (*onselection)(MFSViewer* l, MFSViewer_Item* s))
{
    m_element_mutex_request(&l->el);
    l->onselection = onselection;
    m_element_mutex_release(&l->el);
}
#endif
