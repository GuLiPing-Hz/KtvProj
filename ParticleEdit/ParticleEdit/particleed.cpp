/*
** Haaf's Game Engine 1.7
** Copyright (C) 2007, Relish Games
** hge.relishgames.com
**
** Particle systems editor
*/


#include "particleed.h"
#include <vector>
#include <Windows.h>

HGE *hge=0;

int PSI_START = 0;
hgeFont			*fnt;
hgeGUI			*gui;
HTEXTURE		texGui, texParticles;

CGLSprite		*sprLeftPane1, *sprLeftPane2, *sprLeftPane3, *sprRightPane1, *sprRightPane2;
CGLSprite		*sprCursor, *sprColor, *sprBBox;
hgeAnimation	*sprParticles[9];

HTEXTURE		gTexParticle[9] = {0};
CGLSprite*		gSprParticle[9] = {0};

bool bvectPreset[9];
PEditorState	state;
int			curIndex=0;
float			psx=640, psy=360;

CCritSec g_csLock;

void			InitEditor();
void			DoneEditor();
void			CreateGUI();


bool FrameFunc()
{
	float		px, py;
	float		dt=hge->Timer_GetDelta();
	
	// Update
	
	hge->Input_GetMousePos(&state.mx, &state.my);
	if(hge->Input_GetKeyState(HGEK_RBUTTON)) { psx=state.mx; psy=state.my; }
	else { psx=state.ps[state.nPreset]->m_info.fOriginX; psy=state.ps[state.nPreset]->m_info.fOriginY; }

	if(state.bIFace)//是否有界面
	{
		if(psx > 1111) psx=1111;
		if(psx < 168) psx=168;
	}
	for(int i=0;i<10;i++)
	{
		if (bvectPreset[i] && state.ps[i])
		{
			state.ps[i]->GetPosition(&px, &py);
			if (i == state.nPreset)
			{
				state.ps[i]->MoveTo(px+(psx-px)*10*dt, py+(psy-py)*10*dt);
			}
			state.ps[i]->Update(dt);
		}
	}
// 	state.ps->GetPosition(&px, &py);
// 	state.ps->MoveTo(px+(psx-px)*10*dt, py+(psy-py)*10*dt);
// 	state.ps->Update(dt);

	if(HandleKeys(hge->Input_GetKey())) return true;

	if(state.bIFace)//更新界面
	{
		if(DoCommands(gui->Update(dt))) return true;
	}

	GetTextCtrl(CMD_NPARTICLES)->printf("%d", state.ps[state.nPreset]?state.ps[state.nPreset]->GetParticlesAlive():0);
	GetTextCtrl(CMD_FPS)->printf("%d", hge->Timer_GetFPS());
// 	GetTextCtrl(TEXT_CIRCLE_IN_A)->printf("%.0f",state.ps[state.nPreset]->m_info.circleInsideA);
// 	GetTextCtrl(TEXT_CIRCLE_OUT_A)->printf("%.0f",state.ps[state.nPreset]->m_info.circleOutsideA);
// 	GetTextCtrl(TEXT_CIRCLE_IN_B)->printf("%.0f",state.ps[state.nPreset]->m_info.circleInsideB);
// 	GetTextCtrl(TEXT_CIRCLE_OUT_B)->printf("%.0f",state.ps[state.nPreset]->m_info.circleOutsideB);

	return false;
}

