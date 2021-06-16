#include <domination.h>
#include "ui_nSearchTool.h"

N::SearchTool:: SearchTool ( QWidget * parent , Plan * p )
              : TabWidget  (           parent ,        p )
              , AttachDock (                           p )
              , ui         ( new Ui::nSearchTool         )
              , Mode       ( 0                           )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::SearchTool::~SearchTool(void)
{
  delete ui;
}

void N::SearchTool::resizeEvent(QResizeEvent * event)
{
  QTabWidget :: resizeEvent ( event ) ;
  Relocation                (       ) ;
}

void N::SearchTool::showEvent(QShowEvent * event)
{
  QTabWidget :: showEvent ( event ) ;
  Relocation              (       ) ;
}

void N::SearchTool::closeEvent(QCloseEvent * event)
{
  QTabWidget::closeEvent(event) ;
}

void N::SearchTool::contextMenuEvent(QContextMenuEvent * event)
{
  if (Menu(event->pos())) event->accept( ) ;
  else QTabWidget::contextMenuEvent(event) ;
}

void N::SearchTool::Docking(QMainWindow       * Main  ,
                            QString             title ,
                            Qt::DockWidgetArea  area  ,
                            Qt::DockWidgetAreas areas )
{
  AttachDock::Docking(Main,this,title,area,areas) ;
  QObject::connect                                (
    Dock,SIGNAL(visibilityChanged(bool))          ,
    this,SLOT  (Visible          (bool))        ) ;
}

void N::SearchTool::Visible(bool visible)
{
  Visiblity(visible) ;
}

void N::SearchTool::DockIn(bool shown)
{
  Show(shown);
}

void N::SearchTool::Configure(void)
{
  QFont f = plan->fonts[Fonts::TreeView]            ;
  f . setPixelSize    ( f.pixelSize() - 4 )         ;
  PictureCompare=new PictureComparsion(this,plan)   ;
  ui       -> setupUi ( this )                      ;
  addTab(PictureCompare,tr("Picture comparsion"))   ;
  plan     -> setFont ( this )                      ;
  tabBar() -> setFont ( f    )                      ;
  ///////////////////////////////////////////////////
  MathOps << "="                                    ;
  MathOps << "!="                                   ;
  MathOps << ">"                                    ;
  MathOps << ">="                                   ;
  MathOps << "<"                                    ;
  MathOps << "<="                                   ;
  ///////////////////////////////////////////////////
  ui->pwOp    ->addItems(MathOps)                   ;
  ui->phOp    ->addItems(MathOps)                   ;
  ui->PictureC->addItems(MathOps)                   ;
  ///////////////////////////////////////////////////
  ui->PictureItems->blockSignals ( true  )          ;
  ui->PictureItems->addItem(tr("Picture size"  ),1) ;
  ui->PictureItems->addItem(tr("Picture colors"),2) ;
  ui->PictureItems->blockSignals ( false )          ;
  ///////////////////////////////////////////////////
  ui->PictureOp   ->addItem(tr("None")          ,0) ;
  ui->PictureOp   ->addItem(tr("And" )          ,1) ;
  ui->PictureOp   ->addItem(tr("Or"  )          ,2) ;
  ui->PictureOp   ->addItem(tr("Xor" )          ,3) ;
  ui->PictureOp   ->addItem(tr("Not" )          ,4) ;
  ///////////////////////////////////////////////////
  NewTreeWidgetItem(phead)                          ;
  phead->setText ( 0 , tr("Operation") )            ;
  phead->setText ( 1 , tr("Parament" ) )            ;
  phead->setText ( 2 , tr("Rule"     ) )            ;
  ui   ->PictureSearch->setHeaderItem ( phead )     ;
  ///////////////////////////////////////////////////
  connect(this,SIGNAL(EmitPictures())               ,
          this,SLOT  (SendPictures())             ) ;
}

bool N::SearchTool::Relocation(void)
{
  QWidget * w = currentWidget()                                   ;
  if (IsNull(w)) return false                                     ;
//  nDropOut ( IsNull(w) )                                          ;
  QSize s  = w->size()                                            ;
  /////////////////////////////////////////////////////////////////
  QRect pps = ui->PictureStack->geometry()                        ;
  ui->PictureStack->resize(pps.width(),s.height()-pps.y())        ;
  /////////////////////////////////////////////////////////////////
  QRect psi = ui->PictureSearch->geometry()                       ;
  ui->PictureSearch->resize(s.width()-psi.x(),s.height()-psi.y()) ;
  return true                                                     ;
}

