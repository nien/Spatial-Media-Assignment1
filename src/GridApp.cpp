//
//  GridApp.cpp
//  Grid
//
//  Created by Nien Lam on 1/29/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace std;


class GridApp : public AppBasic 
{
  public:
	GridApp() : mDrawState( DRAW_STATE_1 ), mMouseLoc( -1, -1 ), mRedSquarePos( 0, 0 )  {}
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings( Settings *settings );
	void keyDown( KeyEvent event ); 
	void mouseMove( MouseEvent event ) { mMouseLoc = event.getPos(); } 
  private:
	// Constants for application.
	static const int SQUARE_SIZE  = 10;
	static const int GRID_SPACING = 20;
	static const int GRID_WIDTH   = 20;
	static const int GRID_HEIGHT  = 20;
	
	// Enums and string display for draw states.
	enum DrawState { DRAW_STATE_1 = 0, DRAW_STATE_2 = 1, DRAW_STATE_3 = 2 };
	static const string DRAW_STATE_TEXT[];
	
	// Remember draw state.
	DrawState mDrawState;
	
	// Track red square position on grid.
	Vec2i mRedSquarePos;
	
	// Track mouse location on mouseMove.
	Vec2i mMouseLoc;
	
	// Draws a (width x height) grid of white squares.
	void drawGrid1( int width, int height );

	// Draws a (width x height) grid of white squares with red square at redSquarePos.
	void drawGrid2( int width, int height, Vec2i redSquarePos );

	// Draws a (width x height) grid of red and white squares in a checker pattern.
	void drawGrid3( int width, int height );

	// Draws text display red square position.
	void drawText();
};

const string GridApp::DRAW_STATE_TEXT[] = { "Draw Grid 1", "Draw Grid 2", "Draw Grid 3" };


void GridApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 800, 600 );
    settings->setFrameRate( 60.0f );
}

void GridApp::setup()
{

}

void GridApp::mouseDown( MouseEvent event )
{
}

void GridApp::update()
{
}

void GridApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::enableAlphaBlending( false );

	switch(mDrawState) 
	{
		case DRAW_STATE_1:
			drawGrid1( GRID_WIDTH, GRID_HEIGHT );
			break;

		case DRAW_STATE_2:
			drawGrid2( GRID_WIDTH, GRID_HEIGHT, mRedSquarePos );
			break;
		
		case DRAW_STATE_3:
			drawGrid3( GRID_WIDTH, GRID_HEIGHT );
			break;
	
		default:
			console() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ":" << "Unsupported Draw State" << endl;
			exit( 1 );
	}

	drawText();
}

void GridApp::drawText()
{
	gl::color( Color( 1.0f, 1.0f, 1.0f ) );
	
	TextLayout textLayout;
	textLayout.clear( ColorA( 0.0f, 0.0f, 0.0f, 0.0f ) );
	textLayout.setFont( Font( "Arial Black", 20 ) );
	textLayout.setColor( ColorA(0.0f, 0.95f, 0.5f, 1.0f ) );
	
	textLayout.addLine( DRAW_STATE_TEXT[mDrawState] );
	if ( mDrawState == DRAW_STATE_2 ) 
	{
		stringstream str;
		str << "Red Square " << mRedSquarePos.x << ":" << mRedSquarePos.y;
		textLayout.addLine( str.str() );
	}
	
	gl::Texture texture = gl::Texture( textLayout.render( true, false ) );
	texture.enableAndBind();
	gl::draw( texture, Vec2f( getWindowWidth() - 200, 10 ) );
	texture.unbind();
}

void GridApp::keyDown( KeyEvent event ) 
{
	// Change draw state based on keyboard input.
    if ( event.getChar() == '1' )
	{
		mDrawState = DRAW_STATE_1;
	}
    else if ( event.getChar() == '2' )
	{	
		mRedSquarePos = Vec2i::zero();
		mDrawState = DRAW_STATE_2;
	}
	else if ( event.getChar() == '3' )
	{
		mDrawState = DRAW_STATE_3;
	}
	
	// Change the red square position when using the arrow keys.
	// Keep position contained within the grid.
    if ( event.getCode() == KeyEvent::KEY_LEFT )
	{
		mRedSquarePos.x = max( 0, mRedSquarePos.x - 1 ); 
	} 
	else if ( event.getCode() == KeyEvent::KEY_RIGHT )
	{
		mRedSquarePos.x = min( GRID_WIDTH - 1, mRedSquarePos.x + 1 ); 
    }
	else if ( event.getCode() == KeyEvent::KEY_UP )
	{
		mRedSquarePos.y = max( 0, mRedSquarePos.y - 1 ); 
	}
	else if ( event.getCode() == KeyEvent::KEY_DOWN )
	{
		mRedSquarePos.y = min( GRID_HEIGHT - 1, mRedSquarePos.y + 1 ); 
	}
}

void GridApp::drawGrid1( int width, int height )
{
	gl::color( Color( 1.0f, 1.0f, 1.0f ) );
	for ( int ii = 0; ii < height; ii++ ) 
	{
		for ( int jj = 0; jj < width; jj++ ) 
		{
			Vec2i location( jj * ( SQUARE_SIZE + GRID_SPACING ), ii * ( SQUARE_SIZE + GRID_SPACING ) );
			Rectf rect( location.x, location.y, location.x + SQUARE_SIZE, location.y + SQUARE_SIZE );
			gl::drawSolidRect( rect );
		}
	}
}

void GridApp::drawGrid2( int width, int height, Vec2i redSquarePos )
{
	for ( int ii = 0; ii < height; ii++ ) 
	{
		for ( int jj = 0; jj < width; jj++ ) 
		{
			Vec2i location( jj * ( SQUARE_SIZE + GRID_SPACING ), ii * ( SQUARE_SIZE + GRID_SPACING ) );

			// Make square red if position equals redSquarePos or if mouse is over square.
			if ( ( redSquarePos.x == jj && redSquarePos.y == ii ) ||
				 ( location.x <= mMouseLoc.x && mMouseLoc.x <= location.x + SQUARE_SIZE &&
				   location.y <= mMouseLoc.y && mMouseLoc.y <= location.y + SQUARE_SIZE ) )
			{
				gl::color( Color( 1.0f, 0.0f, 0.0f ) );
			}
			else
			{
				gl::color( Color( 1.0f, 1.0f, 1.0f ) );
			}
			
			Rectf rect( location.x, location.y, location.x + SQUARE_SIZE, location.y + SQUARE_SIZE );
			gl::drawSolidRect( rect );
		}
	}
}

void GridApp::drawGrid3( int width, int height )
{
	// Toggle flag used to create checker pattern.
	bool toggle = false;
	
	for ( int ii = 0; ii < height; ii++ ) 
	{
		toggle = !toggle;
		for ( int jj = 0; jj < width; jj++ ) 
		{
			toggle = !toggle;
			if ( toggle )	
			{
				gl::color(Color(1.0f, 0.0f, 0.0f));
			}
			else
			{
				gl::color(Color(1.0f, 1.0f, 1.0f));
			}
			
			Vec2i location( jj * ( SQUARE_SIZE + GRID_SPACING ), ii * ( SQUARE_SIZE + GRID_SPACING ) );
			Rectf rect( location.x, location.y, location.x + SQUARE_SIZE, location.y + SQUARE_SIZE );
			gl::drawSolidRect( rect );
		}
	}
}


CINDER_APP_BASIC( GridApp, RendererGl )