bool RenderFunc()
{
	hgeRect		bbox;

	// Render
	
	hge->Gfx_Clear(0);
	hge->Gfx_BeginScene();

	if(state.sprBG)	
		state.sprBG->Render(640, 320);

	//state.ps->Render();//render 粒子系统
	{
		CAutoLock lock(&g_csLock);
		for(int i=0;i<9;i++)
		{
			if (bvectPreset[i] && state.ps[i] && state.ps[i]->m_info.sprite)
			{
				state.ps[i]->drawRender();
			}
		}
	}


	if(state.bBBox)
	{
		state.ps[state.nPreset]->GetBoundingBox(&bbox);
		if(!bbox.IsClean()) sprBBox->RenderStretch(bbox.x1, bbox.y1, bbox.x2, bbox.y2);
	}

	if(state.bIFace)
	{
		//sprLeftPane1->Render(0,0);//左界面
		//sprLeftPane2->Render(0,512);
		//sprLeftPane3->Render(168,0);
		sprRightPane1->Render(1112,0);//右界面
		//sprRightPane2->Render(1112,512);
		CGLSprite* pSprite = state.ps[state.nPreset]->m_info.sprite;
		if(pSprite)
			pSprite->Render(0+pSprite->GetWidth()/2, 0+pSprite->GetHeight()/2);//render 样本
		gui->Render();//render 界面
		if(gSprParticle[state.nPreset])
			gSprParticle[state.nPreset]->SetColor(state.ps[state.nPreset]->m_info.colColorStart.GetHWColor() | 0xFF000000);
		//sprParticles[state.nPreset]->Render(16,16);//render 样本
		sprColor->Render(1122,221);//render 颜色变化
	}

	if(state.bHelp)
	{
		fnt->SetColor(0xFFFFFFFF);
		fnt->Render(189, 18, HGETEXT_LEFT, "Left mouse button - fire particle system\n"
			"Right mouse button - move the system with mouse\n\n"
			"Keys 1 to 9 - select preset\nPresets are saved and loaded automatically\n\n"
			"TAB - Hide editor panels\n"
			"Esc - Exit\n\n"
			"Edit PARTICLEED.INI file to change backdrop or fullscreen/windowed mode");
	}

	if(hge->Input_IsMouseOver() && !hge->Input_GetKeyState(HGEK_RBUTTON)) sprCursor->Render(state.mx, state.my);

	hge->Gfx_EndScene();

	return false;
}

#include "OS_Vector.h"

// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
int glp()
{

	D3DXMATRIX a(1,2,3,4,2,2,2,2,3,3,3,3,1,1,1,1);
	D3DXMATRIX b(4,3,2,1,3,3,3,3,3,3,3,3,1,1,1,1);

	OS_MatrixMultiply(&a,&a,&b);

	hge = hgeCreate(HGE_VERSION);
	//HEFFECT hEffect = hge->Effect_Load("C:\\Users\\Administrator\\Desktop\\gu.WAV");
	//hge->Effect_PlayEx(hEffect,100,0,1.0,true);
	hge->System_SetState(HGE_INIFILE, "particleed.ini");//定义配置文件
	hge->System_SetState(HGE_LOGFILE, "particleed.log");//定义日志文件
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//定义更新函数
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);//定义渲染函数
	hge->System_SetState(HGE_TITLE, "HGE Particle Systems Editor");//定义标题
	hge->System_SetState(HGE_SCREENWIDTH, WIN_WIDTH);//宽
	hge->System_SetState(HGE_SCREENHEIGHT, WIN_HEIGHT);//高
	hge->System_SetState(HGE_SCREENBPP, 32);//几位颜色
	hge->System_SetState(HGE_USESOUND, false);//是否使用声音

	if(hge->Ini_GetInt("HGE", "FullScreen",0))	
		hge->System_SetState(HGE_WINDOWED, false);
	else 
		hge->System_SetState(HGE_WINDOWED, true);

	PSI_START = hge->Ini_GetInt("HGE","NPSI_START",0);
	if(hge->System_Initiate())
	{
		InitEditor();//初始化属性
		hge->System_Start();
		DoneEditor();//善后
	}
	else MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_TOPMOST|MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	hge->System_Shutdown();
	hge->Release();
	return 0;
}

