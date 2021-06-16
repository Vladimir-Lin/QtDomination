#include <domination.h>

N::CiosPanel:: CiosPanel ( QWidget * parent , Plan * p )
             : Widget    (           parent ,        p )
             , Timer     ( new QTimer ( this )         )
             , wrapper   ( NULL                        )
             , StartId   ( 0                           )
             , IdChanged ( false                       )
{
  WidgetClass                     ;
  addIntoWidget ( parent , this ) ;
  Configure     (               ) ;
}

N::CiosPanel::~CiosPanel (void)
{
}

void N::CiosPanel::resizeEvent(QResizeEvent * e)
{
  Display     ( ) ;
  e -> accept ( ) ;
}

void N::CiosPanel::showEvent(QShowEvent * e)
{
  Display     ( ) ;
  e -> accept ( ) ;
}

void N::CiosPanel::Configure(void)
{
  table      = "branon"                                                    ;
  panel      = "branonpanel"                                               ;
  item       = "branon"                                                    ;
  iconSize   =     QSize     ( 64 , 64                                   ) ;
  Shortcuts << new QShortcut ( Qt::Key_Left  , this , SLOT ( Left  ( ) ) ) ;
  Shortcuts << new QShortcut ( Qt::Key_Right , this , SLOT ( Right ( ) ) ) ;
  Shortcuts << new QShortcut ( Qt::Key_End   , this , SLOT ( End   ( ) ) ) ;
  Shortcuts << new QShortcut ( Qt::Key_Home  , this , SLOT ( First ( ) ) ) ;
  nConnect ( Timer , SIGNAL ( timeout   ( ) )                              ,
             this  , SLOT   ( IdUpdated ( ) )                            ) ;
  setMinimumSize ( QSize ( 384 , 64 ) )                                    ;
  setMaximumSize ( QSize ( 384 , 64 ) )                                    ;
  nConnect ( this , SIGNAL ( Hide   ( ) )                                  ,
             this , SLOT   ( Hidden ( ) )                                ) ;
}

void N::CiosPanel::setTable(QString t)
{
  table = t ;
}

void N::CiosPanel::setPanel(QString t)
{
  panel = t ;
}

void N::CiosPanel::setItem(QString it)
{
  item = it ;
}

void N::CiosPanel::setButtonSize(QSize is)
{
  iconSize = is ;
}

void N::CiosPanel::setWrapper(ToolWrapper * w)
{
  wrapper  = w                     ;
  wrapper -> setWidget   ( this  ) ;
  wrapper -> setAbstract ( this  ) ;
  move                   ( 0 , 0 ) ;
}

void N::CiosPanel::Evacuate(void)
{
  Finished  (        ) ;
  Alert     ( Action ) ;
  emit Hide (        ) ;
}

void N::CiosPanel::Appear(void)
{
  QWidget * w = this                      ;
  if ( NULL != wrapper )                  {
    w = wrapper                           ;
  }                                       ;
  nDropOut         ( w -> isVisible ( ) ) ;
  w -> setGeometry ( DockArea           ) ;
  FadeIn           ( w , 10             ) ;
}

void N::CiosPanel::Disappear(void)
{
  QWidget * w = this     ;
  if ( NULL != wrapper ) {
    w = wrapper          ;
  }                      ;
  w -> hide ( )          ;
}

void N::CiosPanel::Eradicate(void)
{
  QWidget * w = this                 ;
  if ( NULL != wrapper ) w = wrapper ;
  w -> deleteLater ( )               ;
}

void N::CiosPanel::Hidden(void)
{
  QWidget * w = this                ;
  if ( NULL != wrapper )            {
    w = wrapper                     ;
  }                                 ;
  nDropOut ( ! w -> isVisible ( ) ) ;
  FadeOut  ( w , 10               ) ;
}

void N::CiosPanel::Finish(void)
{
  emit Quit ( ) ;
}

