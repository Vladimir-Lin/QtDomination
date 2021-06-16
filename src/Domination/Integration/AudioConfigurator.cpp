#include <domination.h>

//#@ dependency : N::Splitter
//#@ dependency : N::StackedWidget
//#@ dependency : QToolButton
//#@ dependency : N::ComboBox
//#@ dependency : N::DirectoryEditor
//#@ dependency : N::PickAudio

N::AudioConfigurator:: AudioConfigurator ( QWidget      * parent , Plan * p )
                     : Splitter          ( Qt::Vertical , parent ,        p )
                     , bar               ( NULL                             )
                     , stack             ( NULL                             )
                     , button            ( NULL                             )
                     , item              ( NULL                             )
                     , output            ( NULL                             )
                     , input             ( NULL                             )
                     , directory         ( NULL                             )
{
  Configure ( ) ;
}

N::AudioConfigurator::~AudioConfigurator (void)
{
}

QSize N::AudioConfigurator::sizeHint(void) const
{
  return QSize ( 480 , 480 ) ;
}

void N::AudioConfigurator::Configure (void)
{
  setWindowTitle ( tr ( "Audio configuration" )  )                 ;
  Notify    = (SUID)N::Command::Click                              ;
  //////////////////////////////////////////////////////////////////
  bar       = new Splitter        ( Qt::Horizontal , this , plan ) ;
  stack     = new StackedWidget   (                  this , plan ) ;
  button    = new QToolButton     ( bar                          ) ;
  item      = new ComboBox        ( bar                   , plan ) ;
  output    = new PickAudio       ( stack                 , plan ) ;
  input     = new PickAudio       ( stack                 , plan ) ;
  directory = new DirectoryEditor ( stack                 , plan ) ;
  //////////////////////////////////////////////////////////////////
  addWidget                       ( bar                          ) ;
  addWidget                       ( stack                        ) ;
  bar    -> addWidget             ( button                       ) ;
  bar    -> addWidget             ( item                         ) ;
  stack  -> addWidget             ( output                       ) ;
  stack  -> addWidget             ( input                        ) ;
  stack  -> addWidget             ( directory                    ) ;
  //////////////////////////////////////////////////////////////////
  setHandleWidth                  ( 0                            ) ;
  bar    -> setHandleWidth        ( 1                            ) ;
  //////////////////////////////////////////////////////////////////
  item   -> addItem               ( tr("Audio output device")    ) ;
  item   -> addItem               ( tr("Audio input device" )    ) ;
  item   -> addItem               ( tr("Audio directories"  )    ) ;
  item   -> setMinimumHeight      ( 28                           ) ;
  item   -> setMaximumHeight      ( 28                           ) ;
  item   -> setEditable           ( true                         ) ;
  //////////////////////////////////////////////////////////////////
  QSize S ( 120 , 28 )                                             ;
  button -> setAutoRaise          ( true                         ) ;
  button -> setToolButtonStyle    ( Qt::ToolButtonTextOnly       ) ;
  button -> setText               ( tr("Back")                   ) ;
  button -> setMinimumSize        ( S                            ) ;
  button -> setMaximumSize        ( S                            ) ;
  //////////////////////////////////////////////////////////////////
  bar    -> setMinimumHeight      ( 28                           ) ;
  bar    -> setMaximumHeight      ( 28                           ) ;
  //////////////////////////////////////////////////////////////////
  if ( plan -> EquipmentIDs . contains(Plan::DefaultAudioOutput) ) {
    int id = plan->EquipmentIDs[Plan::DefaultAudioOutput]          ;
    output -> setDefault          ( id                           ) ;
  }                                                                ;
  output -> List                  ( Acoustics::Output            ) ;
  output -> setNotify             ( Notify                       ) ;
  output -> setClickable          ( 1                            ) ;
  //////////////////////////////////////////////////////////////////
  if ( plan -> EquipmentIDs . contains(Plan::DefaultAudioInput)  ) {
    int id = plan->EquipmentIDs[Plan::DefaultAudioInput]           ;
    input  -> setDefault          ( id                           ) ;
  }                                                                ;
  input  -> List                  ( Acoustics::Input             ) ;
  input  -> setNotify             ( Notify                       ) ;
  input  -> setClickable          ( 1                            ) ;
  //////////////////////////////////////////////////////////////////
//  directory -> setScope           ( "AudioDepot"                 ) ;
  //////////////////////////////////////////////////////////////////
  button -> hide                  (                              ) ;
  //////////////////////////////////////////////////////////////////
  connect              ( output , SIGNAL ( DeviceChanged  (int))   ,
                         this   , SLOT   ( Output         (int)) ) ;
  connect              ( input  , SIGNAL ( DeviceChanged  (int))   ,
                         this   , SLOT   ( Input          (int)) ) ;
  connect              ( item   , SIGNAL ( activated      (int))   ,
                         stack  , SLOT   ( setCurrentIndex(int)) ) ;
}

void N::AudioConfigurator::setClickable(int click)
{
  output -> setClickable ( click ) ;
  input  -> setClickable ( click ) ;
}

void N::AudioConfigurator::setIcon(bool enable,QIcon icon)
{
  output -> setIcon ( enable , icon ) ;
  input  -> setIcon ( enable , icon ) ;
}

void N::AudioConfigurator::setNotify(SUID notify)
{
  Notify     = notify                     ;
  output    -> setNotify ( Notify       ) ;
  input     -> setNotify ( Notify       ) ;
#pragma message("Temporary use this way")
  directory -> setScope  ( "AudioDepot" ) ;
}

void N::AudioConfigurator::setVisible(bool visible,QString text)
{
  button -> setText ( text )    ;
  if ( visible ) button->show() ;
            else button->hide() ;
}

void N::AudioConfigurator::Input(int device)
{
  QMutexLocker locker ( &mutex )                              ;
  int     deviceId = device                                   ;
  QString DEVICE   = input  -> deviceName ( )                 ;
  plan->Equipments  [Plan::DefaultAudioInput ] = DEVICE       ;
  plan->EquipmentIDs[Plan::DefaultAudioInput ] = deviceId     ;
  plan->settings.beginGroup ("Interface"                    ) ;
  plan->settings.setValue   ("AudioInput"   ,DEVICE.toUtf8()) ;
  plan->settings.setValue   ("AudioInputID" ,deviceId       ) ;
  plan->settings.endGroup   (                               ) ;
}

void N::AudioConfigurator::Output(int device)
{
  QMutexLocker locker ( &mutex )                              ;
  int     deviceId = device                                   ;
  QString DEVICE   = output -> deviceName ( )                 ;
  plan->Equipments  [Plan::DefaultAudioOutput] = DEVICE       ;
  plan->EquipmentIDs[Plan::DefaultAudioOutput] = deviceId     ;
  plan->settings.beginGroup ("Interface"                    ) ;
  plan->settings.setValue   ("AudioOutput"  ,DEVICE.toUtf8()) ;
  plan->settings.setValue   ("AudioOutputID",deviceId       ) ;
  plan->settings.endGroup   (                               ) ;
}
