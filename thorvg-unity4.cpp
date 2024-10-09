#include "src/renderer/sw_engine/tvgSwImage.cpp"
#include "src/renderer/sw_engine/tvgSwMath.cpp"
#include "src/renderer/sw_engine/tvgSwPostEffect.cpp"
#include "src/renderer/sw_engine/tvgSwRaster.cpp"
#include "src/renderer/sw_engine/tvgSwRenderer.cpp"

#ifndef FLUX_TVG_SW_RENDERER_PATCH_1
#    error Flux ThorVG software renderer patch 1 not applied

/* TODO Patch line 378

bool SwRenderer::clear()
{
    if (surface)
        return rasterClear(surface, 0, 0, surface->w, surface->h);
    return false;
}

bool SwRenderer::sync()
{
    for (auto task = tasks.data; task < tasks.end(); ++task) {
        if ((*task)->disposed) {
            delete(*task);
        } else {
            (*task)->done();
            (*task)->pushed = false;
        }
    }
    tasks.clear();

    if (!sharedMpool) mpoolClear(mpool);

    // if (surface) {
    //     vport.x = vport.y = 0;
    //     vport.w = surface->w;
    //     vport.h = surface->h;
    // }

    return true;
}

*/
#endif //FLUX_TVG_SW_RENDERER_PATCH_1

#ifndef FLUX_TVG_SW_RENDERER_PATCH_2
#    error Flux ThorVG software renderer patch 2 not applied

/*
Disable rasterClear in SWRender preRender method to allow interoperability with other software renderer

TODO Patch line 448
bool SwRenderer::preRender()
{
    //return rasterClear(surface, 0, 0, surface->w, surface->h);
    
    if (surface) {
        vport.x = vport.y = 0;
        vport.w = surface->w;
        vport.h = surface->h;
    }

    return true;
}
*/
#endif// FLUX_TVG_SW_RENDERER_PATCH_2
