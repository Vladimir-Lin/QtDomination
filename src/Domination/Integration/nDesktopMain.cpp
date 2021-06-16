#include <domination.h>

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#define DISABLE_DESKTOP 1
#else
#endif

////////////////////////////////////////////////////////////////////////////

#define Decide decisions[GuiMainId]

#ifdef DISABLE_DESKTOP
#else

static N::ConditionItem DesktopStates[]    =  {
  { N::DesktopMain::Configured      , false } ,
  { N::DesktopMain::NoAccess        , false } ,
  { N::DesktopMain::Loading         , false } ,
  { N::DesktopMain::Loaded          , false } ,
  { N::DesktopMain::Started         , false } ,

  { N::DesktopMain::HasConfigurator , false } ,
  { N::DesktopMain::HasStacked      , true  } ,
  { N::DesktopMain::HasMdi          , true  } ,
  { N::DesktopMain::HasDebugger     , true  } ,
  { N::DesktopMain::AcceptMenu      , true  } ,

  { N::DesktopMain::StackedGui      , false } ,
  { N::DesktopMain::MdiGui          , true  } ,
  { N::DesktopMain::Visibility      , false } ,
  { N::DesktopMain::Interactive     , false } ,
}                                             ;

static N::ConditionItem PrepareNDB []      =  {
  { N::DesktopMain::Configured      , false } ,
  { N::DesktopMain::NoAccess        , false } ,
}                                             ;

static N::ConditionItem EnterConf  []      =  {
  { N::DesktopMain::Configured      , false } ,
  { N::DesktopMain::NoAccess        , true  } ,
  { N::DesktopMain::HasConfigurator , true  } ,
}                                             ;

static N::ConditionItem CanNotEnter[]      =  {
  { N::DesktopMain::Configured      , false } ,
  { N::DesktopMain::NoAccess        , true  } ,
  { N::DesktopMain::HasConfigurator , false } ,
}                                             ;

static N::ConditionItem ReadNDB    []      =  {
  { N::DesktopMain::Configured      , true  } ,
  { N::DesktopMain::NoAccess        , false } ,
  { N::DesktopMain::Loading         , false } ,
  { N::DesktopMain::Loaded          , false } ,
  { N::DesktopMain::Started         , false } ,
}                                             ;

static N::ConditionItem StartPrimary []    =  {
  { N::DesktopMain::Configured      , true  } ,
  { N::DesktopMain::NoAccess        , false } ,
  { N::DesktopMain::Loading         , false } ,
  { N::DesktopMain::Loaded          , true  } ,
  { N::DesktopMain::Started         , false } ,
}                                             ;

#endif

////////////////////////////////////////////////////////////////////////////

N::DesktopMain:: DesktopMain (int argc,char ** argv,QWidget * parent)
               : CommonMain  (    argc,        argv,          parent)
               , capturer    ( NULL                                 )
               , pcap        ( NULL                                 )
{
  WidgetClass                                        ;
#ifdef DISABLE_DESKTOP
#else
  decisions . Blank ( GuiMainId )                    ;
#endif
  capturer = NULL                                    ;
  ////////////////////////////////////////////////////
  nConnect ( this , SIGNAL ( GoRelax  ( ) )          ,
             this , SLOT   ( OnRelax  ( ) )        ) ;
  nConnect ( this , SIGNAL ( OnBusy   ( ) )          ,
             this , SLOT   ( WhenBusy ( ) )        ) ;
  nConnect ( this , SIGNAL ( WatchPicture(QImage))   ,
             this , SLOT   ( ViewPicture (QImage)) ) ;
}

N::DesktopMain::~DesktopMain (void)
{
}

void N::DesktopMain::InstallDecisions(void)
{
#ifdef DISABLE_DESKTOP
#else
  Decide . setConditions ( 14 , DesktopStates   ) ;
  /////////////////////////////////////////////////
  DecisionAction A01                              ;
  A01 . action = N::DesktopMain::Preparation      ; // prepare
  A01 . addConditions    (  2 , PrepareNDB      ) ;
  /////////////////////////////////////////////////
  DecisionAction A02                              ;
  A02 . action = N::DesktopMain::Configuration    ; // EnterConf   => configure
  A02 . addConditions    (  3 , EnterConf       )      ;
  /////////////////////////////////////////////////
  DecisionAction A03                              ;
  A03 . action = N::DesktopMain::Failure          ; // CanNotEnter => failure
  A03 . addConditions    (  3 , CanNotEnter     ) ;
  /////////////////////////////////////////////////
  DecisionAction A04                              ;
  A04 . action = N::DesktopMain::Warmup           ; // Warmup      => loading
  A04 . addConditions    (  5 , ReadNDB         ) ;
  /////////////////////////////////////////////////
  DecisionAction A05                              ;
  A05 . action = N::DesktopMain::Primary          ; // Primary     => primary
  A05 . addConditions    (  5 , StartPrimary    ) ;
  /////////////////////////////////////////////////
  Decide . addAction ( A01 )                      ;
  Decide . addAction ( A02 )                      ;
  Decide . addAction ( A03 )                      ;
  Decide . addAction ( A04 )                      ;
  Decide . addAction ( A05 )                      ;
  /////////////////////////////////////////////////
  plan  -> Booleans [ "Desktop" ] = true          ;
  plan  -> Booleans [ "Pad"     ] = false         ;
  plan  -> Booleans [ "Phone"   ] = false         ;
#endif
}

