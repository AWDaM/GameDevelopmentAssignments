#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1SceneChange.h"
#include "j1SceneSwitch.h"
#include "j1IntroScene.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "UIElement.h"
#include "InheritedImage.h"
#include "InteractiveLabelledImage.h"
#include "Window.h"



j1IntroScene::j1IntroScene()
{
	name.create("introscene");
}


j1IntroScene::~j1IntroScene()
{
}

bool j1IntroScene::Awake()
{
	return true;
}

bool j1IntroScene::Start()
{

//	p2SString name("gui/background.png");
//	SDL_Rect rect = { 0,0,0,0 };
//	App->gui->AddImage_From_otherFile(rect, { 0,0 }, name);
//
////	background = App->tex->Load("textures/introbg.png");
//	SDL_Rect test;
//	test.x = 0;
//	test.y = 0;
//	test.w = 123;
//	test.h = 74;
//	testWindow = App->gui->AddWindow(test, true);
//
//	InheritedImage* tmp = App->gui->AddImage(test, { 0,0 }, &test, true);
//
//	testWindow->AddElementToWindow(tmp, { 0,0 });
	pugi::xml_document	Gui_config_file;
	pugi::xml_node		guiconfig;

	guiconfig = App->LoadConfig(Gui_config_file, "Gui_config.xml");

	guiconfig = guiconfig.child("introscene");

	App->gui->Load_UIElements(guiconfig,this);
	App->gui->Load_SceneWindows(guiconfig,this);
	return true;
}

bool j1IntroScene::PreUpdate()
{
	return true;
}

bool j1IntroScene::Update(float dt)
{

	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->sceneswitch->SwitchScene(App->scene,this);
	}

	App->gui->Draw(dt);

	//App->render->Blit(background, 0, 0);

	return true;
}

bool j1IntroScene::PostUpdate()
{
	return true;
}

bool j1IntroScene::CleanUp()
{
	return true;
}

bool j1IntroScene::OnEvent(UIElement * element, int eventType)
{
	bool ret = true;
	element->HandleAnimation(eventType);

	if (eventType == EventTypes::PRESSED_ENTER)
		ret = element->OnEvent();

	if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == QUIT)
	{
		LOG("Quit button");
		ret = false;
	}

	else if (eventType == EventTypes::LEFT_MOUSE_PRESSED && element->type == NEWGAME)
	{
		App->sceneswitch->SwitchScene(App->scene, App->introscene);
		LOG("New game");
	}
	return ret;
}
