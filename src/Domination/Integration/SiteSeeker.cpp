#include <domination.h>

N::SiteSeeker:: SiteSeeker      ( QObject * parent , Plan * p )
              : QObject         (           parent            )
              , LearnSite       (                           p )
              , GroupItems      (                           p )
              , PictureManager  (                           p )
              , PeopleManager   (                           p )
              , VideoManager    (                           p )
              , HistoryManager  (                           p )
              , FinanceManager  (                           p )
              , plan            (                           p )
              , Seeker          ( NULL                        )
              , decompressing   ( false                       )
{
  Translations [ MsgParsing ] = tr("Parsing %1"     )                         ;
  Translations [ MsgUpdate  ] = tr("Update %1"      )                         ;
  Translations [ MsgFetch   ] = tr("Fetch %1 header")                         ;
  Translations [ MsgIMS     ] = tr("Import %1 pages")                         ;
  Translations [ MsgIMD     ] = tr("Import URL : <%1>")                       ;
  Translations [ MsgIMB     ] = tr("Import %1 bytes into %2")                 ;
  Translations [ MsgMMB     ] = tr("Import multimedia file %1 bytes into %2") ;
  Translations [ MsgHNH     ] = tr("[%1] has no header")                      ;
  Translations [ MsgRCI     ] = tr("[%1] return code is %2")                  ;
  Translations [ MsgURM     ] = tr("URI is <%1>")                             ;
  Translations [ MsgAMT     ] = tr("Alter mimetype to %1")                    ;
  Translations [ MsgDLC     ] = tr("Downloading %1")                          ;
  Translations [ MsgDLF     ] = tr("Download %1 failure")                     ;
  Translations [ MsgMSF     ] = tr("%1 failure")                              ;
  Translations [ MsgAPU     ] = tr("Attach picture with url")                 ;
  Translations [ MsgDBG     ] = tr("Picture connected to %1")                 ;
  Translations [ MsgIPM     ] = tr("Importing picture"      )                 ;
  Translations [ MsgIPC     ] = tr("Importing picture <%1>" )                 ;
  Translations [ MsgCIF     ] = tr("Check image format")                      ;
  Translations [ MsgWAG     ] = tr("Web agent : <%1>")                        ;
  Translations [ MsgRDL     ] = tr("Redirect %1 to %2")                       ;
  Translations [ MsgImage   ] = tr("Image file: %1")                          ;
  Translations [ MsgSuffix  ] = tr("Suffix: %1")                              ;
  /////////////////////////////////////////////////////////////////////////////
  AutoMap = true                                                              ;
  /////////////////////////////////////////////////////////////////////////////
  nConnect ( this , SIGNAL ( Ungzip     (QString,QStringList))                ,
             this , SLOT   ( Uncompress (QString,QStringList))              ) ;
}

N::SiteSeeker::~SiteSeeker (void)
{
}

void N::SiteSeeker::showMessage(QString message)
{
  if ( IsNull(Seeker) ) return      ;
  Seeker -> showMessage ( message ) ;
  Time :: skip ( 50 )               ;
}

void N::SiteSeeker::LookUrl(QString url)
{
  if ( IsNull(Seeker) ) return ;
  Seeker -> LookUrl ( url )    ;
}

void N::SiteSeeker::setSeeker(CommonSeeker * s)
{
  Seeker = s ;
}

QString N::SiteSeeker::Temporary(QString filename)
{
  if ( IsNull(Seeker) ) return filename   ;
  return Seeker -> Temporary ( filename ) ;
}

QString N::SiteSeeker::GZipLocation(void)
{
  return "" ;
}

QString N::SiteSeeker::Proxy(void)
{
  if ( IsNull(Seeker) ) return ""   ;
  return Seeker -> ProxyAddress ( ) ;
}

int N::SiteSeeker::Port(void)
{
  if ( IsNull(Seeker) ) return 0 ;
  return Seeker -> ProxyPort ( ) ;
}

int N::SiteSeeker::DownloadTime(void)
{
  if ( IsNull(Seeker) ) return 480 ;
  return Seeker->DownloadTime()    ;
}

int N::SiteSeeker::ConnectTimeout(void)
{
  if ( IsNull(Seeker) ) return 90 ;
  return Seeker->ConnectTime()    ;
}

bool N::SiteSeeker::TryReferer(void)
{
  return true ;
}

bool N::SiteSeeker::EraseJava(QByteArray & XML)
{
  int start = XML.indexOf("<script"        )      ;
  if ( start < 0   ) return true                  ;
  int end   = XML.indexOf("</script>",start)      ;
  if ( end   < 0   ) return true                  ;
  end += 9                                        ;
  if ( start > end ) return true                  ;
  /////////////////////////////////////////////////
  unsigned char * p = (unsigned char *)XML.data() ;
  for (int i = start ; i < end ; i++ )            {
    p [ i ] = ' '                                 ;
  }                                               ;
  /////////////////////////////////////////////////
  return EraseJava ( XML )                        ;
}

