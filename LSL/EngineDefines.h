#pragma once

namespace Defines
{
	////////////////////////////////////////////////
	/*			  ENGINE DEFINES				  */


	////////////////////////////////////////////////
	/*				GAME DEFINES				  */

	 const int BL_FOREGROUND = 1;
	 const int BL_BACKGROUND = 0;

	 const int BL_NONE = 0;

	 const int BL_TEX_ROTATE_LEFT = 0;
	 const int BL_TEX_ROTATE_RIGHT = 1;
	 const int BL_TEX_ROTATE_TOP = 2;
	 const int BL_TEX_ROTATE_BOTTOM = 3;

	 const int DIR_UP = 0;
	 const int DIR_DOWN = 1;
	 const int DIR_LEFT = 2;
	 const int DIR_RIGHT = 3;

	//

	 const int CELL_DECOR_INNER = 0;
	 const int CELL_DECOR_INNER_1 = 1;
	 const int CELL_DECOR_INNER_2URDL = 2;
	 const int CELL_DECOR_INNER_2ULR = 3;
	 const int CELL_DECOR_INNER_1UR2DLR = 4;
	 const int CELL_DECOR_INNER_4 = 5;
	 const int CELL_DECOR_CORNER = 6;
	 const int CELL_DECOR_CORNER_1 = 7;
	 const int CELL_DECOR_LINE = 8;
	 const int CELL_DECOR_LINE_1L = 9;
	 const int CELL_DECOR_LINE_1R = 10;
	 const int CELL_DECOR_LINE_2 = 11;
	 const int CELL_DECOR_TWOLINE = 12;
	 const int CELL_DECOR_P = 13;
	 const int CELL_DECOR_O = 14;
	 	
	//

	 const int TYPE_PLAYER = 0;
	 const int TYPE_SPRITE = 1;
	 const int TYPE_LEVELOBJ = 2;
	 
	//

	 const int EV_APP_EXIT = 1;
	 const int EV_CAM_MOVE = 2;
	 const int EV_WINDOW_RESIZED = 3;
	 const int EV_CAM_ZOOM = 4;
	 const int EV_UPDATE_PHY_OBJ = 5;
	 const int EV_INIT_UI = 6;
	 const int EV_APP_START = 7;

	////////////////////////////////////////////////
	/*				RESOURCE DEFINES			  */

	//Shaders

	 const int SH_INVALID = -1;
	
	//Texture Atlases

	 const int ATLAS_INVALID = -1;
	
	//Sounds

	 const int SOUND_INVALID = -1;

	//Textures

	 const int TEX_INVALID = -1;
	
	 //Sprites

	 const int SPRITE_INVALID = -1;
	 const int SPRITE_QUAD = 0;
}