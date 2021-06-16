#include <domination.h>

#ifdef Q_OS_ANDROID

bool N::InitAndroid(void)
{
  srand       ( time ( NULL ) ) ;
  initNetwork (               ) ;
  initLIBAV   (               ) ;
  initCURL    (               ) ;
  initMath    (               ) ;
  srand       ( rand()        ) ;
  return true                   ;
}

#else

bool N::InitAndroid(void)
{
  return false ;
}

#endif

void N::AndroidArena (QDir root,QString TarXz)
{
  N::VirtualIO VIO           ;
  N::File      TXZ ( TarXz ) ;
  VIO.setFile(TXZ)           ;
  if (VIO.Detect())          {
    VIO.directory = root     ;
    VIO.Unpack()             ;
  }                          ;
}

void N::AndroidPreparation (QString NDB,QString TarXz)
{
  N :: InitAndroid ( )                     ;
  QString   dbf                            ;
  QFileInfo F                              ;
  QDir      D                              ;
  D         = QDir::homePath()             ;
  dbf       = D . absoluteFilePath ( NDB ) ;
  F . setFile ( dbf )                      ;
  if (F.exists()) return                   ;
  AndroidArena ( D , TarXz )               ;
}