bool N::SiteSeeker::Obtain  (
       SqlConnection & SC   ,
       QUrl            url  ,
       SUID          & uuid ,
       QByteArray    & XML  )
{
  urlLock . lock   ( )                                                         ;
  uuid = assurePage ( SC , url )                                               ;
  urlLock . unlock ( )                                                         ;
  if ( uuid <= 0 )                                                             {
    showMessage ( QString("Drop %1 : %2").arg(uuid).arg(url.toString())      ) ;
    return false                                                               ;
  }                                                                            ;
  showMessage ( QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") ) ;
  showMessage ( QString("%1 : %2").arg(uuid).arg(url.toString())             ) ;
  Download    ( SC , uuid , url                                              ) ;
  showMessage ( QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") ) ;
  //////////////////////////////////////////////////////////////////////////////
  QString Q                                                                    ;
  XML.clear()                                                                  ;
  urlLock . lock   ( )                                                         ;
  Q = SC.sql.SelectFrom                                                        (
        "body"                                                                 ,
        PlanTable(ImportBodies)                                                ,
        SC . WhereUuid   ( uuid )                                              ,
        SC . OrderByDesc ( "id" )                                              ,
        SC . sql.Limit   ( 0,1  )                                            ) ;
  if ( SC.Fetch(Q)     ) XML = SC . ByteArray ( 0 )                            ;
  if ( XML.size() <= 0 ) GetXml ( SC , uuid , XML )                            ;
  urlLock . unlock ( )                                                         ;
  //////////////////////////////////////////////////////////////////////////////
  return ( XML.size() > 0 )                                                    ;
}

SUID N::SiteSeeker::UrlPicture(SqlConnection & SC,SUID xuid)
{
  QString Q                             ;
  QString T                             ;
  SUID    zuid = 0                      ;
  if ( xuid <= 0 ) return 0             ;
  GroupItems GI ( plan )                ;
  GI . AutoMap = true                   ;
  T = GI . LookTable                    (
        Types  :: Picture               ,
        Types  :: URL                   ,
        Groups :: Subordination       ) ;
  Q = SC.sql.SelectFrom                 (
        "first"                         ,
        T                               ,
        SecondItem                      (
          xuid                          ,
          Types  :: Picture             ,
          Types  :: URL                 ,
          Groups :: Subordination   ) ) ;
  if (SC.Fetch(Q)) zuid = SC.Uuid ( 0 ) ;
  return zuid                           ;
}

bool N::SiteSeeker::hadCached  (
       SqlConnection & SC   ,
       QUrl            url  ,
       SUID          & puid ,
       QByteArray    & XML  )
{
  QString Q                      ;
  urlLock . lock   ( )           ;
  puid = assurePage ( SC , url ) ;
  urlLock . unlock ( )           ;
  if ( puid <= 0 )               {
    showMessage ( tr("No cahce : %1").arg(url.toString()) ) ;
    return false                 ;
  }                              ;
  Q = SC . sql . SelectFrom      (
        "body"                   ,
        PlanTable(ImportBodies)  ,
        SC.WhereUuid(puid)       ,
        SC.OrderByDesc("id")     ,
        SC.sql.Limit(0,1)      ) ;
  if (SC.Fetch(Q))               {
    XML = SC . ByteArray ( 0 )   ;
  }                              ;
  if (XML.size()>0) return true  ;
  Q = SC . sql . SelectFrom      (
        "xml"                    ,
        PlanTable(XmlDepot)      ,
        SC.WhereUuid(puid)       ,
        SC.OrderByDesc("id")     ,
        SC.sql.Limit(0,1)      ) ;
  if (SC.Fetch(Q))               {
    XML = SC . ByteArray ( 0 )   ;
  }                              ;
  return ( XML . size ( ) > 0 )  ;
}

void N::SiteSeeker::Uncompress(QString exe,QStringList paraments)
{
  QStringList X                    ;
  X  << exe                        ;
  X  << paraments                  ;
  showMessage ( X . join ( " " ) ) ;
  QProcess mp ( this )             ;
  mp . start ( exe , paraments   ) ;
  mp . waitForFinished (         ) ;
  decompressing = false            ;
}

void N::SiteSeeker::Execute(QString exe,QStringList paraments)
{
  decompressing = true            ;
  emit Ungzip ( exe , paraments ) ;
  while ( decompressing )         {
    plan -> processEvents ( )     ;
  }                               ;
}

void N::SiteSeeker::Convert(SqlConnection & SC,QByteArray & XML,SUID puid)
{
  if (XML.at(0)!=(char)0x1F) return                         ;
  if (XML.at(1)!=(char)0x8B) return                         ;
  QString     CMD = "C:/Windows/System32/cmd.exe"           ;
  QString     BAT = Temporary(QString("%1.bat" ).arg(puid)) ;
  QString     EXE = GZipLocation ( )                        ;
  QString     xxx = Temporary(QString("%1.gz"  ).arg(puid)) ;
  QString     hhh = Temporary(QString("%1.html").arg(puid)) ;
  QStringList GGG                                           ;
  QByteArray  ZML                                           ;
  QString     BCT                                           ;
  if (EXE.length()<=0) return                               ;
  N::File::toFile(xxx,XML)                                  ;
  xxx = xxx.replace('/','\\')                               ;
  hhh = hhh.replace('/','\\')                               ;
  EXE = EXE.replace('/','\\')                               ;
  BAT = BAT.replace('/','\\')                               ;
  GGG << EXE                                                ;
  GGG << "-dc"                                              ;
  GGG << xxx                                                ;
  GGG << ">"                                                ;
  GGG << hhh                                                ;
  BCT  = GGG.join(" ")                                      ;
  BCT += "\r\n"                                             ;
  BCT += "exit"                                             ;
  ZML  = BCT.toUtf8()                                       ;
  N::File::toFile(BAT,ZML)                                  ;
  ZML  . clear ( )                                          ;
  GGG  . clear ( )                                          ;
  GGG << "/k"                                               ;
  GGG << BAT                                                ;
  CMD  = CMD.replace('/','\\')                              ;
  ///////////////////////////////////////////////////////////
  Execute(CMD,GGG)                                          ;
  ///////////////////////////////////////////////////////////
  N::File::toByteArray(hhh,ZML)                             ;
  QFile::remove(xxx)                                        ;
  if (ZML.size()>0) QFile::remove(hhh)                      ;
  QFile::remove(BAT)                                        ;
  ///////////////////////////////////////////////////////////
  assignBody ( SC , puid , ZML )                            ;
  XML = ZML                                                 ;
}

bool N::SiteSeeker::AttachPeople (
       SqlConnection & SC        ,
       QUrl            url       ,
       SUID            people    ,
       QString         name      ,
       SUID            gallery   )
{
  QString    Q                                                                 ;
  GroupItems GI ( plan )                                                       ;
  SUID       wuid = gallery                                                    ;
  SUID       xuid = 0                                                          ;
  SUID       zuid = 0                                                          ;
  urlLock . lock   ( )                                                         ;
  xuid = assurePage ( SC , url  )                                              ;
  if ( xuid > 0 ) zuid = UrlPicture ( SC , xuid )                              ;
  urlLock . unlock ( )                                                         ;
  GI . AutoMap = true                                                          ;
  //////////////////////////////////////////////////////////////////////////////
  if ( ( xuid > 0 ) && ( zuid <= 0 ) )                                         {
    showMessage ( nTimeNow.toString("yyyy/MM/dd hh:mm:ss")                   ) ;
    showMessage ( tr("Download %1").arg(url.toString())                      ) ;
    Download    ( SC , xuid , url                                            ) ;
    showMessage ( nTimeNow.toString("yyyy/MM/dd hh:mm:ss")                   ) ;
    zuid = UrlPicture ( SC , xuid )                                            ;
  }                                                                            ;
  //////////////////////////////////////////////////////////////////////////////
  if (zuid>0)                                                                  {
    showMessage ( tr("Attach picture %1 to people %2").arg(zuid).arg(people) ) ;
    ////////////////////////////////////////////////////////////////////////////
    GI . GroupTable = GI . LookTable                                           (
                        Types  :: People                                       ,
                        Types  :: Picture                                      ,
                        Groups :: Subordination                              ) ;
    ////////////////////////////////////////////////////////////////////////////
    GI . InsertItem ( SC                                                       ,
                 people                                                        ,
                 zuid                                                          ,
                 Types  :: People                                              ,
                 Types  :: Picture                                             ,
                 Groups :: Subordination                                     ) ;
    ////////////////////////////////////////////////////////////////////////////
    if (wuid>0)                                                                {
      GI . GroupTable = GI . LookTable                                         (
                          Types  :: Gallery                                    ,
                          Types  :: Picture                                    ,
                          Groups :: Subordination                            ) ;
      GI . InsertItem ( SC                                                     ,
                   wuid                                                        ,
                   zuid                                                        ,
                   Types  :: Gallery                                           ,
                   Types  :: Picture                                           ,
                   Groups :: Subordination                                   ) ;
    }                                                                          ;
    ////////////////////////////////////////////////////////////////////////////
    GI . GroupTable = GI . LookTable                                           (
                        Types  :: People                                       ,
                        Types  :: Picture                                      ,
                        Groups :: Icon                                       ) ;
    Q  = SC.sql.SelectFrom                                                     (
          "second"                                                             ,
          GI . GroupTable                                                      ,
          FirstItem(people,Types::People,Types::Picture,Groups::Icon)        ) ;
    if (SC.Fetch(Q))                                                           {
      showMessage ( tr("%1 icon already exists").arg(name)                   ) ;
    } else                                                                     {
      showMessage ( tr("assign %1 icon into %2").arg(name).arg(zuid)         ) ;
      //////////////////////////////////////////////////////////////////////////
      GI . InsertItem  ( SC                                                    ,
                    people                                                     ,
                    zuid                                                       ,
                    Types  :: People                                           ,
                    Types  :: Picture                                          ,
                    Groups :: Icon                                           ) ;
    }                                                                          ;
  }                                                                            ;
  //////////////////////////////////////////////////////////////////////////////
  if (xuid>0)                                                                  {
    showMessage ( tr("Attach url %1 to people %2").arg(xuid).arg(people) )     ;
    GI . GroupTable = GI . LookTable                                           (
                        Types  :: People                                       ,
                        Types  :: URL                                          ,
                        Groups :: Subordination                              ) ;
    GI . InsertItem  ( SC                                                      ,
                  people                                                       ,
                  xuid                                                         ,
                  Types  :: People                                             ,
                  Types  :: URL                                                ,
                  Groups :: Subordination                                    ) ;
  }                                                                            ;
  //////////////////////////////////////////////////////////////////////////////
  return ( ( xuid > 0 ) && ( zuid > 0 ) )                                      ;
}

bool N::SiteSeeker::AppendNames(SqlConnection & SC,SUID uuid,int vLanguage,QString name)
{
  SUID nuid                                                                        ;
  nuid = SC   . Unique        ( PlanTable(MajorUuid) , "uuid" , 123391           ) ;
  SC          . assureUuid    ( PlanTable(MajorUuid) , nuid   , Types::Name      ) ;
  SC          . assureName    ( PlanTable(Names    ) , nuid   , vLanguage , name ) ;
  GroupItems :: InsertNameMap ( SC , uuid , nuid                                 ) ;
  return true                                                                      ;
}

bool N::SiteSeeker::MergePeople (
       SqlConnection & SC       ,
       SUID            merge    ,
       SUID            puid     ,
       QStringList     names    ,
       int             language )
{
  if (names.count()<=0) return false                                           ;
  QString     f = names[0]                                                     ;
  QString     n                                                                ;
  QStringList Same                                                             ;
  UUIDs       Merge                                                            ;
  foreach (n,names)                                                            {
    SUID u = FindPeople(SC,n)                                                  ;
    if ( u > 0 )                                                               {
      if ( ( u != puid ) && ( ! Merge . contains ( u ) ) ) Merge << u          ;
    } else Same << n                                                           ;
  }                                                                            ;
  //////////////////////////////////////////////////////////////////////////////
  foreach ( n , Same ) if ( n != f ) AppendNames ( SC , puid , language , n )  ;
  if (Same.count()>1)                                                          {
    showMessage ( tr("[ %1 ] are the same person").arg(Same.join(" ] / [ ")) ) ;
  }                                                                            ;
  //////////////////////////////////////////////////////////////////////////////
  if ( Merge . count ( ) <= 0 ) return true                                    ;
  if (!Merge.contains(puid))                                                   {
    Merge << puid                                                              ;
  }                                                                            ;
  if (Merge.count()<=1) return true                                            ;
  GroupItems GI ( plan )                                                       ;
  GI . AutoMap    = true                                                       ;
  GI . GroupTable = GI . LookTable                                             (
                      Types  :: Division                                       ,
                      Types  :: People                                         ,
                      Groups :: Subordination                                ) ;
  GI . Join                                                                    (
    SC                                                                         ,
    merge                                                                      ,
    Types  :: Division                                                         ,
    Types  :: People                                                           ,
    Groups :: Subordination                                                    ,
    0                                                                          ,
    Merge                                                                      ,
    false                                                                    ) ;
  showMessage ( tr("%1 has duplicated records").arg(f) )                       ;
  //////////////////////////////////////////////////////////////////////////////
  return true                                                                  ;
}

bool N::SiteSeeker::Between (
       RangeInt   & range   ,
       QString      start   ,
       QString      finish  ,
       QByteArray & XML     )
{
  if ( start  . length ( ) <=0 ) return false      ;
  if ( finish . length ( ) <=0 ) return false      ;
  //////////////////////////////////////////////////
  int index                                        ;
  index = XML . indexOf ( start.toUtf8() , range . L ) ;
  if ( index <  0 ) return false                   ;
  //////////////////////////////////////////////////
  index += start.length()                          ;
  if ( index <= 0 ) return false                   ;
  //////////////////////////////////////////////////
  int endix = XML . indexOf ( finish.toUtf8() , index + 1 ) ;
  if ( endix <= 0 ) return false                   ;
  //////////////////////////////////////////////////
  range . L = index                                ;
  range . R = endix                                ;
  return true                                      ;
}

QString N::SiteSeeker::Between(RangeInt & range,QByteArray & XML)
{
  QByteArray S = XML . mid   ( range . L , range . V ( ) ) ;
  return QString :: fromUtf8 ( S                         ) ;
}

bool N::SiteSeeker::NamesByOrder (
       SqlConnection & SC        ,
       SUID            nuid      ,
       NAMEs         & names     )
{
  QString Q                       ;
  QString n                       ;
  UUIDs   U                       ;
  SUID    u                       ;
  /////////////////////////////////
  Q = SC . sql . SelectFrom       (
        "name"                    ,
        PlanTable(NameMaps)       ,
        SC.WhereUuid(nuid)        ,
        SC.OrderByAsc("prefer") ) ;
  SqlLoopNow ( SC , Q )           ;
    U << SC . Uuid ( 0 )          ;
  SqlLoopErr ( SC , Q )           ;
  SqlLoopEnd ( SC , Q )           ;
  /////////////////////////////////
  for (int i=0;i<U.count();i++)   {
    u = U [ i ]                   ;
    Q = SC.sql.SelectFrom         (
          "name"                  ,
          PlanTable(Names)        ,
          SC.WhereUuid(u)         ,
          SC.OrderByDesc("id")    ,
          SC.sql.Limit(0,1)     ) ;
    if (SC.Fetch(Q))              {
      n = SC . String ( 0 )       ;
      names [ i ] = n             ;
    }                             ;
  }                               ;
  return names . count ( )        ;
}

QString N::SiteSeeker::PurgeSpace(QString encoded)
{
  QString s = encoded                                                      ;
  while (s.length()>0 && s.at(0           )==' ') s.remove(0           ,1) ;
  while (s.length()>0 && s.at(s.length()-1)==' ') s.remove(s.length()-1,1) ;
  return s                                                                 ;
}

QString N::SiteSeeker::FixHtml(QString encoded)
{
  if (!encoded.contains("&#")) return encoded             ;
  int index = 0                                           ;
  int start = 0                                           ;
  QString result                                          ;
  do                                                      {
    index = encoded.indexOf("&#",index)                   ;
    if (index>=0)                                         {
      int endix = encoded.indexOf(";",index+1)            ;
      if (endix<0) index = index + 1 ; else               {
        QString m = encoded.mid(index,endix-index)        ;
        QString l = encoded.mid(start,index-start)        ;
        result += l                                       ;
        m.remove(0,2)                                     ;
        if (m.at(0)=='x' || m.at(0)=='X')                 {
          bool okay = true                                ;
          int         v                                   ;
          m.remove(0,1)                                   ;
          v = m.toInt(&okay,16)                           ;
          if (okay)                                       {
            result . append (QChar(v))                    ;
          }                                               ;
        } else                                            {
          bool okay = true                                ;
          int         v                                   ;
          v = m.toInt(&okay,10)                           ;
          if (okay)                                       {
            result . append (QChar(v))                    ;
          }                                               ;
        }                                                 ;
        start = endix + 1                                 ;
        index = endix + 1                                 ;
        if (start>=encoded.length()) index = -1           ;
      }                                                   ;
    } else                                                {
      result += encoded.mid(start,encoded.length()-start) ;
    }                                                     ;
  } while (index>0)                                       ;
  return result                                           ;
}

QString N::SiteSeeker::FixProduct(QString product)
{
  if (product.contains("-")) return product     ;
  int p = -1                                    ;
  for (int i=0;p<0 && i<product.length();i++)   {
    if (product.at(i).isNumber()) p = i         ;
  }                                             ;
  if (p<0) return product                       ;
  QString T = product.left (p                 ) ;
  QString N = product.right(product.length()-p) ;
  for (int i=0;i<N.length();i++)                {
    if (!N.at(i).isNumber()) return product     ;
  }                                             ;
  if (T.length()<=0) return product             ;
  product = PurgeSpace(T) + "-" + PurgeSpace(N) ;
  return product                                ;
}

bool N::SiteSeeker::DvdPublish(QString date,QDateTime & Time)
{
  QString     S = date           ;
  QStringList L                  ;
  S = S.replace("/"," ")         ;
  S = S.replace("-"," ")         ;
  L = S.split  (" "    )         ;
  if (L.count()<3) return false  ;
  int Y = 0                      ;
  int M = 0                      ;
  int D = 0                      ;
  if (L[0].toInt()>1000)         {
    Y = L[0].toInt()             ;
    M = L[1].toInt()             ;
    D = L[2].toInt()             ;
  } else
  if (L[2].toInt()>1000)         {
    Y = L[2].toInt()             ;
    M = L[0].toInt()             ;
    D = L[1].toInt()             ;
  } else return false            ;
  QDate X(Y,M,D)                 ;
  if (!X.isValid()) return false ;
  QTime T(21,0,0)                ;
  Time . setDate ( X )           ;
  Time . setTime ( T )           ;
  return Time.isValid()          ;
}

QStringList N::SiteSeeker::FindOutNames(QString name)
{
  bool        comma    = true                 ;
  QString     pt       = name                 ;
  QString     splitter = ","                  ;
  QStringList ps                              ;
  QStringList Actresses                       ;
  pt = PurgeSpace(pt)                         ;
  pt = pt.replace("/",",")                    ;
  if (!pt.contains(","))                      {
    comma = false                             ;
    for (int i=0;!comma && i<pt.length();i++) {
      if (pt.at(i)==' ')                      {
      } else
      if (pt.at(i).isLetterOrNumber())        {
        comma = true                          ;
      }                                       ;
    }                                         ;
  }                                           ;
  if (!comma) splitter = " "                  ;
  ps = pt.split(splitter)                     ;
  foreach (pt,ps)                             {
    pt = PurgeSpace(pt)                       ;
    if (pt.length()>0) Actresses << pt        ;
  }                                           ;
  return Actresses                            ;
}

SUID N::SiteSeeker::MapToCompany(QString ID)
{
  SUID u                               ;
  foreach (u,Matchings)                {
    QString S = MatchingScripts[u]     ;
    if (Scripts::StringMatching(S,ID)) {
      return ToCompanies [ u ]         ;
    }                                  ;
  }                                    ;
  return 0                             ;
}

bool N::SiteSeeker::LoadCompanyProducts(SqlConnection & SC)
{
  ////////////////////////////////////
  QString Q                          ;
  SUID    u                          ;
  ////////////////////////////////////
  Matchings       . clear ( )        ;
  Companies       . clear ( )        ;
  ToCompanies     . clear ( )        ;
  MatchingScripts . clear ( )        ;
  CompanyNames    . clear ( )        ;
  ////////////////////////////////////
  Q = SC.sql.SelectFrom              (
        "script,company"             ,
        "dvdmatching"                ,
        "order by id asc"          ) ;
  SqlLoopNow ( SC , Q )              ;
    SUID SCRIPT  = SC.Uuid(0)        ;
    SUID COMPANY = SC.Uuid(1)        ;
    Matchings << SCRIPT              ;
    Companies << COMPANY             ;
    ToCompanies [ SCRIPT ] = COMPANY ;
  SqlLoopErr ( SC , Q )              ;
  SqlLoopEnd ( SC , Q )              ;
  ////////////////////////////////////
  foreach (u,Matchings)              {
    Q = SC.sql.SelectFrom            (
          "script"                   ,
          PlanTable(Scripts)         ,
          SC.WhereUuid(u)          ) ;
    if (SC.Fetch(Q))                 {
      QString XX = SC.String(0)      ;
      MatchingScripts [ u ] = XX     ;
    }                                ;
  }                                  ;
  ////////////////////////////////////
  foreach (u,Companies)              {
    QStringList L                    ;
    L = GroupItems :: Names ( SC,u ) ;
    if (L.count()>0)                 {
      CompanyNames[u] = L[0]         ;
    }                                ;
  }                                  ;
  ////////////////////////////////////
  return true                        ;
}

SUID N::SiteSeeker::LookForVideo(N::SqlConnection & SC,SUID url,QString identifier)
{
  QString Q                                      ;
  SUID    u = 0                                  ;
  SUID    c = 0                                  ;
  GroupItems GI ( plan )                         ;
  GI . AutoMap    = true                         ;
  GI . GroupTable = GI . LookTable               (
                      Types  :: Album            ,
                      Types  :: URL              ,
                      Groups :: Subordination  ) ;
  Q = SC.sql.SelectFrom                          (
        "first"                                  ,
        GI . GroupTable                          ,
        SecondItem ( url                         ,
                     Types  :: Album             ,
                     Types  :: URL               ,
                     Groups :: Subordination ) ) ;
  if (SC.Fetch(Q)) u = SC . Uuid ( 0 )           ;
  if (u>0) return u                              ;
  c = FindIdentifier(SC,identifier)              ;
  if (c<=0) return 0                             ;
  GI . GroupTable = GI . LookTable               (
                      Types  :: Commodity        ,
                      Types  :: Album            ,
                      Groups :: Subordination  ) ;
  Q = SC.sql.SelectFrom                          (
        "second"                                 ,
        GI . GroupTable                          ,
        FirstItem  ( c                           ,
                     Types  :: Commodity         ,
                     Types  :: Album             ,
                     Groups :: Subordination )   ,
        SC.OrderByDesc("position")               ,
        SC.sql.Limit(0,1)                      ) ;
  if (SC.Fetch(Q)) u = SC . Uuid ( 0 )           ;
  return u                                       ;
}

SUID N::SiteSeeker::assureProduct(N::SqlConnection & SC,QString title,QString product,UUIDs & peoples)
{
  nKickOut (  title.length() <= 0   , 0 )                    ;
  nKickOut ( !product.contains("-") , 0 )                    ;
  nKickOut (  product.contains(" ") , 0 )                    ;
  GroupItems GI ( plan )                                     ;
  GI . AutoMap    = true                                     ;
  SUID tuid  = 0                                             ;
  bool found = false                                         ;
  SUID iuid  = FindIdentifier(SC,product)                    ;
  if (iuid>0)                                                {
    found = true                                             ;
    tuid  = iuid                                             ;
  }                                                          ;
  if (!found)                                                {
    SUID uuid = appendCommodity(SC)                          ;
    appendNamemap(SC,uuid,2726,title)                        ;
    assureIdentifier(SC,uuid,product)                        ;
    if (peoples.count()>0)                                   {
      GI . GroupTable = GI . LookTable                       (
                          Types  :: Commodity                ,
                          Types  :: People                   ,
                          Groups :: Subordination          ) ;
      GI . Join                                              (
        SC                                                   ,
        uuid                                                 ,
        N::Types::Commodity                                  ,
        N::Types::People                                     ,
        N::Groups::Subordination                             ,
        0,peoples,false                                    ) ;
    }                                                        ;
    tuid = uuid                                              ;
  } else                                                     {
    if (peoples.count()>0)                                   {
      GI . GroupTable = GI . LookTable                       (
                          Types  :: Commodity                ,
                          Types  :: People                   ,
                          Groups :: Subordination          ) ;
      GI . Join                                              (
        SC                                                   ,
        tuid                                                 ,
        N::Types::Commodity                                  ,
        N::Types::People                                     ,
        N::Groups::Subordination                             ,
        0,peoples,false                                    ) ;
    }                                                        ;
  }                                                          ;
  return tuid                                                ;
}

SUID N::SiteSeeker::assureDocument(N::SqlConnection & SC,QString title,QString content)
{
  QDateTime XT = QDateTime::currentDateTime()      ;
  QString   version                                ;
  SUID      duid = appendDocument  (SC)            ;
  SUID      puid = acquirePlainText(SC)            ;
  version = XT.toString("yyyy/MM/dd hh:mm:ss.zzz") ;
  SC.assureName                                    (
    PlanTable(Names)                               ,
    duid                                           ,
    plan->LanguageId                               ,
    title                                        ) ;
  savePlainText(SC,puid,content)                   ;
  JoinDocument(SC,duid,puid,737,version)           ;
  return duid                                      ;
}

SUID N::SiteSeeker::assureVideo(N::SqlConnection & SC,QString title)
{
  int position = 0                                    ;
  QString Q                                           ;
  SUID uuid = SC.Unique(PlanTable(MajorUuid),"uuid")  ;
  SC.assureUuid                                       (
    PlanTable(MajorUuid)                              ,
    uuid                                              ,
    N::Types::Album                                 ) ;
  SC.assureUuid                                       (
    PlanTable(Divisions)                              ,
    uuid                                              ,
    N::Types::Album                                 ) ;
  SC.assureName                                       (
    PlanTable(Names)                                  ,
    uuid                                              ,
    plan->LanguageId                                  ,
    title                                           ) ;
  /////////////////////////////////////////////////////
  Q = SC.sql.SelectFrom                               (
        "position"                                    ,
        PlanTable(Divisions)                          ,
        QString("where `type` = %1 "
                "order by `position` desc "
                "limit 0,1"                           )
        .arg(N::Types::Album                      ) ) ;
  if (SC.Fetch(Q)) position = SC.Int(0)               ;
  position++                                          ;
  /////////////////////////////////////////////////////
  Q = SC.sql.Update                                   (
        PlanTable(Divisions)                          ,
        "where uuid = :UUID"                          ,
        1                                             ,
        "position"                                  ) ;
  SC . Prepare ( Q                                  ) ;
  SC . Bind    ( "uuid"     , uuid                  ) ;
  SC . Bind    ( "position" , position              ) ;
  SC . Exec    (                                    ) ;
  /////////////////////////////////////////////////////
  return uuid                                         ;
}

bool N::SiteSeeker::JoinAlbum(N::SqlConnection & SC,SUID group,SUID album)
{
  GroupItems GI ( plan )                      ;
  GI . AutoMap    = true                      ;
  GI . GroupTable = GI . LookTable            (
                    Types  :: Division        ,
                    Types  :: Album           ,
                    Groups :: Subordination ) ;
  UUIDs Juids                                 ;
  Juids << album                              ;
  GI . Join                                   (
    SC                                        ,
    group                                     ,
    Types  :: Division                        ,
    Types  :: Album                           ,
    Groups :: Subordination                   ,
    0                                         ,
    Juids                                     ,
    false                                   ) ;
  return true                                 ;
}

bool N::SiteSeeker::AttachVideo (
       SqlConnection & SC       ,
       QUrl            url      ,
       QUrl            source   ,
       SUID            video    ,
       SUID            gallery  ,
       UUIDs         & peoples  )
{
  urlLock . lock   ( )                                                               ;
  QString Q                                                                          ;
  SUID wuid = gallery                                                                ;
  SUID xuid = assurePage ( SC , url    )                                             ;
  SUID srcd = PageUuid   ( SC , source )                                             ;
  SUID zuid = UrlPicture ( SC , xuid   )                                             ;
  urlLock . unlock ( )                                                               ;
  GroupItems GI ( plan )                                                             ;
  GI . AutoMap    = true                                                             ;
  ////////////////////////////////////////////////////////////////////////////////////
  assureParent ( SC , xuid , srcd )                                                  ;
  if (zuid<=0)                                                                       {
    showMessage ( QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") )     ;
    showMessage ( tr("Download %1").arg(url.toString())                        )     ;
    Download    ( SC , xuid , url                                              )     ;
    showMessage ( QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") )     ;
    zuid = UrlPicture ( SC , xuid   )                                                ;
  }                                                                                  ;
  ////////////////////////////////////////////////////////////////////////////////////
  if (zuid>0)                                                                        {
    showMessage  ( tr("Attach picture %1 to video %2").arg(zuid).arg(video) )        ;
    GI . GroupTable = GI . LookTable                                                 (
                        Types  :: Album                                              ,
                        Types  :: Picture                                            ,
                        Groups :: Subordination                                    ) ;
    GI . InsertItem   ( SC                                                           ,
                   video                                                             ,
                   zuid                                                              ,
                   Types  :: Album                                                   ,
                   Types  :: Picture                                                 ,
                   Groups :: Subordination                                         ) ;
    if (wuid>0)                                                                      {
      GI . GroupTable = GI . LookTable                                               (
                          Types  :: Gallery                                          ,
                          Types  :: Picture                                          ,
                          Groups :: Subordination                                  ) ;
      GI . InsertItem ( SC                                                           ,
                   wuid                                                              ,
                   zuid                                                              ,
                   Types  :: Gallery                                                 ,
                   Types  :: Picture                                                 ,
                   Groups :: Subordination                                         ) ;
    }                                                                                ;
    //////////////////////////////////////////////////////////////////////////////////
    GI . GroupTable = GI . LookTable                                                 (
                        Types  :: Album                                              ,
                        Types  :: Picture                                            ,
                        Groups :: Icon                                             ) ;
    Q  = SC.sql.SelectFrom                                                           (
          "second"                                                                   ,
          GI . GroupTable                                                            ,
          FirstItem ( video                                                          ,
                      Types  :: Album                                                ,
                      Types  :: Picture                                              ,
                      Groups :: Icon                                             ) ) ;
    if (SC.Fetch(Q))                                                                 {
      showMessage  ( tr("%1 icon already exists").arg(video)                       ) ;
    } else                                                                           {
      showMessage  ( tr("assign %1 icon into %2").arg(zuid).arg(video)             ) ;
      GI . GroupTable = GI . LookTable                                               (
                          Types  :: Album                                            ,
                          Types  :: Picture                                          ,
                          Groups :: Icon                                           ) ;
      GI . InsertItem   ( SC                                                         ,
                     video                                                           ,
                     zuid                                                            ,
                     Types  :: Album                                                 ,
                     Types  :: Picture                                               ,
                     Groups :: Icon                                                ) ;
    }                                                                                ;
    //////////////////////////////////////////////////////////////////////////////////
    SUID pzid                                                                        ;
    foreach (pzid,peoples)                                                           {
      showMessage  ( tr("Attach picture %1 to people %2").arg(zuid).arg(pzid) )      ;
      GI . GroupTable = GI . LookTable                                               (
                          Types  :: People                                           ,
                          Types  :: Picture                                          ,
                          Groups :: Subordination                                  ) ;
      GI . InsertItem   ( SC                                                         ,
                     pzid                                                            ,
                     zuid                                                            ,
                     Types  :: People                                                ,
                     Types  :: Picture                                               ,
                     Groups :: Subordination                                       ) ;
    }                                                                                ;
  }                                                                                  ;
  ////////////////////////////////////////////////////////////////////////////////////
  if (xuid>0)                                                                        {
    SUID pxid                                                                        ;
    GI . GroupTable = GI . LookTable                                                 (
                        Types  :: People                                             ,
                        Types  :: URL                                                ,
                        Groups :: Subordination                                    ) ;
    foreach (pxid,peoples)                                                           {
      showMessage  ( tr("Attach url %1 to people %2").arg(xuid).arg(pxid)          ) ;
      GI . InsertItem ( SC                                                           ,
                     pxid                                                            ,
                     xuid                                                            ,
                     Types  :: People                                                ,
                     Types  :: URL                                                   ,
                     Groups :: Subordination                                       ) ;
    }                                                                                ;
  }                                                                                  ;
  return true                                                                        ;
}

bool N::SiteSeeker::AttachCompany   (
       N::SqlConnection & SC        ,
       QString            Product   ,
       SUID               ProductId ,
       SUID               MakerId   ,
       QString            MakerName ,
       SUID               video     ,
       UUIDs            & peoples   )
{
  SUID    cuid = 0                              ;
  UUIDs   PUIDs                                 ;
  cuid = MapToCompany ( Product )               ;
  if ( cuid <= 0 ) cuid = MakerId               ;
  if ( cuid <= 0 ) return false                 ;
  ///////////////////////////////////////////////
  QString CID                                   ;
  if (CompanyNames.contains(cuid))              {
    CID = CompanyNames[cuid]                    ;
  } else CID = MakerName                        ;
  CID = tr("Video album joins `%1`").arg(CID)   ;
  showMessage ( CID )                           ;
  ///////////////////////////////////////////////
  GroupItems GI ( plan )                        ;
  GI . AutoMap    = true                        ;
  PUIDs  . clear ( )                            ;
  PUIDs << ProductId                            ;
  GI . GroupTable = GI . LookTable              (
                      Types  :: Organization    ,
                      Types  :: Commodity       ,
                      Groups :: Subordination ) ;
  GI . Join                                     (
    SC                                          ,
    cuid                                        ,
    Types  :: Organization                      ,
    Types  :: Commodity                         ,
    Groups :: Subordination                     ,
    0                                           ,
    PUIDs                                       ,
    false                                     ) ;
  ///////////////////////////////////////////////
  PUIDs  . clear ( )                            ;
  PUIDs << video                                ;
  GI . GroupTable = GI . LookTable              (
                      Types  :: Organization    ,
                      Types  :: Album           ,
                      Groups :: Subordination ) ;
  GI . Join                                     (
    SC                                          ,
    cuid                                        ,
    Types  :: Organization                      ,
    Types  :: Album                             ,
    Groups :: Subordination                     ,
    0                                           ,
    PUIDs                                       ,
    false                                     ) ;
  ///////////////////////////////////////////////
  PUIDs  . clear ( )                            ;
  PUIDs << peoples                              ;
  GI . GroupTable = GI . LookTable              (
                      Types  :: Organization    ,
                      Types  :: People          ,
                      Groups :: Subordination ) ;
  GI . Join                                     (
    SC                                          ,
    cuid                                        ,
    Types  :: Organization                      ,
    Types  :: People                            ,
    Groups :: Subordination                     ,
    0                                           ,
    PUIDs                                       ,
    false                                     ) ;
  return true                                   ;
}

SUID N::SiteSeeker::FindCompany(SqlConnection & SC,QString name)
{
  QString Q                                  ;
  UUIDs   U                                  ;
  UUIDs   R                                  ;
  SUID    u                                  ;
  ////////////////////////////////////////////
  Q = SC.sql.SelectFrom                      (
        "uuid"                               ,
        PlanTable(Names)                     ,
        SC.sql.Where(1,"name")          )    ;
  SC . Prepare ( Q                      )    ;
  SC . Bind    ( "name" , name.toUtf8() )    ;
  if (SC.Exec()) while (SC.Next())           {
    U << SC . Uuid ( 0 )                     ;
  }                                          ;
  if (U.count()<=0) return 0                 ;
  ////////////////////////////////////////////
  foreach ( u , U )                          {
    Q = SC.sql.SelectFrom                    (
          "type"                             ,
          PlanTable(MajorUuid)               ,
          SC.WhereUuid(u)                  ) ;
    if (SC.Fetch(Q))                         {
      int t = SC.Int(0)                      ;
      if (t==Types::Name) R << u             ;
    }                                        ;
  }                                          ;
  if (R.count()<=0) return 0                 ;
  ////////////////////////////////////////////
  U . clear ( )                              ;
  foreach ( u , R )                          {
    Q = SC.sql.SelectFrom                    (
          "uuid"                             ,
          PlanTable(NameMaps)                ,
          QString("where name = %1").arg(u)) ;
    SqlLoopNow ( SC , Q )                    ;
      U << SC . Uuid ( 0 )                   ;
    SqlLoopErr ( SC , Q )                    ;
    SqlLoopEnd ( SC , Q )                    ;
  }                                          ;
  if (U.count()<=0) return 0                 ;
  ////////////////////////////////////////////
  R . clear ( )                              ;
  foreach ( u , U )                          {
    Q = SC.sql.SelectFrom                    (
          "type"                             ,
          PlanTable(MajorUuid)               ,
          SC.WhereUuid(u)                  ) ;
    if (SC.Fetch(Q))                         {
      int t = SC.Int(0)                      ;
      if (t==Types::Organization) R << u     ;
    }                                        ;
  }                                          ;
  if (R.count()<=0) return 0                 ;
  ////////////////////////////////////////////
  return R.last()                            ;
}

bool N::SiteSeeker::AttachOrganization(SqlConnection & SC,QUrl url,SUID company)
{
  QString Q                                                                           ;
  urlLock . lock   ( )                                                                ;
  SUID xuid = assurePage ( SC , url  )                                                ;
  SUID zuid = UrlPicture ( SC , xuid )                                                ;
  urlLock . unlock ( )                                                                ;
  GroupItems GI ( plan )                                                              ;
  GI . AutoMap    = true                                                              ;
  if (zuid<=0)                                                                        {
    showMessage ( QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") )      ;
    showMessage ( tr("Download %1").arg(url.toString())                        )      ;
    Download    ( SC , xuid , url                                              )      ;
    showMessage ( QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") )      ;
    zuid = UrlPicture ( SC , xuid )                                                   ;
  }                                                                                   ;
  if (zuid>0)                                                                         {
    showMessage ( tr("Attach picture %1 to organization %2").arg(zuid).arg(company) ) ;
    GI . GroupTable = GI . LookTable                                                  (
                        Types  :: Organization                                        ,
                        Types  :: Picture                                             ,
                        Groups :: Subordination                                     ) ;
    GI . InsertItem  ( SC                                                             ,
                  company                                                             ,
                  zuid                                                                ,
                  Types  :: Organization                                              ,
                  Types  :: Picture                                                   ,
                  Groups :: Subordination                                           ) ;
    GI . GroupTable = GI . LookTable                                                  (
                        Types  :: Organization                                        ,
                        Types  :: Picture                                             ,
                        Groups :: Icon                                              ) ;
    Q  = SC.sql.SelectFrom                                                            (
          "second"                                                                    ,
          GI . GroupTable                                                             ,
          FirstItem ( company                                                         ,
                      Types::Organization                                             ,
                      Types::Picture                                                  ,
                      Groups::Icon                                                ) ) ;
    if (SC.Fetch(Q))                                                                  {
      showMessage ( tr("%1 icon already exists").arg(company)                       ) ;
    } else                                                                            {
      showMessage ( tr("assign %1 icon into %2").arg(zuid).arg(company)             ) ;
      GI . GroupTable = GI . LookTable                                                (
                          Types  :: Organization                                      ,
                          Types  :: Picture                                           ,
                          Groups :: Icon                                            ) ;
      GI . InsertItem  ( SC                                                           ,
                    company                                                           ,
                    zuid                                                              ,
                    Types  :: Organization                                            ,
                    Types  :: Picture                                                 ,
                    Groups :: Icon                                                  ) ;
    }                                                                                 ;
  }                                                                                   ;
  return true                                                                         ;
}

bool N::SiteSeeker::assureEnglish(SqlConnection & SC,SUID people,QStringList English)
{
  QStringList PeopleNames                                            ;
  QString     ns                                                     ;
  QString     rs                                                     ;
  QString     Q                                                      ;
  PeopleNames = N :: PeopleManager :: Names(SC,people)               ;
  ns = English[1] + " " + English[0]                                 ;
  rs = English[0] + " " + English[1]                                 ;
  showMessage ( tr("%1 has english name [%2]").arg(people).arg(ns) ) ;
  if (PeopleNames.contains(rs))                                      {
    UUIDs U                                                          ;
    SUID  u                                                          ;
    SUID  nuid = 0                                                   ;
    U = SC.Uuids                                                     (
          PlanTable(NameMaps)                                        ,
          "name"                                                     ,
          QString ( "%1 %2"                                          )
          .arg( SC . WhereUuid  ( people   )                         )
          .arg( SC . OrderByAsc ( "prefer" )                     ) ) ;
    for (int i=0;nuid==0 && i<U.count();i++)                         {
      u = U[i]                                                       ;
      Q = SC.sql.SelectFrom                                          (
            "name"                                                   ,
            PlanTable(Names)                                         ,
            SC.WhereUuid(u)                                        ) ;
      if (SC.Fetch(Q))                                               {
        QString n = SC.String(0)                                     ;
        if (n==rs) nuid = u                                          ;
      }                                                              ;
    }                                                                ;
    //////////////////////////////////////////////////////////////////
    if (nuid>0)                                                      {
      QByteArray N = ns.toUtf8()                                     ;
      int        L = N.size()                                        ;
      Q = QString  ( "update `%1` set `length` = :LENGTH , `name` = :NAME where `uuid` = :UUID ;" ).arg(PlanTable(Names)) ;
      SC . Prepare ( Q               )                               ;
      SC . Bind    ( "uuid"   , nuid )                               ;
      SC . Bind    ( "length" , L    )                               ;
      SC . Bind    ( "name"   , N    )                               ;
      SC . Exec    (                 )                               ;
      DeleteFrom   ( SC                                              ,
                     SecondItem ( nuid                               ,
                                  Types  :: Surname                  ,
                                  Types  :: Name                     ,
                                  Groups :: Subordination        ) ) ;
      showMessage ( tr("Alter [%1] into [%2] for [%3]").arg(rs).arg(ns).arg(people) ) ;
    }                                                                ;
  } else
  if (!PeopleNames.contains(ns))                                     {
    AppendNames ( SC , people , 1819 , ns                          ) ;
    showMessage ( tr("Append [%1] into [%2]").arg(ns).arg(people)  ) ;
  }                                                                  ;
  return true                                                        ;
}

void N::SiteSeeker::CommodityJoinAlbum (
       SqlConnection & SC              ,
       SUID            Product         ,
       SUID            Album           )
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
    Types  :: Commodity            ,
    Types  :: Album                ,
    Groups :: Subordination      ) ;
  UUIDs DUIDs                      ;
  DUIDs << Album                   ;
  GI . Join                        (
    SC                             ,
    Product                        ,
    Types  :: Commodity            ,
    Types  :: Album                ,
    Groups :: Subordination        ,
    0                              ,
    DUIDs                        ) ;
}

void N::SiteSeeker::CommodityJoinPeople (
       SqlConnection & SC               ,
       SUID            Product          ,
       UUIDs         & people           )
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
    Types  :: Commodity            ,
    Types  :: People               ,
    Groups :: Subordination      ) ;
  GI . Join                        (
    SC                             ,
    Product                        ,
    Types  :: Commodity            ,
    Types  :: People               ,
    Groups :: Subordination        ,
    0                              ,
    people                       ) ;
}

void N::SiteSeeker::AlbumJoinPeople (
       SqlConnection & SC           ,
       SUID            Album        ,
       UUIDs         & peoples      )
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
    Types  :: Album                ,
    Types  :: People               ,
    Groups :: Subordination      ) ;
  GI . Join                        (
    SC                             ,
    Album                          ,
    Types  :: Album                ,
    Types  :: People               ,
    Groups :: Subordination        ,
    0                              ,
    peoples                      ) ;
}

