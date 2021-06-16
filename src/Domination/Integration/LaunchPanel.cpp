#include <domination.h>

N::LaunchPanel:: LaunchPanel ( QWidget * parent , Plan * p )
               : Widget      (           parent ,        p )
               , Timer       ( new QTimer ( this )         )
               , stack       ( NULL                        )
               , splitter    ( NULL                        )
               , StartId     ( 0                           )
               , IdChanged   ( false                       )
{
  WidgetClass                                                  ;
  addIntoWidget ( parent , this )                              ;
  Shortcuts << new QShortcut(Qt::Key_Left ,this,SLOT(Left ())) ;
  Shortcuts << new QShortcut(Qt::Key_Right,this,SLOT(Right())) ;
  Shortcuts << new QShortcut(Qt::Key_End  ,this,SLOT(End  ())) ;
  Shortcuts << new QShortcut(Qt::Key_Home ,this,SLOT(First())) ;
  nConnect ( Timer , SIGNAL ( timeout   ( ) )                  ,
             this  , SLOT   ( IdUpdated ( ) )                ) ;
  setMinimumSize ( QSize ( 384 , 64 ) )                        ;
  setMaximumSize ( QSize ( 384 , 64 ) )                        ;
}

N::LaunchPanel::~LaunchPanel (void)
{
}

void N::LaunchPanel::resizeEvent(QResizeEvent * event)
{
  Display       ( ) ;
  event->accept ( ) ;
}

void N::LaunchPanel::Evacuate(void)
{
  Finished  (        ) ;
  Alert     ( Action ) ;
  emit Hide (        ) ;
}

void N::LaunchPanel::Finish(void)
{
  emit Quit() ;
}

void N::LaunchPanel::Finished(void)
{
  if ( Floatings . count ( ) > 0 )                      {
    FloatingPanel * fp                                  ;
    fp = Floatings     [ 0 ]                            ;
    Floatings . takeAt ( 0 )                            ;
    if ( IsNull(splitter))                              {
      if ( fp -> isVisible() && ! fp->Fading )          {
        fp   -> FadeOut       ( )                       ;
        plan -> processEvents ( )                       ;
      }                                                 ;
      fp -> deleteLater       ( )                       ;
    } else                                              {
      fp -> deleteLater       ( )                       ;
      QTimer::singleShot ( 100,this,SLOT(MoveStack()) ) ;
    }                                                   ;
  }                                                     ;
}

void N::LaunchPanel::Left(void)
{
  if (StartId>0) StartId--                 ;
  Display ( )                              ;
  IdChanged = true                         ;
  Lastest   = QDateTime::currentDateTime() ;
  Alert ( Action )                         ;
}

void N::LaunchPanel::Right(void)
{
  StartId++                                ;
  if ((StartId+5)>Catalog.count())         {
    StartId = Catalog.count() - 5          ;
  }                                        ;
  Display ( )                              ;
  IdChanged = true                         ;
  Lastest   = QDateTime::currentDateTime() ;
  Alert ( Action )                         ;
}

void N::LaunchPanel::First(void)
{
  StartId   = 0                              ;
  Display ( )                                ;
  IdChanged = true                           ;
  Lastest   = QDateTime::currentDateTime ( ) ;
  Alert ( Action )                           ;
}

void N::LaunchPanel::End(void)
{
  StartId   = Catalog.count() - 5            ;
  if (StartId<0) StartId = 0                 ;
  Display ( )                                ;
  IdChanged = true                           ;
  Lastest   = QDateTime::currentDateTime ( ) ;
  Alert ( Action )                           ;
}

void N::LaunchPanel::add(SUID Id,QString text,QIcon icon)
{
  QToolButton * T = new QToolButton (this)   ;
  QAction     * A = new QAction     (T   )   ;
  T -> setIconSize      (QSize(64,64))       ;
  T -> setAutoRaise     (true        )       ;
  A -> setIcon          (icon        )       ;
  A -> setToolTip       (text        )       ;
  T -> setDefaultAction (A           )       ;
  MapOrders[Catalog.count()] = Id            ;
  Catalog   [Id] = T                         ;
  Actions       << A                         ;
  CatalogMap[A ] = Id                        ;
  MapCatalog[Id] = A                         ;
  connect(T   ,SIGNAL(triggered(QAction*))   ,
          this,SLOT  (activate (QAction*)) ) ;
}

void N::LaunchPanel::activate(QAction * action)
{
  if (!Actions.contains(action)) return ;
  SUID Id = CatalogMap    [ action ]    ;
  Alert ( Action )                      ;
  action -> setEnabled    ( false  )    ;
  plan   -> processEvents (        )    ;
  catalog ( Id , action->toolTip() )    ;
  action -> setEnabled    ( true   )    ;
}

