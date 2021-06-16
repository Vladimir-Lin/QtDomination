#include <domination.h>
#include "ui_AlbumEditor.h"

N::AlbumEditor:: AlbumEditor   ( QWidget * parent , Plan * p )
               : StackedWidget (           parent ,        p )
               , Object        ( 0 , Types::Album            )
               , GroupItems    (                           p )
               , VideoManager  (                           p )
               , ui            ( new Ui::AlbumEditor         )
               , pan           ( NULL                        )
               , clip          ( NULL                        )
               , clips         ( NULL                        )
               , people        ( NULL                        )
               , pictures      ( NULL                        )
               , documents     ( NULL                        )
               , bookmarks     ( NULL                        )
               , plain         ( NULL                        )
               , groups        ( NULL                        )
               , identifiers   ( NULL                        )
               , torrents      ( NULL                        )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::AlbumEditor::~AlbumEditor(void)
{
  delete ui ;
}

void N::AlbumEditor::Configure(void)
{
  ui   -> setupUi                         ( this                   ) ;
  ui   -> Groups      -> setEnabled       ( false                  ) ;
  ui   -> Clips       -> setEnabled       ( false                  ) ;
  ui   -> Actors      -> setEnabled       ( false                  ) ;
  ui   -> Commodities -> setMaximumHeight ( 60                     ) ;
  ////////////////////////////////////////////////////////////////////
  setAccessibleName                       ( "N::AlbumEditor"       ) ;
  pan   = new PanWidget                   ( ui -> EditAlbum , plan ) ;
  ui   -> EditAlbum   -> addWidget        ( pan                    ) ;
  ui   -> Profile     -> setWidget        ( ui -> EditAlbum        ) ;
  ui   -> GroupMember -> setHeaderLabel   ( tr("Group")            ) ;
  ////////////////////////////////////////////////////////////////////
  identifiers = new N :: IdentifierEditor ( pan , plan             ) ;
  pan  -> addWidget                       ( identifiers            ) ;
  ////////////////////////////////////////////////////////////////////
  plan -> setFont                         ( this                   ) ;
  ////////////////////////////////////////////////////////////////////
  nConnect ( this , SIGNAL ( setActors      (QString) )              ,
             this , SLOT   ( setPlainText   (QString) )            ) ;
  nConnect ( this , SIGNAL ( EmitName       (QString) )              ,
             this , SLOT   ( assignName     (QString) )            ) ;
  nConnect ( this , SIGNAL ( EmitId         (QString) )              ,
             this , SLOT   ( assignId       (QString) )            ) ;
  nConnect ( this , SIGNAL ( EmitImage      (QImage*) )              ,
             this , SLOT   ( assignImage    (QImage*) )            ) ;
  nConnect ( this , SIGNAL ( EmitLanguage   (int    ) )              ,
             this , SLOT   ( setLanguage    (int    ) )            ) ;
  nConnect ( this , SIGNAL ( assignEnabling (bool   ) )              ,
             this , SLOT   ( setEnabling    (bool   ) )            ) ;
  nConnect ( this , SIGNAL ( GoStart        (       ) )              ,
             this , SLOT   ( Bootup         (       ) )            ) ;
  nConnect ( this , SIGNAL ( GoPeople       (       ) )              ,
             this , SLOT   ( StartPeople    (       ) )            ) ;
  nConnect ( this , SIGNAL ( GoPicture      (       ) )              ,
             this , SLOT   ( StartPicture   (       ) )            ) ;
}

void N::AlbumEditor::run(int T,ThreadData * data)
{
  switch ( T )         {
    case 10001         :
      startLoading ( ) ;
      Load         ( ) ;
      stopLoading  ( ) ;
    break              ;
    case 10002         :
      startLoading ( ) ;
      NewPeople    ( ) ;
      stopLoading  ( ) ;
    break              ;
    case 10003         :
      startLoading ( ) ;
      NewActor     ( ) ;
      stopLoading  ( ) ;
    break              ;
    case 10004         :
      startLoading ( ) ;
      NewGroup     ( ) ;
      stopLoading  ( ) ;
    break              ;
    case 10005         :
      startLoading ( ) ;
      RemoveGroup  ( ) ;
      stopLoading  ( ) ;
    break              ;
  }                    ;
}

#define CanStopID   1000003

