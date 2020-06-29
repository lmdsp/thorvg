#include "testCommon.h"

/************************************************************************/
/* Drawing Commands                                                     */
/************************************************************************/

void tvgDrawCmds(tvg::Canvas* canvas)
{
    canvas->reserve(3);                          //reserve 3 shape nodes (optional)

    //Prepare Round Rectangle
    auto shape1 = tvg::Shape::gen();
    shape1->appendRect(0, 0, 400, 400, 50);      //x, y, w, h, cornerRadius
    shape1->fill(0, 255, 0, 255);                //r, g, b, a
    canvas->push(move(shape1));

    //Prepare Circle
    auto shape2 = tvg::Shape::gen();
    shape2->appendCircle(400, 400, 200, 200);    //cx, cy, radiusW, radiusH
    shape2->fill(255, 255, 0, 255);              //r, g, b, a
    canvas->push(move(shape2));

    //Prepare Ellipse
    auto shape3 = tvg::Shape::gen();
    shape3->appendCircle(600, 600, 150, 100);    //cx, cy, radiusW, radiusH
    shape3->fill(0, 255, 255, 255);              //r, g, b, a
    canvas->push(move(shape3));
}


/************************************************************************/
/* Sw Engine Test Code                                                  */
/************************************************************************/

static unique_ptr<tvg::SwCanvas> swCanvas;

void tvgSwTest(uint32_t* buffer)
{
    //Create a Canvas
    swCanvas = tvg::SwCanvas::gen();
    swCanvas->target(buffer, WIDTH, WIDTH, HEIGHT);

    /* Push the shape into the Canvas drawing list
       When this shape is into the canvas list, the shape could update & prepare
       internal data asynchronously for coming rendering.
       Canvas keeps this shape node unless user call canvas->clear() */
    tvgDrawCmds(swCanvas.get());
}

void drawSwView(void* data, Eo* obj)
{
    swCanvas->draw();
    swCanvas->sync();
}


/************************************************************************/
/* GL Engine Test Code                                                  */
/************************************************************************/

static unique_ptr<tvg::GlCanvas> glCanvas;

void initGLview(Evas_Object *obj)
{
    static constexpr auto BPP = 4;

    //Create a Canvas
    glCanvas = tvg::GlCanvas::gen();
    glCanvas->target(nullptr, WIDTH * BPP, WIDTH, HEIGHT);

    /* Push the shape into the Canvas drawing list
       When this shape is into the canvas list, the shape could update & prepare
       internal data asynchronously for coming rendering.
       Canvas keeps this shape node unless user call canvas->clear() */
    tvgDrawCmds(glCanvas.get());
}

void drawGLview(Evas_Object *obj)
{
    auto gl = elm_glview_gl_api_get(obj);
    int w, h;
    elm_glview_size_get(obj, &w, &h);
    gl->glViewport(0, 0, w, h);
    gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl->glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    gl->glEnable(GL_BLEND);

    glCanvas->draw();
    glCanvas->sync();
}


/************************************************************************/
/* Main Code                                                            */
/************************************************************************/

int main(int argc, char **argv)
{
    tvg::CanvasEngine tvgEngine = tvg::CanvasEngine::Sw;

    if (argc > 1) {
        if (!strcmp(argv[1], "gl")) tvgEngine = tvg::CanvasEngine::Gl;
    }

    //Initialize ThorVG Engine
    if (tvgEngine == tvg::CanvasEngine::Sw) {
        cout << "tvg engine: software" << endl;
    } else {
        cout << "tvg engine: opengl" << endl;
    }

    //Initialize ThorVG Engine
    tvg::Initializer::init(tvgEngine);

    elm_init(argc, argv);

    elm_config_accel_preference_set("gl");

    if (tvgEngine == tvg::CanvasEngine::Sw) {
        createSwView();
    } else {
        createGlView();
    }

    elm_run();
    elm_shutdown();

    //Terminate ThorVG Engine
    tvg::Initializer::term(tvgEngine);
}