#
## Makefile for MCTCP support code.
#
#

obj-$(CONFIG_MCPS) += mcps.o

mcps-y := mcps_main.o mcps_gro_main.o mcps_core.o mcps_params.o mcps_buffer.o
mcps-y += migration/mcps_migration.o
mcps-y += utils/mcps_cpu.o
mcps-$(CONFIG_MCPS_DEBUG) += mcps_debug.o

GCOV_PROFILE := y