void N::CiosPanel::Finished(void)
{
  nDropOut ( Floatings . count ( ) <= 0 )   ;
  CiosFloating * fp = Floatings . first ( ) ;
  Floatings . takeFirst ( )                 ;
  fp   -> disappear     ( )                 ;
  plan -> processEvents ( )                 ;
}

void N::CiosPanel::Left(void)
{
  if ( StartId > 0 ) StartId -- ;
  Display (        )            ;
  IdChanged = true              ;
  Lastest   = nTimeNow          ;
  Alert   ( Action )            ;
}

void N::CiosPanel::Right(void)
{
  StartId ++                                   ;
  if ( ( StartId + 5 ) > Catalog . count ( ) ) {
    StartId = Catalog . count ( ) - 5          ;
  }                                            ;
  Display (        )                           ;
  IdChanged = true                             ;
  Lastest   = nTimeNow                         ;
  Alert   ( Action )                           ;
}

void N::CiosPanel::First(void)
{
  StartId   = 0        ;
  Display (        )   ;
  IdChanged = true     ;
  Lastest   = nTimeNow ;
  Alert   ( Action )   ;
}

void N::CiosPanel::End(void)
{
  StartId   = Catalog . count ( ) - 5 ;
  if ( StartId < 0 ) StartId = 0      ;
  Display (        )                  ;
  IdChanged = true                    ;
  Lastest   = nTimeNow                ;
  Alert   ( Action )                  ;
}

void N::CiosPanel::add(SUID Id,QString text,QIcon icon)
{
  QToolButton * T = new QToolButton ( this              ) ;
  QAction     * A = new QAction     ( T                 ) ;
  /////////////////////////////////////////////////////////
  T -> setIconSize                  ( QSize ( 64 , 64 ) ) ;
  T -> setAutoRaise                 ( true              ) ;
  A -> setIcon                      ( icon              ) ;
  A -> setToolTip                   ( text              ) ;
  T -> setDefaultAction             ( A                 ) ;
  /////////////////////////////////////////////////////////
  MapOrders  [ Catalog . count ( ) ] = Id                 ;
  Catalog    [ Id                  ] = T                  ;
  CatalogMap [ A                   ] = Id                 ;
  MapCatalog [ Id                  ] = A                  ;
  Actions                           << A                  ;
  /////////////////////////////////////////////////////////
  nConnect ( T    , SIGNAL ( triggered(QAction*) )        ,
             this , SLOT   ( activate (QAction*) )      ) ;
}

void N::CiosPanel::activate(QAction * a)
{
  if ( ! Actions . contains ( a ) ) return        ;
  SUID Id = CatalogMap  [ a                     ] ;
  Alert                 ( Action                ) ;
  a    -> setEnabled    ( false                 ) ;
  plan -> processEvents (                       ) ;
  catalog               ( Id , a -> toolTip ( ) ) ;
  a    -> setEnabled    ( true                  ) ;
}

