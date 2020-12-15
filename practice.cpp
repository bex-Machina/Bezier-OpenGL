#include <Windows.h>  // for access to the windows APIs
#include <crtdbg.h>   // for debug stuff
#include "GLSetup.h"
#include "Utils.h"
#include "Matrix.h"
#include "RenderingContext.h"
#include "time.h"
#include <vector>
#include <math.h>
//#include "SavedPoints.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600



static HWND hwnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void OnCreate();
void OnDraw();
void OnSize(DWORD type, UINT cx, UINT cy);
void OnTimer(UINT nIDEvent);
void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
void OnLButtonDown(UINT nFlags, int x, int y);
void OnRButtonDown(UINT nFlags, int x, int y);
void OnLButtonUp(UINT nFlags, int x, int y);
void OnMouseMove(UINT nFlags, int x, int y);
void pushLineVector();






// try using a vector insted.




int CurveHittest(int x, int y);
int pointhit(int x, int y);

int frontpointHitTest(int x, int y);
int backpointtHitTest(int x, int y);

void drawPointss();


void createObjects(int numOfSegnebts);

void drawPoints();
void c1Assert();
void drawCurve();
void drawFrontAndBack();
//int CurveHittest(int x, int y);
void cleanUp();



struct savedPoint {
    savedPoint(float x, float y) : x(x), y(y){}
    float x, y;
};





struct savedLinesSegments {
    savedLinesSegments(float x, float y):x(x), y(y) {}
    float x, y;
};

std::vector<savedPoint*>* Vpoint(new std::vector<savedPoint*>());
std::vector<savedLinesSegments*>* Lpoint(new std::vector<savedLinesSegments*>());


inline savedPoint* linearBezier(const savedPoint* p0, const savedPoint* p1, float t)
{
    float x = p0->x * (1 - t) + p1->x * t;
    float y = p0->y * (1 - t) + p1->y * t;
    //return new savedPoint(p0->x * (1 - t) + p1->x * t, p0->y * (1 - t) + p1->y * t);
    return new savedPoint(x, y);
}

inline savedLinesSegments* LinelinearBezier(const savedPoint* p0, const savedPoint* p1, float t)
{
    float x = p0->x * (1 - t) + p1->x * t;
    float y = p0->y * (1 - t) + p1->y * t;
    //return new savedPoint(p0->x * (1 - t) + p1->x * t, p0->y * (1 - t) + p1->y * t);
    return new savedLinesSegments(x, y);
}

inline savedLinesSegments* cubicBezier(savedLinesSegments* p0, savedLinesSegments* p1, savedLinesSegments* p2, savedLinesSegments* p3, float t)
{
    const double temp = (1.0 - t);
    float x = p0->x * pow(temp, 3) + p1->x * 3 * pow(temp, 2) * t + p2->x * 3 * temp * pow(t, 2) + p3->x * pow(t, 3);
    float y = p0->y * pow(temp, 3) + p1->y * 3 * pow(temp, 2) * t + p2->y * 3 * temp * pow(t, 2) + p3->y * pow(t, 3);
    //return new LineSegmentPoint(p0->x * pow(temp, 3) + p1->x * 3 * pow(temp, 2) * t + p2->x * 3 * temp * pow(t, 2) + p3->x * pow(t, 3), p0->y * pow(temp, 3) + p1->y * 3 * pow(temp, 2) * t + p2->y * 3 * temp * pow(t, 2) + p3->y * pow(t, 3));
    return new savedLinesSegments(x, y);
}


void addPoint(savedPoint* point);
bool pointHitTestP(const savedPoint* point, UINT x, UINT y);
bool pointHitTestL(const savedLinesSegments* point, UINT x, UINT y);

savedPoint* selected;
unsigned int VBOS[2];
RenderingContext rcontext;







