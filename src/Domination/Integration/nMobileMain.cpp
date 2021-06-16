#include <domination.h>

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#else
#define DISABLE_MOBILE 1
#endif

////////////////////////////////////////////////////////////////////////////

#define Decide decisions[GuiMainId]

#ifdef DISABLE_DESKTOP

#else

static N::ConditionItem DesktopStates[]   =  {
  { N::MobileMain::Configured      , false } ,
  { N::MobileMain::NoAccess        , false } ,
  { N::MobileMain::Loading         , false } ,
  { N::MobileMain::Loaded          , false } ,
  { N::MobileMain::Started         , false } ,

  { N::MobileMain::HasConfigurator , false } ,
  { N::MobileMain::HasStacked      , true  } ,
  { N::MobileMain::HasMdi          , true  } ,
  { N::MobileMain::HasDebugger     , true  } ,
  { N::MobileMain::AcceptMenu      , true  } ,

  { N::MobileMain::StackedGui      , false } ,
  { N::MobileMain::MdiGui          , true  } ,
  { N::MobileMain::Visibility      , false } ,
  { N::MobileMain::Brand           , false } ,
  { N::MobileMain::Advertising     , false } ,

  { N::MobileMain::Interactive     , false } ,
  { N::MobileMain::Leaving         , false } ,
  { N::MobileMain::ShutOff         , false } ,
  { N::MobileMain::Storing         , false } ,
  { N::MobileMain::Closing         , false }
}                                            ;

static N::ConditionItem PrepareNDB []     =  {
  { N::MobileMain::Configured      , false } ,
  { N::MobileMain::NoAccess        , false } ,
}                                            ;

static N::ConditionItem EnterConf  []     =  {
  { N::MobileMain::Configured      , false } ,
  { N::MobileMain::NoAccess        , true  } ,
  { N::MobileMain::HasConfigurator , true  } ,
}                                            ;

static N::ConditionItem CanNotEnter[]     =  {
  { N::MobileMain::Configured      , false } ,
  { N::MobileMain::NoAccess        , true  } ,
  { N::MobileMain::HasConfigurator , false } ,
}                                            ;

static N::ConditionItem ReadNDB    []     =  {
  { N::MobileMain::Configured      , true  } ,
  { N::MobileMain::NoAccess        , false } ,
  { N::MobileMain::Loading         , false } ,
  { N::MobileMain::Loaded          , false } ,
  { N::MobileMain::Started         , false } ,
}                                            ;

static N::ConditionItem StartBrand []     =  {
  { N::MobileMain::Configured      , true  } ,
  { N::MobileMain::NoAccess        , false } ,
  { N::MobileMain::Loading         , false } ,
  { N::MobileMain::Loaded          , true  } ,
  { N::MobileMain::Started         , false } ,
  { N::MobileMain::Brand           , false } ,
}                                            ;

static N::ConditionItem StartAdvert []    =  {
  { N::MobileMain::Loaded          , true  } ,
  { N::MobileMain::Started         , false } ,
  { N::MobileMain::Brand           , true  } ,
  { N::MobileMain::Advertising     , false } ,
  { N::MobileMain::Interactive     , false } ,
}                                            ;

static N::ConditionItem StartPrimary []   =  {
  { N::MobileMain::Loaded          , true  } ,
  { N::MobileMain::Started         , false } ,
  { N::MobileMain::Brand           , true  } ,
  { N::MobileMain::Advertising     , true  } ,
  { N::MobileMain::Interactive     , false } ,
}                                            ;

#endif

////////////////////////////////////////////////////////////////////////////

N::MobileMain:: MobileMain (int argc,char ** argv,QWidget * parent)
              : CommonMain (    argc,        argv,          parent)
{
  WidgetClass   ;
#ifdef DISABLE_MOBILE
#else
  allowGesture = true                         ;
  decisions . Blank ( GuiMainId             ) ;
  grabGesture       ( Qt::TapGesture        ) ;
  grabGesture       ( Qt::TapAndHoldGesture ) ;
  grabGesture       ( Qt::PanGesture        ) ;
  grabGesture       ( Qt::PinchGesture      ) ;
  grabGesture       ( Qt::SwipeGesture      ) ;
#endif
}

N::MobileMain::~MobileMain (void)
{
}

