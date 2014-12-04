#ifndef __UVISOR_LIB_H__
#define __UVISOR_LIB_H__

#ifdef __cplusplus
#define UVISOR_EXTERN extern "C"
#else
#define UVISOR_EXTERN extern
#endif

#define SET_UVISOR_MODE(mode) UVISOR_EXTERN const uint32_t __uvisor_mode = (mode);

#endif/*__UVISOR_LIB_H__*/