bool N::LaunchPanel::Load(void)
{
  PictureManager PM(plan     )                                           ;
  SqlConnection  SC(plan->sql)                                           ;
  if (SC.open("BranonPanel","Start"))                                    {
    UUIDs Branons                                                        ;
    SUID  uuid                                                           ;
    QString Q                                                            ;
    Q = plan->sql.SelectFrom("uuid","branon"                             ,
          "where user = :USER and host = :HOST order by position asc ;") ;
    SC.Prepare(Q)                                                        ;
    SC.Bind("user",plan->UserUuid)                                       ;
    SC.Bind("host",plan->Hostname.toUtf8())                              ;
    if (SC.Exec()) while (SC.Next()) Branons << SC.Uuid(0)               ;
    foreach (uuid,Branons)                                               {
      SUID icon                                                          ;
      SUID nuid                                                          ;
      QString name                                                       ;
      Q = plan -> sql . SelectFrom                                       (
            "icon,name"                                                  ,
            "branon"                                                     ,
            QString("where uuid = %1").arg(uuid)                       ) ;
      if (SC.Query(Q) && SC.Next())                                      {
        QImage * image                                                   ;
        QIcon    Icon                                                    ;
        icon  = SC . Uuid(0)                                             ;
        nuid  = SC . Uuid(1)                                             ;
        name  = SC . getName                                             (
                       plan->Tables[Tables::Names]                       ,
                       "uuid"                                            ,
                       plan->LanguageId                                  ,
                       nuid                                            ) ;
        image = PM . Thumb   (SC,icon)                                   ;
        Icon  = QIcon(QPixmap::fromImage(*image))                        ;
        delete image                                                     ;
        add(uuid,name,Icon)                                              ;
      }                                                                  ;
    }                                                                    ;
    SC.close()                                                           ;
  }                                                                      ;
  SC.remove()                                                            ;
  return true                                                            ;
}

void N::LaunchPanel::Initialize(void)
{
  for (int i=0;i<6;i++) Buttons[i]  = new QToolButton (this)  ;
  for (int i=0;i<6;i++) Buttons[i] -> setAutoRaise    (true)  ;
  Buttons[0]->setIcon(QIcon(":/images/close.png"))            ;
  Buttons[0]->setIconSize(QSize(64,64))                       ;
  Buttons[0]->setToolTip(tr("Close all and quit"))            ;
  Buttons[1]->setIcon(QIcon(":/images/hidewindow.png"))       ;
  Buttons[1]->setIconSize(QSize(64,64))                       ;
  Buttons[1]->setToolTip(tr("Hide"))                          ;
  Buttons[2]->setIcon(QIcon(":/images/GoRight.png"))          ;
  Buttons[2]->setIconSize(QSize(64,64))                       ;
  Buttons[2]->setToolTip(tr("Next"))                          ;
  Buttons[3]->setIcon(QIcon(":/images/GoLeft.png"))           ;
  Buttons[3]->setIconSize(QSize(64,64))                       ;
  Buttons[3]->setToolTip(tr("Previous"))                      ;
  Buttons[4]->setIcon(QIcon(":/images/first.png"))            ;
  Buttons[4]->setIconSize(QSize(16,16))                       ;
  Buttons[4]->setToolTip(tr("Start"))                         ;
  Buttons[5]->setIcon(QIcon(":/images/end.png"))              ;
  Buttons[5]->setIconSize(QSize(16,16))                       ;
  Buttons[5]->setToolTip(tr("End"))                           ;
  connect(Buttons[0],SIGNAL(clicked()),this,SLOT(Finish  ())) ;
  connect(Buttons[1],SIGNAL(clicked()),this,SLOT(Evacuate())) ;
  connect(Buttons[2],SIGNAL(clicked()),this,SLOT(Right   ())) ;
  connect(Buttons[3],SIGNAL(clicked()),this,SLOT(Left    ())) ;
  connect(Buttons[4],SIGNAL(clicked()),this,SLOT(First   ())) ;
  connect(Buttons[5],SIGNAL(clicked()),this,SLOT(End     ())) ;
  setToolTip(tr("Branon"))                                    ;
  for (int i=0;i<6;i++) Buttons[i] -> show()                  ;
  plan->settings.beginGroup ("Panel")                         ;
  if (plan->settings.contains("StartId"))                     {
    StartId = plan->settings.value("StartId").toInt()         ;
  }                                                           ;
  plan->settings.endGroup   (       )                         ;
  Display()                                                   ;
}

