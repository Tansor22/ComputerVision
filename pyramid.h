#ifndef PYRAMID_H
#include <imagetoprocess.h>
#define PYRAMID_H


class Pyramid
{
public:
    Pyramid();
    ~Pyramid() {
        delete img;
    };

    Pyramid(ImageToProcess *img, int nOctaves, int nLayers)
        :  nOctaves(nOctaves), nLayers(nLayers), img(img) {};

    int nOctaves;
    int nLayers;
    ImageToProcess *img;

    double sigmaLocal;
    double sigmaEffective;

    // octaves & layers
    int getOctaves() const {return nOctaves;}
    void setOctaves(int value)  {nOctaves = value;}
    int getLayers() const {return nLayers;}
    void setLayers(int value)  {nLayers = value;}
    // image
    ImageToProcess *getImage() const {return img;}
    void setImage(ImageToProcess *img){this->img = img;}
    // sigmas
    double getSigmaLocal() const {return sigmaLocal;}
    void setSigmaLocal(double value) {sigmaLocal = value;}
    double getSigmaEffective() const{return sigmaEffective;}
    void setSigmaEffective(double value) {sigmaEffective = value;}
};

#endif // PYRAMID_H
