#include <domination.h>

N::CiosFloating:: CiosFloating ( QWidget * parent , Plan * p )
                : Widget       (           parent ,        p )
                , uuid         ( 0                           )
                , type         ( 0                           )
                , columns      ( 6                           )
                , Fading       ( false                       )
                , wrapper      ( NULL                        )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::CiosFloating::~CiosFloating (void)
{
}

bool & N::CiosFloating::isFading(void)
{
  return Fading ;
}

void N::CiosFloating::Configure(void)
{
  iconSize = QSize ( 64 , 64 ) ;
  table    = "branonpanel"     ;
  item     = "branon"          ;
  plan -> setFont  ( this    ) ;
}

void N::CiosFloating::setColumns(int c)
{
  columns = c ;
}

void N::CiosFloating::setButtonSize(QSize s)
{
  iconSize = s ;
}

void N::CiosFloating::setTable(QString t)
{
  table = t ;
}

void N::CiosFloating::setItem(QString it)
{
  item = it ;
}

void N::CiosFloating::setWrapper(ToolWrapper * w)
{
  wrapper  = w                     ;
  wrapper -> setWidget   ( this  ) ;
  wrapper -> setAbstract ( this  ) ;
  move                   ( 0 , 0 ) ;
}

void N::CiosFloating::Arrangement(void)
{
  QWidget * w = this              ;
  if       ( NULL != wrapper    ) {
    w = wrapper                   ;
  }                               ;
  Fading = true                   ;
  display  (                    ) ;
  FadeIn   ( w , 10             ) ;
  setFocus ( Qt::TabFocusReason ) ;
  Fading = false                  ;
}

void N::CiosFloating::FadeOut(void)
{
  QWidget * w = this              ;
  if ( NULL != wrapper )          {
    w = wrapper                   ;
  }                               ;
  Fading = true                   ;
  AbstractGui::FadeOut ( w , 10 ) ;
  hide                 (        ) ;
  Fading = false                  ;
}

void N::CiosFloating::clear(void)
{
  for (int i = 0 ; i < buttons . count ( ) ; i++ ) {
    buttons [ i ] -> deleteLater ( )               ;
  }                                                ;
  for (int i = 0 ; i < actions . count ( ) ; i++ ) {
    actions [ i ] -> deleteLater ( )               ;
  }                                                ;
  buttons    . clear ( )                           ;
  actions    . clear ( )                           ;
  CatalogMap . clear ( )                           ;
  uuid       = 0                                   ;
  type       = 0                                   ;
}

void N::CiosFloating::display(void)
{
  for (int i = 0 ; i < buttons . count ( ) ; i++ )                   {
    int x = i % columns                                              ;
    int y = i / columns                                              ;
    x *= iconSize . width  ( )                                       ;
    y *= iconSize . height ( )                                       ;
    QRect R ( x , y , iconSize . width ( ) , iconSize . height ( ) ) ;
    buttons [ i ] -> setEnabled  ( true )                            ;
    buttons [ i ] -> setGeometry ( R    )                            ;
  }                                                                  ;
}

void N::CiosFloating::disappear(void)
{
  if ( NULL != wrapper )       {
    wrapper -> deleteLater ( ) ;
  } else                       {
    deleteLater            ( ) ;
  }                            ;
}

void N::CiosFloating::add(SUID Id,QString text,QIcon icon)
{
  QToolButton * T = new QToolButton ( this     )    ;
  QAction     * A = new QAction     ( T        )    ;
  T -> setIconSize                  ( iconSize )    ;
  T -> setAutoRaise                 ( true     )    ;
  A -> setIcon                      ( icon     )    ;
  A -> setToolTip                   ( text     )    ;
  T -> setDefaultAction             ( A        )    ;
  buttons          << T                             ;
  actions          << A                             ;
  CatalogMap [ A  ] = Id                            ;
  ButtonIDs  [ Id ] = T                             ;
  ActionIDs  [ Id ] = A                             ;
  nConnect ( T    , SIGNAL (triggered(QAction*) )   ,
             this , SLOT   (activate (QAction*) ) ) ;
}

void N::CiosFloating::activate(QAction * a)
{
  SUID u = CatalogMap [ a ]                              ;
  ButtonIDs [ u ] -> setEnabled ( false                ) ;
  emit Execute                  ( u , a -> toolTip ( ) ) ;
  ButtonIDs [ u ] -> setEnabled ( true                 ) ;
}

void N::CiosFloating::assign(SUID u,int t,QRect Rect,int user)
{
  int  total = 0                                    ;
  int  items = 0                                    ;
  clear ( )                                         ;
  uuid       = u                                    ;
  type       = t                                    ;
  SqlConnection SC ( plan -> sql )                  ;
  if ( SC . open ( FunctionString ) )               {
    UUIDs   U                                       ;
    SUID    u                                       ;
    QString Q                                       ;
    Q = SC . sql . SelectFrom                       (
          "count(*)"                                ,
          table                                     ,
          QString ( "where `%1` = %2"               )
          . arg   ( item                            )
          . arg   ( uuid                        ) ) ;
    if ( SC . Fetch ( Q ) ) total = SC . Int ( 0 )  ;
    U . clear()                                     ;
    U = SC . Uuids                                  (
          table                                     ,
          "execution"                               ,
          QString ( "where `%1` = %2 %3"            )
          . arg   ( item                            )
          . arg   ( uuid                            )
          . arg   ( SC . OrderByAsc("position"))  ) ;
    foreach       ( u , U                         ) {
      Execution ne                                  ;
      ne . clear  (                               ) ;
      ne . Load   ( plan , SC , u                 ) ;
      add         ( u , ne . Name , ne . Icon     ) ;
    }                                               ;
    SC . close    (                               ) ;
  }                                                 ;
  SC   . remove   (                               ) ;
  ///////////////////////////////////////////////////
  items = total                                     ;
  if ( type == 2 ) items += user                    ;
  ///////////////////////////////////////////////////
  int y = Rect . top ( ) - 2                        ;
  int h = ( items / columns )                       ;
  if ( ( items % columns ) > 0 ) h++                ;
  h *= iconSize . height ( )                        ;
  y -= h                                            ;
  ///////////////////////////////////////////////////
  QRect V                                           ;
  QRect B                  ( 0 , 0 , 0 , 0        ) ;
  QSize S                  ( Rect . width ( ) , h ) ;
  V . setLeft              ( Rect . left  ( )     ) ;
  V . setTop               ( y                    ) ;
  V . setSize              ( S                    ) ;
  B . setSize              ( S                    ) ;
  ///////////////////////////////////////////////////
  if                       ( NULL != wrapper      ) {
    wrapper -> setGeometry ( V                    ) ;
    setGeometry            ( B                    ) ;
  } else                                            {
    setGeometry            ( V                    ) ;
  }                                                 ;
}
