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

#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include "PS2Defines.h"
#include "ps2matrix4x4.h"
#include "sps2wrap.h"
#include "dma.h"
#include "primitives.h"


PS2Sprite::PS2Sprite()  : 
m_ScreenW(640.0f),
m_ScreenH(512.0f)
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 128.0f;
	
	Initialise();
}

PS2Sprite::PS2Sprite(float x, float y) : 
m_ScreenW(640.0f),
m_ScreenH(512.0f)
{
	m_x = x; 
	m_y = y;
	m_z = 128.0f;
	
	Initialise();	
}

PS2Sprite::PS2Sprite(float x, float y, float width, float height) : 
m_ScreenW(640.0f),
m_ScreenH(512.0f)
{
	m_x = x; 
	m_y = y;
	m_z = 128.0f;
	
	Initialise();	
	m_w = width;
	m_h = height;
	RecalculateVerts();
}

PS2Sprite::PS2Sprite(float x, float y, float z) : 
m_ScreenW(640.0f),
m_ScreenH(512.0f)
{
	m_x = x; 
	m_y = y;
	m_z = z;
	
	Initialise();
}

PS2Sprite::~PS2Sprite()
{
	// Nothing to destruct
}

int PS2Sprite::operator < (const PS2Sprite &rhs) const
{
   if( this->m_z < rhs.m_z ) return 1;
   return 0;
}

void PS2Sprite::Initialise(void)
{	
	//m_z = 128.0f;
	m_w = m_h = 20.0f;
	m_r = m_g = m_b = m_a = 0x80;
	
	m_Vx = 1.7f;
	m_Vy = 2.2f;
	scaleFactor = 1.0f;
	
	m_TopLeft.x 		= -m_w/2.0f;
	m_TopLeft.y 		= -m_h/2.0f;
	m_BottomLeft.x 		= -m_w/2.0f;
	m_BottomLeft.y 		=  m_h/2.0f;
	m_TopRight.x 		=  m_w/2.0f;
	m_TopRight.y 		= -m_h/2.0f;
	m_BottomRight.x 	=  m_w/2.0f;
	m_BottomRight.y 	=  m_h/2.0f;
}

void PS2Sprite::RecalculateVerts()
{
	m_TopLeft.x 		= -m_w/2.0f;
	m_TopLeft.y 		= -m_h/2.0f;
	m_BottomLeft.x 		= -m_w/2.0f;
	m_BottomLeft.y 		=  m_h/2.0f;
	m_TopRight.x 		=  m_w/2.0f;
	m_TopRight.y 		= -m_h/2.0f;
	m_BottomRight.x 	=  m_w/2.0f;
	m_BottomRight.y 	=  m_h/2.0f;
}

void PS2Sprite::UniformScale(float scale)
{
	scaleFactor = scale;
}

void PS2Sprite::SetWidth(float width)
{
	m_w = width;
	RecalculateVerts();
}

void PS2Sprite::SetColour(int colour) 
{
	m_r = (colour & 0xFF0000) >> 16;
	m_g = (colour & 0x00FF00) >> 8;
	m_b = (colour & 0x0000FF);
}

void PS2Sprite::Move(void)
{
	m_x += m_Vx;
	m_y += m_Vy;
	
	if((m_x >= (m_ScreenW - m_w)/2.0f) && (m_Vx > 0.0f))m_Vx = -m_Vx;
	if((m_x <= (-m_ScreenW + m_w)/2.0f) && (m_Vx < 0.0f))m_Vx = -m_Vx;
	
	if((m_y >= (m_ScreenH - m_h)/2.0f) && (m_Vy > 0.0f))m_Vy = -m_Vy;
	if((m_y <= (-m_ScreenH + m_h)/2.0f) && (m_Vy < 0.0f))m_Vy = -m_Vy;
}

void PS2Sprite::MoveTo(const float x, const float y, const float z)
{
	m_x = x;
	m_y = y;
	m_z = z;	
	
}

void PS2Sprite::MoveTo(float x, float y)
{
	m_x = x;
	m_y = y;
}

void PS2Sprite::Skew(float leftHeight, float rightHeight)
{
	m_TopLeft.x 		= -m_w/2.0f;
	m_TopLeft.y 		= -leftHeight/2.0f;
	m_BottomLeft.x 		= -m_w/2.0f;
	m_BottomLeft.y 		=  leftHeight/2.0f;
	m_TopRight.x 		=  m_w/2.0f;
	m_TopRight.y 		= -rightHeight/2.0f;
	m_BottomRight.x 	=  m_w/2.0f;
	m_BottomRight.y 	=  rightHeight/2.0f;	
}

void PS2Sprite::SetDepth(const float z)
{
	m_z = z;
}