bool N::AlbumEditor::canStop(void)
{
  if ( LimitValues [ CanStopID ] > 0 ) return false ;
  if ( ! identifiers -> canStop ( )  ) return false ;
  #define CANSTOPX(itx) if ( ( NULL != itx ) && ( ! itx -> canStop ( ) ) ) return false
  CANSTOPX ( groups    )                            ;
  CANSTOPX ( clip      )                            ;
  CANSTOPX ( clips     )                            ;
  CANSTOPX ( people    )                            ;
  CANSTOPX ( pictures  )                            ;
  CANSTOPX ( documents )                            ;
  CANSTOPX ( bookmarks )                            ;
  CANSTOPX ( plain     )                            ;
  CANSTOPX ( torrents  )                            ;
  #undef  CANSTOPX
  return true                                       ;
}

void N::AlbumEditor::setEnabling(bool enable)
{
  setEnabled ( enable ) ;
}

bool N::AlbumEditor::startup(void)
{
  emit GoStart ( ) ;
  return true      ;
}

bool N::AlbumEditor::Bootup(void)

{
  ui -> Languages       << plan->languages                               ;
  ui -> PeopleLanguages << plan->languages                               ;
  ////////////////////////////////////////////////////////////////////////
  if (ObjectUuid()<=0)                                                   {
    ui -> Name -> setFocus ( Qt::TabFocusReason )                        ;
    return true                                                          ;
  }                                                                      ;
  ////////////////////////////////////////////////////////////////////////
  ui -> Groups -> setParent              ( ui -> Main                  ) ;
  ui -> Clips  -> setParent              ( ui -> Main                  ) ;
  ui -> Actors -> setParent              ( ui -> Main                  ) ;
  ui -> Main   -> addTab                 ( ui -> Groups , tr("Groups") ) ;
  ui -> Main   -> addTab                 ( ui -> Clips  , tr("Clips" ) ) ;
  ui -> Main   -> addTab                 ( ui -> Actors , tr("Actors") ) ;
  ui -> Groups -> show                   (                             ) ;
  ui -> Clips  -> show                   (                             ) ;
  ui -> Actors -> show                   (                             ) ;
  ////////////////////////////////////////////////////////////////////////
  pictures  = new N :: PictureView       ( NULL              , plan    ) ;
  torrents  = new N :: TorrentLists      ( NULL              , plan    ) ;
  documents = new N :: DocumentItems     ( NULL              , plan    ) ;
  bookmarks = new N :: Bookmarks         ( NULL              , plan    ) ;
  people    = new N :: PeopleView        ( NULL              , plan    ) ;
  plain     = new N :: PlainEditor       ( NULL              , plan    ) ;
  clip      = new N :: ClipList          ( NULL              , plan    ) ;
  clips     = new N :: AlbumClip         ( NULL              , plan    ) ;
  groups    = new N :: ComboBox          ( NULL              , plan    ) ;
  ////////////////////////////////////////////////////////////////////////
  ui   -> Commodities-> setEnabled       ( false                       ) ;
  ui   -> PeopleTool -> setParent        ( ui -> Actors                ) ;
  ui   -> PeopleTool -> setMinimumHeight ( 28                          ) ;
  ui   -> PeopleTool -> setMaximumHeight ( 28                          ) ;
  ////////////////////////////////////////////////////////////////////////
  ui   -> Main       -> addTab           ( pictures  , tr("Pictures" ) ) ;
  ui   -> Main       -> addTab           ( torrents  , tr("Torrents" ) ) ;
  ui   -> Main       -> addTab           ( documents , tr("Documents") ) ;
  ui   -> Main       -> addTab           ( bookmarks , tr("Bookmarks") ) ;
  ui   -> Actors     -> addWidget        ( people                      ) ;
  ui   -> Actors     -> addWidget        ( ui -> PeopleTool            ) ;
  ui   -> Actors     -> addWidget        ( plain                       ) ;
  ui   -> Clips      -> addWidget        ( clips                       ) ;
  ui   -> Clips      -> addWidget        ( clip                        ) ;
  ////////////////////////////////////////////////////////////////////////
  pictures  -> setGui                    ( pictures                    ) ;
  torrents  -> setGui                    ( torrents                    ) ;
  documents -> setGui                    ( documents                   ) ;
  bookmarks -> setGui                    ( bookmarks                   ) ;
  people    -> setGui                    ( people                      ) ;
  plain     -> setGui                    ( plain                       ) ;
  clip      -> setGui                    ( clip                        ) ;
  clips     -> setGui                    ( clips                       ) ;
  groups    -> setGui                    ( groups                      ) ;
  ////////////////////////////////////////////////////////////////////////
  ui   -> UnusedGroupLists -> hide       (                             ) ;
  ui   -> GroupSelect-> addWidget        ( groups                      ) ;
  ////////////////////////////////////////////////////////////////////////
  ui   -> Groups     -> setEnabled       ( true                        ) ;
  ui   -> Clips      -> setEnabled       ( true                        ) ;
  ui   -> Actors     -> setEnabled       ( true                        ) ;
  ui   -> Identifier -> setEnabled       ( false                       ) ;
  ui   -> PeopleTool -> show             (                             ) ;
  ////////////////////////////////////////////////////////////////////////
  clip       -> setLimitValue            ( 1  , 5                      ) ;
  pictures   -> setLimitValue            ( 49 , 1                      ) ;
  people     -> setLimitValue            ( 49 , 1                      ) ;
  ////////////////////////////////////////////////////////////////////////
  groups     -> setEditable              ( true                        ) ;
  groups     -> setMaxVisibleItems       ( 30                          ) ;
  ////////////////////////////////////////////////////////////////////////
  documents  -> SourceType = Types::Album                                ;
  clips      -> setObjectUuid            ( ObjectUuid()                ) ;
  pictures   -> setFirst                 ( ObjectUuid() , ObjectType() ) ;
  pictures   -> MountCommands            (                             ) ;
  torrents   -> setOwner                 ( ObjectUuid() , ObjectType() ) ;
  people     -> setFirst                 ( ObjectUuid() , ObjectType() ) ;
  bookmarks  -> setObjectUuid            ( ObjectUuid ( )              ) ;
  bookmarks  -> setObjectType            ( ObjectType ( )              ) ;
  ////////////////////////////////////////////////////////////////////////
  pictures   -> setWindowTitle           ( tr ( "Pictures of %1"         )
                                           . arg ( windowTitle ( )   ) ) ;
  torrents   -> setWindowTitle           ( tr ( "Torrents of %1"         )
                                           . arg ( windowTitle ( )   ) ) ;
  people     -> setWindowTitle           ( tr ( "Actors of %1"           )
                                           . arg ( windowTitle ( )   ) ) ;
  documents  -> setWindowTitle           ( tr ( "Documents of %1"        )
                                           . arg ( windowTitle ( )   ) ) ;
  clip       -> setWindowTitle           ( tr ( "Video clips of %1"      )
                                           . arg ( windowTitle ( )   ) ) ;
  clips      -> setWindowTitle           ( tr ( "Clips of %1"            )
                                           . arg ( windowTitle ( )   ) ) ;
  ////////////////////////////////////////////////////////////////////////
  plan        -> processEvents           (                             ) ;
  emit GoPeople                          (                             ) ;
  emit GoPicture                         (                             ) ;
  torrents    -> startup                 (                             ) ;
  clips       -> startup                 (                             ) ;
  clip        -> startup                 (                             ) ;
  documents   -> startup                 ( ObjectUuid ( )              ) ;
  bookmarks   -> startup                 (                             ) ;
  identifiers -> setObjectUuid           ( ObjectUuid ( )              ) ;
  identifiers -> setObjectType           ( ObjectType ( )              ) ;
  identifiers -> startup                 (                             ) ;
  ////////////////////////////////////////////////////////////////////////
  nConnect ( pictures   , SIGNAL (Command      (QWidget*,QString,SUID))  ,
             this       , SIGNAL (Command      (QWidget*,QString,SUID))) ;
  ////////////////////////////////////////////////////////////////////////
  nConnect ( people     , SIGNAL (ListSubgroups(QString,SUID))           ,
             this       , SIGNAL (ListSubgroups(QString,SUID))         ) ;
  ////////////////////////////////////////////////////////////////////////
  nConnect ( documents , SIGNAL (Open(SUID,SUID,int,QString,QString))    ,
             this      , SIGNAL (Open(SUID,SUID,int,QString,QString))  ) ;
  nConnect ( documents , SIGNAL (Processors(UUIDs))                      ,
             this      , SIGNAL (Processors(UUIDs))                    ) ;
  ////////////////////////////////////////////////////////////////////////
  nConnect ( bookmarks , SIGNAL(Web(QUrl,bool))                          ,
             this      , SIGNAL(Web(QUrl,bool))                        ) ;
  nConnect ( bookmarks , SIGNAL(ImportRules(QString,SUID,int))           ,
             this      , SIGNAL(ImportRules(QString,SUID,int))         ) ;
  nConnect ( bookmarks->bookmarks , SIGNAL(RunSets(VarArgLists&))        ,
             this                 , SIGNAL(RunSets(VarArgLists&))      ) ;
  ////////////////////////////////////////////////////////////////////////
  start ( 10001 )                                                        ;
  ////////////////////////////////////////////////////////////////////////
  for (int i = 0 ; i < ui -> Main -> count ( ) ; i++ )                   {
    ui -> Main -> widget ( i ) -> update ( )                             ;
  }                                                                      ;
  ui -> Main -> update ( )                                               ;
  ////////////////////////////////////////////////////////////////////////
  return true                                                            ;
}

