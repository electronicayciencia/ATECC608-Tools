/* Cryptoauthlib Configuration File */
/* Include this file before cryptoauthlib.h to overwrite local atca_config.h if
 * it exists. */

#ifndef ATCA_CONFIG_H
#define ATCA_CONFIG_H


/* Include HALS */
#define ATCA_HAL_I2C

/* Included device support */
/* This will disable ATCA_CA2_SUPPORT. Some CA2 functions are not applicable to
 * CA devices. Like calib_info_lock_status */ 
#define ATCA_ATECC608_SUPPORT

#ifndef ATCA_POST_DELAY_MSEC
#define ATCA_POST_DELAY_MSEC 25
#endif


#endif // ATCA_CONFIG_H

