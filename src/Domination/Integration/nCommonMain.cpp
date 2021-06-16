#include <domination.h>

N::CommonMain:: CommonMain    ( int argc , char ** argv , QWidget * parent )
              : QMainWindow   (                                     parent )
              , VirtualGui    ( this , &MainPlan                           )
              , Thread        ( 0    , false                               )
              , Tray          ( NULL                                       )
              , Stacked       ( NULL                                       )
              , Mdi           ( NULL                                       )
              , Debug         ( NULL                                       )
              , AttachMode    ( 0                                          )
              , Ready         ( false                                      )
              , DockBar       ( NULL                                       )
              , CliDock       ( NULL                                       )
              , InputVolume   ( NULL                                       )
              , OutputVolume  ( NULL                                       )
              , torrentWidget ( NULL                                       )

{
  WidgetClass                                                      ;
  N::qPlan = &MainPlan                                             ;
  owFlags  = windowFlags (             )                           ;
  MainPlan . setObject   ( this        )                           ;
  plan    -> arguments   ( argc , argv )                           ;
  plan    -> setMain     ( this        )                           ;
  plan    -> setWidget   ( this        )                           ;
  if ( NotNull ( plan -> Neighbors ) )                             {
    nConnect ( plan->Neighbors , SIGNAL ( Changed          ( ) )   ,
               this            , SLOT   ( NeighborsChanged ( ) ) ) ;
  }                                                                ;
  addConnector  ( "WaitCursor"                                     ,
                  this      , SIGNAL ( assignWaitCursor  ( ) )     ,
                  this      , SLOT   ( setWaitCursor     ( ) )   ) ;
  addConnector  ( "ArrowCursor"                                    ,
                  this      , SIGNAL ( assignArrowCursor ( ) )     ,
                  this      , SLOT   ( setArrowCursor    ( ) )   ) ;
  onlyConnector ( "WaitCursor"                                   ) ;
  onlyConnector ( "ArrowCursor"                                  ) ;
}

N::CommonMain::~CommonMain (void)
{
}

void N::CommonMain::focusInEvent(QFocusEvent * event)
{
  if (FocusIn ()) event->accept   (     ) ;
  else QMainWindow::focusInEvent  (event) ;
}

void N::CommonMain::focusOutEvent(QFocusEvent * event)
{
  if (FocusOut()) event->accept   (     ) ;
  else QMainWindow::focusOutEvent (event) ;
}

void N::CommonMain::showEvent(QShowEvent * event)
{
  QMainWindow::showEvent(event)             ;
  if (!isVisible ( )) plan->Visible = false ; else
  if (isHidden   ( )) plan->Visible = false ; else
  if (isMinimized( )) plan->Visible = false ; else
                      plan->Visible = true  ;
  setVisibility     ( plan->Visible )       ;
}

void N::CommonMain::closeEvent(QCloseEvent * e)
{
  if (Shutdown()) e -> accept ( ) ; else
  QMainWindow::closeEvent ( e )   ;
}

void N::CommonMain::setVisibility(bool visibility)
{ Q_UNUSED ( visibility ) ;
}

void N::CommonMain::obtainFocus(bool focus)
{ Q_UNUSED ( focus ) ;
}

void N::CommonMain::Quit(void)
{
  nDropOut   ( ! Shutdown ( ) ) ;
  ForceClose (                ) ;
}

void N::CommonMain::ForceClose(void)
{
  qApp -> closeAllWindows ( ) ;
  qApp -> quit            ( ) ;
}

void N::CommonMain::MountTray(void)
{
  nDropOut ( NotNull(Tray) )                                       ;
  Tray  = new QSystemTrayIcon ( this         )                     ;
  Tray -> setIcon             ( windowIcon() )                     ;
  Tray -> show                (              )                     ;
  //////////////////////////////////////////////////////////////////
  nConnect                                                          (
    Tray , SIGNAL (activated(QSystemTrayIcon::ActivationReason))   ,
    this , SLOT   (TrayClick(QSystemTrayIcon::ActivationReason)) ) ;
}

void N::CommonMain::TrayClick(QSystemTrayIcon::ActivationReason reason)
{ Q_UNUSED ( reason )                 ;
  #ifdef ACTUALLY_THIS_IS_A_DEMOSTRATION_CODE_FOR_YOU_TO_COPY_ELSEWHERE
  switch (reason)                     {
    case QSystemTrayIcon::Unknown     :
      TrayUnknown       ( )           ;
    break                             ;
    case QSystemTrayIcon::Context     :
      TrayMenu          ( )           ;
    break                             ;
    case QSystemTrayIcon::DoubleClick :
      TrayDoubleClicked ( )           ;
    break                             ;
    case QSystemTrayIcon::Trigger     :
      TrayTrigger       ( )           ;
    break                             ;
    case QSystemTrayIcon::MiddleClick :
      TrayMiddleClicked ( )           ;
    break                             ;
  }                                   ;
  #endif
}

void N::CommonMain::TrayMenu(void)
{
}

void N::CommonMain::TrayUnknown(void)
{
}

void N::CommonMain::TrayDoubleClicked(void)
{
}

void N::CommonMain::TrayTrigger(void)
{
}

void N::CommonMain::TrayMiddleClicked(void)
{
}

void N::CommonMain::resizeEvent(QResizeEvent * event)
{
  QMainWindow :: resizeEvent ( event )     ;
  if (NotNull(plan)) plan->screen.Detect() ;
}

void N::CommonMain::connectDock(QWidget * widget)
{
  nMountSlot(widget,this,attachDock(QWidget*,QString,Qt::DockWidgetArea,Qt::DockWidgetAreas)) ;
  nMountSlot(widget,this,attachMdi (QWidget*,int                                           )) ;
}

void N::CommonMain::attachDock(QWidget * widget,QString title,Qt::DockWidgetArea area,Qt::DockWidgetAreas areas)
{
  QMdiSubWindow   * mdi  = Casting ( QMdiSubWindow   , widget->parent() ) ;
  N::TreeDock     * tree = Casting ( N::TreeDock     , widget           ) ;
  N::ListDock     * list = Casting ( N::ListDock     , widget           ) ;
  N::DebugView    * dbgs = Casting ( N::DebugView    , widget           ) ;
  N::ControlPad   * cpad = Casting ( N::ControlPad   , widget           ) ;
  N::SourcesView  * srcs = Casting ( N::SourcesView  , widget           ) ;
  N::ProjectsView * pros = Casting ( N::ProjectsView , widget           ) ;
  if (NotNull(mdi )) mdi  -> deleteLater (                              ) ;
  if (NotNull(tree)) tree -> Docking     ( this , title , area , areas  ) ;
  if (NotNull(list)) list -> Docking     ( this , title , area , areas  ) ;
  if (NotNull(dbgs)) dbgs -> Docking     ( this , title , area , areas  ) ;
  if (NotNull(cpad)) cpad -> Docking     ( this , title , area , areas  ) ;
  if (NotNull(srcs)) srcs -> Docking     ( this , title , area , areas  ) ;
  if (NotNull(pros)) pros -> Docking     ( this , title , area , areas  ) ;
}

void N::CommonMain::attachMdi(QWidget * widget,int direction)
{
  N::TreeDock     * tree = Casting ( N::TreeDock     , widget ) ;
  N::ListDock     * list = Casting ( N::ListDock     , widget ) ;
  N::DebugView    * dbgs = Casting ( N::DebugView    , widget ) ;
  N::ControlPad   * cpad = Casting ( N::ControlPad   , widget ) ;
  N::SourcesView  * srcs = Casting ( N::SourcesView  , widget ) ;
  N::ProjectsView * pros = Casting ( N::ProjectsView , widget ) ;
  N::AttachDock   * dbgd = (N::AttachDock *) dbgs               ;
  N::AttachDock   * srcd = (N::AttachDock *) srcs               ;
  N::AttachDock   * prod = (N::AttachDock *) pros               ;
  switch (direction)                                            {
    case 0                                                      :
      widget -> setParent ( Mdi                )                ;
      Mdi    -> Attach    ( widget             )                ;
      widget -> show      (                    )                ;
    break                                                       ;
    case Qt::Vertical                                           :
    case Qt::Horizontal                                         :
      widget -> setParent ( Mdi                )                ;
      Mdi    -> Attach    ( widget , direction )                ;
      widget -> show      (                    )                ;
    break                                                       ;
    default                                                     :
    break                                                       ;
  }                                                             ;
  if ( NotNull ( tree ) ) tree -> Detach ( this )               ;
  if ( NotNull ( list ) ) list -> Detach ( this )               ;
  if ( NotNull ( dbgs ) ) dbgd -> Detach ( this )               ;
  if ( NotNull ( cpad ) ) cpad -> Detach ( this )               ;
  if ( NotNull ( srcs ) ) srcd -> Detach ( this )               ;
  if ( NotNull ( pros ) ) prod -> Detach ( this )               ;
}

