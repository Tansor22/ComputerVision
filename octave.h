#ifndef OCTAVE_H
#include <pyramid.h>
#define OCTAVE_H


class Octave
{
    QList<Pyramid*> *layers;
    int octave;
public:
    Octave();
    // destructor
    ~Octave() {
        for (int i = 0; i < layers->count(); ++i) {
            delete (*layers)[i];
        }
        layers->clear();
        delete layers;
    }


    Octave(QList<Pyramid*> *layers, int octave) : layers(layers), octave(octave)  {};

    QList<Pyramid *> *getLayers() const {return  layers;}
    void setLayers(QList<Pyramid *> *value) { layers = value;}
    int getOctave() const {return octave;}
    void setOctave(int value) {octave = value;}
};

#endif // OCTAVE_H
