#include <domination.h>

//////////////////////////////////////////////////////////////////////////////

static QScriptValue ssCreate(QScriptContext * context,QScriptEngine * engine)
{
  N::ScriptStrings * ss = new N::ScriptStrings(engine) ;
  ss->setupTranslation()                               ;
  return engine->newQObject(ss)                        ;
}

//////////////////////////////////////////////////////////////////////////////

N::SearchPanel:: SearchPanel   ( QWidget * parent , Plan * p )
               : StackedWidget (           parent ,        p )
               , wrapper       ( NULL                        )
               , watcher       ( NULL                        )
               , splitter      ( NULL                        )
               , engines       ( NULL                        )
               , line          ( NULL                        )
               , loading       ( false                       )
{
  Configure ( ) ;
}

N::SearchPanel::~SearchPanel (void)
{
}

QSize N::SearchPanel::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 384 , 48 ) ) ;
}

void N::SearchPanel::Configure(void)
{
  QSize s ( 384 , 48 )                                        ;
  /////////////////////////////////////////////////////////////
  setMinimumSize ( s )                                        ;
  setMaximumSize ( s )                                        ;
  /////////////////////////////////////////////////////////////
  js       = plan -> Path           ( Directory::Scripts    ) ;
  splitter = new QSplitter          ( Qt::Vertical , this   ) ;
  engines  = new ComboBox           ( splitter     , plan   ) ;
  line     = new QLineEdit          ( splitter              ) ;
  /////////////////////////////////////////////////////////////
  engines -> setEditable            ( true                  ) ;
  engines -> setInsertPolicy        ( QComboBox::NoInsert   ) ;
  engines -> setMinimumHeight       ( 24                    ) ;
  engines -> setMaximumHeight       ( 24                    ) ;
  engines -> setToolTip             ( tr("Search engines")  ) ;
  /////////////////////////////////////////////////////////////
  line    -> setMinimumHeight       ( 24                    ) ;
  line    -> setMaximumHeight       ( 24                    ) ;
  line    -> setPlaceholderText     ( tr("Search keywords") ) ;
  /////////////////////////////////////////////////////////////
  addWidget                         ( splitter              ) ;
  splitter -> setHandleWidth        ( 0                     ) ;
  splitter -> addWidget             ( engines               ) ;
  splitter -> addWidget             ( line                  ) ;
  /////////////////////////////////////////////////////////////
  js = js . absoluteFilePath        ( "Search"              ) ;
  setPath                           ( js                    ) ;
  nConnect            ( line , SIGNAL ( returnPressed ( ) )   ,
                        this , SLOT   ( GoSearch      ( ) ) ) ;
}

void N::SearchPanel::setPath(QDir & dir)
{
  if ( NotNull(watcher) ) watcher -> deleteLater (       ) ;
  watcher  = new QFileSystemWatcher ( this               ) ;
  js       = dir                                           ;
  LoadScripts        (                                   ) ;
  watcher -> addPath ( js.absoluteFilePath("")           ) ;
  connect ( watcher , SIGNAL (directoryChanged(QString))   ,
            this    , SLOT   (changed         (QString)) ) ;
  connect ( watcher , SIGNAL (fileChanged     (QString))   ,
            this    , SLOT   (changed         (QString)) ) ;
}

void N::SearchPanel::changed(const QString &)
{
  if ( loading ) return ;
  LoadScripts ( )       ;
}

void N::SearchPanel::setWrapper(ToolWrapper * w)
{
  wrapper = w ;
}

void N::SearchPanel::Appear(void)
{
  QWidget * w = this                 ;
  if ( NULL != wrapper ) w = wrapper ;
  nDropOut ( w -> isVisible ( ) )    ;
  FadeIn   ( w , 25             )    ;
}

void N::SearchPanel::Disappear(void)
{
  QWidget * w = this     ;
  if ( NULL != wrapper ) {
    w = wrapper          ;
  }                      ;
  w -> hide ( )          ;
}

