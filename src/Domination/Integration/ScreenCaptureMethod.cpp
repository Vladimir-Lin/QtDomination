#include <domination.h>

#include "ui_ScreenCaptureMethod.h"

N::ScreenCaptureMethod:: ScreenCaptureMethod ( QWidget * parent , Plan * p )
                       : Widget              (           parent ,        p )
                       , ui                  ( new Ui::ScreenCaptureMethod )
{
  WidgetClass ;
  ui -> setupUi         ( this                                                  ) ;
  ui -> Path -> setText ( plan -> Path ( Directory::Images ) . absolutePath ( ) ) ;
}

N::ScreenCaptureMethod::~ScreenCaptureMethod (void)
{
  delete ui;
}

bool N::ScreenCaptureMethod::isWindow(void)
{
  return ui->Window->isChecked() ;
}

bool N::ScreenCaptureMethod::isDesktop(void)
{
  return ui->Desktop->isChecked() ;
}

bool N::ScreenCaptureMethod::isImmediate(void)
{
  return ui->Immediate->isChecked() ;
}

bool N::ScreenCaptureMethod::isDelay(void)
{
  return ui->Delay->isChecked() ;
}

bool N::ScreenCaptureMethod::isPeriod(void)
{
  return ui->Period->isChecked() ;
}

int N::ScreenCaptureMethod::Seconds(void)
{
  if (isDelay ()) return ui -> Seconds       -> value ( ) ;
  if (isPeriod()) return ui -> PeriodSeconds -> value ( ) ;
  return 0                                                ;
}

int N::ScreenCaptureMethod::Period(void)
{
  return ui->Record->value() ;
}

QString N::ScreenCaptureMethod::Path(void)
{
  return ui->Path->text() ;
}

void N::ScreenCaptureMethod::BrowsePath(void)
{
  QString dir = ui->Path->text()                                  ;
  dir = QFileDialog::getExistingDirectory(this,windowTitle(),dir) ;
  if (dir.length()>0) ui->Path->setText(dir)                      ;
}
