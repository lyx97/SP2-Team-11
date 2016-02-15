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
		kAmbient.Set(1.f, 1.f, 1.f);
		kDiffuse.Set(1.f, 1.f, 1.f);
		kSpecular.Set(1.f, 1.f, 1.f);
		kShininess = 1.f;
	}
};

#endif // MATERIAL_H