void N::DesktopMain::InstallMetaTypes(void)
{
  BasicMetaTypes ( ) ;
}

bool N::DesktopMain::DefaultBootup(void)
{
#ifdef DISABLE_DESKTOP
#else
  UUIDs react                      ;
  SUID  u                          ;
  InstallDecisions ( )             ;
  InstallMetaTypes ( )             ;
  do                               {
    plan -> processEvents ( )      ;
    react = Decide.reactions()     ;
    if (react.count()>0)           {
      foreach (u,react)            {
        plan -> processEvents ( )  ;
        switch (u)                 {
          case Preparation         :
            if ( ! prepare   ( ) ) {
              return false         ;
            }                      ;
          break                    ;
          case Failure             :
          return false             ;
          case Configuration       :
            if ( ! configure ( ) ) {
              return false         ;
            }                      ;
          break                    ;
          case Warmup              :
            if ( ! loading   ( ) ) {
              return false         ;
            }                      ;
          break                    ;
          case Primary             :
            if ( ! primary   ( ) ) {
              return false         ;
            }                      ;
          break                    ;
        }                          ;
      }                            ;
    }                              ;
  } while (react.count()>0)        ;
#endif
  return true                      ;
}

bool N::DesktopMain::startup(void)
{
  return DefaultBootup ( ) ;
}

int N::DesktopMain::abort(void)
{
  if ( plan -> Booleans . contains( "Existing" ) )  {
    if ( plan -> Booleans [ "Existing" ] )          {
      return 0                                      ;
    }                                               ;
  }                                                 ;
  Notify ( tr("Configuration files was missing.") ) ;
  return 0                                          ;
}

void N::DesktopMain::setVisibility(bool visibility)
{
#ifdef DISABLE_DESKTOP
  Q_UNUSED              ( visibility              ) ;
#else
  Decide . setCondition ( Visibility , visibility ) ;
#endif
}

void N::DesktopMain::TrayMessage(QString title,QString message)
{
  if (IsNull(Tray)      ) return   ;
  if (!Tray->isVisible()) return   ;
  Tray->showMessage(title,message) ;
}

N::PictureView * N::DesktopMain::CreatePictures(QString name,SUID group)
{
  PictureView * PV = new PictureView ( Mdi , plan     ) ;
  connectDock ( PV )                                    ;
  nConnect    ( PV   , SIGNAL(Command     (SUID))       ,
                this , SLOT  (ViewPicture (SUID))     ) ;
  nConnect    ( PV   , SIGNAL(Leave       (QWidget*))   ,
                this , SLOT  (PictureLeave(QWidget*)) ) ;
  nConnect    ( PV   , SIGNAL(Stitch(QString,SUID,UUIDs&))   ,
                this , SLOT  (Stitch(QString,SUID,UUIDs&)) ) ;
  PV  -> setFirst      ( group , N::Types::Gallery    ) ;
  Mdi -> Attach        ( PV                           ) ;
  PV  -> MountCommands (                              ) ;
  PV  -> showMaximized (                              ) ;
  if (name.length()>0) PV->setWindowTitle ( name      ) ;
  PV  -> Reload        (                              ) ;
  return PV                                             ;
}

N::PictureView * N::DesktopMain::CreatePictures(QString name,UUIDs & Uuids)
{
  PictureView * PV = new PictureView ( Mdi , plan  ) ;
  connectDock       ( PV                  )          ;
  nConnect ( PV   , SIGNAL(Command     (SUID))       ,
             this , SLOT  (ViewPicture (SUID))     ) ;
  nConnect ( PV   , SIGNAL(Leave       (QWidget*))   ,
             this , SLOT  (PictureLeave(QWidget*)) ) ;
  nConnect ( PV   , SIGNAL(Stitch(QString,SUID,UUIDs&))   ,
             this , SLOT  (Stitch(QString,SUID,UUIDs&)) ) ;
  Mdi -> Attach        ( PV    )                     ;
  PV  -> MountCommands (       )                     ;
  PV  -> show          (       )                     ;
  if (name.length()>0)                               {
    PV->setWindowTitle ( name  )                     ;
  }                                                  ;
  PV  -> View          ( Uuids )                     ;
  Mdi -> Connect       ( PV    )                     ;
  return PV                                          ;
}

N::ObjectGallery * N::DesktopMain::CreateObjectGallery(QString name,SUID uuid,int type)
{
  ObjectGallery * AG = new ObjectGallery ( Mdi , plan  ) ;
  connectDock ( AG )                                     ;
  nConnect ( AG   , SIGNAL(Command     (SUID    ))       ,
             this , SLOT  (ViewPicture (SUID    ))     ) ;
  nConnect ( AG   , SIGNAL(Leave       (QWidget*))       ,
             this , SLOT  (PictureLeave(QWidget*))     ) ;
  nConnect ( AG   , SIGNAL(Stitch(QString,SUID,UUIDs&))   ,
             this , SLOT  (Stitch(QString,SUID,UUIDs&)) ) ;
  AG  -> setFirst      ( uuid , type                   ) ;
  Mdi -> Attach        ( AG                            ) ;
  AG  -> MountCommands (                               ) ;
  AG  -> show          (                               ) ;
  if (name.length()>0)                                   {
    AG->setWindowTitle ( name                          ) ;
  }                                                      ;
  AG  -> Reload        ( true                          ) ;
  return AG                                              ;
}

