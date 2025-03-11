/*
 * Visualize a magnetic field around a wire.
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

    //Visual model setup
    morph::Visual v(1024, 768, "morph::QuiverVisual");
    morph::vec<float, 3> offset = { 0.0, 0.0, 0.0 };
    v.zNear = 0.001;
    v.showCoordArrows = true;
    v.showTitle = true;
    v.backgroundBlack();
    v.lightingEffects();

    //size of the grid of quivers
    int size = 12;
    int halfsize;
    halfsize = size / 2;


    //Definitions, see below for uses.
    std::vector<morph::vec<float, 3>> xline(size*size*size);
    std::vector<morph::vec<float, 3>> xz(size*size*size);
    std::vector<morph::vec<float, 3>> I(size*size*size);
    std::vector<morph::vec<float, 3>> B(size*size*size);
    std::vector<morph::vec<float, 3>> F(size*size*size);


    //iterator.
    size_t a = 0;

    for (int i = 0; i < size; ++i) {
        for (int j = -halfsize; j < halfsize; ++j) {
            for (int k = -halfsize; k < halfsize; ++k) {
                float x = 0.1*i;
                float y = 0.1*j;
                float z = 0.1*k;

                //coords
                xline[a] = {x, 0, 0}; // The line {x,0,0} for the wire.
                xz[a] = {x, 0, z}; //the xz plane for the B field.

                //fields
                I[a]={1,0,0}; //The current I on {x,0,0}.
                B[a]={0,1,0}; //uniform B field on {x,0,z}.
                F[a]=I[a].cross(B[a]); //The Laplace force (special case of the Lorentz force)

                a++;
            }
        }
    }


    /*
        vmp is the B field visulization.
        vmp2 is the current visualization.
        vmp3 is the force visualization.
    */

    auto vmp = std::make_unique<morph::QuiverVisual<float>>(&xz, offset, &B, morph::ColourMapType::MonochromeRed);
    v.bindmodel (vmp);
    vmp->quiver_length_gain = 0.5f;
    vmp->quiver_thickness_gain = 0.02f;
    vmp->shapesides = 12;

    vmp->finalize();
    v.addVisualModel (vmp);

    auto vmp2 = std::make_unique<morph::QuiverVisual<float>>(&xline, offset, &I, morph::ColourMapType::MonochromeBlue);
    v.bindmodel (vmp2);
    vmp2->quiver_length_gain = 0.5f;
    vmp2->quiver_thickness_gain = 0.02f;
    vmp2->shapesides = 12;

    vmp2->finalize();
    v.addVisualModel (vmp2);

    auto vmp3 = std::make_unique<morph::QuiverVisual<float>>(&xline, offset, &F, morph::ColourMapType::MonochromeGreen);
    v.bindmodel (vmp3);
    vmp3->quiver_length_gain = 0.5f;
    vmp3->quiver_thickness_gain = 0.05f;
    vmp3->finalize();
    v.addVisualModel (vmp3);
    v.keepOpen();


    return 0;
}