void InitEditor()
{
	structParticleSystemInfo psi;
	char *bgName;
	int bgw, bgh;

	state.texBG = 0;
	state.sprBG = 0;

	bgName = hge->Ini_GetString("HGE", "Background", 0);//解析背景图片
	if(bgName)
	{
		unsigned int w,h;
		state.texBG = hge->Texture_Load(bgName,w,h);
		bgw = hge->Texture_GetWidth(state.texBG, true);
		bgh = hge->Texture_GetHeight(state.texBG, true);
		state.sprBG = new CGLSprite(state.texBG, 0, 0, (float)bgw, (float)bgh);
		state.sprBG->SetHotSpot((float)bgw/2, (float)bgh/2);
	}

	//hge->Resource_AttachPack("particleed.paq");//解压，压缩包，包含纹理图片zip
	for(int i=0;i<9;i++)
	{
		structParticleSystemInfo psi = {0};
		state.ps[i] = new hgeParticleSystem(&psi);
	}

	state.bIFace=false;
	state.bHelp=false;
	state.bBBox=false;
	state.nPreset=0;

	unsigned int w,h;
	texParticles= NULL;//hge->Texture_Load("particles.png",w,h);
	
// 	for(int i=0;i<9;i++)
// 	{
// 		sprParticles[i]=new hgeAnimation(texParticles, 16, 1.0f, 0, 0, 32, 32);
// 		sprParticles[i]->SetHotSpot(16,16);
// 		memset(&psi, 0, sizeof(structParticleSystemInfo));
// 		psi.sprite=sprParticles[i];
// 
// 
// 		state.ps[i]=new hgeParticleSystem(&psi);
// 		state.ps[i]->TrackBoundingBox(true);
// 		state.ps[i]->MoveTo(psx, psy);
// 	}

	fnt=new hgeFont("font3.fnt");
	texGui=hge->Texture_Load("pgui.png",w,h);

	sprCursor=new CGLSprite(texGui, 487, 181, 19, 26);
	sprColor=new CGLSprite(texGui, 466, 208, 14, 80);
	sprBBox=new CGLSprite(texGui, 337, 209, 128, 128);

	//sprLeftPane1=new CGLSprite(texGui, 0, 0, 168, 512);
	//sprLeftPane2=new CGLSprite(texGui, 336, 0, 168, 88);
	//sprLeftPane3=new CGLSprite(texGui,0,512,168,512);
	sprRightPane1=new CGLSprite(texGui, 168, 0, 168, 512);
	//sprRightPane2=new CGLSprite(texGui, 336, 88, 168, 88);

	gui=new hgeGUI();
	CreateGUI();

	cmdLoadFromFile(state.nPreset);
//////////////////////////////////////////////////////////////////////////
	for (int i=0;i<9;i++)
	{
		bvectPreset[i]=false;
	}
	bvectPreset[0] = true;
}

void DoneEditor()
{
	cmdSavePreset(state.nPreset);

	if(state.sprBG) delete state.sprBG;
	if(state.texBG) hge->Texture_Free(state.texBG);

	delete gui;
	//delete sprLeftPane1;
	//delete sprLeftPane2;
	//delete sprLeftPane3;
	delete sprRightPane1;
	//delete sprRightPane2;
	delete sprBBox;
	delete sprColor;
	delete sprCursor;
	delete fnt;
	for(int i=0;i<9;i++)
	{
		if(state.ps[i])
			delete state.ps[i];
		//delete sprParticles[i];
		if(gSprParticle[i])
			delete gSprParticle[i];
		if(gTexParticle[i])
			hge->Texture_Free(gTexParticle[i]);
	}
	
	hge->Texture_Free(texParticles);
	hge->Texture_Free(texGui);

	hge->Resource_RemoveAllPacks();
}