N::VcfView * N::DesktopMain::OpenPicture(SUID uuid)
{
  VcfView * VPV = new VcfView ( Mdi  , plan ) ;
  VPV -> ViewPicture          ( uuid , Mdi  ) ;
  return VPV                                  ;
}

void N::DesktopMain::EditScripts(void)
{
  BerthIn ( N::ScriptEditor , SE , 0 ) ;
  SE -> startup (  )                   ;
}

void N::DesktopMain::Translations(QString title,UUIDs & Uuids)
{
  QString T = tr("Translation [%1]").arg(title)              ;
  nNewWindow            ( N::ItemNameEditor , INE          ) ;
  Mdi -> Attach         ( INE                              ) ;
  INE -> setWindowTitle ( T                                ) ;
  INE -> show           (                                  ) ;
  INE -> startup        ( Uuids , plan->languages.Supports ) ;
}

void N::DesktopMain::ListItems (
       UUIDs & Uuids           ,
       RMAPs & Types           ,
       NAMEs & Names           ,
       QString title           )
{
  BerthIn             ( N::ObjectLists , NOL , Qt::Vertical ) ;
  NOL->setWindowTitle ( title                               ) ;
  connectDock         ( NOL                                 ) ;
  NOL->startup        ( Uuids,Types,Names                   ) ;
}

void N::DesktopMain::CaptureScreen(void)
{
  ContainerDialog     * NCD = new ContainerDialog     ( this , plan ) ;
  ScreenCaptureMethod * SCM = new ScreenCaptureMethod ( NCD  , plan ) ;
  ///////////////////////////////////////////////////////////////////////////
  NCD  -> setWidget ( SCM )                                                 ;
  plan -> setFont   ( NCD )                                                 ;
  plan -> setFont   ( SCM )                                                 ;
  ///////////////////////////////////////////////////////////////////////////
  if ( QDialog::Accepted == NCD->exec() )                                   {
    NCD  -> hide          ( )                                               ;
    plan -> processEvents ( )                                               ;
    if (SCM->isImmediate())                                                 {
      CapturePath = SCM->Path()                                             ;
      if (SCM->isDesktop())                                                 {
        CaptureDesktop ( )                                                  ;
      } else
      if (SCM->isWindow ())                                                 {
        CaptureThis    ( )                                                  ;
      }                                                                     ;
    } else
    if (SCM->isDelay    ())                                                 {
      CapturePath = SCM->Path()                                             ;
      if (SCM->isDesktop())                                                 {
        int ms = SCM->Seconds() * 1000                                      ;
        QTimer::singleShot(ms,this,SLOT(CaptureThis()))                     ;
      } else
      if (SCM->isWindow ())                                                 {
        int ms = SCM->Seconds() * 1000                                      ;
        QTimer::singleShot(ms,this,SLOT(CaptureDesktop()))                  ;
      }                                                                     ;
    } else
    if (SCM->isPeriod   ())                                                 {
      CapturePath = SCM->Path()                                             ;
      if (SCM->isDesktop())                                                 {
        CaptureSeconds                                                      (
          (QWidget *)QApplication::desktop()                                ,
          CapturePath                                                       ,
          SCM->Seconds() * 1000                                             ,
          SCM->Period () * 1000                                           ) ;
      } else
      if (SCM->isWindow ())                                                 {
        CaptureSeconds                                                      (
          this                                                              ,
          CapturePath                                                       ,
          SCM->Seconds() * 1000                                             ,
          SCM->Period () * 1000                                           ) ;
      }                                                                     ;
    }                                                                       ;
  }                                                                         ;
  ///////////////////////////////////////////////////////////////////////////
  NCD -> deleteLater ( )                                                    ;
}

void N::DesktopMain::CaptureThis(void)
{
  for (int i=0;i<30;i++)                           {
    plan->processEvents()                          ;
  }                                                ;
  QPixmap pix = QPixmap::grabWindow(this->winId()) ;
  SaveCapture(pix,CapturePath)                     ;
}

void N::DesktopMain::CaptureDesktop(void)
{
  QDesktopWidget * dw = QApplication::desktop()    ;
  if (NotNull(dw))                                 {
    for (int i=0;i<30;i++)                         {
      plan->processEvents()                        ;
    }                                              ;
    QPixmap pix = QPixmap::grabWindow(dw->winId()) ;
    SaveCapture(pix,CapturePath)                   ;
  }                                                ;
}

