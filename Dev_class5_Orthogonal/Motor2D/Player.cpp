#include "j1App.h"
#include "Player.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1SceneChange.h"
#include "j1Scene.h"
#include "j1EntityController.h"

Player::Player() : Entity(entityType::PLAYER)
{
}


Player::~Player()
{
}

bool Player::Awake(pugi::xml_node & config)
{
	bool ret = true;
	config = config.child("player");

	jumpFX = config.child("jumpFX").attribute("source").as_string();
	deathFX = config.child("deathFX").attribute("source").as_string();
	landFX = config.child("landFX").attribute("source").as_string();
	dashFX = config.child("dashFX").attribute("source").as_string();
	timestopFX = config.child("timestopFX").attribute("source").as_string();

	jumpForce.x = config.child("jumpForce").attribute("x").as_int();
	jumpForce.y = config.child("jumpForce").attribute("y").as_int();

	maxSpeed.x = config.child("maxSpeed").attribute("x").as_int();
	maxSpeed.y = config.child("maxSpeed").attribute("y").as_int();

	dashingSpeed.x = config.child("dashingSpeed").attribute("x").as_int();
	dashingSpeed.y = config.child("dashingSpeed").attribute("y").as_int();

	Dashtime = config.child("dashtime").attribute("value").as_int();
	gravity = config.child("gravity").attribute("value").as_int();

	colOffset.x = config.child("colOffset").attribute("x").as_int();
	colOffset.y = config.child("colOffset").attribute("y").as_int();

	animationSpeed = 5;

	SightCollider.x = 0;
	SightCollider.y = 0;
	SightCollider.w = 1;
	SightCollider.h = 1;
	return ret;
}

bool Player::Start()
{
	LoadPushbacks();

	speed = { 0,0 };

	App->audio->LoadFx(jumpFX.GetString());
	App->audio->LoadFx(deathFX.GetString());
	App->audio->LoadFx(landFX.GetString());
	App->audio->LoadFx(dashFX.GetString());
	App->audio->LoadFx(timestopFX.GetString());

	alive = true;

	isJumping = false;
	isDashing = false;
	canDash = true;

	Current_Animation = &idle;

	//Sets the player in the start position
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Collisions"))
		{
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
			{
				if (objdata->data->name == 6)
				{
					Collider.h = objdata->data->height;
					Collider.w = objdata->data->width;
					Collider.x = objdata->data->x;
					Collider.y = objdata->data->y;
				}
				else if (objdata->data->name == 4)
				{
					position = { objdata->data->x, objdata->data->y };
					Collider.x = position.x + colOffset.x;
					Collider.y = position.y + colOffset.y;
				}
			}
		}
	}

	return true;
}

bool Player::PreUpdate()
{

	return true;
}

bool Player::Update(float dt)
{
	App->scene->test = position;
	if (dt < 1 && !isDying)
	{
		FlipImage();

		if (!isDashing && canDash)
		{
			if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
				StartDashing(dt);
		}
		else if (isDashing)
		{

			if (dashtimer.Read() >= Dashtime || App->input->GetKey(SDL_SCANCODE_X) == KEY_UP)
			{
				StopDashing();
			}
		}
		if (!isDashing)
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				direction_x = 1;
				speed.x = maxSpeed.x*dt*direction_x;
			}
			else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				direction_x = -1;
				speed.x = maxSpeed.x*dt*direction_x;
			}
			else
				speed.x = 0;


			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && grounded && !App->entitycontroller->godmode)
			{
				AddSFX(1, 0);
				isJumping = true;
				grounded = false;
				//maxSpeed.x += jumpForce.x;
				//speed.x = jumpForce.x*direction_x*dt;
				speed.y = jumpForce.y;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && App->entitycontroller->godmode)
			{
				isJumping = true;
				grounded = false;
				//maxSpeed.x += jumpForce.x;
				//speed.x = jumpForce.x*direction_x*dt;
				speed.y = jumpForce.y;
			}

			if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
			{
				if (App->entitycontroller->time_state == NORMAL)
				{
					AddSFX(5, 0);
					App->entitycontroller->wanttostop = true;
				}
			}
			speed.y += gravity*dt;
		}
		//speed.y = speed.y*dt;
		//speed.x = speed.x*dt;


		speed = SpeedBoundaries(speed);
		grounded = false;


		speed = Collider_Overlay(speed);
		speed.x = (int)speed.x;
		speed.y = (int)speed.y;
		NormalizeAnimationSpeed(dt);
		ChangeAnimation();
		PlayerMovement();
		PositionCollider();

	}
	else if (dt < 1 && isDying)
	{

		speed.x = 200*dt*-direction_x;
		speed.y = gravity*dt*7;

		speed = Collider_Overlay(speed);
		speed.x = (int)speed.x;
		speed.y = (int)speed.y;


		if (speed.y == 0 || position.y > App->map->data.height*App->map->data.tile_height)
		{
			alive = false;
			speed.x = 0;
		}

		ChangeAnimation();
		PlayerMovement();
		PositionCollider();
	}


	/*LOG("CurrenFrame: %f", Current_Animation->GetCurrentFrameinFloat());
	LOG("CurrentAnimation speed: %f", Current_Animation->speed);*/
	return true;
}

bool Player::PostUpdate()
{
	if (!alive)
	{
		App->scenechange->ChangeMap(App->scene->currentMap, App->scene->fade_time);
	}

	PositionCameraOnPlayer();

	return true;
}



