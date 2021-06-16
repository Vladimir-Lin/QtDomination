#include <domination.h>

N::SeekerTemplate:: SeekerTemplate ( QWidget * parent , Plan * p )
                  : StackedWidget  (           parent ,        p )
                  , shuting        ( false                       )
                  , lines          ( 0                           )
                  , seeker         ( NULL                        )
{
  WidgetClass ;
}

N::SeekerTemplate::~SeekerTemplate (void)
{
}

CUIDs N::SeekerTemplate::SupportedSeekers (void)
{
  CUIDs  IDs ;
  return IDs ;
}

N::SiteSeeker * N::SeekerTemplate::AskSeeker(int type,QWidget * parent,Plan * p)
{ Q_UNUSED ( type   ) ;
  Q_UNUSED ( parent ) ;
  Q_UNUSED ( p      ) ;
  return NULL         ;
}

bool N::SeekerTemplate::startup (void)
{
  DoProcessEvents           ;
  loadGeometry    (       ) ;
  DoProcessEvents           ;
  Relocation      (       ) ;
  DoProcessEvents           ;
  start           ( 10001 ) ;
  return true               ;
}

void N::SeekerTemplate::resizeEvent(QResizeEvent * event)
{
  QStackedWidget :: resizeEvent ( event ) ;
  Relocation                    (       ) ;
}

void N::SeekerTemplate::closeEvent(QCloseEvent * event)
{
  if (shutdown())                          {
    QStackedWidget :: closeEvent ( event ) ;
  } else event->ignore()                   ;
}

bool N::SeekerTemplate::shutdown(void)
{
  if (shuting) return false ;
  shuting = true            ;
  saveGeometry ( )          ;
  return true               ;
}

void N::SeekerTemplate::loadGeometry(QString prefix)
{
  QString MDI = prefix + "Mdi"                              ;
  QString GEO = prefix + "Geometry"                         ;
  QMdiSubWindow * msw = Casting(QMdiSubWindow,parent())     ;
  bool mdi      = false                                     ;
  bool geometry = false                                     ;
  ///////////////////////////////////////////////////////////
  plan -> site . beginGroup ( MDI )                         ;
  if ( plan -> site . contains ( "size" ) ) mdi = true      ;
  plan -> site . endGroup   (     )                         ;
  plan -> site . beginGroup ( GEO )                         ;
  if ( plan -> site . contains ( "size" ) ) geometry = true ;
  plan -> site . endGroup   (     )                         ;
  ///////////////////////////////////////////////////////////
  if (mdi && NotNull(msw))                                  {
    QList<int> INTs                                         ;
    INTs = plan->site.getIntList(MDI)                       ;
    QRect R                                                 ;
    R . setLeft   ( INTs [0] )                              ;
    R . setTop    ( INTs [1] )                              ;
    R . setWidth  ( INTs [2] )                              ;
    R . setHeight ( INTs [3] )                              ;
    msw->setGeometry(R)                                     ;
  }                                                         ;
  ///////////////////////////////////////////////////////////
  if (geometry)                                             {
    QList<int> INTs                                         ;
    INTs = plan->site.getIntList(GEO)                       ;
    QRect R                                                 ;
    R . setLeft   ( INTs [0] )                              ;
    R . setTop    ( INTs [1] )                              ;
    R . setWidth  ( INTs [2] )                              ;
    R . setHeight ( INTs [3] )                              ;
    setGeometry(R)                                          ;
  }                                                         ;
}

void N::SeekerTemplate::saveGeometry(QString prefix)
{
  QString MDI = prefix + "Mdi"                          ;
  QString GEO = prefix + "Geometry"                     ;
  QMdiSubWindow * msw = Casting(QMdiSubWindow,parent()) ;
  QRect mg = geometry()                                 ;
  CUIDs MINTs                                           ;
  CUIDs GINTs                                           ;
  if (NotNull(msw))                                     {
    QRect mr = msw->geometry()                          ;
    MINTs << mr.left   ()                               ;
    MINTs << mr.top    ()                               ;
    MINTs << mr.width  ()                               ;
    MINTs << mr.height ()                               ;
    plan -> site . setIntList(MDI,MINTs)                ;
  }                                                     ;
  GINTs << mg.left   ()                                 ;
  GINTs << mg.top    ()                                 ;
  GINTs << mg.width  ()                                 ;
  GINTs << mg.height ()                                 ;
  plan -> site . setIntList(GEO,GINTs)                  ;
}

void N::SeekerTemplate::loadGeometry(void)
{
  loadGeometry ( Prefix ) ;
}

void N::SeekerTemplate::saveGeometry(void)
{
  saveGeometry ( Prefix ) ;
}

void N::SeekerTemplate::Relocation(void)
{
}

void N::SeekerTemplate::SeekerChanged(int state)
{
  SeekerModified ( ) ;
}

void N::SeekerTemplate::SeekerModified(void)
{
}

void N::SeekerTemplate::urlPressed(void)
{
}

void N::SeekerTemplate::showMessage(QString message)
{
}

void N::SeekerTemplate::LookUrl(QString url)
{
}

void N::SeekerTemplate::Probe(bool enabled)
{
}

void N::SeekerTemplate::HunterChanged(int index)
{
}

void N::SeekerTemplate::LookForTemp(void)
{
}

void N::SeekerTemplate::TempChanged(void)
{
}

void N::SeekerTemplate::siteClicked(QTreeWidgetItem * item,int column)
{
}

void N::SeekerTemplate::run(int Type,ThreadData * data)
{
  switch ( Type )    {
    case 10001       :
      Initialize ( ) ;
    break            ;
    case 10002       :
      Seeking    ( ) ;
    break            ;
  }                  ;
}

bool N::SeekerTemplate::ObtainSql(void)
{
  QString CN = QtUUID::createUuidString()  ;
  SqlConnection SC ( plan->sql )            ;
  if (SC.open("ObtainSql",CN))              {
    SUID u = hunter->toUuid()               ;
    HunterSql = SC . GetConnection          (
                  PlanTable(SqlConnections) ,
                  u                       ) ;
    SC.close()                              ;
  }                                         ;
  SC.remove()                               ;
  return HunterSql . isValid   ( )          ;
}

void N::SeekerTemplate::Initialize(void)
{
}

void N::SeekerTemplate::Seeking(void)
{
}
