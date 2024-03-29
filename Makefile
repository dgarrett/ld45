# deploy or debug/build
JS_ENV = deploy
# SHELL_FILE = node_modules/gbdkjs/shell_debug/build/index.html
SHELL_FILE = node_modules/gbdkjs/shell_$(JS_ENV)/index.html
EMCC = emcc -O2 -Wno-implicit-function-declaration -Inode_modules/gbdkjs/include -Inode_modules/gbdkjs/gbdk/include -Iinclude --js-library node_modules/gbdkjs/emscripten_bindings.js
CC = lcc -Wa-l -Wl-m -Wl-j -Wl-yt1 -Iinclude -Igenerated -Inode_modules/gbdkjs/include

ROM_BUILD_DIR = build/gb
WEB_BUILD_DIR = build/web

OBJDIR = obj

all:	game.html

$(OBJDIR)/%.o:	src/%.c include/assets.h
	$(CC) -c -o $@ $<

$(OBJDIR)/%.o:	src/data/%.c
	$(CC) -c -o $@ $<	

$(OBJDIR)/%.s:	%.c
	$(CC) -S -o $@ $<

$(OBJDIR)/%.o:	%.s
	$(CC) -c -o $@ $<

$(OBJDIR)/%.bc:	src/%.c
	$(EMCC) -o $@ $<

$(OBJDIR)/%.bc:	src/data/%.c
	$(EMCC) -o $@ $<	

$(ROM_BUILD_DIR)/%.gb:	$(OBJDIR)/%.o
	mkdir -p $(ROM_BUILD_DIR)
	$(CC) -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -Wl-yt1 -Wl-yo32 -Wl-ya4 -o $@ $^	

$(WEB_BUILD_DIR)/%.html:	$(OBJDIR)/%.bc
	mkdir -p $(WEB_BUILD_DIR)
	cp -r node_modules/gbdkjs/shell_$(JS_ENV)/static $(WEB_BUILD_DIR)
	$(EMCC) --shell-file $(SHELL_FILE) -s ASSERTIONS=1 -o $@ $^

clean:
	rm -f obj/*
	rm -rf build

rom: assets $(ROM_BUILD_DIR)/game.gb

run_rom: rom
	open build/gb/game.gb

web: $(WEB_BUILD_DIR)/game.html
	mv $(WEB_BUILD_DIR)/game.html $(WEB_BUILD_DIR)/index.html

web_deploy: web rom
	cp -r $(WEB_BUILD_DIR)/* ./docs/
	cp $(ROM_BUILD_DIR)/game.gb ./docs/

web_test: web
	live-server build/web

assets: FORCE
	cd assets && ./build.sh

FORCE:
