#include <domination.h>

nMain:: nMain         ( int argc , char ** argv , QWidget * parent )
      : QMainWindow   (                                     parent )
      , nThread       ( 0 , false                                  )
      , Ready         ( false                                      )
      , Stacked       ( NULL                                       )
      , Mdi           ( NULL                                       )
      , DockBar       ( NULL                                       )
      , Tray          ( NULL                                       )
      , Opacity       ( 1.00                                       )
      #ifdef QT_TORRENT_LIB
      , TorrentWidget ( NULL                                       )
      #endif
{
  Plan    . setObject   ( this        ) ;
  Plan    . setWidget   ( this        ) ;
  Plan    . arguments   ( argc , argv ) ;
  owFlags = windowFlags (             ) ;
  Neutrino::qPlan = &Plan               ;
}

nMain::~nMain (void)
{
}

void nMain::focusInEvent(QFocusEvent * event)
{
  if (FocusIn ()) event->accept   (     ) ;
  else QMainWindow::focusInEvent  (event) ;
}

void nMain::focusOutEvent(QFocusEvent * event)
{
  if (FocusOut()) event->accept   (     ) ;
  else QMainWindow::focusOutEvent (event) ;
}

bool nMain::FocusIn(void)
{
  return false ;
}

bool nMain::FocusOut(void)
{
  return false ;
}

void nMain::showEvent(QShowEvent * event)
{
  QMainWindow::showEvent(event)            ;
  if (!isVisible ( )) Plan.Visible = false ; else
  if (isHidden   ( )) Plan.Visible = false ; else
  if (isMinimized( )) Plan.Visible = false ; else
                      Plan.Visible = true  ;
}

void nMain::closeEvent (QCloseEvent * event)
{
  if (Shutdown()) event->accept() ; else
  QMainWindow::closeEvent(event)  ;
}

Qt::WindowFlags nMain::Frameless(QWidget * widget)
{
  Qt::WindowFlags wflags = widget->windowFlags() ;
  unsigned int    mask   = 0xFFFFFFFFL           ;
  mask   ^= Qt::WindowTitleHint                  ;
  wflags  = Qt::WindowStaysOnTopHint             ;
  wflags |= Qt::FramelessWindowHint              ;
  wflags |= Qt::CustomizeWindowHint              ;
  wflags &= mask                                 ;
  widget->setWindowFlags(wflags)                 ;
  return wflags                                  ;
}

void nMain::FadeIn(QWidget * widget,int steps)
{
  if (Opacity<0.1) Opacity = 0.1           ;
  qreal dw = Opacity - 0.1                 ;
  qreal opacity = 0.1                      ;
  dw /= steps                              ;
  for (int i=0;i<=steps;i++)               {
    widget->setWindowOpacity(opacity)      ;
    opacity += dw                          ;
    if (opacity>Opacity) opacity = Opacity ;
    widget->show()                         ;
    qApp->processEvents()                  ;
    qApp->sendPostedEvents()               ;
    nTime::msleep(50)                      ;
  }                                        ;
  widget->setWindowOpacity(Opacity)        ;
  widget->show()                           ;
}

void nMain::FadeOut(QWidget * widget,int steps)
{
  if (Opacity<0.1) Opacity = 0.1      ;
  qreal dw = Opacity - 0.1            ;
  qreal opacity = Opacity             ;
  dw /= steps                         ;
  for (int i=0;i<=steps;i++)          {
    widget->setWindowOpacity(opacity) ;
    opacity -= dw                     ;
    if (opacity<0) opacity = 0        ;
    widget->show()                    ;
    qApp->processEvents()             ;
    qApp->sendPostedEvents()          ;
    nTime::msleep(50)                 ;
  }                                   ;
  widget->setWindowOpacity(opacity)   ;
  widget->hide()                      ;
}

void nMain::Notify(QString message)
{
  QMessageBox::information(this,windowTitle(),message) ;
}

void nMain::Notify(QString caption,QString message)
{
  QMessageBox::information(this,caption,message) ;
}

void nMain::Quit(void)
{
  nDropOut ( !Shutdown() )       ;
  qApp->closeAllWindows       () ;
  qApp->quitOnLastWindowClosed() ;
}

void nMain::MountMdi(void)
{
  nDropOut ( NotNull(Mdi) )                        ;
  QWidget * widget = Stacked                       ;
  if (IsNull(widget)) widget = this                ;
  Mdi = new nMdiArea(widget,&Plan)                 ;
  if (widget==this) setCentralWidget(Mdi)          ;
  //////////////////////////////////////////////////
  connect(Mdi ,SIGNAL(childChanged())              ,
          this,SLOT  (ChildChanged())            ) ;
  connect(Mdi ,SIGNAL(Files      (QList<QUrl>))    ,
          this,SLOT  (acceptFiles(QList<QUrl>))  ) ;
}

void nMain::MountTray(void)
{
  nDropOut ( NotNull(Tray) )                                       ;
  Tray  = new QSystemTrayIcon ( this         )                     ;
  Tray -> setIcon             ( windowIcon() )                     ;
  Tray -> show                (              )                     ;
  //////////////////////////////////////////////////////////////////
  connect                                                          (
    Tray , SIGNAL (activated(QSystemTrayIcon::ActivationReason))   ,
    this , SLOT   (TrayClick(QSystemTrayIcon::ActivationReason)) ) ;
}