void N::CommonMain::deleteLater(QWidget * widget)
{
  nDropOut ( IsNull(widget) )                      ;
  QWidget       * pwx = widget->parentWidget()     ;
  QMdiSubWindow * msw = Casting(QMdiSubWindow,pwx) ;
  if (NotNull(msw)) msw    -> deleteLater ( )      ;
               else widget -> deleteLater ( )      ;
}

void N::CommonMain::InstallStackMdi(void)
{
  Stacked  = new StackedWidget ( this    , plan ) ;
  setCentralWidget             ( Stacked        ) ;
  Mdi      = new MdiArea       ( Stacked , plan ) ;
  Stacked -> addWidget         ( Mdi            ) ;
  Stacked -> setCurrentWidget  ( Mdi            ) ;
  AttachMode = 2                                  ;
  connect(Mdi ,SIGNAL(childChanged())             ,
          this,SLOT  (ChildChanged())           ) ;
  connect(Mdi ,SIGNAL(Files      (QList<QUrl>))   ,
          this,SLOT  (acceptFiles(QList<QUrl>)) ) ;
}

QWidget * N::CommonMain::Container(void)
{
  switch (AttachMode)      {
    case 1: return Stacked ;
    case 2: return Mdi     ;
  }                        ;
  return this              ;
}

void N::CommonMain::Parking(QWidget * widget,int alignment)
{
  switch (AttachMode)                                    {
    case 1                                               :
      Stacked -> addWidget        ( widget             ) ;
      Stacked -> setCurrentWidget ( widget             ) ;
    break                                                ;
    case 2                                               :
      Mdi     -> Attach           ( widget , alignment ) ;
      widget  -> show             (                    ) ;
    break                                                ;
  }                                                      ;
}

void N::CommonMain::ApplySql(QWidget * widget,Sql * sql)
{
  plan -> settings . SaveSqlSettings ( "SQL" , *sql ) ;
  plan -> sql      = *sql                             ;
  deleteLater ( widget )                              ;
}

void N::CommonMain::InstallDecisions(void)
{
  decisions . Blank ( GuiMainId ) ;
}

void N::CommonMain::InstallFonts(double defaultSize)
{
  CUIDs FIDs                                         ;
  int   fid                                          ;
  FIDs << N::Fonts::Default                          ;
  FIDs << N::Fonts::Menu                             ;
  FIDs << N::Fonts::Editor                           ;
  FIDs << N::Fonts::Tree                             ;
  FIDs << N::Fonts::ToolTip                          ;
  FIDs << N::Fonts::Status                           ;
  FIDs << N::Fonts::Message                          ;
  FIDs << N::Fonts::ComboBox                         ;
  FIDs << N::Fonts::TreeView                         ;
  FIDs << N::Fonts::ListView                         ;
  FIDs << N::Fonts::TableView                        ;
  FIDs << N::Fonts::Label                            ;
  FIDs << N::Fonts::CheckBox                         ;
  FIDs << N::Fonts::Progress                         ;
  FIDs << N::Fonts::Button                           ;
  FIDs << N::Fonts::Spin                             ;
  FIDs << N::Fonts::IPA                              ;
  FIDs << N::Fonts::Math                             ;
  FIDs << N::Fonts::User                             ;
  FIDs << N::Fonts::Class                            ;
  FIDs << N::Fonts::Inherit                          ;
  FIDs << N::Fonts::Keyword                          ;
  FIDs << N::Fonts::Specifier                        ;
  FIDs << N::Fonts::Qualifier                        ;
  FIDs << N::Fonts::Member                           ;
  FIDs << N::Fonts::Function                         ;
  FIDs << N::Fonts::Symbol                           ;
  FIDs << N::Fonts::Requirement                      ;
  FIDs << N::Fonts::Comment                          ;
  foreach (fid,FIDs)                                 {
    QString FID = QString("Font %1").arg(fid)        ;
    plan->fonts[fid]    = plan->font                 ;
    plan->fonts[fid].cm = defaultSize                ;
    plan->settings.LoadFont(FID,plan->fonts[fid])    ;
    plan->fonts[fid].setScreen(plan->screen)         ;
  }                                                  ;
}

bool N::CommonMain::DeployPlan (
       int     Language        ,
       QString Username        ,
       QString Settings        ,
       double  defaultSize     ,
       bool    plugin          ,
       bool    progress        )
{
  if (plugin)                                  {
    plan -> addPlugin ( "Plugins"  )           ;
  }                                            ;
  plan -> LanguageId = Language                ;
  plan -> settings   . Username = Username     ;
  plan -> setSettings ( Settings   )           ;
  if (plugin)                                  {
    plan->setLibraryPaths()                    ;
  }                                            ;
  plan -> Initialize  (            )           ;
  N::InstallTables    ( *plan      )           ;
  setFont             ( plan->font )           ;
  if (NotNull(plan->status))                   {
    plan->status->setFont(plan->font)          ;
  }                                            ;
  InstallFonts    ( defaultSize )              ;
  plan -> setFont ( this , N::Fonts::Default ) ;
  plan -> setFont ( this                     ) ;
  InstallLocal    (                          ) ;
  InstallExtras   ( progress                 ) ;
  InstallDirs     (                          ) ;
  AttachActions   (                          ) ;
  InstallIcons    ( 0                        ) ;
  return true                                  ;
}

void N::CommonMain::InstallDirs(void)
{
  #define DPATH(NAME,V)                                      \
    plan -> Dirs    [ Directory::NAME ] = plan->Path ( V ) ; \
    plan -> setPath ( Directory::NAME , "Directory" , V  )
  DPATH ( Documents   , "Documents"                        ) ;
  DPATH ( Images      , "Images"                           ) ;
  DPATH ( Sounds      , "Sounds"                           ) ;
  DPATH ( Download    , "Download"                         ) ;
  DPATH ( Upload      , "Upload"                           ) ;
  DPATH ( History     , "History"                          ) ;
  DPATH ( Development , "Development"                      ) ;
  DPATH ( Plugins     , "Plugins"                          ) ;
  DPATH ( Player      , "SMPlayer"                         ) ;
  DPATH ( Web         , "Web"                              ) ;
  DPATH ( Scripts     , "Scripts"                          ) ;
  DPATH ( Qt          , "Qt"                               ) ;
  DPATH ( MetaTrader  , "MetaTrader"                       ) ;
  DPATH ( Include     , "Development/ZOS/include"          ) ;
  DPATH ( Help        , "Help"                             ) ;
  DPATH ( Backup      , "Backup"                           ) ;
  DPATH ( Android     , "Android"                          ) ;
  DPATH ( Temporary   , "Temp"                             ) ;
  DPATH ( Users       , "Users"                            ) ;
  DPATH ( Binary      , "Bin"                              ) ;
  #undef  DPATH
}

bool N::CommonMain::InstallIcons(int style)
{
  #define AI(TYPE,icon) plan -> addIcon ( Types::TYPE ,1,0,icon )
  AI ( Album      , QIcon(":/images/videos.png"     ) )         ;
  AI ( URL        , QIcon(":/images/bookmarks.png"  ) )         ;
  AI ( Gallery    , QIcon(":/images/gallery.png"    ) )         ;
  AI ( ColorGroup , QIcon(":/images/colorgroups.png") )         ;
  AI ( Palette    , QIcon(":/images/palettes.png"   ) )         ;
  AI ( Document   , QIcon(":/images/folder.png"     ) )         ;
  AI ( Eyes       , QIcon(":/images/eyeliner.png"   ) )         ;
  AI ( EyesShape  , QIcon(":/images/eyeshadows.png" ) )         ;
  AI ( Hairs      , QIcon(":/images/eyeshadower.png") )         ;
  AI ( FaceShape  , QIcon(":/images/faces.png"      ) )         ;
  return true                                                   ;
  #undef  AI
}

void N::CommonMain::InstallLocal(void)
{
  plan->site.SQL      = new N::Sql()   ;
  (*(plan->site.SQL)) = plan->sql      ;
  plan->site.Table    = "settings"     ;
  plan->site.Username = plan->Hostname ;
  plan->site.Initialize ( )            ;
  plan->LoadSupports(plan->languages)  ;
}

