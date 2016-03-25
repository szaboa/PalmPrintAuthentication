#pragma once

class IFeature
{
public:
	virtual void save(int userId) = 0;

	virtual ~IFeature(){};
};

