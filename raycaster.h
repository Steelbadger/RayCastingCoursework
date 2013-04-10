#ifndef __RAYCASTER_H__
#define __RAYCASTER_H__

#include "vector2.h"
#include "level.h"

struct RayReturnData
{
	RayReturnData(float r, float UVBase, unsigned int texID):range(r), v(UVBase), TextureID(texID){};
	RayReturnData(float r, unsigned int texID):range(r), v(0), TextureID(texID){};	
	RayReturnData():range(999999.99f), v(0), TextureID(0){};
	float range;
	float v;
	unsigned int TextureID;
};


class RayCaster
{
public:
	RayCaster();
	~RayCaster();
		
	RayReturnData CastRay(Vector2 base, Vector2 direction, Level& level);
	RayReturnData AltCastRay(Vector2 base, Vector2 direction, Level& level);
	
private:
	RayReturnData CheckHorizontalDivisions(Vector2 base, Vector2 direction, Level& level);
	RayReturnData CheckVerticalDivisions(Vector2 base, Vector2 direction, Level& level);
	RayReturnData CheckInterleavedDivisions(Vector2 base, Vector2 direction, Level& level);
	
	static const int MAXRANGE;
	static const float MAXRANGESQR;
	
};

inline bool operator > (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return (r1.range > r2.range);
}

inline bool operator < (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return (r1.range < r2.range);
}

inline bool operator <= (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return r1.range <= r2.range;
}

inline bool operator >= (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return r1.range >= r2.range;
}

inline bool operator == (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return r1.range == r2.range;
}

inline RayReturnData Min (	const RayReturnData &r1,
							const RayReturnData &r2)
{
	return ((r1 < r2)? r1 : r2);
}

inline RayReturnData Max (	const RayReturnData &r1,
							const RayReturnData &r2)
{
	return ((r1 > r2)? r1 : r2);
}


#endif