#include "CommonHeader.h"

GameCore* g_pGameCore = 0;

GameCore::GameCore()
{
    g_pGameCore = this;

    m_VBO = 0;
    m_IBO = 0;
	m_TBO = 0;

    m_pShader = 0;

	m_Radius = 0;
	m_RadiusAlpha = 0;
	m_CenterPoint[0] = 300;
	m_CenterPoint[1] = 300;

	m_BackColour[0] = 0;
	m_BackColour[1] = 0;
	m_BackColour[2] = 0;
	m_BackColour[3] = 0;

	m_Time = 0.0f;
}	
GameCore::~GameCore()
{
    // delete the allocated buffers.
    glDeleteBuffers( 1, &m_VBO );
    glDeleteBuffers( 1, &m_IBO );
	glDeleteBuffers( 1, &m_TBO );
    CheckForGLErrors();

    // delete the shader.
    delete m_pShader;
}

void GameCore::OneTimeInit()
{
    GenerateBuffers();

    // Create a shader
    m_pShader = new ShaderProgram;
    m_pShader->Init( "Data/Shaders/simple.vert", "Data/Shaders/simple.frag" );

    // Load a texture
	unsigned char* pngbuffer;
	unsigned int width, height;
	unsigned int error = -1;
	error = lodepng_decode32_file( &pngbuffer, &width, &height, "Data/Health_Alert.png" );
	assert(error = -1);

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_TBO );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngbuffer );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}

void GameCore::Tick(double TimePassed)
{
	m_Time += TimePassed;

	m_Radius = (cos(m_Time)*cos(m_Time)) * 800;
	m_RadiusAlpha = m_Radius * 0.5;
}

void GameCore::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    // draw to the entire window
    glViewport( 0, 0, width, height );
    CheckForGLErrors();
}

void GameCore::GenerateBuffers()
{
    // ATM this can only be called once, so we assert if it's called again with a VBO already allocated
    assert( m_VBO == 0 );

    // vertex position info for a triangle
    VertexFormat* vertexattributes = new VertexFormat[4];
    vertexattributes[0]  = VertexFormat( -1.0f,  1.0f,    0.0f,0.0f,0.0f,1.0f,   0.0f,0.0f ); // top left
	vertexattributes[1]  = VertexFormat(  1.0f,  1.0f,    0.0f,0.0f,0.0f,1.0f,   1.0f,0.0f ); // top right
    vertexattributes[2]  = VertexFormat( -1.0f, -1.0f,    0.0f,0.0f,0.0f,1.0f,   0.0f,1.0f ); // bottom left
    vertexattributes[3]  = VertexFormat(  1.0f, -1.0f,    0.0f,0.0f,0.0f,1.0f,   1.0f,1.0f ); // bottom right

	//vertexattributes[4]  = VertexFormat(  0.0f, 0.0f,    0.0f,0.0f,0.0f,1.0f,   0.5f,0.5f ); // center point 

    GLushort vertexindices[] = { 0,1,2, 2,1,3 };

    // gen and fill buffer with our attributes
    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat)*4, vertexattributes, GL_STATIC_DRAW );

    // gen and fill buffer with our attributes
    glGenBuffers( 1, &m_IBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*6, vertexindices, GL_STATIC_DRAW );

    // check for errors
    CheckForGLErrors();
}

void GameCore::OnDrawFrame()
{
    // check for errors before we start drawing
    CheckForGLErrors();

    // clear the screen to darkish blue
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    // use our shader.
    glUseProgram( m_pShader->m_Program );

    // draw the triangle
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );

        GLint loc = glGetAttribLocation( m_pShader->m_Program, "a_Position" );
        if( loc != -1 )
        {
            glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,position) );
            glEnableVertexAttribArray( loc );
        }

		GLint color = glGetAttribLocation( m_pShader->m_Program, "a_Color" );
        if( color != -1 )
        {
            glVertexAttribPointer( color, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,colour) );
            glEnableVertexAttribArray( color );
        }

		GLint uv = glGetAttribLocation( m_pShader->m_Program, "a_UV" );
        if( uv != -1 )
        {
            glVertexAttribPointer( uv, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,uv) );
            glEnableVertexAttribArray( uv );
        }

		GLint background = glGetAttribLocation( m_pShader->m_Program, "a_Texture" );
        if( background != -1 )
        {
            glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_TBO);
			glUniform1i(background, 0);
        }

		GLint centerPoint = glGetUniformLocation( m_pShader->m_Program, "u_Center" );
		if(centerPoint != -1)
		{
			glUniform2f( centerPoint, m_CenterPoint[0], m_CenterPoint[1]);
		}

		GLint radius = glGetUniformLocation( m_pShader->m_Program, "u_Radius" );
		if(radius != -1)
		{
			glUniform1f(radius, m_Radius);
		}

		GLint radiusAlpha = glGetUniformLocation( m_pShader->m_Program, "u_RadiusAlpha" );
		if(radiusAlpha != -1)
		{
			glUniform1f(radiusAlpha, m_RadiusAlpha);
		}

		GLint backColour = glGetUniformLocation( m_pShader->m_Program, "u_BackColor" );
		if(backColour != -1)
		{
			glUniform4f( backColour, m_BackColour[0], m_BackColour[1], m_BackColour[2], m_BackColour[3]);
		}

        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0 );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    // check for errors again when we're done drawing
    CheckForGLErrors();
}