void N::MobileMain::InstallDecisions(void)
{
#ifdef DISABLE_MOBILE
#else
  Decide . setConditions ( 20 , DesktopStates   ) ;
  /////////////////////////////////////////////////
  DecisionAction A01                              ;
  A01 . action = N::MobileMain::Preparation       ; // prepare
  A01 . addConditions    (  2 , PrepareNDB      ) ;
  /////////////////////////////////////////////////
  DecisionAction A02                              ;
  A02 . action = N::MobileMain::Configuration     ; // EnterConf   => configure
  A02 . addConditions    (  3 , EnterConf       )      ;
  /////////////////////////////////////////////////
  DecisionAction A03                              ;
  A03 . action = N::MobileMain::Failure           ; // CanNotEnter => failure
  A03 . addConditions    (  3 , CanNotEnter     ) ;
  /////////////////////////////////////////////////
  DecisionAction A04                              ;
  A04 . action = N::MobileMain::Warmup            ; // Warmup      => loading
  A04 . addConditions    (  5 , ReadNDB         ) ;
  /////////////////////////////////////////////////
  DecisionAction A05                              ;
  A05 . action = N::MobileMain::Brand             ; // Brand       => brand
  A05 . addConditions    (  6 , StartBrand      ) ;
  /////////////////////////////////////////////////
  DecisionAction A06                              ;
  A06 . action = N::MobileMain::Advertising       ; // Advertising => advertising
  A06 . addConditions    (  5 , StartAdvert     ) ;
  /////////////////////////////////////////////////
  DecisionAction A07                              ;
  A07 . action = N::MobileMain::Primary           ; // Primary     => primary
  A07 . addConditions    (  5 , StartPrimary    ) ;
  /////////////////////////////////////////////////
  Decide . addAction ( A01 )                      ;
  Decide . addAction ( A02 )                      ;
  Decide . addAction ( A03 )                      ;
  Decide . addAction ( A04 )                      ;
  Decide . addAction ( A05 )                      ;
  Decide . addAction ( A06 )                      ;
  Decide . addAction ( A07 )                      ;
  /////////////////////////////////////////////////
  QRect R   = qApp->desktop()->screenGeometry()   ;
  bool  pad = true                                ;
  if ( R . width  ( ) < 600 ) pad = false         ;
  if ( R . height ( ) < 600 ) pad = false         ;
  plan  -> Booleans [ "Desktop" ] = false         ;
  plan  -> Booleans [ "Pad"     ] =   pad         ;
  plan  -> Booleans [ "Phone"   ] = ! pad         ;
#endif
}

bool N::MobileMain::DefaultBootup(void)
{
#ifdef DISABLE_MOBILE
  return false                    ;
#else
  UUIDs react                     ;
  SUID  u                         ;
  InstallDecisions ( )            ;
  do                              {
    plan -> processEvents ( )     ;
    react = Decide.reactions()    ;
    if (react.count()>0)          {
      foreach (u,react)           {
        plan -> processEvents ( ) ;
        switch (u)                {
          case Preparation        :
            prepare     ( )       ;
          break                   ;
          case Failure            :
          return false            ;
          case Configuration      :
            configure   ( )       ;
          break                   ;
          case Warmup             :
            loading     ( )       ;
          break                   ;
          case Brand              :
            brand       ( )       ;
          break                   ;
          case Advertising        :
            advertising ( )       ;
          break                   ;
          case Primary            :
            primary     ( )       ;
          break                   ;
        }                         ;
      }                           ;
    }                             ;
  } while (react.count()>0)       ;
  return true                     ;
#endif
}

