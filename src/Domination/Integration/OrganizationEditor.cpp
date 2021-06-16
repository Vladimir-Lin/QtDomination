#include <domination.h>
#include "ui_OrganizationEditor.h"

N::OrganizationEditor:: OrganizationEditor ( QWidget * parent , Plan * p )
                      : StackedWidget      (           parent ,        p )
                      , Object             ( 0 , Types::Organization     )
                      , GroupItems         (                           p )
                      , PictureManager     (                           p )
                      , ui                 ( new Ui::OrganizationEditor  )
                      , names              ( NULL                        )
                      , people             ( NULL                        )
                      , gallery            ( NULL                        )
                      , commodity          ( NULL                        )
                      , videos             ( NULL                        )
                      , documents          ( NULL                        )
                      , bookmarks          ( NULL                        )
                      , processor          ( NULL                        )
                      , pan                ( NULL                        )
                      , ocgroups           ( NULL                        )
                      , octypes            ( NULL                        )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::OrganizationEditor::~OrganizationEditor(void)
{
  delete ui ;
}

QSize N::OrganizationEditor::sizeHint(void) const
{
  return QSize ( 800 , 600 ) ;
}

void N::OrganizationEditor::Configure(void)
{
  QFont f                                                                  ;
  f = plan -> fonts [ Fonts::ListView ]                                    ;
  //////////////////////////////////////////////////////////////////////////
  ui   -> setupUi                        ( this                          ) ;
  //////////////////////////////////////////////////////////////////////////
  ui   -> Profile -> setWidget           ( ui -> EditOrganization        ) ;
  //////////////////////////////////////////////////////////////////////////
  pan   = new PanWidget                  ( ui -> EditOrganization , plan ) ;
  ui   -> EditOrganization -> addWidget  ( pan                           ) ;
  //////////////////////////////////////////////////////////////////////////
  processor  = NewProcessor              ( ui -> EditOrganization        ) ;
  processor -> setPrompt                 ( tr("Command >")               ) ;
  processor -> setMinimumHeight          ( 40                            ) ;
  ui -> EditOrganization -> addWidget    ( processor                     ) ;
  //////////////////////////////////////////////////////////////////////////
  QSize mxs                              ( 16777215 , 16777215           ) ;
  //////////////////////////////////////////////////////////////////////////
  ui -> OrganizationGroups -> setMaximumSize ( mxs                       ) ;
  ui -> OrganizationTypes  -> setMaximumSize ( mxs                       ) ;
  //////////////////////////////////////////////////////////////////////////
  ocgroups  = new ComboBox               ( ui->OrganizationTab    ,plan  ) ;
  octypes   = new ComboBox               ( ui->OrganizationTypeTab,plan  ) ;
  ocgroups -> setMaxVisibleItems         ( 30                            ) ;
  octypes  -> setMaxVisibleItems         ( 30                            ) ;
  ocgroups -> setEditable                ( true                          ) ;
  octypes  -> setEditable                ( true                          ) ;
  ocgroups -> setInsertPolicy            ( QComboBox::NoInsert           ) ;
  octypes  -> setInsertPolicy            ( QComboBox::NoInsert           ) ;
  ui -> OrganizationTab     -> addWidget ( ocgroups                      ) ;
  ui -> OrganizationTypeTab -> addWidget ( octypes                       ) ;
  //////////////////////////////////////////////////////////////////////////
  QSize ms = ui->OrganizationGroupButtons->minimumSize( )                  ;
  ui -> OrganizationGroupButtons -> setMaximumSize ( ms )                  ;
  ui -> OrganizationTypeButtons  -> setMaximumSize ( ms )                  ;
  //////////////////////////////////////////////////////////////////////////
  ui  -> OrganizationGroups -> setParent ( pan                           ) ;
  ui  -> OrganizationTypes  -> setParent ( pan                           ) ;
  pan -> addWidget                       ( ui->OrganizationGroups        ) ;
  pan -> addWidget                       ( ui->OrganizationTypes         ) ;
  //////////////////////////////////////////////////////////////////////////
  NewTreeWidgetItem                      ( groupHead                     ) ;
  groupHead -> setText                   ( 0 , tr ( "Group"   )          ) ;
  groupHead -> setFont                   ( 0 , f                         ) ;
  groupHead -> setTextAlignment          ( 0 , Qt::AlignCenter           ) ;
  ui -> GroupLists -> setHeaderItem      ( groupHead                     ) ;
  //////////////////////////////////////////////////////////////////////////
  NewTreeWidgetItem                      ( typeHead                      ) ;
  typeHead  -> setText                   ( 0 , tr("Type of organization")) ;
  typeHead  -> setFont                   ( 0 , f                         ) ;
  typeHead  -> setTextAlignment          ( 0 , Qt::AlignCenter           ) ;
  ui -> TypeLists  -> setHeaderItem      ( typeHead                      ) ;
  //////////////////////////////////////////////////////////////////////////
  pan       -> setEnabled                ( false                         ) ;
  processor -> setEnabled                ( false                         ) ;
  //////////////////////////////////////////////////////////////////////////
  plan -> setFont                        ( this                          ) ;
}

