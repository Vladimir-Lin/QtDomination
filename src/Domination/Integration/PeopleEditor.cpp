#include <domination.h>
#include "ui_PeopleEditor.h"

N::PeopleEditor:: PeopleEditor   ( QWidget * parent,Plan * p )
                : StackedWidget  (           parent,       p )
                , Object         ( 0 , Types::People         )
                , GroupItems     (                         p )
                , PeopleManager  (                         p )
                , PictureManager (                         p )
                , ui             ( new Ui::PeopleEditor      )
                , reporter       ( NULL                      )
                , names          ( NULL                      )
                , gallery        ( NULL                      )
                , potentials     ( NULL                      )
                , subgroups      ( NULL                      )
                , faces          ( NULL                      )
                , videos         ( NULL                      )
                , documents      ( NULL                      )
                , bookmarks      ( NULL                      )
                , sexuality      ( NULL                      )
                , hairs          ( NULL                      )
                , eyes           ( NULL                      )
                , processor      ( NULL                      )
                , pan            ( NULL                      )
                , crowds         ( NULL                      )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PeopleEditor::~PeopleEditor(void)
{
  delete ui ;
}

QSize N::PeopleEditor::sizeHint(void) const
{
  return QSize ( 800 , 600 ) ;
}

void N::PeopleEditor::Configure(void)
{
  QFont f                                                       ;
  f = plan -> fonts [ Fonts::ComboBox ]                         ;
  ///////////////////////////////////////////////////////////////
  ui   -> setupUi                  ( this                     ) ;
  ///////////////////////////////////////////////////////////////
  ui   -> Profile    -> setWidget  ( ui -> EditPeople         ) ;
  ///////////////////////////////////////////////////////////////
  sexuality  = new SexualityList   ( ui -> IconDetails , plan ) ;
  sexuality -> setMinimumHeight    ( f . pixelSize ( ) + 6    ) ;
  sexuality -> setMaximumHeight    ( f . pixelSize ( ) + 6    ) ;
  ui -> IconDetails -> addWidget   ( sexuality                ) ;
  ///////////////////////////////////////////////////////////////
  hairs      = new HairsList       ( ui -> IconDetails , plan ) ;
  hairs     -> setMinimumHeight    ( f . pixelSize ( ) + 6    ) ;
  hairs     -> setMaximumHeight    ( f . pixelSize ( ) + 6    ) ;
  ui -> IconDetails -> addWidget   ( hairs                    ) ;
  ///////////////////////////////////////////////////////////////
  eyes       = new EyesList        ( ui -> IconDetails , plan ) ;
  eyes      -> setMinimumHeight    ( f . pixelSize ( ) + 6    ) ;
  eyes      -> setMaximumHeight    ( f . pixelSize ( ) + 6    ) ;
  ui -> IconDetails -> addWidget   ( eyes                     ) ;
  ///////////////////////////////////////////////////////////////
  pan        = new PanWidget       ( ui->EditPeople , plan    ) ;
  ui -> EditPeople -> addWidget    ( pan                      ) ;
  ///////////////////////////////////////////////////////////////
  processor  = NewProcessor        ( ui->EditPeople           ) ;
  processor -> setPrompt           ( tr("Command >")          ) ;
  processor -> setMinimumHeight    ( 40                       ) ;
  ui -> EditPeople -> addWidget    ( processor                ) ;
  ///////////////////////////////////////////////////////////////
  QSplitter * split                                             ;
  ///////////////////////////////////////////////////////////////
  split = new QSplitter            ( Qt::Vertical , pan       ) ;
  ui -> BloodHeight   -> setParent ( split                    ) ;
  ui -> BodyMeasures  -> setParent ( split                    ) ;
  ui -> HistoryEvents -> setParent ( split                    ) ;
  ui -> PeopleHistory -> setParent ( split                    ) ;
  split -> addWidget               ( ui -> BloodHeight        ) ;
  split -> addWidget               ( ui -> BodyMeasures       ) ;
  split -> addWidget               ( ui -> HistoryEvents      ) ;
  split -> addWidget               ( ui -> PeopleHistory      ) ;
  pan   -> addWidget               ( split                    ) ;
  ///////////////////////////////////////////////////////////////
  split  = new QSplitter           ( Qt::Vertical , pan       ) ;
  ui -> PeopleCrowds  -> setParent ( split                    ) ;
  ui -> PeopleGroups  -> setParent ( split                    ) ;
  crowds = new ComboBox            ( ui->PeopleCrowds , plan  ) ;
  crowds-> setMinimumHeight      ( ui->PeopleCrowds->height() ) ;
  crowds-> setMaximumHeight      ( ui->PeopleCrowds->height() ) ;
  crowds-> setInsertPolicy         ( QComboBox::NoInsert      ) ;
  crowds-> setEditable             ( true                     ) ;
  ui    -> PeopleCrowds->addWidget ( crowds                   ) ;
  split -> addWidget               ( ui -> PeopleCrowds       ) ;
  split -> addWidget               ( ui -> PeopleGroups       ) ;
  pan   -> addWidget               ( split                    ) ;
  ///////////////////////////////////////////////////////////////
  QDateTime DX = nTimeNow                                       ;
  DX = DX . addSecs                ( - 86400 * 365 * 20       ) ;
  ui -> PeopleTime -> blockSignals ( true                     ) ;
  ui -> PeopleTime -> setDateTime  ( DX                       ) ;
  ui -> PeopleTime -> blockSignals ( false                    ) ;
  ///////////////////////////////////////////////////////////////
  ui -> HistoryTypes->blockSignals ( true                     ) ;
  ui -> HistoryTypes->addItem      ( tr("Birthday") , History::PeopleBirth ) ;
  ui -> HistoryTypes->addItem      ( tr("Death"   ) , History::PeopleDie   ) ;
  ui -> HistoryTypes->blockSignals ( false                    ) ;
  ///////////////////////////////////////////////////////////////
  f = plan -> fonts [ Fonts::ListView ]                         ;
  NewTreeWidgetItem ( peopleHead )                              ;
  peopleHead -> setText ( 0, tr ( "Item"   ) )                  ;
  peopleHead -> setText ( 1, tr ( "Time"   ) )                  ;
  peopleHead -> setText ( 2, tr ( "Explain") )                  ;
  for (int i=0;i<3;i++)                                         {
    peopleHead -> setFont          ( i , f                    ) ;
    peopleHead -> setTextAlignment ( i , Qt::AlignCenter      ) ;
  }                                                             ;
  ui -> PeopleHistory -> setHeaderItem ( peopleHead )           ;
  ///////////////////////////////////////////////////////////////
  NewTreeWidgetItem ( groupHead )                               ;
  groupHead -> setText          ( 0 , tr ( "Crowd")           ) ;
  groupHead -> setFont          ( 0 , f                       ) ;
  groupHead -> setTextAlignment ( 0 , Qt::AlignCenter         ) ;
  ui -> PeopleGroups -> setHeaderItem ( groupHead )             ;
  ///////////////////////////////////////////////////////////////
  ui -> Blood -> addItem           ( "O"                      ) ;
  ui -> Blood -> addItem           ( "A"                      ) ;
  ui -> Blood -> addItem           ( "B"                      ) ;
  ui -> Blood -> addItem           ( "AB"                     ) ;
  ///////////////////////////////////////////////////////////////
  ui -> BloodHeight   -> show      (                          ) ;
  ui -> BodyMeasures  -> show      (                          ) ;
  ui -> HistoryEvents -> show      (                          ) ;
  ui -> PeopleHistory -> show      (                          ) ;
  ui -> PeopleCrowds  -> show      (                          ) ;
  ui -> PeopleGroups  -> show      (                          ) ;
  ///////////////////////////////////////////////////////////////
  sexuality -> setEnabled          ( false                    ) ;
  hairs     -> setEnabled          ( false                    ) ;
  eyes      -> setEnabled          ( false                    ) ;
  processor -> setEnabled          ( false                    ) ;
  ///////////////////////////////////////////////////////////////
  plan -> setFont                  ( this                     ) ;
}

