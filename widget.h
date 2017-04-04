#ifndef WIDGET_H
#define WIDGET_H

#include <QGLWidget>
#include <QDebug>
#include "followcamera.h"
#include "controlbuttonstate.h"
#include "vectors.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLFunctions_3_0>
#include <GL/glu.h>

class Widget : public QGLWidget, public QOpenGLFunctions_3_0
{
    Q_OBJECT
public:
    explicit Widget(QGLWidget *parent = 0);
    void setH(float *vec, int _h);

    CFollowCamera *camera;
    CControlButtonState control_state;
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    float *arr;
    int n;
    int meshN;
    float rotX, rotY, rotZ, dist;

    GLuint vao_id;
    GLuint verts_id;
    GLuint inds_id;

    GLuint vao_null_id;
    GLuint verts_null_id;
    GLuint inds_null_id;

    int h;
    vec3 *final_verts;
    vec3 *null_verts;

    vec3i *final_meshes;
    vec3i *null_meshes;
    bool lines;

    void keyPressEvent(QKeyEvent *ke);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
signals:

public slots:
};

#endif // WIDGET_H