bool N::SearchTool::startup(void)
{
  return true ;
}

bool N::SearchTool::Menu(QPoint)
{
  return true ;
}

void N::SearchTool::run(void)
{
  setEnabled          ( false )         ;
  plan->Talk ( tr("Start searching")  ) ;
  DoProcessEvents                       ;
  switch ( Mode )                       {
    nFastCast ( 101 , PictureRules () ) ;
  }                                     ;
  setEnabled          ( true  )         ;
  DoProcessEvents                       ;
  Alert  ( Done )                       ;
}

/******************************************************************************
 *                                                                            *
 *                          Picture search section                            *
 *                                                                            *
 ******************************************************************************/

void N::SearchTool::PictureScripts(void)
{
  emit PictureScript ( ) ;
}

void N::SearchTool::PictureItems(int index)
{
  ui->PictureStack->setCurrentIndex(index) ;
}

void N::SearchTool::AddPictureRules(void)
{
  QString pwOp   = ui -> pwOp         -> currentText  ( )     ;
  QString phOp   = ui -> phOp         -> currentText  ( )     ;
  int     Width  = ui -> pwValue      -> value        ( )     ;
  int     Height = ui -> phValue      -> value        ( )     ;
  bool    isW    = ui -> PictureW     -> isChecked    ( )     ;
  bool    isH    = ui -> PictureH     -> isChecked    ( )     ;
  int     pi     = ui -> PictureItems -> currentIndex ( )     ;
  int     po     = ui -> PictureOp    -> currentIndex ( )     ;
  QString pc     = ui -> PictureC     -> currentText  ( )     ;
  int     Colors = ui -> pwColors     -> value        ( )     ;
  QString Q                                                   ;
  /////////////////////////////////////////////////////////////
  NewTreeWidgetItem(item)                                     ;
  switch (pi)                                                 {
    case 0                                                    :
      if (isW && isH)                                         {
        Q = QString("( ( width %1 %2 ) and ( height %3 %4 ) )")
            .arg(pwOp).arg(Width).arg(phOp).arg(Height)       ;
      } else
      if (isW)                                                {
        Q = QString("( width %1 %2 )"                         )
            .arg(pwOp).arg(Width)                             ;
      } else
      if (isH)                                                {
        Q = QString("( height %3 %4 )"                        )
            .arg(phOp).arg(Height)                            ;
      } else return                                           ;
    break                                                     ;
    case 1                                                    :
      Q = QString("( colors %1 %2 ) "                         )
          .arg(pc).arg(Colors)                                ;
    break                                                     ;
  }                                                           ;
  item->setData(0,Qt::UserRole,po                )            ;
  item->setData(1,Qt::UserRole,pi                )            ;
  item->setText(0,ui->PictureOp   ->currentText())            ;
  item->setText(1,ui->PictureItems->currentText())            ;
  item->setText(2,Q                              )            ;
  ui->PictureSearch->addTopLevelItem(item)                    ;
  /////////////////////////////////////////////////////////////
  ui->RemovePicture->setEnabled(true)                         ;
  ui->PictureTrash ->setEnabled(true)                         ;
  ui->PictureUp    ->setEnabled(true)                         ;
  ui->PictureDown  ->setEnabled(true)                         ;
  if (po<=0)                                                  {
    ui -> PictureOp -> setCurrentIndex ( 1 )                  ;
  }                                                           ;
}

void N::SearchTool::RemovePictures(void)
{
  QTreeWidgetItem * item                               ;
  item = ui->PictureSearch->currentItem()              ;
  nDropOut ( IsNull(item) )                            ;
  int index                                            ;
  index = ui->PictureSearch->indexOfTopLevelItem(item) ;
  ui->PictureSearch->takeTopLevelItem(index)           ;
  nDropOut (ui->PictureSearch->topLevelItemCount()>0)  ;
  ui->RemovePicture->setEnabled      ( false )         ;
  ui->PictureTrash ->setEnabled      ( false )         ;
  ui->PictureUp    ->setEnabled      ( true  )         ;
  ui->PictureDown  ->setEnabled      ( true  )         ;
  ui->PictureOp    ->setCurrentIndex ( 0     )         ;
}

void N::SearchTool::TrashPictures(void)
{
  ui->PictureSearch->clear           (       ) ;
  ui->RemovePicture->setEnabled      ( false ) ;
  ui->PictureTrash ->setEnabled      ( false ) ;
  ui->PictureUp    ->setEnabled      ( true  ) ;
  ui->PictureDown  ->setEnabled      ( true  ) ;
  ui->PictureOp    ->setCurrentIndex ( 0     ) ;
}

