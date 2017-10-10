#pragma once

#include "Camera.h"
#include "Hero.h"
#include "Projectile.h"
#include "Scene.h"
#include "SingleEmitter.h"
#include "RadialEmitter.h"
#include <vector>

class Scene_Shooter :public Scene
{
public:
	Scene_Shooter( Keyboard &Kbd, Graphics &Gfx );

	void Update( float DeltaTime );
	void Draw()const;

private:
	void DoHeroInput();
	void RemoveDeadProjectiles();
	void CollectNewProjectiles();
	void UpdateProjectiles( float DeltaTime );
private:
	Hero m_hero;
	TrackingCamera m_camera;

	ParticleVector m_projectiles;
	SingleEmitter m_ammoEmitter;
	RadialEmitter m_explosionEmitter;
};