void N::AlbumEditor::NameChanged(void)
{
  QString name = ui -> Name -> text ( )    ;
  if (name.length()<=0) return             ;
  if (ObjectUuid()>0) UpdateAlbum ( name ) ;
                 else AppendAlbum ( name ) ;
}

void N::AlbumEditor::PrepareAlbum(QString name)
{
  ui -> Name -> blockSignals ( true  ) ;
  ui -> Name -> setText      ( name  ) ;
  ui -> Name -> blockSignals ( false ) ;
}

void N::AlbumEditor::AppendAlbum(QString name)
{
  int language                              ;
  language = (int)N::GetUuid(ui->Languages) ;
  SqlConnection SC ( plan->sql )            ;
  if ( SC . open ( FunctionString ) )       {
    SUID u = appendAlbum(SC,language,name)  ;
    if (u>0)                                {
      setObjectUuid ( u )                   ;
      if (ui->hasCommodity->isChecked())    {
        assureProduct(SC,language,name,u)   ;
      }                                     ;
      OldName = name                        ;
    }                                       ;
    SC . close    (    )                    ;
  }                                         ;
  SC   . remove   (    )                    ;
  if (ObjectUuid()<=0) return               ;
  startup ( )                               ;
}

void N::AlbumEditor::UpdateAlbum(QString name)
{
  if ( name == OldName ) return             ;
  int language                              ;
  language = (int)N::GetUuid(ui->Languages) ;
  SqlConnection SC ( plan->sql )            ;
  if ( SC . open ( FunctionString ) )       {
    SC . assureName                         (
      PlanTable(Names)                      ,
      ObjectUuid()                          ,
      language                              ,
      name                                ) ;
    OldName = name                          ;
    SC . close    (    )                    ;
  }                                         ;
  SC   . remove   (    )                    ;
}

SUID N::AlbumEditor::appendAlbum(SqlConnection & SC,int language,QString name)
{
  int     position = 0                                ;
  QString Q                                           ;
  SUID    uuid                                        ;
  uuid = SC.Unique(PlanTable(MajorUuid),"uuid",37281) ;
  SC.assureUuid                                       (
    PlanTable(MajorUuid)                              ,
    uuid                                              ,
    Types::Album                                    ) ;
  SC.assureUuid                                       (
    PlanTable(Divisions)                              ,
    uuid                                              ,
    Types::Album                                    ) ;
  SC.assureName                                       (
    PlanTable(Names)                                  ,
    uuid                                              ,
    language                                          ,
    name                                            ) ;
  /////////////////////////////////////////////////////
  Q = SC.sql.SelectFrom                               (
        "position"                                    ,
        PlanTable(Divisions)                          ,
        QString("where type = %1 "
                "order by position desc "
                "limit 0,1"                           )
        .arg(Types::Album                         ) ) ;
  if (SC.Fetch(Q)) position = SC.Int(0)               ;
  position++                                          ;
  /////////////////////////////////////////////////////
  Q = SC.sql.Update                                   (
        PlanTable(Divisions)                          ,
        SC.sql.Where(1,"uuid")                        ,
        1                                             ,
        "position"                                  ) ;
  SC . Prepare ( Q                                  ) ;
  SC . Bind    ( "uuid"     , uuid                  ) ;
  SC . Bind    ( "position" , position              ) ;
  SC . Exec    (                                    ) ;
  return uuid                                         ;
}

