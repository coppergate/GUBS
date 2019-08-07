#pragma once

#include "infrastructure.h"

namespace GUBS_Supply
{

	class Volume
	{
		//	All linear measurements are in METERS

		double _Height;
		double _Width;
		double _Length;

	public:

		Volume() : Volume(0, 0, 0) {}

		Volume(double h, double w, double l) : _Height(h), _Width(w), _Length(l)
		{

		}

		Volume(const Volume& vol) = default;

		operator double() const
		{
			return _Height * _Width * _Length;
		}
	};
}