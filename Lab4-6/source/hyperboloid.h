#ifndef HYPERBOLOID_H
#define HYPERBOLOID_H

#include <QVector>
#include <QVector4D>
#include <QMatrix4x4>
#include <QtMath>

// maxX, maxY, maxZ = 1; a, b, c = 1;

class Hyperboloid {
public:
    struct Polygon {
        QVector <int> p;
        QVector4D n;
        //QVector4D l;
        QVector3D color;
        Polygon (QVector <int> nv) {
            p = nv;
        }
    };
    QVector <QVector4D> points;
    QVector <Polygon> polygons;
    QVector4D innerPoint;
    int approximation;
    Hyperboloid () {}
    Hyperboloid (int approx);
    float dotProduct (QVector4D v1, QVector4D v2) {
        return v1.x () * v2.x () + v1.y () * v2.y () + v1.z () * v2.z ();
    }
    QVector4D crossProduct (QVector4D v1, QVector4D v2) {
        return QVector4D (
                    v1.y () * v2.z () - v1.z () * v2.y (),
                    v1.z () * v2.x () - v1.x () * v2.z (),
                    v1.x () * v2.y () - v1.y () * v2.x (),
                    1);
    }

    QVector <float> vertices;
    void * GetVerticesArray ();
    QVector <int> indices;
    int * GetIndicesArray ();
    QVector <float> pNormals;
    void * GetNormalsArray ();
};

#endif // HYPERBOLOID_H
