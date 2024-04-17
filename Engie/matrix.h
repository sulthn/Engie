#pragma once

#include <Windows.h>

#include <math.h>
#include <vector>
#include <fstream>
#include <strstream>

using std::vector;
using std::string;
using std::ifstream;
using std::strstream;

struct vec3d
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

struct mat4x4
{
	float m[4][4] = { 0 };
};

struct triangle
{
	vec3d p[3];
	uint32_t col = 0;
};

struct mesh
{
	vector<triangle> tris;

	uint32_t color = 0;

	bool LoadFromObjectFile(string sFilename)
	{
		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		vector<vec3d> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}

		return true;
	}

};

vec3d Matrix_MultiplyVector(mat4x4& m, vec3d& i);
mat4x4 Matrix_MakeIdentity();
mat4x4 Matrix_MakeRotationX(float fAngleRad);
mat4x4 Matrix_MakeRotationY(float fAngleRad);
mat4x4 Matrix_MakeRotationZ(float fAngleRad);
mat4x4 Matrix_MakeTranslation(float x, float y, float z);
mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2);
mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up);
mat4x4 Matrix_QuickInverse(mat4x4& m);

vec3d Vector_Add(vec3d& v1, vec3d& v2);
vec3d Vector_Sub(vec3d& v1, vec3d& v2);
vec3d Vector_Mul(vec3d& v1, float k);
vec3d Vector_Div(vec3d& v1, float k);
float Vector_DotProduct(vec3d& v1, vec3d& v2);
float Vector_Length(vec3d& v);
vec3d Vector_Normalise(vec3d& v);
vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2);
vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd);

int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2);

