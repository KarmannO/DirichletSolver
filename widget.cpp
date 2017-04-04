#include "widget.h"

Widget::Widget(QGLWidget *parent) : QGLWidget(parent)
{
    arr = 0;
    rotX = rotY = rotZ = 0;
    dist = 4.0;

    rotX = 45.0;
    rotY = 45.0;

    lines = false;
    camera = new CFollowCamera();
}

void Widget::setH(float *vec, int _h)
{
   n = _h-1;
   if(!arr)
   {
       delete[] arr;
   }
    arr = new float[n*n];

    for(int i = 0; i<n*n;++i)
        arr[i] = vec[i];

    float max = 0.0;

    for (int i = 0; i < n*n; ++i)
    {
        if (arr[i] > max)
            max = arr[i];
    }

    for (int i = 0; i < n*n; ++i)
    {
        arr[i] = (arr[i]*0.4/max);
    }

    h = _h;

    float step = 3.0/(float)h;
    final_verts = new vec3[n * n];
    null_verts = new vec3[n * n];
    for(int i = 0; i < n * n; i++) {
        final_verts[i][0] = -1.5 + step + step*(i%(n));
        final_verts[i][1] = -1.5 + step + step*(i/(n));
        final_verts[i][2] = -arr[i] * 2;

        null_verts[i][0] = final_verts[i][0];
        null_verts[i][1] = final_verts[i][1];
        null_verts[i][2] = 0;
    }

    meshN = (n-1) * (n-1) * 2;
    final_meshes = new vec3i[meshN];
    null_meshes = new vec3i[meshN];
    int cur_mesh = 0;
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - 1; j++) {
            int cur = i * n + j;
            int down = (i + 1) * n + j;
            int right = i * n + (j + 1);

            int reverse = (i + 1) * n + (j + 1);
            final_meshes[cur_mesh][0] = right;
            final_meshes[cur_mesh][1] = cur;
            final_meshes[cur_mesh][2] = down;

            null_meshes[cur_mesh][0] = right;
            null_meshes[cur_mesh][1] = cur;
            null_meshes[cur_mesh][2] = down;

            cur_mesh++;

            final_meshes[cur_mesh][0] = right;
            final_meshes[cur_mesh][1] = down;
            final_meshes[cur_mesh][2] = reverse;

            null_meshes[cur_mesh][0] = right;
            null_meshes[cur_mesh][1] = down;
            null_meshes[cur_mesh][2] = reverse;

            cur_mesh++;
        }
    }
    int center = n / 2;
    int center_dim = center * n + center;
    camera->SetTarget((double*)final_meshes[center_dim]);
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 1.0, 0.5);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &verts_id);
    glBindBuffer(GL_ARRAY_BUFFER, verts_id);
    glBufferData(GL_ARRAY_BUFFER, n * n * sizeof(vec3), final_verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &inds_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inds_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshN * sizeof(vec3i), final_meshes ,GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //==================================

    glGenVertexArrays(1, &vao_null_id);
    glBindVertexArray(vao_null_id);

    glGenBuffers(1, &verts_null_id);
    glBindBuffer(GL_ARRAY_BUFFER, verts_null_id);
    glBufferData(GL_ARRAY_BUFFER, n * n * sizeof(vec3), null_verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &inds_null_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inds_null_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshN * sizeof(vec3i), null_meshes ,GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f,(float)w/(float)h,0.5f,1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Widget::paintGL()
{
    if(!arr)
        return;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    camera->SetView();

    glColor3f(0.0,0.0,0.0);

    /*for (int i = 0; i < (h + 1)*(h + 1); ++i)//большой квадрат
    {
        glBegin(GL_POINTS);
            glVertex3f(-1.5 + step*(i%(h + 1)), 0,-1.5 + step*(i/(h + 1)));
        glEnd();
    }

    if (lines)
    {
        glColor3f(0.0, 0.0, 0.3);

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (i != n - 1)
                {
                    glBegin(GL_LINES);
                        glVertex3f(-1.5 + step + step*(j), arr[i*n + j], -1.5 + step + step*(i));
                        glVertex3f(-1.5 + step + step*(j), arr[(i + 1)*n + j], -1.5 + step + step*(i + 1));
                    glEnd();
                }

                if (j != n - 1)
                {
                    glBegin(GL_LINES);
                        glVertex3f(-1.5 + step + step*(j), arr[i*n + j], -1.5 + step + step*(i));
                        glVertex3f(-1.5 + step + step*(j + 1), arr[i*n + j + 1], -1.5 + step + step*(i));
                    glEnd();
                }
            }
        }
    }

    glColor3f(0.5,0.0,0.0);
    */
    glBindVertexArray(vao_id);
    glDrawElements(GL_TRIANGLES, meshN * sizeof(vec3i), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);

    /*glBindVertexArray(vao_null_id);
    glDrawElements(GL_TRIANGLES, meshN * sizeof(vec3i), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);*/
}

void Widget::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_Left)
        rotY -= 1.0;
    else
    if (ke->key() == Qt::Key_Right)
        rotY += 1.0;
    else
    if (ke->key() == Qt::Key_Up)
        rotX += 1.0;
    else
    if (ke->key() == Qt::Key_Down)
        rotX -= 1.0;
    else
    if (ke->key() == Qt::Key_Plus)
        dist -= 0.1;
    else
    if (ke->key() == Qt::Key_Minus)
        dist += 0.1;
    else
    if (ke->key() == Qt::Key_L)
        lines = !lines;

    if (rotX < -120)
        rotX = 120;
    else
    if (rotX > 120)
        rotX = 120;

    if (rotY < -120)
        rotY = 120;
    else
    if (rotY > 120)
        rotY = 120;

    if (rotZ < -120)
        rotZ = 120;
    else
    if (rotZ > 120)
        rotZ = 120;

    if (dist < 1.0)
        dist = 1.0;
    else
    if (dist > 30.0)
        dist = 30.0;

    updateGL();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    if(control_state.old_mx != -100 && control_state.old_my != -100)
    {
        float dx = (float)(x - control_state.old_mx);
        float dy = (float)(y - control_state.old_my);
        if(control_state.lm_btn)
        {
            camera->RotZ(180.0f * dx / (float)width());
            camera->RotXY(180.0f * dy / (float)height());
        }
    }
    control_state.old_mx = x;
    control_state.old_my = y;
    updateGL();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        control_state.lm_btn = true;
    if(event->button() == Qt::RightButton)
        control_state.rm_btn = true;
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        control_state.lm_btn = false;
    if(event->button() == Qt::RightButton)
        control_state.rm_btn = false;
}

void Widget::wheelEvent(QWheelEvent *event)
{
    float dang = (float)event->angleDelta().y() / 8.0f;
    camera->Move(dang / 20.0f);
    updateGL();
}

