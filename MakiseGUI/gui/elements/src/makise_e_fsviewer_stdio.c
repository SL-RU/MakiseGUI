#include "makise_e_fsviewer.h"
#if MAKISE_E_FSVIEWER == MAKISE_E_FSVIEWER_STDIO //STDIO
static void m_fsviewer_loadchunk(MFSViewer *l, uint32_t required_id); //load chunk with required position

uint32_t fsviewer_count_files ( TCHAR* path )
{
    uint32_t count = 0;
    
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(path); /* There should be error handling after this */
    if(dirp == NULL)
    {
	MAKISE_ERROR_OUTPUT("Directory read error!\n");
	return 0;
    }

    while ((entry = readdir(dirp)) != NULL) {
	if(
	    //(entry->d_type == DT_REG || entry->d_type == DT_DIR) &&
	    !(entry->d_name[0] == '.' && entry->d_name[1] == 0) &&
	    !(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == 0))
	    count++;
    }
    
    //MAKISE_DEBUG_OUTPUT("file count: %d\n", count);
    closedir(dirp);
    return count;
}

static void m_fsviewer_loadchunk(MFSViewer *l, uint32_t required_id)
{
    taskENTER_CRITICAL();
    char bu[5] = "";
    //do we need to show [..](go back)
    getcwd(bu, 5);
    uint8_t isroot = (bu[0] == '/') && (bu[1] == 0);
    //MAKISE_DEBUG_OUTPUT("root %s| %d\n", bu, isroot);

    l->files_count = fsviewer_count_files(".") + !isroot;

    //MAKISE_DEBUG_OUTPUT("files coint: %d\n", count);
    
    //calculate chunk's start position
    if(required_id >= FM_BUFFERED/2)
    {
	l->current_chunk_position = required_id - FM_BUFFERED/2;
	if(l->current_chunk_position != 0 &&
	   l->current_chunk_position + FM_BUFFERED/2 - 1 >= l->files_count)
	{
	    uint32_t d = l->current_chunk_position + FM_BUFFERED/2 - 1 - l->files_count;
	    if(d > l->current_chunk_position)
	    {
		l->current_chunk_position = 0;
	    }
	    else
	    {
		l->current_chunk_position -= d;
	    }
	}
    }
    else
    {
	l->current_chunk_position = 0;
    }
    //MAKISE_DEBUG_OUTPUT("ch st: %d req %d\n", l->current_chunk_position, required_id);
    //FRESULT res;
    DIR* dir;
    //static struct stat fno;
    struct dirent * entry;

    uint32_t ci = !isroot, //file's ID
	bi = 0; //id in the buffer
    //uint8_t sel_dir = 0; //was current dir selected

    //add go back items
    if(l->current_chunk_position == 0 && !isroot)
    {
	strcpy(l->buffer[0].name, "..");
	l->buffer[0].am_dir = 1;
    }
    
    dir = opendir(".");                       /* Open current directory */
    if (dir != 0) {	
	//MAKISE_DEBUG_OUTPUT("sclust %d\n", dir.obj.sclust);
        for (;;) {
	    if(ci >= l->current_chunk_position)
	    {
		bi = ci - l->current_chunk_position;
		//fno.lfname = l->buffer[bi].name;
		//fno.lfsize = _MAX_LFN + 1;
	    }
	    entry = readdir(dir);
	    if(entry == 0) break;
	    if(
//		(entry->d_type == DT_REG || entry->d_type == DT_DIR) &&
		!(entry->d_name[0] == '.' && entry->d_name[1] == 0) &&
		!(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == 0))
	    {
		//MAKISE_DEBUG_OUTPUT("cl %d %s ", ci, fno.fname);
		if(ci >= l->current_chunk_position)
		{
		    strcpy(l->buffer[bi].name, entry->d_name);
		    //MAKISE_DEBUG_OUTPUT("file %d %s\n", ci, entry->d_name);
		    l->buffer[bi].text_width =
			makise_d_string_width(entry->d_name, MDTextAll,
					      b->item_style->font);
		    l->buffer[bi].scroll_x = 0;

		    l->buffer[bi].am_dir = entry->d_type == DT_DIR;

		    l->buffer[bi].id = ci;
		    bi ++;
		}
		ci ++;
		if(bi >= FM_BUFFERED)
		    break;
	    }
	    else if(entry->d_name[0] == '.' && entry->d_name[1] == 0)
	    {
		l->current_folder = entry->d_ino; //set current dir
		//sel_dir = l->current_folder == l->selected_folder; //is current dir the selected
	    }
	}
        closedir(dir);

    }
    else
    {
	MAKISE_ERROR_OUTPUT("ERROR while oppenning directory!\n\r");
    }
    taskEXIT_CRITICAL();

}

void m_fsviewer_deselect(MFSViewer *l)
{
    MAKISE_MUTEX_REQUEST(&l->el.mutex);
    l->was_selected = 0;
    MAKISE_MUTEX_RELEASE(&l->el.mutex);
}

void _fsviewer_open( MFSViewer *l, char* path )
{
    //set path
    l->path = path;
    //go to dir
    chdir(path);
    
    //reset selection and etc
    //l->selected_file[0] = 0;
    l->current_position = 0;
    l->files_count = 0;
    l->current_folder = 0;

    //load first chunk
    m_fsviewer_loadchunk(l, 0);   
}

void fsviewer_open(MFSViewer *l, char *path)
{
    MAKISE_MUTEX_REQUEST(&l->el.mutex);
    _fsviewer_open(l, path);
    MAKISE_MUTEX_RELEASE(&l->el.mutex);
}

void m_fsviewer_set_onselection(
    MFSViewer *l,
    uint8_t (*onselection)(MFSViewer* l, MFSViewer_Item* s))
{
    MAKISE_MUTEX_REQUEST(&l->el.mutex);
    l->onselection = onselection;
    MAKISE_MUTEX_RELEASE(&l->el.mutex);
}

#endif