bool N::AlbumEditor::assureProduct(SqlConnection & SC,int language,QString name,SUID u)
{
  QString        ID = ui->Identifier->text()     ;
  FinanceManager FM ( plan )                     ;
  GroupItems     GI ( plan )                     ;
  SUID           commodity = 0                   ;
  QString        Q                               ;
  ////////////////////////////////////////////////
  if ( ID . length ( ) > 0 )                     {
    commodity = FM . FindIdentifier ( SC , ID )  ;
    if (commodity>0)                             {
      Q = SC . sql . SelectFrom                  (
            "type"                               ,
            PlanTable(MajorUuid)                 ,
            SC.WhereUuid(commodity)            ) ;
      if (SC.Fetch(Q))                           {
        int t = SC . Int ( 0 )                   ;
        if ( Types::Commodity != t )             {
          commodity = 0                          ;
        }                                        ;
      }                                          ;
    }                                            ;
  }                                              ;
  ////////////////////////////////////////////////
  if ( commodity <= 0 )                          {
    commodity = FM . appendCommodity ( SC )      ;
    FM.appendNamemap(SC,commodity,language,name) ;
    if (ID.length()>0)                           {
      FM . assureIdentifier ( SC,commodity,ID  ) ;
    }                                            ;
  }                                              ;
  ////////////////////////////////////////////////
  if ( commodity > 0 )                           {
    UUIDs Puids                                  ;
    Puids << u                                   ;
    GI . Join                                    (
      SC                                         ,
      commodity                                  ,
      Types  :: Commodity                        ,
      Types  :: Album                            ,
      Groups :: Subordination                    ,
      0                                          ,
      Puids                                    ) ;
  }                                              ;
  ////////////////////////////////////////////////
  return true                                    ;
}

void N::AlbumEditor::assignName(QString n)
{
  ui -> Name  -> blockSignals   ( true  ) ;
  ui -> Name  -> setText        ( n     ) ;
  ui -> Name  -> blockSignals   ( false ) ;
  identifiers -> setWindowTitle ( n     ) ;
  setWindowTitle                ( n     ) ;
}

void N::AlbumEditor::setLanguage(int language)
{
  ui -> Languages -> blockSignals ( true  ) ;
  (*ui ->Languages) <= language             ;
  ui -> Languages -> blockSignals ( false ) ;
  ui -> Languages -> setEnabled   ( false ) ;
}

void N::AlbumEditor::assignId(QString ID)
{
  ui -> Identifier -> blockSignals ( true ) ;
  ui -> Identifier -> setText      ( ID   ) ;
  ui -> Identifier -> blockSignals ( false) ;
}

void N::AlbumEditor::assignImage(QImage * image)
{
  QPixmap pix                                 ;
  pix = QPixmap::fromImage     ( *image   )   ;
  ui -> AlbumIcon -> setPixmap ( pix      )   ;
  delete image                                ;
}

