#ifndef VIEWER_H
#define VIEWER_H

#include <Qt>
#include <GL/glut.h> // use this on rooster
#include <GL/glu.h> // use this on rooster
#include <QApplication>
#include <QGLViewer/qglviewer.h>
#if QT_VERSION >= 0x040000
# include <QKeyEvent>
#endif

#include "../MISC/general.h"
#include "../experiment.h"
#include "../ROB_SENSORS_OBJS/simple_agents.h"
#include "../ROB_SENSORS_OBJS/simple_objects.h"

//#include "video.h"
#include "sky.h"

class Viewer : public QGLViewer
{
  Q_OBJECT;
 public:
  Viewer(QWidget *parent, EXP_Class *_exp, int argc, char** argv /*, Video *_video */ );
  ~Viewer();
  void keyPressEvent(QKeyEvent *e);
  void glDrawCircle(double x1, double y1, double radius);

  EXP_Class *exp;
  SKY mySky;
  //Video *video;

 protected:
  virtual void init();
  virtual void draw();
  

  
 private:
  unsigned char *pRGB;
};

#endif