void N::DesktopMain::SaveCapture(QPixmap & pixmap,QString path)
{
  QDateTime DT = nTimeNow                                                      ;
  QDir    d ( path )                                                           ;
  QString filename = QString("%1.png").arg(DT.toString("yyyy-MM-dd-hh-mm-ss")) ;
  filename = d.absoluteFilePath(filename)                                      ;
  filename = QFileDialog::getSaveFileName                                      (
               this                                                            ,
               tr("Save captured image")                                       ,
               filename                                                        ,
               N::Images::ImageFilters()                                     ) ;
  if (filename.length()<=0) return                                             ;
  QImage I = pixmap.toImage()                                                  ;
  I . save ( filename )                                                        ;
  Alert    ( Done     )                                                        ;
}

void N::DesktopMain::CaptureSeconds(QWidget * widget,QDir path,int afterMs,int TotalMs)
{
  if (IsNull(capturer))                     {
    capturer  = new N::ScreenCapturer(this) ;
  } else                                    {
    capturer -> Stop()                      ;
  }                                         ;
  capturer -> setDirectory ( path    )      ;
  capturer -> setWidget    ( widget  )      ;
  capturer -> setMaxTime   ( TotalMs )      ;
  capturer -> setDelay     ( afterMs )      ;
  capturer -> Start        (         )      ;
}

void N::DesktopMain::ViewPicture(QImage image)
{
  VcfView * VPV = new VcfView ( Mdi   , plan ) ;
  VPV -> ViewPicture          ( image , Mdi  ) ;
}

void N::DesktopMain::WhenBusy(void)
{
  plan -> StartBusy ( ) ;
}

void N::DesktopMain::OnRelax(void)
{
  plan -> StopBusy  ( ) ;
}

void N::DesktopMain::run(int Type,ThreadData * data)
{
  VarArgs V = data->Arguments ;
  switch ( Type )             {
    case 10002                :
      emit OnBusy  (   )      ;
      Stitching    ( V )      ;
      emit GoRelax (   )      ;
    break                     ;
  }                           ;
}

void N::DesktopMain::Stitch(QString name,SUID uuid,UUIDs & Uuids)
{ Q_UNUSED   ( name      ) ;
  VarArgs V                ;
  V << uuid                ;
  V << Uuids.count()       ;
  toVariants ( Uuids , V ) ;
  start      ( 10002 , V ) ;
}

void N::DesktopMain::Stitching(VarArgs & V)
{
  int   total = V[1] . toInt       ( ) ;
  UUIDs U                              ;
  for (int i=0;i<total;i++)            {
    U << V [ i + 2 ] . toULongLong ( ) ;
  }                                    ;
  //////////////////////////////////////
  PictureManager PM ( plan )           ;
  QList<QImage> Images                 ;
  EnterSQL ( SC , plan -> sql )        ;
    SUID u                             ;
    foreach (u,U)                      {
      QImage * img                     ;
      img = PM . Picture ( SC , u )    ;
      if (NotNull(img))                {
        Images << *img                 ;
      }                                ;
    }                                  ;
  ErrorSQL ( SC , plan -> sql )        ;
  LeaveSQL ( SC , plan -> sql )        ;
  if (Images.count()<=0) return        ;
  //////////////////////////////////////
  QImage I                             ;
  if (PM.Stitch(I,Images,0.05))        {
    Alert             ( Done  )        ;
    emit WatchPicture ( I     )        ;
  } else                               {
    Alert             ( Error )        ;
  }                                    ;
}

void N::DesktopMain::DockPad(QString scope)
{
  if ( NotNull ( plan -> pad ) ) return           ;
  plan -> pad = new N::ControlPad ( this , plan ) ;
  connectDock ( plan -> pad )                     ;
  plan -> pad -> Scope = scope                    ;
  plan -> pad -> Docking                          (
      this                                        ,
      tr("Control pad")                           ,
      Qt::RightDockWidgetArea                     ,
      Qt::LeftDockWidgetArea                      |
      Qt::RightDockWidgetArea                     |
      Qt::TopDockWidgetArea                       |
      Qt::BottomDockWidgetArea                  ) ;
  plan -> pad -> DockIn ( false )                 ;
}

void N::DesktopMain::ClearClipboard(void)
{
  if ( NotNull(qApp->clipboard()) ) {
    qApp->clipboard()->clear()      ;
  }                                 ;
}

void N::DesktopMain::CloseAll(void)
{
  if ( NotNull(Mdi) )   {
    Mdi -> CloseAll ( ) ;
  }                     ;
}

void N::DesktopMain::SimpleNote(void)
{
  nNewWindow            ( N::PlainEditor , PE             ) ;
  Mdi -> Attach         ( PE                              ) ;
  PE  -> setWindowTitle ( tr("Simple note")               ) ;
  PE  -> setWindowIcon  ( QIcon(":/images/plaintext.png") ) ;
  PE  -> show           (                                 ) ;
}

void N::DesktopMain::ClipboardContents(void)
{
  BerthIn        ( N::ClipboardView , NCV , 0 ) ;
  NCV -> startup (                            ) ;
}