void N::AlbumEditor::Load(void)
{
  int language = N :: GetUuid ( ui -> Languages )   ;
  nDropOut          ( ObjectUuid ( ) <= 0 )         ;
  GroupItems     GI ( plan                )         ;
  FinanceManager FM ( plan                )         ;
  PictureManager PM ( plan                )         ;
  SqlConnection  SC ( plan -> sql         )         ;
  UUIDs          UX                                 ;
  ///////////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )               {
    CUIDs L = plan -> languages . Supports          ;
    int   i = L . indexOf ( language )              ;
    bool  b = false                                 ;
    UUIDs U                                         ;
    SUID  u                                         ;
    /////////////////////////////////////////////////
    GI . AutoMap = true                             ;
    if (i>=0) L . takeAt  ( i        )              ;
    L  . prepend          ( language )              ;
    for (int i=0;!b && i<L.count();i++)             {
      QString n                                     ;
      n = SC . getName                              (
            PlanTable(Names)                        ,
            "uuid"                                  ,
            L[i]                                    ,
            ObjectUuid ( )                      )   ;
      if (n.length()>0)                             {
        emit EmitLanguage ( L [ i ] )               ;
        emit EmitName     ( n       )               ;
        OldName = n                                 ;
        b       = true                              ;
      }                                             ;
    }                                               ;
    /////////////////////////////////////////////////
    if ( ObjectUuid ( ) > 0 )                       {
      GI . GroupTable = GI . LookTable              (
        Types::Commodity                            ,
        Types::Album                                ,
        Groups::Subordination                     ) ;
      U = GI . GetOwners                            (
            SC                                      ,
            ObjectUuid ( )                          ,
            Types::Commodity                        ,
            Types::Album                            ,
            Groups::Subordination                   ,
            SC . OrderByDesc ( "id" )             ) ;
      if ( U . count ( ) > 0 )                      {
        SUID    cxid = U . first ( )                ;
        QString ID   = FM.FetchIdentifier(SC,cxid)  ;
        if (ID.length()>0)                          {
          emit EmitId ( ID )                        ;
        }                                           ;
      }                                             ;
    }                                               ;
    /////////////////////////////////////////////////
    if ( ObjectUuid ( ) > 0 )                       {
      QString L = SC . sql.Limit  ( 0 , 1       )   ;
      QString O = SC . OrderByAsc ( "position"  )   ;
      SUID     puid                                 ;
      QImage * image = NULL                         ;
      GI . GroupTable = GI . LookTable              (
            ObjectType ( )                          ,
            Types  :: Picture                       ,
            Groups :: Icon                        ) ;
      puid = GI . FindSecond                        (
               SC                                   ,
               ObjectUuid ( )                       ,
               ObjectType ( )                       ,
               Types  :: Picture                    ,
               Groups :: Icon                       ,
               O                                    ,
               L                                  ) ;
      if ( puid > 0 )                               {
        image = PM . Thumb ( SC , puid )            ;
      }                                             ;
      if ( NotNull ( image ) )                      {
        emit EmitImage ( image )                    ;
      }                                             ;
    }                                               ;
    /////////////////////////////////////////////////
    UX = SC . Uuids                                 (
           PlanTable(VideoGroups)                   ,
           "uuid"                                   ,
           SC . OrderByDesc ( "id" )              ) ;
    if ( UX . count ( ) > 0 )                       {
      groups -> pendItems   ( UX         )          ;
    }                                               ;
    /////////////////////////////////////////////////
    GI . GroupTable = GI . LookTable                (
      Types::Division                               ,
      Types::Album                                  ,
      Groups::Subordination                       ) ;
    U = GI . GetOwners                              (
          SC                                        ,
          ObjectUuid()                              ,
          Types::Division                           ,
          Types::Album                              ,
          Groups::Subordination                     ,
          SC . OrderByAsc ( "id" )                ) ;
    foreach ( u , U ) if ( UX . contains ( u ) )    {
      QString n                                     ;
      n = SC . getName                              (
            PlanTable(Names)                        ,
            "uuid"                                  ,
            vLanguageId                             ,
            u                                   )   ;
      if ( n . length ( ) > 0 )                     {
        NewTreeWidgetItem ( it                  )   ;
        it -> setData     ( 0,Qt::UserRole,u    )   ;
        it -> setText     ( 0,n                 )   ;
        ui -> GroupMember -> addTopLevelItem ( it ) ;
      }                                             ;
    }                                               ;
    /////////////////////////////////////////////////
    SC . close            (            )            ;
  }                                                 ;
  SC   . remove           (            )            ;
  ///////////////////////////////////////////////////
}

void N::AlbumEditor::AddGroup(void)
{
  start ( 10004 ) ;
}

void N::AlbumEditor::NewGroup(void)
{
  if ( ObjectUuid() <= 0 ) return                 ;
  SUID u = N::GetUuid((QComboBox *)groups)        ;
  if ( u <= 0 ) return                            ;
  QString n = groups->currentText()               ;
  /////////////////////////////////////////////////
  GroupItems    GI ( plan      )                  ;
  SqlConnection SC ( plan->sql )                  ;
  if ( SC . open ( FunctionString ) )             {
    UUIDs   U                                     ;
    U << ObjectUuid()                             ;
    GI . Join                                     (
      SC                                          ,
      u                                           ,
      Types  :: Division                          ,
      Types  :: Album                             ,
      Groups :: Subordination                     ,
      0                                           ,
      U                                         ) ;
    ///////////////////////////////////////////////
    NewTreeWidgetItem ( it )                      ;
    it -> setData ( 0 , Qt::UserRole , u )        ;
    it -> setText ( 0 , n                )        ;
    ui -> GroupMember->addTopLevelItem(it)        ;
    ///////////////////////////////////////////////
    SC . close    (    )                          ;
  }                                               ;
  SC   . remove   (    )                          ;
  /////////////////////////////////////////////////
  alert ( "Done" , tr ( "New group appended." ) ) ;
}