// Win32 entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // This mini section is really useful to find memory leaks
#ifdef _DEBUG   // only include this section of code in the DEBUG build
//  _CrtSetBreakAlloc(65);  // really useful line of code to help find memory leaks
    _onexit(_CrtDumpMemoryLeaks); // check for memory leaks when the program exits
#endif
    srand(time(NULL));
    // To create a window, we must first define various attributes about it
    WNDCLASSEX classname;
    classname.cbSize = sizeof(WNDCLASSEX);
    classname.style = CS_HREDRAW | CS_VREDRAW;
    classname.lpfnWndProc = (WNDPROC)WndProc;   // This is the name of the event-based callback method
    classname.cbClsExtra = 0;
    classname.cbWndExtra = 0;
    classname.hInstance = hInstance;
    classname.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    classname.hCursor = LoadCursor(NULL, IDC_ARROW);
    classname.hbrBackground = ::GetSysColorBrush(COLOR_3DFACE);
    classname.lpszMenuName = NULL;
    classname.lpszClassName = L"GettingStarted";
    classname.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    RegisterClassEx(&classname);

    // Default size will be 800x600 and we'll centre it on the screen - this include the caption and window borders so might not be the canvas size (which will be smaller)
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;
    int offx = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int offy = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;

    // Create the window using the definition provided above
    CreateWindowEx(NULL, L"GettingStarted", L"Getting Started with Win32", WS_OVERLAPPEDWINDOW | WS_VISIBLE, offx, offy, width, height, NULL, NULL, hInstance, NULL);

    // Set the event-based message system up
    MSG msg;
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
    while (msg.message != WM_QUIT)  // keep looping until we get the quit message
    {
        if (GetMessage(&msg, NULL, 0, 0)) // cause this thread to wait until there is a message to process
        {
            // These two lines of code take the MSG structure, mess with it, and correctly dispatch it to the WndProc defined during the window creation
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

}

void cleanUp() 
{
    glDeleteBuffers(2, VBOS);
    glDeleteProgram(rcontext.glprogram);

    for (int i(0); i < Vpoint->size(); i++) {
        delete (*Vpoint)[i];
    }

    for (int i(0); i < Lpoint->size(); i++) {
        delete (*Lpoint)[i];
    }

    delete Vpoint;
    delete Lpoint;
}


// This is our message handling method and is called by the system (via the above while-loop) when we have a message to process
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        hwnd = hWnd;
        OnCreate();
        return 0;
        break;
    case WM_SIZE:
        OnSize((DWORD)wParam, (UINT)(lParam & 0xFFFF), (UINT)(lParam >> 16));
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT paint;
        BeginPaint(hwnd, &paint);
        OnDraw();
        EndPaint(hwnd, &paint);
    }
    break;
    case WM_TIMER:
        OnDraw();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        PostQuitMessage(0);
        cleanUp();
        break;
    case WM_LBUTTONDOWN:
        OnLButtonDown((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF));
        break;
    case WM_RBUTTONDOWN:
        OnRButtonDown((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF));
        break;
    case WM_LBUTTONUP:
        OnLButtonUp((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF));
        break;
    case WM_MOUSEMOVE:
        OnMouseMove((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF));
        break;
    case WM_KEYDOWN:
        OnKeyDown(wParam, lParam & 0xFFFF, lParam >> 16);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// This is called then the window is first created and useful to get things ready (e.g. load or create pens, brushes, images, etc)
void OnCreate()
{
    initGL(hwnd);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        displayMessage((char*)glewGetErrorString(err));

    rcontext.glprogram = loadShaders(L"vertshader.txt", L"fragshader.txt");

    rcontext.poshandle = glGetAttribLocation(rcontext.glprogram, "a_position");
    //colhandle = glGetAttribLocation(glprogram, "a_colour");
    rcontext.diffusehandle = glGetUniformLocation(rcontext.glprogram, "u_m_diffuse");
    rcontext.mvphandle = glGetUniformLocation(rcontext.glprogram, "u_mvpmatrix");

    glUseProgram(rcontext.glprogram);

    createObjects(50);


    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}






