#!/bin/bash

ASSET_FILE=../include/assets.h

# Build tilesets
#ggbgfx tileset platformer.png -o platformer_tileset.png
ggbgfx tileset win.png -o win_tileset.png

# Build sprites
ggbgfx sprite dot_sprites.png > $ASSET_FILE
ggbgfx sprite dot_legs.png >> $ASSET_FILE
ggbgfx sprite borb_full_grey.png >> $ASSET_FILE

# Build tiledata
ggbgfx tiledata platformer_tileset.png >> $ASSET_FILE
ggbgfx tiledata win_tileset.png >> $ASSET_FILE

# Build tilemaps
ggbgfx tilemap -n level1 level1.png platformer_tileset.png >> $ASSET_FILE
ggbgfx tilemap -n win_tiles win.png win_tileset.png >> $ASSET_FILE
