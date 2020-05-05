#include "TCoordinates4.h"

using namespace cagd;
using namespace std;

// default constructor
inline TCoordinate4::TCoordinate4()
{
    _data[0] = _data[1] = _data[2] = 0.0;
    _data[3] = 1.0;
}

// homework: special constructor
TCoordinate4::TCoordinate4(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
    _data[0] = s;
    _data[1] = t;
    _data[2] = r;
    _data[3] = q;
}

// homework: get components by value
GLfloat TCoordinate4::operator[](GLuint rhs) const
{
    return _data[rhs];
}

GLfloat TCoordinate4::s() const
{
    return _data[0];
}

GLfloat TCoordinate4::t() const
{
    return _data[1];
}

GLfloat TCoordinate4::r() const
{
    return _data[2];
}

GLfloat TCoordinate4::q() const
{
    return _data[3];
}

// homework: get components by reference
GLfloat& TCoordinate4::operator[](GLuint rhs)
{
    return _data[rhs];
}

GLfloat& TCoordinate4::s()
{
    return _data[0];
}

GLfloat& TCoordinate4::t()
{
    return _data[1];
}

GLfloat& TCoordinate4::r()
{
    return _data[2];
}

GLfloat& TCoordinate4::q()
{
    return _data[3];
}