void N::MobileMain::InstallDirs(void)
{
  #define DPATH(NAME,V)                                      \
    Nextstep        (                                    ) ; \
    plan -> Dirs    [ Directory::NAME ] = plan->Path ( V ) ; \
    plan -> setPath ( Directory::NAME , "Directory" , V  )
  #ifdef Q_OS_ANDROID
  DPATH(Images     ,"files/Images"     )                     ;
  DPATH(Sounds     ,"files/Sounds"     )                     ;
  DPATH(Download   ,"files/Download"   )                     ;
  DPATH(Upload     ,"files/Upload"     )                     ;
  DPATH(History    ,"files/History"    )                     ;
  DPATH(Development,"files/Development")                     ;
  DPATH(Plugins    ,"plugins"          )                     ;
  DPATH(Player     ,"files/Player"     )                     ;
  DPATH(Web        ,"files/Web"        )                     ;
  DPATH(Scripts    ,"files/Scripts"    )                     ;
  DPATH(Qt         ,"files/Qt"         )                     ;
  DPATH(MetaTrader ,"files/MetaTrader" )                     ;
  DPATH(Include    ,"files/include"    )                     ;
  DPATH(Help       ,"files/Help"       )                     ;
  DPATH(Backup     ,"files/Backup"     )                     ;
  DPATH(Android    ,"files"            )                     ;
  DPATH(Temporary  ,"cache"            )                     ;
  DPATH(Users      ,"files/Users"      )                     ;
  DPATH(Binary     ,"files/Bin"        )                     ;
  #endif
  #ifdef Q_OS_IOS
  DPATH(Images     ,"Documents/Images"     )                 ;
  DPATH(Sounds     ,"Documents/Sounds"     )                 ;
  DPATH(Download   ,"Documents/Download"   )                 ;
  DPATH(Upload     ,"Documents/Upload"     )                 ;
  DPATH(History    ,"Documents/History"    )                 ;
  DPATH(Development,"Documents/Development")                 ;
  DPATH(Plugins    ,"plugins"              )                 ;
  DPATH(Player     ,"Documents/Player"     )                 ;
  DPATH(Web        ,"Documents/Web"        )                 ;
  DPATH(Scripts    ,"Documents/Scripts"    )                 ;
  DPATH(Qt         ,"Documents/Qt"         )                 ;
  DPATH(MetaTrader ,"Documents/MetaTrader" )                 ;
  DPATH(Include    ,"Documents/include"    )                 ;
  DPATH(Help       ,"Documents/Help"       )                 ;
  DPATH(Backup     ,"Documents/Backup"     )                 ;
  DPATH(Android    ,"Documents"            )                 ;
  DPATH(Temporary  ,"cache"                )                 ;
  DPATH(Users      ,"Documents/Users"      )                 ;
  DPATH(Binary     ,"Documents/Bin"        )                 ;
  #endif
  #undef  DPATH
}

void N::MobileMain::InstallSounds(void)
{
  Nextstep ( )                                                    ;
  plan -> AudioDepots << MachinePath("Sounds")                    ;
  Nextstep ( )                                                    ;
  plan -> Audio    . setParent ( this )                           ;
  Nextstep ( )                                                    ;
  plan -> Recorder . setParent ( this )                           ;
  Nextstep ( )                                                    ;
  plan -> addSound ( N::Command::Wrong   ,"Wrong"   ,"Wrong"    ) ;
  Nextstep ( )                                                    ;
  plan -> addSound ( N::Command::Click   ,"Click"   ,"Click"    ) ;
  Nextstep ( )                                                    ;
  plan -> addSound ( N::Command::Selected,"Selected","Selected" ) ;
  Nextstep ( )                                                    ;
  plan -> addSound ( N::Command::Menu    ,"Menu"    ,"Menu"     ) ;
  Nextstep ( )                                                    ;
  plan -> addSound ( N::Command::Error   ,"Error"   ,"Error"    ) ;
  Nextstep ( )                                                    ;
  plan -> addSound ( N::Command::Done    ,"Done"    ,"Done"     ) ;
  Nextstep ( )                                                    ;
  plan -> addSound ( N::Command::Action  ,"Action"  ,"Action"   ) ;
}

QString N::MobileMain::MachinePath(QString path)
{
  #ifdef Q_OS_ANDROID
  return plan -> Path ( QString("files/%1"    ).arg(path) ) ;
  #endif
  #ifdef Q_OS_IOS
  return plan -> Path ( QString("Documents/%1").arg(path) ) ;
  #endif
  return plan -> Path ( path )                              ;
}

bool N::MobileMain::TurnOff(void)
{
  return true ;
}

bool N::MobileMain::Deposit(void)
{
  return true ;
}

void N::MobileMain::Nextstep(void)
{
}

bool N::MobileMain::DefaultEnding(void)
{
  if (!Decide.conditions[Leaving])    {
    bool sd = Shutdown()              ;
    if (!sd) return false             ;
    Decide.setCondition(Leaving,true) ;
  }                                   ;
  if (!Decide.conditions[ShutOff])    {
    bool sd = TurnOff()               ;
    if (!sd) return false             ;
    Decide.setCondition(ShutOff,true) ;
  }                                   ;
  if (!Decide.conditions[Storing])    {
    bool sd = Deposit()               ;
    if (!sd) return false             ;
    Decide.setCondition(Storing,true) ;
  }                                   ;
  Decide.setCondition(Closing,true)   ;
  qApp -> closeAllWindows        ( )  ;
  qApp -> quitOnLastWindowClosed ( )  ;
  return true                         ;
}

bool N::MobileMain::startup(void)
{
  return DefaultBootup ( ) ;
}

void N::MobileMain::setVisibility(bool visibility)
{ Q_UNUSED ( visibility ) ;
}