void N::CommonMain::InstallExtras(bool progress)
{
  QProgressBar * P = NULL                                          ;
  if (progress)                                                    {
    P = plan->Progress(tr("Initialize %v/%m"))                     ;
  }                                                                ;
  nIfSafe(P)                                                       {
    P  -> setTextVisible (false           )                        ;
    P  -> setRange       (0,21+Fonts::User) ; DoProcessEvents      ;
    P  -> setValue       ( 5              ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> loadPaper      ("papersize"     )                        ;
  nIfSafe(P)                                                       {
    P  -> setValue       ( 6              ) ; DoProcessEvents      ;
    P  -> setValue       ( 7+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> AudioDepot     ("AudioDepot"    )                        ;
  plan -> VideoDepot     ("VideoDepot"    )                        ;
  nIfSafe(P)                                                       {
    P  -> setValue       ( 9+Fonts::User  ) ; DoProcessEvents      ;
    P  -> setValue       (10+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> Audio.setParent(this            )                        ;
  nIfSafe(P)                                                       {
    P  -> setValue       (11+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> Recorder.setParent(this         )                        ;
  nIfSafe(P)                                                       {
    P  -> setValue       (12+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> addSound       (Command::Wrong   ,"Wrong"   ,"Wrong"   ) ;
  nIfSafe(P)                                                       {
    P  -> setValue       (13+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> addSound       (Command::Click   ,"Click"   ,"Click"   ) ;
  nIfSafe(P)                                                       {
    P  -> setValue       (14+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> addSound       (Command::Selected,"Selected","Selected") ;
  nIfSafe(P)                                                       {
    P  -> setValue       (15+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> addSound       (Command::Menu    ,"Menu"    ,"Menu"    ) ;
  nIfSafe(P)                                                       {
    P  -> setValue       (16+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> addSound       (Command::Error   ,"Error"   ,"Error"   ) ;
  nIfSafe(P)                                                       {
    P  -> setValue       (17+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> addSound       (Command::Done    ,"Done"    ,"Done"    ) ;
  nIfSafe(P)                                                       {
    P  -> setValue       (18+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> addSound       (Command::Action  ,"Action"  ,"Action"  ) ;
  plan -> settings . beginGroup ( "Scheduler" )                    ;
  plan -> Uuids [ "Organization" ]                                 =
    plan -> settings . value ( "Organization" ) . toULongLong ( )  ;
  nIfSafe(P)                                                       {
    P  -> setValue       (19+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> Uuids [ "Crowd" ]                                        =
    plan -> settings . value ( "Crowd"        ) . toULongLong ( )  ;
  nIfSafe(P)                                                       {
    P  -> setValue       (20+Fonts::User  ) ; DoProcessEvents      ;
  }                                                                ;
  plan -> settings . endGroup   (             )                    ;
  nIfSafe(P)                                                       {
    P  -> setValue       (21+Fonts::User  ) ; DoProcessEvents      ;
    P  -> hide           (                )                        ;
    P  -> deleteLater    (                )                        ;
  }                                                                ;
}

QList<QMenu *> N::CommonMain::MainMenus (void)
{
  QMenuBar * m = menuBar()                   ;
  QList<QMenu *> Menus                       ;
  if (IsNull(m)) return Menus                ;
  QObjectList QOList                         ;
  QObject * olist                            ;
  QOList = m->children()                     ;
  foreach (olist,QOList)                     {
    QMenu * v = qobject_cast<QMenu *>(olist) ;
    if (NotNull(v))                          {
      QObjectList qo = v->children()         ;
      Menus << v                             ;
      if (qo.count()>0)                      {
        Menus << SubMenus(v)                 ;
      }                                      ;
    }                                        ;
  }                                          ;
  return Menus                               ;
}

QList<QMenu *> N::CommonMain::SubMenus(QObject * parent)
{
  QList<QMenu *> Menus                       ;
  QMenu       *  m                           ;
  m = qobject_cast<QMenu *>(parent)          ;
  if (IsNull(m)) return Menus                ;
  QObjectList QOList                         ;
  QObject * olist                            ;
  QOList = m->children()                     ;
  foreach (olist,QOList)                     {
    QMenu * v = qobject_cast<QMenu *>(olist) ;
    if (NotNull(v))                          {
      QObjectList qo = v->children()         ;
      Menus << v                             ;
      if (qo.count()>0)                      {
        Menus << SubMenus(v)                 ;
      }                                      ;
    }                                        ;
  }                                          ;
  return Menus                               ;
}

void N::CommonMain::Maximized(void)
{
  showMaximized  ( ) ;
}

void N::CommonMain::Minimized(void)
{
  showMinimized  ( ) ;
}

void N::CommonMain::FullScreen(void)
{
  showFullScreen ( ) ;
}

void N::CommonMain::NormalWindow(void)
{
  showNormal     ( ) ;
}

void N::CommonMain::DockTray(void)
{
}

void N::CommonMain::Engross(bool shown)
{
  if (shown)             {
    FullScreen   (     ) ;
    DisplayMenu  (false) ;
  } else                 {
    Maximized    (     ) ;
    DisplayMenu  (true ) ;
  }                      ;
}

void N::CommonMain::SubwindowView(void)
{
  nDropOut ( IsNull(Mdi)  ) ;
  Mdi -> Subwindow (      ) ;
}

void N::CommonMain::TabbedView(void)
{
  nDropOut ( IsNull(Mdi)  ) ;
  Mdi->Tabbed (           ) ;
}

void N::CommonMain::CascadeWindows(void)
{
  nDropOut ( IsNull(Mdi)  ) ;
  Mdi->Cascade (          ) ;
}

void N::CommonMain::TileWindows(void)
{
  nDropOut  ( IsNull(Mdi) ) ;
  Mdi->Tile (             ) ;
}

void N::CommonMain::ChildChanged(void)
{
}

void N::CommonMain::DisplayTitle(bool shown)
{
}

void N::CommonMain::DisplayMenu(bool shown)
{
  nDropOut ( IsNull ( menuBar() ) )     ;
  menuBar   ( ) -> setVisible ( shown ) ;
}

void N::CommonMain::DisplayStatus(bool shown)
{
  nDropOut ( IsNull ( statusBar() ) )   ;
  statusBar ( ) -> setVisible ( shown ) ;
}

void N::CommonMain::AttachActions(void)
{
}

bool N::CommonMain::prepare(void)
{
  return true ;
}

bool N::CommonMain::configure(void)
{
  return true ;
}

bool N::CommonMain::loading(void)
{
  return true ;
}

bool N::CommonMain::brand(void)
{
  return true ;
}

bool N::CommonMain::advertising(void)
{
  return true ;
}

bool N::CommonMain::primary(void)
{
  return true ;
}

void N::CommonMain::DockDebugger(void)
{
  nDropOut ( plan -> Verbose < EnableDebugger )             ;
  ///////////////////////////////////////////////////////////
  DebugView * Debugger = new DebugView ( this , plan )      ;
  Debugger -> Trigger  = NULL                               ;
  Debugger -> Scope    = "Debugger"                         ;
  Debugger -> Docking                                       (
    this , tr("Debug window")                               ,
    Qt::BottomDockWidgetArea                                ,
    Qt::TopDockWidgetArea                                   |
    Qt::BottomDockWidgetArea                              ) ;
  plan     -> debugWidget       = Debugger                  ;
  Debugger -> Data . Controller = & ( plan -> canContinue ) ;
  ///////////////////////////////////////////////////////////
  Debugger -> DockIn  ( false                             ) ;
  connectDock         ( Debugger                          ) ;
  Debugger -> Startup (                                   ) ;
}

void N::CommonMain::MountProgress(void)
{
  plan -> progressManager  = new ProgressManager ( this , plan     ) ;
  plan -> progressManager -> Trigger = NULL                          ;
  plan -> progressManager -> Scope   = "ProgressManager"             ;
  plan -> progressManager -> Docking    ( this                       ,
                                          tr("Progress Manager")     ,
                                          Qt::BottomDockWidgetArea   ,
                                          Qt::TopDockWidgetArea      |
                                          Qt::BottomDockWidgetArea ) ;
  plan -> progressManager -> DockIn     ( false                    ) ;
  DoProcessEvents                                                    ;
  connectDock                           ( plan -> progressManager  ) ;
  DoProcessEvents                                                    ;
  plan -> progressManager -> StartFlush (                          ) ;
  DoProcessEvents                                                    ;
}

void N::CommonMain::StopTorrents(void)
{
  if (NotNull(torrentWidget))        {
    torrentWidget -> stopIt      ( ) ;
    torrentWidget -> deleteLater ( ) ;
    torrentWidget  = NULL            ;
  }                                  ;
}

void N::CommonMain::DockTorrent(QAction * action)
{
  FastParkIn                      (
    TorrentWidget                 ,
    torrentWidget                 ,
    this                          ,
    plan                          ,
    action                        ,
    "VideoTorrent"                ,
    tr("Torrent manager")         ,
    Qt::BottomDockWidgetArea      ,
    Qt::TopDockWidgetArea         |
    Qt::BottomDockWidgetArea    ) ;
  connectDock   ( torrentWidget ) ;
  torrentWidget -> Show ( false ) ;
}

void N::CommonMain::DockCLI(void)
{
  CliDock  = new N::ScrollDock ( this, plan               ) ;
  CliDock -> Docking           ( this                       ,
                                 tr("Command shell")        ,
                                 Qt::BottomDockWidgetArea   ,
                                 Qt::LeftDockWidgetArea     |
                                 Qt::RightDockWidgetArea    |
                                 Qt::TopDockWidgetArea      |
                                 Qt::BottomDockWidgetArea ) ;
  CliDock -> Show              ( false                    ) ;
}

void N::CommonMain::GoScript(SUID uuid)
{ Q_UNUSED ( uuid ) ;
}

void N::CommonMain::HelpDragDrop(void)
{
  setStatusBar               ( new DragDropStatusBar( this ) ) ;
  plan -> status = statusBar (                               ) ;
}

void N::CommonMain::setCpu(void)
{
  CPUs[0]      = new CpuLabel(NULL)              ;
  CPUs[0]     -> Flag = false                    ;
  CPUs[0]     -> setToolTip(tr("Program usage")) ;
  statusBar() -> addPermanentWidget(CPUs[0])     ;
  CPUs[1]      = new CpuLabel(NULL)              ;
  CPUs[1]     -> setToolTip(tr("Machine usage")) ;
  statusBar() -> addPermanentWidget(CPUs[1])     ;
  CPUs[0]     -> hide  ( )                       ;
  CPUs[1]     -> hide  ( )                       ;
  CPUs[0]     -> Start ( )                       ;
  CPUs[1]     -> Start ( )                       ;
}

void N::CommonMain::stopCpu(void)
{
  CUIDs IDs = CPUs . keys ( )           ;
  int   id                              ;
  foreach ( id , IDs )                  {
    if ( NULL != CPUs [ id ] )          {
      CPUs [ id ] -> Stop        (    ) ;
      CPUs [ id ] -> deleteLater (    ) ;
      CPUs . remove              ( id ) ;
    }                                   ;
  }                                     ;
}

void N::CommonMain::MachineUsage(bool shown)
{
  CPUs [ 1 ] -> setVisible ( shown ) ;
}

void N::CommonMain::ProgramUsage(bool shown)
{
  CPUs [ 0 ] -> setVisible ( shown ) ;
}

void N::CommonMain::VoiceOutput(bool enabled)
{
  plan->VoiceOutput = enabled ;
}

void N::CommonMain::ClearVoices(void)
{
  plan -> Mouth . Clear ( ) ;
}

void N::CommonMain::CameraInput(QMenu * menu)
{
  QActionGroup * g = N::Acoustics::DeviceMenu    (
                       plan                      ,
                       Acoustics::Camera         ,
                       menu                    ) ;
  if (IsNull(g)) return                          ;
  connect(g   ,SIGNAL(triggered    (QAction*))   ,
          this,SLOT  (CameraDevice (QAction*)) ) ;
}

void N::CommonMain::CameraDevice(QAction * device)
{
  if (IsNull(device)) return                                ;
  plan->Equipments[N::Plan::DefaultCamera] = device->text() ;
  CameraChanged ( )                                         ;
}

void N::CommonMain::CameraChanged(void)
{
}

void N::CommonMain::AudioOutput(QMenu * menu)
{
  QActionGroup * g = N::Acoustics::DeviceMenu    (
                       plan                      ,
                       Acoustics::Output         ,
                       menu                    ) ;
  if (IsNull(g)) return                          ;
  connect(g   ,SIGNAL(triggered    (QAction*))   ,
          this,SLOT  (OutputDevice (QAction*)) ) ;
}

void N::CommonMain::OutputDevice(QAction * device)
{
  if (IsNull(device)) return                                               ;
  plan->Equipments  [N::Plan::DefaultAudioOutput] = device->text()         ;
  plan->EquipmentIDs[N::Plan::DefaultAudioOutput] = device->data().toInt() ;
  OutputChanged ( )                                                        ;
}

void N::CommonMain::OutputChanged(void)
{
}

void N::CommonMain::AudioInput(QMenu * menu)
{
  QActionGroup * g = N::Acoustics::DeviceMenu  (
                       plan                    ,
                       N::Acoustics::Input     ,
                       menu                  ) ;
  if (IsNull(g)) return                        ;
  connect(g   ,SIGNAL(triggered  (QAction*))   ,
          this,SLOT  (InputDevice(QAction*)) ) ;
}

void N::CommonMain::InputDevice(QAction * device)
{
  if (IsNull(device)) return                                              ;
  plan->Equipments  [N::Plan::DefaultAudioInput] = device->text()         ;
  plan->EquipmentIDs[N::Plan::DefaultAudioInput] = device->data().toInt() ;
  InputChanged  ( )                                                       ;
}

void N::CommonMain::InputChanged(void)
{
}

void N::CommonMain::setupVolumes(void)
{
  int v = (int)( plan -> Audio . currentVolume ( ) / 100 )                   ;
  InputVolume  = new QSlider           ( Qt::Horizontal                    ) ;
  OutputVolume = new QSlider           ( Qt::Horizontal                    ) ;
  ////////////////////////////////////////////////////////////////////////////
  QFont          font = statusBar ( ) -> font ( )                            ;
  QSize          s    = statusBar ( ) -> size ( )                            ;
  QSize          m ( s . width ( ) / 6 , s . height ( ) )                    ;
  if ( font . pixelSize ( ) > 0 )                                            {
    m             . setHeight          ( font . pixelSize ( ) - 2          ) ;
    font          . setPixelSize       ( font . pixelSize ( ) - 2          ) ;
    InputVolume  -> setFont            ( font                              ) ;
    OutputVolume -> setFont            ( font                              ) ;
  }                                                                          ;
  InputVolume    -> setMinimumSize     ( m                                 ) ;
  InputVolume    -> setMaximumSize     ( m                                 ) ;
  OutputVolume   -> setMinimumSize     ( m                                 ) ;
  OutputVolume   -> setMaximumSize     ( m                                 ) ;
  ////////////////////////////////////////////////////////////////////////////
  statusBar ( )  -> addPermanentWidget ( InputVolume                       ) ;
  statusBar ( )  -> addPermanentWidget ( OutputVolume                      ) ;
  ////////////////////////////////////////////////////////////////////////////
  InputVolume    -> setRange           ( 0 , 100                           ) ;
  InputVolume    -> setTickInterval    ( 5                                 ) ;
  InputVolume    -> setTickPosition    ( QSlider::TicksBelow               ) ;
  InputVolume    -> setToolTip         ( tr("Audio input volume"  )        ) ;
  InputVolume    -> hide               (                                   ) ;
  OutputVolume   -> setRange           ( 0 , 100                           ) ;
  OutputVolume   -> setTickInterval    ( 5                                 ) ;
  OutputVolume   -> setTickPosition    ( QSlider::TicksBelow               ) ;
  OutputVolume   -> setToolTip         ( tr("Audio output  volume")        ) ;
  OutputVolume   -> setValue           ( v                                 ) ;
  OutputVolume   -> hide               (                                   ) ;
  ////////////////////////////////////////////////////////////////////////////
  nConnect ( InputVolume  , SIGNAL ( valueChanged        ( int ) )           ,
             this         , SLOT   ( VolumeInputChanged  ( int ) )         ) ;
  nConnect ( OutputVolume , SIGNAL ( valueChanged        ( int ) )           ,
             this         , SLOT   ( VolumeOutputChanged ( int ) )         ) ;
  ////////////////////////////////////////////////////////////////////////////
  DoProcessEvents                                                            ;
}

void N::CommonMain::ChangeInputVolume(bool visible)
{
  InputVolume  -> setVisible ( visible ) ;
}

void N::CommonMain::ChangeOutputVolume(bool visible)
{
  OutputVolume -> setVisible ( visible                                    ) ;
  OutputVolume -> setValue   ( (int)( plan->Audio.currentVolume() / 100 ) ) ;
}

void N::CommonMain::VolumeInputChanged(int value)
{
}

void N::CommonMain::VolumeOutputChanged(int value)
{
  plan -> Audio . setVolume ( value * 100 ) ;
}

void N::CommonMain::DismantleHunters(void)
{
  hunterPlugins . clear ( )                 ;
  for (int i=0;i<hunterActions.count();i++) {
    hunterActions[i]->deleteLater()         ;
  }                                         ;
  hunterActions . clear ( )                 ;
  ///////////////////////////////////////////
  for (int i=0;i<hunters.count();i++)       {
    HunterPlugin  * hp = hunters[i]         ;
    QPluginLoader * pl = hunterLoaders[hp]  ;
    pl->unload()                            ;
    pl->deleteLater()                       ;
  }                                         ;
  hunters       . clear ( )                 ;
  hunterLoaders . clear ( )                 ;
}

void N::CommonMain::LoadHunters(QMenu * menu,QString catalog)
{
  QDir PD = plan->Dirs[N::Directory::Plugins] ;
  QDir HP = PD.absoluteFilePath("Hunters")    ;
  if (!Plugins::LoadHunters                   (
         HP,hunters,hunterLoaders)) return    ;
  HuntersMenu(menu,catalog)                   ;
}

void N::CommonMain::HuntersMenu(QMenu * menu,QString catalog)
{
  for (int i=0;i<hunters.count();i++)                {
    QString     name = hunters [ i ] -> name     ( ) ;
    QString     hcat = hunters [ i ] -> catalog  ( ) ;
    QStringList lcat = hunters [ i ] -> listings ( ) ;
    bool    addition = true                          ;
    if (hcat.length()>0)                             {
      if (!lcat.contains(hcat)) lcat << hcat         ;
    }                                                ;
    if (catalog.length()>0 && lcat.count()>0)        {
      addition = lcat . contains ( catalog )         ;
    }                                                ;
    if (addition)                                    {
      QAction * action                               ;
      action  = menu->addAction(name)                ;
      action -> setCheckable(true)                   ;
      hunterActions << action                        ;
      hunterPlugins   [action] = hunters[i]          ;
      connect(action,SIGNAL(toggled     (bool))      ,
              this  ,SLOT  (acceptHunter(bool))    ) ;
    }                                                ;
  }                                                  ;
}

void N::CommonMain::acceptHunter(bool checked)
{ Q_UNUSED ( checked )                                    ;
  HunterPlugin * hp = NULL                                ;
  for (int i=0;IsNull(hp) && i<hunterActions.count();i++) {
    QAction * action = hunterActions [ i ]                ;
    if (action->isChecked())                              {
      hp = hunterPlugins [ action ]                       ;
      action -> blockSignals ( true  )                    ;
      action -> setChecked   ( false )                    ;
      action -> blockSignals ( false )                    ;
    }                                                     ;
  }                                                       ;
  if (IsNull(hp)) return                                  ;
  QWidget * w                                             ;
  hp  -> initialize ( plan->sql )                         ;
  w    = hp->create ( Mdi ,plan )                         ;
  if ( IsNull(w) ) return                                 ;
  Mdi -> Fit        ( w         )                         ;
  w   -> show       (           )                         ;
  hp  -> startup    ( w         )                         ;
}

void N::CommonMain::DismantlePictures(void)
{
  picturePlugins . clear ( )                 ;
  for (int i=0;i<pictureActions.count();i++) {
    pictureActions[i]->deleteLater()         ;
  }                                          ;
  pictureActions . clear ( )                 ;
  ////////////////////////////////////////////
  for (int i=0;i<pictureFilters.count();i++) {
    PicturePlugin * hp = pictureFilters[i ]  ;
    QPluginLoader * pl = pictureLoaders[hp]  ;
    pl->unload()                             ;
    pl->deleteLater()                        ;
  }                                          ;
  pictureFilters . clear ( )                 ;
  pictureLoaders . clear ( )                 ;
}

void N::CommonMain::LoadPictures(QMenu * menu,QString catalog)
{
  QDir PD = plan->Dirs[N::Directory::Plugins]       ;
  QDir HP = PD.absoluteFilePath("Pictures")         ;
  if (!N::Plugins::LoadPictures                     (
         HP,pictureFilters,pictureLoaders)) return  ;
  PicturesMenu(menu,catalog)                        ;
}

void N::CommonMain::PicturesMenu(QMenu * menu,QString catalog)
{
  for (int i=0;i<pictureFilters.count();i++)                                  {
    QString name     = pictureFilters[i]->name   ()                           ;
    QString hcat     = pictureFilters[i]->catalog()                           ;
    bool    addition = true                                                   ;
    if (catalog.length()>0 && hcat.length()>0) addition = ( catalog == hcat ) ;
    if (addition)                                                             {
      QAction * action                                                        ;
      action  = menu->addAction(name)                                         ;
      action -> setCheckable(true)                                            ;
      pictureActions << action                                                ;
      picturePlugins   [action] = pictureFilters[i]                           ;
      connect(action,SIGNAL(toggled     (bool))                               ,
              this  ,SLOT  (acceptPictures(bool))                           ) ;
    }                                                                         ;
  }                                                                           ;
}

void N::CommonMain::acceptPictures(bool checked)
{ Q_UNUSED ( checked )                                     ;
  PicturePlugin * pp = NULL                                ;
  for (int i=0;IsNull(pp) && i<pictureActions.count();i++) {
    QAction * action = pictureActions [ i ]                ;
    if (action->isChecked())                               {
      pp = picturePlugins [ action ]                       ;
      action -> blockSignals ( true  )                     ;
      action -> setChecked   ( false )                     ;
      action -> blockSignals ( false )                     ;
    }                                                      ;
  }                                                        ;
  if (IsNull(pp)) return                                   ;
  QObject * w                                              ;
  pp  -> initialize ( plan       )                         ;
  w    = pp->create ( Mdi , plan )                         ;
  if (pp->hasGui())                                        {
    QWidget * wgx = Casting(QWidget,w)                     ;
    if (NotNull(wgx))                                      {
      Mdi -> Fit     ( wgx )                               ;
      wgx -> show    (     )                               ;
      pp  -> startup ( wgx )                               ;
    }                                                      ;
  } else                                                   {
    pp  -> docking ( this , w )                            ;
    pp  -> startup ( w        )                            ;
  }                                                        ;
}

void N::CommonMain::DismantleDocuments(void)
{
  documentPlugins . clear ( )                 ;
  for (int i=0;i<documentActions.count();i++) {
    documentActions[i]->deleteLater()         ;
  }                                           ;
  documentActions . clear ( )                 ;
  /////////////////////////////////////////////
  for (int i=0;i<documentFilters.count();i++) {
    DocumentPlugin * hp = documentFilters[i ] ;
    QPluginLoader  * pl = documentLoaders[hp] ;
    pl->unload()                              ;
    pl->deleteLater()                         ;
  }                                           ;
  documentFilters . clear ( )                 ;
  documentLoaders . clear ( )                 ;
}

void N::CommonMain::LoadDocuments(QMenu * menu,QString catalog,int type)
{
  QDir PD = plan->Dirs[N::Directory::Plugins]        ;
  QDir HP = PD.absoluteFilePath("Documents")         ;
  if (!Plugins::LoadDocuments                        (
         HP,documentFilters,documentLoaders)) return ;
  DocumentsMenu(menu,catalog,type)                   ;
}

void N::CommonMain::DocumentsMenu(QMenu * menu,QString catalog,int type)
{
  for (int i=0;i<documentFilters.count();i++)               {
    QString     name     = documentFilters[i]->name     ( ) ;
    QString     hcat     = documentFilters[i]->catalog  ( ) ;
    QStringList lcat     = documentFilters[i]->listings ( ) ;
    int         Type     = documentFilters[i]->type     ( ) ;
    bool        addition = true                             ;
    if (hcat.length()>0)                                    {
      if (!lcat.contains(hcat)) lcat << hcat                ;
    }                                                       ;
    if (catalog.length()>0 && lcat.count()>0)               {
      addition = lcat . contains ( catalog )                ;
    }                                                       ;
    if (Type>0 && Type!=type) addition = false              ;
    if (0 == Type           ) addition = true               ;
    if (addition)                                           {
      QAction * action                                      ;
      action  = menu->addAction(name)                       ;
      action -> setCheckable(true)                          ;
      documentActions << action                             ;
      documentPlugins   [action] = documentFilters[i]       ;
      connect(action,SIGNAL(toggled        (bool))          ,
              this  ,SLOT  (acceptDocuments(bool))        ) ;
    }                                                       ;
  }                                                         ;
}

void N::CommonMain::acceptDocuments(bool checked)
{ Q_UNUSED ( checked )                                      ;
  DocumentPlugin * pp = NULL                                ;
  for (int i=0;IsNull(pp) && i<documentActions.count();i++) {
    QAction * action = documentActions [ i ]                ;
    if (action->isChecked())                                {
      pp = documentPlugins [ action ]                       ;
      action -> blockSignals ( true  )                      ;
      action -> setChecked   ( false )                      ;
      action -> blockSignals ( false )                      ;
    }                                                       ;
  }                                                         ;
  if (IsNull(pp)) return                                    ;
  QObject * w                                               ;
  pp  -> initialize ( plan       )                          ;
  w    = pp->create ( Mdi , plan )                          ;
  if (pp->hasGui())                                         {
    QWidget * wgx = Casting(QWidget,w)                      ;
    if (NotNull(wgx))                                       {
      Mdi -> Fit     ( wgx )                                ;
      wgx -> show    (     )                                ;
      pp  -> startup ( wgx )                                ;
    }                                                       ;
  } else                                                    {
    pp  -> docking ( this , w )                             ;
    pp  -> startup ( w        )                             ;
  }                                                         ;
}

void N::CommonMain::DismantleSQLs(void)
{
  sqlPlugins . clear ( )                   ;
  for (int i=0;i<sqlActions.count();i++)   {
    sqlActions[i]->deleteLater()           ;
  }                                        ;
  sqlActions . clear ( )                   ;
  //////////////////////////////////////////
  for (int i=0;i<SqlPlugins.count();i++)   {
    SqlPlugin     * hp = SqlPlugins [ i  ] ;
    QPluginLoader * pl = SqlLoaders [ hp ] ;
    pl->unload()                           ;
    pl->deleteLater()                      ;
  }                                        ;
  SqlPlugins . clear ( )                   ;
  SqlLoaders . clear ( )                   ;
}

void N::CommonMain::LoadSQLs(QMenu * menu)
{
  QDir PD = plan->Dirs[N::Directory::Plugins]                 ;
  QDir HP = PD.absoluteFilePath("Sql")                        ;
  if (!N::Plugins::LoadSqls(HP,SqlPlugins,SqlLoaders)) return ;
  SQLsMenu ( menu )                                           ;
}

void N::CommonMain::SQLsMenu(QMenu * menu)
{
  for (int i=0;i<SqlPlugins.count();i++)             {
    QString name = SqlPlugins [ i ] -> name ( )      ;
    QAction * action                                 ;
    action  = menu->addAction(name)                  ;
    action -> setCheckable(true)                     ;
    sqlActions << action                             ;
    sqlPlugins   [action] = SqlPlugins[i]            ;
    nConnect ( action , SIGNAL ( toggled  (bool) )   ,
               this   , SLOT   ( acceptSQL(bool) ) ) ;
  }                                                  ;
}

void N::CommonMain::acceptSQL(bool checked)
{ Q_UNUSED ( checked )                                 ;
  N::SqlPlugin * sp = NULL                             ;
  for (int i=0;IsNull(sp) && i<sqlActions.count();i++) {
    QAction * action = sqlActions [ i ]                ;
    if (action->isChecked())                           {
      sp = sqlPlugins [ action ]                       ;
      action -> blockSignals ( true  )                 ;
      action -> setChecked   ( false )                 ;
      action -> blockSignals ( false )                 ;
    }                                                  ;
  }                                                    ;
  if ( IsNull(sp)   ) return                           ;
  if (!sp->exists() ) return                           ;
  //////////////////////////////////////////////////////
  sp -> initialize ( plan )                            ;
  sp -> create     (      )                            ;
  sp -> startup    ( this )                            ;
}

void N::CommonMain::LoadScripts(int type)
{
  scriptID . clear ( )                          ;
  EnterSQL(SC,plan->sql)                        ;
    UUIDs Uuids                                 ;
    SUID  zuid                                  ;
    Uuids = SC.Uuids                            (
      plan->Tables[Tables::Scripts]             ,
      "uuid"                                    ,
      QString("where type = %1 order by id desc")
     .arg(type)                               ) ;
    foreach (zuid,Uuids)                        {
      QString nnn = SC.getName                  (
        plan->Tables[Tables::Names]             ,
        "uuid",plan->LanguageId,zuid          ) ;
      scriptID[zuid] = nnn                      ;
    }                                           ;
  LeaveSQL(SC,plan->sql)                        ;
}

void N::CommonMain::MountScripts(QMenu * menu)
{
  UUIDs Uuids = scriptID.keys()                    ;
  SUID  uuid                                       ;
  foreach (uuid,Uuids)                             {
    QAction * action                               ;
    QString s = scriptID[uuid]                     ;
    action  = menu->addAction ( s    )             ;
    action -> setData         ( uuid )             ;
    action -> setCheckable    ( true )             ;
    scriptActions[uuid] = action                   ;
    connect(action,SIGNAL(toggled        (bool))   ,
            this  ,SLOT  (ScriptActivated(bool)) ) ;
  }                                                ;
}

void N::CommonMain::ScriptActivated(bool)
{
  UUIDs     Uuids  = scriptID.keys()                  ;
  SUID      uuid                                      ;
  QAction * action = NULL                             ;
  if (Uuids.count()<=0) return                        ;
  for (int i=0;IsNull(action) && i<Uuids.count();i++) {
    uuid = Uuids[i]                                   ;
    if (scriptActions[uuid]->isChecked())             {
      action  = scriptActions [ uuid ]                ;
      action -> blockSignals ( true  )                ;
      action -> setChecked   ( false )                ;
      action -> blockSignals ( false )                ;
    }                                                 ;
  }                                                   ;
  if (IsNull(action)) return                          ;
  uuid = action->data().toULongLong()                 ;
  GoScript ( uuid )                                   ;
}

void N::CommonMain::MountClassifiers(QDir PathDir)
{
  NAMEs HaarPath                                                              ;
  ParamentQuery PQ(Dynamic::Functionality,3,"Classifiers"                   ) ;
  SqlConnection SC(plan->sql)                                                 ;
  if (SC.open("nMain","MountClassifiers"))                                    {
    QString path                                                              ;
    #define SPQ(ITEM)                                                         \
      path=PQ.String(SC,plan->Tables[Tables::Paraments],#ITEM)              ; \
      if (path.length()>0)                                                  { \
        path = PathDir.absoluteFilePath(path)                               ; \
        QFileInfo F(path)                                                   ; \
        if (F.exists())                                                     { \
          HaarPath[Faces::ITEM] = path                                      ; \
        }                                                                   ; \
      }
    SPQ ( FrontalFace )                                                       ;
    SPQ ( LeftEye     )                                                       ;
    SPQ ( RightEye    )                                                       ;
    SPQ ( Mouth       )                                                       ;
    SPQ ( Nose        )                                                       ;
    SPQ ( LeftEar     )                                                       ;
    SPQ ( RightEar    )                                                       ;
    SPQ ( Glass       )                                                       ;
    SPQ ( Body        )                                                       ;
    SPQ ( UpperBody   )                                                       ;
    SPQ ( LowerBody   )                                                       ;
    #undef  SPQ
    SC.close()                                                                ;
  }                                                                           ;
  SC.remove()                                                                 ;
  plan->classifiers = PictureRecognizer::Load ( HaarPath )                    ;
}

void N::CommonMain::Open(QFileInfo FI)
{
  nDropOut        ( !FI.exists()        )       ;
  VirtualIO VIO                                 ;
  nDropOut        ( !VIO.acceptable(FI) )       ;
  VIO.setFileInfo (  FI                 )       ;
  nDropOut        ( !VIO.Detect()       )       ;
  ///////////////////////////////////////////////
  if (VIO.isSingle())                           {
    if (VIO.isCompressed())                     {
      QString fn                                ;
      fn = QFileDialog::getSaveFileName         (
             this                               ,
             tr("Uncompress to file")           ,
             plan->Temporary("")                ,
             "*.*"                            ) ;
      if (fn.length()>0)                        {
        File F                                  ;
        F   . setFileName ( fn )                ;
        VIO . Uncompress  ( F  )                ;
        Alert ( Done  )                         ;
      } else                                    {
        Alert ( Error )                         ;
      }                                         ;
    } else                                      {
      Alert   ( Error )                         ;
    }                                           ;
  } else                                        {
    QString title = FI.absoluteFilePath()       ;
    nNewWindow          ( VirtualFiles , NVF  ) ;
    NVF->setWindowTitle ( title               ) ;
    Mdi->Attach         ( NVF                 ) ;
    NVF->show           (                     ) ;
    connectDock         ( NVF                 ) ;
    NVF->startup        ( FI                  ) ;
  }                                             ;
}

void N::CommonMain::acceptFiles(const QList<QUrl> & urls)
{
  QList<QUrl> URLs                                       ;
  QList<QUrl> SRLs                                       ;
  QList<QUrl> IRLs                                       ;
  QList<QUrl> TRLs                                       ;
  QList<QUrl> ZRLs                                       ;
  QList<QUrl> PRLs                                       ;
  ////////////////////////////////////////////////////////
  QStringList IFX = Images::ExtensionFilters() ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , urls.count() )                         {
    if (urls[i].isLocalFile())                           {
      QString   LF = urls[i].toLocalFile()               ;
      QFileInfo FI(LF)                                   ;
      if (FI.exists()) URLs << urls[i]                   ;
    }                                                    ;
  }                                                      ;
  nDropOut ( URLs.count() <= 0 )                         ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , URLs.count() )                         {
    QString   LF = URLs[i].toLocalFile()                 ;
    QFileInfo FI(LF)                                     ;
    QString   suffix = FI.suffix()                       ;
    suffix = suffix.toLower()                            ;
    suffix = QString("*.%1").arg(suffix)                 ;
    if (IFX.contains(suffix))                            {
      IRLs << URLs[i]                                    ;
    } else                                               {
      SRLs << URLs[i]                                    ;
    }                                                    ;
  }                                                      ;
  URLs = SRLs                                            ;
  SRLs . clear ( )                                       ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , URLs.count() )                         {
    QString   LF = URLs[i].toLocalFile()                 ;
    QFileInfo FI(LF)                                     ;
    QString   suffix = FI.suffix()                       ;
    suffix = suffix.toLower()                            ;
    if (suffix=="txt")                                   {
      TRLs << URLs[i]                                    ;
    } else
    if (FI.fileName()=="README")                         {
      TRLs << URLs[i]                                    ;
    } else
    if (FI.fileName()=="readme")                         {
      TRLs << URLs[i]                                    ;
    } else                                               {
      SRLs << URLs[i]                                    ;
    }                                                    ;
  }                                                      ;
  URLs = SRLs                                            ;
  SRLs . clear ( )                                       ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , URLs.count() )                         {
    QString   LF = URLs[i].toLocalFile()                 ;
    QFileInfo FI(LF)                                     ;
    bool      addition = false                           ;
    if (FI.exists())                                     {
      VirtualIO VIO                                      ;
      if (VIO.acceptable(FI))                            {
        VIO.setFileInfo (FI)                             ;
        if (VIO.Detect())                                {
          if (VIO.isSingle())                            {
            if (VIO.isCompressed())                      {
              addition = true                            ;
            }                                            ;
          }                                              ;
        }                                                ;
      }                                                  ;
    }                                                    ;
    if (addition)                                        {
      ZRLs << URLs[i]                                    ;
    } else                                               {
      SRLs << URLs[i]                                    ;
    }                                                    ;
  }                                                      ;
  URLs = SRLs                                            ;
  SRLs . clear ( )                                       ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , URLs.count() )                         {
    QString   LF = URLs[i].toLocalFile()                 ;
    QFileInfo FI(LF)                                     ;
    bool      addition = false                           ;
    if (FI.exists())                                     {
      VirtualIO VIO                                      ;
      if (VIO.acceptable(FI))                            {
        VIO.setFileInfo (FI)                             ;
        if (VIO.Detect())                                {
          if (!VIO.isSingle())                           {
            addition = true                              ;
          }                                              ;
        }                                                ;
      }                                                  ;
    }                                                    ;
    if (addition)                                        {
      PRLs << URLs[i]                                    ;
    } else                                               {
      SRLs << URLs[i]                                    ;
    }                                                    ;
  }                                                      ;
  URLs = SRLs                                            ;
  SRLs . clear ( )                                       ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , TRLs.count() )                         {
    QString   LF = TRLs[i].toLocalFile()                 ;
    QFileInfo FI(LF)                                     ;
    if (FI.exists()) OpenPlainText(FI)                   ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , IRLs.count() )                         {
    QString   LF = IRLs[i].toLocalFile()                 ;
    QFileInfo FI(LF)                                     ;
    if (FI.exists()) dropInPicture(FI)                   ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , PRLs.count() )                         {
    QString   LF = PRLs[i].toLocalFile()                 ;
    QFileInfo FI(LF)                                     ;
    if (FI.exists()) UnpackFile(FI)                      ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  nFullLoop ( i , ZRLs.count() )                         {
    QString   LF = ZRLs[i].toLocalFile()                 ;
    QFileInfo FI(LF)                                     ;
    if (FI.exists()) UncompressFile(FI)                  ;
  }                                                      ;
}

void N::CommonMain::dropInPicture(QFileInfo & file)
{
  PictureManager PM                           ;
  PM . Import ( file . absoluteFilePath ( ) ) ;
  /////////////////////////////////////////////
  VcfView * VPV = new VcfView(Mdi,plan)       ;
  VPV -> ViewPicture ( file , Mdi           ) ;
}

void N::CommonMain::OpenPlainText(QFileInfo & file)
{
  QByteArray Body                                              ;
  if (!File::toByteArray(file.absoluteFilePath(),Body)) return ;
  BerthIn              ( PlainEditor , PE , 0            )     ;
  PE  -> setWindowIcon ( QIcon(":/images/plaintext.png") )     ;
  PE  -> setPlainText  ( QString::fromUtf8(Body)         )     ;
}

void N::CommonMain::OpenPlainText(QString title,QByteArray & Explain,int arrangement)
{
  BerthIn               ( PlainEditor , PE , 0            ) ;
  PE  -> setWindowIcon  ( QIcon(":/images/plaintext.png") ) ;
  PE  -> setWindowTitle ( title                           ) ;
  PE  -> setPlainText   ( QString::fromUtf8(Explain)      ) ;
}

void N::CommonMain::FoundDocuments(QString name,UUIDs & Uuids)
{
  nNewWindow     ( DocumentItems , DI  ) ;
  if (name.length()>0)                   {
    DI->setWindowTitle(name)             ;
  }                                      ;
  Mdi -> Attach  ( DI                  ) ;
  DI  -> show    (                     ) ;
  connectDock    ( DI                  ) ;
  DI  -> startup ( Uuids               ) ;
  ////////////////////////////////////////////////////////////////////
  connect(DI  ,SIGNAL(Open        (SUID,SUID,int,QString,QString))   ,
          this,SLOT  (OpenDocument(SUID,SUID,int,QString,QString)) ) ;
  connect(DI  ,SIGNAL(Processors        (UUIDs))                     ,
          this,SLOT  (DocumentProcessors(UUIDs))                   ) ;
}

void N::CommonMain::KeywordMatched(QString title,NAMEs & Keywords)
{
  UUIDs Uuids = Keywords.keys()         ;
  nNewWindow     ( KeywordLists , NKL ) ;
  if (title.length()>0)                 {
    NKL->setWindowTitle(title)          ;
  }                                     ;
  connectDock    ( NKL                ) ;
  Mdi -> Attach  ( NKL                ) ;
  NKL -> show    (                    ) ;
  NKL -> startup ( Uuids              ) ;
}

void N::CommonMain::SqlQuery(QString query)
{
  nNewWindow     ( N::SqlQuery , NSQ ) ;
  Mdi -> Attach  ( NSQ               ) ;
  NSQ -> show    (                   ) ;
  NSQ -> startup ( query             ) ;
}

void N::CommonMain::ListUrls(QString name,UUIDs & Uuids)
{
  nNewWindow            ( PageTree , NPT ) ;
  Mdi -> Attach         ( NPT            ) ;
  NPT -> show           (                ) ;
  NPT -> setWindowTitle ( name           ) ;
  NPT -> startup        ( Uuids          ) ;
}

void N::CommonMain::UncompressFile(QFileInfo & FI)
{
  nDropOut        ( !FI.exists()        )  ;
  VirtualIO VIO                            ;
  nDropOut        ( !VIO.acceptable(FI) )  ;
  VIO.setFileInfo (  FI                 )  ;
  nDropOut        ( !VIO.Detect()       )  ;
  nDropOut        ( !VIO.isCompressed() )  ;
  //////////////////////////////////////////
  QString fn                               ;
  fn = QFileDialog::getSaveFileName        (
         this                              ,
         tr("Uncompress to file")          ,
         plan->Temporary("")               ,
         "*.*"                           ) ;
  if (fn.length()>0)                       {
    File F                                 ;
    F   . setFileName ( fn )               ;
    VIO . Uncompress  ( F  )               ;
    Alert ( Done  )                        ;
  } else                                   {
    Alert ( Error )                        ;
  }                                        ;
}

void N::CommonMain::UnpackFile(QFileInfo & FI)
{
  nDropOut            ( !FI.exists()        ) ;
  VirtualIO VIO                               ;
  nDropOut            ( !VIO.acceptable(FI) ) ;
  VIO.setFileInfo     (  FI                 ) ;
  nDropOut            ( !VIO.Detect()       ) ;
  nDropOut            (  VIO.isSingle()     ) ;
  /////////////////////////////////////////////
  QString title = FI.absoluteFilePath()       ;
  nNewWindow          ( VirtualFiles , NVF  ) ;
  NVF->setWindowTitle ( title               ) ;
  Mdi->Attach         ( NVF                 ) ;
  NVF->show           (                     ) ;
  connectDock         ( NVF                 ) ;
  NVF->startup        ( FI                  ) ;
}

void N::CommonMain::Finished(Classifiers * classifier)
{
  nDropOut ( IsNull(classifier) )             ;
  int index = classifiers.indexOf(classifier) ;
  if (index>=0) classifiers.takeAt(index)     ;
  classifier -> deleteLater ( )               ;
  Alert ( Done )                              ;
}

void N::CommonMain::RunSets(VarArgLists & Operations)
{
  Classifiers * classifier = new Classifiers ( this , plan )   ;
  classifiers << classifier                                    ;
  QObject::connect(classifier,SIGNAL(Finished(Classifiers*))   ,
                   this      ,SLOT  (Finished(Classifiers*)) ) ;
  classifier  -> startup ( Operations )                        ;
}

void N::CommonMain::MountWebDock(QString section)
{
  DockBar = new WebDockBar ( this , plan )                            ;
  addToolBar ( Qt::TopToolBarArea , DockBar )                         ;
  DockBar -> hide ( )                                                 ;
  plan->settings.beginGroup ( "Bookmarks" )                           ;
  if (plan->settings.contains(section))                               {
    DockBar -> Bookmark = plan->settings.value(section).toULongLong() ;
  }                                                                   ;
  plan->settings.endGroup   (             )                           ;
  /////////////////////////////////////////////////////////////////////
  DockBar->Load()                                                     ;
}

N::TagsEditor * N::CommonMain::NewTags (
                   int     ObjectType  ,
                   QString scope       ,
                   QString title       ,
                   QString header      ,
                   bool    checkable   )
{
  TagsEditor * TE = new TagsEditor ( Mdi,plan       ) ;
  connectDock              ( TE                     ) ;
  NewTreeWidgetItem        ( head                   ) ;
  head -> setText          ( 0 , header             ) ;
  TE   -> Trigger       = NULL                        ;
  TE   -> Scope         = scope                       ;
  TE   -> TagsCheckable = checkable                   ;
  TE   -> ItemType      = ObjectType                  ;
  TE   -> setWindowTitle   ( title                  ) ;
  TE   -> setFont          ( head , Fonts::ListView ) ;
  TE   -> setAllAlignments ( head , Qt::AlignCenter ) ;
  TE   -> setHeaderItem    ( head                   ) ;
  Mdi  -> Attach           ( TE , Qt::Vertical      ) ;
  TE   -> show             (                        ) ;
  TE  -> startup           (                        ) ;
  return TE                                           ;
}

void N::CommonMain::ThreadEvent(void)
{
  plan -> processEvents ( ) ;
}

bool N::CommonMain::Bustle(void)
{
  Mutex . lock           ( ) ;
  emit assignWaitCursor  ( ) ;
  return true                ;
}

bool N::CommonMain::Vacancy(void)
{
  emit assignArrowCursor ( ) ;
  Mutex . unlock         ( ) ;
  return true                ;
}

void N::CommonMain::setWaitCursor(void)
{
  setCursor ( Qt::WaitCursor ) ;
}

void N::CommonMain::setArrowCursor(void)
{
  setCursor ( Qt::ArrowCursor ) ;
}

int N::CommonMain::RequestInput(QString device,int rate,int bits,int channels,int format)
{
  CUIDs       IDs = plan->AudioChannels.keys()    ;
  int         MID = 0                             ;
  AudioJack * jack                                ;
  jack  = new AudioJack (        )                ;
  jack -> setParent     ( this   )                ;
  jack -> setDevice     ( device )                ;
  jack -> set                                     (
    rate                                          ,
    bits                                          ,
    channels                                      ,
    (N::Acoustics::SampleFormat)format          ) ;
  if (IDs.count()>0)                              {
    for (int i=0;i<IDs.count();i++)               {
      if (IDs[i]>MID) MID = IDs[i]                ;
    }                                             ;
  }                                               ;
  MID++                                           ;
  plan->AudioChannels[MID] = (AudioChannel *)jack ;
  return MID                                      ;
}

N::Junction * N::CommonMain::MountChannel(int MID)
{
  nKickOut ( !plan->AudioChannels.contains(MID) , NULL )       ;
  AudioConduit  * acot                                         ;
  AudioJunction * junc                                         ;
  acot = new AudioConduit  ( this , plan->AudioChannels[MID] ) ;
  junc = new AudioJunction ( this                            ) ;
  plan->junctions << junc                                      ;
  junc->interval = 90                                          ;
  junc->addConduit ( acot )                                    ;
  return (Junction *)junc                                      ;
}

int N::CommonMain::RequestChannel(QString filename)
{
  CUIDs         IDs = plan->AudioChannels.keys()    ;
  int           MID = 0                             ;
  AudioFeeder * aio                                 ;
  aio  = new AudioFeeder (      )                   ;
  aio -> setParent       ( this )                   ;
  aio -> Filename = filename                        ;
  if (IDs.count()>0)                                {
    for (int i=0;i<IDs.count();i++)                 {
      if (IDs[i]>MID) MID = IDs[i]                  ;
    }                                               ;
  }                                                 ;
  MID++                                             ;
  plan->AudioChannels[MID] = (N::AudioChannel *)aio ;
  nConnect ( aio  , SIGNAL(Finished     ())         ,
             this , SLOT  (ChannelCeased())       ) ;
  return MID                                        ;
}

void N::CommonMain::ChannelCeased(void)
{
  QObject * sndr = sender()        ;
  nDropOut ( IsNull(sndr) )        ;
  AudioChannel * aic               ;
  aic = (AudioChannel *)sndr       ;
  nDropOut ( IsNull(aic)      )    ;
  nDropOut ( aic->type() != 2 )    ;
  AudioFeeder * aio                ;
  aio = (AudioFeeder *)aic         ;
  nDropOut ( IsNull(aio)      )    ;
  aio -> finalize ( 100 )          ;
}

QString N::CommonMain::AcquireAudio(void)
{
  QStringList filters                          ;
  QString     filename                         ;
  QDir        d                                ;
  filters  = N::Acoustics::ExtensionFilters(0) ;
  filters  . prepend ( "*.*" )                 ;
  d        = plan->Dirs[N::Directory::Sounds]  ;
  filename = QFileDialog::getOpenFileName      (
               this                            ,
               tr("Open audio file")           ,
               d.absoluteFilePath("")          ,
               filters.join("\n")            ) ;
  return filename                              ;
}

void N::CommonMain::NeighborsChanged(void)
{
  if ( IsNull ( plan                               ) ) return ;
  if ( IsNull ( plan -> Neighbors                  ) ) return ;
  if ( ! plan -> Booleans . contains ( "Neighbors" ) ) return ;
  if ( ! plan -> Booleans [ "Neighbors" ]            ) return ;
  QStringList H    = plan -> Neighbors -> Hostnames           ;
  QString     host                                            ;
  /////////////////////////////////////////////////////////////
  foreach ( host , H )                                        {
    if ( 0 == plan -> Neighbors -> Status [ host ] )          {
      plan -> Neighbors -> Status [ host ] = 1                ;
      Visit ( host )                                          ;
    } else
    if ( 2 == plan -> Neighbors -> Status [ host ] )          {
      Away  ( host )                                          ;
      plan -> Neighbors -> Take ( host )                      ;
    }                                                         ;
  }                                                           ;
}

void N::CommonMain::Visit(QString hostname)
{
}

void N::CommonMain::Away(QString hostname)
{
}

bool N::CommonMain::BindBeacon(int port)
{
  if ( IsNull(plan) ) return false                                       ;
  if ( plan -> Existing ( this , port ) )                                {
    plan -> Booleans [ "Existing" ] = true                               ;
    if ( plan -> Booleans [ "Clone"  ] ) return true                     ;
    if ( plan -> Booleans [ "Awake"  ] )                                 {
      QHostAddress host                                                  ;
      QString      address                                               ;
      host    = plan -> LocalAddress ( QAbstractSocket::IPv4Protocol )   ;
      address = host  . toString     (                               )   ;
      plan -> Request . Exists ( address , port )                        ;
      Time::skip(500)                                                    ;
    }                                                                    ;
    return false                                                         ;
  } else                                                                 {
    if ( plan -> Booleans [ "House"  ] )                                 {
      plan -> UdpBind   ( port )                                         ;
    }                                                                    ;
    if ( plan -> Booleans [ "Knock"  ] )                                 {
      plan -> UdpBeacon ( port )                                         ;
    }                                                                    ;
    if ( plan -> Booleans [ "XmlRpc" ] )                                 {
      if ( plan -> RPC . Start  ( port ) )                               {
        plan -> RPC . setParent ( this                                   ,
                                  SLOT ( Maximized (               ) )   ,
                                  SLOT ( Login     (QString,QString) ) ) ;
      }                                                                  ;
      if ( 0 == plan -> Request . running )                              {
        if ( plan -> Booleans . contains ( "Heartbeat" ) )               {
          if ( plan -> Booleans [ "Heartbeat" ] )                        {
            plan -> Request . start ( )                                  ;
          }                                                              ;
        }                                                                ;
      }                                                                  ;
    }                                                                    ;
  }                                                                      ;
  return true                                                            ;
}