void N::LaunchPanel::Display(void)
{
  if (Buttons.count()==6)                                 {
    int w = width     (       )                           ;
    Buttons[0]->move  (w-32,32)                           ;
    Buttons[0]->resize(  32,32)                           ;
    Buttons[1]->move  (w-32, 0)                           ;
    Buttons[1]->resize(  32,32)                           ;
    Buttons[2]->move  (w-48, 0)                           ;
    Buttons[2]->resize(  16,48)                           ;
    Buttons[3]->move  (   0, 0)                           ;
    Buttons[3]->resize(  16,48)                           ;
    Buttons[4]->move  (   0,48)                           ;
    Buttons[4]->resize(  16,16)                           ;
    Buttons[5]->move  (w-48,48)                           ;
    Buttons[5]->resize(  16,16)                           ;
  }                                                       ;
  if (Catalog.count()>0)                                  {
    int id  = StartId                                     ;
    int eid = id + 5                                      ;
    if (eid>Catalog.count())                              {
      eid = Catalog.count()                               ;
    }                                                     ;
    for (int i=0;i<Catalog.count();i++)                   {
      SUID uuid = MapOrders[i]                            ;
      if (id<=i && i<eid)                                 {
        int s = i - id                                    ;
        Catalog[uuid]->setVisible(true)                   ;
        Catalog[uuid]->move(16+(s*64),0)                  ;
        Catalog[uuid]->resize(64,64)                      ;
      } else                                              {
        Catalog[uuid]->setVisible(false)                  ;
      }                                                   ;
    }                                                     ;
  }                                                       ;
  Buttons[3]->setEnabled(!(StartId==0))                   ;
  Buttons[4]->setEnabled(!(StartId==0))                   ;
  Buttons[2]->setEnabled(!((Catalog.count()-StartId)<=5)) ;
  Buttons[5]->setEnabled(!((Catalog.count()-StartId)<=5)) ;
}

void N::LaunchPanel::IdUpdated(void)
{
  if (!IdChanged) return                                     ;
  if (Lastest.secsTo(QDateTime::currentDateTime())<5) return ;
  shutdown()                                                 ;
  IdChanged = false                                          ;
  Lastest   = QDateTime::currentDateTime()                   ;
}

void N::LaunchPanel::shutdown(void)
{
  plan -> settings . beginGroup ( "Panel"             ) ;
  plan -> settings . setValue   ( "StartId" , StartId ) ;
  plan -> settings . endGroup   (                     ) ;
}

void N::LaunchPanel::catalog(SUID uuid,QString name)
{
  if ( NotNull ( splitter ) )                          {
    if ( Floatings . count ( ) > 0 )                   {
      FloatingPanel * fp                               ;
      fp = Floatings     [ 0 ]                         ;
      Floatings . takeAt ( 0 )                         ;
      fp -> deleteLater       ( )                      ;
    }                                                  ;
  }                                                    ;
  int  total = 0                                       ;
  int  type  = 0                                       ;
  bool go    = false                                   ;
  FriendUuid . clear()                                 ;
  SqlConnection  SC(plan->sql)                         ;
  if (SC.open("BranonPanel","catalog"))                {
    QString Q                                          ;
    Q = plan->sql.SelectFrom                           (
          "count(*)"                                   ,
          "branonpanel"                                ,
          QString("where branon = %1").arg(uuid)     ) ;
    if (SC.Query(Q) && SC.Next())                      {
      total = SC.Value(0).toInt()                      ;
    }                                                  ;
    Q = plan->sql.SelectFrom                           (
          "type","branon"                              ,
          QString("where uuid = %1").arg(uuid)       ) ;
    if (SC.Query(Q) && SC.Next())                      {
      type = SC.Value(0).toInt()                       ;
    }                                                  ;
    SC.close()                                         ;
  }                                                    ;
  SC.remove()                                          ;
  if (type==2) go = true                               ;
  if (total>0) go = true                               ;
  if (!go) return                                      ;
  QRect  R = rect        ( )                           ;
  QPoint P = R.topLeft   ( )                           ;
  P        = mapToGlobal (P)                           ;
  QRect  Z(P.x(),P.y(),R.width(),R.height())           ;
  plan->Talk(name)                                     ;
  FloatingPanel * Floating                             ;
  Floating = new FloatingPanel(NULL,plan)              ;
  Floating->setWindowTitle(name)                       ;
  Floating->setWindowIcon(windowIcon())                ;
  Floating->setToolTip(name)                           ;
  Floating->opacity = opacity                          ;
  Floating->assign(uuid,type,Z,FriendIP.count())       ;
  if (type==2 && (FriendIP.count()>0))                 {
    QStringList Hosts = FriendIP.keys()                ;
    QString     host                                   ;
    SUID        user = 0xFFFFFFFFFFFFFFFELL            ;
    foreach (host,Hosts)                               {
      Floating->add                                    (
        user,host                                      ,
        QIcon(":/images/remoteuser.png")             ) ;
      FriendUuid[user] = host                          ;
      user--                                           ;
    }                                                  ;
  }                                                    ;
  connect ( Floating , SIGNAL(Finished()           )   ,
            this     , SLOT  (Finished()           ) ) ;
  connect ( Floating , SIGNAL(Execute(SUID,QString))   ,
            this     , SLOT  (Execute(SUID,QString)) ) ;
  Floatings << Floating                                ;
  if ( IsNull ( splitter ) )                           {
    Floating -> Arrangement ( )                        ;
  } else                                               {
    QRect g = Floating -> geometry ( )                 ;
    QSize s = g         . size     ( )                 ;
    g         . setTopLeft     ( QPoint(0,0)  )        ;
    Floating -> setMaximumSize ( s            )        ;
    splitter -> insertWidget   ( 0 , Floating )        ;
    Floating -> setGeometry    ( g            )        ;
    Floating -> display        (              )        ;
    Floating -> setMaximumSize ( s            )        ;
    Floating -> show           (              )        ;
    QTimer::singleShot ( 100,this,SLOT(MoveStack())  ) ;
  }                                                    ;
}

