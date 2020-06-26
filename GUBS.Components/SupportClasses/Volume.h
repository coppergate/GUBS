#pragma once

#include "SupportClasses\infrastructure.h"

namespace GUBS_Support
{

	struct Volume
	{
		//	All linear measurements are in METERS
		 double _Height { 0 };
		 double _Width { 0 };
		 double _Length { 0 };

		Volume(double h, double w, double l) : _Height(h), _Width(w), _Length(l)
		{	}

		Volume() = default;
		virtual ~Volume() = default;				// destructor (virtual if Volume is meant to be a base class)
		Volume(const Volume&) = default;			// copy constructor
		Volume(Volume&&) = default;					// move constructor
		Volume& operator=(const Volume&) = default;	// copy assignment
		Volume& operator=(Volume&&) = default;		// move assignment

		operator double() const
		{
			return _Height * _Width * _Length;
		}

		static const Volume EmptyVolume;
	};


}