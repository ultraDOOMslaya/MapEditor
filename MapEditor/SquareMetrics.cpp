#include "SquareMetrics.h"



SquareMetrics::SquareMetrics()
{
	noiseSource.load("PerlinNoise.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}


SquareMetrics::~SquareMetrics()
{
}


Ogre::Vector3 SquareMetrics::SampleNoise(Ogre::Vector3 position)
{
	Ogre::ColourValue val = noiseSource.getColourAt(position.x, position.z, 0);
	Ogre::Vector3 filteredVector = Ogre::Vector3(val.r, val.g, val.b);
	return filteredVector;
}


//Ogre::Vector3 SquareMetrics::Perturb(Ogre::Vector3 position)
//{
//	Ogre::Vector3 sample = SampleNoise(position);
//	position.x += sample.x;
//	position.y += sample.y;
//	position.z += sample.z;
//	return position;
//}


Ogre::Vector3 SquareMetrics::Perturb(int x, int y, int z)
{
	Ogre::ColourValue val = noiseSource.getColourAt(x, z, 0);
	Ogre::Vector3 sample = Ogre::Vector3(val.r, val.g, val.b);

	//Ogre::Vector3 sample = SampleNoise(position);
	float pos_x = x + (val.r * cellPerturbStrength);
	float pos_y = y;
	float pos_z = z + (val.b * cellPerturbStrength);

	Ogre::Vector3 position = Ogre::Vector3(pos_x, y, pos_z);
	return position;
}