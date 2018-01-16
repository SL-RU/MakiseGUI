ifndef MODULE_MAKISE_GUI_OPTIMIZATION
	MODULE_MAKISE_GUI_OPTIMIZATION = -g3 -O0
endif

MAKISE_GUI_H_FILE	:= $(shell find module_makise_gui/ -maxdepth 10 -type f -name "*.h" )
MAKISE_GUI_C_FILE	:= $(shell find module_makise_gui/ -maxdepth 10 -type f -name "*.c" )
MAKISE_GUI_DIR		:= $(shell find module_makise_gui/ -maxdepth 10 -type d -name "*" )
MAKISE_GUI_PATH		:= $(addprefix -I, $(MAKISE_GUI_DIR))
MAKISE_GUI_OBJ_FILE	:= $(addprefix build/obj/, $(MAKISE_GUI_C_FILE))
MAKISE_GUI_OBJ_FILE	:= $(patsubst %.c, %.o, $(MAKISE_GUI_OBJ_FILE))

build/obj/module_makise_gui/%.o:	module_makise_gui/%.c $(USER_CFG_H_FILE) 
	@echo [CC] $<
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) $(FAT_FS_PATH) $(USER_CFG_PATH) $(MAKISE_GUI_PATH) $(MODULE_MAKISE_GUI_OPTIMIZATION) -c $< -o $@

# Добавляем к общим переменным проекта.
PROJECT_PATH			+= $(MAKISE_GUI_PATH)
PROJECT_OBJ_FILE		+= $(MAKISE_GUI_OBJ_FILE)