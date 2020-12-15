//#include <Windows.h>  // for access to the windows APIs
//#include <crtdbg.h>   // for debug stuff
//#include "GLSetup.h"
//#include "Utils.h"
//#include "Matrix.h"
//#include "RenderingContext.h"
//#include "time.h"
//#include <vector>
//#include <math.h>
//#include "SavedPoints.h"
//
//#define SCREEN_WIDTH 800
//#define SCREEN_HEIGHT 600
//
//
//
//static HWND hwnd;
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//void OnCreate();
//void OnDraw();
//void OnSize(DWORD type, UINT cx, UINT cy);
//void OnTimer(UINT nIDEvent);
//void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//void OnLButtonDown(UINT nFlags, int x, int y);
//void OnRButtonDown(UINT nFlags, int x, int y);
//void OnLButtonUp(UINT nFlags, int x, int y);
//void OnMouseMove(UINT nFlags, int x, int y);
//
//
//
//
//
//
//// try using a vector insted.
//
//
//
//bool pointHitTest(const savedPoint* point, UINT x, UINT y);
//int CurveHittest(int x, int y);
//int pointhit(int x, int y);
//
//int frontpointHitTest(int x, int y);
//int backpointtHitTest(int x, int y);
//
//void c1Assert2();
//
//void createObjects(int numOfSegnebts);
//void addPoint(savedPoint* point);
//void drawPoints();
//void c1Assert();
//void drawCurve();
//void drawFrontAndBack();
//void cleanUp();
//
//std::vector<savedPoint*>* Vpoint(new std::vector<savedPoint*>());
//savedPoint* selected;
//unsigned int VBOS[2];
//RenderingContext rcontext;
//
//
//// try and move bezir calcs in a class
//savedPoint* linearBezier(const savedPoint* p0, const savedPoint* p1, float t);
//
//
//
//
//// Win32 entry point
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//    // This mini section is really useful to find memory leaks
//#ifdef _DEBUG   // only include this section of code in the DEBUG build
////  _CrtSetBreakAlloc(65);  // really useful line of code to help find memory leaks
//    _onexit(_CrtDumpMemoryLeaks); // check for memory leaks when the program exits
//#endif
//    srand(time(NULL));
//    // To create a window, we must first define various attributes about it
//    WNDCLASSEX classname;
//    classname.cbSize = sizeof(WNDCLASSEX);
//    classname.style = CS_HREDRAW | CS_VREDRAW;
//    classname.lpfnWndProc = (WNDPROC)WndProc;   // This is the name of the event-based callback method
//    classname.cbClsExtra = 0;
//    classname.cbWndExtra = 0;
//    classname.hInstance = hInstance;
//    classname.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//    classname.hCursor = LoadCursor(NULL, IDC_ARROW);
//    classname.hbrBackground = ::GetSysColorBrush(COLOR_3DFACE);
//    classname.lpszMenuName = NULL;
//    classname.lpszClassName = L"GettingStarted";
//    classname.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
//    RegisterClassEx(&classname);
//
//    // Default size will be 800x600 and we'll centre it on the screen - this include the caption and window borders so might not be the canvas size (which will be smaller)
//    int width = SCREEN_WIDTH;
//    int height = SCREEN_HEIGHT;
//    int offx = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
//    int offy = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;
//
//    // Create the window using the definition provided above
//    CreateWindowEx(NULL, L"GettingStarted", L"Getting Started with Win32", WS_OVERLAPPEDWINDOW | WS_VISIBLE, offx, offy, width, height, NULL, NULL, hInstance, NULL);
//
//    // Set the event-based message system up
//    MSG msg;
//    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
//    while (msg.message != WM_QUIT)  // keep looping until we get the quit message
//    {
//        if (GetMessage(&msg, NULL, 0, 0)) // cause this thread to wait until there is a message to process
//        {
//            // These two lines of code take the MSG structure, mess with it, and correctly dispatch it to the WndProc defined during the window creation
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//}
//
//void cleanUp() 
//{
//    glDeleteBuffers(2, VBOS);
//    glDeleteProgram(rcontext.glprogram);
//
//    for (int i(0); i < Vpoint->size(); i++) {
//        delete (*Vpoint)[i]->backpoint;
//        delete (*Vpoint)[i]->frontpoint;
//        delete (*Vpoint)[i];
//    }
//
//    delete Vpoint;
//}
//
//
//// This is our message handling method and is called by the system (via the above while-loop) when we have a message to process
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_CREATE:
//        hwnd = hWnd;
//        OnCreate();
//        return 0;
//        break;
//    case WM_SIZE:
//        OnSize((DWORD)wParam, (UINT)(lParam & 0xFFFF), (UINT)(lParam >> 16));
//        break;
//    case WM_PAINT:
//    {
//        PAINTSTRUCT paint;
//        BeginPaint(hwnd, &paint);
//        OnDraw();
//        EndPaint(hwnd, &paint);
//    }
//    break;
//    case WM_TIMER:
//        OnDraw();
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    case WM_CLOSE:
//        DestroyWindow(hwnd);
//        PostQuitMessage(0);
//        cleanUp();
//        break;
//    case WM_LBUTTONDOWN:
//        OnLButtonDown((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF));
//        break;
//    case WM_RBUTTONDOWN:
//        OnRButtonDown((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF));
//        break;
//    case WM_LBUTTONUP:
//        OnLButtonUp((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF));
//        break;
//    case WM_MOUSEMOVE:
//        OnMouseMove((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF));
//        break;
//    case WM_KEYDOWN:
//        OnKeyDown(wParam, lParam & 0xFFFF, lParam >> 16);
//        break;
//    }
//    return DefWindowProc(hWnd, message, wParam, lParam);
//}
//
//// This is called then the window is first created and useful to get things ready (e.g. load or create pens, brushes, images, etc)
//void OnCreate()
//{
//    initGL(hwnd);
//
//    GLenum err = glewInit();
//    if (err != GLEW_OK)
//        displayMessage((char*)glewGetErrorString(err));
//
//    rcontext.glprogram = loadShaders(L"vertshader.txt", L"fragshader.txt");
//
//    rcontext.poshandle = glGetAttribLocation(rcontext.glprogram, "a_position");
//    //colhandle = glGetAttribLocation(glprogram, "a_colour");
//    rcontext.diffusehandle = glGetUniformLocation(rcontext.glprogram, "u_m_diffuse");
//    rcontext.mvphandle = glGetUniformLocation(rcontext.glprogram, "u_mvpmatrix");
//
//    glUseProgram(rcontext.glprogram);
//
//    createObjects(50);
//
//
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_DEPTH_TEST);
//}
//
//
//
//
//
//
//// This is called when the window needs to be redrawn
//void OnDraw()
//{
//
//    glUseProgram(rcontext.glprogram);
//
//    rcontext.initModelMatrix(true);
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glEnableVertexAttribArray(rcontext.poshandle);
//    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[1]);
//    glVertexAttribPointer(rcontext.poshandle, 3, GL_DOUBLE, false, 0, (void*)0);
//
//
//    c1Assert();
//    drawPoints();
//    drawFrontAndBack();
//    drawCurve();
//    //c1Assert2();
//    
//    
//
//
//
//    glFinish();
//    SwapBuffers(wglGetCurrentDC());
//}
//
//void drawPoints()
//{
//    for (int i = 0; i < Vpoint->size(); i++)
//    {
//        rcontext.pushModelMatrix();
//        rcontext.translate((*Vpoint)[i]->x, (*Vpoint)[i]->y, 0.0f);
//        rcontext.scale(20.0f, 20.0f, 1.0f);
//        rcontext.updateMVPs();
//
//        glUniformMatrix4fv(rcontext.mvphandle, 1, false, rcontext.mvpmatrix);
//        glUniform4f(rcontext.diffusehandle, 1.0f, 0.0f, 0.0f, 1.0f);
//
//        glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);
//
//        rcontext.popModelMatrix();
//    }
//}
//
//
//
//void drawCurve()
//{
//    if (Vpoint->size() >= 2) {
//        for (int i(1); i < Vpoint->size(); i++)
//        {
//            for (float j = 0; j < 1; j+=0.0001) {
//                rcontext.pushModelMatrix();
//                savedPoint* temp = cubicBezier((*Vpoint)[i - 1], (*Vpoint)[i - 1]->frontpoint, (*Vpoint)[i]->backpoint, (*Vpoint)[i],j);
//                rcontext.translate(temp->x, temp->y, 0.0f);
//                delete temp;
//
//                rcontext.scale(8.0f, 8.0f, 1.0f);
//                rcontext.updateMVPs();
//
//                glUniformMatrix4fv(rcontext.mvphandle, 1, false, rcontext.mvpmatrix);
//                glUniform4f(rcontext.diffusehandle, 0.0f, 0.0f, 1.0f, 1.0f);
//
//                glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);
//                rcontext.popModelMatrix();
//            }
//        }
//    }
//}
//
//void drawFrontAndBack()
//{
//
//    for (int i(0); i < Vpoint->size(); i++)
//    {
//        if ((*Vpoint)[i]->frontpoint != nullptr)
//        {
//            rcontext.pushModelMatrix();
//            rcontext.translate((*Vpoint)[i]->frontpoint->x, (*Vpoint)[i]->frontpoint->y, 0.0f);
//            rcontext.scale(8.0f, 8.0f, 1.0f);
//            rcontext.updateMVPs();
//            glUniformMatrix4fv(rcontext.mvphandle, 1, false, rcontext.mvpmatrix);
//            glUniform4f(rcontext.diffusehandle, 1.0f, 1.0f, 0.0f, 1.0f);
//            glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);
//            rcontext.popModelMatrix();
//        }
//
//        
//    }
//
//    for (int i(0); i < Vpoint->size(); i++)
//    {
//        if ((*Vpoint)[i]->backpoint != nullptr)
//        {
//            rcontext.pushModelMatrix();
//            rcontext.translate((*Vpoint)[i]->backpoint->x, (*Vpoint)[i]->backpoint->y, 0.0f);
//            rcontext.scale(8.0f, 8.0f, 1.0f);
//            rcontext.updateMVPs();
//            glUniformMatrix4fv(rcontext.mvphandle, 1, false, rcontext.mvpmatrix);
//            glUniform4f(rcontext.diffusehandle, 1.0f, 0.0f, 1.0f, 1.0f);
//            glDrawElements(GL_TRIANGLE_FAN, 50 * 3, GL_UNSIGNED_SHORT, 0);
//
//            rcontext.popModelMatrix();
//        }
//    }
//    
//}
//
//
//
//// Called when the window is resized
//void OnSize(DWORD type, UINT cx, UINT cy)
//{
//    if (cx > 0 && cy > 0)
//    {
//        glViewport(0, 0, cx, cy);
//        Matrix::setOrtho(rcontext.projectionmatrix, 0, cx, 0, cy, 0, 100);
//        //Matrix::setOrtho(rcontext.projectionmatrix, 0, cx, 0, cy, 0, 200);
//    }
//
//}
//
//
//
//
//void createObjects(int noOfSegments)
//{
//    int noOfverts = noOfSegments + 1;
//
//    float radius = 0.5;
//    float theta = 360 / noOfSegments;
//
//    double* VERTICES = (double*)malloc(sizeof(double) * noOfverts * 3);
//
//    if (VERTICES) {
//
//        size_t i = 1;
//        for (float coordinates = 0;
//            i < noOfverts && coordinates < 360.0f;
//            i++, coordinates += theta)
//        {
//            const float coordinatesToRadians = Matrix::degreesToRadians(coordinates);
//            VERTICES[i * 3] = (double)(radius * sin(coordinatesToRadians));
//            VERTICES[i * 3 + 1] = (double)(radius * cos(coordinatesToRadians));
//            VERTICES[i * 3 + 2] = 0;
//        }
//        VERTICES[0] = VERTICES[1] = VERTICES[2] = 0;
//    }
//
//    short* INDICIES = (short*)malloc(sizeof(short) * noOfSegments * 3);
//    if (INDICIES) {
//        int i = 0;
//        while (i < noOfSegments)
//        {
//            INDICIES[i * 3] = 0;
//            INDICIES[i * 3 + 2] = i + 1;
//            INDICIES[i * 3 + 1] = i + 2;
//            i++;
//        }
//        INDICIES[(i - 1) * 3 + 1] = 1;
//    }
//
//    glGenBuffers(2, (unsigned int*)VBOS);
//    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * noOfverts * 3, VERTICES, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[1]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * noOfSegments * 3, INDICIES, GL_STATIC_DRAW);
//
//    free(VERTICES);
//    free(INDICIES);
//}
//
//
//
//
//// a key has been pressed while this window has focus
//void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//
//}
//
//// when the left mouse button is pressed down
//void OnRButtonDown(UINT nFlags, int x, int y)
//{
//    RECT rect;
//    ::GetClientRect(hwnd, &rect);
//    y = rect.bottom - y;
//
//    int n = pointhit(x, y);
//    if (n != -1)
//    {
//
//        if ((*Vpoint)[n] == Vpoint->front() && Vpoint->size() != 1)
//        {
//            delete(*Vpoint)[n]->frontpoint;
//            delete(*Vpoint)[n];
//            Vpoint->erase(Vpoint->begin() + n);
//
//           
//            delete(*Vpoint)[n]->backpoint;
//            (*Vpoint)[n]->backpoint = nullptr;
//
//        }
//        else if ((*Vpoint)[n] == Vpoint->back() && Vpoint->size() != 1)
//        {
//            delete(*Vpoint)[n]->backpoint;
//            delete(*Vpoint)[n];
//            Vpoint->erase(Vpoint->begin() + n);
//
//            delete(*Vpoint)[n-1]->frontpoint;
//            (*Vpoint)[n-1]->frontpoint = nullptr;
//
//        }
//        else
//        {
//            delete(*Vpoint)[n]->backpoint;
//            delete(*Vpoint)[n]->frontpoint;
//            delete(*Vpoint)[n];
//            Vpoint->erase(Vpoint->begin() + n);
//        }
//        
//        
//    }
//    OnDraw();
//}
//
//// when the left mouse button is pressed down
////try using maloc
//void OnLButtonDown(UINT nFlags, int x, int y)
//{
//
//    RECT rect;
//    ::GetClientRect(hwnd, &rect);
//    y = rect.bottom - y;
//
//    savedPoint* point = (savedPoint*)malloc(sizeof(savedPoint));
//    point->x = x;
//    point->y = y;
//    if (Vpoint->empty())
//    {
//        Vpoint->push_back(new savedPoint(point->x, point->y));
//    }
//    else if (pointhit(x, y) == -1)
//    {
//        
//        int pos = CurveHittest(x, y);
//        if (pos == -1)
//        {
//            addPoint(point);
//            
//        }
//        else
//        {
//            savedPoint* temp = new savedPoint(point->x, point->y);
//            delete (*Vpoint)[pos - 1]->frontpoint;
//            delete (*Vpoint)[pos]->backpoint;
//            (*Vpoint)[pos-1]->frontpoint = linearBezier(temp, (*Vpoint)[pos - 1], 0.75);
//            (*Vpoint)[pos]->backpoint = linearBezier(temp, (*Vpoint)[pos], 0.75);
//            temp->backpoint = linearBezier(temp, (*Vpoint)[pos - 1], 0.25);
//            temp->frontpoint = linearBezier(temp, (*Vpoint)[pos], 0.75);;
//            //temp->frontpoint = new savedPoint((2 * temp->x) - temp->backpoint->x, (2 * temp->y) - temp->backpoint->y);
//            Vpoint->insert(Vpoint->begin() + pos, temp);
//        }
//        
//    }
//    else
//    {
//       /* int f = frontpointHitTest(x, y);
//        if (f !=-1)
//        {
//            selected = (*Vpoint)[f]->frontpoint;
//        }
//        else
//        {
//            selected = (*Vpoint)[pointhit(x, y)];
//        }*/
//        selected = (*Vpoint)[pointhit(x, y)];
//    }
//    free(point);
//    OnDraw();
//
//}
//
//
//// when the left mouse button is released
//void OnLButtonUp(UINT nFlags, int x, int y)
//{
//    selected = nullptr;
//}
//
//// when the mouse moves across the window.  use nFlags to determine if button is down or not plus other stuff
//// select the sidepoints
//void OnMouseMove(UINT nFlags, int x, int y)
//{
//
//    if (selected) {
//        RECT rect;
//        ::GetClientRect(hwnd, &rect);
//        y = rect.bottom - y;
//        selected->x = x;
//        selected->y = y;
//        OnDraw();
//    }
//    
//}
//
//// called when a timer event fires - use nIDEvent to figure out which timer event it was
//void OnTimer(UINT nIDEvent)
//{
//
//}
//
//
//bool pointHitTest(const savedPoint* point, UINT x, UINT y) {
//    int dx = point->x - x;
//    int dy = point->y - y;
//
//    const int radius = 16;
//    return (dx * dx + dy * dy) < (radius * radius);
//}
//
//int CurveHittest(int x, int y)
//{
//    for (int i(1); i < Vpoint->size(); i++) {
//        
//       for (float j = 0; j < 1; j += 0.0001) {
//           savedPoint* temp = cubicBezier((*Vpoint)[i - 1], (*Vpoint)[i - 1]->frontpoint, (*Vpoint)[i]->backpoint, (*Vpoint)[i], j);
//           if (pointHitTest(temp, x, y)) {
//               delete temp;
//               return i;
//           }
//           delete temp;
//       }
//        
//    }
//    return -1;
//}
//
//int pointhit(int x, int y)
//{
//    for (int i(0); i < Vpoint->size(); i++)
//    {
//        if (pointHitTest((*Vpoint)[i], x, y))
//        {
//            return i;
//        }
//    }
//    return -1;
//}
//
//int frontpointHitTest(int x, int y)
//{
//   /* int dx = point->x - x;
//    int dy = point->y - y;
//
//    const int radius = 16;
//    return (dx * dx + dy * dy) < (radius * radius);*/
//    const int radius = 16;
//    if (Vpoint->size() >= 2)
//    {
//        for (int i(0); i < Vpoint->size(); i++)
//        {   
//            if ((*Vpoint)[i]->frontpoint != nullptr)
//            {
//                if (pointHitTest((*Vpoint)[i]->frontpoint, x, y))
//                {
//                    return i;
//                }
//            }
//           
//        }
//       
//    }
//     return -1;
//}
//
//int backpointtHitTest(int x, int y)
//{
//    for (int i(0); i < Vpoint->size(); i++)
//    {
//        if (pointHitTest((*Vpoint)[i]->backpoint, x, y))
//        {
//            return i;
//        }
//    }
//    return -1;
//}
//
//
//// MAKE SURE TO EDIT
//void addPoint(savedPoint* point)
//{
//    savedPoint* temp = new savedPoint(point->x, point->y);
//    Vpoint->push_back(temp);
//    (*Vpoint)[Vpoint->size() - 2]->frontpoint = linearBezier((*Vpoint)[Vpoint->size() - 2], temp, 0.25);
//    temp->backpoint = linearBezier((*Vpoint)[Vpoint->size() - 2], point, 0.75);
//}
//
//
//// add more to it
//void c1Assert()
//{
//    for (int i = Vpoint->size()-2; i >= 0; i-=1)
//    {
//        if ((*Vpoint)[i] != Vpoint->front() && (*Vpoint)[i] != Vpoint->front())
//        {
//            savedPoint temp = savedPoint((*Vpoint)[i]->x, (*Vpoint)[i]->y);
//            temp.x *= 2;
//            temp.y *= 2;
//
//            (*Vpoint)[i]->backpoint->x = temp.x - (*Vpoint)[i]->frontpoint->x;
//            (*Vpoint)[i]->backpoint->y = temp.y - (*Vpoint)[i]->frontpoint->y;
//            //(*Vpoint)[i]->frontpoint->x = temp.x - (*Vpoint)[i]->backpoint->x;
//           
//           // (*Vpoint)[i]->frontpoint->y = temp.y - (*Vpoint)[i]->backpoint->y;
//            
//
//            //(*Vpoint)[i]->backpoint->x = temp.x - (*Vpoint)[i]->frontpoint->x;
//            
//
//            //(*Vpoint)[i]->frontpoint->y = temp.y - (*Vpoint)[i]->backpoint->y;(*Vpoint)[i]->frontpoint->x = temp.x - (*Vpoint)[i]->backpoint->x;
//            //(*Vpoint)[i]->backpoint->y = temp.y - (*Vpoint)[i]->frontpoint->y;
//        }
//    }
//}
//
//
//void c1Assert2()
//{
//    for (int i = Vpoint->size() - 2; i >= 0; i -= 1)
//    {
//        if ((*Vpoint)[i] != Vpoint->front() && (*Vpoint)[i] != Vpoint->front())
//        {
//            savedPoint temp = savedPoint((*Vpoint)[i]->x, (*Vpoint)[i]->y);
//            temp.x *= 2;
//            temp.y *= 2;
//
//            (*Vpoint)[i]->frontpoint->x = temp.x - (*Vpoint)[i]->backpoint->x;
//            (*Vpoint)[i]->frontpoint->y = temp.y - (*Vpoint)[i]->backpoint->y;
//            //(*Vpoint)[i]->frontpoint->x = temp.x - (*Vpoint)[i]->backpoint->x;
//
//           // (*Vpoint)[i]->frontpoint->y = temp.y - (*Vpoint)[i]->backpoint->y;
//
//
//            //(*Vpoint)[i]->backpoint->x = temp.x - (*Vpoint)[i]->frontpoint->x;
//
//
//            //(*Vpoint)[i]->frontpoint->y = temp.y - (*Vpoint)[i]->backpoint->y;(*Vpoint)[i]->frontpoint->x = temp.x - (*Vpoint)[i]->backpoint->x;
//            //(*Vpoint)[i]->backpoint->y = temp.y - (*Vpoint)[i]->frontpoint->y;
//        }
//    }
//}
//
//
//
//