void N::DesktopMain::AttachDevices(void)
{
  int id                                         ;
  id = Menus::DeviceMenuId + Acoustics::Input    ;
  if ( plan->menus.contains(id) )                {
    nConnect ( plan -> menus [ id ]              ,
               SIGNAL ( aboutToShow      ( ) )   ,
               this                              ,
               SLOT   ( ShowInputDevices ( ) ) ) ;
  }                                              ;
  id = Menus::DeviceMenuId + Acoustics::Output   ;
  if ( plan->menus.contains(id) )                {
    nConnect ( plan -> menus [ id ]              ,
               SIGNAL ( aboutToShow      ( ) )   ,
               this                              ,
               SLOT   ( ShowOutputDevices( ) ) ) ;
  }                                              ;
  id = Menus::DeviceMenuId + Acoustics::Camera   ;
  if ( plan->menus.contains(id) )                {
    nConnect ( plan -> menus [ id ]              ,
               SIGNAL ( aboutToShow      ( ) )   ,
               this                              ,
               SLOT   ( ShowCameras      ( ) ) ) ;
  }                                              ;
  id = N::Menus::DeviceMenuId + DeviceCPUs       ;
  if ( plan->menus.contains(id) )                {
    nConnect ( plan -> menus [ id ]              ,
               SIGNAL ( aboutToShow      ( ) )   ,
               this                              ,
               SLOT   ( ShowCPUs         ( ) ) ) ;
  }                                              ;
  id = N::Menus::DeviceMenuId + DeviceNICs       ;
  if ( plan->menus.contains(id) )                {
    nConnect ( plan -> menus [ id ]              ,
               SIGNAL ( aboutToShow      ( ) )   ,
               this                              ,
               SLOT   ( ShowNICs         ( ) ) ) ;
  }                                              ;
}

void N::DesktopMain::ShowInputDevices(void)
{
  AudioInput  ( plan -> menus [ Menus::DeviceMenuId + Acoustics::Input  ] ) ;
}

void N::DesktopMain::ShowOutputDevices(void)
{
  AudioOutput ( plan -> menus [ Menus::DeviceMenuId + Acoustics::Output ] ) ;
}

void N::DesktopMain::ShowCameras(void)
{
  CameraInput ( plan -> menus [ Menus::DeviceMenuId + Acoustics::Camera ] ) ;
}

void N::DesktopMain::WwwBrowser(QUrl url,bool reuse)
{
  WebBrowser * WB = NULL                                  ;
  if ( reuse )                                            {
    QWidget * w                                           ;
    w = Mdi -> findWidget ( "WebBrowser" )                ;
    if (NotNull(w))                                       {
      WB = qobject_cast<WebBrowser *> ( w )               ;
    }                                                     ;
  }                                                       ;
  if (IsNull(WB))                                         {
    WB   = new WebBrowser ( Mdi , plan )                  ;
    WB  -> DockBar = DockBar                              ;
    Mdi -> Attach         ( WB         )                  ;
    WB  -> show           (            )                  ;
    nConnect ( WB   , SIGNAL(Analysis    (WebBrowser*))   ,
               this , SLOT  (AnalysisPage(WebBrowser*)) ) ;
  }                                                       ;
  #ifdef QT_WEBENGINE_LIB
  WB    -> setPath        ( url        )                  ;
  #endif
}

void N::DesktopMain::AnalysisPage(N::WebBrowser * view)
{
}

void N::DesktopMain::CheckCUDA(QMenu * menu)
{
  bool correct = true                                     ;
  int  r       = QtCUDA::Initialize ( )                   ;
  int  devices = 0                                        ;
  if ( 0 == r )                                           {
    devices = QtCUDA::Devices ( )                         ;
    if ( devices <= 0 ) correct = false                   ;
  }                                                       ;
  if ( ! correct )                                        {
    plan -> Booleans [ "CUDA" ] = false                   ;
    menu -> setVisible ( false )                          ;
    return                                                ;
  }                                                       ;
  plan -> Booleans [ "CUDA" ] = true                      ;
  /////////////////////////////////////////////////////////
  int            version = QtCUDA::Version  (      )      ;
  QActionGroup * ag      = new QActionGroup ( menu )      ;
  cudaHandle     cuda                                     ;
  QAction      * aa                                       ;
  QString        m                                        ;
  QString        s                                        ;
  QString        pci                                      ;
  size_t         size                                     ;
  /////////////////////////////////////////////////////////
  s     = tr("CUDA Version %1").arg ( version )           ;
  aa    = menu -> addAction         ( s       )           ;
  menu -> addSeparator              (         )           ;
  aa   -> setEnabled                ( false   )           ;
  /////////////////////////////////////////////////////////
  for (int i=0;i<devices;i++)                             {
    cuda  = QtCUDA::Handle          ( i       )           ;
    s     = QtCUDA::Name            ( cuda    )           ;
    pci   = QtCUDA::PciBus          ( cuda    )           ;
    size  = QtCUDA::Memory          ( cuda    )           ;
    size /= 1024                                          ;
    size /= 1024                                          ;
    m     = tr("%1, %2MB (%3)").arg(s).arg(size).arg(pci) ;
    aa    = menu -> addAction       ( m       )           ;
    aa   -> setCheckable            ( true    )           ;
    aa   -> setData                 ( i       )           ;
    ag   -> addAction               ( aa      )           ;
  }                                                       ;
}