void CreateGUI()
{
	hgeGUIButton *button;
	hgeGUISlider *slider;
	hgeGUIText	 *text;

	// Presets & stuff

	gui->AddCtrl(new hgeGUIButton(CMD_EXIT, 718+480, 484, 73, 17, texGui, 336, 191));
	button=new hgeGUIButton(CMD_HELP, 641+480, 484, 73, 17, texGui, 336, 191);
	button->SetMode(true);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET1, 641+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	button->SetState(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET1_CHECK,641+480, 404, 8, 8, texGui,368,176);
	button->SetMode(true);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET2, 657+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET2_CHECK,657+480, 404, 8, 8, texGui,368,176);
	button->SetMode(false);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET3, 673+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET3_CHECK,673+480, 404, 8, 8, texGui,368,176);
	button->SetMode(false);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET4, 689+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET4_CHECK,689+480, 404, 8, 8, texGui,368,176);
	button->SetMode(false);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET5, 705+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET5_CHECK,705+480, 404, 8, 8, texGui,368,176);
	button->SetMode(false);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET6, 721+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET6_CHECK,721+480, 404, 8, 8, texGui,368,176);
	button->SetMode(false);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET7, 737+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET7_CHECK,737+480, 404, 8, 8, texGui,368,176);
	button->SetMode(false);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET8, 753+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET8_CHECK,753+480, 404, 8, 8, texGui,368,176);
	button->SetMode(false);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_PRESET9, 769+480, 389, 16, 15, texGui, 384, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_PRESET9_CHECK,769+480, 404, 8, 8, texGui,368,176);
	button->SetMode(false);
	gui->AddCtrl(button);

	button=new hgeGUIButton(CMD_BOUNDINGBOX, 641+480, 417, 8, 8, texGui, 368, 176);
	button->SetMode(true);
	gui->AddCtrl(button);

	text=new hgeGUIText(CMD_FPS, 641+480, 456, 28, 16, fnt);
	text->SetMode(HGETEXT_RIGHT);
	text->SetColor(0xFFD4C25A);
	gui->AddCtrl(text);
	text=new hgeGUIText(CMD_NPARTICLES, 641+480, 437, 28, 16, fnt);
	text->SetMode(HGETEXT_RIGHT);
	text->SetColor(0xFFD4C25A);
	gui->AddCtrl(text);

	// System parameters

// 	slider=new hgeGUISlider(CMD_SYS_LIFETIME, 10, 44, 149, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(0, 10, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	text=new hgeGUIText(CMD_SYS_TLIFETIME, 113, 30, 28, 12, fnt);
// 	text->SetMode(HGETEXT_RIGHT);
// 	text->SetText("0");
// 	gui->AddCtrl(text);
// 	button=new hgeGUIButton(CMD_SYS_LIFECONT, 9, 55, 8, 8, texGui, 368, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	button=new hgeGUIButton(CMD_SYS_ONESHOT,90,55,8,8,texGui,368,176);
// 	button->SetMode(false);
// 	gui->AddCtrl(button);
// 
// 	slider=new hgeGUISlider(CMD_SYS_EMISSION, 10, 91, 149, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(0, 1000, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	text=new hgeGUIText(CMD_SYS_TEMISSION, 103, 77, 28, 12, fnt);
// 	text->SetMode(HGETEXT_RIGHT);
// 	text->SetText("0");
// 	gui->AddCtrl(text);
// 
// 	button=new hgeGUIButton(CMD_SYS_PARLIFETIMELOCK, 144, 111, 16, 11, texGui, 336, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	slider=new hgeGUISlider(CMD_SYS_PARLIFETIMEMIN, 33, 126, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(0, 5, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_SYS_PARLIFETIMEMAX, 33, 138, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(0, 5, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	button=new hgeGUIButton(CMD_SYS_BLENDADDITIVE, 72, 174, 8, 8, texGui, 368, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	button=new hgeGUIButton(CMD_SYS_BLENDBLEND, 72, 188, 8, 8, texGui, 368, 176);
// 	button->SetMode(true);
// 	button->SetState(true);
// 	gui->AddCtrl(button);
	button=new hgeGUIButton(CMD_SYS_TEXTURE, 1280-49, 437, 32, 32, texGui, 337, 339);
	button->SetMode(true);
	gui->AddCtrl(button);

	// Particle movement

