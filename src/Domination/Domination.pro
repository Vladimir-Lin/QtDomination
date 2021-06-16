NAME         = Domination
TARGET       = $${NAME}

QT           = core
QT          += gui
QT          += network
QT          += sql
QT          += script
QT          += positioning
QT          += QtCURL
QT          += QtCUDA
QT          += QtTorrent
QT          += QtZMQ
QT          += QtPCAP
QT          += QtOpenCV
QT          += QtProlog
QT          += QtLISP
QT          += QtPython
QT          += QtFFmpeg
QT          += QtAudio
QT          += Essentials
QT          += QtCalendar
QT          += QtCryptography
QT          += QtMIME
QT          += SqlAnalyzer
QT          += QtGMP
QT          += QtDAI
QT          += QtGSL
QT          += QtANN
QT          += QtAlgebra
QT          += QtDiscrete
QT          += QtFFT
QT          += Mathematics
QT          += QtFuzzy
QT          += QtFLP
QT          += QtGeography
QT          += NetProtocol
QT          += QtXmlRPC
QT          += QtUDT
QT          += QtFTP
QT          += QtRPC
QT          += QtJsonRPC
QT          += AudioIO
QT          += QtPhonemes
QT          += QtVocal
QT          += QtSpeech
QT          += QtNLP
QT          += QtMatcher
QT          += QtLinguistics
QT          += QtForth
QT          += QtNLC
QT          += QtYacc
QT          += QtFoundation
QT          += QtInference
QT          += QtDecision
QT          += Science
QT          += QtAutomation
QT          += QtAutonomy
QT          += QtGeometry
QT          += QtGadgets
QT          += QtWorld
QT          += QtComponents
QT          += QtAIV
QT          += QtManagers
QT          += Physics
QT          += Chemistry
QT          += Biology
QT          += Business
QT          += QtForex
QT          += Marketing
QT          += QtModels
QT          += QtAnalog
QT          += NetWidgets
QT          += MathWidgets
QT          += QtGIS
QT          += QtCLF
QT          += QtVCF
QT          += DataWidgets
QT          += QtTasks
QT          += QtPhotography
QT          += QtHuman
QT          += Society
QT          += QtAV
QT          += QScintilla2

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/domination.h

include ($${PWD}/Integration/Integration.pri)
include ($${PWD}/Rules/Rules.pri)

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)
