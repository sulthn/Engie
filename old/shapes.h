#pragma once

#include "matrix.h"

#include <vector>
#include <fstream>
#include <strstream>

using std::vector;
using std::string;
using std::ifstream;
using std::strstream;

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
