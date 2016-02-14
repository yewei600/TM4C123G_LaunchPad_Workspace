################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
bitband.obj: ../bitband.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/Program Files (x86)/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/Program Files (x86)/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RA1 --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="bitband.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/Program Files (x86)/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="C:/Program Files (x86)/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RA1 --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


