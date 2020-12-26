#include <Windows.h>  
#include <crtdbg.h>   
#include "GLSetup.h"
#include "Utils.h"
#include "Matrix.h"
#include "RenderingContext.h"
#include "time.h"
#include <math.h>
#include "SavedPoints.h"


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



void createObjects(int numOfSegnebts);
void drawPoints();
void c1Assert();
void drawCurve();
void drawAnchorPoints();



void pushLineVector();
int CurveHittest(int x, int y);
void addToBack();
void removePoints(int n);


void cleanUp();

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

    // create shader 
    rcontext.glprogram[RenderingContext::RADIUS_FILL] = loadShaders(L"vertshaderRadiusFill.txt", L"fragshaderRadiusFill.txt");
    glUseProgram(rcontext.glprogram[RenderingContext::RADIUS_FILL]);

    //get shader handles 
    rcontext.poshandle[RenderingContext::RADIUS_FILL] = glGetAttribLocation(rcontext.glprogram[RenderingContext::RADIUS_FILL], "a_position");
    rcontext.diffusehandle[RenderingContext::RADIUS_FILL] = -1;//glGetUniformLocation(rcontext.glprogram, "u_m_diffuse");
    rcontext.mvphandle[RenderingContext::RADIUS_FILL] = glGetUniformLocation(rcontext.glprogram[RenderingContext::RADIUS_FILL], "u_mvpmatrix");
    glEnableVertexAttribArray(rcontext.glprogram[RenderingContext::RADIUS_FILL]);

    // create shader 
    rcontext.glprogram[RenderingContext::DIFFUSE_FILL] = loadShaders(L"vertshaderDiffuse.txt", L"fragshaderDiffuse.txt");
    glUseProgram(rcontext.glprogram[RenderingContext::DIFFUSE_FILL]);

    //get shader handles 
    rcontext.poshandle[RenderingContext::DIFFUSE_FILL] = glGetAttribLocation(rcontext.glprogram[RenderingContext::DIFFUSE_FILL], "a_position");
    rcontext.diffusehandle[RenderingContext::DIFFUSE_FILL] = glGetUniformLocation(rcontext.glprogram[RenderingContext::DIFFUSE_FILL], "u_m_diffuse");
    rcontext.mvphandle[RenderingContext::DIFFUSE_FILL] = glGetUniformLocation(rcontext.glprogram[RenderingContext::DIFFUSE_FILL], "u_mvpmatrix");
    glEnableVertexAttribArray(rcontext.glprogram[RenderingContext::DIFFUSE_FILL]);

    rcontext.currentShader = RenderingContext::RADIUS_FILL;

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

    glUseProgram(rcontext.glprogram[rcontext.currentShader]);

    rcontext.initModelMatrix(true);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(rcontext.poshandle[rcontext.currentShader]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[1]);
    glVertexAttribPointer(rcontext.poshandle[rcontext.currentShader], 3, GL_DOUBLE, false, 0, (void*)0);


    c1Assert();
    drawPoints();
    drawAnchorPoints();
    drawCurve();



    glFinish();
    SwapBuffers(wglGetCurrentDC());
}

void drawPoints()
{
    rcontext.changeShader(RenderingContext::DIFFUSE_FILL);
    for (int i = 0; i < Vpoint->size(); i++)
    {
        rcontext.pushModelMatrix();
        rcontext.translate((*Vpoint)[i]->x, (*Vpoint)[i]->y, 0.0f);
        rcontext.scale(20.0f, 20.0f, 0.0f);
        rcontext.updateMVPs();

        glUniformMatrix4fv(rcontext.mvphandle[rcontext.currentShader], 1, false, rcontext.mvpmatrix);
        glUniform4f(rcontext.diffusehandle[rcontext.currentShader], 1.0f, 0.0f, 1.0f, 1.0f);

        glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);

        rcontext.popModelMatrix();


    }
}

void drawAnchorPoints()
{
    rcontext.changeShader(RenderingContext::DIFFUSE_FILL);
    for (int i = 0; i < Lpoint->size(); i++)
    {
        if (i % 3 == 1 || i % 3 == 2)
        {
            rcontext.pushModelMatrix();
            rcontext.translate((*Lpoint)[i]->x, (*Lpoint)[i]->y, 0.0f);
            rcontext.scale(15.0f, 15.0f, 0.0f);
            rcontext.updateMVPs();

            glUniformMatrix4fv(rcontext.mvphandle[rcontext.currentShader], 1, false, rcontext.mvpmatrix);
            glUniform4f(rcontext.diffusehandle[rcontext.currentShader], 1.0f, 1.0f, 0.0f, 1.0f);

            glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);

            rcontext.popModelMatrix();
        }

    }
}




