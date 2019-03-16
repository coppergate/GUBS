
#include "stdafx.h"

#include "Volume.h"

namespace GUBS_Supply
{
	Volume::Volume() : Volume(0, 0, 0)
	{

	}

	Volume::Volume(float h, float w, float l) : _Height(h), _Width(w), _Length(l)
	{

	}

	Volume::Volume(const Volume& vol) : Volume(vol._Height, vol._Width, vol._Length)
	{

	}

	Volume::operator float() const
	{
		return _Height * _Width * _Length;
	}
}