bool N::CiosPanel::Load(void)
{
  PictureManager PM ( plan        )                                          ;
  SqlConnection  SC ( plan -> sql )                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )                                        {
    UUIDs Branons                                                            ;
    SUID  uuid                                                               ;
    QString Q                                                                ;
    Q = plan -> sql . SelectFrom                                             (
          "uuid"                                                             ,
          table                                                              ,
          "where user = :USER and host = :HOST order by position asc ;"    ) ;
    SC . Prepare ( Q                                      )                  ;
    SC . Bind    ( "user" , plan -> UserUuid              )                  ;
    SC . Bind    ( "host" , plan -> Hostname . toUtf8 ( ) )                  ;
    if ( SC . Exec ( ) ) while ( SC . Next ( ) ) Branons << SC . Uuid ( 0 )  ;
    //////////////////////////////////////////////////////////////////////////
    foreach ( uuid , Branons )                                               {
      SUID    icon                                                           ;
      SUID    nuid                                                           ;
      QString name                                                           ;
      Q = plan -> sql . SelectFrom                                           (
            "`icon`,`name`"                                                  ,
            table                                                            ,
            SC.WhereUuid(uuid)                                             ) ;
      if ( SC . Fetch ( Q ) )                                                {
        QImage * image                                                       ;
        QIcon    Icon                                                        ;
        icon  = SC . Uuid(0)                                                 ;
        nuid  = SC . Uuid(1)                                                 ;
        name  = SC . getName                                                 (
                       PlanTable(Names)                                      ,
                       "uuid"                                                ,
                       plan -> LanguageId                                    ,
                       nuid                                                ) ;
        image = PM . Thumb ( SC , icon )                                     ;
        if ( NULL != image )                                                 {
          Icon  = QIcon ( QPixmap::fromImage ( *image ) )                    ;
          delete image                                                       ;
        }                                                                    ;
        add ( uuid , name , Icon )                                           ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    SC . close  ( )                                                          ;
  }                                                                          ;
  SC   . remove ( )                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

void N::CiosPanel::Initialize(void)
{
  for (int i = 0 ; i < 6 ; i++ ) Buttons [ i ]  = new QToolButton ( this )   ;
  for (int i = 0 ; i < 6 ; i++ ) Buttons [ i ] -> setAutoRaise    ( true )   ;
  ////////////////////////////////////////////////////////////////////////////
  Buttons [ 0 ] -> setIcon     ( QIcon(":/images/close.png")               ) ;
  Buttons [ 0 ] -> setIconSize ( QSize(64,64)                              ) ;
  Buttons [ 0 ] -> setToolTip  ( tr("Close all and quit")                  ) ;
  Buttons [ 1 ] -> setIcon     ( QIcon(":/images/hidewindow.png")          ) ;
  Buttons [ 1 ] -> setIconSize ( QSize(64,64)                              ) ;
  Buttons [ 1 ] -> setToolTip  ( tr("Hide")                                ) ;
  Buttons [ 2 ] -> setIcon     ( QIcon(":/images/GoRight.png")             ) ;
  Buttons [ 2 ] -> setIconSize ( QSize(64,64)                              ) ;
  Buttons [ 2 ] -> setToolTip  ( tr("Next")                                ) ;
  Buttons [ 3 ] -> setIcon     ( QIcon(":/images/GoLeft.png")              ) ;
  Buttons [ 3 ] -> setIconSize ( QSize(64,64)                              ) ;
  Buttons [ 3 ] -> setToolTip  ( tr("Previous")                            ) ;
  Buttons [ 4 ] -> setIcon     ( QIcon(":/images/first.png")               ) ;
  Buttons [ 4 ] -> setIconSize ( QSize(16,16)                              ) ;
  Buttons [ 4 ] -> setToolTip  ( tr("Start")                               ) ;
  Buttons [ 5 ] -> setIcon     ( QIcon(":/images/end.png")                 ) ;
  Buttons [ 5 ] -> setIconSize ( QSize(16,16)                              ) ;
  Buttons [ 5 ] -> setToolTip  ( tr("End")                                 ) ;
  ////////////////////////////////////////////////////////////////////////////
  nConnect ( Buttons [ 0 ] , SIGNAL(clicked()) , this , SLOT( Finish  () ) ) ;
  nConnect ( Buttons [ 1 ] , SIGNAL(clicked()) , this , SLOT( Evacuate() ) ) ;
  nConnect ( Buttons [ 2 ] , SIGNAL(clicked()) , this , SLOT( Right   () ) ) ;
  nConnect ( Buttons [ 3 ] , SIGNAL(clicked()) , this , SLOT( Left    () ) ) ;
  nConnect ( Buttons [ 4 ] , SIGNAL(clicked()) , this , SLOT( First   () ) ) ;
  nConnect ( Buttons [ 5 ] , SIGNAL(clicked()) , this , SLOT( End     () ) ) ;
  ////////////////////////////////////////////////////////////////////////////
  setToolTip ( tr("CIOS") )                                                  ;
  for (int i = 0 ; i < 6 ; i++ ) Buttons [ i ] -> show ( )                   ;
  ////////////////////////////////////////////////////////////////////////////
  plan -> settings . beginGroup ( "Panel" )                                  ;
  if ( plan -> settings . contains ( "StartId" ) )                           {
    StartId = plan -> settings . value ( "StartId" ) . toInt ( )             ;
  }                                                                          ;
  plan -> settings . endGroup ( )                                            ;
  Display ( )                                                                ;
}

void N::CiosPanel::Display(void)
{
  if ( 6 == Buttons . count ( ) )                                            {
    int w = width           (             )                                  ;
    Buttons [ 0 ] -> move   ( w - 32 , 32 )                                  ;
    Buttons [ 0 ] -> resize (     32 , 32 )                                  ;
    Buttons [ 1 ] -> move   ( w - 32 ,  0 )                                  ;
    Buttons [ 1 ] -> resize (     32 , 32 )                                  ;
    Buttons [ 2 ] -> move   ( w - 48 ,  0 )                                  ;
    Buttons [ 2 ] -> resize (     16 , 48 )                                  ;
    Buttons [ 3 ] -> move   (      0 ,  0 )                                  ;
    Buttons [ 3 ] -> resize (     16 , 48 )                                  ;
    Buttons [ 4 ] -> move   (      0 , 48 )                                  ;
    Buttons [ 4 ] -> resize (     16 , 16 )                                  ;
    Buttons [ 5 ] -> move   ( w - 48 , 48 )                                  ;
    Buttons [ 5 ] -> resize (     16 , 16 )                                  ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( Catalog . count ( ) > 0 )                                             {
    int id  = StartId                                                        ;
    int eid = id + 5                                                         ;
    if ( eid > Catalog . count ( ) ) eid = Catalog . count ( )               ;
    for (int i = 0 ; i < Catalog . count ( ) ; i++ )                         {
      SUID u = MapOrders [ i ]                                               ;
      if ( ( id <= i ) && ( i < eid ) )                                      {
        int s = i - id                                                       ;
        Catalog [ u ] -> setVisible ( true                 )                 ;
        Catalog [ u ] -> move       ( 16 + ( s * 64 ) ,  0 )                 ;
        Catalog [ u ] -> resize     ( 64              , 64 )                 ;
      } else                                                                 {
        Catalog [ u ] -> setVisible ( false                )                 ;
      }                                                                      ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  Buttons [ 3 ] -> setEnabled ( ! ( 0 == StartId )                         ) ;
  Buttons [ 4 ] -> setEnabled ( ! ( 0 == StartId )                         ) ;
  Buttons [ 2 ] -> setEnabled ( ! ( ( Catalog.count ( ) - StartId ) <= 5 ) ) ;
  Buttons [ 5 ] -> setEnabled ( ! ( ( Catalog.count ( ) - StartId ) <= 5 ) ) ;
}

void N::CiosPanel::IdUpdated(void)
{
  if ( ! IdChanged                         ) return ;
  if (   Lastest . secsTo ( nTimeNow ) < 5 ) return ;
  shutdown ( )                                      ;
  IdChanged = false                                 ;
  Lastest   = nTimeNow                              ;
}

void N::CiosPanel::shutdown(void)
{
  plan -> settings . beginGroup ( "Panel"             ) ;
  plan -> settings . setValue   ( "StartId" , StartId ) ;
  plan -> settings . endGroup   (                     ) ;
}

void N::CiosPanel::catalog(SUID u,QString name)
{
  Finished ( )                                                               ;
  ////////////////////////////////////////////////////////////////////////////
  int  total = 0                                                             ;
  int  type  = 0                                                             ;
  bool go    = false                                                         ;
  FriendUuid . clear ( )                                                     ;
  SqlConnection  SC ( plan -> sql )                                          ;
  if ( SC . open ( FunctionString ) )                                        {
    QString Q                                                                ;
    //////////////////////////////////////////////////////////////////////////
    Q = SC . sql . SelectFrom                                                (
          "count(*)"                                                         ,
          panel                                                              ,
          QString("where `%1` = %2").arg(item).arg(u)                      ) ;
    if ( SC . Fetch ( Q ) ) total = SC . Int ( 0 )                           ;
    Q = SC . sql . SelectFrom                                                (
          "type"                                                             ,
          table                                                              ,
          SC . WhereUuid ( u )                                             ) ;
    if ( SC . Fetch ( Q ) ) type = SC . Int ( 0 )                            ;
    //////////////////////////////////////////////////////////////////////////
    SC . close  ( )                                                          ;
  }                                                                          ;
  SC   . remove ( )                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( 2    == type ) go = true                                              ;
  if ( total > 0    ) go = true                                              ;
  if ( ! go         ) return                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  QRect  R = rect        (   )                                               ;
  QPoint P = R . topLeft (   )                                               ;
  P        = mapToGlobal ( P )                                               ;
  QRect  Z ( P . x ( ) , P . y ( ) , R . width ( ) , R . height ( ) )        ;
  plan -> Talk ( name )                                                      ;
  ////////////////////////////////////////////////////////////////////////////
  ToolWrapper  * w                                                           ;
  CiosFloating * Floating                                                    ;
  w         = new ToolWrapper  ( NULL , plan    )                            ;
  Floating  = new CiosFloating ( w    , plan    )                            ;
  Floating -> setWrapper       ( w              )                            ;
  Floating -> setWindowTitle   ( name           )                            ;
  Floating -> setWindowIcon    ( windowIcon ( ) )                            ;
  Floating -> setToolTip       ( name           )                            ;
  Floating -> setTable         ( panel          )                            ;
  Floating -> setItem          ( item           )                            ;
  Floating -> opacity = opacity                                              ;
  Floating -> assign ( u , type , Z , FriendIP . count ( ) )                 ;
  ////////////////////////////////////////////////////////////////////////////
  if ( ( 2 == type ) && ( FriendIP . count ( ) > 0 ) )                       {
    QStringList Hosts = FriendIP . keys ( )                                  ;
    QString     host                                                         ;
    SUID        user = 0xFFFFFFFFFFFFFFFELL                                  ;
    foreach ( host , Hosts )                                                 {
      Floating -> add                                                        (
        user                                                                 ,
        host                                                                 ,
        QIcon(":/images/remoteuser.png")                                   ) ;
      FriendUuid [ user ] = host                                             ;
      user --                                                                ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  nConnect ( Floating , SIGNAL ( Execute  ( SUID , QString ) )               ,
             this     , SLOT   ( Execute  ( SUID , QString ) )             ) ;
  Floatings << Floating                                                      ;
  Floating  -> Arrangement ( )                                               ;
}

void N::CiosPanel::Execute(SUID u,QString name)
{
  Finished ( )                             ;
  //////////////////////////////////////////
  if ( FriendUuid . contains ( u ) )       {
    QString Host = FriendUuid [ u    ]     ;
    QString IP   = FriendIP   [ Host ]     ;
    int     Port = FriendPort [ Host ]     ;
    emit RemoteUser ( Host , IP , Port )   ;
    return                                 ;
  }                                        ;
  emit Hide ( )                            ;
  //////////////////////////////////////////
  bool      success = false                ;
  Execution ne                             ;
  SqlConnection  SC ( plan -> sql )        ;
  if ( SC . open ( FunctionString ) )      {
    ne . clear          (                ) ;
    success = ne . Load ( plan , SC , u  ) ;
    SC . close          (                ) ;
  }                                        ;
  SC   . remove         (                ) ;
  if ( ! success ) return                  ;
  //////////////////////////////////////////
  plan -> Talk          ( name           ) ;
  plan -> showMessage   ( name           ) ;
  ne    . Run           (                ) ;
}