void N::SiteSeeker::AlbumJoinPictures (
       SqlConnection & SC             ,
       SUID            Album          ,
       UUIDs         & picture        )
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
    Types  :: Album                ,
    Types  :: Picture              ,
    Groups :: Subordination      ) ;
  GI . Join                        (
    SC                             ,
    Album                          ,
    Types  :: Album                ,
    Types  :: Picture              ,
    Groups :: Subordination        ,
    0                              ,
    picture                      ) ;
}

void N::SiteSeeker::AlbumJoinURLs (
       SqlConnection & SC         ,
       SUID            Album      ,
       UUIDs         & URLs       )
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
    Types  :: Album                ,
    Types  :: URL                  ,
    Groups :: Subordination      ) ;
  GI . Join                        (
    SC                             ,
    Album                          ,
    Types  :: Album                ,
    Types  :: URL                  ,
    Groups :: Subordination        ,
    0                              ,
    URLs                         ) ;
}

void N::SiteSeeker::GalleryJoinPictures (
       SqlConnection & SC               ,
       SUID            gallery          ,
       UUIDs         & picture          )
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
    Types  :: Gallery              ,
    Types  :: Picture              ,
    Groups :: Subordination      ) ;
  GI . Join                        (
    SC                             ,
    gallery                        ,
    Types  :: Gallery              ,
    Types  :: Picture              ,
    Groups :: Subordination        ,
    0                              ,
    picture                      ) ;
}

void N::SiteSeeker::PeopleJoinPictures (
       SqlConnection & SC              ,
       SUID            people          ,
       UUIDs         & picture         )
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
    Types  :: People               ,
    Types  :: Picture              ,
    Groups :: Subordination      ) ;
  GI . Join                        (
    SC                             ,
    people                         ,
    Types  :: People               ,
    Types  :: Picture              ,
    Groups :: Subordination        ,
    0                              ,
    picture                      ) ;
}

void N::SiteSeeker::PeopleJoinURLs (
       SqlConnection & SC          ,
       SUID            people      ,
       UUIDs         & URLs        )
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI . LookTable (
    Types  :: People               ,
    Types  :: URL                  ,
    Groups :: Subordination      ) ;
  GI . Join                        (
    SC                             ,
    people                         ,
    Types  :: People               ,
    Types  :: URL                  ,
    Groups :: Subordination        ,
    0                              ,
    URLs                         ) ;
}