void N::LaunchPanel::MoveStack(void)
{
  QSize s = stack -> size ( )                  ;
  QSize t ( 384 , 64 )                         ;
  if ( Floatings.count()> 0 )                  {
    FloatingPanel * fp                         ;
    QSize m                                    ;
    fp = Floatings . last  ( )                 ;
    m  = fp -> maximumSize ( )                 ;
    t  . setHeight ( t.height() + m.height() ) ;
  }                                            ;
  s . setWidth ( 384 )                         ;
  if ( t == s ) return                         ;
  QPoint BP = AvailArea . bottomRight ( )      ;
  QPoint VP = AvailArea . bottomRight ( )      ;
  QPoint XP                                    ;
  QRect  DP                                    ;
  QSize  SP                                    ;
  BP -= QPoint ( s . width() , s . height() )  ;
  VP -= QPoint ( t . width() , t . height() )  ;
  XP  . setX     ( BP . x ( ) )                ;
  SP  . setWidth ( 384        )                ;
  if ( BP . y ( ) > VP . y ( ) )               {
    for (int i=0 ; i <= 10 ; i++ )             {
      int y                                    ;
      int h                                    ;
      y  = BP . y      ( ) * ( 10 - i )        ;
      y += VP . y      ( ) *        i          ;
      y /= 10                                  ;
      h  = s  . height ( ) * ( 10 - i )        ;
      h += t  . height ( ) *        i          ;
      h /= 10                                  ;
      XP . setY                 ( y  )         ;
      SP . setHeight            ( h  )         ;
      DP . setTopLeft           ( XP )         ;
      DP . setSize              ( SP )         ;
      stack -> setGeometry      ( DP )         ;
      qApp  -> processEvents    (    )         ;
      qApp  -> sendPostedEvents (    )         ;
      Time  :: msleep(50)                      ;
    }                                          ;
  }                                            ;
  DP        . setTopLeft       ( VP )          ;
  DP        . setSize          ( t  )          ;
  stack    -> setGeometry      ( DP )          ;
  splitter -> setSizeIncrement ( t  )          ;
}

void N::LaunchPanel::Execute(SUID uuid,QString name)
{
  Finished()                               ;
  if (FriendUuid.contains(uuid))           {
    QString Host = FriendUuid [uuid]       ;
    QString IP   = FriendIP   [Host]       ;
    int     Port = FriendPort [Host]       ;
    emit RemoteUser (Host,IP,Port)         ;
    return                                 ;
  }                                        ;
  emit Hide()                              ;
  bool success = false                     ;
  Execution ne                             ;
  SqlConnection  SC(plan->sql)             ;
  if (SC.open("BranonFloating","Execute")) {
    ne.clear()                             ;
    success = ne.Load(plan,SC,uuid)        ;
    SC.close()                             ;
  }                                        ;
  SC.remove()                              ;
  if (!success) return                     ;
  plan->Talk(name)                         ;
  ne.Run()                                 ;
}
