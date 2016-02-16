#ifndef MATERIAL_H
#define MATERIAL_H

struct Component
{
	float r, g, b;
	void Set(float r, float g, float b){ this->r = r, this->g = g, this->b = b; }
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f){ Set(r, g, b); }
};

struct Material
{
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess;

	Material()
	{
		kAmbient.Set(0.5f, 0.5f, 0.5f);
		kDiffuse.Set(0.5f, 0.5f, 0.5f);
		kSpecular.Set(0.7f, 0.7f, 0.7f);
		kShininess = 0.78125f;
	}
};

#endif // MATERIAL_H
