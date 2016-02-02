#pragma once

//#define MYGUI_STATIC

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "../../tools/libs/SDL2/SDL2.lib")
#pragma comment(lib, "../../tools/libs/SDL2//SDL2main.lib")
#pragma comment(lib, "../../tools/libs/SDL2/SDL2_image.lib")
#pragma comment(lib, "../../tools/libs/SDL2//SDL2_ttf")

#ifdef _DEBUG
#pragma comment(lib, "MyGUIEngine_d.lib")
#pragma comment(lib, "MyGUI.OpenGLPlatform_d.lib")
//#pragma comment(lib, "../../tools/libs/tb_static_d.lib")
#else
#pragma comment(lib, "MyGUIEngine.lib")
#pragma comment(lib, "MyGUI.OpenGLPlatform.lib")
//#pragma comment(lib, "../../tools/libs/tb_static.lib")
#endif

//#pragma comment(lib, "../../tools/libs/glew32.lib")
//#pragma comment(lib, "winmm.lib")

#pragma warning( disable :4244)
#pragma warning( disable :4290)
#pragma warning( disable :4005)

#include "../../tools/includes/GL/glew.h"
#include "../../tools/includes/SDL.h"
#include "../../tools/includes/SDL_opengl.h"
#include "../../tools/includes/SDL_image.h"
#include "../../tools/includes/SDL_ttf.h"

#include "../../tools/includes/glm/glm.hpp"
#include "../../tools/includes/glm/gtc/matrix_transform.hpp"

//#include "tb_core.h"
//#include "tb_widgets.h"
//#include "tb_widgets_common.h"
//#include "tb_widgets_reader.h"
//#include "tb_widgets_listener.h"
//#include "tb_skin.h"
//#include "tb_message_window.h"
//#include "tb_msg.h"
//#include "tb_scroller.h"
//#include "tb_system.h"
//#include "tb_inline_select.h"
//#include "tb_select.h"
//#include "tb_menu_window.h"
//#include "tb_editfield.h"
//#include "tb_tab_container.h"
//#include "tb_bitmap_fragment.h"
//#include "animation/tb_widget_animation.h"
//#include "tb_node_tree.h"
//#include "tb_tempbuffer.h"
//#include "tb_font_renderer.h"
//#include "image/tb_image_manager.h"
//#include "utf8/utf8.h"
//#include "renderers/tb_renderer_gl.h"
//#include "tb_font_renderer.h"

#include "../../tools/includes/mygui/MyGUI.h"
#include "../../tools/includes/mygui/OpenGLPlatform/include/MyGUI_OpenGLPlatform.h"

//#include "../../tools/includes/cegui/CEGUI/CEGUI.h"
//#include "../../tools/includes/cegui/CEGUI/RendererModules/OpenGL/GL3Renderer.h"
//#include "../../tools/includes/cegui/CEGUI/RendererModules/OpenGL/Texture.h"

#include <iostream>

// C++ runtime headers
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <iostream>

#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <algorithm>
#include <process.h>

///////////////////////////////////

//#include "ResourceManager.h"
#include "EngineDefines.h"
	
///////////////////////////////////

//void register_tbbf_font_renderer();

using namespace std;
//map<unsigned int, CEGUI::Key::Scan>* initKeyMap();
