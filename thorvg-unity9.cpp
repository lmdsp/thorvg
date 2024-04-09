#include "src/loaders/lottie/tvgLottieBuilder.cpp"
#include "src/loaders/lottie/tvgLottieInterpolator.cpp"
#include "src/loaders/lottie/tvgLottieLoader.cpp"
#include "src/loaders/lottie/tvgLottieModel.cpp"

#ifndef FLUX_TVG_LOTTIE_PATCHED
#    error Flux ThorVG lottie loader patch not applied

/*
Apply following patch at line 236

auto ret = fread(content, sizeof(char), size, f);
    if (ret < size) {

        //hack for  windows !!! don't know why but sometimes size is not correct
        // e.g. size == 33113, ret == 31708
        #define FLUX_TVG_LOTTIE_PATCHED
        if (ret < size - (size / 16))
        {
            fclose(f);
            return false;
        }
        size = ret;
    }

*/
#endif// FLUX_TVG_LOTTIE_PATCHED