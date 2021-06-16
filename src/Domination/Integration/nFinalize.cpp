#include <domination.h>

static void initialOpenMP(void)
{
  N::CPU::Usage cpu               ;
  int CPUs = cpu . Processors ( ) ;
  if ( CPUs > 2 ) CPUs = CPUs - 1 ;
  cpu . setThreads ( CPUs )       ;
}

#if defined(Q_OS_ANDROID) || defined (Q_OS_IOS)

static bool initNeutrinoAll(void)
{
  initialOpenMP         ( ) ;
  N::Math::Initialize   ( ) ;
  N::Socket::Initialize ( ) ;
  N::initLIBAV          ( ) ;
  QtCURL::Initialize    ( ) ;
  srand ( time ( NULL ) )   ;
  srand ( rand (      ) )   ;
  return true               ;
}

bool N::Initialize(InitialSystem init)
{
  srand ( time ( NULL ) )       ;
  switch (init)                 {
    case InitAll                :
      initNeutrinoAll       ( ) ;
    break                       ;
    case InitNetwork            :
      N::Socket::Initialize ( ) ;
    break                       ;
    case InitLIBAV              :
      initLIBAV             ( ) ;
    break                       ;
    case InitCURL               :
      QtCURL::Initialize    ( ) ;
    break                       ;
    case InitMath               :
      N::Math::Initialize   ( ) ;
    break                       ;
  }                             ;
  srand ( time ( NULL ) )       ;
  srand ( rand (      ) )       ;
  return true                   ;
}

#else

static bool initNeutrinoAll(void)
{
  initialOpenMP         ( ) ;
  N::Math::Initialize   ( ) ;
  N::Socket::Initialize ( ) ;
  QtCURL::Initialize    ( ) ;
  N::initLIBAV          ( ) ;
  srand ( time ( NULL ) )   ;
  srand ( rand (      ) )   ;
  return true               ;
}

bool N::Initialize(InitialSystem init)
{
  srand ( time ( NULL ) )       ;
  switch (init)                 {
    case InitAll                :
      initNeutrinoAll       ( ) ;
    break                       ;
    case InitNetwork            :
      N::Socket::Initialize ( ) ;
    break                       ;
    case InitLIBAV              :
      initLIBAV             ( ) ;
    break                       ;
    case InitCURL               :
      QtCURL::Initialize    ( ) ;
    break                       ;
    case InitMath               :
      N::Math::Initialize   ( ) ;
    break                       ;
  }                             ;
  srand ( time ( NULL ) )       ;
  srand ( rand (      ) )       ;
  return true                   ;
}

#endif
