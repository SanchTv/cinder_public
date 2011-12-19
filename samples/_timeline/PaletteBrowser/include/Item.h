//
//  Item.h
//  PaletteBrowser
//
//  Created by Robert Hodgin on 11/7/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Rand.h"
#include "cinder/Timeline.h"
#include "cinder/Rect.h"
#include "cinder/Area.h"
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "cinder/Font.h"
#include "cinder/Text.h"
#include "Swatch.h"


class Item {
  public:
	Item( int index, ci::Vec2f pos, const std::string &title, const std::string &desc, ci::Surface palette );
	void setColors();
	void mouseOver( ci::Timeline &timeline );
	void mouseOff( ci::Timeline &timeline );
	void select( ci::Timeline &timeline, float leftBorder );
	void deselect( ci::Timeline &timeline );
	void update();
	void drawText() const;
	void drawBgBar() const;
	void drawSwatches() const;
	
	void setSelected(){ mIsSelected = true; };
	void setDeselected(){ mIsSelected = false; };
	
	std::string getTitle() const { return mTitle; };
	bool getSelected() const { return mIsSelected; };
	bool isPointIn( const ci::Vec2f &pt ) const;
	bool isBelowTextThreshold() const;
	
	int mIndex;

	static void	setFonts( const ci::Font &smallFont, const ci::Font &bigFont );
	
  private:
	// title
	std::string			mTitle;
	ci::gl::Texture		mTitleTex, mTitleBigTex;
	ci::Anim<ci::Vec2f>	mTitlePos;
	ci::Vec2f			mTitleStart, mTitleDest1, mTitleDest2, mTitleFinish;
	ci::Vec2f			mMouseOverDest;
	ci::Anim<ci::Color> mTitleColor;
	ci::Anim<float>		mTitleAlpha;
	ci::Area			mTitleArea;
	float				mTitleWidth, mTitleHeight;
	
	// desc
	std::string			mDesc;
	ci::gl::Texture		mDescTex;
	ci::Anim<ci::Vec2f> mDescPos;
	ci::Vec2f			mDescStart, mDescDest;
	ci::Anim<float>		mDescAlpha;
	
	// swatches
	ci::Surface			mPalette;	
	std::vector<Swatch> mSwatches;	
	
	// bar
	ci::Vec2f			mBarPos;
	ci::Anim<float>		mBarWidth, mBarHeight;
	float				mMaxBarWidth;
	ci::Rectf			mBarRect;
	ci::Anim<ci::Color> mBarColor;
	ci::Anim<float>		mBarAlpha;
	
	ci::Anim<float>		mFadeFloat;

	// TODO: isBeingSelected is a sloppy fix
	bool				mIsSelected, mIsBeingSelected;
	
	static ci::Font sSmallFont, sBigFont;	// small and large fonts for Text textures
};