N::OrganizationProcessor * N::OrganizationEditor::NewProcessor (QWidget * widget)
{
  return new OrganizationProcessor ( widget , plan ) ;
}

void N::OrganizationEditor::startup(void)
{
  if (ObjectUuid()<=0)                                         {
    ui -> Name -> setFocus ( Qt::TabFocusReason )              ;
    return                                                     ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  processor  -> setObjectUuid ( ObjectUuid() )                 ;
  //////////////////////////////////////////////////////////////
  names  = new ItemNames      ( ui -> Main , plan            ) ;
  names -> setOwner           ( ObjectUuid() , ObjectType()  ) ;
  //////////////////////////////////////////////////////////////
  people   = new ActorView    ( ui -> Main , plan )            ;
  people  -> setFirst ( ObjectUuid() , ObjectType()          ) ;
  //////////////////////////////////////////////////////////////
  gallery  = new ObjectGallery ( ui -> Main , plan )           ;
  gallery -> setFirst ( ObjectUuid() , ObjectType()          ) ;
  gallery -> MountCommands (      )                            ;
  //////////////////////////////////////////////////////////////
  commodity  = new CommodityEdit ( ui->Main , plan )           ;
  commodity -> setObjectUuid ( ObjectUuid ( )        )         ;
  commodity -> setObjectType ( ObjectType ( )        )         ;
  commodity -> setConnexion  ( Groups::Subordination )         ;
  //////////////////////////////////////////////////////////////
  videos    = new VideoContainer ( ui->Main , plan )           ;
  videos   -> setFirst ( ObjectUuid ( ) , ObjectType ( ) )     ;
  //////////////////////////////////////////////////////////////
  documents = new DocumentItems ( ui->Main , plan )            ;
  documents -> SourceType = ObjectType ( )                     ;
  //////////////////////////////////////////////////////////////
  bookmarks  = new Bookmarks ( ui->Main , plan )               ;
  bookmarks -> setObjectUuid ( ObjectUuid ( )  )               ;
  bookmarks -> setObjectType ( ObjectType ( )  )               ;
  //////////////////////////////////////////////////////////////
  ui -> Main -> addTab ( names     , tr("Name lists"       ) ) ;
  ui -> Main -> addTab ( people    , tr("Members"          ) ) ;
  ui -> Main -> addTab ( gallery   , tr("Gallery"          ) ) ;
  ui -> Main -> addTab ( commodity , tr("Commodities"      ) ) ;
  ui -> Main -> addTab ( videos    , tr("Video albums"     ) ) ;
  ui -> Main -> addTab ( documents , tr("Documents"        ) ) ;
  ui -> Main -> addTab ( bookmarks , tr("Bookmarks"        ) ) ;
  //////////////////////////////////////////////////////////////
  people     -> setLimitValue ( 49 , 0                       ) ;
  gallery    -> setLimitValue ( 49 , 0                       ) ;
  videos     -> setLimitValue ( 49 , 0                       ) ;
  //////////////////////////////////////////////////////////////
  plan       -> processEvents (                              ) ;
  names      -> Reload        (                              ) ;
  people     -> startup       (                              ) ;
  gallery    -> Reload        ( true                         ) ;
  commodity  -> startup       (                              ) ;
  videos     -> startup       (                              ) ;
  documents  -> startup       ( ObjectUuid ( )               ) ;
  bookmarks  -> startup       (                              ) ;
  ocgroups   -> addDivision   ( Types::Organization          ) ;
  octypes    -> addItems(PlanTable(Comity),Qt::AscendingOrder) ;
  //////////////////////////////////////////////////////////////
  pan        -> setEnabled    ( true                         ) ;
  processor  -> setEnabled    ( true                         ) ;
  //////////////////////////////////////////////////////////////
  Load                        (                              ) ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( gallery    , SIGNAL ( Command       (QWidget*,QString,SUID))   ,
             this       , SIGNAL ( Command       (QWidget*,QString,SUID)) ) ;
  nConnect ( gallery    , SIGNAL ( ListSubgroups (QString,SUID))            ,
             this       , SIGNAL ( ListSubgroups (QString,SUID))          ) ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( names , SIGNAL(Update    (QWidget*,SUID))   ,
             this  , SLOT  (NameUpdate(QWidget*,SUID)) ) ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( documents , SIGNAL (Open(SUID,SUID,int,QString,QString))  ,
             this      , SIGNAL (Open(SUID,SUID,int,QString,QString))) ;
  nConnect ( documents , SIGNAL (Processors(UUIDs))                    ,
             this      , SIGNAL (Processors(UUIDs))                  ) ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( videos , SIGNAL(UuidItems(UUIDs&,RMAPs&,NAMEs&,QString))   ,
             this   , SIGNAL(UuidItems(UUIDs&,RMAPs&,NAMEs&,QString)) ) ;
  nConnect ( videos , SIGNAL(Edit(SUID,QString))                        ,
             this   , SIGNAL(Edit(SUID,QString))                      ) ;
  nConnect ( videos , SIGNAL(ShowMenu (QWidget*,QPoint))                ,
             videos , SLOT  (ActorMenu(QWidget*,QPoint))              ) ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( people , SIGNAL(Gallery(QWidget*,SUID,QString))   ,
             this   , SIGNAL(Gallery(QWidget*,SUID,QString)) ) ;
  nConnect ( people , SIGNAL(PotentialPictures(QWidget*,SUID,QString))   ,
             this   , SIGNAL(PotentialPictures(QWidget*,SUID,QString)) ) ;
  nConnect ( people , SIGNAL(VideoAlbum(QWidget*,SUID,QString))   ,
             this   , SIGNAL(VideoAlbum(QWidget*,SUID,QString)) ) ;
  nConnect ( people , SIGNAL(PeopleNames(QWidget*,SUID,QString))   ,
             this   , SIGNAL(PeopleNames(QWidget*,SUID,QString)) ) ;
  nConnect ( people , SIGNAL(PeopleDocuments(QWidget*,SUID,QString))   ,
             this   , SIGNAL(PeopleDocuments(QWidget*,SUID,QString)) ) ;
  nConnect ( people , SIGNAL(Internet(QWidget*,SUID,QString,int))   ,
             this   , SIGNAL(Internet(QWidget*,SUID,QString,int)) ) ;
  nConnect ( people , SIGNAL(Translations(QString,UUIDs&))   ,
             this   , SIGNAL(Translations(QString,UUIDs&)) ) ;
  nConnect ( people , SIGNAL(Feature(int,SUID))   ,
             this   , SIGNAL(Feature(int,SUID)) ) ;
  nConnect ( people , SIGNAL(MergePeople(UUIDs&))   ,
             this   , SIGNAL(MergePeople(UUIDs&)) ) ;
  nConnect ( people , SIGNAL(ListSubgroups  (QString,SUID))   ,
             this   , SIGNAL(PeopleSubgroups(QString,SUID)) ) ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( bookmarks , SIGNAL(Web(QUrl,bool))                  ,
             this      , SIGNAL(Web(QUrl,bool))                ) ;
  nConnect ( bookmarks , SIGNAL(ImportRules(QString,SUID,int))   ,
             this      , SIGNAL(ImportRules(QString,SUID,int)) ) ;
  nConnect ( bookmarks , SIGNAL(RunSets(VarArgLists&))           ,
             this      , SIGNAL(RunSets(VarArgLists&))         ) ;
}

void N::OrganizationEditor::Load(void)
{
  nDropOut         ( ObjectUuid() <=0 )                ;
  SqlConnection SC ( plan->sql        )                ;
  if (SC.open("OrganizationEditor","Load"))            {
    QString     Q                                      ;
    QString     N                                      ;
    QStringList names                                  ;
    UUIDs       U                                      ;
    SUID        u                                      ;
    ////////////////////////////////////////////////////
    names = GroupItems::Names    ( SC , ObjectUuid() ) ;
    if (names.count()>0)                               {
      setWindowTitle             ( names [0]         ) ;
      ui -> Name -> blockSignals ( true              ) ;
      ui -> Name -> setText      ( names[0]          ) ;
      ui -> Name -> blockSignals ( false             ) ;
    }                                                  ;
    ////////////////////////////////////////////////////
    U = GetOwners                                      (
          SC                                           ,
          ObjectUuid()                                 ,
          Types::TypeOfOrganization                    ,
          ObjectType()                                 ,
          Groups::Subordination                        ,
          SC.OrderBy("id",Qt::AscendingOrder)        ) ;
    foreach (u,U)                                      {
      N = SC . getName                                 (
            PlanTable(Names)                           ,
            "uuid"                                     ,
            vLanguageId                                ,
            u                                        ) ;
      NewTreeWidgetItem ( it )                         ;
      it -> setData ( 0 , Qt::UserRole , u )           ;
      it -> setText ( 0 , N                )           ;
      ui -> TypeLists -> addTopLevelItem ( it )        ;
    }                                                  ;
    ////////////////////////////////////////////////////
    U = GetOwners                                      (
          SC                                           ,
          ObjectUuid()                                 ,
          Types::Division                              ,
          ObjectType()                                 ,
          Groups::Subordination                        ,
          SC.OrderBy("id",Qt::AscendingOrder)        ) ;
    foreach (u,U)                                      {
      Q = SC . sql . SelectFrom                        (
            "type"                                     ,
            PlanTable(Divisions)                       ,
            SC.WhereUuid(u)                          ) ;
      if (SC.Fetch(Q))                                 {
        int tt = SC.Int(0)                             ;
        if ( Types::Organization == tt )               {
          N = SC . getName                             (
                PlanTable(Names)                       ,
                "uuid"                                 ,
                vLanguageId                            ,
                u                                    ) ;
            NewTreeWidgetItem ( it )                   ;
            it -> setData ( 0 , Qt::UserRole , u )     ;
            it -> setText ( 0 , N                )     ;
            ui -> GroupLists -> addTopLevelItem ( it ) ;
        }                                              ;
      }                                                ;
    }                                                  ;
    ////////////////////////////////////////////////////
    SC . close    (    )                               ;
  }                                                    ;
  SC   . remove   (    )                               ;
}

void N::OrganizationEditor::NameChanged(void)
{
  QString name = ui->Name->text()                 ;
  if (name.length()<=0) return                    ;
  if (ObjectUuid()>0) UpdateOrganization ( name ) ;
                 else AppendOrganization ( name ) ;
}

bool N::OrganizationEditor::AppendName (
       SqlConnection & SC              ,
       SUID            uuid            ,
       QString         name            )
{
  GroupItems GI ( plan )        ;
  SUID nuid                     ;
  nuid = SC . Unique            (
           PlanTable(MajorUuid) ,
           "uuid"               ,
           77892              ) ;
  SC . assureUuid               (
    PlanTable(MajorUuid)        ,
    nuid                        ,
    Types::Name               ) ;
  SC . assureName               (
    PlanTable(Names)            ,
    nuid                        ,
    vLanguageId                 ,
    name                      ) ;
  GI . InsertNameMap            (
    SC                          ,
    uuid                        ,
    nuid                      ) ;
  return true                   ;
}

void N::OrganizationEditor::PrepareOrganization(QString name)
{
  ui -> Name -> blockSignals ( true               ) ;
  ui -> Name -> setText      ( name               ) ;
  ui -> Name -> blockSignals ( false              ) ;
  ui -> Name -> setFocus     ( Qt::TabFocusReason ) ;
}

void N::OrganizationEditor::AppendOrganization(QString name)
{
  SqlConnection SC ( plan->sql )                          ;
  if (SC.open("OrganizationEditor","AppendOrganization")) {
    SUID       u                                          ;
    u  = SC.Unique                                        (
           PlanTable(MajorUuid)                           ,
           "uuid"                                         ,
           77891                                        ) ;
    SC . assureUuid                                       (
      PlanTable(MajorUuid)                                ,
      u                                                   ,
      Types::Organization                               ) ;
    SC . insertUuid                                       (
      PlanTable(Organizations)                            ,
      u                                                   ,
      "uuid"                                            ) ;
    AppendName ( SC , u , name )                          ;
    if (u>0) setObjectUuid ( u )                          ;
    SC . close    (    )                                  ;
  }                                                       ;
  SC   . remove   (    )                                  ;
  if (ObjectUuid()<=0) return                             ;
  startup ( )                                             ;
}

void N::OrganizationEditor::UpdateOrganization(QString name)
{
  if (names->topLevelItemCount()<=0) return               ;
  QTreeWidgetItem * it = names->topLevelItem(0)           ;
  if (IsNull(it)) return                                  ;
  SUID nuid = nTreeUuid ( it , 0 )                        ;
  if (nuid<=0) return                                     ;
  int  lang = nTreeInt  ( it , 1 )                        ;
  if (lang<=0) return                                     ;
  SqlConnection SC ( plan->sql )                          ;
  if (SC.open("OrganizationEditor","UpdateOrganization")) {
    QString    Q                                          ;
    QByteArray B = name.toUtf8()                          ;
    Q = SC . sql . Update                                 (
          PlanTable(Names)                                ,
          SC.sql.Where(2,"uuid","language")               ,
          2                                               ,
          "name"                                          ,
          "length"                            )           ;
    SC . Prepare  ( Q                         )           ;
    SC . Bind     ( "uuid"     , nuid         )           ;
    SC . Bind     ( "language" , lang         )           ;
    SC . Bind     ( "name"     , B            )           ;
    SC . Bind     ( "length"   , B.size()     )           ;
    SC . Exec     (                           )           ;
    it -> setText ( 0 , name                  )           ;
    SC . close    (                           )           ;
  }                                                       ;
  SC   . remove   (                           )           ;
  Alert           ( Done                      )           ;
}

void N::OrganizationEditor::AddGroup(void)
{
  if (ObjectUuid()<=0) return                              ;
  UUIDs U                                                  ;
  SUID  u                                                  ;
  for (int i=0;i<ui->GroupLists->topLevelItemCount();i++)  {
    QTreeWidgetItem * it = ui->GroupLists->topLevelItem(i) ;
    U << nTreeUuid ( it , 0 )                              ;
  }                                                        ;
  u = ocgroups -> toUuid ( )                               ;
  if (U.contains(u)) return                                ;
  QString n = ocgroups->currentText()                      ;
  SqlConnection SC ( plan->sql )                           ;
  if (SC.open("OrganizationEditor","AddGroup"))            {
    U  . clear      ( )                                    ;
    U << ObjectUuid ( )                                    ;
    Join ( SC                                              ,
           u                                               ,
           Types::Division                                 ,
           ObjectType()                                    ,
           Groups::Subordination                           ,
           0                                               ,
           U                                               ,
           false                                         ) ;
    NewTreeWidgetItem ( item )                             ;
    item -> setData ( 0 , Qt::UserRole , u )               ;
    item -> setText ( 0 , n                )               ;
    ui   -> GroupLists->addTopLevelItem(item)              ;
    SC . close    (                           )            ;
  }                                                        ;
  SC   . remove   (                           )            ;
  Alert           ( Done                      )            ;
}

void N::OrganizationEditor::DeleteGroup(void)
{
  if (ObjectUuid()<=0) return                          ;
  QTreeWidgetItem * it = ui->GroupLists->currentItem() ;
  if (IsNull(it)) return                               ;
  SUID u = nTreeUuid ( it , 0 )                        ;
  SqlConnection SC ( plan->sql )                       ;
  if (SC.open("OrganizationEditor","DeleteGroup"))     {
    UUIDs U                                            ;
    int   idx                                          ;
    U << ObjectUuid ( )                                ;
    Detach ( SC                                        ,
             u                                         ,
             Types::Division                           ,
             ObjectType()                              ,
             Groups::Subordination                     ,
             U                                       ) ;
    idx = ui->GroupLists->indexOfTopLevelItem(it)      ;
    if (idx>=0)                                        {
      ui->GroupLists->takeTopLevelItem(idx)            ;
    }                                                  ;
    SC . close    (                          )         ;
  }                                                    ;
  SC   . remove   (                          )         ;
  Alert           ( Done                     )         ;
}

void N::OrganizationEditor::AddType(void)
{
  if (ObjectUuid()<=0) return                             ;
  UUIDs U                                                 ;
  SUID  u                                                 ;
  for (int i=0;i<ui->TypeLists->topLevelItemCount();i++)  {
    QTreeWidgetItem * it = ui->TypeLists->topLevelItem(i) ;
    U << nTreeUuid ( it , 0 )                             ;
  }                                                       ;
  u = octypes -> toUuid ( )                               ;
  if (U.contains(u)) return                               ;
  QString n = octypes->currentText()                      ;
  SqlConnection SC ( plan->sql )                          ;
  if (SC.open("OrganizationEditor","AddType"))            {
    U  . clear      ( )                                   ;
    U << ObjectUuid ( )                                   ;
    Join ( SC                                             ,
           u                                              ,
           Types::TypeOfOrganization                      ,
           ObjectType()                                   ,
           Groups::Subordination                          ,
           0                                              ,
           U                                              ,
           false                                        ) ;
    NewTreeWidgetItem ( item )                            ;
    item -> setData ( 0 , Qt::UserRole , u )              ;
    item -> setText ( 0 , n                )              ;
    ui   -> TypeLists->addTopLevelItem(item)              ;
    SC . close    (                          )            ;
  }                                                       ;
  SC   . remove   (                          )            ;
  Alert           ( Done                     )            ;
}

void N::OrganizationEditor::DeleteType(void)
{
  if (ObjectUuid()<=0) return                         ;
  QTreeWidgetItem * it = ui->TypeLists->currentItem() ;
  if (IsNull(it)) return                              ;
  SUID u = nTreeUuid ( it , 0 )                       ;
  SqlConnection SC ( plan->sql )                      ;
  if (SC.open("OrganizationEditor","DeleteType"))     {
    UUIDs U                                           ;
    int   idx                                         ;
    U << ObjectUuid ( )                               ;
    Detach ( SC                                       ,
             u                                        ,
             Types::TypeOfOrganization                ,
             ObjectType()                             ,
             Groups::Subordination                    ,
             U                                      ) ;
    idx = ui->TypeLists->indexOfTopLevelItem(it)      ;
    if (idx>=0)                                       {
      ui->TypeLists->takeTopLevelItem(idx)            ;
    }                                                 ;
    SC . close    (                          )        ;
  }                                                   ;
  SC   . remove   (                          )        ;
  Alert           ( Done                     )        ;
}
