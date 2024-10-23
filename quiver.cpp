/*
 * Visualize an example quiver field
 */
#include <morph/Visual.h>
#include <morph/ColourMap.h>
#include <morph/QuiverVisual.h>
#include <morph/vec.h>
#include <iostream>
#include <array>
#include <stdexcept>
#include <string>

int main()
{
    int rtn = -1;

    // Demonstrates use of offset (left at 0,0,0), lengths (3,2,1) and the 'thickness'
    // scaling factor (0.5) for the coordinate arrows
    morph::Visual v(1024, 768, "morph::QuiverVisual", {0.8,-0.8}, {.05,.05,.05}, 2.0f, 0.01f);
    v.zNear = 0.001;
    v.showCoordArrows = true;
    v.showTitle = true;
    // For a white background:
    v.backgroundBlack();
    v.lightingEffects();

    try {
        morph::vec<float, 3> offset = { 0.0, 0.0, 0.0 };

        int size = 6;
        int halfsize;
        halfsize = size / 2;

        std::vector<morph::vec<float, 3>> coords(size*size*size);
        std::vector<morph::vec<float, 3>> quivs(size*size*size);

        size_t a = 0;
        size_t k = 0;
        for (int i = -halfsize; i < halfsize; ++i) {
            for (int j = -halfsize; j < halfsize; ++j) {
                for (int l = -halfsize; l < halfsize; ++l) {
                    float x = 0.1*i;
                    float y = 0.1*j;
                    float z = 0.1*l;
                    float L;
                    //map coords
                    coords[a] = {x, y, z};

                    L = std::sqrt((x*x)+(y*y)+(z*z));
                    if(L !=0){quivs[k] = {x/L,y/L,z/L};}else{quivs[k]={0.0f,0.0f,0.0f};};
                    a++;
                    k++;

                }
            }
        }

        auto vmp = std::make_unique<morph::QuiverVisual<float>>(&coords, offset, &quivs, morph::ColourMapType::Viridis);
        v.bindmodel (vmp);
        vmp->quiver_length_gain = 0.1f; // Scale the length of the quivers on screen
        vmp->quiver_thickness_gain = 0.05f; // Scale thickness of the quivers
        // vmp->fixed_quiver_thickness = 0.003f; // Also possible to request a fixed thickness
        vmp->shapesides = 12; // Default is 12, this makes the graphics look nicer - it
                              // controls how many triangles go into each cone, rod and
                              // sphere in the quivers.
        vmp->finalize();
        v.addVisualModel (vmp);

        auto qplot2 = std::make_unique<morph::QuiverVisual<float>>(&coords2, offset, &quivs2, morph::ColourMapType::Viridis);
        v.bindmodel (qplot2);
        qplot2->quiver_length_gain = 0.5f; // Scale the length of the quivers on screen
        qplot2->quiver_thickness_gain = 0.05f; // Scale thickness of the quivers
        // qplot2->fixed_quiver_thickness = 0.003f; // Also possible to request a fixed thickness
        qplot2->shapesides = 12; // Default is 12, this makes the graphics look nicer - it
                              // controls how many triangles go into each cone, rod and
                              // sphere in the quivers.
        qplot2->finalize();
        v.addVisualModel(qplot2);
        v.keepOpen();


    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        rtn = -1;
    }

    return rtn;
}