void N::DesktopMain::CheckDisplays(QMenu * display)
{
  QAction * aa                                       ;
  QString   s                                        ;
  if ( qApp->desktop()->isVirtualDesktop() )         {
    s = tr ( "Virtual desktop"    )                  ;
  } else                                             {
    s = tr ( "No virtual desktop" )                  ;
  }                                                  ;
  aa       = display -> addAction ( s     )          ;
  aa      -> setEnabled           ( false )          ;
  display -> addSeparator         (       )          ;
  for (int i=0;i<qApp->desktop()->screenCount();i++) {
    QRect r = qApp->desktop()->screenGeometry(i)     ;
    s = tr    ( "Screen %1 : (%2,%3) %4 x %5"        )
        . arg ( i + 1                                )
        . arg ( r . left   ( )                       )
        . arg ( r . top    ( )                       )
        . arg ( r . width  ( )                       )
        . arg ( r . height ( )                     ) ;
    aa  = display -> addAction ( s     )             ;
    aa -> setEnabled           ( false )             ;
  }                                                  ;
}

void N::DesktopMain::InstallGL(QMenu * gl)
{
  QMap<int,QString> GLI                                                  ;
  QAction         * aaa                                                  ;
  QString           sss                                                  ;
  if ( ! QtGL :: Details ( GLI ) ) return                                ;
  sss  = tr("Version : <%1>" ).arg(GLI [ GL_VERSION                  ] ) ;
  aaa  = gl -> addAction ( sss   )                                       ;
  aaa -> setEnabled      ( false )                                       ;
  sss  = tr("Vendor : <%1>"  ).arg(GLI [ GL_VENDOR                   ] ) ;
  aaa  = gl -> addAction ( sss   )                                       ;
  aaa -> setEnabled      ( false )                                       ;
  sss  = tr("Renderer : <%1>").arg(GLI [ GL_RENDERER                 ] ) ;
  aaa  = gl -> addAction ( sss   )                                       ;
  aaa -> setEnabled      ( false )                                       ;
  sss  = tr("Shading : <%1>" ).arg(GLI [ GL_SHADING_LANGUAGE_VERSION ] ) ;
  aaa  = gl -> addAction ( sss   )                                       ;
  aaa -> setEnabled      ( false )                                       ;
}

QString N::DesktopMain::UpTimeToString(qint64 t)
{
  if ( t <= 0 ) return tr("Not ready")    ;
  qint64  days = t / 86400000             ;
  qint64  msto = t % 86400000             ;
  QString s                               ;
  QTime   T ( 0 , 0 , 0 , 0 )             ;
  T  = T . addMSecs ( msto )              ;
  s  = T . toString ( "hh:mm:ss.zzz" )    ;
  if ( days > 0 )                         {
    s = tr("%1 days %2").arg(days).arg(s) ;
  }                                       ;
  return s                                ;
}

void N::DesktopMain::ShowCPUs(void)
{
  int id = N::Menus::DeviceMenuId + DeviceCPUs    ;
  if ( ! plan -> menus . contains ( id ) ) return ;
  ShowCPU ( plan -> menus [ id ] )                ;
}

