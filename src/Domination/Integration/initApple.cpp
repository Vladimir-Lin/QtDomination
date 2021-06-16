#include <domination.h>

#ifdef Q_OS_IOS

bool N::InitApple(void)
{
  srand       ( time ( NULL ) ) ;
  initNetwork (               ) ;
  initLIBAV   (               ) ;
  initCURL    (               ) ;
  initMath    (               ) ;
  nRegMeta    ( QAudio::State ) ;
  srand       ( rand()        ) ;
  return true                   ;
}

#else

bool N::InitApple(void)
{
  return false ;
}

#endif

void N::AppleArena (QDir root,QString TarXz)
{
  N::VirtualIO VIO           ;
  N::File      TXZ ( TarXz ) ;
  VIO.setFile(TXZ)           ;
  if (VIO.Detect())          {
    VIO.directory = root     ;
    VIO.Unpack()             ;
  }                          ;
}

void N::ApplePreparation (QString NDB,QString App,QString TarXz)
{
  N :: InitApple ( )                                 ;
  QString   dbf                                      ;
  QFileInfo F                                        ;
  QDir      D                                        ;
  QString   Doc                                      ;
  QDir      R                                        ;
  D         = QDir::homePath()                       ;
  Doc       = D . absoluteFilePath ( "Documents" )   ;
  R         = QDir ( Doc )                           ;
  dbf       = R . absoluteFilePath ( NDB )           ;
  F . setFile ( dbf )                                ;
  if (F.exists()) return                             ;
  QString TXZ = QString("%1/%2").arg(App).arg(TarXz) ;
  TXZ = D . absoluteFilePath ( TXZ )                 ;
  AppleArena ( R , TXZ )                             ;
}
