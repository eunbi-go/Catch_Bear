#include "pch.h"
#include "ShieldParticle.h"

ShieldParticle::ShieldParticle()
{
}

ShieldParticle::~ShieldParticle()
{
}

void ShieldParticle::LateUpdate()
{
	if (isOn) ShieldParticleOn();
}

void ShieldParticle::ShieldParticleOn()
{
	
}
