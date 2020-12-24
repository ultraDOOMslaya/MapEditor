#pragma once

#include "Ogre.h"

class SquareMetrics
{
public:
	SquareMetrics();
	~SquareMetrics();

	Ogre::Vector3 SampleNoise(Ogre::Vector3 position);
	//Ogre::Vector3 Perturb(Ogre::Vector3 position);
	Ogre::Vector3 Perturb(int x, int y, int z);

	const float cellPerturbStrength = 8.0f;
	Ogre::Image noiseSource;

};

