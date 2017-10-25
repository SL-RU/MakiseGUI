MAKISE_GUI_H_FILE	:= $(shell find MakiseGUI/ -maxdepth 10 -type f -name "*.h" )
MAKISE_GUI_C_FILE	:= $(shell find MakiseGUI/ -maxdepth 10 -type f -name "*.c" )
MAKISE_GUI_DIR		:= $(shell find MakiseGUI/ -maxdepth 10 -type d -name "*" )
MAKISE_GUI_PATH		:= $(addprefix -I, $(MAKISE_GUI_DIR))
MAKISE_GUI_OBJ_FILE	:= $(addprefix build/obj/, $(MAKISE_GUI_C_FILE))
MAKISE_GUI_OBJ_FILE	:= $(patsubst %.c, %.o, $(MAKISE_GUI_OBJ_FILE))

build/obj/MakiseGUI/%.o:	MakiseGUI/%.c $(USER_CFG_H_FILE) 
	@echo [CC] $<
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) $(FAT_FS_PATH) $(USER_CFG_PATH) $(MAKISE_GUI_PATH) $(MAKISE_GUI_OPTIMIZATION) -c $< -o $@

# Добавляем к общим переменным проекта.
PROJECT_PATH			+= $(MAKISE_GUI_PATH)
PROJECT_OBJ_FILE		+= $(MAKISE_GUI_OBJ_FILE)