void drawCurve()
{
    rcontext.changeShader(RenderingContext::RADIUS_FILL);
    if (Vpoint->size() >= 2) {

        for (int i = 3; i < Lpoint->size(); i += 3)
        {

            for (float j = 0; j < 1; j += 0.001) {
                rcontext.pushModelMatrix();

                savedLinesSegments* temp = cubicBezier((*Lpoint)[i - 3], (*Lpoint)[i - 2], (*Lpoint)[i - 1], (*Lpoint)[i], j);
                rcontext.translate(temp->x, temp->y, 0.0f);
                delete temp;

                rcontext.scale(10.0f, 10.0f, 0.0f);
                rcontext.updateMVPs();

                glUniformMatrix4fv(rcontext.mvphandle[rcontext.currentShader], 1, false, rcontext.mvpmatrix);

                glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);
                rcontext.popModelMatrix();
            }
        }

    }
}



// Called when the window is resized
void OnSize(DWORD type, UINT cx, UINT cy)
{
    if (cx > 0 && cy > 0)
    {
        glViewport(0, 0, cx, cy);
        Matrix::setOrtho(rcontext.projectionmatrix, 0, cx, 0, cy, 0, 100);
    }

}




void createObjects(int noOfSegments)
{
    int noOfverts = noOfSegments + 1;
    float radius = 0.5;
    float theta = 360 / noOfSegments;

    double* VERTICES = (double*)malloc(sizeof(double) * noOfverts * 3);
    short* INDICIES = (short*)malloc(sizeof(short) * noOfSegments * 3);

    if (VERTICES) {
        float coordinates = 0;
        int i = 0;
        while (coordinates < 360.0f && i < noOfverts)
        {
            float coordinatesToRadians = (3.14 / 180) * coordinates;
            VERTICES[i * 3] = (radius * sin(coordinatesToRadians));
            VERTICES[i * 3 + 1] = (radius * cos(coordinatesToRadians));
            VERTICES[i * 3 + 2] = 0;
            i++;
            coordinates += theta;
        }
    }

    if (INDICIES) {
        int i = 0;
        while (i < noOfSegments)
        {
            INDICIES[i * 3] = 0;
            INDICIES[i * 3 + 2] = i + 1;
            INDICIES[i * 3 + 1] = i + 2;
            i++;
        }
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
        removePoints(n);
        Vpoint->erase(Vpoint->begin() + n);
    }
    OnDraw();
}

// when the left mouse button is pressed down
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
        addToBack();
    }
    else if (controlPointHIT(x, y) == -1 && anchorPointHIT(x, y) == -1)
    {
        int pos = CurveHittest(x, y);
        if (pos == -1)
        {
            Vpoint->push_back(new savedPoint(point->x, point->y));
            addToBack();

        }
        else
        {
            savedPoint* temp = new savedPoint(point->x, point->y);
            Vpoint->insert(Vpoint->begin() + (pos) / 3, temp);
            pushLineVector();
        }
    }
    else
    {

        if (anchorPointHIT(x, y) != -1)
        {
            selectedAnchor = (*Lpoint)[anchorPointHIT(x, y)];
        }
        else if (pointhit(x, y) != -1)
        {
            selected = (*Vpoint)[pointhit(x, y)];
            selectedControlPont = (*Lpoint)[controlPointHIT(x, y)];
        }
    }
    free(point);
    OnDraw();

}



void OnLButtonUp(UINT nFlags, int x, int y)
{
    if (selected) selected = nullptr;
    if (selectedAnchor) selectedAnchor = nullptr;
    if (selectedControlPont) selectedControlPont = nullptr;
}


void OnMouseMove(UINT nFlags, int x, int y)
{
    RECT rect;
    ::GetClientRect(hwnd, &rect);
    y = rect.bottom - y;

    if (selected) {


        if (findSelectedAnchor() != -1)
        {
            if (Vpoint->size() > 1)
            {
                if (selectedControlPont == (*Lpoint)[Lpoint->size() - 1])
                {
                    int changeX = selectedControlPont->x - (*Lpoint)[findSelectedAnchor() - 1]->x;
                    int changeY = selectedControlPont->y - (*Lpoint)[findSelectedAnchor() - 1]->y;

                    (*Lpoint)[findSelectedAnchor() - 1]->x = x - changeX;
                    (*Lpoint)[findSelectedAnchor() - 1]->y = y - changeY;
                }
                else if (selectedControlPont == (*Lpoint)[0])
                {
                    int changeX = selectedControlPont->x - (*Lpoint)[findSelectedAnchor() + 1]->x;
                    int changeY = selectedControlPont->y - (*Lpoint)[findSelectedAnchor() + 1]->y;

                    (*Lpoint)[findSelectedAnchor() + 1]->x = x - changeX;
                    (*Lpoint)[findSelectedAnchor() + 1]->y = y - changeY;
                }
                else
                {
                    int changeX = selectedControlPont->x - (*Lpoint)[findSelectedAnchor() - 1]->x;
                    int changeY = selectedControlPont->y - (*Lpoint)[findSelectedAnchor() - 1]->y;

                    (*Lpoint)[findSelectedAnchor() - 1]->x = x - changeX;
                    (*Lpoint)[findSelectedAnchor() - 1]->y = y - changeY;

                    changeX = selectedControlPont->x - (*Lpoint)[findSelectedAnchor() + 1]->x;
                    changeY = selectedControlPont->y - (*Lpoint)[findSelectedAnchor() + 1]->y;

                    (*Lpoint)[findSelectedAnchor() + 1]->x = x - changeX;
                    (*Lpoint)[findSelectedAnchor() + 1]->y = y - changeY;


                }
            }
        }
        selected->x = x;
        selected->y = y;
        selectedControlPont->x = x;
        selectedControlPont->y = y;
        OnDraw();
    }
    else if (selectedAnchor)
    {
        selectedAnchor->x = x;
        selectedAnchor->y = y;
        c1Assert();
        OnDraw();
    }
}

