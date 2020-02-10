#ifndef __GameCore_H__
#define __GameCore_H__

class GameCore;

extern GameCore* g_pGameCore;

struct VertexFormat
{
    float position[2];
    float colour[4];
	float uv[2];

    VertexFormat() {}
    VertexFormat(float x, float y, float r, float g, float b, float a, float u, float v)
    {
        position[0] = x; position[1] = y;
        colour[0] = r; colour[1] = g; colour[2] = b; colour[3] = a;
		uv[0] = u; uv[1] = v;
    }    
};

class GameCore
{
public:
    GLuint m_VBO;
    GLuint m_IBO;
	GLuint m_TBO;

	float m_Time;

    ShaderProgram* m_pShader;

public:
    GameCore();
    virtual ~GameCore();

    virtual void OneTimeInit();
    virtual void Tick(double TimePassed);

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

    void GenerateBuffers();

    virtual void OnDrawFrame();

private:
	float m_Radius;
	float m_RadiusAlpha;
	float m_CenterPoint[2];

	float m_BackColour[4];
};

#endif //__GameCore_H__
