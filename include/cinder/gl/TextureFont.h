/*
 Copyright (c) 2011, The Cinder Project: http://libcinder.org All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Text.h"
#include "cinder/Font.h"
#include "cinder/gl/Texture.h"

#include <map>

namespace cinder { namespace gl {

typedef std::shared_ptr<class TextureFont>	TextureFontRef;

class TextureFont {
  public:
	class Format {
	  public:
		Format() : mTextureWidth( 1024 ), mTextureHeight( 1024 ), mPremultiply( false )
		{}
		
		//! Sets the width of the textures created internally for glyphs. Default \c 1024
		Format&		textureWidth( int32_t textureWidth ) { mTextureWidth = textureWidth; return *this; }
		//! Returns the width of the textures created internally for glyphs. Default \c 1024
		int32_t		getTextureWidth() const { return mTextureWidth; }
		//! Sets the height of the textures created internally for glyphs. Default \c 1024
		Format&		textureHeight( int32_t textureHeight ) { mTextureHeight = textureHeight; return *this; }
		//! Sets the height of the textures created internally for glyphs. Default \c 1024
		int32_t		getTextureHeight() const { return mTextureHeight; }
		
		//! Sets whether the TextureFont render premultiplied output. Default \c false
		Format&		premultiply( bool premult = true ) { mPremultiply = premult; return *this; }
		//! Returns whether the TextureFont render premultiplied output. Default \c false
		bool		getPremultiply() const { return mPremultiply; }
		
	  protected:
		int32_t		mTextureWidth, mTextureHeight;
		bool		mPremultiply;
	};

	struct DrawOptions {
		DrawOptions() : mClipHorizontal( true ), mClipVertical( true ), mPixelSnap( true ), mLigate( false ) {}

		//! Returns whether the output clips horizontally
		bool			getClipHorizontal() const { return mClipHorizontal; }		
		//! Sets whether the output clips horizontally
		DrawOptions&	clipHorizontal( bool clipH = true ) { mClipHorizontal = clipH; return *this; }

		//! Returns whether the output clips vertically
		bool			getClipVertical() const { return mClipVertical; }		
		//! Sets whether the output clips vertically
		DrawOptions&	clipVertical( bool clipV = true ) { mClipVertical = clipV; return *this; }

		//! Returns whether the output glyphs are snapped to pixel boundaries. This sharpens static text but prevents subpixel animation
		bool			getPixelSnap() const { return mPixelSnap; }		
		//! Sets whether the output glyphs are snapped to pixel boundaries. This sharpens static text but prevents subpixel animation
		DrawOptions&	pixelSnap( bool pixelSnap = true ) { mPixelSnap = pixelSnap; return *this; }

		//! Returns whether advanced ligatures are used, which must have been instantiated by the \a supportedChars parameter of the TextureFont::create() call. Default to \c false.
		bool			getLigate() const { return mLigate; }
		//! Sets whether advanced ligatures are used, which must have been instantiated by the \a supportedChars parameter of the TextureFont::create() call. Default to \c false.
		DrawOptions&	ligate( bool useLigatures = true ) { mLigate = useLigatures; return *this; }
						
	  protected:
		bool		mClipHorizontal, mClipVertical, mPixelSnap, mLigate;
	};

	//! Creates a new TextureFontRef with font \a font, ensuring that glyphs necessary to render \a supportedChars are renderable, and format \a format
	static TextureFontRef		create( const Font &font, const std::string &supportedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456890().?!,:;'\"&*=+-/\\@#_[]<>%^llflphridséèáà", const Format &format = Format() )
	{ return TextureFontRef( new TextureFont( font, supportedChars, format ) ); }
	
	//! Draws string \a str at baseline \a baseline with DrawOptions \a options
	void	drawString( const std::string &str, const Vec2f &baseline, const DrawOptions &options = DrawOptions() );
	//! Draws string \a str fit inside \a fitRect, with internal offset \a offset and DrawOptions \a options
	void	drawString( const std::string &str, const Rectf &fitRect, const Vec2f &offset = Vec2f::zero(), const DrawOptions &options = DrawOptions() );
#if defined( CINDER_COCOA )
	//! Draws word-wrapped string \a str fit inside \a fitRect, with internal offset \a offset and DrawOptions \a options. Mac & iOS only.
	void	drawStringWrapped( const std::string &str, const Rectf &fitRect, const Vec2f &offset = Vec2f::zero(), const DrawOptions &options = DrawOptions() );
#endif
	//! Draws the glyphs in \a glyphMeasures at baseline \a baseline with DrawOptions \a options. \a glyphMeasures is a vector of pairs of glyph indices and offsets for the glyph baselines
	void	drawGlyphs( const std::vector<std::pair<uint16_t,Vec2f> > &glyphMeasures, const Vec2f &baseline, const DrawOptions &options = DrawOptions() );
	//! Draws the glyphs in \a glyphMeasures clipped by \a clip, with \a offset added to each of the glyph offsets with DrawOptions \a options. \a glyphMeasures is a vector of pairs of glyph indices and offsets for the glyph baselines.
	void	drawGlyphs( const std::vector<std::pair<uint16_t,Vec2f> > &glyphMeasures, const Rectf &clip, Vec2f offset, const DrawOptions &options = DrawOptions() );

	//! Returns the ascent of the font
	float	getAscent() const { return mFont.getAscent(); }
	//! Returns the descent of the font
	float	getDescent() const { return mFont.getDescent(); }
	//! Returns whether the TextureFont output premultipled output. Default is \c false.
	bool	isPremultiplied() const { return mFormat.getPremultiply(); }

  protected:
	TextureFont( const Font &font, const std::string &supportedChars, const Format &format );

	struct GlyphInfo {
		uint8_t		mTextureIndex;
		Area		mTexCoords;
		Vec2f		mOriginOffset;
	};
	
	std::map<Font::Glyph, GlyphInfo>		mGlyphMap;
	std::vector<gl::Texture>				mTextures;
	Font									mFont;
	Format									mFormat;
};

} } // namespace cinder::gl