void N::SearchPanel::Eradicate(void)
{
  QWidget * w = this                 ;
  if ( NULL != wrapper ) w = wrapper ;
  w -> deleteLater ( )               ;
}

void N::SearchPanel::ObtainFocus (void)
{
  line -> setFocus ( Qt::ActiveWindowFocusReason ) ;
}

void N::SearchPanel::LoadScripts (void)
{
  QStringList   filters                              ;
  QFileInfoList fil                                  ;
  loading = true                                     ;
  engines -> clear ( )                               ;
  Scripts  . clear ( )                               ;
  filters << "*.js"                                  ;
  fil = js . entryInfoList ( filters , QDir::Files ) ;
  for (int i=0;i<fil.count();i++)                    {
    QString    path = fil[i].absoluteFilePath()      ;
    QString    name = fil[i].fileName()              ;
    QByteArray body                                  ;
    N::File::toByteArray ( path , body )             ;
    if ( body . size ( ) > 0 )                       {
      QString          s                             ;
      QString          result                        ;
      QScriptEngine    engine                        ;
      QScriptValue     func                          ;
      QScriptValue     global                        ;
      QScriptValue     ename                         ;
      QScriptValue     sname                         ;
      QScriptValue     Again                         ;
      QScriptValueList args                          ;
      s = QString :: fromUtf8 ( body )               ;
      engine.globalObject().setProperty              (
        "ScriptStrings"                              ,
        engine.newFunction(ssCreate)               ) ;
      result  = ""                                   ;
      func    = engine . evaluate ( s              ) ;
      global  = engine . globalObject (            ) ;
      ////////////////////////////////////////////////
      ename   = global . property ( "EngineName"   ) ;
      sname   = global . property ( "SearchEngine" ) ;
      if (ename.isFunction() && sname.isFunction() ) {
        Again   = ename  . call     ( func , args  ) ;
        result  = Again  . toString (              ) ;
        if ( result . length ( ) > 0 )               {
          int id = engines -> count ( )              ;
          name           = result                    ;
          Scripts [ id ] = s                         ;
          engines -> addItem ( name )                ;
        }                                            ;
      }                                              ;
    }                                                ;
  }                                                  ;
  loading = false                                    ;
}

void N::SearchPanel::GoSearch(void)
{
  int     id   = engines -> currentIndex ( )           ;
  QString text = line    -> text         ( )           ;
  if ( id                <  0 ) return                 ;
  if ( text . length ( ) <= 0 ) return                 ;
  //////////////////////////////////////////////////////
  QString          s    = Scripts [ id ]               ;
  QString          result                              ;
  QScriptEngine    engine                              ;
  QScriptValue     func                                ;
  QScriptValue     global                              ;
  QScriptValue     sname                               ;
  QScriptValue     Again                               ;
  QScriptValueList args                                ;
  //////////////////////////////////////////////////////
  args << text                                         ;
  engine.globalObject().setProperty                    (
    "ScriptStrings"                                    ,
    engine.newFunction(ssCreate)                     ) ;
  result  = ""                                         ;
  func    = engine . evaluate ( s              )       ;
  global  = engine . globalObject (            )       ;
  //////////////////////////////////////////////////////
  sname   = global . property ( "SearchEngine" )       ;
  Again   = sname  . call     ( func , args    )       ;
  result  = Again  . toString (                )       ;
  if ( result . length ( ) > 0 )                       {
    QUrl url ( result )                                ;
    QDesktopServices :: openUrl ( url )                ;
  } else                                               {
    Notify ( tr("Search engine")                       ,
             tr("Script %1 is possible incorrect"      )
             .arg(engines->currentText())            ) ;
  }                                                    ;
  //////////////////////////////////////////////////////
  if (!strings.contains(text))                         {
    QCompleter * cp                                    ;
    strings << text                                    ;
    cp    = new QCompleter     ( strings , this      ) ;
    cp   -> setCaseSensitivity ( Qt::CaseInsensitive ) ;
    line -> setCompleter       ( cp                  ) ;
  }                                                    ;
}