void Player::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	speed.x = data.child("speed").attribute("x").as_int();
	speed.y = data.child("speed").attribute("y").as_int();
	Collider.w = data.child("collider").attribute("width").as_int();
	Collider.h = data.child("collider").attribute("height").as_int();
	Collider.x = data.child("collider").attribute("x").as_int();
	Collider.y = data.child("collider").attribute("y").as_int();
	grounded = data.child("grounded").attribute("value").as_bool();
	isDashing = data.child("dashing").attribute("value").as_bool();
	currentDashtime = data.child("dashtime").attribute("value").as_float();

}

// Save Game State
void Player::Save(pugi::xml_node& data) const
{
	data.append_child("player");
	
	data.child("player").append_child("position").append_attribute("x") = position.x;
	data.child("player").child("position").append_attribute("y") = position.y;
	data.child("player").append_child("speed").append_attribute("x") = speed.x;
	data.child("player").child("speed").append_attribute("y") = speed.y;
	data.child("player").append_child("collider").append_attribute("width") = Collider.w;
	data.child("player").child("collider").append_attribute("height") = Collider.h;
	data.child("player").child("collider").append_attribute("x") = Collider.x;
	data.child("player").child("collider").append_attribute("y") = Collider.y;
	data.child("player").append_child("grounded").append_attribute("value") = grounded;
	data.child("player").append_child("dashing").append_attribute("value") = isDashing;
	data.child("player").append_child("currentDashtime").append_attribute("value") = currentDashtime;

}




void Player::ChangeAnimation()
{
	if (!isDying)
	{
		if (!isDashing)
		{
			if (speed.y == 0 && grounded)
				if (speed.x == 0)
					Current_Animation = &idle;
				else
					Current_Animation = &running;
			else if (speed.y < 0)
				Current_Animation = &jumping_up;
			else
				Current_Animation = &falling;
		}

		else
			Current_Animation = &dashing;
	}
	else
		Current_Animation = &dying;
}

void Player::PlayerMovement()
{

	position.x += speed.x;
	position.y += speed.y;

	
}

void Player::Restart()
{
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
		if (obj->data->name == ("Collisions"))
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
				if (objdata->data->name == 4)
				{
					position = { objdata->data->x, objdata->data->y };
					Collider.x = position.x + colOffset.x;
					Collider.y = position.y + colOffset.y;
					speed.x = 0;
					speed.y = 0;
					Current_Animation = &idle;
				}
	LOG("Ded");
	isDying = false;
	alive = true;
}

bool Player::PositionCameraOnPlayer()
{
	App->render->camera.x = position.x - App->render->camera.w / 3;
	if (App->render->camera.x < 0)App->render->camera.x = 0;
	App->render->camera.y = position.y - App->render->camera.h / 2;
	if (App->render->camera.y + App->win->height > App->map->data.height*App->map->data.tile_height)App->render->camera.y = App->map->data.height*App->map->data.tile_height - App->win->height;
	return true;
}

void Player::LoadPushbacks()
{
	idle.PushBack({ 5, 17, 56, 73 });

	running.PushBack({ 89, 17, 60, 73 });
	running.PushBack({ 180, 17, 60, 73 });
	running.PushBack({ 277, 17, 60, 73 });
	running.PushBack({ 375, 17, 60, 73 });
	running.PushBack({ 470, 17, 60, 73 });
	running.PushBack({ 565, 17, 60, 73 });
	running.loop = true;
	running.speed = 0.25f;

	jumping_up.PushBack({ 672, 27, 53, 63 });
	jumping_up.PushBack({ 764, 0, 49, 75 });
	jumping_up.loop = false;
	jumping_up.speed = 0.5f;

	falling.PushBack({ 861, 17, 53, 73 });

	dashing.PushBack({ 635, 224, 79, 71 });
	dashing.PushBack({ 741, 226, 81, 69 });
	dashing.PushBack({ 834, 227, 82, 68 });
	dashing.PushBack({ 929, 228, 82, 67 });
	dashing.PushBack({ 390, 223, 73, 72 });
	dashing.PushBack({ 468, 219, 76, 69 });
	dashing.PushBack({ 548, 219, 76, 69 });
	dashing.loop = false;
	dashing.speed = 0.6f;

	dying.PushBack({ 0, 112, 95, 77 });
	dying.PushBack({ 95, 112, 95, 77 });
	dying.PushBack({ 190, 112, 95, 77 });
	dying.PushBack({ 285, 112, 95, 77 });
	dying.PushBack({ 380, 112, 95, 77 });
	dying.PushBack({ 475, 112, 95, 77 });
	dying.PushBack({ 570, 112, 95, 77 });
	dying.loop = false;
	dying.speed = 1.5f;
}



void Player::CleanUp()
{
	LOG("Deleting player");

}



void Player::BecomeGrounded()
{
	if (isJumping)
	{
 		isJumping = false;
		maxSpeed.x -= jumpForce.x;
	}

	if (Current_Animation == &falling)
		AddSFX(3, 0);

	canDash = true;
	grounded = true;
	jumping_up.Reset();
}

void Player::StartDashing(float dt)
{
	AddSFX(4, 0);
	isDashing = true;
	canDash = false;
	speed.x = dashingSpeed.x * direction_x*dt;
	speed.y = dashingSpeed.y*dt;
	dashtimer.Start();
}

void Player::StopDashing()
{
	isDashing = false;
	dashing.Reset();
}
