#ifndef DATA_VEHICLE_GENERIC_GLOBAL_H
#define DATA_VEHICLE_GENERIC_GLOBAL_H

#ifdef _MSC_VER
#  if defined(DATA_VEHICLE_GENERIC_LIBRARY)
#    define DATA_VEHICLE_GENERICSHARED_EXPORT __declspec(dllexport)
#  else
#    define DATA_VEHICLE_GENERICSHARED_EXPORT __declspec(dllexport)
#  endif
#else
#  define DATA_VEHICLE_GENERICSHARED_EXPORT
#endif

#endif // DATA_VEHICLE_GENERIC_GLOBAL_H