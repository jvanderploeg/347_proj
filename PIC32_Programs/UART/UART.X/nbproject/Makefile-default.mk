#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1360919505/adc_setup.o ${OBJECTDIR}/_ext/1360930046/lcd_setup.o ${OBJECTDIR}/_ext/760805413/misc.o ${OBJECTDIR}/_ext/789330691/setup_timers.o ${OBJECTDIR}/_ext/1472/uart_setup.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1360919505/adc_setup.o.d ${OBJECTDIR}/_ext/1360930046/lcd_setup.o.d ${OBJECTDIR}/_ext/760805413/misc.o.d ${OBJECTDIR}/_ext/789330691/setup_timers.o.d ${OBJECTDIR}/_ext/1472/uart_setup.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1360919505/adc_setup.o ${OBJECTDIR}/_ext/1360930046/lcd_setup.o ${OBJECTDIR}/_ext/760805413/misc.o ${OBJECTDIR}/_ext/789330691/setup_timers.o ${OBJECTDIR}/_ext/1472/uart_setup.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c   
	
${OBJECTDIR}/_ext/1360919505/adc_setup.o: ../adc/adc_setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360919505 
	@${RM} ${OBJECTDIR}/_ext/1360919505/adc_setup.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360919505/adc_setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/1360919505/adc_setup.o.d" -o ${OBJECTDIR}/_ext/1360919505/adc_setup.o ../adc/adc_setup.c   
	
${OBJECTDIR}/_ext/1360930046/lcd_setup.o: ../lcd/lcd_setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930046 
	@${RM} ${OBJECTDIR}/_ext/1360930046/lcd_setup.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930046/lcd_setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/1360930046/lcd_setup.o.d" -o ${OBJECTDIR}/_ext/1360930046/lcd_setup.o ../lcd/lcd_setup.c   
	
${OBJECTDIR}/_ext/760805413/misc.o: ../misc/misc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760805413 
	@${RM} ${OBJECTDIR}/_ext/760805413/misc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760805413/misc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/760805413/misc.o.d" -o ${OBJECTDIR}/_ext/760805413/misc.o ../misc/misc.c   
	
${OBJECTDIR}/_ext/789330691/setup_timers.o: ../timers/setup_timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/789330691 
	@${RM} ${OBJECTDIR}/_ext/789330691/setup_timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/789330691/setup_timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/789330691/setup_timers.o.d" -o ${OBJECTDIR}/_ext/789330691/setup_timers.o ../timers/setup_timers.c   
	
${OBJECTDIR}/_ext/1472/uart_setup.o: ../uart_setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/uart_setup.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/uart_setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/1472/uart_setup.o.d" -o ${OBJECTDIR}/_ext/1472/uart_setup.o ../uart_setup.c   
	
else
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c   
	
${OBJECTDIR}/_ext/1360919505/adc_setup.o: ../adc/adc_setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360919505 
	@${RM} ${OBJECTDIR}/_ext/1360919505/adc_setup.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360919505/adc_setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/1360919505/adc_setup.o.d" -o ${OBJECTDIR}/_ext/1360919505/adc_setup.o ../adc/adc_setup.c   
	
${OBJECTDIR}/_ext/1360930046/lcd_setup.o: ../lcd/lcd_setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930046 
	@${RM} ${OBJECTDIR}/_ext/1360930046/lcd_setup.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930046/lcd_setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/1360930046/lcd_setup.o.d" -o ${OBJECTDIR}/_ext/1360930046/lcd_setup.o ../lcd/lcd_setup.c   
	
${OBJECTDIR}/_ext/760805413/misc.o: ../misc/misc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/760805413 
	@${RM} ${OBJECTDIR}/_ext/760805413/misc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760805413/misc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/760805413/misc.o.d" -o ${OBJECTDIR}/_ext/760805413/misc.o ../misc/misc.c   
	
${OBJECTDIR}/_ext/789330691/setup_timers.o: ../timers/setup_timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/789330691 
	@${RM} ${OBJECTDIR}/_ext/789330691/setup_timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/789330691/setup_timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/789330691/setup_timers.o.d" -o ${OBJECTDIR}/_ext/789330691/setup_timers.o ../timers/setup_timers.c   
	
${OBJECTDIR}/_ext/1472/uart_setup.o: ../uart_setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/uart_setup.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/uart_setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../timers" -I".." -I"../misc" -I"../lcd" -I"../adc" -MMD -MF "${OBJECTDIR}/_ext/1472/uart_setup.o.d" -o ${OBJECTDIR}/_ext/1472/uart_setup.o ../uart_setup.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-L"../C:/Program Files (x86)/Microchip/MPLAB C32 Suite/lib",-L"../C:/Program Files (x86)/Microchip/MPLAB C32 Suite/pic32mx/lib",-Map="${DISTDIR}/demo.X.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/UART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/UART.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-L"../C:/Program Files (x86)/Microchip/MPLAB C32 Suite/lib",-L"../C:/Program Files (x86)/Microchip/MPLAB C32 Suite/pic32mx/lib",-Map="${DISTDIR}/demo.X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/UART.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
