REPO_ROOT = $(SOURCELIB_ROOT)/..
VSCODE_DEBUGGER = cortex-debug
JLINK_CPU = STM32F429ZI
OBJ_DIR = $(SOURCELIB_ROOT)

.PHONY: launch
launch:
	@$(SOURCELIB_ROOT)/tools/vscode/generate_launch.py --debugger $(VSCODE_DEBUGGER)  --device $(JLINK_CPU) --elf $(shell pwd)/main.elf --dest $(REPO_ROOT)/.vscode --svd $(SOURCELIB_ROOT)/tools/vscode/STM32F429.svd

.PHONY: vscode
vscode:
	@$(SOURCELIB_ROOT)/tools/vscode/generate_cpp_properties.py --dest $(REPO_ROOT)/.vscode --app $(PROJ_NAME) --compiler $(shell which $(CC)) --app_dir $(shell pwd)
	@$(SOURCELIB_ROOT)/tools/vscode/generate_settings.py --dest $(REPO_ROOT)/.vscode --python $(shell which python3)
