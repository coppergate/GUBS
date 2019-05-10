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

		Volume() : Volume(0, 0, 0) {}

		Volume(float h, float w, float l) : _Height(h), _Width(w), _Length(l)
		{

		}

		Volume(const Volume& vol) = default;

		operator float() const
		{
			return _Height * _Width * _Length;
		}
	};
}