// This is called when the window needs to be redrawn
void OnDraw()
{

    glUseProgram(rcontext.glprogram);

    rcontext.initModelMatrix(true);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(rcontext.poshandle);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[1]);
    glVertexAttribPointer(rcontext.poshandle, 3, GL_DOUBLE, false, 0, (void*)0);


   /* c1Assert();
    drawPoints();
    drawFrontAndBack();
    drawCurve();*/
    


    pushLineVector();
    c1Assert();
    //c1Assert();
    drawPoints();
    drawPointss();
    drawCurve();
    


    glFinish();
    SwapBuffers(wglGetCurrentDC());
}

void drawPoints()
{
    for (int i = 0; i < Vpoint->size(); i++)
    {
        rcontext.pushModelMatrix();
        rcontext.translate((*Vpoint)[i]->x, (*Vpoint)[i]->y, 0.0f);
        rcontext.scale(20.0f, 20.0f, 1.0f);
        rcontext.updateMVPs();

        glUniformMatrix4fv(rcontext.mvphandle, 1, false, rcontext.mvpmatrix);
        glUniform4f(rcontext.diffusehandle, 1.0f, 0.0f, 0.0f, 1.0f);

        glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);

        rcontext.popModelMatrix();
    }
}

void drawPointss()
{
    for (int i = 0; i < Lpoint->size(); i++)
    {
        if (i % 3 == 1 || i % 3 == 2)
        {
            rcontext.pushModelMatrix();
            rcontext.translate((*Lpoint)[i]->x, (*Lpoint)[i]->y, 0.0f);
            rcontext.scale(20.0f, 20.0f, 1.0f);
            rcontext.updateMVPs();

            glUniformMatrix4fv(rcontext.mvphandle, 1, false, rcontext.mvpmatrix);
            glUniform4f(rcontext.diffusehandle, 1.0f, 1.0f, 0.0f, 1.0f);

            glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);

            rcontext.popModelMatrix();
        }
        
    }
}




void drawCurve()
{
    if (Vpoint->size() >= 2) {
        //pushLineVector();

        for (int i(3); i < Lpoint->size(); i += 3)
        {

            for (int j(0); j < 1000; j++) {
                rcontext.pushModelMatrix();

                savedLinesSegments* temp = cubicBezier((*Lpoint)[i - 3], (*Lpoint)[i - 2], (*Lpoint)[i - 1], (*Lpoint)[i], j / 1000.0f);
                rcontext.translate(temp->x, temp->y, 0.0f);
                delete temp;

                rcontext.scale(10.0f, 10.0f, 0.0f);
                rcontext.updateMVPs();

                glUniformMatrix4fv(rcontext.mvphandle, 1, false, rcontext.mvpmatrix);
                glUniform4f(rcontext.diffusehandle, 0.0f, 0.0f, 1.0f, 1.0f);

                glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);
                rcontext.popModelMatrix();
            }
        }

    }
}

void drawFrontAndBack()
{

  /*  for (int i(0); i < Vpoint->size(); i++)
    {
        if ((*Vpoint)[i]->frontpoint != nullptr)
        {
       
        }

        
    }*/

    for (int i(0); i < Vpoint->size(); i++)
    {

    }
    
}



// Called when the window is resized
void OnSize(DWORD type, UINT cx, UINT cy)
{
    if (cx > 0 && cy > 0)
    {
        glViewport(0, 0, cx, cy);
        Matrix::setOrtho(rcontext.projectionmatrix, 0, cx, 0, cy, 0, 100);
        //Matrix::setOrtho(rcontext.projectionmatrix, 0, cx, 0, cy, 0, 200);
    }

}