void N::DesktopMain::ShowCPU(QMenu * menu)
{
  QAction * aa                                                           ;
  menu -> clear ( )                                                      ;
  ////////////////////////////////////////////////////////////////////////
  if ( NULL == plan -> profiler )                                        {
    aa  = menu->addAction(tr("CPU profiler is not enabled."))            ;
    aa -> setEnabled ( false )                                           ;
    return                                                               ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  qint64 * tmem  = plan->profiler->TuidPointer ( "CPU"    ,"Memory"    ) ;
  qint64 * ppeak = plan->profiler->TuidPointer ( "Process","Peak"      ) ;
  qint64 * pwork = plan->profiler->TuidPointer ( "Process","Working"   ) ;
  qint64 * ppage = plan->profiler->TuidPointer ( "Process","Page"      ) ;
  qint32 * load  = plan->profiler->IntPointer  ( "Process","Load"      ) ;
  qint64 * total = plan->profiler->TuidPointer ( "Process","Total"     ) ;
  qint64 * avail = plan->profiler->TuidPointer ( "Process","Available" ) ;
  menu -> addSection ( tr("Memory") )                                    ;
  if ( NULL != tmem )                                                    {
    QString tx = tr("Physical memory size : %1").arg(N::DataFB(*tmem))   ;
    aa  = menu -> addAction ( tx    )                                    ;
    aa -> setEnabled        ( false )                                    ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  menu -> addSection ( tr("Machine memory") )                            ;
  if ( NULL != load )                                                    {
    QString tx = tr("Machine memory load : %1%").arg(*load)              ;
    aa  = menu -> addAction ( tx    )                                    ;
    aa -> setEnabled        ( false )                                    ;
  }                                                                      ;
  if ( NULL != total )                                                   {
    QString tx = tr("Machine total memory : %1").arg(N::DataFB(*total))  ;
    aa  = menu -> addAction ( tx    )                                    ;
    aa -> setEnabled        ( false )                                    ;
  }                                                                      ;
  if ( NULL != avail )                                                   {
    QString tx = tr("Machine available memory : %1").arg(N::DataFB(*avail)) ;
    aa  = menu -> addAction ( tx    )                                    ;
    aa -> setEnabled        ( false )                                    ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  menu -> addSection ( tr("Process memory") )                            ;
  if ( NULL != pwork )                                                   {
    QString tx = tr("Working memory size : %1").arg(N::DataFB(*pwork))   ;
    aa  = menu -> addAction ( tx    )                                    ;
    aa -> setEnabled        ( false )                                    ;
  }                                                                      ;
  if ( NULL != ppeak )                                                   {
    QString tx = tr("Peak memory size : %1").arg(N::DataFB(*ppeak))      ;
    aa  = menu -> addAction ( tx    )                                    ;
    aa -> setEnabled        ( false )                                    ;
  }                                                                      ;
  if ( NULL != ppage )                                                   {
    QString tx = tr("Page memory size : %1").arg(N::DataFB(*ppage))      ;
    aa  = menu -> addAction ( tx    )                                    ;
    aa -> setEnabled        ( false )                                    ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  menu -> addSection ( tr("Process time") )                              ;
  ////////////////////////////////////////////////////////////////////////
  qint64 bot = plan -> profiler -> BootUpTime      ( )                   ;
  qint64 upt = plan -> profiler -> ApplicationTime ( )                   ;
  aa  = menu->addAction(tr("Application(%1)").arg(UpTimeToString(upt)))  ;
  aa -> setEnabled ( false )                                             ;
  aa  = menu->addAction(tr("Bootup(%1)").arg(UpTimeToString(bot)))       ;
  aa -> setEnabled ( false )                                             ;
}

void N::DesktopMain::SniffTcp(void)
{
  if ( ! plan -> actiongroups . contains ( 20001 ) ) return ;
  QActionGroup * ag = plan -> actiongroups [ 20001 ]        ;
  if ( NULL == ag                                  ) return ;
  QAction      * aa = ag -> checkedAction ( )               ;
  if ( NULL == aa                                  ) return ;
  QString n = aa -> text ( )                                ;
  if ( n . length ( ) <= 0                         ) return ;
  SniffTcp ( n )                                            ;
}

void N::DesktopMain::SniffTcp(QString device)
{
}

void N::DesktopMain::ShowNICs(void)
{
  InstallPCAP ( plan -> menus [ N::Menus::DeviceMenuId + 100 ] ) ;
}

void N::DesktopMain::InstallPCAP(QMenu * menu)
{
  QAction * aa                                                          ;
  menu -> clear ( )                                                     ;
  ///////////////////////////////////////////////////////////////////////
  if ( NULL == pcap )                                                   {
    aa  = menu -> addAction ( tr("Sniff disabled") )                    ;
    aa -> setEnabled        ( false                )                    ;
    return                                                              ;
  }                                                                     ;
  ///////////////////////////////////////////////////////////////////////
  QString dev                                                           ;
  QString err                                                           ;
  QString ver = QtPCAP::Version ( )                                     ;
  ///////////////////////////////////////////////////////////////////////
  if ( ver . length ( ) > 30 )                                          {
    ver = ver . left ( 30    )                                          ;
    ver . append     ( "..." )                                          ;
  }                                                                     ;
  aa    = menu -> addAction ( ver   )                                   ;
  aa   -> setEnabled        ( false )                                   ;
  menu -> addSeparator      (       )                                   ;
  ///////////////////////////////////////////////////////////////////////
  dev = pcap -> Lookup ( err )                                          ;
  if ( dev.length() <= 0 )                                              {
    aa  = menu -> addAction ( tr("Sniff disabled") )                    ;
    aa -> setEnabled        ( false                )                    ;
    return                                                              ;
  }                                                                     ;
  ///////////////////////////////////////////////////////////////////////
  if ( ! pcap -> Probe ( ) )                                            {
    aa  = menu -> addAction ( tr("Sniff disabled") )                    ;
    aa -> setEnabled        ( false                )                    ;
    return                                                              ;
  }                                                                     ;
  ///////////////////////////////////////////////////////////////////////
  QActionGroup * ag = new QActionGroup ( menu )                         ;
  plan -> actiongroups [ 20001 ] = ag                                   ;
  for (int i=0;i<pcap ->Interfaces.count();i++)                         {
    int     tt = pcap -> Interfaces [ i ] . Address . count ( )         ;
    QString ds = pcap -> Interfaces [ i ] . Description                 ;
    QString na = pcap -> Interfaces [ i ] . Name                        ;
    QMenu * mi = menu -> addMenu ( ds )                                 ;
    aa  = mi -> addAction ( na    )                                     ;
    aa -> setCheckable    ( true  )                                     ;
    ag -> addAction       ( aa    )                                     ;
    nConnect ( aa   , SIGNAL ( triggered ( )                          ) ,
               this , SLOT   ( SniffTcp  ( ) )                        ) ;
    mi -> addSeparator    (       )                                     ;
    for (int j = 0 ; j < tt ; j++ )                                     {
      PcapAddress pa = pcap -> Interfaces [ i ] . Address [ j ]         ;
      if ( pa . address     . length ( ) > 0 )                          {
        aa  = mi -> addAction  ( tr("Address : %1").arg(pa.address)   ) ;
        aa       -> setEnabled ( false                                ) ;
      }                                                                 ;
      if ( pa . netmask     . length ( ) > 0 )                          {
        aa  = mi -> addAction  ( tr("Netmask : %1").arg(pa.netmask)   ) ;
        aa       -> setEnabled ( false                                ) ;
      }                                                                 ;
      if ( pa . broadcast   . length ( ) > 0 )                          {
        aa  = mi -> addAction  ( tr("Broadcast : %1").arg(pa.broadcast) ) ;
        aa       -> setEnabled ( false                                ) ;
      }                                                                 ;
      if ( pa . destination . length ( ) > 0 )                          {
        aa  = mi -> addAction  ( tr("Destination : %1").arg(pa.destination) ) ;
        aa       -> setEnabled ( false                                ) ;
      }                                                                 ;
      if ( ( j + 1 ) != tt ) mi -> addSeparator ( )                     ;
    }                                                                   ;
  }                                                                     ;
}

void N::DesktopMain::addEditTool(void)
{
  N::EditToolBar * etb                                     ;
  etb = new N::EditToolBar ( NULL , plan                 ) ;
  addToolBar               ( Qt::BottomToolBarArea , etb ) ;
  etb -> Relocation        (                             ) ;
  etb -> hide              (                             ) ;
}

void N::DesktopMain::RelayCommand(QString method,int port)
{
  QStringList arguments = plan -> Arguments                        ;
  if ( arguments . count ( ) < 2 ) return                          ;
  QHostAddress host                                                ;
  QString      address                                             ;
  host    = plan -> LocalAddress ( QAbstractSocket::IPv4Protocol ) ;
  address = host  . toString     (                               ) ;
  N::XmlRpcClient c              ( address , port                ) ;
  N::XmlRpcValue  Args                                             ;
  N::XmlRpcValue  result                                           ;
  QString         cmd = arguments [ 1 ]                            ;
  //////////////////////////////////////////////////////////////////
  cmd = cmd . toLower ( )                                          ;
  if ( ( "quit"     != cmd )                                      &&
       ( "command"  != cmd )                                      &&
       ( "control"  != cmd )                                      &&
       ( "interact" != cmd )                                      &&
       ( "question" != cmd )                                      &&
       ( "send"     != cmd )                                      &&
       ( "options"  != cmd )                                     ) {
    for (int i=1;i<arguments.count();i++)                          {
      QFileInfo FXI ( arguments [ i ] )                            ;
      if ( FXI . exists ( ) )                                      {
        arguments [ i ] = FXI . absoluteFilePath ( )               ;
      }                                                            ;
    }                                                              ;
  }                                                                ;
  //////////////////////////////////////////////////////////////////
  for (int i=1;i<arguments.count();i++)                            {
    Args [ i - 1 ] = N::XmlRpcValue( arguments [ i ] )             ;
  }                                                                ;
  c . execute ( method.toUtf8().constData() , Args , result )      ;
}

void N::DesktopMain::BeaconCommand(QString method,int port)
{
  QStringList arguments = plan -> Arguments                        ;
  if ( arguments . count ( ) < 2 ) return                          ;
  QHostAddress host                                                ;
  QString      address                                             ;
  host    = plan -> LocalAddress ( QAbstractSocket::IPv4Protocol ) ;
  address = host  . toString     (                               ) ;
  XmlClient c                    ( address , port                ) ;
  XmlValue  Args                                                   ;
  XmlValue  result                                                 ;
  QString         cmd = arguments [ 1 ]                            ;
  //////////////////////////////////////////////////////////////////
  cmd = cmd . toLower ( )                                          ;
  if ( ( "quit"     != cmd )                                      &&
       ( "command"  != cmd )                                      &&
       ( "control"  != cmd )                                      &&
       ( "interact" != cmd )                                      &&
       ( "question" != cmd )                                      &&
       ( "send"     != cmd )                                      &&
       ( "options"  != cmd )                                     ) {
    for (int i=1;i<arguments.count();i++)                          {
      QFileInfo FXI ( arguments [ i ] )                            ;
      if ( FXI . exists ( ) )                                      {
        arguments [ i ] = FXI . absoluteFilePath ( )               ;
      }                                                            ;
    }                                                              ;
  }                                                                ;
  //////////////////////////////////////////////////////////////////
  for (int i=1;i<arguments.count();i++)                            {
    Args [ i - 1 ] = XmlValue( arguments [ i ] )                   ;
  }                                                                ;
  c . execute ( method , Args , result )                           ;
}

void N::DesktopMain::FullNames(SUID u)
{
  BerthIn              ( N::NameComplex , NNC , Qt::Vertical ) ;
  NNC -> setObjectUuid ( u                                   ) ;
  NNC -> show          (                                     ) ;
  NNC -> startup       (                                     ) ;
}

void N::DesktopMain::CompleteNames(QString n,SUID u)
{ Q_UNUSED  ( n ) ;
  FullNames ( u ) ;
}
