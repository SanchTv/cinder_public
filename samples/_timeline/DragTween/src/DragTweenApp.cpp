#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Easing.h"
#include "cinder/Timeline.h"
#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;

class Circle {
  public:
	Circle( Color color, float radius, Vec2f initialPos, Vec2f homePos )
		: mColor( color ), mRadius( radius ), mPos( initialPos ), mHomePos( homePos )
	{}
	
	void draw() const {
		gl::color( ColorA( mColor, 0.75f ) );
		gl::drawSolidCircle( mPos, mRadius );
	}

	void startDrag() {
		//if( mDragTween ) // if we're heading somewhere, stop going there and start listening to the drag
		//	mDragTween->removeSelf();
		// mDragTween.stop();
		mPos.stop();
	}
	
	void dragRelease() {
		// tween back home
		// mDragTween = app::getTimeline().apply( &mPos, mHomePos, 1.0f, EaseOutBack( 3 ) );
		// return to our home position in 1sec, easing using EaseOutBack
		app::timeline().apply( &mPos, mHomePos, 1.0f, EaseOutBack( 3 ) );
	}
	
	Color				mColor;
	Vec2f				mHomePos;
	Anim<Vec2f>			mPos;
	Anim<float>			mRadius;
//	Anim<Vec2f>			mDragTween;
};

class DragTweenApp : public AppNative {
  public:
    void prepareSettings( Settings *settings ) { settings->enableMultiTouch( false ); }
	void setup();
	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void mouseUp( MouseEvent event );
	void keyDown( KeyEvent event );
	void draw();
	
	vector<Circle>			mCircles;
	Circle					*mCurrentDragCircle;
};

void DragTweenApp::setup()
{
/*	TimelineRef dummy = Timeline::create();
	Anim<float> test1( 3 );
	dummy->apply( &test1, 2.0f, 0.5f );*/

	// setup the initial animation
	const size_t numCircles = 35;
	for( size_t c = 0; c < numCircles; ++c ) {
		float angle = c / (float)numCircles * 4 * M_PI;
		Vec2f pos = getWindowCenter() + ( 50 + c / (float)numCircles * 200 ) * Vec2f( cos( angle ), sin( angle ) );
		mCircles.push_back( Circle( Color( CM_HSV, c / (float)numCircles, 1, 1 ), 0, getWindowCenter(), pos ) );
		//getTimeline().append( &mCircles.back().mPos, pos, 0.5f, EaseOutAtan( 10 ) )->delay( -0.45f );
		timeline().appendBack( &mCircles.back().mPos, pos, 0.5f, EaseOutAtan( 10 ) )->delay( -0.45f );
		//getTimeline().append( &mCircles.back().mRadius, 30.0f, 0.5f, EaseOutAtan( 10 ) )->delay( -0.5f );
		timeline().appendBack( &mCircles.back().mRadius, 30.0f, 0.5f, EaseOutAtan( 10 ) )->delay( -0.5f );
	}
	
	mCurrentDragCircle = 0;
}

void DragTweenApp::mouseDown( MouseEvent event )
{
	// see if the mouse is in any of the circles
	vector<Circle>::iterator circleIt = mCircles.end();
	for( circleIt = mCircles.begin(); circleIt != mCircles.end(); ++circleIt )
		if( circleIt->mPos.value().distance( event.getPos() ) <= circleIt->mRadius )
			break;

	// if we hit one, tell it to startDrag()
	if( circleIt != mCircles.end() ) {
		mCurrentDragCircle = &(*circleIt);
		mCurrentDragCircle->startDrag();
	}
}

void DragTweenApp::mouseDrag( MouseEvent event )
{
	// if we're dragging a circle, set its position to be where the mouse is
	if( mCurrentDragCircle )
		mCurrentDragCircle->mPos = event.getPos();
}

void DragTweenApp::mouseUp( MouseEvent event )
{
	// if we were dragging a circle, tell it we're done
	if( mCurrentDragCircle )
		mCurrentDragCircle->dragRelease();
		
	mCurrentDragCircle = 0;
}

void DragTweenApp::keyDown( KeyEvent event )
{
	for( size_t c = 0; c < mCircles.size(); ++c ) {
		float angle = c / (float)mCircles.size() * 4 * M_PI;
		Vec2f pos = getWindowCenter() + ( 50 + c / (float)mCircles.size() * 200 ) * Vec2f( cos( angle ), sin( angle ) );
		//getTimeline().append( &mCircles.back().mPos, pos, 0.5f, EaseOutAtan( 10 ) )->delay( -0.45f );
		timeline().apply( &mCircles.back().mPos, pos, 0.5f, EaseOutAtan( 10 ) )->delay( -0.45f );
		//getTimeline().append( &mCircles.back().mRadius, 30.0f, 0.5f, EaseOutAtan( 10 ) )->delay( -0.5f );
		timeline().apply( &mCircles.back().mRadius, 30.0f, 0.5f, EaseOutAtan( 10 ) )->delay( -0.5f );
	}	
}

void DragTweenApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.8f, 0.8f, 0.8f ) );
	gl::enableAlphaBlending();
	
	for( vector<Circle>::const_iterator circleIt = mCircles.begin(); circleIt != mCircles.end(); ++circleIt )
		circleIt->draw();
}


CINDER_APP_NATIVE( DragTweenApp, RendererGl )
