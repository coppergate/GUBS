#pragma once

#include "infrastructure.h"

namespace GUBS_Supply
{

	class Volume
	{
		//	All linear measurements are in METERS

		float _Height;
		float _Width;
		float _Length;

	public:

		Volume();

		Volume(float h, float w, float l);

		Volume(const Volume& vol);

		operator float() const;
	};
}