void N::AlbumEditor::DeleteGroup(void)
{
  start ( 10005 ) ;
}

void N::AlbumEditor::RemoveGroup(void)
{
  QTreeWidgetItem * it                               ;
  it = ui -> GroupMember -> currentItem ( )          ;
  if (IsNull(it)) return                             ;
  SUID u = nTreeUuid ( it , 0 )                      ;
  if (u<=0) return                                   ;
  ////////////////////////////////////////////////////
  GroupItems    GI ( plan      )                     ;
  SqlConnection SC ( plan->sql )                     ;
  if ( SC . open ( FunctionString ) )                {
    int index                                        ;
    GI . ExactDetach                                 (
      SC                                             ,
      u                                              ,
      ObjectUuid ( )                                 ,
      Types  :: Division                             ,
      Types  :: Album                                ,
      Groups :: Subordination                      ) ;
    index = ui->GroupMember->indexOfTopLevelItem(it) ;
    if (index>=0)                                    {
      ui -> GroupMember -> takeTopLevelItem( index ) ;
    }                                                ;
    SC . close    (    )                             ;
  }                                                  ;
  SC   . remove   (    )                             ;
  ////////////////////////////////////////////////////
  alert ( "Done" , tr ("Remove group completed.")  ) ;
}

void N::AlbumEditor::groupClicked(QTreeWidgetItem * item,int column)
{ Q_UNUSED ( column )                                ;
  item = ui -> GroupMember -> currentItem ( )        ;
  ui  -> DeleteGroup -> setEnabled ( NotNull(item) ) ;
}

void N::AlbumEditor::StartPeople(void)
{
  people -> startup ( ) ;
}

void N::AlbumEditor::StartPicture(void)
{
  pictures -> startup ( ) ;
}

void N::AlbumEditor::AddActor(void)
{
  start ( 10003 ) ;
}

void N::AlbumEditor::setPlainText(QString text)
{
  plain -> setPlainText ( text ) ;
}

void N::AlbumEditor::NewActor(void)
{
  QString     t = plain -> toPlainText (      ) ;
  if (t.length()<=0) return                     ;
  QStringList s = t      . split       ( "\n" ) ;
  if (s.count ()<=0) return                     ;
  s = File :: PurifyLines ( s )                 ;
  emit assignEnabling ( false     )             ;
  ///////////////////////////////////////////////
  bool simplify = ui -> Spacing -> isChecked () ;
  bool added    = false                         ;
  GroupItems    GI    ( plan      )             ;
  PeopleManager PM    ( plan      )             ;
  SqlConnection SC    ( plan->sql )             ;
  if ( SC . open ( FunctionString ) )           {
    QStringList L                               ;
    QString     n                               ;
    UUIDs       U                               ;
    SUID        u                               ;
    foreach (n,s)                               {
      if (n.length()>0)                         {
        if ( simplify )                         {
          n = n . simplified ( )                ;
        }                                       ;
        u = PM . FindPeople ( SC , n )          ;
        if (u<=0)                               {
          L << n                                ;
        } else                                  {
          U << u                                ;
        }                                       ;
      }                                         ;
    }                                           ;
    if (L.count()>0)                            {
      n = L . join("\n")                        ;
    } else                                      {
      n = ""                                    ;
    }                                           ;
    emit setActors ( n )                        ;
    if (U.count()>0)                            {
      GI . Join                                 (
        SC                                      ,
        ObjectUuid ( )                          ,
        ObjectType ( )                          ,
        Types  :: People                        ,
        Groups :: Subordination                 ,
        0                                       ,
        U                                     ) ;
      added = true                              ;
    }                                           ;
    SC . close        (       )                 ;
  }                                             ;
  SC   . remove       (       )                 ;
  ///////////////////////////////////////////////
  emit assignEnabling ( true  )                 ;
  if ( added )                                  {
    emit GoPeople     (       )                 ;
  } else                                        {
    Alert             ( Error )                 ;
  }                                             ;
}

