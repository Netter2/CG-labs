
#include "hyperboloid.h"

#include <QRandomGenerator>

Hyperboloid::Hyperboloid (int approx) {
    approximation = approx;
    innerPoint = QVector4D (0, 0.3, 0, 1);
    // polygons:
    int circlePoints = 2 + approx * 2;
    points.append (QVector4D (0, 0.5 / approx, 0, 1));
    for (int i = 1; i <= approx; i ++) {
        float h = 1.0 / approx * i;
        for (int j = 0; j < circlePoints; j ++) {
            points.append (QVector4D (0.5 * h * sin (2.0 * M_PI * j / circlePoints), 0.5 * h, 0.5 * h * cos (2.0 * M_PI * j / circlePoints), 1));
        }
    }
    for (int i = 1; i < circlePoints; i ++) {
        polygons.append (QVector <int> ({0, i, i + 1}));
    }
    polygons.append (QVector <int> ({0, circlePoints, 1}));
    for (int i = 0; i < approx - 1; i ++) {
        for (int j = circlePoints * i + 1; j < circlePoints * (i + 1); j ++) {
            polygons.append (QVector <int> ({j, j + 1, j + circlePoints + 1, j + circlePoints}));
        }
        polygons.append (QVector <int> ({circlePoints * (i + 1),
                                         circlePoints * i + 1,
                                         circlePoints * (i + 1) + 1,
                                         circlePoints * (i + 2)}));
    }
    points.append (QVector4D (0, 0.5, 0, 1));
    for (int i = 1 + circlePoints * (approx - 1); i < circlePoints * approx; i ++) {
        polygons.append (QVector <int> ({i, i + 1, points.size () - 1}));
    }
    polygons.append (QVector <int> ({circlePoints * approx, 1 + circlePoints * (approx - 1), points.size () - 1}));

    // normals:
    for (int i = 0; i < polygons.size (); i ++) {
        polygons [i].n = crossProduct (points [polygons [i].p [0]] - points [polygons [i].p [1]],
                                       points [polygons [i].p [2]] - points [polygons [i].p [1]]);
        if (dotProduct (innerPoint - points [polygons [i].p [1]], polygons [i].n) > 0) {
            polygons [i].n *= -1;
        }
        polygons [i].n.normalize ();
    }

    pNormals.append (0.0);
    pNormals.append (-1.0);
    pNormals.append (0.0);
    for (int i = 1; i < points.size () - 1; i ++) {
        QVector3D normal (0.0, 0.0, 0.0);
        for (int j = 0; j < polygons.size (); j ++) {
            if (polygons [j].p.contains (i)) {
                normal += QVector3D (polygons [j].n.x (), polygons [j].n.y (), polygons [j].n.z ());
            }
        }
        normal.normalize ();
        pNormals.append (normal.x ());
        pNormals.append (normal.y ());
        pNormals.append (normal.z ());
    }
    pNormals.append (0.0);
    pNormals.append (1.0);
    pNormals.append (0.0);
}


void * Hyperboloid::GetVerticesArray () {
    vertices.clear ();
    for (int i = 0; i < points.size (); i ++) {
        vertices.append (points [i].x ());
        vertices.append (points [i].y ());
        vertices.append (points [i].z ());
    }
    return (void *) vertices.data ();
}

void * Hyperboloid::GetNormalsArray () {
    return (void *) pNormals.data ();
}

int * Hyperboloid::GetIndicesArray () {
    int circlePoints = 2 + approximation * 2;
    int layers = approximation;
    indices.clear ();
    for (int i = 0; i <= circlePoints; i ++) {
        indices.append (i);
    }
    indices.append (1);
    ////
    for (int i = 0; i < layers - 1; i ++) {
        for (int j = circlePoints * i + 1; j < circlePoints * (i + 1) + 1; j ++) {
            indices.append (j               );
            indices.append (j + circlePoints);
        }
        indices.append (circlePoints * i + 1);
        indices.append (circlePoints * (i + 1) + 1);
    }
    ////
    for (int i = points.size () - 1; i >= points.size () - (approximation * 2 + 2) - 1; i --) {
        indices.append (i);
    }
    indices.append (points.size () - 2);

    return indices.data ();
}