void nMain::TrayClick(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason)                     {
    case QSystemTrayIcon::Unknown     :
    break                             ;
    case QSystemTrayIcon::Context     :
      show()                          ;
    break                             ;
    case QSystemTrayIcon::DoubleClick :
    break                             ;
    case QSystemTrayIcon::Trigger     :
      show()                          ;
    break                             ;
    case QSystemTrayIcon::MiddleClick :
    break                             ;
  }                                   ;
}

bool nMain::LoadPlan(int Language,QString Username,QString Settings)
{
  QProgressBar * P = Plan.Progress("Initialize %v/%m")                ;
  P->setTextVisible(false)                                            ;
  P->setRange(0,19+Neutrino::Fonts::User) ; Plan.processEvents()      ;
  Plan.LanguageId        = Language                                   ;
  Plan.Settings.Username = Username                                   ;
  P->setValue( 1); Plan.processEvents()                               ;
  Plan.setSettings(Settings)                                          ;
  P->setValue( 2); Plan.processEvents()                               ;
  Plan.setLibraryPaths()                                              ;
  P->setValue( 3); Plan.processEvents()                               ;
  Plan.Initialize()                                                   ;
  P->setValue( 4); Plan.processEvents()                               ;
  Plan.Site.SQL      = new nSql()                                     ;
(*Plan.Site.SQL)     = Plan.Sql                                       ;
  Plan.Site.Table    = "settings"                                     ;
  Plan.Site.Username = Plan.Hostname                                  ;
  Plan.Site.Initialize()                                              ;
  P->setValue( 5); Plan.processEvents()                               ;
  Plan.loadPaper("papersize")                                         ;
  P->setValue( 6); Plan.processEvents()                               ;
  Neutrino::InstallTables(Plan)                                       ;
  P->setValue( 7); Plan.processEvents()                               ;
  for (int i=Neutrino::Fonts::Default;i<Neutrino::Fonts::User;i++)    {
    QString FN = QString("Font %1").arg(i)                            ;
    QFont F = font()                                                  ;
    Plan.Settings.LoadFont(FN,F)                                      ;
    F.setStyleStrategy((enum QFont::StyleStrategy)                    (
                       QFont::PreferAntialias                         |
                       QFont::PreferQuality)                        ) ;
    Plan.Fonts[i] = nFont(F)                                          ;
    P->setValue( 7+i); Plan.processEvents()                           ;
  }                                                                   ;
  P->setValue( 7+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.setFont(this,Neutrino::Fonts::Default)                         ;
  P->setValue( 8+Neutrino::Fonts::User); Plan.processEvents()         ;
  if (NotNull(menuBar()))                                             {
    Plan.setFont (menuBar(),Neutrino::Fonts::Menu)                    ;
    QList<QMenu *> Menus = MainMenus()                                ;
    for (int i=0;i<Menus.count();i++)                                 {
      Plan.setFont(Menus[i],Neutrino::Fonts::Menu)                    ;
    }                                                                 ;
  }                                                                   ;
  Plan.AudioDepot("AudioDepot")                                       ;
  Plan.VideoDepot("VideoDepot")                                       ;
  P->setValue( 9+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.setFont(statusBar(),Neutrino::Fonts::Status)                   ;
  Plan.setFont(P,Neutrino::Fonts::Progress)                           ;
  P->setValue(10+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan . Audio    . setParent(this)                                   ;
  P->setValue(11+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan . Recorder . setParent(this)                                   ;
  P->setValue(12+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.addSound(Neutrino::Command::Wrong   ,"Wrong"   ,"Wrong"   )    ;
  P->setValue(13+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.addSound(Neutrino::Command::Click   ,"Click"   ,"Click"   )    ;
  P->setValue(14+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.addSound(Neutrino::Command::Selected,"Selected","Selected")    ;
  P->setValue(15+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.addSound(Neutrino::Command::Menu    ,"Menu"    ,"Menu"    )    ;
  P->setValue(16+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.addSound(Neutrino::Command::Error   ,"Error"   ,"Error"   )    ;
  P->setValue(17+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.addSound(Neutrino::Command::Done    ,"Done"    ,"Done"    )    ;
  P->setValue(18+Neutrino::Fonts::User); Plan.processEvents()         ;
  Plan.addSound(Neutrino::Command::Action  ,"Action"  ,"Action"  )    ;
  P->setValue(19+Neutrino::Fonts::User); Plan.processEvents()         ;
  #define DPATH(NAME,V)                                               \
    Plan . Dirs    [ Neutrino::Directory::NAME ] = Plan.Path ( V ) ;  \
    Plan . setPath ( Neutrino::Directory::NAME , "Directory" , V)
  DPATH(Images     ,"Images"     )                                    ;
  DPATH(Sounds     ,"Sounds"     )                                    ;
  DPATH(Download   ,"Download"   )                                    ;
  DPATH(Upload     ,"Upload"     )                                    ;
  DPATH(History    ,"History"    )                                    ;
  DPATH(Development,"Development")                                    ;
  #ifdef Q_OS_WIN64
  DPATH(Plugins    ,"Plugins/x64")                                    ;
  #else
  DPATH(Plugins    ,"Plugins"    )                                    ;
  #endif
  DPATH(Player     ,"SMPlayer"   )                                    ;
  DPATH(Web        ,"Web"        )                                    ;
  DPATH(Scripts    ,"Scripts"    )                                    ;
  DPATH(Qt         ,"Qt"         )                                    ;
  DPATH(MetaTrader ,"MetaTrader" )                                    ;
  DPATH(Include    ,"include"    )                                    ;
  DPATH(Help       ,"Help"       )                                    ;
  DPATH(Backup     ,"Backup"     )                                    ;
  DPATH(Android    ,"Android"    )                                    ;
  DPATH(Temporary  ,"Temp"       )                                    ;
  #undef  DPATH
  P->hide        ()                                                   ;
  P->deleteLater ()                                                   ;
  AttachActions  ()                                                   ;
  return true                                                         ;
}

bool nMain::LoadIcons(int style)
{
  #define AI(TYPE,icon) Plan . addIcon ( Neutrino::Types::TYPE ,1,0,icon )
  AI ( Album      , QIcon(":/images/videos.png"     ) )                  ;
  AI ( URL        , QIcon(":/images/bookmarks.png"  ) )                  ;
  AI ( Gallery    , QIcon(":/images/gallery.png"    ) )                  ;
  AI ( ColorGroup , QIcon(":/images/colorgroups.png") )                  ;
  AI ( Palette    , QIcon(":/images/palettes.png"   ) )                  ;
  return true                                                            ;
  #undef  AI
}

void nMain::EventSignal(int EventId)
{
  Plan.Play(EventId) ;
}

QList<QMenu *> nMain::MainMenus (void)
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

QList<QMenu *> nMain::SubMenus(QObject * parent)
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

void nMain::Maximized(void)
{
  showMaximized  ( ) ;
}

void nMain::Minimized(void)
{
  showMinimized  ( ) ;
}

void nMain::FullScreen(void)
{
  showFullScreen ( ) ;
}

void nMain::NormalWindow(void)
{
  showNormal     ( ) ;
}

void nMain::DockTray(void)
{
}

void nMain::Engross(bool shown)
{
  if (shown)             {
    FullScreen   (     ) ;
    DisplayMenu  (false) ;
  } else                 {
    Maximized    (     ) ;
    DisplayMenu  (true ) ;
  }                      ;
}

void nMain::SubwindowView(void)
{
  nDropOut ( IsNull(Mdi)  ) ;
  Mdi -> Subwindow (      ) ;
}

void nMain::TabbedView(void)
{
  nDropOut ( IsNull(Mdi)  ) ;
  Mdi->Tabbed (           ) ;
}

void nMain::CascadeWindows(void)
{
  nDropOut ( IsNull(Mdi)  ) ;
  Mdi->Cascade (          ) ;
}

void nMain::TileWindows(void)
{
  nDropOut  ( IsNull(Mdi) ) ;
  Mdi->Tile (             ) ;
}

void nMain::ChildChanged(void)
{
}

void nMain::DisplayTitle(bool shown)
{
}

void nMain::DisplayMenu(bool shown)
{
  menuBar   ( ) -> setVisible ( shown ) ;
}

void nMain::DisplayStatus(bool shown)
{
  statusBar ( ) -> setVisible ( shown ) ;
}

void nMain::AttachActions(void)
{
}

void nMain::AttachDock(QWidget * widget,QString title,Qt::DockWidgetArea area,Qt::DockWidgetAreas areas)
{
  QMdiSubWindow * mdi  = Casting(QMdiSubWindow,widget->parent()) ;
  nTreeDock     * tree = Casting(nTreeDock    ,widget          ) ;
  nListDock     * list = Casting(nListDock    ,widget          ) ;
  nSourcesView  * srcs = Casting(nSourcesView ,widget          ) ;
  nProjectsView * pros = Casting(nProjectsView,widget          ) ;
  nDebugView    * dbgs = Casting(nDebugView   ,widget          ) ;
  if (NotNull(mdi )) mdi  -> deleteLater (                     ) ;
  if (NotNull(tree)) tree -> Docking     (this,title,area,areas) ;
  if (NotNull(list)) list -> Docking     (this,title,area,areas) ;
  if (NotNull(srcs)) srcs -> Docking     (this,title,area,areas) ;
  if (NotNull(pros)) pros -> Docking     (this,title,area,areas) ;
  if (NotNull(dbgs)) dbgs -> Docking     (this,title,area,areas) ;
}

void nMain::AttachMdi(QWidget * widget,int Direction)
{
  nTreeDock     * tree = Casting(nTreeDock    ,widget) ;
  nListDock     * list = Casting(nListDock    ,widget) ;
  nSourcesView  * srcs = Casting(nSourcesView ,widget) ;
  nProjectsView * pros = Casting(nProjectsView,widget) ;
  nDebugView    * dbgs = Casting(nDebugView   ,widget) ;
  nAttachDock   * srcd = (nAttachDock *)srcs           ;
  nAttachDock   * prod = (nAttachDock *)pros           ;
  nAttachDock   * dbgd = (nAttachDock *)dbgs           ;
  switch (Direction)                                   {
    case 0                                             :
      widget -> setParent ( Mdi                )       ;
      Mdi    -> Attach    ( widget             )       ;
      widget -> show      (                    )       ;
    break                                              ;
    case Qt::Vertical                                  :
    case Qt::Horizontal                                :
      widget -> setParent ( Mdi                )       ;
      Mdi    -> Attach    ( widget , Direction )       ;
      widget -> show      (                    )       ;
    break                                              ;
    default                                            :
    break                                              ;
  }                                                    ;
  if (NotNull(tree)) tree->Detach(this)                ;
  if (NotNull(list)) list->Detach(this)                ;
  if (NotNull(srcs)) srcd->Detach(this)                ;
  if (NotNull(pros)) prod->Detach(this)                ;
  if (NotNull(dbgs)) dbgd->Detach(this)                ;
}

nTagsEditor * nMain::NewTags(int ObjectType,QString scope,QString title,QString header,bool checkable)
{
  nTagsEditor * TE = new nTagsEditor(Mdi,&Plan) ;
  TE  -> Trigger       = NULL                   ;
  TE  -> Scope         = scope                  ;
  TE  -> TagsCheckable = checkable              ;
  TE  -> ItemType      = ObjectType             ;
  TE  -> setWindowTitle ( title               ) ;
  TE  -> setHeaderLabel ( header              ) ;
  Mdi -> Attach         ( TE , Qt::Vertical   ) ;
  TE  -> show           (                     ) ;
  connectSignal         ( TE                  ) ;
  connectDock           ( TE                  ) ;
  TE  -> List           (                     ) ;
  return TE                                     ;
}

bool nMain::Bustle(void)
{
  Mutex . lock   (                ) ;
  setCursor      ( Qt::WaitCursor ) ;
  return true                       ;
}

bool nMain::Vacancy(void)
{
  setCursor      ( Qt::ArrowCursor ) ;
  Mutex . unlock (                 ) ;
  return true                        ;
}

void nMain::HelpDragDrop(void)
{
  nDragDropStatusBar * ddsb = new nDragDropStatusBar( this ) ;
  setStatusBar ( ddsb )                                      ;
}

void nMain::setCpu(void)
{
  CPUs[0]      = new nCpuLabel(NULL)             ;
  CPUs[0]     -> Flag = false                    ;
  CPUs[0]     -> setToolTip(tr("Program usage")) ;
  statusBar() -> addPermanentWidget(CPUs[0])     ;
  CPUs[1]      = new nCpuLabel(NULL)             ;
  CPUs[1]     -> setToolTip(tr("Machine usage")) ;
  statusBar() -> addPermanentWidget(CPUs[1])     ;
  CPUs[0]     -> hide ( )                        ;
  CPUs[1]     -> hide ( )                        ;
}

void nMain::AudioOutput(QMenu * menu)
{
  QActionGroup * g = Neutrino::Acoustics::DeviceMenu (
                       &Plan                         ,
                       Neutrino::Acoustics::Output   ,
                       menu                        ) ;
  if (IsNull(g)) return                              ;
  connect(g   ,SIGNAL(triggered    (QAction*))       ,
          this,SLOT  (OutputDevice (QAction*))     ) ;
}

void nMain::OutputDevice(QAction * device)
{
  if (IsNull(device)) return                                  ;
  Plan.Equipments[nPlan::DefaultAudioOutput] = device->text() ;
  OutputChanged ( )                                           ;
}

void nMain::OutputChanged(void)
{
}

void nMain::AudioInput(QMenu * menu)
{
  QActionGroup * g = Neutrino::Acoustics::DeviceMenu (
                       &Plan                         ,
                       Neutrino::Acoustics::Input    ,
                       menu                        ) ;
  if (IsNull(g)) return                              ;
  connect(g   ,SIGNAL(triggered    (QAction*))       ,
          this,SLOT  (InputDevice  (QAction*))     ) ;
}

void nMain::InputDevice(QAction * device)
{
  if (IsNull(device)) return                                  ;
  Plan.Equipments[nPlan::DefaultAudioInput ] = device->text() ;
  InputChanged  ( )                                           ;
}

void nMain::InputChanged(void)
{
}

void nMain::DockDebugger(void)
{
  nDebugView * Debugger                                 ;
  Debugger = new nDebugView(this,&Plan)                 ;
  Debugger-> Trigger = NULL                             ;
  Debugger-> Scope   = "Debugger"                       ;
  Debugger-> Docking ( this , tr("Debug window")        ,
    Qt::BottomDockWidgetArea                            ,
    Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea  ) ;
  Debugger -> DockIn ( false                          ) ;
  Plan . DebugWidget = Debugger                         ;
  connectSignal ( Debugger )                            ;
  connectDock   ( Debugger )                            ;
}

void nMain::connectSignal(QWidget * widget)
{
}

void nMain::connectDock(QWidget * widget)
{
  connect(widget,SIGNAL(AttachDock (QWidget*,QString,Qt::DockWidgetArea,Qt::DockWidgetAreas))   ,
          this  ,SLOT  (AttachDock (QWidget*,QString,Qt::DockWidgetArea,Qt::DockWidgetAreas)) ) ;
  connect(widget,SIGNAL(AttachMdi  (QWidget*,int   ))   ,
          this  ,SLOT  (AttachMdi  (QWidget*,int   )) ) ;
}

void nMain::DismantleHunters(void)
{
  HunterPlugins . clear ( )                 ;
  for (int i=0;i<HunterActions.count();i++) {
    HunterActions[i]->deleteLater()         ;
  }                                         ;
  HunterActions . clear ( )                 ;
  ///////////////////////////////////////////
  for (int i=0;i<Hunters.count();i++)       {
    nHunterPlugin * hp = Hunters[i]         ;
    QPluginLoader * pl = HunterLoaders[hp]  ;
    pl->unload()                            ;
    pl->deleteLater()                       ;
  }                                         ;
  Hunters       . clear ( )                 ;
  HunterLoaders . clear ( )                 ;
}

void nMain::LoadHunters(QMenu * menu,QString catalog)
{
  QDir PD = Plan.Dirs[Neutrino::Directory::Plugins] ;
  QDir HP = PD.absoluteFilePath("Hunters")          ;
  if (!Neutrino::Plugins::LoadHunters               (
         HP,Hunters,HunterLoaders)) return          ;
  HuntersMenu(menu,catalog)                         ;
}

void nMain::HuntersMenu(QMenu * menu,QString catalog)
{
  for (int i=0;i<Hunters.count();i++)                                         {
    QString   name = Hunters[i]->name   ()                                    ;
    QString   hcat = Hunters[i]->catalog()                                    ;
    bool      addition = true                                                 ;
    if (catalog.length()>0 && hcat.length()>0) addition = ( catalog == hcat ) ;
    if (addition)                                                             {
      QAction * action                                                        ;
      action  = menu->addAction(name)                                         ;
      action -> setCheckable(true)                                            ;
      HunterActions << action                                                 ;
      HunterPlugins   [action] = Hunters[i]                                   ;
      connect(action,SIGNAL(toggled     (bool))                               ,
              this  ,SLOT  (acceptHunter(bool))                             ) ;
    }                                                                         ;
  }                                                                           ;
}

void nMain::acceptHunter(bool checked)
{
  nHunterPlugin * hp = NULL ;
  for (int i=0;IsNull(hp) && i<HunterActions.count();i++) {
    QAction * action = HunterActions [ i ]                ;
    if (action->isChecked())                              {
      hp = HunterPlugins [ action ]                       ;
      action -> blockSignals ( true  )                    ;
      action -> setChecked   ( false )                    ;
      action -> blockSignals ( false )                    ;
    }                                                     ;
  }                                                       ;
  if (IsNull(hp)) return                                  ;
  QWidget * w                                             ;
  hp  -> initialize ( Plan.Sql   )                        ;
  w    = hp->create ( Mdi ,&Plan )                        ;
  Mdi -> Fit        ( w          )                        ;
  w   -> show       (            )                        ;
  connectSignal     ( w          )                        ;
  hp  -> startup    ( w          )                        ;
}

void nMain::DismantlePictures(void)
{
  PicturePlugins . clear ( )                 ;
  for (int i=0;i<PictureActions.count();i++) {
    PictureActions[i]->deleteLater()         ;
  }                                          ;
  PictureActions . clear ( )                 ;
  ////////////////////////////////////////////
  for (int i=0;i<PictureFilters.count();i++) {
    nPicturePlugin * hp = PictureFilters[i]  ;
    QPluginLoader * pl = PictureLoaders[hp]  ;
    pl->unload()                             ;
    pl->deleteLater()                        ;
  }                                          ;
  PictureFilters . clear ( )                 ;
  PictureLoaders . clear ( )                 ;
}

void nMain::LoadPictures(QMenu * menu,QString catalog)
{
  QDir PD = Plan.Dirs[Neutrino::Directory::Plugins] ;
  QDir HP = PD.absoluteFilePath("Pictures")         ;
  if (!Neutrino::Plugins::LoadPictures              (
         HP,PictureFilters,PictureLoaders)) return  ;
  PicturesMenu(menu,catalog)                        ;
}

void nMain::PicturesMenu(QMenu * menu,QString catalog)
{
  for (int i=0;i<PictureFilters.count();i++)                                  {
    QString   name = PictureFilters[i]->name   ()                             ;
    QString   hcat = PictureFilters[i]->catalog()                             ;
    bool      addition = true                                                 ;
    if (catalog.length()>0 && hcat.length()>0) addition = ( catalog == hcat ) ;
    if (addition)                                                             {
      QAction * action                                                        ;
      action  = menu->addAction(name)                                         ;
      action -> setCheckable(true)                                            ;
      PictureActions << action                                                ;
      PicturePlugins   [action] = PictureFilters[i]                           ;
      connect(action,SIGNAL(toggled     (bool))                               ,
              this  ,SLOT  (acceptPictures(bool))                           ) ;
    }                                                                         ;
  }                                                                           ;
}

void nMain::acceptPictures(bool checked)
{
  nPicturePlugin * pp = NULL                               ;
  for (int i=0;IsNull(pp) && i<PictureActions.count();i++) {
    QAction * action = PictureActions [ i ]                ;
    if (action->isChecked())                               {
      pp = PicturePlugins [ action ]                       ;
      action -> blockSignals ( true  )                     ;
      action -> setChecked   ( false )                     ;
      action -> blockSignals ( false )                     ;
    }                                                      ;
  }                                                        ;
  if (IsNull(pp)) return                                   ;
  QObject * w                                              ;
  pp  -> initialize ( &Plan      )                         ;
  w    = pp->create ( Mdi ,&Plan )                         ;
  if (pp->hasGui())                                        {
    QWidget * wgx = Casting(QWidget,w)                     ;
    if (NotNull(wgx))                                      {
      Mdi -> Fit     ( wgx )                               ;
      wgx -> show    (     )                               ;
      connectSignal  ( wgx )                               ;
      pp  -> startup ( wgx )                               ;
    }                                                      ;
  } else                                                   {
    pp  -> docking ( this , w )                            ;
    pp  -> startup (  w       )                            ;
  }                                                        ;
}

void nMain::DismantleDocuments(void)
{
  DocumentPlugins . clear ( )                  ;
  for (int i=0;i<DocumentActions.count();i++)  {
    DocumentActions[i]->deleteLater()          ;
  }                                            ;
  DocumentActions . clear ( )                  ;
  //////////////////////////////////////////////
  for (int i=0;i<DocumentFilters.count();i++)  {
    nDocumentPlugin * hp = DocumentFilters[i ] ;
    QPluginLoader   * pl = DocumentLoaders[hp] ;
    pl->unload()                               ;
    pl->deleteLater()                          ;
  }                                            ;
  DocumentFilters . clear ( )                  ;
  DocumentLoaders . clear ( )                  ;
}

void nMain::LoadDocuments(QMenu * menu,QString catalog,int type)
{
  QDir PD = Plan.Dirs[Neutrino::Directory::Plugins]  ;
  QDir HP = PD.absoluteFilePath("Documents")         ;
  if (!Neutrino::Plugins::LoadDocuments              (
         HP,DocumentFilters,DocumentLoaders)) return ;
  DocumentsMenu(menu,catalog,type)                   ;
}

void nMain::DocumentsMenu(QMenu * menu,QString catalog,int type)
{
  for (int i=0;i<DocumentFilters.count();i++)                                 {
    QString name = DocumentFilters[i]->name    ()                             ;
    QString hcat = DocumentFilters[i]->catalog ()                             ;
    int     Type = DocumentFilters[i]->type    ()                             ;
    bool    addition = true                                                   ;
    if (catalog.length()>0 && hcat.length()>0) addition = ( catalog == hcat ) ;
    if (Type>0 && Type!=type) addition = false                                ;
    if (addition)                                                             {
      QAction * action                                                        ;
      action  = menu->addAction(name)                                         ;
      action -> setCheckable(true)                                            ;
      DocumentActions << action                                               ;
      DocumentPlugins   [action] = DocumentFilters[i]                         ;
      connect(action,SIGNAL(toggled        (bool))                            ,
              this  ,SLOT  (acceptDocuments(bool))                          ) ;
    }                                                                         ;
  }                                                                           ;
}

void nMain::acceptDocuments(bool checked)
{
  nDocumentPlugin * pp = NULL                               ;
  for (int i=0;IsNull(pp) && i<DocumentActions.count();i++) {
    QAction * action = DocumentActions [ i ]                ;
    if (action->isChecked())                                {
      pp = DocumentPlugins [ action ]                       ;
      action -> blockSignals ( true  )                      ;
      action -> setChecked   ( false )                      ;
      action -> blockSignals ( false )                      ;
    }                                                       ;
  }                                                         ;
  if (IsNull(pp)) return                                    ;
  QObject * w                                               ;
  pp  -> initialize ( &Plan      )                          ;
  w    = pp->create ( Mdi ,&Plan )                          ;
  if (pp->hasGui())                                         {
    QWidget * wgx = Casting(QWidget,w)                      ;
    if (NotNull(wgx))                                       {
      Mdi -> Fit     ( wgx )                                ;
      wgx -> show    (     )                                ;
      connectSignal  ( wgx )                                ;
      pp  -> startup ( wgx )                                ;
    }                                                       ;
  } else                                                    {
    pp  -> docking ( this , w )                             ;
    pp  -> startup (  w       )                             ;
  }                                                         ;
}

void nMain::MountProgress(void)
{
  Plan.ProgressManager  = new nProgressManager ( this , &Plan )  ;
  Plan.ProgressManager-> Trigger = NULL                          ;
  Plan.ProgressManager-> Scope   = "ProgressManager"             ;
  Plan.ProgressManager-> Docking ( this , tr("Progress Manager") ,
    Qt::BottomDockWidgetArea                                     ,
    Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea           ) ;
  Plan.ProgressManager -> DockIn ( false                       ) ;
  connectSignal ( Plan.ProgressManager )                         ;
  connectDock   ( Plan.ProgressManager )                         ;
}

void nMain::LoadScripts(int type)
{
  ScriptID . clear ( )                                ;
  EnterSQL(SC,Plan.Sql)                               ;
    UUIDs Uuids                                       ;
    SUID  zuid                                        ;
    Uuids = SC.Uuids                                  (
      Plan.Tables[Neutrino::Tables::Scripts] , "uuid" ,
      QString("where type = %1 order by id desc"      )
     .arg(type)                                     ) ;
    foreach (zuid,Uuids)                              {
      QString nnn = SC.getName                        (
        Plan.Tables[Neutrino::Tables::Names]          ,
        "uuid",Plan.LanguageId,zuid                 ) ;
      ScriptID[zuid] = nnn                            ;
    }                                                 ;
  LeaveSQL(SC,Plan.Sql)                               ;
}

void nMain::MountScripts(QMenu * menu)
{
  UUIDs Uuids = ScriptID.keys()                    ;
  SUID  uuid                                       ;
  foreach (uuid,Uuids)                             {
    QAction * action                               ;
    QString s = ScriptID[uuid]                     ;
    action  = menu->addAction ( s    )             ;
    action -> setData         ( uuid )             ;
    action -> setCheckable    ( true )             ;
    ScriptActions[uuid] = action                   ;
    connect(action,SIGNAL(toggled        (bool))   ,
            this  ,SLOT  (ScriptActivated(bool)) ) ;
  }                                                ;
}

void nMain::ScriptActivated(bool)
{
  UUIDs     Uuids  = ScriptID.keys()                  ;
  SUID      uuid                                      ;
  QAction * action = NULL                             ;
  if (Uuids.count()<=0) return                        ;
  for (int i=0;IsNull(action) && i<Uuids.count();i++) {
    uuid = Uuids[i]                                   ;
    if (ScriptActions[uuid]->isChecked())             {
      action  = ScriptActions [ uuid ]                ;
      action -> blockSignals ( true  )                ;
      action -> setChecked   ( false )                ;
      action -> blockSignals ( false )                ;
    }                                                 ;
  }                                                   ;
  if (IsNull(action)) return                          ;
  uuid = action->data().toULongLong()                 ;
  GoScript ( uuid )                                   ;
}

void nMain::GoScript(SUID uuid)
{
}

void nMain::MountClassifiers(QDir PathDir)
{
  NAMEs HaarPath                                                              ;
  nParamentQuery PQ(Neutrino::Dynamic::Functionality,3,"Classifiers"        ) ;
  nSqlConnection SC(Plan.Sql)                                                 ;
  if (SC.open("nMain","MountClassifiers"))                                    {
    QString path                                                              ;
    #define SPQ(ITEM)                                                         \
      path=PQ.String(SC,Plan.Tables[Neutrino::Tables::Paraments],#ITEM)     ; \
      if (path.length()>0)                                                  { \
        path = PathDir.absoluteFilePath(path)                               ; \
        QFileInfo F(path)                                                   ; \
        if (F.exists())                                                     { \
          HaarPath[Neutrino::Faces::ITEM] = path                            ; \
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
  Plan.Classifiers = nPictureRecognizer::Load ( HaarPath )                    ;
}

void nMain::Open(QFileInfo FI)
{
  nDropOut        ( !FI.exists()        )       ;
  nVirtualIO VIO                                ;
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
             Plan.Temporary("")                 ,
             "*.*"                            ) ;
      if (fn.length()>0)                        {
        nFile F                                 ;
        F   . setFileName ( fn )                ;
        VIO . Uncompress  ( F  )                ;
        EventSignal (Neutrino::Command::Done )  ;
      } else                                    {
        EventSignal (Neutrino::Command::Error)  ;
      }                                         ;
    } else                                      {
      EventSignal (Neutrino::Command::Error)    ;
    }                                           ;
  } else                                        {
    QString title = FI.absoluteFilePath()       ;
    nNewWindow          ( nVirtualFiles , NVF ) ;
    NVF->setWindowTitle ( title               ) ;
    Mdi->Attach         ( NVF                 ) ;
    NVF->show           (                     ) ;
    connectSignal       ( NVF                 ) ;
    connectDock         ( NVF                 ) ;
    NVF->startup        ( FI                  ) ;
  }                                             ;
}

void nMain::acceptFiles(const QList<QUrl> & urls)
{
  QList<QUrl> URLs                                       ;
  QList<QUrl> SRLs                                       ;
  QList<QUrl> IRLs                                       ;
  QList<QUrl> TRLs                                       ;
  QList<QUrl> ZRLs                                       ;
  QList<QUrl> PRLs                                       ;
  ////////////////////////////////////////////////////////
  QStringList IFX = Neutrino::Images::ExtensionFilters() ;
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
      nVirtualIO VIO                                     ;
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
      nVirtualIO VIO                                     ;
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

void nMain::dropInPicture(QFileInfo & file)
{
  nPictureManager PM                                     ;
  PM . Import ( file . absoluteFilePath ( ) )            ;
  ////////////////////////////////////////////////////////
  VcfGraphicsView * VPV = new VcfGraphicsView(Mdi,&Plan) ;
  connectSignal ( VPV )                                  ;
  VPV -> ViewPicture ( file , Mdi                      ) ;
}

void nMain::OpenPlainText(QFileInfo & file)
{
  QByteArray Body                                                        ;
  if (!Neutrino::File::toByteArray(file.absoluteFilePath(),Body)) return ;
  nNewWindow     ( nPlainEditor , PE               )                     ;
  Mdi -> Attach  ( PE                              )                     ;
  PE  -> setWindowIcon (QIcon(":/images/plaintext.png"))                 ;
  PE  -> show    (                                 )                     ;
  connectSignal  ( PE                              )                     ;
  PE  -> setPlainText ( QString::fromUtf8(Body)    )                     ;
}

void nMain::OpenPlainText(QString title,QByteArray & Explain,int arrangement)
{
  nNewWindow            ( nPlainEditor , PE               ) ;
  Mdi -> Attach         ( PE , arrangement                ) ;
  PE  -> setWindowIcon  ( QIcon(":/images/plaintext.png") ) ;
  PE  -> setWindowTitle ( title                           ) ;
  PE  -> show           (                                 ) ;
  connectSignal         ( PE                              ) ;
  PE  -> setPlainText   ( QString::fromUtf8(Explain)      ) ;
}

void nMain::FoundDocuments(QString name,UUIDs & Uuids)
{
  nNewWindow     ( nDocumentItems , DI ) ;
  if (name.length()>0)                   {
    DI->setWindowTitle(name)             ;
  }                                      ;
  Mdi -> Attach  ( DI                  ) ;
  DI  -> show    (                     ) ;
  connectSignal  ( DI                  ) ;
  connectDock    ( DI                  ) ;
  DI  -> startup ( Uuids               ) ;
  ////////////////////////////////////////////////////////////////////
  connect(DI  ,SIGNAL(Open        (SUID,SUID,int,QString,QString))   ,
          this,SLOT  (OpenDocument(SUID,SUID,int,QString,QString)) ) ;
  connect(DI  ,SIGNAL(Processors        (UUIDs))                     ,
          this,SLOT  (DocumentProcessors(UUIDs))                   ) ;
}

void nMain::KeywordMatched(QString title,NAMEs & Keywords)
{
  UUIDs Uuids = Keywords.keys()          ;
  nNewWindow     ( nKeywordLists , NKL ) ;
  if (title.length()>0)                  {
    NKL->setWindowTitle(title)           ;
  }                                      ;
  connectSignal  ( NKL                 ) ;
  connectDock    ( NKL                 ) ;
  Mdi -> Attach  ( NKL                 ) ;
  NKL -> show    (                     ) ;
  NKL -> startup ( Uuids               ) ;
}

void nMain::SqlQuery(QString query)
{
  nNewWindow     ( nSqlQuery , NSQ ) ;
  Mdi -> Attach  ( NSQ             ) ;
  NSQ -> show    (                 ) ;
  connectSignal  ( NSQ             ) ;
  NSQ -> startup ( query           ) ;
}

void nMain::ListUrls(QString name,UUIDs & Uuids)
{
  nNewWindow            ( nPageTree , NPT ) ;
  Mdi -> Attach         ( NPT             ) ;
  NPT -> show           (                 ) ;
  connectSignal         ( NPT             ) ;
  NPT -> setWindowTitle ( name            ) ;
  NPT -> startup        ( Uuids           ) ;
}

void nMain::UncompressFile(QFileInfo & FI)
{
  nDropOut        ( !FI.exists()        )  ;
  nVirtualIO VIO                           ;
  nDropOut        ( !VIO.acceptable(FI) )  ;
  VIO.setFileInfo (  FI                 )  ;
  nDropOut        ( !VIO.Detect()       )  ;
  nDropOut        ( !VIO.isCompressed() )  ;
  //////////////////////////////////////////
  QString fn                               ;
  fn = QFileDialog::getSaveFileName        (
         this                              ,
         tr("Uncompress to file")          ,
         Plan.Temporary("")                ,
         "*.*"                           ) ;
  if (fn.length()>0)                       {
    nFile F                                ;
    F   . setFileName ( fn )               ;
    VIO . Uncompress  ( F  )               ;
    EventSignal (Neutrino::Command::Done ) ;
  } else                                   {
    EventSignal (Neutrino::Command::Error) ;
  }                                        ;
}

void nMain::UnpackFile(QFileInfo & FI)
{
  nDropOut            ( !FI.exists()        ) ;
  nVirtualIO VIO                              ;
  nDropOut            ( !VIO.acceptable(FI) ) ;
  VIO.setFileInfo     (  FI                 ) ;
  nDropOut            ( !VIO.Detect()       ) ;
  nDropOut            (  VIO.isSingle()     ) ;
  /////////////////////////////////////////////
  QString title = FI.absoluteFilePath()       ;
  nNewWindow          ( nVirtualFiles , NVF ) ;
  NVF->setWindowTitle ( title               ) ;
  Mdi->Attach         ( NVF                 ) ;
  NVF->show           (                     ) ;
  connectSignal       ( NVF                 ) ;
  connectDock         ( NVF                 ) ;
  NVF->startup        ( FI                  ) ;
}

void nMain::DockTorrent(QAction * action)
{
  #ifdef QT_TORRENT_LIB
  FastParkIn                                            (
    nTorrentWidget                                      ,
    TorrentWidget                                       ,
    this                                                ,
    &Plan                                               ,
    action                                              ,
    "VideoTorrent"                                      ,
    tr("Torrent manager")                               ,
    Qt::BottomDockWidgetArea                            ,
    Qt::TopDockWidgetArea  | Qt::BottomDockWidgetArea ) ;
  connectSignal ( TorrentWidget )                       ;
  connectDock   ( TorrentWidget )                       ;
  TorrentWidget->Show(false)                            ;
  #endif
}

void nMain::Finished(nClassifiers * classifier)
{
  nDropOut ( IsNull(classifier) )             ;
  int index = Classifiers.indexOf(classifier) ;
  if (index>=0) Classifiers.takeAt(index)     ;
  classifier -> deleteLater ( )               ;
  EventSignal( Neutrino::Command::Done )      ;
}

void nMain::RunSets(QList<nVarArgs> & Operations)
{
  nClassifiers * classifier = new nClassifiers ( this , &Plan ) ;
  Classifiers << classifier                                     ;
  QObject::connect(classifier,SIGNAL(Finished(nClassifiers*))   ,
                   this      ,SLOT  (Finished(nClassifiers*)) ) ;
  classifier  -> startup ( Operations )                         ;
}

void nMain::MountWebDock(QString section)
{
  DockBar = new nWebDockBar ( this , &Plan )                              ;
  addToolBar ( Qt::TopToolBarArea , DockBar )                             ;
  DockBar -> hide ( )                                                     ;
  Plan . Settings . beginGroup ( "Bookmarks" )                            ;
  if (Plan.Settings.contains(section))                                    {
    DockBar -> Bookmark = Plan . Settings . value (section).toULongLong() ;
  }                                                                       ;
  Plan . Settings . endGroup   (             )                            ;
  /////////////////////////////////////////////////////////////////////////
  DockBar->Load()                                                         ;
}