void createObjects(int noOfSegments)
{
    int noOfverts = noOfSegments + 1;

    float radius = 0.5;
    float theta = 360 / noOfSegments;

    double* VERTICES = (double*)malloc(sizeof(double) * noOfverts * 3);

    if (VERTICES) {

        size_t i = 1;
        for (float coordinates = 0;
            i < noOfverts && coordinates < 360.0f;
            i++, coordinates += theta)
        {
            const float coordinatesToRadians = Matrix::degreesToRadians(coordinates);
            VERTICES[i * 3] = (double)(radius * sin(coordinatesToRadians));
            VERTICES[i * 3 + 1] = (double)(radius * cos(coordinatesToRadians));
            VERTICES[i * 3 + 2] = 0;
        }
        VERTICES[0] = VERTICES[1] = VERTICES[2] = 0;
    }

    short* INDICIES = (short*)malloc(sizeof(short) * noOfSegments * 3);
    if (INDICIES) {
        int i = 0;
        while (i < noOfSegments)
        {
            INDICIES[i * 3] = 0;
            INDICIES[i * 3 + 2] = i + 1;
            INDICIES[i * 3 + 1] = i + 2;
            i++;
        }
        INDICIES[(i - 1) * 3 + 1] = 1;
    }

    glGenBuffers(2, (unsigned int*)VBOS);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * noOfverts * 3, VERTICES, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * noOfSegments * 3, INDICIES, GL_STATIC_DRAW);

    free(VERTICES);
    free(INDICIES);
}




// a key has been pressed while this window has focus
void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

// when the left mouse button is pressed down
void OnRButtonDown(UINT nFlags, int x, int y)
{
    RECT rect;
    ::GetClientRect(hwnd, &rect);
    y = rect.bottom - y;

    int n = pointhit(x, y);
    if (n != -1)
    {

        delete(*Vpoint)[n];
        Vpoint->erase(Vpoint->begin() + n);
        
        
    }
    OnDraw();
}

// when the left mouse button is pressed down
//try using maloc
void OnLButtonDown(UINT nFlags, int x, int y)
{

    RECT rect;
    ::GetClientRect(hwnd, &rect);
    y = rect.bottom - y;

    savedPoint* point = (savedPoint*)malloc(sizeof(savedPoint));
    point->x = x;
    point->y = y;
    if (Vpoint->empty())
    {
        Vpoint->push_back(new savedPoint(point->x, point->y));
    }
    else if (pointhit(x, y) == -1)
    {
        
        
        int pos = CurveHittest(x, y);
        if (pos == -1)
        {
            addPoint(point);
            
        }
        else
        {
            savedPoint* temp = new savedPoint(point->x, point->y);
            Vpoint->insert(Vpoint->begin() + (pos) / 3, temp);
        }
        //addPoint(point);
        //savedPoint* temp = new savedPoint(point->x, point->y);
        ////float n = Vpoint->begin() + (pos) / 3;
        //Vpoint->insert(Vpoint->begin() + (pos) / 3, temp);
        
    }
    else
    {
        selected = (*Vpoint)[pointhit(x, y)];
    }
    free(point);
    OnDraw();

}


// when the left mouse button is released
void OnLButtonUp(UINT nFlags, int x, int y)
{
    selected = nullptr;
}

// when the mouse moves across the window.  use nFlags to determine if button is down or not plus other stuff
// select the sidepoints
void OnMouseMove(UINT nFlags, int x, int y)
{

    if (selected) {
        RECT rect;
        ::GetClientRect(hwnd, &rect);
        y = rect.bottom - y;
        selected->x = x;
        selected->y = y;
        OnDraw();
    }
    
}

// called when a timer event fires - use nIDEvent to figure out which timer event it was
void OnTimer(UINT nIDEvent)
{

}


bool pointHitTestP(const savedPoint* point, UINT x, UINT y) {
    int dx = point->x - x;
    int dy = point->y - y;

    const int radius = 16;
    return (dx * dx + dy * dy) < (radius * radius);
}

bool pointHitTestL(const savedLinesSegments* point, UINT x, UINT y) {
    int dx = point->x - x;
    int dy = point->y - y;

    const int radius = 16;
    return (dx * dx + dy * dy) < (radius * radius);
}



