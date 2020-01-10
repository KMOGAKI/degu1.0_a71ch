cmake_minimum_required(VERSION 3.13.1)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(mainA71CH)

FILE(GLOB app_sources source/*.c)
target_sources(app PRIVATE ${app_sources})

zephyr_include_directories(
  .
  $ENV{ZEPHYR_BASE}/../modules/lib/se_hostlib/tst
  $ENV{ZEPHYR_BASE}/../modules/lib/se_hostlib/inc
  $ENV{ZEPHYR_BASE}/../modules/lib/se_hostlib/platform
  $ENV{ZEPHYR_BASE}/../modules/lib/se_hostlib/common
  $ENV{ZEPHYR_BASE}/../modules/crypto/mbedtls/include
  )

add_definitions(
  -DZEPHYR
  -DFTR_FILE_SYSTEM
  -DTGT_A71CH
  -DI2C
  -DMBEDTLS
  -DSCP_MODE=NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC
  )
