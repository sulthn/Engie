#include "graphics.h"

std::chrono::high_resolution_clock::time_point last;
std::chrono::high_resolution_clock::time_point ab;

ID2D1Factory* pD2DFactory = NULL;

double deltaTime = 0;

uint32_t GetColour(float lum, uint32_t baseColor)
{
	int red = (int)((float)((baseColor >> 16) & 0xFF) * lum) & 0xFF;
	int green = (int)((float)((baseColor >> 8) & 0xFF) * lum) & 0xFF;
	int blue = (int)((float)((baseColor) & 0xFF) * lum) & 0xFF;
	return RGB(red, green, blue);
}

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

ID2D1PathGeometry* GenTriangleGeometry(D2D1_POINT_2F pt1, D2D1_POINT_2F pt2, D2D1_POINT_2F pt3)
{
    ID2D1GeometrySink* pSink = NULL;
    ID2D1PathGeometry* m_pPathGeometry = NULL;
    HRESULT hr = S_OK;

    // Create a path geometry.
    if (SUCCEEDED(hr))
    {
        hr = pD2DFactory->CreatePathGeometry(&m_pPathGeometry);

        if (SUCCEEDED(hr))
        {
            // Write to the path geometry using the geometry sink.
            hr = m_pPathGeometry->Open(&pSink);
            

            if (SUCCEEDED(hr))
            {
                pSink->BeginFigure(
                    pt1,
                    D2D1_FIGURE_BEGIN_FILLED
                );

                pSink->AddLine(pt2);

                pSink->AddLine(pt3);

                pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

                hr = pSink->Close();
            }
            SAFE_RELEASE(pSink);
        }
    }

    return m_pPathGeometry;
}


D2D1_POINT_2F D2PF(int x, int y) 
{
    D2D1_POINT_2F p = { x, y };
    return p;
}

int SCRW = 1024;
int SCRH = 720;

float fTheta;
mesh meshCube;
mat4x4 matProj;
vec3d vCamera;
vec3d vLookDir;

float fYaw;

// INITIALIZE

void initialize() {
    meshCube.LoadFromObjectFile("teapot.obj");
    meshCube.color = RGB(255, 255, 255);

    matProj = Matrix_MakeProjection(70.0f, (float)SCRH / (float)SCRW, 0.1f, 1000.0f);
    //vCamera.y += 16.0f;
}


// RENDER

void render(ID2D1HwndRenderTarget* pRT, uint32_t d)
{

    // Set up rotation matrices
    mat4x4 matRotZ, matRotX;
    //fTheta += 1.0f * deltaTime();
    fTheta = 0.0f;
    
    matRotZ = Matrix_MakeRotationZ(fTheta);
    matRotX = Matrix_MakeRotationX(fTheta * 0.5f);

    mat4x4 matTrans;
    matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 8.0f);

    mat4x4 matWorld;
    matWorld = Matrix_MakeIdentity();
    matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX);
    matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);

    vec3d vUp = { 0, 1, 0 };
    vec3d vTarget = { 0, 0, 1 };
    mat4x4 matCameraRot = Matrix_MakeRotationY(fYaw);
    vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
    vTarget = Vector_Add(vCamera, vLookDir);
    mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

    mat4x4 matView = Matrix_QuickInverse(matCamera);

    // Store triagles for rastering later
    vector<triangle> vecTrianglesToRaster;

    // Draw Triangles
    for (auto tri : meshCube.tris)
    {
        triangle triProjected, triTransformed, triViewed;

        triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
        triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
        triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);

        // Use Cross-Product to get surface normal
        vec3d normal, line1, line2;

        line1 = Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
        line2 = Vector_Sub(triTransformed.p[2], triTransformed.p[0]);

        normal = Vector_CrossProduct(line1, line2);
        normal = Vector_Normalise(normal);

        vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera);

        //if (normal.z < 0)
        if (Vector_DotProduct(normal, vCameraRay) < 0.0f)
        {
            // Illumination
            vec3d light_direction = { 0.0f, 1.0f, -1.0f };
            light_direction = Vector_Normalise(light_direction);

            // How similar is normal to light direction
            float dp = max(0.1f, Vector_DotProduct(light_direction, normal));

            // Choose console colours as required (much easier with RGB)
            uint32_t c = GetColour(dp, meshCube.color);
            //c = d;
            triTransformed.col = c;


            // Convert World Space --> View Space
            triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
            triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
            triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
            triViewed.col = triTransformed.col;

            int nClippedTriangles = 0;
            triangle clipped[2];
            nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

            for (int n = 0; n < nClippedTriangles; n++)
            {
                // Project triangles from 3D --> 2D
                triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
                triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
                triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);
                triProjected.col = clipped[n].col;

                triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
                triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
                triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

                // X/Y are inverted so put them back
                triProjected.p[0].x *= -1.0f;
                triProjected.p[1].x *= -1.0f;
                triProjected.p[2].x *= -1.0f;
                triProjected.p[0].y *= -1.0f;
                triProjected.p[1].y *= -1.0f;
                triProjected.p[2].y *= -1.0f;

                // Scale into view
                vec3d vOffsetView = { 1, 1, 0 };
                triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
                triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
                triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);
                triProjected.p[0].x *= 0.5f * (float)SCRW;
                triProjected.p[0].y *= 0.5f * (float)SCRH;
                triProjected.p[1].x *= 0.5f * (float)SCRW;
                triProjected.p[1].y *= 0.5f * (float)SCRH;
                triProjected.p[2].x *= 0.5f * (float)SCRW;
                triProjected.p[2].y *= 0.5f * (float)SCRH;

                // Store triangle for sorting
                vecTrianglesToRaster.push_back(triProjected);
            }
        }

    }

    // Sort triangles from back to front
    sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
        {
            float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
            float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
            return z1 > z2;
        });
    

    pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    ID2D1PathGeometry* geometry = NULL;
    for (auto& triToRaster : vecTrianglesToRaster)
    {
        triangle clipped[2];
        list<triangle> listTriangles;

        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;

        for (int p = 0; p < 4; p++)
        {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0)
            {
                triangle test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                switch (p)
                {
                case 0:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)SCRH - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ (float)SCRW - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                }

                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }

        for (auto &t : listTriangles)
        {
            D2D1_POINT_2F p0 = D2PF(t.p[0].x, t.p[0].y);
            D2D1_POINT_2F p1 = D2PF(t.p[1].x, t.p[1].y);
            D2D1_POINT_2F p2 = D2PF(t.p[2].x, t.p[2].y);

            geometry = GenTriangleGeometry(p0, p1, p2);

            if (!geometry) printf("pussy");
            printf("pussy");

            ID2D1SolidColorBrush* pBrush = NULL;
            pRT->CreateSolidColorBrush(
                D2D1::ColorF(t.col),
                &pBrush
            );

            ID2D1SolidColorBrush* pbBrush = NULL;
            pRT->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &pbBrush
            );

            if (pBrush && geometry)
            {
                pRT->FillGeometry(geometry, pBrush);
                if (pbBrush) pRT->DrawGeometry(geometry, pbBrush);
            }

            SAFE_RELEASE(geometry);
            SAFE_RELEASE(pBrush);
            SAFE_RELEASE(pbBrush);
        }
    }
}

// RESIZE
void resize(float w, float h)
{
    matProj = Matrix_MakeProjection(70.0f, h / w, 0.1f, 1000.0f);
}