// ********* Don't Change anything in this method
void PS2Sprite::Render(void) const
{
	VIFDynamicDMA.StartDirect();
	
	// Add the GIFTag
	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(	4,							// NLOOP
											1,							// EOP
											1,							// PRE
											GS_PRIM(PRIM_TRI_STRIP, 	// PRIM
												PRIM_IIP_FLAT, 
												PRIM_TME_OFF,
												PRIM_FGE_OFF, 
												PRIM_ABE_OFF, 
												PRIM_AA1_OFF, 
												PRIM_FST_UV, 
												PRIM_CTXT_CONTEXT1, 
												PRIM_FIX_NOFIXDDA),
											GIF_FLG_PACKED,					//FLG
											GS_BATCH_2(	GIF_REG_RGBAQ, 
														GIF_REG_XYZ2)));	//BATCH	
		
	// Add the colour (Top Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position (Top Left)  	   	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopLeft.x*scaleFactor)*16.0f), 
									 (int)((2048.0f + m_y + m_TopLeft.y*scaleFactor)*16.0f),  
									 (int)m_z, 
									  0));
	
	// Add the colour (Bottom Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position (Bottom Left)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomLeft.x*scaleFactor)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomLeft.y*scaleFactor)*16.0f),  
									 (int)m_z, 
									  0));
									  
	
	// Add the colour (Top Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position (Top Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopRight.x*scaleFactor)*16.0f), 
									 (int)((2048.0f + m_y + m_TopRight.y*scaleFactor)*16.0f),  
									 (int)m_z, 
									  0));

	// Add the colour (Bottom Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position  (Bottom Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomRight.x*scaleFactor)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomRight.y*scaleFactor)*16.0f),  
									 (int)m_z, 
									  0));
	
	
	VIFDynamicDMA.EndDirect();
}

PS2SpriteT::PS2SpriteT()
{
	m_tu = 0;
	m_tv = 0;
	m_tw = 64;
	m_th = 64;
	
}
	
PS2SpriteT::PS2SpriteT(const float x, const float y):
PS2Sprite(x, y)
{
	m_tu = 0;
	m_tv = 0;
	m_tw = 64;
	m_th = 64;
}


PS2SpriteT::PS2SpriteT(const float x, const float y, const float z):
PS2Sprite(x, y, z)
{
	m_tu = 0;
	m_tv = 0;
	m_tw = 64;
	m_th = 64;
}

PS2SpriteT::PS2SpriteT(const float x, const float y, const float width, const float height):
	PS2Sprite(x, y, width, height)
{
	m_tu = 0;
	m_tv = 0;
	m_tw = 64;
	m_th = 64;
}

void PS2SpriteT::SetUVs(int baseU, int baseV, int width, int height)
{
	m_tu = baseU;
	m_tv = baseV;
	m_tw = width;
	m_th = height;
}

PS2SpriteT::~PS2SpriteT()
{
	// Nothing to destruct
}

// ********* Don't Change anything in this method
void PS2SpriteT::Render(void) const
{
	VIFDynamicDMA.StartDirect();
	
	// Add the GIFTag
	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(	4,							// NLOOP
											1,							// EOP
											1,							// PRE
											GS_PRIM(PRIM_TRI_STRIP, 	// PRIM
												PRIM_IIP_FLAT, 
												PRIM_TME_ON,
												PRIM_FGE_OFF, 
												PRIM_ABE_ON, 
												PRIM_AA1_OFF, 
												PRIM_FST_UV, 
												PRIM_CTXT_CONTEXT1, 
												PRIM_FIX_NOFIXDDA),
											GIF_FLG_PACKED,					//FLG
											GS_BATCH_3(	GIF_REG_RGBAQ,
														GIF_REG_UV, 
														GIF_REG_XYZ2)));	//BATCH	
		
	// Add the colour (Top Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	
	// Add UV texture coordinates (Top Left)
    VIFDynamicDMA.Add128(PACKED_UV(	((m_tu)<<4), ((m_tv)<<4)));
		    	
	// Add the position (Top Left)  	   	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopLeft.x*scaleFactor)*16.0f), 
									 (int)((2048.0f + m_y + m_TopLeft.y*scaleFactor)*16.0f),  
									 (int)m_z, 
									  0));
	
	// Add the colour (Bottom Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	
	// Add UV texture coordinates (Bottom Left)
    VIFDynamicDMA.Add128(PACKED_UV(	(m_tu)<<4, (m_tv + m_th)<<4));
	
		    	
	// Add the position (Bottom Left)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomLeft.x*scaleFactor)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomLeft.y*scaleFactor)*16.0f),  
									 (int)m_z, 
									  0));
									  
	
	// Add the colour (Top Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	
	// Add UV texture coordinates (Top Right)
    VIFDynamicDMA.Add128(PACKED_UV(	(m_tu + m_tw)<<4, (m_tv)<<4));
		    	
	// Add the position (Top Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopRight.x*scaleFactor)*16.0f), 
									 (int)((2048.0f + m_y + m_TopRight.y*scaleFactor)*16.0f),  
									 (int)m_z, 
									  0));

	// Add the colour (Bottom Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	
	// Add UV texture coordinates
    VIFDynamicDMA.Add128(PACKED_UV(	(m_tu + m_tw)<<4, (m_tv + m_th)<<4));
		    	
	// Add the position  (Bottom Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomRight.x*scaleFactor)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomRight.y*scaleFactor)*16.0f),  
									 (int)m_z, 
									  0));
	
	
	VIFDynamicDMA.EndDirect();
}

