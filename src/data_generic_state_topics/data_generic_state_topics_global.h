#ifndef DATA_GENERIC_STATE_TOPICS_GLOBAL_H
#define DATA_GENERIC_STATE_TOPICS_GLOBAL_H

#ifdef _MSC_VER
#  if defined(DATA_GENERIC_STATE_TOPICS_LIBRARY)
#    define DATA_GENERIC_STATE_TOPICSSHARED_EXPORT  __declspec(dllexport)
#  else
#    define DATA_GENERIC_STATE_TOPICSSHARED_EXPORT  __declspec(dllimport)
#  endif
#else
#  define DATA_GENERIC_STATE_TOPICSSHARED_EXPORT
#endif

#endif // DATA_GENERIC_STATE_TOPICS_GLOBAL_H
