#include <domination.h>

QScriptValue N::FullAttachment(QScriptContext * context,QScriptEngine * engine)
{
  Q_UNUSED ( context )                                                  ;
  N::FullManager * nfm = new N::FullManager ( engine , (Plan *) qPlan ) ;
  return engine -> newQObject               ( nfm                     ) ;
}

N::FullManager:: FullManager       ( QObject * parent , Plan * p )
               : FoundationManager (           parent ,        p )
               , fullplan          (                           p )
{
  netm = new NetworkManager ( p ) ;
  picm = new PictureManager ( p ) ;
}

N::FullManager::~FullManager(void)
{
  delete netm ;
  delete picm ;
}

QString N::FullManager::ImportPicture(QString filename)
{
  PictureManager PM ( fullplan ) ;
  SUID           u = 0           ;
  PM . Import ( filename , u )   ;
  return QString::number ( u )   ;
}

bool N::FullManager::RemoveFile(QString filename)
{
  return QFile::remove ( filename ) ;
}

bool N::FullManager::ConnectSql(QString name)
{
  if ( Connections . contains ( name ) ) return true     ;
  SqlConnection * SC = new SqlConnection ( plan -> sql ) ;
  if ( ! SC -> open ( FunctionString ) )                 {
    SC -> close  ( )                                     ;
    SC -> remove ( )                                     ;
    delete SC                                            ;
    return false                                         ;
  }                                                      ;
  Connections [ name ] = SC                              ;
  return true                                            ;
}

bool N::FullManager::RemoveSql(QString name)
{
  if ( ! Connections . contains ( name ) ) return false ;
  SqlConnection * SC = Connections [ name ]             ;
  Connections . remove ( name )                         ;
  SC -> close  ( )                                      ;
  SC -> remove ( )                                      ;
  delete SC                                             ;
  return true                                           ;
}

QString N::FullManager::ImportPicture(QString cn,QString filename)
{
  if ( ! Connections . contains ( cn ) ) return "0" ;
  SqlConnection * SC = Connections [ cn ]           ;
  SUID u = 0                                        ;
  picm -> Import ( *SC , filename , u )             ;
  return QString::number ( u )                      ;
}

bool N::FullManager::DomainIndex(QString cn)
{
  if ( ! Connections . contains ( cn ) ) return false ;
  SqlConnection * SC = Connections [ cn ]             ;
  return netm -> LoadDomainIndex ( *SC )              ;
}

QString N::FullManager::PageUuid(QString cn,QString url)
{
  if ( ! Connections . contains ( cn ) ) return "0" ;
  SqlConnection * SC = Connections [ cn ]           ;
  SUID u = netm -> PageUuid ( *SC , url )           ;
  return QString::number ( u )                      ;
}

QString N::FullManager::InsertPage(QString cn,QString url)
{
  if ( ! Connections . contains ( cn ) ) return "0" ;
  SqlConnection * SC = Connections [ cn ]           ;
  QUrl page ( url )                                 ;
  SUID u = netm -> insertPage ( *SC , page )        ;
  return QString::number ( u )                      ;
}

QString N::FullManager::AssurePage(QString cn,QString url)
{
  if ( ! Connections . contains ( cn ) ) return "0" ;
  SqlConnection * SC = Connections [ cn ]           ;
  QUrl page ( url )                                 ;
  SUID u = netm -> assurePage ( *SC , page )        ;
  return QString::number ( u )                      ;
}
