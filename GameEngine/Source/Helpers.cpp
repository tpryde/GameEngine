#include "CommonHeader.h"

void LogError(char* string, ...)
{
    char tempbuffer[1024];

    va_list args;
    va_start( args, string );
    vsprintf_s( tempbuffer, 1024, string, args );
    va_end( args );

    OutputDebugString( tempbuffer );
}

bool CheckForGLErrors(const char* file, int line)
{
    GLenum errorID = GL_NO_ERROR;
    char* errorStr;
    int errorCount = 0;

    while( (errorID = glGetError()) != GL_NO_ERROR )
    {
        errorCount++;

        switch( errorID )
        {
        case GL_INVALID_ENUM:
            errorStr = "GL_INVALID_ENUM";
            break;

        case GL_INVALID_VALUE:
            errorStr = "GL_INVALID_VALUE";
            break;

        case GL_INVALID_OPERATION:
            errorStr = "GL_INVALID_OPERATION";
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;

        case GL_OUT_OF_MEMORY:
            errorStr = "GL_OUT_OF_MEMORY";
            break;

        default:
            errorStr = "Unknown GL message type.";
            break;
        }

        LogError( "GLError -> %d - %s - %d - %s(%d)\n", errorID, errorStr, errorCount, file, line );

        assert( false );
    }

    return( errorCount > 0 ? true : false );
}

char* LoadCompleteFile(const char* filename)
{
    FILE* file;
    errno_t err = fopen_s( &file, filename, "rb" );

    char* buffer;

    if( file )
    {
        fseek( file, 0, SEEK_END );
        long size = ftell( file );
        rewind( file );

        buffer = new char[size+1];
        fread( buffer, size, 1, file );
        buffer[size] = 0;

        fclose( file );

        return buffer;
    }

    return 0;
}

double MyGetSystemTime()
{
    SYSTEMTIME time;
    GetSystemTime( &time );

    return time.wSecond + time.wMilliseconds / 1000.0;
}
