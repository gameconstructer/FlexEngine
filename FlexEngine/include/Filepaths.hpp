#pragma once

// TODO: Rename folder to data
#define ROOT_LOCATION								"../../../FlexEngine/"

#define RESOURCE_LOCATION							ROOT_LOCATION "resources/"
#define CONFIG_DIRECTORY							ROOT_LOCATION "config/"
#define SAVED_LOCATION								ROOT_LOCATION "saved/"

#define MESH_DIRECTORY								RESOURCE_LOCATION "meshes/"
#define TEXTURE_DIRECTORY							RESOURCE_LOCATION "textures/"
#define ICON_DIRECTORY								TEXTURE_DIRECTORY "icons/"
#define SFX_DIRECTORY								RESOURCE_LOCATION "audio/"
#define FONT_DIRECTORY								RESOURCE_LOCATION "fonts/"
#define SHADER_SOURCE_DIRECTORY						RESOURCE_LOCATION "shaders/"
#define PREFAB_DIRECTORY							RESOURCE_LOCATION "prefabs/"
#define SCENE_DEFAULT_DIRECTORY						RESOURCE_LOCATION "scenes/default/"
#define SCENE_SAVED_DIRECTORY						RESOURCE_LOCATION "scenes/saved/"
#define SCRIPTS_DIRECTORY							RESOURCE_LOCATION "scripts/"
#define UI_CONFIG_DIRECTORY							RESOURCE_LOCATION "ui/"

#define FONT_SDF_DIRECTORY							SAVED_LOCATION "fonts/"
#define SCREENSHOT_DIRECTORY						SAVED_LOCATION "screenshots/"
#define COMPILED_SHADERS_DIRECTORY					SAVED_LOCATION "spv/"

#define GAME_OBJECT_TYPES_LOCATION					RESOURCE_LOCATION "GameObjectTypes.txt"
#define MATERIALS_FILE_LOCATION						RESOURCE_LOCATION "scenes/materials.json"
#define SPECIALIZATION_CONSTANTS_LOCATION			RESOURCE_LOCATION "specialization_constants.json"
#define DEBUG_OVERLAY_NAMES_LOCATION				RESOURCE_LOCATION "debug_overlay_names.json"

#define SHADER_SPECIALIZATION_CONSTANTS_LOCATION	SHADER_SOURCE_DIRECTORY "shader_specialization_constants.json"

#define UI_SETTINGS_LOCATION						UI_CONFIG_DIRECTORY "uiSettings.json"
#define UI_PLAYER_QUICK_ACCESS_LOCATION				UI_CONFIG_DIRECTORY "playerQuickAccessUI.json"
#define UI_PLAYER_INVENTORY_LOCATION				UI_CONFIG_DIRECTORY "playerInventoryUI.json"

#define COMMON_CONFIG_LOCATION						CONFIG_DIRECTORY "common.json"
#define WINDOW_CONFIG_LOCATION						CONFIG_DIRECTORY "window-settings.json"
#define INPUT_BINDINGS_LOCATION						CONFIG_DIRECTORY "input-bindings.json"
#define RENDERER_SETTINGS_LOCATION					CONFIG_DIRECTORY "renderer-settings.json"
#define FONT_DEFINITION_LOCATION					CONFIG_DIRECTORY "fonts.json"
#define IMGUI_INI_LOCATION							CONFIG_DIRECTORY "imgui.ini"
#define IMGUI_LOG_LOCATION							CONFIG_DIRECTORY "imgui.log"
#define UI_WINDOW_CACHE_LOCATION					CONFIG_DIRECTORY "ui_window_cache.json"

#define SHADER_CHECKSUM_LOCATION					SAVED_LOCATION "vk-shader-checksum.dat"