int pointhit(int x, int y)
{
    for (int i(0); i < Vpoint->size(); i++)
    {
        if (pointHitTestP((*Vpoint)[i], x, y))
        {
            return i;
        }
    }
    return -1;
}

int frontpointHitTest(int x, int y)
{
    return 0;
}

int backpointtHitTest(int x, int y)
{
    return 0;
}


// MAKE SURE TO EDIT
void addPoint(savedPoint* point)
{

    /*savedPoint* temp = linearBezier((*Vpoint)[Vpoint->size() - 1], point, 0.25);
    Vpoint->push_back(new AnchorFront(temp->x, temp->y));

    delete temp;

    temp = linearBezier((*Vpoint)[Vpoint->size() - 1], point, 0.75);
    Vpoint->push_back(new AnchorBack(temp->x, temp->y));*/
    Vpoint->push_back(new savedPoint(point->x, point->y));
    //Lpoint->push_back(new savedLinesSegments(point->x, point->y));
}

void pushLineVector()
{
    for (int i(0); i < Lpoint->size(); i++) {
        delete (*Lpoint)[i];
    }
    Lpoint->clear();
    if (Vpoint->size() >= 1)
    {
        Lpoint->push_back(new savedLinesSegments((*Vpoint)[0]->x, (*Vpoint)[0]->y));
        for (int i = 1; i < Vpoint->size(); i++)
        {

            //Lpoint->push_back(new savedLinesSegments((*Vpoint)[i-1]->x, (*Vpoint)[i-1]->y));
            savedLinesSegments* temp = LinelinearBezier((*Vpoint)[i - 1], (*Vpoint)[i], 0.25);
            Lpoint->push_back(temp);
            temp = LinelinearBezier((*Vpoint)[i - 1], (*Vpoint)[i], 0.75);
            Lpoint->push_back(temp);
            Lpoint->push_back(new savedLinesSegments((*Vpoint)[i]->x, (*Vpoint)[i]->y));




            /*  savedPoint* temp = linearBezier((*Vpoint)[Vpoint->size() - 1], point, 0.25);
              Vpoint->push_back(new AnchorFront(temp->x, temp->y));

              delete temp;

              temp = linearBezier((*Vpoint)[Vpoint->size() - 1], point, 0.75);
              Vpoint->push_back(new AnchorBack(temp->x, temp->y));
              Vpoint->push_back(new savedPoint(point->x, point->y));*/
        }
    }
    
}


// add more to it
void c1Assert()
{
    for (int i = Lpoint->size() - 3; i >= 0; i -= 3)
    {
        if ((*Lpoint)[i - 1] == (*Lpoint)[0])
        {
            break;
        }
        savedPoint temp = savedPoint((*Lpoint)[i - 1]->x, (*Lpoint)[i - 1]->y);

        temp.x *= 2;
        temp.y *= 2;
        /*temp.x -= Vpoints[i]->x;
        temp.y -= Vpoints[i]->y;*/


        (*Lpoint)[i]->x = temp.x - (*Lpoint)[i - 2]->x;
        (*Lpoint)[i]->y = temp.y - (*Lpoint)[i - 2]->y;
        /*Vpoints[i - 2]->x = temp.x - Vpoints[i]->x;
        Vpoints[i - 2]->y = temp.y - Vpoints[i]->y;*/
    }
}

int CurveHittest(int x, int y)
{
    for (int i(3); i < Lpoint->size(); i+=3) {
        
       for (float j = 0; j < 1; j += 0.0001) {
           savedLinesSegments* temp = cubicBezier((*Lpoint)[i - 3], (*Lpoint)[i - 2], (*Lpoint)[i-1], (*Lpoint)[i], j);
           if (pointHitTestL(temp, x, y)) {
               delete temp;
               return i;
           }
           delete temp;
       }
        
    }
    return -1;
}





