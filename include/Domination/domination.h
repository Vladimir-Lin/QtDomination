/****************************************************************************
 *                                                                          *
 * Copyright (C) 2001 ~ 2016 Neutrino International Inc.                    *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_DOMINATION_H
#define QT_DOMINATION_H

#include <QtCore>
#include <QtNetwork>
#include <QtSql>
#include <QtScript>
#include <QtPositioning>
#include <QtCURL>
#include <QtCUDA>
#include <QtTorrent>
#include <QtZMQ>
#include <QtPCAP>
#include <QtOpenCV>
#include <QtProlog>
#include <QtLISP>
#include <QtPython>
#include <QtFFmpeg>
#include <QtAudio>
#include <Essentials>
#include <QtCalendar>
#include <QtCryptography>
#include <QtMIME>
#include <SqlAnalyzer>
#include <QtGMP>
#include <QtDAI>
#include <QtGSL>
#include <QtANN>
#include <QtAlgebra>
#include <QtDiscrete>
#include <QtFFT>
#include <Mathematics>
#include <QtFuzzy>
#include <QtFLP>
#include <QtGeography>
#include <NetProtocol>
#include <QtXmlRPC>
#include <QtUDT>
#include <QtFTP>
#include <QtRPC>
#include <QtJsonRPC>
#include <AudioIO>
#include <QtPhonemes>
#include <QtVocal>
#include <QtSpeech>
#include <QtNLP>
#include <QtMatcher>
#include <QtLinguistics>
#include <QtForth>
#include <QtNLC>
#include <QtYacc>
#include <QtFoundation>
#include <QtInference>
#include <QtDecision>
#include <Science>
#include <QtAutomation>
#include <QtAutonomy>
#include <QtGeometry>
#include <QtGadgets>
#include <QtWorld>
#include <QtComponents>
#include <QtAIV>
#include <QtManagers>
#include <Physics>
#include <Chemistry>
#include <Biology>
#include <Business>
#include <QtForex>
#include <Marketing>
#include <QtModels>
#include <QtAnalog>
#include <NetWidgets>
#include <MathWidgets>
#include <QtGIS>
#include <QtCLF>
#include <QtVCF>
#include <DataWidgets>
#include <QtTasks>
#include <QtPhotography>
#include <QtHuman>
#include <Society>
#include <QtAV>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_DOMINATION_LIB)
#      define Q_DOMINATION_EXPORT Q_DECL_EXPORT
#    else
#      define Q_DOMINATION_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_DOMINATION_EXPORT
#endif


namespace Ui                                  {
class Q_DOMINATION_EXPORT nSearchTool         ;
class Q_DOMINATION_EXPORT PeopleEditor        ;
class Q_DOMINATION_EXPORT AlbumEditor         ;
class Q_DOMINATION_EXPORT OrganizationEditor  ;
class Q_DOMINATION_EXPORT ScreenCaptureMethod ;
}

namespace N
{

Q_DOMINATION_EXPORT bool Initialize         (InitialSystem init = InitAll) ;

Q_DOMINATION_EXPORT bool InitAndroid        (void) ;
Q_DOMINATION_EXPORT void AndroidArena       (QDir root,QString TarXz) ;
Q_DOMINATION_EXPORT void AndroidPreparation (QString NDB,QString TarXz) ;
Q_DOMINATION_EXPORT bool InitApple          (void) ;
Q_DOMINATION_EXPORT void AppleArena         (QDir root,QString TarXz) ;
Q_DOMINATION_EXPORT void ApplePreparation   (QString NDB,QString App,QString TarXz) ;

class Q_DOMINATION_EXPORT FullManager         ;
class Q_DOMINATION_EXPORT SearchTool          ;
class Q_DOMINATION_EXPORT ScreenCaptureMethod ;
class Q_DOMINATION_EXPORT SiteSeeker          ;
class Q_DOMINATION_EXPORT SeekerTemplate      ;
class Q_DOMINATION_EXPORT PeopleEditor        ;
class Q_DOMINATION_EXPORT AlbumEditor         ;
class Q_DOMINATION_EXPORT OrganizationEditor  ;
class Q_DOMINATION_EXPORT AudioConfigurator   ;
class Q_DOMINATION_EXPORT CiosFloating        ;
class Q_DOMINATION_EXPORT CiosPanel           ;
class Q_DOMINATION_EXPORT LaunchPanel         ;
class Q_DOMINATION_EXPORT CommonMain          ;
class Q_DOMINATION_EXPORT DesktopMain         ;
class Q_DOMINATION_EXPORT MobileMain          ;

Q_DOMINATION_EXPORT QScriptValue FullAttachment (QScriptContext * context,QScriptEngine * engine) ;

class Q_DOMINATION_EXPORT FullManager : public FoundationManager
{
  Q_OBJECT
  public:

    explicit        FullManager   (QObject * parent , Plan * plan) ;
    virtual        ~FullManager   (void) ;

  protected:

    Plan                       *  fullplan    ;
    NetworkManager             *  netm        ;
    PictureManager             *  picm        ;
    QMap<QString,SqlConnection *> Connections ;

  private:

  public slots:

    virtual bool    RemoveFile    (QString filename) ;

    virtual bool    ConnectSql    (QString name) ;
    virtual bool    RemoveSql     (QString name) ;

    virtual QString ImportPicture (QString filename) ;
    virtual QString ImportPicture (QString connection,QString filename) ;

    virtual bool    DomainIndex   (QString connection) ;
    virtual QString PageUuid      (QString connection,QString url) ;
    virtual QString InsertPage    (QString connection,QString url) ;
    virtual QString AssurePage    (QString connection,QString url) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_DOMINATION_EXPORT SearchTool : public TabWidget
                                     , public AttachDock
{
  Q_OBJECT
  public:

    explicit SearchTool           (StandardConstructor) ;
    virtual ~SearchTool           (void);

  protected:

    Ui::nSearchTool   * ui             ;
    PictureComparsion * PictureCompare ;
    QStringList         MathOps        ;
    int                 Mode           ;
    UUIDs               PictureUuids   ;

    virtual void Configure        (void) ;

    virtual void resizeEvent      (QResizeEvent      * event) ;
    virtual void showEvent        (QShowEvent        * event) ;
    virtual void closeEvent       (QCloseEvent       * event) ;
    virtual void contextMenuEvent (QContextMenuEvent * event) ;

    virtual void run              (void) ;

    virtual void PictureRules     (void) ;

  private:

    UUIDs UUID                    (SqlConnection & SC,QString Q) ;

  public slots:

    virtual bool startup          (void) ;
    virtual bool Relocation       (void) ;
    virtual void PictureScripts   (void) ;
    virtual void SearchPictures   (void) ;
    virtual void AddPictureRules  (void) ;
    virtual void RemovePictures   (void) ;
    virtual void TrashPictures    (void) ;
    virtual void PictureUp        (void) ;
    virtual void PictureDown      (void) ;

    virtual void Docking          (QMainWindow       * Main   ,
                                   QString             title  ,
                                   Qt::DockWidgetArea  area   ,
                                   Qt::DockWidgetAreas areas) ;
    virtual void DockIn           (bool shown) ;

  protected slots:

    void         Visible          (bool visible) ;
    virtual bool Menu             (QPoint pos) ;

    virtual void PictureItems     (int index) ;

  private slots:

    void SendPictures             (void) ;

  signals:

    void EmitPictures             (void) ;
    void Pictures                 (QString name,UUIDs & Uuids) ;
    void PictureScript            (void) ;

};

class Q_DOMINATION_EXPORT ScreenCaptureMethod : public Widget
{
  Q_OBJECT
  public:

    explicit ScreenCaptureMethod (StandardConstructor) ;
    virtual ~ScreenCaptureMethod (void) ;

    bool     isWindow            (void) ;
    bool     isDesktop           (void) ;

    bool     isImmediate         (void) ;
    bool     isDelay             (void) ;
    bool     isPeriod            (void) ;

    int      Seconds             (void) ;
    int      Period              (void) ;
    QString  Path                (void) ;

  protected:

    Ui::ScreenCaptureMethod * ui ;

  private:

  public slots:

    void BrowsePath              (void) ;

  protected slots:

  private slots:

  signals:

};

class Q_DOMINATION_EXPORT SiteSeeker : public QObject
                                     , public LearnSite
                                     , public GroupItems
                                     , public PictureManager
                                     , public PeopleManager
                                     , public VideoManager
                                     , public HistoryManager
                                     , public FinanceManager
{
  Q_OBJECT
  public:

    explicit            SiteSeeker          (QObject * parent,Plan * plan) ;
    virtual            ~SiteSeeker          (void) ;

    virtual int         Type                (void    ) const = 0 ;
    virtual QString     Name                (void    ) const = 0 ;
    virtual bool        Supported           (int type) const = 0 ;
    virtual QString     Name                (int type) const = 0 ;
    virtual QWidget *   Configurator        (int type)       = 0 ;

    virtual QString     Temporary           (QString filename) ;
    virtual QString     Proxy               (void) ;
    virtual int         Port                (void) ;

  protected:

    Plan         * plan            ;
    CommonSeeker * Seeker          ;
    VcfWidgets     Widgets         ;
    BMAPs          Enabled         ;
    UUIDs          Matchings       ;
    UUIDs          Companies       ;
    SMAPs          ToCompanies     ;
    NAMEs          MatchingScripts ;
    NAMEs          CompanyNames    ;
    bool           decompressing   ;
    QMutex         urlLock         ;

    virtual int         DownloadTime        (void) ;
    virtual int         ConnectTimeout      (void) ;
    virtual bool        TryReferer          (void) ;

    virtual bool        Obtain              (SqlConnection & SC        ,
                                             QUrl            url       ,
                                             SUID          & uuid      ,
                                             QByteArray    & XML     ) ;
    virtual bool        AttachPeople        (SqlConnection & SC        ,
                                             QUrl            url       ,
                                             SUID            people    ,
                                             QString         name      ,
                                             SUID            gallery ) ;
    virtual bool        MergePeople         (SqlConnection & SC        ,
                                             SUID            merge     ,
                                             SUID            puid      ,
                                             QStringList     names     ,
                                             int             language) ;
    virtual SUID        UrlPicture          (SqlConnection & SC        ,
                                             SUID            xuid    ) ;
    virtual bool        EraseJava           (QByteArray    & XML     ) ;
    virtual bool        AppendNames         (SqlConnection & SC        ,
                                             SUID            uuid      ,
                                             int             vLanguage ,
                                             QString         name    ) ;
    virtual bool        Between             (RangeInt      & range     ,
                                             QString         start     ,
                                             QString         finish    ,
                                             QByteArray    & XML     ) ;
    virtual QString     Between             (RangeInt      & range     ,
                                             QByteArray    & XML     ) ;
    virtual bool        NamesByOrder        (SqlConnection & SC        ,
                                             SUID            nuid      ,
                                             NAMEs         & names   ) ;
    virtual QString     PurgeSpace          (QString encoded) ;
    virtual QString     FixHtml             (QString encoded) ;
    virtual QString     FixProduct          (QString product) ;
    virtual QStringList FindOutNames        (QString product) ;
    virtual bool        DvdPublish          (QString date,QDateTime & Time) ;
    virtual SUID        MapToCompany        (QString ID) ;
    virtual bool        LoadCompanyProducts (SqlConnection & SC) ;
    virtual bool        JoinAlbum           (SqlConnection & SC,
                                             SUID group,
                                             SUID album) ;
    virtual SUID        LookForVideo        (SqlConnection & SC,
                                             SUID url,
                                             QString identifier) ;
    virtual SUID        assureProduct       (SqlConnection & SC         ,
                                             QString         title      ,
                                             QString         product    ,
                                             UUIDs         & peoples  ) ;
    virtual SUID        assureDocument      (SqlConnection & SC         ,
                                             QString         title      ,
                                             QString         content  ) ;
    virtual SUID        assureVideo         (SqlConnection & SC         ,
                                             QString         title    ) ;
    virtual bool        AttachVideo         (SqlConnection & SC         ,
                                             QUrl            url        ,
                                             QUrl            source     ,
                                             SUID            video      ,
                                             SUID            gallery    ,
                                             UUIDs         & peoples  ) ;
    virtual bool        AttachCompany       (SqlConnection & SC         ,
                                             QString         Product    ,
                                             SUID            ProductId  ,
                                             SUID            MakerId    ,
                                             QString         MakerName  ,
                                             SUID            video      ,
                                             UUIDs         & peoples  ) ;
    virtual SUID        FindCompany         (SqlConnection & SC         ,
                                             QString         name     ) ;
    virtual bool        AttachOrganization  (SqlConnection & SC         ,
                                             QUrl            url        ,
                                             SUID            company  ) ;
    virtual bool        assureEnglish       (SqlConnection & SC         ,
                                             SUID            people     ,
                                             QStringList     English  ) ;

    virtual QString     GZipLocation        (void) ;
    virtual void        Execute             (QString     exe        ,
                                             QStringList paraments) ;
    virtual bool        hadCached           (SqlConnection & SC     ,
                                             QUrl            url    ,
                                             SUID          & zuid   ,
                                             QByteArray    & XML  ) ;
    virtual void        Convert             (SqlConnection & SC     ,
                                             QByteArray    & XML    ,
                                             SUID            puid ) ;

    virtual void        CommodityJoinAlbum  (SqlConnection & SC        ,
                                             SUID            Product   ,
                                             SUID            Album   ) ;
    virtual void        CommodityJoinPeople (SqlConnection & SC        ,
                                             SUID            Product   ,
                                             UUIDs         & peoples ) ;
    virtual void        AlbumJoinPeople     (SqlConnection & SC        ,
                                             SUID            Album     ,
                                             UUIDs         & peoples ) ;
    virtual void        AlbumJoinPictures   (SqlConnection & SC        ,
                                             SUID            Album     ,
                                             UUIDs         & picture ) ;
    virtual void        AlbumJoinURLs       (SqlConnection & SC        ,
                                             SUID            Album     ,
                                             UUIDs         & URLs    ) ;
    virtual void        GalleryJoinPictures (SqlConnection & SC        ,
                                             SUID            Gallery   ,
                                             UUIDs         & picture ) ;
    virtual void        PeopleJoinPictures  (SqlConnection & SC        ,
                                             SUID            people    ,
                                             UUIDs         & picture ) ;
    virtual void        PeopleJoinURLs      (SqlConnection & SC        ,
                                             SUID            people    ,
                                             UUIDs         & URLs    ) ;

  private:

  public slots:

    virtual void        showMessage         (QString message) ;
    virtual void        LookUrl             (QString url) ;
    virtual void        setSeeker           (CommonSeeker * seeker) ;

    virtual bool        setEnabled          (int type,bool enable) = 0 ;
    virtual bool        prepare             (void) = 0 ;
    virtual bool        configure           (QString scope) = 0 ;
    virtual bool        startup             (int type) = 0 ;

  protected slots:

    virtual void        Uncompress          (QString exe,QStringList paraments) ;

  private slots:

  signals:

    void                Ungzip              (QString exe,QStringList paraments) ;

};

class Q_DOMINATION_EXPORT SeekerTemplate : public StackedWidget
{
  Q_OBJECT
  public:

    explicit SeekerTemplate          (StandardConstructor) ;
    virtual ~SeekerTemplate          (void) ;

  protected:

    QString                Prefix    ;
    bool                   shuting   ;
    int                    lines     ;
    CommonSeeker         * seeker    ;
    ComboBox             * hunter    ;
    Sql                    HunterSql ;
    QMap<int,SiteSeeker *> Seekers   ;

    virtual void resizeEvent         (QResizeEvent * event) ;
    virtual void closeEvent          (QCloseEvent  * event) ;

    virtual CUIDs SupportedSeekers   (void) ;
    virtual SiteSeeker * AskSeeker   (int type,QWidget * parent,Plan * plan) ;

    virtual void run                 (int Type,ThreadData * data) ;

    void         loadGeometry        (QString prefix) ;
    void         saveGeometry        (QString prefix) ;

  private:

  public slots:

    virtual bool startup             (void) ;
    virtual bool shutdown            (void) ;

    virtual void showMessage         (QString message) ;
    virtual void Probe               (bool enabled) ;

    virtual void loadGeometry        (void) ;
    virtual void saveGeometry        (void) ;

    virtual void Relocation          (void) ;

    virtual void Initialize          (void) ;
    virtual void Seeking             (void) ;

  protected slots:

    virtual void SeekerChanged       (int state) ;
    virtual void SeekerModified      (void) ;
    virtual void urlPressed          (void) ;
    virtual void LookUrl             (QString url) ;
    virtual void HunterChanged       (int index) ;
    virtual void siteClicked         (QTreeWidgetItem * item,int column) ;
    virtual void LookForTemp         (void) ;
    virtual void TempChanged         (void) ;
    virtual bool ObtainSql           (void) ;

  private slots:

  signals:

};

class Q_DOMINATION_EXPORT PeopleEditor : public StackedWidget
                                       , public Object
                                       , public GroupItems
                                       , public PeopleManager
                                       , public PictureManager
{
  Q_OBJECT
  public:

    explicit PeopleEditor          (StandardConstructor) ;
    virtual ~PeopleEditor          (void) ;

    virtual QSize    sizeHint      (void) const ;

  protected:

    Ui::PeopleEditor * ui         ;
    PeopleReporter   * reporter   ;
    ItemNames        * names      ;
    ObjectGallery    * gallery    ;
    ObjectGallery    * potentials ;
    SubgroupPictures * subgroups  ;
    FaceView         * faces      ;
    VideoContainer   * videos     ;
    DocumentItems    * documents  ;
    Bookmarks        * bookmarks  ;
    SexualityList    * sexuality  ;
    HairsList        * hairs      ;
    EyesList         * eyes       ;
    PeopleProcessor  * processor  ;
    PanWidget        * pan        ;
    ComboBox         * crowds     ;

    virtual void    Configure      (void) ;

    virtual void    run            (int Type,ThreadData * data) ;

    virtual PeopleReporter  * NewReporter  (QWidget * widget) ;
    virtual PeopleProcessor * NewProcessor (QWidget * widget) ;

    virtual void    DisplayBirth   (SqlConnection & SC) ;
    virtual QString setAge         (QDateTime dt,QDateTime et) ;
    virtual void    DisplayGroups  (SqlConnection & SC) ;

  private:

  public slots:

    virtual void    startup        (void) ;
    virtual void    Load           (void) ;

    virtual void    NameChanged    (void) ;
    virtual void    AddHistory     (void) ;
    virtual void    DeleteHistory  (void) ;
    virtual void    AddCrowd       (void) ;
    virtual void    DeleteCrowd    (void) ;

    virtual void    PreparePeople  (QString name) ;
    virtual void    AppendPeople   (QString name) ;
    virtual void    UpdatePeople   (QString name) ;

  protected slots:

    virtual void    PeopleSex      (int index) ;
    virtual void    PeopleHairs    (int index) ;
    virtual void    PeopleEyes     (int index) ;
    virtual void    PeopleBlood    (int index) ;
    virtual void    PeopleHeight   (void) ;
    virtual void    PeopleWeight   (void) ;
    virtual void    PeopleBust     (void) ;
    virtual void    PeopleWaist    (void) ;
    virtual void    PeopleHip      (void) ;
    virtual void    NameUpdate     (QWidget * widget,SUID uuid) ;

  private slots:

  signals:

    // SubgroupPictures
    void SeePictures    (SUID object,SUID subgroup,QString name) ;
    void Constraints    (QString name,SUID uuid) ;
    void RuleTables     (QString name,SUID uuid) ;
    // ObjectGallery
    void Command        (nDeclWidget,QString name,SUID uuid) ;
    void ListSubgroups  (QString name,SUID uuid) ;
    // DocumentItems
    void Open           (SUID    document  ,
                         SUID    fileuuid  ,
                         int     extension ,
                         QString version   ,
                         QString title   ) ;
    void Processors     (UUIDs documents) ;
    void Edit           (SUID album,QString name) ;
    void UuidItems      (UUIDs & Uuids,RMAPs & Types,NAMEs & Names,QString title) ;
    // Bookmarks
    void Web               (QUrl url,bool reuse) ;
    void ImportRules       (QString name,SUID uuid,int type) ;
    void RunSets           (VarArgLists & Operations) ;

};

class Q_DOMINATION_EXPORT AlbumEditor : public StackedWidget
                                      , public Object
                                      , public GroupItems
                                      , public VideoManager
{
  Q_OBJECT
  public:

    explicit     AlbumEditor    (StandardConstructor) ;
    virtual     ~AlbumEditor    (void) ;

    virtual bool canStop        (void) ;

  protected:

    Ui :: AlbumEditor      * ui          ;
    N  :: PanWidget        * pan         ;
    N  :: ClipList         * clip        ;
    N  :: AlbumClip        * clips       ;
    N  :: PeopleView       * people      ;
    N  :: PictureView      * pictures    ;
    N  :: DocumentItems    * documents   ;
    N  :: Bookmarks        * bookmarks   ;
    N  :: PlainEditor      * plain       ;
    N  :: ComboBox         * groups      ;
    N  :: IdentifierEditor * identifiers ;
    N  :: TorrentLists     * torrents    ;
    QString                  OldName     ;

    virtual void Configure      (void) ;

    virtual void run            (int type,ThreadData * data) ;

    SUID         appendAlbum    (SqlConnection & SC,int language,QString name) ;
    bool         assureProduct  (SqlConnection & SC,int language,QString name,SUID u) ;

  private:

  public slots:

    virtual bool startup        (void) ;
    virtual void Load           (void) ;
    virtual bool Bootup         (void) ;

    virtual void NameChanged    (void) ;
    virtual void PrepareAlbum   (QString name) ;
    virtual void AppendAlbum    (QString name) ;
    virtual void UpdateAlbum    (QString name) ;

    virtual void FindAlbum      (void) ;
    virtual void AddGroup       (void) ;
    virtual void DeleteGroup    (void) ;
    virtual void AddActor       (void) ;
    virtual void AppendPeople   (void) ;

    virtual void Dig            (void) ;

  protected slots:

    virtual void groupClicked   (QTreeWidgetItem * item,int column) ;

    virtual void NewPeople      (void) ;
    virtual void NewActor       (void) ;
    virtual void NewGroup       (void) ;
    virtual void RemoveGroup    (void) ;
    virtual void setPlainText   (QString text) ;
    virtual void StartPeople    (void) ;
    virtual void StartPicture   (void) ;

    virtual void assignName     (QString name) ;
    virtual void setLanguage    (int language) ;
    virtual void assignId       (QString name) ;
    virtual void assignImage    (QImage * image) ;

    virtual void setEnabling    (bool enable) ;

  private slots:

  signals:

    void GoStart                (void) ;
    void GoPeople               (void) ;
    void GoPicture              (void) ;
    void EmitName               (QString name) ;
    void EmitLanguage           (int language) ;
    void EmitId                 (QString name) ;
    void EmitImage              (QImage * image) ;
    void setActors              (QString text) ;
    // SubgroupPictures
    void SeePictures            (SUID object,SUID subgroup,QString name) ;
    void Constraints            (QString name,SUID uuid) ;
    void RuleTables             (QString name,SUID uuid) ;
    // ObjectGallery
    void Command                (nDeclWidget,QString name,SUID uuid) ;
    void ListSubgroups          (QString name,SUID uuid) ;
    // DocumentItems
    void Open                   (SUID    document  ,
                                 SUID    fileuuid  ,
                                 int     extension ,
                                 QString version   ,
                                 QString title   ) ;
    void Processors             (UUIDs documents) ;
    void Edit                   (SUID album,QString name) ;
    void UuidItems              (UUIDs & Uuids,RMAPs & Types,NAMEs & Names,QString title) ;
    // ActorView
    void Gallery                (QWidget * widget,SUID people,QString name) ;
    void VideoAlbum             (QWidget * widget,SUID people,QString name) ;
    void PeopleNames            (QWidget * widget,SUID people,QString name) ;
    void PeopleDocuments        (QWidget * widget,SUID people,QString name) ;
    void Internet               (QWidget * widget,SUID people,QString name,int Id);
    void Translations           (QString title,UUIDs & Uuids) ;
    void Feature                (int Catalog,SUID uuid) ;
    void MergePeople            (UUIDs & Uuids) ;
    void PeopleSubgroups        (QString name,SUID uuid) ;
    // Bookmarks
    void Web                    (QUrl url,bool reuse) ;
    void ImportRules            (QString name,SUID uuid,int type) ;
    void RunSets                (VarArgLists & Operations) ;
    void assignEnabling         (bool enable) ;

};

class Q_DOMINATION_EXPORT OrganizationEditor : public StackedWidget
                                             , public Object
                                             , public GroupItems
                                             , public PictureManager
{
  Q_OBJECT
  public:

    explicit OrganizationEditor      (StandardConstructor) ;
    virtual ~OrganizationEditor      (void) ;

    virtual QSize sizeHint           (void) const ;

  protected:

    Ui::OrganizationEditor * ui        ;
    ItemNames              * names     ;
    ActorView              * people    ;
    ObjectGallery          * gallery   ;
    CommodityEdit          * commodity ;
    VideoContainer         * videos    ;
    DocumentItems          * documents ;
    Bookmarks              * bookmarks ;
    OrganizationProcessor  * processor ;
    PanWidget              * pan       ;
    ComboBox               * ocgroups  ;
    ComboBox               * octypes   ;

    virtual void Configure           (void) ;

    virtual OrganizationProcessor * NewProcessor (QWidget * widget) ;

    bool         AppendName          (SqlConnection & Connection,SUID uuid,QString name) ;

  private:

  public slots:

    virtual void startup             (void) ;
    virtual void Load                (void) ;
    virtual void NameChanged         (void) ;

    virtual void PrepareOrganization (QString name) ;
    virtual void AppendOrganization  (QString name) ;
    virtual void UpdateOrganization  (QString name) ;

    virtual void AddGroup            (void) ;
    virtual void DeleteGroup         (void) ;
    virtual void AddType             (void) ;
    virtual void DeleteType          (void) ;

  protected slots:

  private slots:

  signals:

    // SubgroupPictures
    void SeePictures       (SUID object,SUID subgroup,QString name) ;
    void Constraints       (QString name,SUID uuid) ;
    void RuleTables        (QString name,SUID uuid) ;
    // ObjectGallery
    void Command           (nDeclWidget,QString name,SUID uuid) ;
    void ListSubgroups     (QString name,SUID uuid) ;
    // DocumentItems
    void Open              (SUID    document  ,
                            SUID    fileuuid  ,
                            int     extension ,
                            QString version   ,
                            QString title   ) ;
    void Processors        (UUIDs documents) ;
    void Edit              (SUID album,QString name) ;
    void UuidItems         (UUIDs & Uuids,RMAPs & Types,NAMEs & Names,QString title) ;
    // ActorView
    void Gallery           (QWidget * widget,SUID people,QString name) ;
    void PotentialPictures (QWidget * widget,SUID people,QString name) ;
    void VideoAlbum        (QWidget * widget,SUID people,QString name) ;
    void PeopleNames       (QWidget * widget,SUID people,QString name) ;
    void PeopleDocuments   (QWidget * widget,SUID people,QString name) ;
    void Internet          (QWidget * widget,SUID people,QString name,int Id);
    void Translations      (QString title,UUIDs & Uuids) ;
    void Feature           (int Catalog,SUID uuid) ;
    void MergePeople       (UUIDs & Uuids) ;
    void PeopleSubgroups   (QString name,SUID uuid) ;
    // Bookmarks
    void Web               (QUrl url,bool reuse) ;
    void ImportRules       (QString name,SUID uuid,int type) ;
    void RunSets           (VarArgLists & Operations) ;

};

class Q_DOMINATION_EXPORT AudioConfigurator : public Splitter
{
  Q_OBJECT
  public:

    explicit AudioConfigurator (StandardConstructor) ;
    virtual ~AudioConfigurator (void) ;

    virtual QSize sizeHint     (void) const ;

  protected:

    Splitter        * bar       ;
    StackedWidget   * stack     ;
    QToolButton     * button    ;
    ComboBox        * item      ;
    PickAudio       * output    ;
    PickAudio       * input     ;
    DirectoryEditor * directory ;
    SUID              Notify    ;
    QMutex            mutex     ;

    virtual void Configure     (void) ;

  private:

  public slots:

    virtual void setClickable  (int click) ;
    virtual void setIcon       (bool enable,QIcon icon) ;
    virtual void setNotify     (SUID notify) ;
    virtual void setVisible    (bool visible,QString text) ;

  protected slots:

    virtual void Input         (int device) ;
    virtual void Output        (int device) ;

  private slots:

  signals:

} ;

class Q_DOMINATION_EXPORT CiosFloating : public Widget
{
  Q_OBJECT
  public:

    explicit CiosFloating      (StandardConstructor) ;
    virtual ~CiosFloating      (void) ;

    virtual bool & isFading    (void) ;

  protected:

    SUID                                uuid       ;
    int                                 type       ;
    int                                 columns    ;
    bool                                Fading     ;
    QSize                               iconSize   ;
    QString                             table      ;
    QString                             item       ;
    ToolWrapper                     *   wrapper    ;
    QList < QToolButton             * > buttons    ;
    QList < QAction                 * > actions    ;
    QMap  < QAction * , SUID          > CatalogMap ;
    QMap  < SUID      , QToolButton * > ButtonIDs  ;
    QMap  < SUID      , QAction     * > ActionIDs  ;

    virtual void Configure     (void) ;

  private:

  public slots:

    virtual void clear         (void) ;
    virtual void FadeOut       (void) ;
    virtual void Arrangement   (void) ;
    virtual void display       (void) ;
    virtual void disappear     (void) ;

    virtual void setColumns    (int column) ;
    virtual void setButtonSize (QSize s) ;
    virtual void setTable      (QString table) ;
    virtual void setItem       (QString itme) ;
    virtual void setWrapper    (ToolWrapper * wrapper) ;

    virtual void add           (SUID Id,QString text,QIcon icon) ;
    virtual void assign        (SUID uuid,int type,QRect Rect,int user) ;

  protected slots:

    void activate              (QAction * action) ;

  private slots:

  signals:

    void Execute               (SUID uuid,QString name) ;

} ;

class Q_DOMINATION_EXPORT CiosPanel : public Widget
{
  Q_OBJECT
  public:

    QMap < QString , QString> FriendIP        ;
    QMap < QString , int    > FriendPort      ;
    QMap < SUID    , QString> FriendUuid      ;
    QMap < QString , WMAPs  > FriendVariables ;
    QRect                     ScreenArea      ;
    QRect                     AvailArea       ;
    QRect                     DockArea        ;

    explicit CiosPanel         (StandardConstructor) ;
    virtual ~CiosPanel         (void) ;

  protected:

    QString                             table      ;
    QString                             panel      ;
    QString                             item       ;
    QSize                               iconSize   ;
    QDateTime                           Lastest    ;
    QList < QShortcut               * > Shortcuts  ;
    QMap  < int       , QToolButton * > Buttons    ;
    QMap  < SUID      , QToolButton * > Catalog    ;
    QList < QAction                 * > Actions    ;
    QMap  < QAction * , SUID          > CatalogMap ;
    QMap  < SUID      , QAction     * > MapCatalog ;
    QMap  < int       , SUID          > MapOrders  ;
    int                                 StartId    ;
    bool                                IdChanged  ;
    QTimer                          *   Timer      ;
    ToolWrapper                     *   wrapper    ;
    QList < CiosFloating            * > Floatings  ;

    virtual void resizeEvent   (QResizeEvent * e) ;
    virtual void showEvent     (QShowEvent   * e) ;

    virtual void Configure     (void) ;

  private:

    void add                   (SUID Id,QString text,QIcon icon) ;
    void catalog               (SUID Id,QString name) ;

  public slots:

    virtual void setTable      (QString table) ;
    virtual void setPanel      (QString table) ;
    virtual void setItem       (QString table) ;
    virtual void setButtonSize (QSize iconSize) ;
    virtual void setWrapper    (ToolWrapper * wrapper) ;

    virtual bool Load          (void) ;
    virtual void Initialize    (void) ;
    virtual void Evacuate      (void) ;
    virtual void Finish        (void) ;
    virtual void Left          (void) ;
    virtual void Right         (void) ;
    virtual void First         (void) ;
    virtual void End           (void) ;
    virtual void shutdown      (void) ;
    virtual void Appear        (void) ;
    virtual void Disappear     (void) ;
    virtual void Eradicate     (void) ;

    virtual void Execute       (SUID uuid,QString name) ;

  protected slots:

    virtual void Hidden        (void) ;
    virtual void Display       (void) ;
    virtual void IdUpdated     (void) ;

  private slots:

    void Finished              (void) ;
    void activate              (QAction * action) ;

  signals:

    void Quit                  (void) ;
    void Hide                  (void) ;
    void Manager               (void) ;
    void RemoteUser            (QString Host,QString IP,int Port) ;

} ;

class Q_DOMINATION_EXPORT LaunchPanel : public Widget
{
  Q_OBJECT
  public:

    QMap<QString,QString> FriendIP   ;
    QMap<QString,int    > FriendPort ;
    QMap<SUID   ,QString> FriendUuid ;
    QStackedWidget      * stack      ;
    QSplitter           * splitter   ;
    QRect                 ScreenArea ;
    QRect                 AvailArea  ;
    QRect                 DockArea   ;

    explicit LaunchPanel    (StandardConstructor) ;
    virtual ~LaunchPanel    (void);

  protected:

    QList<QShortcut       *> Shortcuts  ;
    QMap<int ,QToolButton *> Buttons    ;
    QMap<SUID,QToolButton *> Catalog    ;
    QList<QAction         *> Actions    ;
    QMap<QAction *,SUID    > CatalogMap ;
    QMap<SUID,QAction     *> MapCatalog ;
    QMap<int,SUID>           MapOrders  ;
    int                      StartId    ;
    bool                     IdChanged  ;
    QTimer                *  Timer      ;
    QDateTime                Lastest    ;
    QList<FloatingPanel   *> Floatings  ;

    void resizeEvent        (QResizeEvent * event);

  private:

    void add                (SUID Id,QString text,QIcon icon);
    void catalog            (SUID Id,QString name);

  public slots:

    bool Load               (void);
    void Initialize         (void);
    void Evacuate           (void);
    void Finish             (void);
    void Left               (void);
    void Right              (void);
    void First              (void);
    void End                (void);
    void shutdown           (void);

    void Execute            (SUID uuid,QString name) ;

  protected slots:

    void Display            (void);
    void IdUpdated          (void);

  private slots:

    void Finished           (void) ;
    void MoveStack          (void) ;
    void activate           (QAction * action);

  signals:

    void Quit               (void);
    void Hide               (void);
    void Manager            (void);
    void RemoteUser         (QString Host,QString IP,int Port) ;

} ;

class Q_DOMINATION_EXPORT SearchPanel : public StackedWidget
{
  Q_OBJECT
  public:

    explicit SearchPanel     (StandardConstructor) ;
    virtual ~SearchPanel     (void) ;

    virtual QSize sizeHint   (void) const ;

  protected:

    QDir                 js       ;
    ToolWrapper        * wrapper  ;
    QFileSystemWatcher * watcher  ;
    QSplitter          * splitter ;
    ComboBox           * engines  ;
    QLineEdit          * line     ;
    EMAPs                Scripts  ;
    bool                 loading  ;
    QStringList          strings  ;

    virtual void Configure   (void) ;

  private:

    void         LoadScripts (void) ;

  public slots:

    virtual void setWrapper  (ToolWrapper * w) ;
    virtual void Appear      (void) ;
    virtual void Disappear   (void) ;
    virtual void Eradicate   (void) ;

    virtual void ObtainFocus (void) ;
    virtual void setPath     (QDir & dir) ;

  protected slots:

    virtual void GoSearch    (void) ;
    virtual void changed     (const QString & files) ;

  private slots:

  signals:

};

class Q_DOMINATION_EXPORT CommonMain : public QMainWindow
                                     , public VirtualGui
                                     , public Thread
{
  Q_OBJECT
  public:

    explicit CommonMain (int argc,char ** argv,QWidget * parent) ;
    virtual ~CommonMain (void) ;

  protected:

    ScriptPlan                                MainPlan        ;
    QSystemTrayIcon                        *  Tray            ;
    StackedWidget                          *  Stacked         ;
    MdiArea                                *  Mdi             ;
    DebugView                              *  Debug           ;
    int                                       AttachMode      ;
    bool                                      Ready           ;
    Qt::WindowFlags                           owFlags         ;
    QList <HunterPlugin                    *> hunters         ;
    QList <QAction                         *> hunterActions   ;
    QMap  <QAction        *,HunterPlugin   *> hunterPlugins   ;
    QMap  <HunterPlugin   *,QPluginLoader  *> hunterLoaders   ;
    QList <PicturePlugin                   *> pictureFilters  ;
    QList <QAction                         *> pictureActions  ;
    QMap  <QAction        *,PicturePlugin  *> picturePlugins  ;
    QMap  <PicturePlugin  *,QPluginLoader  *> pictureLoaders  ;
    QList <DocumentPlugin                  *> documentFilters ;
    QList <QAction                         *> documentActions ;
    QMap  <QAction        *,DocumentPlugin *> documentPlugins ;
    QMap  <DocumentPlugin *,QPluginLoader  *> documentLoaders ;
    QList <SqlPlugin                       *> SqlPlugins      ;
    QList <QAction                         *> sqlActions      ;
    QMap  <QAction        *,SqlPlugin      *> sqlPlugins      ;
    QMap  <SqlPlugin      *,QPluginLoader  *> SqlLoaders      ;
    NAMEs                                     scriptID        ;
    QMap  <SUID,QAction                    *> scriptActions   ;
    WebDockBar                             *  DockBar         ;
    ScrollDock                             *  CliDock         ;
    QList <Classifiers                     *> classifiers     ;
    QMap  <int             ,CpuLabel       *> CPUs            ;
    QSlider                                *  InputVolume     ;
    QSlider                                *  OutputVolume    ;
    TorrentWidget                          *  torrentWidget   ;

    virtual void      focusInEvent     (QFocusEvent  * event) ;
    virtual void      focusOutEvent    (QFocusEvent  * event) ;
    virtual void      resizeEvent      (QResizeEvent * event) ;
    virtual void      showEvent        (QShowEvent   * event) ;
    virtual void      closeEvent       (QCloseEvent  * event) ;

    virtual void      InstallDecisions (void) ;
    virtual void      InstallFonts     (double defaultSize = 0.40) ;
    virtual bool      InstallIcons     (int style) ;
    virtual void      InstallDirs      (void) ;
    virtual void      InstallLocal     (void) ;
    virtual void      InstallExtras    (bool progress) ;
    virtual bool      DeployPlan       (int     Language    ,
                                        QString Username    ,
                                        QString Settings    ,
                                        double  defaultSize ,
                                        bool    plugin      ,
                                        bool    progress  ) ;
    virtual QWidget * Container        (void) ;
    void              connectDock      (QWidget * widget) ;
    void              deleteLater      (QWidget * widget) ;

    virtual QList<QMenu *> MainMenus   (void) ;
            QList<QMenu *> SubMenus    (QObject * parent) ;

    virtual QString    AcquireAudio    (void) ;
    virtual Junction * MountChannel    (int MID) ;

    virtual bool       BindBeacon      (int port) ;

    virtual void ThreadEvent           (void) ;

    virtual bool Bustle                (void) ;
    virtual bool Vacancy               (void) ;

  private:

  public slots:

    virtual bool startup               (void) = 0 ;
    virtual int  abort                 (void) = 0 ;
    virtual bool Shutdown              (void) = 0 ;

    virtual bool prepare               (void) ;
    virtual bool configure             (void) ;
    virtual bool loading               (void) ;
    virtual bool brand                 (void) ;
    virtual bool advertising           (void) ;
    virtual bool primary               (void) ;

    virtual void Quit                  (void) ;
    virtual void ForceClose            (void) ;
    virtual void MountTray             (void) ;
    virtual void InstallStackMdi       (void) ;

    virtual void setVisibility         (bool visibility) ;
    virtual void obtainFocus           (bool focus) ;
    virtual void Parking               (QWidget * widget,int alignment = 0) ;

    virtual void DockDebugger          (void) ;
    virtual void MountProgress         (void) ;
    virtual void StopTorrents          (void) ;
    virtual void DockTorrent           (QAction * action) ;
    virtual void DockCLI               (void) ;
    virtual void GoScript              (SUID uuid) ;

    virtual void Open                  (QFileInfo FileInformation) ;
    virtual void acceptFiles           (const QList<QUrl> & urls) ;
    virtual void dropInPicture         (QFileInfo & file) ;
    virtual void UncompressFile        (QFileInfo & file) ;
    virtual void UnpackFile            (QFileInfo & file) ;
    virtual void OpenPlainText         (QFileInfo & file) ;
    virtual void OpenPlainText         (QString title,QByteArray & Explain,int arrangement = 0) ;
    virtual void FoundDocuments        (QString name,UUIDs & Uuids) ;
    virtual void KeywordMatched        (QString title,NAMEs & Keywords) ;
    virtual void SqlQuery              (QString query) ;
    virtual void ListUrls              (QString name,UUIDs & Uuids) ;
    virtual void RunSets               (VarArgLists & Operations) ;

    virtual int  RequestInput          (QString device,int rate,int bits,int channels,int format) ;
    virtual int  RequestChannel        (QString filename) ;
    virtual void ChannelCeased         (void) ;

    virtual void setupVolumes          (void) ;
    virtual void ChangeInputVolume     (bool enabled) ;
    virtual void ChangeOutputVolume    (bool enabled) ;

  protected slots:

    virtual void TrayClick             (QSystemTrayIcon::ActivationReason reason);
    virtual void TrayMenu              (void) ;
    virtual void TrayUnknown           (void) ;
    virtual void TrayDoubleClicked     (void) ;
    virtual void TrayTrigger           (void) ;
    virtual void TrayMiddleClicked     (void) ;

    virtual void attachDock            (QWidget * widget,QString title,Qt::DockWidgetArea area,Qt::DockWidgetAreas areas) ;
    virtual void attachMdi             (QWidget * widget,int Direction) ;
    virtual void ApplySql              (QWidget * widget,Sql * sql) ;

    virtual void AttachActions         (void) ;
    virtual void Maximized             (void) ;
    virtual void Minimized             (void) ;
    virtual void FullScreen            (void) ;
    virtual void NormalWindow          (void) ;

    virtual void DockTray              (void) ;
    virtual void Engross               (bool shown) ;
    virtual void DisplayTitle          (bool shown) ;
    virtual void DisplayMenu           (bool shown) ;
    virtual void DisplayStatus         (bool shown) ;

    virtual void SubwindowView         (void) ;
    virtual void TabbedView            (void) ;
    virtual void CascadeWindows        (void) ;
    virtual void TileWindows           (void) ;
    virtual void ChildChanged          (void) ;

    virtual void HelpDragDrop          (void) ;
    virtual void setCpu                (void) ;
    virtual void stopCpu               (void) ;

    virtual void MachineUsage          (bool shown) ;
    virtual void ProgramUsage          (bool shown) ;

    virtual void VoiceOutput           (bool enabled) ;
    virtual void ClearVoices           (void) ;

    void         CameraInput           (QMenu * menu) ;
    void         CameraDevice          (QAction * device) ;
    virtual void CameraChanged         (void) ;

    void         AudioOutput           (QMenu * menu) ;
    void         OutputDevice          (QAction * device) ;
    virtual void OutputChanged         (void) ;

    void         AudioInput            (QMenu * menu) ;
    void         InputDevice           (QAction * device) ;
    virtual void InputChanged          (void) ;

    virtual void VolumeInputChanged    (int value) ;
    virtual void VolumeOutputChanged   (int value) ;

    virtual void DismantleHunters      (void) ;
    virtual void LoadHunters           (QMenu * menu,QString catalog) ;
    virtual void HuntersMenu           (QMenu * menu,QString catalog) ;
    virtual void acceptHunter          (bool checked) ;

    virtual void DismantlePictures     (void) ;
    virtual void LoadPictures          (QMenu * menu,QString catalog) ;
    virtual void PicturesMenu          (QMenu * menu,QString catalog) ;
    virtual void acceptPictures        (bool checked) ;

    virtual void DismantleDocuments    (void) ;
    virtual void LoadDocuments         (QMenu * menu,QString catalog,int type) ;
    virtual void DocumentsMenu         (QMenu * menu,QString catalog,int type) ;
    virtual void acceptDocuments       (bool checked) ;

    virtual void DismantleSQLs         (void) ;
    virtual void LoadSQLs              (QMenu * menu) ;
    virtual void SQLsMenu              (QMenu * menu) ;
    virtual void acceptSQL             (bool checked) ;

    void LoadScripts                   (int type) ;
    void MountScripts                  (QMenu * menu) ;
    void ScriptActivated               (bool) ;

    void MountWebDock                  (QString section) ;

    TagsEditor * NewTags               (int ObjectType,QString scope,QString title,QString header,bool checkable = true) ;

    virtual void MountClassifiers      (QDir PathDir) ;
    virtual void Finished              (Classifiers * classifier) ;

    virtual void NeighborsChanged      (void) ;

    virtual void Visit                 (QString hostname) ;
    virtual void Away                  (QString hostname) ;

    virtual void setWaitCursor         (void) ;
    virtual void setArrowCursor        (void) ;

  private slots:

  signals:

    void assignWaitCursor              (void) ;
    void assignArrowCursor             (void) ;
} ;

class Q_DOMINATION_EXPORT DesktopMain : public CommonMain
{
  Q_OBJECT
  public:

    typedef enum               {
      Configured      = 100001 ,
      NoAccess        = 100002 ,
      Loading         = 100003 ,
      Loaded          = 100004 ,
      Started         = 100005 ,
      HasConfigurator = 100101 ,
      HasStacked      = 100102 ,
      HasMdi          = 100103 ,
      HasDebugger     = 100104 ,
      AcceptMenu      = 100105 ,
      StackedGui      = 100201 ,
      MdiGui          = 100202 ,
      Visibility      = 100301 ,
      Interactive     = 100302 ,
      AtBusy          = 100303 }
      OperateStates            ;

    typedef enum               {
      Preparation     = 200001 ,
      Failure         = 200002 ,
      Configuration   = 200003 ,
      Warmup          = 200004 ,
      Primary         = 200005 }
      OperateActions           ;

    enum                       {
      DeviceNICs      = 100    ,
      DeviceCPUs      = 101  } ;

    explicit        DesktopMain         (int argc,char ** argv,QWidget * parent) ;
    virtual        ~DesktopMain         (void) ;

    virtual bool    hasConfigurator     (void) = 0 ;

  protected:

    QString          CapturePath  ;
    ScreenCapturer * capturer     ;
    QtPCAP         * pcap         ;

    virtual void    InstallDecisions    (void) ;
    virtual void    InstallMetaTypes    (void) ;
    virtual bool    DefaultBootup       (void) ;

    PictureView   * CreatePictures      (QString name,SUID    group) ;
    PictureView   * CreatePictures      (QString name,UUIDs & Uuids) ;
    ObjectGallery * CreateObjectGallery (QString name,SUID uuid,int type) ;
    VcfView       * OpenPicture         (SUID uuid) ;

    virtual void    run                 (int Type,ThreadData * data) ;

    QString         UpTimeToString      (qint64 t) ;

  private:

  public slots:

    virtual bool    startup             (void) ;
    virtual int     abort               (void) ;
    virtual void    setVisibility       (bool visibility) ;

    virtual void    EditScripts         (void) ;
    virtual void    Translations        (QString title,UUIDs & Uuids) ;
    virtual void    ListItems           (UUIDs & Uuids   ,
                                         RMAPs & Types   ,
                                         NAMEs & Names   ,
                                         QString title ) ;

    void CaptureScreen                  (void) ;
    void CaptureThis                    (void) ;
    void CaptureDesktop                 (void) ;
    void SaveCapture                    (QPixmap & pixmap,QString path) ;
    void CaptureSeconds                 (QWidget * widget,QDir path,int afterMs,int TotalMs) ;

    virtual void Stitch                 (QString name,SUID uuid,UUIDs & Uuids) ;
    virtual void Stitching              (VarArgs & arguments) ;

    virtual void ViewPicture            (QImage image) ;

    virtual void CloseAll               (void) ;
    virtual void ClipboardContents      (void) ;
    virtual void SimpleNote             (void) ;
    virtual void ClearClipboard         (void) ;

    virtual void ShowInputDevices       (void) ;
    virtual void ShowOutputDevices      (void) ;
    virtual void ShowCameras            (void) ;

    virtual void ShowCPUs               (void) ;
    virtual void ShowCPU                (QMenu * menu) ;

    virtual void CheckCUDA              (QMenu * cuda) ;
    virtual void CheckDisplays          (QMenu * display) ;
    virtual void InstallGL              (QMenu * gl) ;

    virtual void SniffTcp               (void) ;
    virtual void SniffTcp               (QString device) ;
    virtual void ShowNICs               (void) ;
    virtual void InstallPCAP            (QMenu * menu) ;

    virtual void RelayCommand           (QString method,int port) ;
    virtual void BeaconCommand          (QString method,int port) ;

    virtual void FullNames              (SUID uuid) ;
    virtual void CompleteNames          (QString name,SUID uuid) ;

  protected slots:

    virtual void DockPad                (QString scope) ;
    virtual void TrayMessage            (QString title,QString message) ;

    virtual void AttachDevices          (void) ;

    virtual void WwwBrowser             (QUrl url,bool reuse = true) ;
    virtual void AnalysisPage           (WebBrowser * view) ;

    virtual void addEditTool            (void) ;

  private slots:

    void         WhenBusy               (void) ;
    void         OnRelax                (void) ;

  signals:

    void         OnBusy                 (void) ;
    void         GoRelax                (void) ;
    void         WatchPicture           (QImage image) ;

};

class Q_DOMINATION_EXPORT MobileMain : public CommonMain
{
  Q_OBJECT
  public:

  typedef enum               {
    Configured      = 100001 ,
    NoAccess        = 100002 ,
    Loading         = 100003 ,
    Loaded          = 100004 ,
    Started         = 100005 ,
    HasConfigurator = 100101 ,
    HasStacked      = 100102 ,
    HasMdi          = 100103 ,
    HasDebugger     = 100104 ,
    AcceptMenu      = 100105 ,
    StackedGui      = 100201 ,
    MdiGui          = 100202 ,
    Visibility      = 100301 ,
    Interactive     = 100302 ,
    Leaving         = 100401 ,
    ShutOff         = 100402 ,
    Storing         = 100403 ,
    Closing         = 100404 ,}
    OperateStates            ;

  typedef enum               {
    Preparation     = 200001 ,
    Failure         = 200002 ,
    Configuration   = 200003 ,
    Warmup          = 200004 ,
    Brand           = 200005 ,
    Advertising     = 200006 ,
    Primary         = 200007 }
    OperateActions           ;

    explicit MobileMain (int argc,char ** argv,QWidget * parent) ;
    virtual ~MobileMain (void) ;

  protected:

    virtual void InstallDirs      (void) ;
    virtual void InstallDecisions (void) ;
    virtual void InstallSounds    (void) ;
    virtual bool DefaultBootup    (void) ;
    virtual bool DefaultEnding    (void) ;

    QString      MachinePath      (QString path) ;

  private:

  public slots:

    virtual bool startup          (void) ;
    virtual void setVisibility    (bool visibility) ;

    virtual bool TurnOff          (void) ;
    virtual bool Deposit          (void) ;

  protected slots:

    virtual void Nextstep         (void) ;

  private slots:

  signals:

};

}

QT_END_NAMESPACE

#endif