// called when a timer event fires - use nIDEvent to figure out which timer event it was
void OnTimer(UINT nIDEvent)
{

}


void pushLineVector()
{
    for (int i = 0; i < Lpoint->size(); i++) {
        delete (*Lpoint)[i];
    }
    Lpoint->clear();
    if (Vpoint->size() >= 1)
    {
        Lpoint->push_back(new savedLinesSegments((*Vpoint)[0]->x, (*Vpoint)[0]->y));
        for (int i = 1; i < Vpoint->size(); i++)
        {
            savedLinesSegments* temp = LinelinearBezier((*Vpoint)[i - 1], (*Vpoint)[i], 0.25);
            Lpoint->push_back(temp);
            temp = LinelinearBezier((*Vpoint)[i - 1], (*Vpoint)[i], 0.75);
            Lpoint->push_back(temp);
            Lpoint->push_back(new savedLinesSegments((*Vpoint)[i]->x, (*Vpoint)[i]->y));
        }
    }

}



void addToBack()
{
    if (Vpoint->size() == 1)
    {
        Lpoint->push_back(new savedLinesSegments((*Vpoint)[0]->x, (*Vpoint)[0]->y));
    }
    if (Vpoint->size() >= 2)
    {
        savedLinesSegments* temp = LinelinearBezier((*Vpoint)[Vpoint->size() - 2], (*Vpoint)[Vpoint->size() - 1], 0.25);
        Lpoint->push_back(temp);
        temp = LinelinearBezier((*Vpoint)[Vpoint->size() - 2], (*Vpoint)[Vpoint->size() - 1], 0.75);
        Lpoint->push_back(temp);
        Lpoint->push_back(new savedLinesSegments((*Vpoint)[Vpoint->size() - 1]->x, (*Vpoint)[Vpoint->size() - 1]->y));
    }
}

void removePoints(int n)
{
    if (Vpoint->size() == 1)
    {
        delete (*Lpoint)[0];
        Lpoint->erase(Lpoint->begin());
    }
    else if (Vpoint->size() > 1)
    {
        if (n == 0)
        {
            for (int i = 0; i < 3; i++)
            {
                delete (*Lpoint)[0];
                Lpoint->erase(Lpoint->begin());
            }
        }
        else if (n == Vpoint->size() - 1)
        {
            for (int i = 0; i < 3; i++)
            {
                delete (*Lpoint)[Lpoint->size() - 1];
                Lpoint->erase(Lpoint->end() - 1);
            }
        }
        else if (Vpoint->size() >= 3)
        {
            for (int i = 0; i < 3; i++)
            {
                delete (*Lpoint)[n * 3 - 1];
                Lpoint->erase(Lpoint->begin() + n * 3 - 1);
            }
        }
    }
}


void c1Assert()
{
    if (findSelected() % 3 == 1)
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

            (*Lpoint)[i - 2]->x = temp.x - (*Lpoint)[i]->x;
            (*Lpoint)[i - 2]->y = temp.y - (*Lpoint)[i]->y;
        }
    }
    else if (findSelected() % 3 == 2)
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



            (*Lpoint)[i]->x = temp.x - (*Lpoint)[i - 2]->x;
            (*Lpoint)[i]->y = temp.y - (*Lpoint)[i - 2]->y;

        }
    }
    else
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

            (*Lpoint)[i]->x = temp.x - (*Lpoint)[i - 2]->x;
            (*Lpoint)[i]->y = temp.y - (*Lpoint)[i - 2]->y;
        }
    }
}

int CurveHittest(int x, int y)
{
    for (int i = 3; i < Lpoint->size(); i += 3) {

        for (float j = 0; j < 1; j += 0.001) {
            savedLinesSegments* temp = cubicBezier((*Lpoint)[i - 3], (*Lpoint)[i - 2], (*Lpoint)[i - 1], (*Lpoint)[i], j);
            if (pointHitTestL(temp, x, y)) {
                delete temp;
                return i;
            }
            delete temp;
        }

    }
    return -1;
}

void cleanUp()
{
    glDeleteBuffers(2, VBOS);
    for (int i = 0; i < RenderingContext::NO_OF_SHADERS; i++)
        glDeleteProgram(rcontext.glprogram[i]);

    for (int i = 0; i < Vpoint->size(); i++) {
        delete (*Vpoint)[i];
    }

    for (int i = 0; i < Lpoint->size(); i++) {
        delete (*Lpoint)[i];
    }

    delete Vpoint;
    delete Lpoint;
}