N::PeopleReporter * N::PeopleEditor::NewReporter (QWidget * widget)
{
  return new PeopleReporter ( widget , plan ) ;
}

N::PeopleProcessor * N::PeopleEditor::NewProcessor (QWidget * widget)
{
  return new PeopleProcessor ( widget , plan ) ;
}

void N::PeopleEditor::startup(void)
{
  if (ObjectUuid()<=0)                                              {
    ui -> PeopleName -> setFocus ( Qt::TabFocusReason )             ;
    return                                                          ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  processor  -> setObjectUuid ( ObjectUuid() )                      ;
  ///////////////////////////////////////////////////////////////////
  reporter    = NewReporter ( ui->Main )                            ;
  ui -> Main -> insertTab ( 0 , reporter , tr("Information") )      ;
  ui -> Main -> setCurrentWidget ( reporter )                       ;
  ///////////////////////////////////////////////////////////////////
  names  = new ItemNames ( ui -> Details , plan )                   ;
  names -> setOwner ( ObjectUuid() , ObjectType() )                 ;
  ui    -> Details -> addWidget ( names )                           ;
  ///////////////////////////////////////////////////////////////////
  gallery = new ObjectGallery ( ui -> Main , plan )                 ;
  gallery -> setFirst ( ObjectUuid() , ObjectType() )               ;
  gallery -> MountCommands (      )                                 ;
  ///////////////////////////////////////////////////////////////////
  potentials  = new ObjectGallery ( ui -> Main , plan )             ;
  potentials -> setFirst                                            (
                  ObjectUuid ( )                                    ,
                  ObjectType ( )                                    ,
                  Groups::Possible                                ) ;
  potentials -> MountCommands ( )                                   ;
  ///////////////////////////////////////////////////////////////////
  subgroups  = new SubgroupPictures ( ui -> Main , plan )           ;
  subgroups -> setObjectUuid  ( ObjectUuid()                  )     ;
  subgroups -> setObjectType  ( N::Types::People              )     ;
  ///////////////////////////////////////////////////////////////////
  faces  = new FaceView ( ui->Main , plan )                         ;
  faces -> Group        = ObjectUuid ( )                            ;
  faces -> SourceType   = Types  :: People                          ;
  faces -> Relationship = Groups :: Subordination                   ;
  faces -> Operation    = 0                                         ;
  ///////////////////////////////////////////////////////////////////
  videos    = new VideoContainer ( ui->Main , plan )                ;
  videos   -> t1 = Types::Album                                     ;
  videos   -> setSecond ( ObjectUuid() , Types::People )            ;
  ///////////////////////////////////////////////////////////////////
  documents = new DocumentItems ( ui->Main , plan )                 ;
  documents -> SourceType = Types::People                           ;
  ///////////////////////////////////////////////////////////////////
  bookmarks  = new Bookmarks ( ui->Main , plan )                    ;
  bookmarks -> setObjectUuid ( ObjectUuid ( )  )                    ;
  bookmarks -> setObjectType ( ObjectType ( )  )                    ;
  ///////////////////////////////////////////////////////////////////
  ui -> Main -> addTab ( gallery    , tr("Gallery"          ) )     ;
  ui -> Main -> addTab ( potentials , tr("Possible pictures") )     ;
  ui -> Main -> addTab ( subgroups  , tr("Subgroups"        ) )     ;
  ui -> Main -> addTab ( faces      , tr("Faces"            ) )     ;
  ui -> Main -> addTab ( videos     , tr("Video albums"     ) )     ;
  ui -> Main -> addTab ( documents  , tr("Documents"        ) )     ;
  ui -> Main -> addTab ( bookmarks  , tr("Bookmarks"        ) )     ;
  ///////////////////////////////////////////////////////////////////
  gallery    -> setLimitValue ( 49 , 0         )                    ;
  potentials -> setLimitValue ( 49 , 0         )                    ;
  videos     -> setLimitValue ( 49 , 0         )                    ;
  ///////////////////////////////////////////////////////////////////
  plan       -> processEvents (                )                    ;
  names      -> Reload        (                )                    ;
  reporter   -> startup       ( ObjectUuid ( ) )                    ;
  gallery    -> Reload        ( true           )                    ;
  potentials -> Reload        ( true           )                    ;
  subgroups  -> startup       (                )                    ;
  faces      -> startup       (                )                    ;
  videos     -> startup       (                )                    ;
  documents  -> startup       ( ObjectUuid ( ) )                    ;
  bookmarks  -> startup       (                )                    ;
  sexuality  -> List          (                )                    ;
  hairs      -> List          (                )                    ;
  eyes       -> List          (                )                    ;
  crowds     -> addDivision   ( Types::People  )                    ;
  ///////////////////////////////////////////////////////////////////
  sexuality  -> setEnabled    ( true           )                    ;
  hairs      -> setEnabled    ( true           )                    ;
  eyes       -> setEnabled    ( true           )                    ;
  processor  -> setEnabled    ( true           )                    ;
  ///////////////////////////////////////////////////////////////////
  Load                        (                )                    ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( sexuality , SIGNAL(activated   (int))                  ,
             this      , SLOT  (PeopleSex   (int))                ) ;
  nConnect ( hairs     , SIGNAL(activated   (int))                  ,
             this      , SLOT  (PeopleHairs (int))                ) ;
  nConnect ( eyes      , SIGNAL(activated   (int))                  ,
             this      , SLOT  (PeopleEyes  (int))                ) ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( subgroups  , SIGNAL ( SeePictures (SUID,SUID,QString) )   ,
             this       , SIGNAL ( SeePictures (SUID,SUID,QString) ) ) ;
  ///////////////////////////////////////////////////////////////////
  nConnect ( gallery    , SIGNAL ( Command       (QWidget*,QString,SUID))   ,
             this       , SIGNAL ( Command       (QWidget*,QString,SUID)) ) ;
  nConnect ( potentials , SIGNAL ( Command       (QWidget*,QString,SUID))   ,
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
  nConnect ( bookmarks , SIGNAL(Web(QUrl,bool))                  ,
             this      , SIGNAL(Web(QUrl,bool))                ) ;
  nConnect ( bookmarks , SIGNAL(ImportRules(QString,SUID,int))   ,
             this      , SIGNAL(ImportRules(QString,SUID,int)) ) ;
  nConnect ( bookmarks , SIGNAL(RunSets(VarArgLists&))           ,
             this      , SIGNAL(RunSets(VarArgLists&))         ) ;
}

void N::PeopleEditor::NameUpdate(QWidget * widget,SUID uuid)
{
  if (names != widget              ) return        ;
  if (names->topLevelItemCount()<=0) return        ;
  if (uuid<=0                      ) return        ;
  QTreeWidgetItem * it = names->topLevelItem(0)    ;
  if (IsNull(it)) return                           ;
  ui -> PeopleName -> blockSignals ( true        ) ;
  ui -> PeopleName -> setText      ( it->text(0) ) ;
  ui -> PeopleName -> blockSignals ( false       ) ;
}

void N::PeopleEditor::Load(void)
{
  nDropOut         ( ObjectUuid() <=0 )                  ;
  SqlConnection SC ( plan->sql        )                  ;
  if (SC.open("PeopleEditor","Load"))                    {
    QStringList names                                    ;
    names = PeopleManager :: Names ( SC , ObjectUuid() ) ;
    if (names.count()>0)                                 {
      setWindowTitle                   ( names [0] )     ;
      ui -> PeopleName -> blockSignals ( true      )     ;
      ui -> PeopleName -> setText      ( names[0]  )     ;
      ui -> PeopleName -> blockSignals ( false     )     ;
      SUID puid = FindSecond                             (
                    SC                                   ,
                    ObjectUuid()                         ,
                    Types::People                        ,
                    Types::Picture                       ,
                    Groups::Icon                         ,
                    "order by position asc limit 0,1"  ) ;
      if  (puid>0)                                       {
        QImage * image = Thumb ( SC , puid )             ;
        if (NotNull(image))                              {
          QPixmap pix = QPixmap::fromImage(*image)       ;
          ui->PeopleIcon->setPixmap(pix)                 ;
          delete image                                   ;
        }                                                ;
      }                                                  ;
    }                                                    ;
    //////////////////////////////////////////////////////
    SUID gender = PeopleGender ( SC , ObjectUuid() )     ;
    if (gender>0) (*sexuality) <= gender                 ;
    //////////////////////////////////////////////////////
    SUID hair = FindFirst                                (
                  SC                                     ,
                  ObjectUuid()                           ,
                  Types::Hairs                           ,
                  Types::People                          ,
                  Groups::Subordination                  ,
                  SC.OrderByAsc("position")            ) ;
    if (hair>0) (*hairs) <= hair                         ;
    //////////////////////////////////////////////////////
    SUID eye  = FindFirst                                (
                  SC                                     ,
                  ObjectUuid()                           ,
                  Types::Eyes                            ,
                  Types::People                          ,
                  Groups::Subordination                  ,
                  SC.OrderByAsc("position")            ) ;
    if (eye>0) (*eyes) <= eye                            ;
    //////////////////////////////////////////////////////
    QString V                                            ;
    if (Variable(SC,ObjectUuid(),"Blood" ,V))            {
      int bid = ui->Blood->findText(V)                   ;
      if (bid>=0)                                        {
        ui -> Blood -> blockSignals    ( true  )         ;
        ui -> Blood -> setCurrentIndex ( bid   )         ;
        ui -> Blood -> blockSignals    ( false )         ;
      }                                                  ;
    }                                                    ;
    if (Variable(SC,ObjectUuid(),"Weight",V))            {
      ui -> Weight -> blockSignals ( true      )         ;
      ui -> Weight -> setValue     ( V.toInt() )         ;
      ui -> Weight -> blockSignals ( false     )         ;
    }                                                    ;
    if (Variable(SC,ObjectUuid(),"Height",V))            {
      ui -> Height -> blockSignals ( true      )         ;
      ui -> Height -> setValue     ( V.toInt() )         ;
      ui -> Height -> blockSignals ( false     )         ;
    }                                                    ;
    if (Variable(SC,ObjectUuid(),"Bust"  ,V))            {
      ui -> Bust   -> blockSignals ( true      )         ;
      ui -> Bust   -> setValue     ( V.toInt() )         ;
      ui -> Bust   -> blockSignals ( false     )         ;
    }                                                    ;
    if (Variable(SC,ObjectUuid(),"Waist" ,V))            {
      ui -> Waist  -> blockSignals ( true      )         ;
      ui -> Waist  -> setValue     ( V.toInt() )         ;
      ui -> Waist  -> blockSignals ( false     )         ;
    }                                                    ;
    if (Variable(SC,ObjectUuid(),"Hip"   ,V))            {
      ui -> Hip    -> blockSignals ( true      )         ;
      ui -> Hip    -> setValue     ( V.toInt() )         ;
      ui -> Hip    -> blockSignals ( false     )         ;
    }                                                    ;
    //////////////////////////////////////////////////////
    DisplayBirth  ( SC )                                 ;
    DisplayGroups ( SC )                                 ;
    //////////////////////////////////////////////////////
    SC . close    (    )                                 ;
  }                                                      ;
  SC   . remove   (    )                                 ;
}

void N::PeopleEditor::DisplayGroups(SqlConnection & SC)
{
  UUIDs GUIDs = GetOwners                     (
                  SC                          ,
                  ObjectUuid()                ,
                  Types::Division             ,
                  ObjectType()                ,
                  Groups::Subordination       ,
                  SC.OrderByAsc("id")       ) ;
  ui->PeopleGroups->clear()                   ;
  if (GUIDs.count()<=0) return                ;
  QString Q                                   ;
  SUID    u                                   ;
  foreach (u,GUIDs)                           {
    Q = SC . sql . SelectFrom                 (
          "type"                              ,
          PlanTable(Divisions)                ,
          SC.WhereUuid(u)                   ) ;
    if (SC.Fetch(Q))                          {
      int t = SC . Int ( 0 )                  ;
      if ( Types::People == t )               {
        QString n                             ;
        n = SC . getName                      (
              PlanTable(Names)                ,
              "uuid"                          ,
              vLanguageId                     ,
              u                             ) ;
        NewTreeWidgetItem ( it )              ;
        it->setData ( 0 , Qt::UserRole , u  ) ;
        it->setText ( 0 , n                 ) ;
        ui->PeopleGroups->addTopLevelItem(it) ;
      }                                       ;
    }                                         ;
  }                                           ;
}

void N::PeopleEditor::DisplayBirth(SqlConnection & SC)
{
  UUIDs TUIDs = Subordination                                        (
                SC                                                   ,
                ObjectUuid()                                         ,
                Types::People                                        ,
                Types::History                                       ,
                Groups::Subordination                                ,
                SC.OrderByAsc("position") )                          ;
  if (TUIDs.count()<=0) return                                       ;
  ////////////////////////////////////////////////////////////////////
  QTreeWidgetItem * birth = NULL                                     ;
  QTreeWidgetItem * death = NULL                                     ;
  QDateTime         BirthTime                                        ;
  QDateTime         DeathTime                                        ;
  ////////////////////////////////////////////////////////////////////
  HistoryManager HM ( plan )                                         ;
  SUID xuid = 0                                                      ;
  TUID tuid = 0                                                      ;
  ////////////////////////////////////////////////////////////////////
  xuid = HM . GetHistoryType                                         (
           SC                                                        ,
           TUIDs                                                     ,
           History::PeopleBirth                                    ) ;
  if (xuid>0)                                                        {
    tuid = HM.GetTime(SC,xuid)                                       ;
    if (tuid>0)                                                      {
      QDateTime D                                                    ;
      D = HM.toDateTime(tuid)                                        ;
      QString DS                                                     ;
      QString TS                                                     ;
      TS        = tr("Birthday")                                     ;
      DS        = D.toString("yyyy/MM/dd hh:mm:ss")                  ;
      BirthTime = D                                                  ;
      birth     = new QTreeWidgetItem ( )                            ;
      birth    -> setData ( 0 , Qt::UserRole , xuid )                ;
      birth    -> setData ( 1 , Qt::UserRole , History::PeopleBirth ) ;
      birth    -> setText ( 0 , TS                  )                ;
      birth    -> setText ( 1 , DS                  )                ;
      ui -> PeopleTime -> blockSignals ( true  )                     ;
      ui -> PeopleTime -> setDateTime  ( D     )                     ;
      ui -> PeopleTime -> blockSignals ( false )                     ;
    }                                                                ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  xuid = 0                                                           ;
  tuid = 0                                                           ;
  ////////////////////////////////////////////////////////////////////
  xuid = HM . GetHistoryType                                         (
           SC                                                        ,
           TUIDs                                                     ,
           History::PeopleDie                                      ) ;
  if (xuid>0)                                                        {
    tuid = HM.GetTime(SC,xuid)                                       ;
    if (tuid>0)                                                      {
      QDateTime D                                                    ;
      D = HM.toDateTime(tuid)                                        ;
      QString DS                                                     ;
      QString TS                                                     ;
      QString AS                                                     ;
      TS        = tr("Death")                                        ;
      DS        = D.toString("yyyy/MM/dd hh:mm:ss")                  ;
      DeathTime = D                                                  ;
      death     = new QTreeWidgetItem ( )                            ;
      AS        = setAge  ( D , nTimeNow                     )       ;
      death    -> setData ( 0 , Qt::UserRole , xuid          )       ;
      birth    -> setData ( 1 , Qt::UserRole , History::PeopleDie )  ;
      death    -> setText ( 0 , TS                           )       ;
      death    -> setText ( 1 , DS                           )       ;
      death    -> setText ( 2 , tr("%1 after death").arg(AS) )       ;
    }                                                                ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  if (NotNull(birth))                                                {
    QString AS                                                       ;
    if (NotNull(death))                                              {
      AS = setAge  ( BirthTime , DeathTime )                         ;
    } else                                                           {
      AS = setAge  ( BirthTime , nTimeNow  )                         ;
    }                                                                ;
    birth -> setText ( 2 , tr("Age %1").arg(AS) )                    ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  ui -> PeopleHistory -> clear ( )                                   ;
  if (NotNull(birth)) ui -> PeopleHistory -> addTopLevelItem (birth) ;
  if (NotNull(death)) ui -> PeopleHistory -> addTopLevelItem (death) ;
  ////////////////////////////////////////////////////////////////////
  for (int i=0;i<2;i++)                                              {
    ui -> PeopleHistory -> resizeColumnToContents ( i )              ;
  }                                                                  ;
}

QString N::PeopleEditor::setAge(QDateTime dt,QDateTime Now)
{
  QDate     ND    = Now.date()                                   ;
  QDate     DD    = dt.date()                                    ;
  QDate     XD    = dt.date()                                    ;
  QDate     SD    = dt.date()                                    ;
  int       Secs  = dt.secsTo(Now)                               ;
  int       Days  = 0                                            ;
  int       Years = 0                                            ;
  bool      done  = false                                        ;
  bool      error = false                                        ;
  if (!dt.isValid() || !DD.isValid() || !XD.isValid())           {
    error = true                                                 ;
    done  = true                                                 ;
  }                                                              ;
  if (Secs<0) done = true                                        ;
  Years = ND.year() - DD.year() - 3                              ;
  if (Years<0)                                                   {
    Years = 0                                                    ;
    error = true                                                 ;
    done  = true                                                 ;
  }                                                              ;
  if (Years>0)                                                   {
    if (SD.month()==2 && SD.day()==29)                           {
      int  dxs  = DD.year() + Years                              ;
      bool skip = false                                          ;
      if ((dxs%400)==0) skip = false                        ; else
      if ((dxs%100)==0) skip = true                         ; else
      if ((dxs%  4)==0) skip = false                        ; else
                        skip = true                              ;
      if (skip)                                                  {
        XD.setDate(dxs,3,1)                                      ;
      } else                                                     {
        XD.setDate(dxs,2,29)                                     ;
      }                                                          ;
    } else                                                       {
      XD.setDate(DD.year()+Years,DD.month(),DD.day())            ;
    }                                                            ;
    DD = XD                                                      ;
  }                                                              ;
  while (!done)                                                  {
    if (Years>5000)                                              {
      error = true                                               ;
      done  = true                                               ;
    }                                                            ;
    if (SD.month()==2 && SD.day()==29)                           {
      int  dxs  = DD.year() + 1                                  ;
      bool skip = false                                          ;
      if ((dxs%400)==0) skip = false                        ; else
      if ((dxs%100)==0) skip = true                         ; else
      if ((dxs%  4)==0) skip = false                        ; else
                        skip = true                              ;
      if (skip)                                                  {
        XD.setDate(dxs,3,1)                                      ;
      } else                                                     {
        XD.setDate(dxs,2,29)                                     ;
      }                                                          ;
    } else                                                       {
      XD.setDate(DD.year()+1,DD.month(),DD.day())                ;
    }                                                            ;
    if (XD>ND)                                                   {
      Days = DD.daysTo(ND)                                       ;
      done = true                                                ;
    } else                                                       {
      Years ++                                                   ;
      DD = XD                                                    ;
    }                                                            ;
  }                                                              ;
  ////////////////////////////////////////////////////////////////
  if (error) return ""                                           ;
  return tr("%1 years %2 days").arg(Years).arg(Days)             ;
}

void N::PeopleEditor::NameChanged(void)
{
  QString name = ui->PeopleName->text()     ;
  if (name.length()<=0) return              ;
  if (ObjectUuid()>0) UpdatePeople ( name ) ;
                 else AppendPeople ( name ) ;
}

void N::PeopleEditor::PreparePeople(QString name)
{
  ui -> PeopleName -> blockSignals ( true               ) ;
  ui -> PeopleName -> setText      ( name               ) ;
  ui -> PeopleName -> blockSignals ( false              ) ;
  ui -> PeopleName -> setFocus     ( Qt::TabFocusReason ) ;
}

void N::PeopleEditor::AppendPeople(QString name)
{
  SqlConnection SC ( plan->sql )              ;
  if (SC.open("PeopleEditor","AppendPeople")) {
    SUID u = addPeople(SC,vLanguageId,name)   ;
    if (u>0) setObjectUuid ( u )              ;
    SC . close    (    )                      ;
  }                                           ;
  SC   . remove   (    )                      ;
  if (ObjectUuid()<=0) return                 ;
  startup ( )                                 ;
}

void N::PeopleEditor::UpdatePeople(QString name)
{
  if (names->topLevelItemCount()<=0) return     ;
  QTreeWidgetItem * it = names->topLevelItem(0) ;
  if (IsNull(it)) return                        ;
  SUID nuid = nTreeUuid ( it , 0 )              ;
  if (nuid<=0) return                           ;
  int  lang = nTreeInt  ( it , 1 )              ;
  if (lang<=0) return                           ;
  SqlConnection SC ( plan->sql )                ;
  if (SC.open("PeopleEditor","UpdatePeople"))   {
    QString    Q                                ;
    QByteArray B = name.toUtf8()                ;
    Q = SC . sql . Update                       (
          PlanTable(Names)                      ,
          SC.sql.Where(2,"uuid","language")     ,
          2                                     ,
          "name"                                ,
          "length"                            ) ;
    SC . Prepare  ( Q                         ) ;
    SC . Bind     ( "uuid"     , nuid         ) ;
    SC . Bind     ( "language" , lang         ) ;
    SC . Bind     ( "name"     , B            ) ;
    SC . Bind     ( "length"   , B.size()     ) ;
    SC . Exec     (                           ) ;
    it -> setText ( 0 , name                  ) ;
    SC . close    (                           ) ;
  }                                             ;
  SC   . remove   (                           ) ;
  Alert           ( Done                      ) ;
}

void N::PeopleEditor::AddHistory(void)
{
  if ( ObjectUuid() <= 0 ) return                              ;
  HistoryManager HM ( plan      )                              ;
  SqlConnection  SC ( plan->sql )                              ;
  if (SC.open("PeopleEditor","AddHistory"))                    {
    int       ht                                               ;
    QDateTime hs                                               ;
    StarDate  SD                                               ;
    SUID      tuid = 0                                         ;
    ////////////////////////////////////////////////////////////
    ht = ui->HistoryTypes->currentIndex()                      ;
    ht = ui->HistoryTypes->itemData(ht).toInt()                ;
    hs = ui->PeopleTime  ->dateTime()                          ;
    SD = hs                                                    ;
    for (int i=0;i<ui->PeopleHistory->topLevelItemCount();i++) {
      QTreeWidgetItem * it                                     ;
      it = ui->PeopleHistory->topLevelItem(i)                  ;
      if ( ht == nTreeInt(it,1) ) tuid = nTreeUuid ( it , 0 )  ;
    }                                                          ;
    ////////////////////////////////////////////////////////////
    tuid = HM . assureTime ( SC , tuid , ht , SD.Stardate    ) ;
    if (tuid>0)                                                {
      UUIDs ZUIDs                                              ;
      ZUIDs << tuid                                            ;
      Join                                                     (
        SC                                                     ,
        ObjectUuid ( )                                         ,
        N::Types::People                                       ,
        N::Types::History                                      ,
        N::Groups::Subordination                               ,
        0                                                      ,
        ZUIDs                                                  ,
        false                                                ) ;
    }                                                          ;
    ////////////////////////////////////////////////////////////
    DisplayBirth ( SC                                        ) ;
    SC . close   (                                           ) ;
  }                                                            ;
  SC   . remove  (                                           ) ;
  Alert          ( Done                                      ) ;
}

void N::PeopleEditor::DeleteHistory(void)
{
  if ( ObjectUuid() <= 0 ) return              ;
  QTreeWidgetItem * it                         ;
  it = ui->PeopleHistory->currentItem()        ;
  if (IsNull(it)) return                       ;
  SUID timespot = nTreeUuid ( it , 0 )         ;
  if ( timespot <= 0 ) return                  ;
  SqlConnection SC ( plan->sql )               ;
  if (SC.open("PeopleEditor","DeleteHistory")) {
    UUIDs U                                    ;
    U << timespot                              ;
    Detach       ( SC                          ,
                   ObjectUuid ( )              ,
                   Types  :: People            ,
                   Types  :: History           ,
                   Groups :: Subordination     ,
                   U                         ) ;
    DisplayBirth ( SC                        ) ;
    SC . close   (                           ) ;
  }                                            ;
  SC   . remove  (                           ) ;
  Alert          ( Done                      ) ;
}

void N::PeopleEditor::AddCrowd(void)
{
  if ( ObjectUuid() <= 0 ) return             ;
  SUID crowd = N :: GetUuid ( crowds )        ;
  if ( crowd <= 0 ) return                    ;
  SqlConnection SC ( plan->sql )              ;
  if (SC.open("PeopleEditor","AddCrowd"))     {
    UUIDs U                                   ;
    U << ObjectUuid (                       ) ;
    Join            ( SC                      ,
                      crowd                   ,
                      Types  :: Division      ,
                      Types  :: People        ,
                      Groups :: Subordination ,
                      0                       ,
                      U                       ,
                      false                 ) ;
    DisplayGroups   ( SC                    ) ;
    SC . close      (                       ) ;
  }                                           ;
  SC   . remove     (                       ) ;
  Alert             ( Done                  ) ;
}

void N::PeopleEditor::DeleteCrowd(void)
{
  if ( ObjectUuid() <= 0 ) return             ;
  QTreeWidgetItem * it                        ;
  it = ui->PeopleGroups->currentItem()        ;
  if (IsNull(it)) return                      ;
  SUID crowd = nTreeUuid ( it , 0 )           ;
  if ( crowd <= 0 ) return                    ;
  SqlConnection SC ( plan->sql )              ;
  if (SC.open("PeopleEditor","DeleteCrowd"))  {
    UUIDs U                                   ;
    U << ObjectUuid (                       ) ;
    Detach          ( SC                      ,
                      crowd                   ,
                      Types  :: Division      ,
                      Types  :: People        ,
                      Groups :: Subordination ,
                      U                     ) ;
    DisplayGroups   ( SC                    ) ;
    SC . close      (                       ) ;
  }                                           ;
  SC   . remove     (                       ) ;
  Alert             ( Done                  ) ;
}

void N::PeopleEditor::PeopleSex(int)
{
  if ( ObjectUuid() <= 0 ) return               ;
  SUID sex = N :: GetUuid ( sexuality )         ;
  if ( sex          <= 0 ) return               ;
  SqlConnection SC ( plan->sql )                ;
  if (SC.open("PeopleEditor","PeopleSex"))      {
    AssignSexuality ( SC , ObjectUuid() , sex ) ;
    SC . close      (                         ) ;
  }                                             ;
  SC   . remove     (                         ) ;
  Alert             ( Done                    ) ;
}

void N::PeopleEditor::PeopleHairs(int)
{
  if ( ObjectUuid() <= 0 ) return            ;
  SUID hair = N :: GetUuid ( hairs )         ;
  if ( hair         <= 0 ) return            ;
  if ( ObjectUuid() <= 0 ) return            ;
  SqlConnection SC ( plan->sql )             ;
  if (SC.open("PeopleEditor","PeopleHairs")) {
    QString Q                                ;
    UUIDs   U                                ;
    Q = SC.sql.DeleteFrom                    (
          PlanTable(Groups)                  ,
          SecondItem                         (
            ObjectUuid()                     ,
            Types::Hairs                     ,
            Types::People                    ,
            Groups::Subordination            ,
            ""                               )
        )                                    ;
    SC . Query ( Q )                         ;
    U << ObjectUuid ( )                      ;
    Join                                     (
      SC                                     ,
      hair                                   ,
      Types::Hairs                           ,
      Types::People                          ,
      Groups::Subordination                  ,
      0                                      ,
      U                                      ,
      false                                ) ;
    SC . close      (                      ) ;
  }                                          ;
  SC   . remove     (                      ) ;
  Alert             ( Done                 ) ;
}

void N::PeopleEditor::PeopleEyes(int)
{
  if ( ObjectUuid() <= 0 ) return           ;
  SUID eye = N :: GetUuid ( eyes )          ;
  if ( eye          <= 0 ) return           ;
  SqlConnection SC ( plan->sql )            ;
  if (SC.open("PeopleEditor","PeopleEyes")) {
    QString Q                               ;
    UUIDs   U                               ;
    Q = SC.sql.DeleteFrom                   (
          PlanTable(Groups)                 ,
          SecondItem                        (
            ObjectUuid()                    ,
            Types::Eyes                     ,
            Types::People                   ,
            Groups::Subordination           ,
            ""                              )
        )                                   ;
    SC . Query ( Q )                        ;
    U << ObjectUuid ( )                     ;
    Join                                    (
      SC                                    ,
      eye                                   ,
      Types::Eyes                           ,
      Types::People                         ,
      Groups::Subordination                 ,
      0                                     ,
      U                                     ,
      false                               ) ;
    SC . close      (                     ) ;
  }                                         ;
  SC   . remove     (                     ) ;
  Alert             ( Done                ) ;
}

void N::PeopleEditor::PeopleBlood(int)
{
  if ( ObjectUuid() <= 0 ) return            ;
  QString blood = ui->Blood->currentText()   ;
  if (blood.length()<=0) return              ;
  SqlConnection SC ( plan->sql )             ;
  if (SC.open("PeopleEditor","PeopleBlood")) {
    assureVariable  ( SC                     ,
                      ObjectUuid()           ,
                      Types::People          ,
                      "Blood"                ,
                      blood                ) ;
    SC . close      (                      ) ;
  }                                          ;
  SC   . remove     (                      ) ;
  Alert             ( Done                 ) ;
}

void N::PeopleEditor::PeopleHeight(void)
{
  if ( ObjectUuid() <= 0 ) return             ;
  int H = ui->Height->value()                 ;
  SqlConnection SC ( plan->sql )              ;
  if (SC.open("PeopleEditor","PeopleHeight")) {
    QString M = QString::number(H)            ;
    if (H<=0)                                 {
      deleteVariable ( SC                     ,
                       ObjectUuid()           ,
                       "Height"             ) ;
    } else                                    {
      assureVariable ( SC                     ,
                       ObjectUuid()           ,
                       Types::People          ,
                       "Height"               ,
                       M                    ) ;
    }                                         ;
    SC . close      (                       ) ;
  }                                           ;
  SC   . remove     (                       ) ;
  Alert             ( Done                  ) ;
}

void N::PeopleEditor::PeopleWeight(void)
{
  if ( ObjectUuid() <= 0 ) return             ;
  int W = ui->Weight->value()                 ;
  SqlConnection SC ( plan->sql )              ;
  if (SC.open("PeopleEditor","PeopleWeight")) {
    QString M = QString::number(W)            ;
    if (W<=0)                                 {
      deleteVariable ( SC                     ,
                       ObjectUuid()           ,
                       "Weight"             ) ;
    } else                                    {
      assureVariable ( SC                     ,
                       ObjectUuid()           ,
                       Types::People          ,
                       "Weight"               ,
                       M                    ) ;
    }                                         ;
    SC . close      (                       ) ;
  }                                           ;
  SC   . remove     (                       ) ;
  Alert             ( Done                  ) ;
}

void N::PeopleEditor::PeopleBust(void)
{
  if ( ObjectUuid() <= 0 ) return           ;
  int B = ui->Bust->value()                 ;
  SqlConnection SC ( plan->sql )            ;
  if (SC.open("PeopleEditor","PeopleBust")) {
    QString M = QString::number(B)          ;
    if (B<=0)                               {
      deleteVariable ( SC                   ,
                       ObjectUuid()         ,
                       "Bust"             ) ;
    } else                                  {
      assureVariable ( SC                   ,
                       ObjectUuid()         ,
                       Types::People        ,
                       "Bust"               ,
                       M                  ) ;
    }                                       ;
    SC . close      (                     ) ;
  }                                         ;
  SC   . remove     (                     ) ;
  Alert             ( Done                ) ;
}

void N::PeopleEditor::PeopleWaist(void)
{
  if ( ObjectUuid() <= 0 ) return            ;
  int W = ui->Waist->value()                 ;
  SqlConnection SC ( plan->sql )             ;
  if (SC.open("PeopleEditor","PeopleWaist")) {
    QString M = QString::number(W)           ;
    if (W<=0)                                {
      deleteVariable ( SC                    ,
                       ObjectUuid()          ,
                       "Waist"             ) ;
    } else                                   {
      assureVariable ( SC                    ,
                       ObjectUuid()          ,
                       Types::People         ,
                       "Waist"               ,
                       M                   ) ;
    }                                        ;
    SC . close      (                      ) ;
  }                                          ;
  SC   . remove     (                      ) ;
  Alert             ( Done                 ) ;
}

void N::PeopleEditor::PeopleHip(void)
{
  if ( ObjectUuid() <= 0 ) return          ;
  int H = ui->Hip->value()                 ;
  SqlConnection SC ( plan->sql )           ;
  if (SC.open("PeopleEditor","PeopleHip")) {
    QString M = QString::number(H)         ;
    if (H<=0)                              {
      deleteVariable ( SC                  ,
                       ObjectUuid()        ,
                       "Hip"             ) ;
    } else                                 {
      assureVariable ( SC                  ,
                       ObjectUuid()        ,
                       Types::People       ,
                       "Hip"               ,
                       M                 ) ;
    }                                      ;
    SC . close      (                    ) ;
  }                                        ;
  SC   . remove     (                    ) ;
  Alert             ( Done               ) ;
}

void N::PeopleEditor::run(int Type,ThreadData * data)
{
}