// 	slider=new hgeGUISlider(CMD_PM_DIRECTION, 9, 267, 149, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(0, M_PI, HGESLIDER_BAR);//2*
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	text=new hgeGUIText(CMD_PM_TDIRECTION, 112, 253, 28, 12, fnt);
// 	text->SetMode(HGETEXT_RIGHT);
// 	text->SetText("0");
// 	gui->AddCtrl(text);
// 	button=new hgeGUIButton(CMD_PM_RELATIVE, 8, 278, 8, 8, texGui, 368, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 
// 	slider=new hgeGUISlider(CMD_PM_SPREAD, 9, 314, 149, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(0, 2*M_PI, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	text=new hgeGUIText(CMD_PM_TSPREAD, 112, 300, 28, 12, fnt);
// 	text->SetMode(HGETEXT_RIGHT);
// 	text->SetText("0");
// 	gui->AddCtrl(text);
// 
// 	button=new hgeGUIButton(CMD_PM_STARTSPEEDLOCK, 143, 334, 16, 11, texGui, 336, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	button=new hgeGUIButton(CMD_FIX_START_SPEED,130,335,8,8,texGui,368,176);
// 	button->SetMode(false);
// 	gui->AddCtrl(button);
// 	slider=new hgeGUISlider(CMD_PM_STARTSPEEDMIN, 32, 349, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-2000, 2000, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_PM_STARTSPEEDMAX, 32, 361, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-2000, 2000, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	button=new hgeGUIButton(CMD_PM_GRAVITYLOCK, 143, 381, 16, 11, texGui, 336, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	slider=new hgeGUISlider(CMD_PM_GRAVITYMIN, 32, 396, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-2000, 2000, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_PM_GRAVITYMAX, 32, 408, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-2000, 2000, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	button=new hgeGUIButton(CMD_PM_RADIALLOCK, 143, 428, 16, 11, texGui, 336, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	slider=new hgeGUISlider(CMD_PM_RADIALMIN, 32, 443, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-2000, 2000, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_PM_RADIALMAX, 32, 455, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-2000, 2000, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	button=new hgeGUIButton(CMD_PM_TANGENTIALLOCK, 143, 475, 16, 11, texGui, 336, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	slider=new hgeGUISlider(CMD_PM_TANGENTIALMIN, 32, 490, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-2000, 2000, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_PM_TANGENTIALMAX, 32, 502, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-2000, 2000, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// //////////////////////////////////////////////////////////////////////////
// 	//Particle Emission glp
// 	button = new hgeGUIButton(CMD_EMISSION_POINT,168+90,528-512,8,8,texGui,368,176);
// 	button->SetMode(false);
// 	gui->AddCtrl(button);
// 
// 	button = new hgeGUIButton(CMD_EMISSION_RECT,168+90,542-512,8,8,texGui,368,176);
// 	button->SetMode(false);
// 	gui->AddCtrl(button);
// 
// 	slider=new hgeGUISlider(CMD_RECT_X,168+9,555-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(0, 1280, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	slider=new hgeGUISlider(CMD_RECT_Y,168+9,577-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(0, 720, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	slider=new hgeGUISlider(CMD_RECT_WIDTH,168+9,602-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(0, 1280, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	slider=new hgeGUISlider(CMD_RECT_HEIGHT,168+9,622-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(0, 720, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	button=new hgeGUIButton(CMD_EMISSION_CIRCLE,168+90,640-512,8,8,texGui,368,176);
// 	button->SetMode(false);
// 	gui->AddCtrl(button);
// 
// 	slider=new hgeGUISlider(CMD_CIRCLE_IN_A,168+9,651-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(0,1280,HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	text=new hgeGUIText(TEXT_CIRCLE_IN_A, 168+51, 634-512, 28, 16, fnt);
// 	text->SetMode(HGETEXT_RIGHT);
// 	text->SetColor(0xFFD4C25A);
// 	gui->AddCtrl(text);
// 	button=new hgeGUIButton(TEXT_CIRCLE_IN_A, 168+82, 634-512, 8, 8, texGui,368,176);
// 	gui->AddCtrl(button);
// 
// 	slider=new hgeGUISlider(CMD_CIRCLE_IN_B,168+9,674-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(0,720,HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	text=new hgeGUIText(TEXT_CIRCLE_IN_B, 168+51, 657-512, 28, 16, fnt);
// 	text->SetMode(HGETEXT_RIGHT);
// 	text->SetColor(0xFFD4C25A);
// 	gui->AddCtrl(text);
// 	button=new hgeGUIButton(TEXT_CIRCLE_IN_B, 168+82, 657-512, 8, 8, texGui,368,176);
// 	gui->AddCtrl(button);
// 
// 	button=new hgeGUIButton(CMD_EMISSION_RING,168+90,687-512,8,8,texGui,368,176);
// 	button->SetMode(false);
// 	gui->AddCtrl(button);
// 
// 	slider=new hgeGUISlider(CMD_CIRCLE_OUT_A,168+9,700-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(0,1279,HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	text=new hgeGUIText(TEXT_CIRCLE_OUT_A, 168+51, 683-512, 28, 16, fnt);
// 	text->SetMode(HGETEXT_RIGHT);
// 	text->SetColor(0xFFD4C25A);
// 	gui->AddCtrl(text);
// 	button=new hgeGUIButton(TEXT_CIRCLE_OUT_A, 168+82, 683-512, 8, 8, texGui,368,176);
// 	gui->AddCtrl(button);
// 
// 
// 	slider=new hgeGUISlider(CMD_CIRCLE_OUT_B,168+9,727-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(0,719,HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	text=new hgeGUIText(TEXT_CIRCLE_OUT_B, 168+51, 710-512, 28, 16, fnt);
// 	text->SetMode(HGETEXT_RIGHT);
// 	text->SetColor(0xFFD4C25A);
// 	gui->AddCtrl(text);
// 	button=new hgeGUIButton(TEXT_CIRCLE_OUT_B, 168+82, 710-512, 8, 8, texGui,368,176);
// 	gui->AddCtrl(button);
// 
// 	slider=new hgeGUISlider(CMD_ORIGIN_POINT_X,168+9,783-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(-10,1280,HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	slider=new hgeGUISlider(CMD_ORIGIN_POINT_Y,168+9,805-512,149,6,texGui,417, 177, 6, 6);
// 	slider->SetMode(-10,720,HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	// Particle appearance
// 
// 	button=new hgeGUIButton(CMD_PA_SIZELOCK, 775+480, 29, 16, 11, texGui, 336, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	slider=new hgeGUISlider(CMD_PA_SIZESTART, 664+480, 44, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(1, 100, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_PA_SIZEEND, 664+480, 56, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(1, 100, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_PA_SIZEVAR, 664+480, 68, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(0, 1, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
// 	button=new hgeGUIButton(CMD_PA_SPINLOCK, 775+480, 88, 16, 11, texGui, 336, 176);
// 	button->SetMode(true);
// 	gui->AddCtrl(button);
// 	slider=new hgeGUISlider(CMD_PA_SPINSTART, 664+480, 103, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-200, 200, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_PA_SPINEND, 664+480, 115, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(-200, 200, HGESLIDER_BARRELATIVE);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 	slider=new hgeGUISlider(CMD_PA_SPINVAR, 664+480, 127, 126, 6, texGui, 417, 177, 6, 6);
// 	slider->SetMode(0, 1, HGESLIDER_BAR);
// 	slider->SetValue(0);
// 	gui->AddCtrl(slider);
// 
	button=new hgeGUIButton(CMD_PA_ALPHALOCK, 775+480, 147, 16, 11, texGui, 336, 176);
	button->SetMode(true);
	gui->AddCtrl(button);
	slider=new hgeGUISlider(CMD_PA_ALPHASTART, 664+480, 162, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);
	slider=new hgeGUISlider(CMD_PA_ALPHAEND, 664+480, 174, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);
	slider=new hgeGUISlider(CMD_PA_ALPHAVAR, 664+480, 186, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);

	slider=new hgeGUISlider(CMD_PA_RSTART, 664+480, 221, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);
	slider=new hgeGUISlider(CMD_PA_GSTART, 664+480, 233, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);
	slider=new hgeGUISlider(CMD_PA_BSTART, 664+480, 245, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);

	slider=new hgeGUISlider(CMD_PA_REND, 664+480, 271, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);
	slider=new hgeGUISlider(CMD_PA_GEND, 664+480, 283, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);
	slider=new hgeGUISlider(CMD_PA_BEND, 664+480, 295, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);

	slider=new hgeGUISlider(CMD_PA_RGBVAR, 664+480, 321, 126, 6, texGui, 417, 177, 6, 6);
	slider->SetMode(0, 1, HGESLIDER_BAR);
	slider->SetValue(0);
	gui->AddCtrl(slider);
}
