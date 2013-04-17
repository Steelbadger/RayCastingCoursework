/*
* STL list Application
*
* University of Abertay Dundee
* May be used for educational purposed only
*
* Author - Dr Henry S Fortuna
*
* $Revision: 1.4 $
* $Date: 2008/01/13 21:02:48 $
*
*/


#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include <sps2tags.h>
#include <sps2regstructs.h>
#include "PS2Defines.h"


class PS2Sprite
{
public:

	PS2Sprite();
	
	PS2Sprite(const float x, const float y);
	
	PS2Sprite(const float x, const float y, const float z);

	PS2Sprite(const float x, const float y, const float width, const float height);
	
	virtual ~PS2Sprite();
	
	// This operator is required for built-in STL list functions like sort
	int operator < (const PS2Sprite & rhs) const;
	
	virtual void Render(void) const;
	
	void Move(void);
	
	void Skew(float lHeight, float rHeight);
	void UniformScale(float factor);
	
	void SetColour(int colour);
	void SetAlpha(char alpha);
	
	void MoveTo(const float x, const float y);
	
	void MoveTo(const float x, const float y, const float z);
	void SetDepth(const float z);
	float GetX(){return m_x;}
	float GetY(){return m_y;}
	float GetZ(){return m_z;}
	float GetWidth(){return m_w;}
	float GetHeight(){return m_h;}
	float GetScale(){return scaleFactor;}
	
	void MoveDeltaX(const float x);
	
	void MoveDeltaY(const float y);
	
	void SetWidth(float width);
	
protected:
		

	void Initialise(void);		// Initialise variables
	void RecalculateVerts();

	float m_x, m_y;				// position on screen
	float m_z;					// z depth (big = near)
	float m_w, m_h;				// width and height
	uint8 m_r, m_g, m_b, m_a;	// colour and alpha
	float m_Vx, m_Vy;			// velocity vector
	float scaleFactor;
	
	struct Vector2{
		float x, y;
	} m_TopLeft, m_BottomLeft, m_TopRight, m_BottomRight;		// Local Space Vertex coordinates
	
	float m_ScreenW;
	float m_ScreenH;
};

class PS2SpriteT : public PS2Sprite
{
public:
	
	PS2SpriteT();
	
	PS2SpriteT(const float x, const float y);
	
	PS2SpriteT(const float x, const float y, const float z);
	
	PS2SpriteT(const float x, const float y, const float width, const float height);	
	
	void SetUVs(int baseU, int baseV, int width, int height);
	
	virtual ~PS2SpriteT();
	
	void Render(void) const;
	
	
protected:
	
	int m_tu, m_tv;				// Texture coordinates (top left)
	int m_tw, m_th;				// Texture coordinates (width and height)
};


#endif