void N::AlbumEditor::FindAlbum(void)
{
  QString ID = ui -> Identifier -> text()     ;
  QString NT = ui -> Name       -> text()     ;
  if (NT.length()<=0 && ID.length()<=0)       {
    Alert ( Error )                           ;
    return                                    ;
  }                                           ;
  emit assignEnabling ( false     )           ;
  /////////////////////////////////////////////
  SUID           u = 0                        ;
  GroupItems     GI   ( plan      )           ;
  FinanceManager FM   ( plan      )           ;
  SqlConnection  SC   ( plan->sql )           ;
  if ( SC . open ( FunctionString ) )         {
    if (ID.length()>0)                        {
      SUID IU                                 ;
      IU = FM . FindIdentifier ( SC,ID )      ;
      if (IU>0)                               {
        UUIDs U                               ;
        U = GI . Subordination                (
              SC                              ,
              IU                              ,
              Types  :: Commodity             ,
              Types  :: Album                 ,
              Groups :: Subordination         ,
              SC.OrderByDesc("position")    ) ;
        if (U.count()>0) u = U [ 0 ]          ;
      }                                       ;
    }                                         ;
    ///////////////////////////////////////////
    if ( u <= 0 && NT.length() > 0 )          {
      QByteArray B = NT . toUtf8 ( )          ;
      QString    Q                            ;
      UUIDs      U                            ;
      Q = SC . sql . SelectFrom               (
            "uuid"                            ,
            PlanTable(Names)                  ,
            SC.sql.Where                      (
              2                               ,
              "length"                        ,
              "name"                    )     ,
            SC.OrderByDesc("id")        )     ;
      SC . Prepare ( Q                  )     ;
      SC . Bind    ( "length",B.size()  )     ;
      SC . Bind    ( "name"  ,B         )     ;
      if (SC.Exec()) while (SC.Next())        {
        SUID x = SC . Uuid ( 0 )              ;
        if (x>0 && !U.contains(x))            {
          U << x                              ;
        }                                     ;
      }                                       ;
      if (U.count()>0)                        {
        for (int i=0;u<=0 && i<U.count();i++) {
          Q = SC . sql . SelectFrom           (
                "type"                        ,
                PlanTable(MajorUuid)          ,
                SC.WhereUuid(U[i])          ) ;
          if (SC.Fetch(Q))                    {
            int t = SC . Int ( 0 )            ;
            if ( Types::Album == t )          {
              u = U [ i ]                     ;
            }                                 ;
          }                                   ;
        }                                     ;
      }                                       ;
    }                                         ;
    ///////////////////////////////////////////
    SC . close        (       )               ;
  }                                           ;
  SC   . remove       (       )               ;
  emit assignEnabling ( true  )               ;
  if ( u > 0 )                                {
    setObjectUuid     ( u     )               ;
    startup           (       )               ;
  } else                                      {
    Alert             ( Error )               ;
  }                                           ;
}

void N::AlbumEditor::AppendPeople(void)
{
  start ( 10002 ) ;
}

void N::AlbumEditor::NewPeople(void)
{
  nDropOut          ( ObjectUuid() <=0 )                    ;
  int     language = N :: GetUuid ( ui -> PeopleLanguages ) ;
  QString t        = plain -> toPlainText ( )               ;
  if (t.length()<=0) return                                 ;
  QStringList s = t . split ( "\n" )                        ;
  if (s.count ()<=0) return                                 ;
  s = File :: PurifyLines ( s )                             ;
  emit assignEnabling ( false     )                         ;
  ///////////////////////////////////////////////////////////
  PeopleManager PM    ( plan      )                         ;
  SqlConnection SC    ( plan->sql )                         ;
  if ( SC . open ( FunctionString ) )                       {
    foreach ( t , s ) if ( t . length ( ) > 0 )             {
      PM . addPeople ( SC , language , t )                  ;
    }                                                       ;
    SC . close        (           )                         ;
  }                                                         ;
  SC   . remove       (           )                         ;
  ///////////////////////////////////////////////////////////
  emit assignEnabling ( true      )                         ;
  Alert               ( Done      )                         ;
}

void N::AlbumEditor::Dig(void)
{
}