void N::SearchTool::PictureUp(void)
{
  QTreeWidgetItem * item                               ;
  item = ui->PictureSearch->currentItem()              ;
  nDropOut ( IsNull(item) )                            ;
  int index                                            ;
  index = ui->PictureSearch->indexOfTopLevelItem(item) ;
  ui->PictureSearch->takeTopLevelItem(index)           ;
  index--                                              ;
  if (index<0) index = 0                               ;
  ui->PictureSearch->insertTopLevelItem(index,item)    ;
}

void N::SearchTool::PictureDown(void)
{
  QTreeWidgetItem * item                               ;
  item = ui->PictureSearch->currentItem()              ;
  nDropOut ( IsNull(item) )                            ;
  int index                                            ;
  index = ui->PictureSearch->indexOfTopLevelItem(item) ;
  ui->PictureSearch->takeTopLevelItem(index)           ;
  index++                                              ;
  int end = ui->PictureSearch->topLevelItemCount()     ;
  if (index>=end) index = end                          ;
  ui->PictureSearch->insertTopLevelItem(index,item)    ;
}

void N::SearchTool::SearchPictures(void)
{
  Mode = 101 ;
  start ( )  ;
}

void N::SearchTool::SendPictures(void)
{
  emit Pictures ( tr("Search pictures") , PictureUuids ) ;
}

UUIDs N::SearchTool::UUID(SqlConnection & SC,QString Q)
{
  UUIDs  Uuids               ;
  SUID   uuid                ;
  SqlLoopNow ( SC , Q )      ;
    uuid   = SC . Uuid ( 0 ) ;
    Uuids << uuid            ;
  SqlLoopErr ( SC , Q )      ;
  SqlLoopEnd ( SC , Q )      ;
  return Uuids               ;
}

void N::SearchTool::PictureRules(void)
{
  PictureUuids . clear ( )                                                ;
  /////////////////////////////////////////////////////////////////////////
  QString Q                                                               ;
  UUIDs   Uuids                                                           ;
  SUID    uuid                                                            ;
  EnterSQL ( SC , plan->sql )                                             ;
    for (int i=0;i<ui->PictureSearch->topLevelItemCount();i++)            {
      QTreeWidgetItem * item = ui->PictureSearch->topLevelItem(i)         ;
      int op = item->data(0,Qt::UserRole).toInt()                         ;
      int pi = item->data(1,Qt::UserRole).toInt()                         ;
      QString OC = ""                                                     ;
      switch (op)                                                         {
        case 0 /* None */                                                 :
        break                                                             ;
        case 1 /* And  */                                                 :
        break                                                             ;
        case 2 /* Or   */                                                 :
        break                                                             ;
        case 3 /* Xor  */                                                 :
        break                                                             ;
        case 4 /* Not  */                                                 :
          OC = "!"                                                        ;
        break                                                             ;
      }                                                                   ;
      switch (pi)                                                         {
        case 0                                                            :
          Q = QString("select uuid from %1 where %2%3 ;"                  )
              .arg(PlanTable(ThumbUuid)                                   )
              .arg(OC                                                     )
              .arg(item->text(2)                                        ) ;
        break                                                             ;
        case 1                                                            :
          Q = QString("select uuid from %1 where %2%3 ;"                  )
              .arg(PlanTable(PictureColors)                               )
              .arg(OC                                                     )
              .arg(item->text(2)                                        ) ;
        break                                                             ;
      }                                                                   ;
      Uuids = UUID(SC,Q)                                                  ;
      switch (op)                                                         {
        case 0 /* None */                                                 :
          PictureUuids = Uuids                                            ;
        break                                                             ;
        case 1 /* And  */                                                 :
          PictureUuids = PictureUuids & Uuids                             ;
        break                                                             ;
        case 2 /* Or   */                                                 :
          PictureUuids = PictureUuids + Uuids                             ;
        break                                                             ;
        case 3 /* Xor  */                                                 :
        break                                                             ;
        case 4 /* Not  */                                                 :
        break                                                             ;
      }                                                                   ;
    }                                                                     ;
  LeaveSQL ( SC , plan->sql )                                             ;
  Alert    ( Done           )                                             ;
  plan -> Talk ( 30 , tr("%1 pictures found").arg(PictureUuids.count()) ) ;
  emit EmitPictures ( )                                                   ;
}
