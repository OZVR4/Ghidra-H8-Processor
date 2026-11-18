typedef unsigned char   undefined;

typedef unsigned char    bool;
typedef unsigned char    byte;
typedef unsigned int    dword;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined3;
typedef unsigned int    undefined4;
typedef unsigned short    ushort;
typedef unsigned short    word;
typedef struct table_3axis_record table_3axis_record, *Ptable_3axis_record;

struct table_3axis_record {
    byte bMode;
    byte bMode_hi;
    ushort wX_ptr;
    ushort wY_ptr;
    byte bNrows;
    ushort wData;
};

typedef unsigned short    wchar16;
typedef struct TableHeader TableHeader, *PTableHeader;

struct TableHeader {
    ushort wRam_ptr1;
    ushort wRam_ptr2;
    ushort wElement_count;
};

typedef struct axis_lookup_record axis_lookup_record, *Paxis_lookup_record;

struct axis_lookup_record {
    ushort wValue_ptr;
    ushort wAxis_ptr;
    ushort wCount;
    ushort wBreakpoints;
};

typedef struct FlashRegionClassifyEntry FlashRegionClassifyEntry, *PFlashRegionClassifyEntry;

struct FlashRegionClassifyEntry {
    byte bPad0;
    byte bRegion_base_hi;
    byte bRegion_base_lo;
    byte bPad1;
    byte bSize_mask_hi;
    byte bSize_mask_lo;
};

typedef struct TableLookupDesc TableLookupDesc, *PTableLookupDesc;

struct TableLookupDesc {
    char cMode;
    char cBias;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    byte bStride;
    undefined field7_0x7;
};

typedef struct table_2d_record table_2d_record, *Ptable_2d_record;

struct table_2d_record {
    byte bMode;
    byte bMode_hi;
    ushort wIndex_src;
    byte bData;
};



ushort DAT_0001f1fc;
undefined PTR_LAB_000138a8;
undefined1 DAT_0001f7e1;
undefined UNK_00003898;
undefined DAT_0000f814;
undefined DAT_0000f862;
undefined1 UNK_0001f7e2;
undefined1 UNK_0001f7e3;
undefined1 UNK_0001f7e4;
undefined1 UNK_0001f7e5;
undefined2 DAT_0000f84a;
undefined1 UNK_0001f7e6;
undefined1 UNK_0001f7e7;
undefined1 UNK_0001f7e8;
undefined1 UNK_0001f7e9;
undefined1 UNK_0001f7ea;
undefined1 UNK_0001f7eb;
undefined1 UNK_0001f7ec;
undefined1 UNK_0001f7ed;
undefined1 UNK_0001f816;
undefined1 UNK_0001f817;
undefined1 UNK_0001f818;
undefined1 UNK_0001f819;
undefined UNK_0000f7e6;
undefined DAT_0000f815;
undefined UNK_0000f81a;
undefined PTR_LAB_000138a4;
short DAT_0001ff46;
undefined UNK_0000f7e1;
undefined DAT_0000f7e2;
undefined UNK_0000f816;
undefined DAT_0000f848;
undefined mut_snapshot_ringbuf_base;
undefined adc_sensor_convert_single;
undefined2 DAT_0000fe7c;
undefined2 DAT_0000fe7e;
undefined1 FLMCR;
undefined init_state_block_f97a;
undefined timer_block_cold_init;
undefined serial_fef4_cold_init;
undefined peripheral_block_fec0_fed0_cold_init;
undefined peripheral_block_ff70_ff88_cold_init;
undefined peripheral_block_ff00_cold_init;
undefined peripheral_block_ff15_cold_init;
undefined init_reg_feb8_feb9;
undefined timer_ff41_ff42_warm_init;
undefined peripheral_ff42_ff62_enable;
undefined peripheral_block_ff70_ff88_warm_init;
undefined update_flag_f9a0_bit6_cond;
undefined peripheral_block_ff15_warm_init;
undefined noop_return_void;
char DAT_000102e2;
ushort DAT_0001f0ea;
undefined1 pwm_channel_track_status_ff74;
undefined2 pwm_channel_control_flags_f9a0;
ushort DAT_0001f0f6;
byte DAT_0001f0d8;
byte DAT_0001fe96;
byte DAT_0001f0d9;
byte DAT_0001f0da;
byte DAT_0001f0db;
byte DAT_0001fe8b;
byte DAT_0001fe8e;
ushort DAT_0001f0fc;
ushort DAT_0001f102;
byte DAT_0001fe8a;
undefined1 P4DR;
undefined adc_channel_convert_and_read;
undefined2 DAT_0001f15a;
char DAT_000102e6;
byte DAT_0001fe97;
byte DAT_0001fe83;
byte DAT_000102ed;
undefined2 output_relay_flags_f0e6;
char DAT_000102e4;
ushort DAT_0001f1f6;
undefined2 DAT_0001f0e8;
undefined1 P1DR;
undefined1 g_egr_enable_config_flag;
char DAT_000102c8;
char DAT_000102c2;
byte DAT_0001fe93;
char DAT_000102e3;
char DAT_000102f3;
ushort DAT_0001f1f4;
undefined2 g_status_flags_f1f2;
ushort DAT_0001f0ec;
char DAT_000102eb;
ushort DAT_0001eed2;
ushort DAT_0001f0f0;
char DAT_000102de;
short DAT_0001f448;
short DAT_0001f45c;
short DAT_0001f452;
short DAT_0001f456;
short DAT_0001f454;
short DAT_0001f458;
ushort DAT_0001f44c;
undefined2 radfan_pwm_duty_target_f494;
undefined ff88_set_scaled_clamped;
undefined2 DAT_0001f9aa;
undefined2 DAT_0001f9ac;
byte DAT_0001ff15;
undefined DAT_0001fe80;
undefined DAT_0001fe84;
undefined DAT_0001fe88;
undefined1 DAT_0001fe8c;
undefined1 DAT_0001fe91;
undefined DAT_0001fe94;
undefined1 DAT_0001ff21;
undefined1 DAT_0001ff31;
undefined1 DAT_0001ff22;
undefined1 DAT_0001ff32;
undefined1 DAT_0001ff41;
undefined1 DAT_0001ff42;
undefined1 DAT_0001ff51;
undefined1 DAT_0001ff52;
undefined1 DAT_0001ff54;
undefined1 DAT_0001ff61;
undefined1 DAT_0001ff62;
undefined1 DAT_0001ff64;
undefined1 T1OERB;
undefined1 DAT_0001ff71;
undefined1 DAT_0001ff72;
undefined1 DAT_0001ff81;
undefined1 DAT_0001ff82;
undefined1 DAT_0001ff84;
undefined1 DAT_0001ff91;
undefined1 DAT_0001ff92;
undefined1 DAT_0001ff94;
undefined DAT_0001feda;
undefined1 DAT_0001feb8;
undefined1 DAT_0001feb9;
undefined1 T1OERA;
undefined1 sci3_timeout_state_ff44;
undefined2 DAT_0001f992;
undefined2 DAT_0001f97a;
undefined2 DAT_0001f97c;
undefined2 DAT_0001f996;
undefined2 DAT_0001f998;
undefined2 DAT_0001f99a;
undefined2 DAT_0001f99c;
undefined2 DAT_0001f994;
undefined2 DAT_0001f99e;
byte DAT_0001ff52;
byte DAT_0001ff51;
undefined2 DAT_0001f1ba;
undefined2 DAT_0001f1dc;
undefined2 DAT_0001f1d4;
undefined DAT_0000f97a;
undefined T3GR1H;
ushort DAT_0001f994;
ushort DAT_0001f99c;
short DAT_0001f992;
ushort DAT_0001f99e;
undefined DAT_0000f97e;
short DAT_0001f1d6;
short DAT_0001f99c;
short DAT_0001f99e;
undefined2 DAT_0001ef64;
undefined2 DAT_00010f56;
undefined2 DAT_0001ef96;
undefined1 DAT_0001ff20;
undefined1 DAT_0001ff40;
undefined1 DAT_0001ff50;
undefined1 DAT_0001ff60;
undefined1 DAT_0001ff30;
byte DAT_0001ff41;
byte DAT_0001ff61;
byte DAT_0001ff42;
byte DAT_0001ff62;
undefined1 DAT_0001ff25;
undefined2 DAT_0001ff26;
byte DAT_0001ff64;
short DAT_0001ff66;
short DAT_0001ff6c;
byte DAT_0001ff63;
byte DAT_0001ff54;
undefined1 T4SRL;
undefined2 T4CNTH;
undefined2 T4DR1H;
short DAT_0001ff4a;
short DAT_0001ff4c;
short DAT_0001ff4e;
byte DAT_0001ff43;
undefined1 T2SRL;
undefined sci3_rx_process_tcu_byte;
byte DAT_0001fe86;
undefined1 DAT_0001ff15;
undefined1 DAT_0001ff1b;
undefined1 DAT_0001ff1c;
undefined1 DAT_0001ff14;
undefined1 DAT_0001fe9a;
undefined2 DAT_0001ff10;
undefined2 DAT_0001ff1e;
undefined1 DAT_0001ff1a;
undefined1 DAT_0001fee0;
undefined1 DAT_0001feec;
undefined2 DAT_0001f974;
undefined2 DAT_0001f976;
undefined2 DAT_0000f974;
undefined2 DAT_0000f976;
undefined1 DAT_0001ff1d;
byte DAT_0001fede;
undefined1 DAT_0001fe96;
undefined1 DAT_0001f0d8;
undefined1 DAT_0001f0d9;
undefined1 DAT_0001f0da;
undefined1 DAT_0001f0db;
undefined1 DAT_0001ff90;
ushort DAT_0001ff96;
byte DAT_0001ff93;
short DAT_0001ff26;
short DAT_0001ff2c;
short DAT_0001ff2e;
short DAT_0001ff3c;
short DAT_0001ff3e;
short DAT_0001ff38;
short DAT_0001ff3a;
undefined2 DAT_0001ff2c;
undefined1 DAT_0001ff23;
undefined2 DAT_0001ff2e;
undefined2 DAT_0001ff3c;
undefined1 DAT_0001ff33;
undefined2 DAT_0001ff3e;
undefined2 DAT_0001ff38;
undefined2 DAT_0001ff3a;
undefined1 T1SRAL;
short DAT_0001ff2a;
short DAT_0001ff56;
short DAT_0001ff5a;
char DAT_000102c3;
undefined1 DAT_0001fef5;
undefined1 DAT_0001fef4;
undefined1 DAT_0001fef9;
undefined1 DAT_0001fef8;
undefined1 DAT_0001fef1;
undefined1 DAT_0001fec9;
undefined1 DAT_0001fec8;
undefined1 DAT_0001fed2;
undefined1 DAT_0001fed1;
undefined1 DAT_0001fed0;
undefined1 DAT_0001fec2;
undefined1 DAT_0001fec1;
undefined1 DAT_0001fec0;
undefined1 SCI1_SCR_0001feca;
ushort DAT_0001f0f2;
char DAT_0001fec9;
char DAT_0001fec8;
byte DAT_0001ff0d;
char DAT_0001fed1;
char DAT_0001fed0;
char DAT_0001fed2;
char DAT_0001fec1;
char DAT_0001fec0;
ushort DAT_0001f516;
undefined1 *DAT_00016672;
byte DAT_0001fec4;
undefined2 DAT_0001f4d0;
undefined2 UNK_0001f4ba;
undefined1 UNK_0001fec3;
ushort DAT_0001f4d0;
byte DAT_0001fec2;
short UNK_0001f4ba;
byte DAT_0001fec5;
undefined2 tcu_rx_frame_state_counter_f4ce;
undefined2 DAT_0001f588;
short DAT_0001f58c;
undefined1 DAT_0001fecb;
byte DAT_0001fecc;
undefined2 DAT_0001f590;
undefined2 g_sci1_rx_status_flags_f522;
ushort DAT_0001f588;
ushort DAT_0001f58c;
byte DAT_0001fecd;
ushort DAT_0001f584;
undefined2 DAT_0001f58a;
ushort DAT_0001f524;
short DAT_0001f58e;
undefined1 DAT_0001fed3;
byte DAT_0001fed4;
undefined2 DAT_0001f592;
undefined2 DAT_0000f576;
ushort DAT_0001f58a;
ushort DAT_0001f58e;
byte DAT_0001fed5;
ushort DAT_0001f586;
undefined2 DAT_0000f9c0;
undefined2 g_sci1_rx_frame_error_flag_f9c2;
undefined1 DTC_DTEF;
undefined1 SCI1_SCR;
undefined1 SCI1_SSR;
undefined1 SCI2_SSR;
undefined serial_fec3_load_next_byte;
undefined1 SCI3_SSR;
undefined2 DAT_0001f5de;
undefined2 DAT_0001f5e0;
undefined2 DAT_0001f5d2;
undefined2 DAT_0001f5d4;
undefined2 DAT_0001f5d6;
undefined2 DAT_0001f5d8;
undefined2 DAT_0001ef66;
undefined2 DAT_0001ef68;
undefined2 DAT_0000f5e2;
undefined2 DAT_0000f5e4;
undefined1 T2CRL;
undefined tcu_shift_torque_and_knock_mgmt;
undefined2 T2CNTH;
undefined2 T4GR1H;
undefined tcu_shift_timing_state_machine;
undefined2 T2GR1H;
undefined1 P5DR;
undefined2 DAT_0000f5d2;
undefined2 DAT_0000f5d4;
undefined2 ipu_ch2ch4_period_delta_raw;
undefined2 DAT_0000f5da;
undefined2 g_ipu_ch2_capture_settle_counter;
undefined2 g_ipu_ch4_capture_settle_counter;
undefined1 DAT_0001ff70;
byte DAT_0001ff72;
undefined2 DAT_0001f978;
undefined2 DAT_0001f9a4;
undefined2 DAT_0001f9b4;
undefined2 DAT_0001f9a8;
undefined2 DAT_0001f9b0;
undefined2 DAT_0001f9b8;
undefined2 DAT_0001f9a2;
undefined2 DAT_0001f9b2;
undefined2 DAT_0001f9a6;
undefined2 DAT_0001f9ae;
undefined2 DAT_0001f9b6;
short DAT_0001ff76;
short DAT_0001ff7a;
short DAT_0001ff7c;
short DAT_0001ff7e;
char DAT_000102fe;
undefined1 DAT_0001ff80;
undefined2 DAT_0001ff88;
undefined2 DAT_0001ff8a;
undefined1 DAT_0001ff35;
short DAT_0001ff8a;
ushort DAT_0001f978;
byte DAT_0001ff73;
ushort DAT_0001ff88;
undefined2 DAT_0000f9a0;
undefined2 DAT_0000f9a2;
undefined2 DAT_0000f9a4;
undefined1 T5SRL;
undefined1 T5OER;
undefined2 DAT_0000f9a6;
undefined2 T5CNTH;
undefined2 DAT_0000f9a8;
undefined2 T5GR2H;
ushort DAT_0001f9ae;
ushort DAT_0001f9ac;
ushort DAT_0001f9b0;
ushort DAT_0001f9aa;
ushort DAT_0001f9b6;
ushort DAT_0001f9b4;
ushort DAT_0001f9b8;
ushort DAT_0001f9b2;
undefined1 DAT_0001ff00;
undefined1 DAT_0001ff01;
undefined1 DAT_0001ff02;
undefined1 DAT_0001ff03;
undefined1 DAT_0001ff04;
undefined1 DAT_0001ff05;
undefined1 DAT_0001ff08;
undefined1 DAT_0001ff09;
undefined1 DAT_0001ff0a;
undefined1 DAT_0001ff0b;
undefined1 DAT_0001ff0c;
undefined1 DAT_0001ff0d;
undefined2 DAT_0001f9c4;
undefined2 DAT_0001f9c6;
undefined2 DAT_0001f9c8;
undefined2 DAT_0001f9ca;
undefined DAT_0000f9c8;
byte DAT_00010200;
ushort DAT_0001f520;
undefined SCI1_RDR;
undefined1 DAT_0001fecd;
undefined2 DAT_0001f9c2;
undefined2 DAT_0001f9ba;
undefined2 DAT_0001f9bc;
undefined2 DAT_0001f9be;
undefined2 DAT_0001f9c0;
undefined SCI1_TDR;
undefined1 *DAT_00013282;
undefined DAT_0000ee80;
ushort DAT_0001f498;
undefined2 DAT_00010e22;
undefined2 DAT_0001f49e;
undefined2 DAT_00010e20;
undefined2 DAT_0001f49c;
undefined2 DAT_0001f49a;
ushort DAT_0001f4ae;
undefined2 DAT_0001f4ac;
undefined2 DAT_00010e46;
undefined2 DAT_0001efec;
undefined2 DAT_0001f4f0;
undefined2 DAT_00010eec;
undefined2 DAT_0001f4ec;
undefined2 DAT_0001f466;
undefined2 DAT_00010ed6;
undefined2 DAT_0001f094;
undefined engine_mode_f20e_f510_check;
ushort DAT_00011130;
ushort DAT_00011134;
undefined2 DAT_0000f442;
ushort DAT_00011132;
ushort DAT_00011136;
ushort DAT_00011138;
ushort DAT_0001112e;
undefined2 g_status_flags_f1fe;
ushort DAT_0001ef70;
ushort DAT_0001f1de;
undefined2 g_tps_load_axis_f10e_prev;
undefined2 g_status_flags_f20e;
undefined2 g_tps_load_axis_f10e;
undefined2 gear_mode_flags_f510;
undefined2 DAT_0000f13a;
short DAT_00010dbc;
short DAT_0001efe4;
ushort DAT_00010dc2;
undefined2 g_rpm_zone_flags_f43e;
short DAT_00010dbe;
short DAT_0001efe6;
ushort DAT_00010dd2;
ushort DAT_00010dd0;
ushort DAT_00010dce;
ushort DAT_00010dcc;
byte DAT_000102ef;
ushort DAT_00010dd6;
short DAT_00010dd4;
short DAT_0001f044;
ushort DAT_00010dc6;
ushort DAT_00010dc4;
undefined2 g_status_flags_f0f8;
short DAT_00010dc0;
undefined2 g_status_flags_f0fa;
undefined2 engine_torque_pct_f17a;
short DAT_0001f042;
undefined2 g_f1e6_hysteresis_gate;
undefined2 g_warmup_state_f594;
undefined2 g_adc_ch7_raw_f13c;
undefined2 g_iat_axis_f118;
undefined2 DAT_0001f45c;
undefined2 DAT_0001f45e;
undefined isc_f03e_f45a_timer_update;
undefined isc_stepper_mode_dispatch;
ushort DAT_00010db4;
ushort DAT_00010db2;
ushort DAT_00010dae;
ushort DAT_00010dac;
undefined2 inj_pw_output_f186;
undefined axis_lookup_interp;
undefined isc_decel_fuel_cut_check;
undefined2 isc_state_flags_f45a;
undefined2 DAT_00011118;
undefined2 DAT_0001111a;
undefined2 DAT_0001111c;
ushort DAT_0001110a;
ushort DAT_0001110c;
char DAT_000102df;
short DAT_0001effe;
ushort DAT_00010daa;
ushort DAT_00010da4;
char DAT_000102e1;
char DAT_000102e0;
ushort DAT_0001111e;
undefined2 g_status_flags_f216;
undefined2 g_rpm_zone_dwell_counter_ef6a;
char DAT_0001030a;
ushort DAT_00010900;
ushort DAT_000108fc;
ushort DAT_0001f75a;
undefined2 coolant_temp_scaled_f130;
short DAT_0001f03e;
ushort DAT_00010da6;
ushort DAT_00010da8;
short DAT_0001effc;
short DAT_00010dba;
ushort DAT_00010db6;
undefined isc_f462_integrator_update;
undefined isc_integrator_update;
undefined isc_openloop_target_calc;
undefined isc_openloop_stepper_scale_calc;
undefined isc_dashpot_override_check;
short DAT_00011114;
short DAT_0001f05e;
undefined2 g_isc_stepper_integrator_f462;
undefined2 DAT_0001f48e;
ushort DAT_0001144e;
ushort DAT_0001f45c;
ushort DAT_0001f208;
ushort DAT_00011236;
ushort DAT_00011234;
ushort DAT_00011104;
ushort DAT_00011106;
ushort DAT_00011102;
ushort DAT_0001f464;
ushort DAT_0001f210;
short DAT_0001f092;
short DAT_0001112c;
ushort DAT_00011128;
ushort DAT_00011110;
ushort DAT_0001110e;
undefined weighted_blend_u8frac;
undefined muldiv_s16_rounded;
undefined clamp_u8;
short DAT_0001f064;
ushort DAT_0001f1c8;
undefined isc_stepper_blend_from_struct;
ushort DAT_0001146a;
undefined mul_u16_hi;
ushort DAT_0001f1cc;
undefined sat_sub_u16_clamp0;
ushort DAT_0001146c;
undefined sat_add_s16;
short DAT_00011468;
short DAT_0001f096;
undefined2 DAT_0001f478;
undefined2 DAT_0001f47a;
undefined2 DAT_0001f476;
undefined2 DAT_0001f474;
undefined2 DAT_0001f472;
undefined2 DAT_0001f47e;
undefined2 DAT_0001f47c;
undefined2 DAT_0001f482;
undefined2 DAT_0001f486;
undefined2 DAT_0001f480;
undefined2 DAT_000113f6;
undefined2 DAT_0001f484;
ushort DAT_0001f48c;
undefined2 g_isc_integrator_f468;
undefined mul_u16_x2_sat_alt;
undefined axis_interp_lerp_u8frac_256;
short DAT_0001f47c;
short DAT_0001f47e;
ushort DAT_000114b8;
undefined2 DAT_0001f470;
undefined2 DAT_0001f05e;
undefined2 DAT_00011100;
undefined f9a0_read_clear_bit4_atomic;
undefined2 g_status_flags_f0ea;
ushort DAT_0001f512;
undefined2 g_egr_activity_decay_reload_cal;
undefined2 g_egr_activity_decay_counter;
ushort DAT_0001ef6e;
undefined2 g_rpm_threshold_scale_cal;
undefined2 DAT_0001ef90;
undefined egr_coolant_rpm_enable_check;
ushort DAT_00010ebc;
undefined2 ign_advance_enable_flags_f440;
ushort DAT_00010ebe;
word WORD_00010ec4;
ushort DAT_00010ec2;
ushort DAT_00010ec0;
undefined mul_u16_x2_sat;
undefined egr_sequence_control;
undefined2 egr_valve_state_flags_f490;
undefined check_hysteresis_gate_f490;
undefined check_mode_gate_f510;
undefined egr_f490_gear_transition_update;
undefined egr_f0e6_valve_bits_update;
ushort DAT_0001f0fe;
byte DAT_00012d70;
byte UNK_00012d71;
byte UNK_00012d72;
byte UNK_00012d73;
undefined2 g_coolant_temp_f1e8;
undefined2 g_tps_load_axis_f110;
ushort DAT_00010e70;
ushort DAT_00010e72;
ushort DAT_00010e74;
ushort DAT_00010e76;
ushort DAT_00010e78;
ushort DAT_00010e7a;
ushort DAT_00010e82;
ushort DAT_00010e84;
ushort DAT_00010e7e;
ushort DAT_00010e80;
short DAT_00010e86;
short DAT_0001eff6;
char DAT_000102c7;
undefined2 DAT_0001eff8;
undefined2 DAT_0001eff4;
short DAT_0001eff4;
short DAT_0001eff8;
short DAT_00010e6a;
short DAT_00010e6c;
ushort DAT_00010e8a;
undefined2 g_radfan_stage_hysteresis_flags_f492;
ushort DAT_00010e8c;
ushort DAT_00010e8e;
ushort DAT_00010e90;
ushort DAT_00010e92;
ushort DAT_00010e94;
ushort DAT_00010e96;
ushort DAT_00010e98;
char DAT_000102ed;
ushort DAT_00010e10;
ushort DAT_00010e18;
ushort DAT_00010e14;
undefined2 g_tps_load_trimmed_f13e;
ushort DAT_00010e12;
ushort DAT_00010e1a;
ushort DAT_00010e16;
ushort DAT_00010e1c;
short DAT_00010e1e;
short DAT_0001efee;
undefined ign_advance_enable_flags_f440_update;
undefined ign_advance_idle_correction_f44e;
undefined2 DAT_0001f1e0;
ushort DAT_00010e00;
ushort DAT_0001eec8;
undefined DAT_00010dfa;
char DAT_000102f0;
byte[2] DAT_00010e08;
byte[2] DAT_00010e06;
byte[2] DAT_00010e04;
byte[2] BYTE_ARRAY_00010e02;
ushort DAT_0001f200;
ushort DAT_00010df4;
undefined2 DAT_00010df6;
undefined2 DAT_0001f040;
byte[2] Octane_Below_to_Disable_Boost_Control;
byte[2] Octane_Above_to_Enable_Boost_Control;
byte[2] DAT_0001f44a;
byte[2] BYTE_ARRAY_00010df0;
byte[2] DAT_0001f05c;
ushort DAT_0001f196;
short DAT_0001f44e;
byte[2] BYTE_ARRAY_00010e0a;
short DAT_0001f44a;
short DAT_0001f000;
short DAT_00010df8;
undefined2 DAT_00010dee;
undefined2 DAT_0001f450;
ushort DAT_00010d7c;
ushort DAT_00010d80;
ushort DAT_00010d7a;
ushort DAT_00010d7e;
undefined2 DAT_0000f43e;
short DAT_00010d78;
short DAT_00010d74;
short DAT_00010d76;
ushort DAT_0001f26e;
ushort DAT_00010dda;
undefined2 g_status_flags_f21c;
short DAT_00010ddc;
char DAT_000102ea;
ushort DAT_0001f4d6;
ushort DAT_0001f3f6;
ushort DAT_00010e44;
short DAT_00010e46;
short DAT_0001efec;
ushort DAT_0001f18e;
ushort DAT_00010e4a;
ushort DAT_00010e4c;
undefined2 g_purge_enable_cond_f4a4;
undefined2 tcu_rx_confirmed_byte2_f4c0;
undefined2 g_purge_enable_cond_f4a2;
ushort DAT_0001f4ac;
undefined2 tcu_rx_confirmed_byte1_f4be;
short DAT_0001eff0;
ushort DAT_00010e5e;
undefined1 DAT_000102c8;
undefined2 g_diag_flags_eed6;
ushort DAT_0001eed8;
ushort DAT_0001f4b4;
ushort DAT_00010e40;
ushort DAT_00010e42;
ushort DAT_0001f4b6;
undefined2 DAT_0001f4b8;
char DAT_0001031f;
undefined2 DAT_0001f196;
undefined2 DAT_0001f198;
undefined2 g_status_flags_f1f0;
undefined2 g_diag_flags_eeda;
undefined2 inj_pw_output_f18c;
char DAT_000102e9;
ushort DAT_00010eac;
ushort DAT_00010eae;
ushort DAT_00010eaa;
ushort DAT_00010eb0;
short DAT_00010ea8;
ushort DAT_00010ea2;
ushort DAT_00010eb6;
ushort DAT_00010eb8;
ushort DAT_00010eb4;
ushort DAT_00010eba;
short DAT_00010eb2;
ushort DAT_00010ea0;
ushort DAT_0001ee8a;
undefined2 DAT_0001f4dc;
undefined o2_heater_duty_downstream_correction;
undefined2 DAT_0000f0ec;
undefined2 sensor_validity_status_flags_f1fe;
short DAT_0001f4e6;
short DAT_00010f08;
short DAT_0001f088;
ushort DAT_00010eea;
ushort DAT_00010ee8;
ushort DAT_00010ee6;
ushort DAT_0001f4de;
ushort DAT_00010ef4;
undefined2 DAT_00010ef0;
ushort DAT_00010ef2;
undefined2 DAT_00010eee;
ushort DAT_0001f4d8;
ushort DAT_00010f04;
ushort DAT_00010f02;
ushort DAT_00010f00;
ushort DAT_00010efe;
char DAT_000102ec;
ushort DAT_00010f46;
ushort DAT_00010f48;
ushort DAT_00010f42;
short DAT_0001f046;
undefined o2_heater_force_off_check;
short DAT_0001f4da;
short DAT_00010efa;
undefined2 DAT_0000f4f4;
ushort DAT_00010f06;
short DAT_00010efc;
byte DAT_000102eb;
undefined2 DAT_0000f520;
undefined2 DAT_0000f5c0;
short DAT_0001f1fc;
undefined2 DAT_0000f5c2;
undefined2 DAT_0000f5c4;
undefined2 DAT_0000f5c6;
undefined eeprom_backup_table_write_dispatch;
undefined mut_configurable_reqid_backup_snapshot_5word;
undefined mut_configurable_reqid_periodic_snapshot_8word;
undefined channel_validity_gated_handler_dispatch;
ushort DAT_0001f84a;
char DAT_00010201;
undefined2 DAT_0001f5b4;
undefined2 DAT_0001f5b8;
undefined div_u16_rounded;
undefined1 cal_o2_sensor_type;
ushort DAT_0001f5b8;
ushort DAT_0001f5e8;
ushort DAT_0001f5b4;
undefined mul_fixedpoint_8;
char DAT_00010202;
ushort DAT_0001f5b6;
char DAT_00010204;
undefined2 DAT_0001f5b6;
undefined dtc_status_flags_f204_f206_build;
undefined tcu_shift_flags_dispatch;
short DAT_0001f250;
ushort DAT_0001f730;
ushort DAT_0001f3dc;
ushort DAT_0001f202;
undefined2 g_status_flags_001f25a;
undefined2 engine_state_flags_f25e;
ushort DAT_00010fd8;
ushort DAT_00010fda;
short DAT_0001f00e;
ushort DAT_00010fd4;
ushort DAT_00010fd6;
ushort DAT_00010fd0;
ushort DAT_0001f11c;
ushort DAT_00010fd2;
short DAT_0001f500;
ushort DAT_0001f204;
ushort DAT_0001f206;
undefined LAB_0000f290;
undefined DAT_0001f20a;
byte DAT_0001fec9;
undefined2 DAT_0001f218;
undefined DAT_0000fd00;
undefined P1DDR;
undefined P3DDR;
char DAT_0002fefa;
char DAT_0002fefb;
char DAT_0002fefc;
char DAT_0002fefd;
char DAT_0002fefe;
char DAT_0002feff;
undefined2 DAT_0000f394;
undefined DAT_0001f200;
undefined DAT_0001f202;
undefined DAT_0001f204;
undefined DAT_0001f206;
undefined DAT_0001f208;
short DAT_0001f214;
undefined1 DAT_0001f274;
undefined2 DAT_0001f270;
uchar DAT_0001fecb;
short DAT_0001f218;
uchar *DAT_0001f218;
char DAT_0001f26c;
undefined2 DAT_0001f268;
undefined1 DAT_0001f26d;
undefined2 DAT_0001f26a;
undefined DAT_0000f21a;
undefined1 DAT_0000f8c1;
char DAT_0001f8a5;
undefined1 DAT_0000f8a0;
undefined DAT_0000f8bb;
byte DAT_0001f27c;
undefined1 DAT_0001f27d;
undefined1 DAT_0001f281;
undefined2 DAT_0001f276;
undefined2 DAT_0001f278;
undefined1 DAT_0001f27a;
undefined2 DAT_0001f26e;
undefined2 DAT_0001f272;
undefined1 DAT_0001f275;
undefined UNK_0000f260;
byte DAT_0001f27e;
short DAT_0001f278;
byte DAT_0001f27a;
char DAT_0001f27d;
char DAT_0001f27a;
ushort DAT_0001f276;
ushort DAT_0001f278;
short DAT_0001f26e;
char DAT_0001f27c;
byte DAT_0001f281;
byte DAT_0001f274;
byte *DAT_0001f270;
byte DAT_0001f275;
byte *DAT_0001f272;
undefined2 DAT_0001fee2;
undefined DAT_0000fc74;
ushort DAT_0001fee2;
undefined status_word_consumer_periodic;
undefined periodic_task_dispatcher;
undefined engine_warmup_periodic_dispatch;
undefined subsystem_unconditional_init_chain;
undefined toggle_flag_fe8b_bit4;
ushort DAT_00010c72;
undefined2 g_knock_octane_trim_ee98;
ushort DAT_00010c70;
ushort DAT_00010912;
ushort DAT_00010910;
undefined swap_invert_high_byte;
ushort DAT_0001ee9a;
ushort DAT_0001ee9c;
ushort DAT_0001eeca;
ushort DAT_0001eecc;
ushort DAT_0001eece;
undefined2 DAT_0001eed4;
undefined throttle_idle_state_cold_init;
undefined ecu_openloop_state_reset;
undefined f2f0_f2f8_ring_and_f27x_f2cc_periodic_init;
undefined adc_read_sequence_main;
undefined adc_read_sequence_b;
undefined adc_read_channel_6;
undefined compose_status_word_f102;
undefined compose_status_words_f0f6_fc;
undefined timer4_oc_arm_from_flags;
undefined2 DAT_0001ee82;
undefined2 DAT_00010f68;
undefined2 DAT_0001ee8a;
undefined2 DAT_0001ef6c;
undefined compose_status_word_f0f8;
undefined2 DAT_0001f10a;
undefined check_flag_fe96_bit7_clear;
undefined2 DAT_0001f10c;
undefined table_select_scale_fanout_f1be_f1c2;
undefined cond_reset_and_latch_f10e_f118_prev;
undefined periodic_status_compose_and_stall_check;
undefined sensor_adc_update;
undefined throttle_body_update;
undefined rpm_load_zone_counter_update;
undefined2 DAT_0000f1f0;
undefined2 DAT_0000f1f2;
undefined2 DAT_0000f1f4;
undefined2 DAT_0000f1f6;
undefined2 DAT_0000f1f8;
undefined2 DAT_0000f1fc;
undefined2 DAT_0001ef70;
undefined fuel_cut_rpm_offset_select;
undefined fuel_cut_enable_check;
char DAT_000102c6;
char DAT_000102e5;
ushort DAT_0001ef6c;
ushort DAT_00010bf8;
ushort DAT_00010bfc;
ushort DAT_00010dc8;
ushort DAT_00010dca;
ushort DAT_00010bfa;
ushort DAT_0001f436;
ushort DAT_00010d30;
ushort DAT_00010d2e;
undefined2 DAT_00010d3a;
undefined2 DAT_0001f066;
ushort DAT_0001f4e8;
undefined1 cal_fuel_cut_enable_flag;
short DAT_00010d34;
short DAT_00010bfe;
undefined2 DAT_0001ef6e;
undefined map_tps_axis_update;
undefined iac_position_integrator;
ushort DAT_0001f108;
ushort DAT_00010f5c;
ushort DAT_00010f5a;
undefined tps_range_clamp_update;
ushort DAT_0001f10a;
short DAT_00010f66;
undefined2 g_rpm_load_zone_flags_f208;
undefined2 DAT_0000f10c;
undefined2 DAT_0000f20e;
undefined2 g_axis_lookup_input_scratch_f0ba;
ushort DAT_00010f6c;
ushort DAT_00010f6a;
undefined2 g_iat_raw_adc_f116;
undefined coolant_temp_range_check;
ushort DAT_000109c6;
short DAT_000109c8;
short DAT_0001ef92;
ushort DAT_0001f12e;
ushort DAT_000109c4;
ushort DAT_000109c2;
undefined2 mut_battery_voltage_f13a;
ushort DAT_00010abc;
ushort DAT_00010abe;
short DAT_0001f068;
ushort DAT_00010ac0;
short DAT_00010ac2;
ushort DAT_00010f70;
ushort DAT_00010f72;
undefined2 DAT_0000f150;
undefined2 DAT_0000f152;
undefined2 DAT_0000f154;
undefined2 g_tps_load_current_f13c;
undefined2 DAT_0000f14c;
undefined2 DAT_0000f14e;
undefined2 isc_stepper_position_eed0;
undefined2 DAT_00010cce;
undefined2 DAT_00010ccc;
ushort DAT_00010cc0;
ushort DAT_0001f14c;
ushort DAT_00010cc6;
ushort DAT_00010cc4;
ushort DAT_00010cc2;
ushort DAT_00010cca;
undefined2 g_idle_target_rpm_f400;
undefined2 g_idle_rpm_actual_f178;
undefined2 g_status_flags_f3f0;
ushort DAT_0001eee2;
char DAT_0001030b;
undefined engine_torque_scale_state_reset;
undefined engine_torque_pct_scale_calc;
undefined2 DAT_0000f170;
undefined2 DAT_0000f172;
undefined2 DAT_0000f182;
undefined2 DAT_0000f174;
undefined2 DAT_0000f178;
undefined2 DAT_0000f17a;
undefined2 candidate_rpm_period_hist_newest;
undefined2 candidate_rpm_axis_bkpt_lo;
undefined2 DAT_0000f17c;
undefined2 DAT_0000f17e;
undefined div_s32_s16_rounded;
undefined2 DAT_0000f5cc;
ushort DAT_000109dc;
undefined2 DAT_0001f174;
undefined2 DAT_0001f17e;
ushort DAT_0001f172;
undefined2 g_throttle_body_pos_target_f170;
undefined2 DAT_00010aec;
undefined2 DAT_0001f184;
undefined2 DAT_00010aea;
undefined o2_wideband_lsu_mode_check;
undefined lambda_correction_output_calc;
short DAT_00010f56;
short DAT_0001ef96;
undefined2 DAT_0000f5e0;
undefined2 DAT_0001f1de;
undefined2 DAT_0000f1b6;
undefined fueling_accel_enrichment_calc;
undefined fueling_openloop_output_copy;
undefined fueling_throttle_zone_flags_update;
undefined fueling_inj_target_select;
undefined fueling_inj_accum_clamp_drain;
undefined fueling_inj_pw_calc;
undefined div_u32_u16_rounded;
short DAT_0001f29a;
short DAT_0001f298;
undefined2 DAT_0001f1c0;
ushort DAT_0001081a;
undefined identity_passthrough_arg1;
undefined2 DAT_0000f1c2;
undefined2 DAT_0000f1c4;
undefined2 DAT_0000f1c6;
undefined2 DAT_0000f1bc;
undefined2 DAT_0000f1be;
ushort DAT_00010a5e;
ushort DAT_00010a5c;
undefined2 g_status_flags_f224;
ushort DAT_00010a60;
undefined2 DAT_0000f224;
undefined2 DAT_0000f012;
undefined2 DAT_0000f014;
ushort DAT_00010aa4;
byte[2] DAT_00010da2;
byte[2] DAT_0001f1b4;
byte[2] BYTE_ARRAY_0001080c;
undefined2 g_status_flags_f226;
byte[2] DAT_0001080e;
char DAT_00010306;
byte[2] DAT_00010958;
undefined2 DAT_0000f1ca;
byte[2] BYTE_ARRAY_00010810;
undefined inc_if_signed_flag;
undefined2 fuel_pw_composite_f1b6;
undefined2 DAT_0001f1e4;
ushort DAT_0001089a;
ushort DAT_0001089c;
ushort DAT_0001089e;
undefined decrement_counters_in_range;
undefined2 g_status_flags_f20a;
undefined2 DAT_0001ee96;
undefined2 DAT_0001ee94;
undefined2 DAT_0001ee92;
undefined2 DAT_0001ee90;
undefined2 DAT_0001ee8e;
undefined2 DAT_0001ee8c;
undefined2 DAT_0001f228;
undefined post_start_enrich_timer_update;
undefined f33e_bit0_update_from_rpm_load_window;
undefined f216_low_nibble_update_table_1cc3;
undefined f218_shift_in_f216_low_nibble;
undefined f216_bit8_update_from_recovery_chain;
undefined tcu_link_confirmed_flag_f226_bit7_update;
undefined2 DAT_0001f212;
undefined2 DAT_0001f210;
undefined2 DAT_0001f214;
undefined2 DAT_0001f220;
undefined2 DAT_0001f21e;
undefined2 DAT_0001f222;
ushort DAT_0001ef54;
undefined2 DAT_000113d2;
undefined2 DAT_0001f0b6;
char DAT_000102c5;
ushort DAT_00010cf0;
ushort DAT_00010cee;
undefined rpm_load_window_condition_check;
ushort DAT_00010b70;
ushort DAT_00010b76;
ushort DAT_00010b74;
undefined2 DAT_0000f216;
undefined2 DAT_0000f1de;
ushort DAT_0001f218;
undefined stub_zero_0x22193;
ushort DAT_0001f242;
ushort DAT_00010d92;
ushort DAT_00010d90;
ushort DAT_00010d8e;
ushort DAT_00010d9a;
short DAT_00010d9e;
ushort DAT_00010d94;
ushort DAT_00010d96;
ushort DAT_00010d9c;
undefined2 g_ef9e_latch_value;
undefined2 g_efa0_max_hold_override_cal;
undefined2 g_efa0_max_hold_value;
char DAT_000102fd;
undefined2 tcu_rx_confirmed_byte3_f4c2;
char DAT_00010305;
ushort DAT_000113ce;
char DAT_000102cb;
ushort DAT_00010a56;
ushort DAT_00010a54;
undefined1 DAT_00011ce0;
undefined1 DAT_00011cde;
undefined check_flag_f0f8_bit2_clear;
byte DAT_000102dd;
undefined2 DAT_00010ee4;
undefined2 DAT_0001effa;
ushort DAT_00010ee0;
undefined2 g_post_start_enrich_timer;
undefined2 g_post_start_enrich_timer_secondary;
undefined2 g_post_start_enrich_timer_reload_cal;
ushort DAT_00010a74;
ushort DAT_00010a72;
ushort DAT_00010a76;
undefined cranking_mode_bit_update;
undefined warmup_mode_state_machine;
undefined f20e_f210_clear_chain_and_o2_closedloop_gate;
ushort DAT_0001f212;
short DAT_0001efa2;
undefined cranking_detect_check;
ushort DAT_00010834;
ushort DAT_00010864;
ushort DAT_00010828;
ushort DAT_00010824;
short DAT_0001082c;
undefined closedloop_fuel_ready_check;
undefined warmup_ign_advance_correction_calc;
undefined f216_bits_f188_f17a_update_and_warmup_gate;
undefined warmup_complete_gate_check;
char DAT_000102cc;
ushort DAT_00010a52;
undefined2 DAT_0001ef74;
short DAT_0001f076;
ushort DAT_00010a46;
ushort DAT_00010a42;
byte[2] BYTE_ARRAY_00010886;
ushort DAT_00010896;
ushort DAT_0001f5ee;
ushort DAT_0001f190;
byte[2] DAT_0001088a;
byte[2] BYTE_ARRAY_00010888;
byte[2] DAT_00010892;
char DAT_000102ca;
ushort DAT_0001088c;
short DAT_0001f018;
short DAT_0001f01a;
short DAT_00010890;
short DAT_0001088e;
ushort DAT_00010892;
undefined2 DAT_000132b6;
undefined2 g_status_flags_f25a;
undefined2 DAT_0000f212;
undefined2 DAT_0000f0f8;
ushort DAT_0001f188;
ushort DAT_0001087a;
ushort DAT_0001087c;
ushort DAT_000108d0;
ushort DAT_000108ce;
short DAT_00010876;
short DAT_0001f026;
char DAT_000102c9;
short DAT_0001086a;
short DAT_0001f23c;
undefined f240_counter_reset_and_f074_table_load;
undefined f21c_bit12_update_dispatcher;
undefined f224_bits01_update_and_efa8_edge_latch;
undefined2 DAT_000108b0;
undefined2 DAT_0001f07e;
ushort DAT_000108b2;
ushort DAT_0001f23e;
undefined2 DAT_0001f240;
undefined2 DAT_0001f0b0;
undefined2 DAT_0001f0ae;
undefined f21c_bit9_hysteresis_and_f23e_table_ramp;
undefined f21c_bit8_hysteresis_from_table_variant;
undefined f21c_bit12_source_condition_check;
ushort DAT_000108aa;
ushort DAT_0001f340;
short DAT_0001f074;
short DAT_0001efa6;
ushort DAT_000108e4;
ushort DAT_000108e6;
undefined2 g_f224_bit1_edge_latch_value_efa8;
short DAT_000108a2;
ushort DAT_00010898;
undefined2 DAT_000108a0;
short DAT_0001efa8;
undefined2 DAT_000108e0;
undefined2 DAT_000108e2;
byte[2] BYTE_ARRAY_000108a4;
byte[2] DAT_0001150e;
short DAT_0001f022;
char DAT_0001031d;
short DAT_0001147c;
short DAT_0001f024;
short DAT_000108a8;
short DAT_00011478;
short DAT_000108a6;
short DAT_0001f01e;
ushort DAT_0001f122;
ushort DAT_0001147a;
undefined2 DAT_0001eeb2;
undefined2 DAT_0001eeb0;
undefined2 DAT_0001eeae;
undefined2 DAT_0001ee9c;
undefined2 DAT_0001ee9a;
undefined2 DAT_00010956;
undefined2 DAT_0001f2f8;
undefined2 DAT_0001f2f2;
undefined2 DAT_0001f2f0;
undefined2 DAT_0001f2f6;
undefined2 DAT_0001f2f4;
byte DAT_000131e4;
byte DAT_000131d8;
undefined2 DAT_0000f2cc;
byte DAT_00011df0;
undefined2 DAT_0000f276;
undefined2 DAT_0000f278;
undefined2 DAT_0000f27a;
undefined2 DAT_0000f27c;
undefined f0ba_f2d0_o2_mode7_8_correction_calc;
undefined f25e_gate_chain_dispatch_b;
undefined f2a0_f2c6_dual_timer_decay_and_f2c8_blend_dispatch;
undefined f2ca_table_override_from_f114_gate;
undefined f25e_gate_chain_dispatch_a;
undefined knock_octane_dual_axis_blend_far_trampoline;
undefined f07c_multi_channel_threshold_gate_check;
undefined f25e_bit10_update_from_f216_gate;
undefined status_word_bit3_conditional_update_via_table;
undefined f25e_bit14_update_from_f186_f17a_gate;
ushort DAT_00010d84;
ushort DAT_00010d82;
ushort DAT_00010d86;
ushort DAT_00010d88;
short DAT_00010d8a;
short DAT_0001f07c;
undefined f216_bit5_hysteresis_gate_check;
ushort DAT_000109ce;
ushort DAT_000109cc;
ushort DAT_000109ca;
undefined2 DAT_0001f2b2;
char DAT_000102f5;
short DAT_00010d60;
ushort DAT_00010d58;
ushort DAT_00010d5a;
ushort DAT_000109ee;
ushort DAT_000109f0;
ushort DAT_000109ec;
ushort DAT_00010b4c;
ushort DAT_00010b4a;
undefined knock_octane_dual_axis_table_blend_f2a8;
undefined2 DAT_0001f2a2;
char DAT_00010309;
ushort DAT_0001f2a0;
ushort DAT_0001083a;
undefined2 DAT_00010842;
ushort DAT_00010846;
ushort DAT_0001083c;
undefined2 DAT_0001f2c2;
undefined2 g_efba_timer_target;
undefined2 DAT_00010844;
undefined2 DAT_0001f2c0;
ushort DAT_0001f2c6;
ushort DAT_00010848;
undefined2 DAT_0001084c;
ushort DAT_00010850;
undefined2 DAT_0001084e;
ushort DAT_0001084a;
undefined2 DAT_0001f2c4;
undefined2 g_efbc_timer_target;
char DAT_000102ce;
ushort DAT_000109da;
ushort DAT_000109de;
ushort DAT_0001f174;
ushort DAT_000109d8;
undefined2 DAT_0000f29e;
char DAT_0001031e;
ushort DAT_00011482;
ushort DAT_0001f11e;
ushort DAT_000109d6;
ushort DAT_000109d4;
ushort DAT_000109d2;
undefined2 DAT_0001f2a8;
byte[2] BYTE_ARRAY_000109e6;
byte[2] BYTE_ARRAY_000109e4;
undefined2 DAT_0001f28e;
undefined knock_octane_trim_ramp_update;
short DAT_000108d2;
short DAT_0001efb0;
byte DAT_00011d5c;
byte DAT_00011d5d;
byte DAT_00011d5e;
byte DAT_00011d5f;
byte DAT_0001321a;
byte DAT_0001321b;
undefined2 map_tps_derived_threshold_f10e;
undefined calc_staged_index_f0f8;
ushort DAT_000108b8;
ushort DAT_000108ba;
ushort DAT_000108bc;
ushort DAT_000108be;
ushort DAT_000108c0;
ushort DAT_000108c2;
ushort DAT_000108fa;
ushort DAT_00010902;
ushort DAT_000108fe;
short DAT_0001f034;
ushort UNK_00010906;
ushort UNK_00010904;
short DAT_0001f2dc;
ushort DAT_0001f2f8;
undefined f27e_table_indexed_clamp_compute_via_table;
undefined f276_f27a_dual_channel_compute_via_table;
undefined f26e_o2_closed_loop_output_blend_via_table;
undefined o2_closed_loop_mode_selector_via_table;
byte[2] DAT_000108cc;
ushort DAT_0001147e;
undefined2 DAT_00011480;
ushort DAT_000108d6;
ushort DAT_000108d4;
char DAT_0001030c;
undefined eeac_adaptive_trim_learn_update_via_table;
undefined f1fe_bit3_clear_and_f1f2_bit4_set_check;
short DAT_0001efae;
ushort DAT_0001090c;
ushort DAT_0001090e;
undefined muldiv_u16_shr8_sat;
undefined TableHeader_0002d440;
undefined f2b4_threshold_table_lookup;
undefined2 DAT_0000f2d6;
undefined2 DAT_0000f2d8;
undefined2 DAT_00010838;
undefined2 DAT_0001f07a;
undefined2 DAT_0000f2b4;
undefined2 DAT_0000f250;
undefined2 DAT_0000f970;
undefined2 DAT_0000f252;
undefined2 DAT_0000f972;
undefined2 DAT_0000f254;
undefined2 DAT_0000f24e;
undefined muldiv_s16_rounded_3op;
undefined2 g_f2ea_correction_calc_gate_ref_efb2;
undefined channel_enable_mask_build_and_dispatch_ff2x_ff3x;
ushort DAT_00010aa2;
ushort DAT_00010852;
undefined DAT_0000fffd;
undefined2 DAT_0000f25e;
undefined f2cc_offset_accumulate;
undefined2 g_f31c_toggle_pulse;
undefined2 DAT_0000f280;
undefined2 DAT_0001f2d2;
undefined2 DAT_0000f282;
undefined2 DAT_0001f2d0;
undefined2 DAT_0000f284;
undefined2 DAT_0000f266;
undefined2 DAT_0000f286;
undefined2 DAT_0000f268;
undefined2 DAT_0000f288;
undefined2 DAT_0000f488;
undefined2 DAT_0000f26a;
undefined2 DAT_0000f28a;
undefined2 DAT_0000f26c;
undefined2 DAT_0000f28c;
undefined2 DAT_0000f48c;
undefined2 DAT_0000f26e;
undefined2 DAT_0000f2d4;
undefined2 DAT_0000f756;
undefined2 DAT_0000f25c;
undefined2 DAT_0000f27e;
ushort DAT_0001f270;
undefined2 DAT_0000f380;
undefined2 DAT_0001f350;
undefined2 DAT_00010b5e;
undefined2 DAT_0001efbe;
byte DAT_0001280c;
byte DAT_00012828;
undefined2 DAT_0000f37e;
undefined update_flag_fe97_bit2_cond;
undefined2 DAT_0000f3a4;
undefined2 DAT_0000f3aa;
undefined2 DAT_0000f3ac;
undefined2 DAT_0000f3ae;
undefined octane_level_gate_and_reset_dispatch;
undefined isc_f342_composite_correction_calc;
undefined2 DAT_0001f18a;
undefined2 DAT_0001f18e;
undefined2 DAT_0000f200;
undefined tcu_slip_and_ff54_reinit_on_f20e_bit4;
undefined per_cyl_knock_flag_update_cyl_a;
undefined2 DAT_0000f3a2;
undefined f3a4_bit10_update_dispatch;
undefined knock_counter_condition_override_eec6;
undefined octane_level_compute_from_knock_counter;
undefined efc2_threshold_update;
undefined2 g_knock_event_counter_eec6;
undefined2 g_octane_level_mut_0x27;
undefined tcu_slip_duty_gain_condition_check;
ushort DAT_00010b06;
undefined2 g_status_flags_f33e;
undefined2 g_f3a2_hold_min_timer_ef88;
undefined f0ec_bit1_threshold_gate_check;
ushort DAT_00010f88;
ushort DAT_00010f86;
ushort DAT_00010f84;
undefined2 DAT_00010b58;
undefined2 DAT_0001f3ba;
ushort DAT_00010b56;
ushort DAT_00010b54;
undefined2 DAT_00010b50;
undefined2 DAT_0001f3bc;
undefined2 knock_per_cyl_flags_f3a4;
undefined2 DAT_00010b52;
ushort DAT_00010b5c;
ushort DAT_00010b5a;
ushort DAT_00010b30;
ushort DAT_00010b34;
ushort DAT_00010b3c;
short DAT_00010b5e;
short DAT_0001efbe;
undefined2 DAT_0001eec6;
undefined2 DAT_0001f200;
undefined o2_downstream_clamp_by_type;
ushort DAT_0001f17e;
ushort DAT_00010b22;
undefined2 g_delta_threshold_calib_load_efc2;
undefined f33e_bit12_clear;
char DAT_000102d4;
undefined f37a_correction_calc_from_f33e_bit2_or_f378;
undefined2 DAT_0000f378;
ushort DAT_00010bac;
ushort DAT_00010bb2;
ushort DAT_00010bae;
ushort DAT_00010bb0;
ushort DAT_00010bb4;
ushort DAT_00010bb6;
ushort DAT_00010bb8;
ushort DAT_00010bba;
short DAT_0001f378;
char DAT_000102d1;
ushort DAT_00010bc6;
ushort DAT_00010bc0;
ushort DAT_00010bc2;
ushort DAT_00010bc4;
ushort DAT_00010ba2;
ushort DAT_00010ba4;
ushort DAT_00010ba8;
ushort DAT_00010ba6;
undefined PTR_vec_trapa4_00010b9e;
ushort DAT_00010b7a;
ushort DAT_00010b7c;
ushort DAT_00010b82;
ushort DAT_00010b7e;
ushort DAT_00010b80;
ushort DAT_00010af0;
ushort DAT_00010aee;
undefined isc_f358_correction_calc;
undefined isc_f35a_correction_calc;
undefined isc_f374_correction_calc;
undefined isc_f36e_f370_target_calc;
undefined isc_f36a_f36c_target_calc;
undefined isc_f37c_ramp_target_calc;
undefined isc_f352_table_lookup;
undefined isc_f354_table_lookup_conditional;
undefined isc_f4d6_gated_offset_calc;
undefined isc_f35e_correction_calc;
ushort DAT_00010f6e;
char DAT_00010304;
ushort DAT_0001f194;
undefined2 DAT_0001f3ee;
short DAT_000113d8;
short DAT_0001f05a;
short DAT_000113dc;
short DAT_0001f030;
undefined2 DAT_000113da;
undefined isc_f4d6_gate_condition_check;
short DAT_0001f374;
ushort DAT_00010e48;
ushort DAT_00010b02;
ushort DAT_000109ba;
ushort DAT_00010b00;
undefined2 DAT_0000f36a;
undefined2 DAT_0000f36c;
short DAT_00010bda;
ushort DAT_00010bdc;
ushort DAT_00010bde;
ushort DAT_00010be0;
short DAT_00010be2;
ushort DAT_0001ef72;
short DAT_00010be6;
ushort DAT_00010be4;
undefined2 g_isc_ramp_direction_flag;
undefined f33e_bit15_hysteresis_update;
ushort DAT_000114ba;
ushort DAT_000114bc;
undefined2 DAT_00010af2;
ushort DAT_0001f182;
undefined o2_sensor2_mode3_midpoint;
char DAT_000102d3;
ushort DAT_00010b8c;
ushort DAT_00010b8a;
ushort DAT_00010b8e;
ushort DAT_0001f376;
undefined2 DAT_00010e62;
undefined2 DAT_00010e64;
undefined2 DAT_0000f376;
char DAT_000102d5;
ushort DAT_00010bca;
ushort DAT_00010bc8;
ushort DAT_00010bce;
ushort DAT_00010bcc;
ushort DAT_00010bd0;
undefined2 DAT_0000f36e;
undefined2 DAT_0000f370;
undefined2 DAT_0001f342;
byte[2] BYTE_ARRAY_00010b1a;
undefined o2_wideband_mode82_check;
ushort DAT_0001f5ca;
ushort DAT_00010adc;
undefined2 DAT_0001f364;
undefined DAT_0000ffec;
undefined2 DAT_00010c74;
undefined2 DAT_0001eece;
undefined2 DAT_0001eecc;
undefined2 DAT_0001eeca;
undefined2 DAT_0001eed2;
ushort DAT_0001eed4;
ushort DAT_00010cbc;
undefined o2_downstream_mode17_check;
undefined isc_f406_clamp_dispatch;
undefined mirror_f406_f408_to_f40a_f40c;
undefined isc_f422_correction_calc;
undefined isc_f42c_correction_calc;
undefined isc_f416_f438_reset_on_f20e_bit4;
undefined idle_target_rpm_compute_f400;
undefined isc_f426_f03a_decay_calc;
undefined f4de_f4e2_octane_correction_calc;
undefined2 DAT_0001f436;
undefined2 DAT_00010c86;
undefined2 DAT_0001efe2;
undefined2 DAT_0001f43a;
undefined f3fa_f3fe_target_select_by_f0f8_bit5;
undefined2 DAT_00010c64;
undefined2 DAT_00010c66;
undefined2 DAT_00010c68;
undefined2 DAT_0001f3fa;
undefined2 DAT_0001f3fe;
undefined2 DAT_0001f3fc;
undefined isc_f41a_correction_calc;
ushort DAT_00010d68;
ushort DAT_00010d62;
ushort DAT_00010d66;
undefined2 DAT_0001f41a;
undefined2 DAT_00010d64;
TableHeader hdr_map_tps_axis;
undefined2 isc_state_flags_f3f4;
char DAT_000102da;
ushort DAT_0001ef76;
ushort DAT_00010c1a;
ushort DAT_00010c12;
ushort DAT_00010c14;
ushort DAT_00010c0e;
ushort DAT_0001efda;
ushort DAT_00010c0a;
ushort DAT_00010c0c;
ushort DAT_0001f42e;
ushort DAT_00010c1c;
ushort DAT_00010c1e;
ushort DAT_00010c10;
ushort DAT_0001f03a;
short DAT_0001f426;
ushort DAT_00010d1a;
ushort DAT_00010d18;
ushort DAT_0001f180;
undefined2 DAT_0001f4de;
undefined2 DAT_00010c36;
undefined update_max;
ushort DAT_0001efd4;
ushort DAT_00010d56;
undefined2 DAT_00010d52;
undefined2 DAT_00010d54;
undefined2 DAT_0001f40e;
short DAT_0001f424;
short DAT_0001f036;
undefined stub_empty_0x26b8b;
undefined isc_f416_f438_correction_calc;
undefined f3f4_bit7_update_dispatch;
ushort DAT_00010c92;
ushort DAT_00010c90;
ushort DAT_00010c96;
ushort DAT_00010c94;
ushort DAT_00010c9a;
ushort DAT_00010c98;
undefined2 DAT_0001efc6;
undefined f0f8_bit_based_constant_select;
ushort DAT_00010c78;
ushort DAT_00010c7a;
ushort DAT_00010c7c;
ushort DAT_00010c7e;
undefined efc8_zero_condition_check;
ushort DAT_00010d46;
ushort DAT_00010d48;
short DAT_00010d4a;
short DAT_0001efc8;
ushort DAT_00010c76;
ushort DAT_00010e66;
ushort DAT_00010ca2;
char DAT_000102d7;
short DAT_00010c2e;
short DAT_0001efd2;
ushort DAT_00010c28;
undefined2 DAT_00010c2a;
undefined2 DAT_0001f428;
char DAT_000102d8;
undefined2 DAT_0001f422;
undefined2 g_status_flags_f3f2;
ushort DAT_00010cfc;
ushort DAT_00010cfe;
ushort DAT_00010cf8;
ushort DAT_00010cfa;
ushort DAT_00010d00;
char DAT_000102d9;
ushort DAT_00010d06;
ushort DAT_00010d08;
ushort DAT_00010d0a;
ushort DAT_00010d0c;
short DAT_0001f084;
short DAT_00010d0e;
short DAT_00010cb0;
short DAT_0001efcc;
ushort DAT_00010cac;
ushort DAT_00010caa;
ushort DAT_00010cb2;
undefined2 DAT_0001efce;
ushort DAT_00010c26;
ushort DAT_0001f048;
ushort DAT_00010d22;
short DAT_0001efe0;
short DAT_00010d1c;
char DAT_000102f7;
undefined2 DAT_00010d1e;
undefined2 DAT_0001f434;
undefined isc_stepper_state_reinit_and_target_select;
undefined isc_stepper_mode_state_reload_b;
undefined isc_stepper_mode_state_reload_c;
undefined isc_stepper_mode_state_reload_d;
undefined isc_efd0_gate_condition_check;
ushort DAT_00010cb8;
undefined2 DAT_00010c2c;
undefined2 DAT_0000f40a;
short DAT_00010d44;
short DAT_0001efd0;
undefined2 DAT_00010c40;
undefined isc_stepper_trim_dispatch;
undefined isc_stepper_correction_dispatch_b;
undefined isc_f408_composite_dispatch;
ushort DAT_0001f40a;
undefined isc_f402_stepper_target_calc;
undefined f414_trim_calc_gated_by_f0f8_f3f0;
undefined2 DAT_0001f414;
ushort DAT_0001f414;
ushort DAT_0001f40c;
short DAT_0001efce;
ushort DAT_00010c52;
ushort DAT_00010c54;
short DAT_0001f402;
short DAT_00010c46;
short DAT_0001efd4;
short DAT_0001f422;
short DAT_0001efd6;
short DAT_0001efd8;
ushort DAT_00010c4a;
undefined isc_f402_composite_correction_calc;
undefined isc_f3fa_f3fe_dispatch;
undefined isc_f41c_table_lookup;
undefined isc_f420_correction_calc;
undefined f430_clear;
ushort DAT_00010cd6;
ushort DAT_00010cd4;
ushort DAT_00010cda;
ushort DAT_00010cd8;
ushort DAT_00010cdc;
ushort DAT_00010cde;
undefined2 DAT_00010ce0;
undefined2 DAT_00010ce4;
undefined2 DAT_00010ce2;
undefined2 DAT_00010d6a;
undefined2 DAT_00010d6c;
undefined2 DAT_00010d66;
undefined2 DAT_0001f420;
ushort DAT_00010cea;
ushort DAT_00010ce6;
ushort DAT_00010c08;
char DAT_000102db;
short DAT_0001f038;
short DAT_0001f29e;
short DAT_00010cf6;
ushort DAT_00010cf4;
ushort DAT_00010cf2;
ushort DAT_0001f4e0;
ushort DAT_00010f12;
undefined2 DAT_00010c20;
short DAT_0001f048;
ushort DAT_00010c24;
ushort DAT_00010c22;
ushort DAT_00010d10;
ushort DAT_00010d12;
ushort DAT_00010d2a;
ushort DAT_00010ca0;
ushort DAT_00010c9e;
ushort DAT_00010c9c;
undefined2 g_isc_correction_f416;
undefined isc_f406_ramp_gate_condition_check;
undefined2 g_isc_stepper_target_f406;
short DAT_0001f03c;
ushort DAT_0001f40e;
undefined select_const_by_flags_eeca;
undefined isc_eeca_eecc_eece_correction_calc;
ushort DAT_0001f3fe;
ushort DAT_0001f3fc;
ushort DAT_0001f3fa;
undefined isc_f412_f082_stall_timer_calc;
ushort DAT_00012eca;
undefined2 DAT_0001f40a;
undefined2 g_isc_stepper_base_f408;
undefined2 DAT_0001f40c;
undefined startup_phase_reset_eed6_eefa_bulk;
undefined2 DAT_00010f50;
undefined2 DAT_0001f09a;
undefined startup_phase_reset_eed6_cluster;
undefined eeea_bit0_update_on_efa2;
undefined knock_sensor_diagnostic_update;
undefined eee0_eeee_diag_flags_reset_dispatch;
undefined knock_octane_f0e6_bit3_gate_dispatch;
ushort DAT_0001eeea;
ushort DAT_0001eeec;
undefined2 DAT_0001f502;
undefined2 DAT_0001ef78;
undefined2 DAT_0001ef7a;
undefined sum_byte_array;
undefined2 g_f09a_calib_select_latch_flags_eeea;
undefined2 g_f09a_calib_select_release_counter_ef6e;
undefined2 DAT_0000f502;
undefined2 DAT_0000f594;
undefined2 DAT_0000f09a;
undefined2 g_f516_request_latch;
undefined eee4_eeda_bit6_update_from_f0f8_bit12;
undefined DAT_0000eed6;
undefined DAT_0000eee0;
ushort DAT_0001f502;
ushort DAT_0001ef78;
ushort DAT_00010f4e;
undefined2 DAT_00010f52;
ushort DAT_00010f54;
short DAT_00010f58;
ushort DAT_0001f1d8;
undefined check_flag_f0d8_bit0_clear;
short DAT_0001f1ec;
ushort DAT_00010f74;
ushort DAT_00010f76;
ushort DAT_00010f7c;
ushort DAT_00010f7e;
short DAT_0001f514;
ushort DAT_00010f66;
ushort DAT_0001f10c;
short DAT_00010f64;
short DAT_0001f00c;
short DAT_0001f09a;
ushort DAT_00010f7a;
ushort DAT_00010f78;
ushort DAT_0001eedc;
undefined2 g_diag_flags_eee4;
undefined2 DAT_0001eeee;
ushort DAT_0001eee0;
undefined2 g_tcu_purge_hysteresis_status_flags_f4ae;
undefined2 g_tcu_purge_duty_gate_byte_f4b0;
undefined2 DAT_0000f4d6;
ushort DAT_0001f526;
ushort DAT_0001f518;
undefined knock_octane_pattern_match_state_machine;
ushort DAT_0001f04a;
ushort DAT_0001eee6;
ushort DAT_0001eee8;
ushort DAT_0001f51c;
undefined2 DAT_0001f51e;
ushort DAT_0001f51a;
undefined2 DAT_0000f0f2;
undefined2 DAT_0000f0f4;
undefined f00e_f520_f526_gate_condition_check;
undefined f510_f512_f00e_f514_reset;
undefined sci1_tx_response_feeder;
undefined sci1_dispatch_and_latch_response;
undefined f526_state_reset_dispatch;
undefined sci1_post_handshake_status_broadcast_dispatch;
undefined2 DAT_0001f534;
undefined sci1_meta_cmd_dispatch_c0_ff;
undefined sci1_latch_and_send_f54a_handshake_byte;
undefined T2CRH;
ushort DAT_0001f514;
undefined2 DAT_0001f526;
ushort DAT_0001f596;
char UNK_00010205;
ushort UNK_00010220;
undefined TMDRA;
undefined2 DAT_0001f512;
undefined2 DAT_0001f00e;
undefined2 DAT_0001f514;
short DAT_0001f532;
undefined UNK_0000ffee;
undefined DAT_0000fff2;
short DAT_0001f526;
undefined2 DAT_0001f528;
undefined2 DAT_0001f52a;
undefined2 DAT_0001f52c;
undefined2 DAT_0001f52e;
undefined2 DAT_0001f530;
undefined2 DAT_0001f598;
undefined2 DAT_0001f59a;
undefined2 DAT_0001f54a;
undefined2 DAT_0001f58c;
ushort DAT_0001f590;
short DAT_0001f584;
short DAT_0001f534;
ushort DAT_0001f59a;
short DAT_0001f598;
undefined sci1_periodic_status_frame_build_f54a_f566;
ushort DAT_0001f54a;
ushort DAT_0001f4b8;
ushort DAT_0001f54c;
undefined2 DAT_0001f54e;
undefined2 DAT_0001f550;
byte[2] BYTE_ARRAY_00010808;
undefined2 accum_latch_100tick_f33a;
undefined2 accum_latch_100tick_f33c;
undefined2 DAT_0001f430;
undefined2 DAT_0001f432;
undefined2 DAT_0001f5f2;
undefined2 DAT_0001f384;
undefined2 DAT_0001f3d0;
undefined2 DAT_00011028;
undefined2 DAT_0001f3da;
undefined2 DAT_0001f3d8;
undefined2 DAT_0001f3d6;
undefined2 DAT_0001f3dc;
undefined2 DAT_0001f5ea;
undefined2 DAT_00010f80;
undefined2 DAT_0001f500;
undefined2 DAT_0001f5e6;
undefined2 DAT_0001f5d0;
undefined2 DAT_0001f5ce;
undefined2 DAT_0001f5cc;
undefined2 DAT_0001f5ca;
undefined2 DAT_0001f5ee;
undefined2 DAT_00010aa0;
undefined2 DAT_0001f2b0;
undefined2 DAT_0000f384;
undefined UNK_0000ffd6;
undefined2 g_tcu_shift_timing_state_flag_efa4;
undefined2 DAT_0000f5ce;
undefined2 DAT_0000f5d0;
undefined check_flag_fe87_bit1_clear;
undefined check_flag_fe8a_bit1_set;
undefined o2_wideband_mode_zero_check;
undefined tcu_f384_f388_hysteresis_gate_check;
undefined compose_status_bits_fe86_fe8a;
undefined sci3_timeout_clear_flag;
undefined sci3_timeout_set_abort;
undefined2 DAT_0000f5e6;
undefined sci3_timeout_set_retry;
undefined update_flag_fe82_bit2_cond;
undefined stub_empty_0x2c129;
undefined tcu_torque_state_reset;
undefined T1GR3L;
undefined T1GR4H;
ushort DAT_0001f384;
ushort DAT_0001f388;
ushort DAT_00010acc;
undefined2 DAT_0001f38a;
ushort DAT_0001f39e;
undefined2 DAT_0001f3c2;
undefined2 DAT_0001f3c0;
undefined2 DAT_0001f3be;
undefined2 DAT_0001f3c8;
undefined2 DAT_0001f3c6;
undefined2 DAT_0001f3c4;
undefined2 DAT_0001f3ca;
undefined tcu_shift_timer_arm;
undefined2 g_f4a4_clear_hold_gate_ef90;
undefined2 g_tcu_shift_timing_timer_gate_efa2;
undefined2 DAT_0000f5ee;
undefined tcu_f382_correction_calc;
undefined2 DAT_0001f5e4;
undefined2 DAT_0001f394;
undefined2 DAT_0001ff46;
undefined2 DAT_0001f39e;
undefined2 DAT_0001f37e;
byte[2] DAT_0001f5ca;
ushort DAT_0001f1da;
short DAT_0001f2b0;
undefined2 DAT_0000f2aa;
ushort DAT_0001f24e;
undefined DAT_0000fde8;
undefined2 DAT_0000f2f0;
undefined2 DAT_0000f2f2;
undefined weighted_blend_4term_u8frac;
undefined2 DAT_0000f2f8;
ushort DAT_0001f5e6;
short DAT_0001f5f2;
short DAT_00010f80;
ushort DAT_0001f5ea;
undefined multi_channel_reg_write_and_poll_ff2x_ff3x;
ushort DAT_0001f228;
undefined2 DAT_00010a7c;
undefined2 DAT_0001ee84;
undefined DAT_00010aae;
undefined2 DAT_0001ee86;
undefined2 DAT_0001ee88;
ushort DAT_0001f20a;
undefined2 DAT_0001ef8c;
undefined2 DAT_0001ef8e;
short DAT_0001ee82;
undefined2 DAT_0000f30e;
undefined2 DAT_0000f312;
undefined2 DAT_0000f314;
undefined2 g_status_word_f102;
undefined2 DAT_0000f336;
undefined2 DAT_0000f338;
undefined f978_increment_on_ff73_bit0;
undefined2 DAT_0000f33a;
undefined flag_read_ff43_bit0_inverted;
undefined2 DAT_0000f33c;
undefined2 DAT_0000f73c;
undefined f978_read_and_clear;
undefined sci1_protocol_state_machine;
undefined update_port_fe82_indexed;
undefined o2_sensor_gain_divisor;
undefined o2_lambda_or_passthrough_select;
undefined mirror_status_f0ea_to_ports;
undefined software_pwm_output_scheduler;
undefined channel_event_or_periodic_dispatch;
undefined2 DAT_0000f1e2;
undefined2 DAT_0000f1e4;
undefined2 g_tcu_purge_duty_mode_select_f4a2;
undefined2 DAT_0000f1ec;
undefined2 DAT_0000f5ec;
undefined2 DAT_0000f1ee;
undefined2 g_tcu_purge_duty_target_f4a8;
undefined2 g_tcu_purge_duty_current_f4aa;
undefined2 DAT_0000f3f8;
undefined2 DAT_0001ef08;
undefined2 DAT_0001ef0a;
undefined2 DAT_0001ef06;
undefined2 DAT_0001ef18;
undefined2 DAT_0001ef1a;
undefined2 DAT_0001ef14;
undefined2 DAT_0001ef16;
undefined2 DAT_0001ef10;
undefined2 DAT_0001ef12;
undefined2 DAT_0001ef1e;
undefined2 DAT_0001ef52;
undefined2 g_tcu_gear_ratio_hist_status_flags;
undefined2 g_tcu_gear_ratio_hist_slot2_lo;
undefined2 g_tcu_gear_ratio_hist_slot2_hi;
undefined2 g_tcu_gear_ratio_hist_slot0_lo;
undefined2 DAT_0001f730;
undefined2 DAT_0001f626;
undefined tcu_shift_timer_scale_init;
undefined2 DAT_0001f624;
undefined2 DAT_0001f622;
undefined2 DAT_0001f620;
undefined2 DAT_0001f62e;
undefined2 DAT_0001f62c;
undefined2 DAT_0001f62a;
undefined2 DAT_0001f628;
undefined2 DAT_0001f656;
undefined2 DAT_0001f654;
undefined2 DAT_0001f652;
undefined2 DAT_0001f64e;
undefined2 DAT_0001f658;
undefined2 DAT_0001f650;
undefined2 DAT_0001f65c;
char DAT_00010300;
undefined2 DAT_0001f664;
undefined2 DAT_0001f662;
char DAT_00010301;
undefined2 DAT_0001f66c;
undefined2 DAT_0001f66a;
undefined2 DAT_0001f668;
undefined2 DAT_0001f666;
char DAT_00010316;
undefined2 DAT_0001f670;
undefined2 DAT_0001f66e;
char DAT_00010314;
undefined2 DAT_0001f65e;
char DAT_00010310;
undefined2 DAT_0001f734;
undefined2 DAT_0001f732;
undefined2 DAT_00011006;
undefined2 DAT_0001f04e;
undefined2 DAT_0001f050;
undefined2 DAT_00011008;
undefined2 DAT_0001f642;
undefined2 DAT_0001f644;
undefined tcu_sensor_range_check_and_f04e_max_update;
undefined tcu_torque_converter_slip_calc_far_trampoline;
ushort DAT_00010fce;
short DAT_0001f646;
undefined2 DAT_0001102e;
ushort DAT_00010ff0;
ushort DAT_00010fee;
undefined2 DAT_0000f642;
undefined2 DAT_00011000;
ushort DAT_0001101c;
undefined2 DAT_0000f644;
ushort DAT_0001101a;
undefined update_max_f04e;
undefined tcu_f050_max_update;
undefined2 DAT_0000f04e;
undefined2 DAT_0000f050;
ushort DAT_0001f04e;
ushort DAT_0001f050;
undefined tcu_torque_converter_slip_calc;
undefined2 DAT_0000f730;
undefined2 DAT_0000f6d4;
undefined2 DAT_0000f714;
undefined2 DAT_0000f6d6;
undefined2 DAT_0000f716;
undefined2 DAT_0000f718;
undefined tiered_threshold_table_select;
undefined2 DAT_0000f71a;
undefined tcu_shift_pattern_threshold_select;
undefined2 DAT_0000f71c;
undefined2 DAT_0000f71e;
ushort DAT_00010fe6;
ushort DAT_00010fec;
ushort DAT_0001f5d8;
ushort DAT_0001f5d6;
undefined tcu_solenoid_diagnostic_calc;
ushort DAT_0001f61e;
undefined2 DAT_0001f96a;
ushort DAT_0001f678;
ushort DAT_0001f672;
ushort DAT_0001f674;
ushort DAT_0001f676;
ushort DAT_00010ffa;
ushort DAT_00010ffc;
ushort DAT_0001f634;
undefined tcu_shift_buffer_average_calc;
short DAT_0001f640;
short DAT_0001f63e;
undefined2 tcu_solenoid_diag_flags_f648;
short DAT_0001f04e;
short DAT_00011006;
ushort DAT_0001150a;
ushort DAT_0001150c;
ushort DAT_0001f64a;
short DAT_0001f6cc;
undefined PTR_DAT_00010321;
ushort DAT_0001f6d4;
undefined2 DAT_00010fe8;
short DAT_0001f6e4;
short DAT_0001f6e6;
short DAT_0001f6e8;
short DAT_0001f6ea;
short DAT_0001f6ec;
short DAT_0001f6ee;
short DAT_0001f6d8;
short DAT_0001f6da;
short DAT_0001f6dc;
short DAT_0001f6de;
short DAT_0001f6e0;
short DAT_0001f6e2;
short DAT_0001f714;
short DAT_0001f716;
short DAT_0001f718;
short DAT_0001f71a;
short DAT_0001f71c;
short DAT_0001f71e;
undefined PTR_INTC_IPRA_000102bd;
ushort DAT_0001f7c8;
ushort DAT_0001f7d8;
ushort DAT_0001f7ca;
ushort DAT_0001f7da;
ushort DAT_0001f7cc;
ushort DAT_0001f7dc;
ushort DAT_0001f7ce;
ushort DAT_0001f7de;
undefined2 DAT_0001f6cc;
undefined2 DAT_0001f6dc;
undefined2 DAT_0001f6da;
undefined2 DAT_0001f6d8;
undefined2 DAT_0001f6e2;
undefined2 DAT_0001f6e0;
undefined2 DAT_0001f6de;
undefined2 DAT_0001f6e8;
undefined2 DAT_0001f6e6;
undefined2 DAT_0001f6e4;
undefined2 DAT_0001f6ee;
undefined2 DAT_0001f6ec;
undefined2 DAT_0001f6ea;
undefined2 DAT_0001f7c8;
undefined2 DAT_0001f7ca;
undefined2 DAT_0001f7cc;
undefined2 DAT_0001f7ce;
ushort DAT_0001f624;
ushort DAT_0001f622;
undefined select_min_xy_triple;
undefined identity_passthrough_u16;
short DAT_0001f3d4;
short DAT_0001f642;
short DAT_0001f638;
short DAT_0001f636;
undefined DAT_0000f3d6;
undefined DAT_0000f3d8;

ushort __stdcall_far_allstack sat_add_u16(ushort a,ushort b)

{
  ushort result;
  
  result = a + b;
  if (CARRY2(a,b)) {
    result = 0xffff;
  }
  return result;
}



bool FUN_0001400f(ushort param_fp1,ushort param_fp2)

{
  ushort in_stack_00000004;
  
  return CARRY2(in_stack_00000004,param_fp1);
}



short __stdcall_far_allstack_32ret sat_add_s16(short a,short b)

{
  short sVar1;
  short in_stack_00000008;
  ushort in_stack_0000000a;
  
  sVar1 = a + in_stack_00000008 + (ushort)CARRY2(b,in_stack_0000000a);
  if (CARRY2(b,in_stack_0000000a)) {
    sVar1 = -1;
  }
  return sVar1;
}



ushort __stdcall_far_allstack sat_sub_u16(ushort a,ushort b)

{
  ushort result;
  
  result = a - b;
  if (a < b) {
    result = 0;
  }
  return result;
}



ushort __stdcall_far_allstack_32ret sat_sub_u16_clamp0(ushort a,ushort b)

{
  ushort result;
  short in_stack_00000008;
  ushort in_stack_0000000a;
  
  result = (a - in_stack_00000008) - (ushort)(b < in_stack_0000000a);
  if (a < in_stack_00000008 + (ushort)(b < in_stack_0000000a)) {
    result = 0;
  }
  return result;
}



ushort __stdcall_far_allstack sat_mul_u16(ushort a,ushort b)

{
  ushort result;
  
  result = (ushort)((uint)a * (uint)b);
  if ((short)((uint)a * (uint)b >> 0x10) != 0) {
    result = 0xffff;
  }
  return result;
}



ushort __stdcall_far_allstack mul_u16_hi(ushort a,ushort b)

{
  return (ushort)((uint)a * (uint)b >> 0x10);
}



// WARNING: Type propagation algorithm not settling

short mul_u16_x2_or_sum_check_overflow(ushort a,ushort b,ushort c,ushort d)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar4;
  bool bVar5;
  ushort in_stack_00000006;
  ushort in_stack_00000008;
  ushort in_stack_0000000a;
  
  if (d == 0) {
    uVar2 = (ushort)((uint)in_stack_0000000a * (uint)in_stack_00000006 >> 0x10);
    sVar1 = (short)((uint)in_stack_00000008 * (uint)in_stack_00000006 >> 0x10);
    uVar3 = (ushort)((uint)in_stack_00000008 * (uint)in_stack_00000006);
    sVar4 = uVar2 + uVar3;
    bVar5 = CARRY2(uVar2,uVar3);
  }
  else {
    if (in_stack_00000008 != 0) {
      return -1;
    }
    sVar1 = (short)((uint)d * (uint)in_stack_0000000a >> 0x10);
    uVar3 = (ushort)((uint)d * (uint)in_stack_0000000a);
    uVar2 = (ushort)((uint)in_stack_00000006 * (uint)in_stack_0000000a >> 0x10);
    sVar4 = uVar2 + uVar3;
    bVar5 = CARRY2(uVar2,uVar3);
  }
  if ((bVar5) || (sVar1 != 0)) {
    return -1;
  }
  return sVar4;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

ushort __stdcall_far_allstack div_u16_sat(ushort a,ushort b)

{
  ushort uVar1;
  
  if (b == 0) {
    a = 0xffff;
  }
  else {
    uVar1 = a / b;
  }
  return a;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

ushort __stdcall_far_allstack div_u32_u16_sat(ushort a_hi,ushort a_lo,ushort b)

{
  undefined2 uVar1;
  
  if ((b == 0) || (b <= a_hi)) {
    return 0xffff;
  }
  uVar1 = (short)(CONCAT22(a_hi,a_lo) / (uint)b);
  return a_lo;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

ushort __stdcall_far_allstack div_u16_sat2(ushort a_hi,ushort a_lo,ushort b)

{
  ushort uVar1;
  
  if (b == 0) {
    a_hi = 0xffff;
  }
  else {
    uVar1 = a_hi / b;
  }
  return a_hi;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

ushort __stdcall_far_allstack div_u16_rounded(ushort a,ushort b)

{
  ushort uVar1;
  
  if (b == 0) {
    return 0xffff;
  }
  uVar1 = a % b;
  uVar1 = a / b;
  if ((SUB21(uVar1 >> 0xf,0)) || (b <= uVar1 * 2)) {
    a = a + 1;
  }
  return a;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

short __stdcall_far_allstack div_s32_s16_rounded(ushort a_hi,ushort a_lo,ushort b)

{
  short sVar1;
  undefined2 uVar2;
  bool bVar3;
  
  if ((b != 0) && (a_hi < b)) {
    sVar1 = (short)(CONCAT22(a_hi,a_lo) % (uint)b);
    uVar2 = (short)(CONCAT22(a_hi,a_lo) / (uint)b);
    if (((!SUB21((ushort)sVar1 >> 0xf,0)) && ((ushort)(sVar1 * 2) < b)) ||
       (bVar3 = a_lo != 0xffff, a_lo = a_lo + 1, bVar3)) {
      return a_lo;
    }
  }
  return -1;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

ushort __stdcall_far_allstack_32ret div_u32_u16_rounded(uint dividend,ushort divisor)

{
  uint uVar1;
  ushort uVar2;
  short sVar3;
  uint uVar4;
  
  if (divisor == 0) {
    uVar2 = 0xffff;
  }
  else {
    uVar2 = dividend._0_2_ % divisor;
    sVar3 = (short)(CONCAT22(uVar2,(short)dividend) % (uint)divisor);
    uVar1 = uVar4 >> 0x10;
    uVar4 = dividend;
    uVar2 = (ushort)uVar1;
    if ((!SUB21((ushort)sVar3 >> 0xf,0)) && ((ushort)(sVar3 * 2) < divisor)) {
      return uVar2;
    }
    uVar2 = uVar2 + (0xfffe < (ushort)uVar4);
  }
  return uVar2;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register
// WARNING: Restarted to delay deadcode elimination for space: stack

ushort __stdcall_far_allstack muldiv_u16(ushort a,ushort b,ushort c)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  undefined1 *puVar4;
  undefined1 auStack_82 [128];
  
  puVar4 = auStack_82 + 0x80;
  uVar2 = (ushort)((uint)a * (uint)b >> 0x10);
  uVar3 = (undefined2)((uint)a * (uint)b);
  uVar1 = *(ushort *)(puVar4 + 10);
  if ((uVar1 == 0) || (uVar1 <= uVar2)) {
    return 0xffff;
  }
  uVar3 = (short)(CONCAT22(uVar2,uVar3) / (uint)uVar1);
  return b;
}


/*
Unable to decompile 'muldiv_u16_scaled'
Cause: Exception while decompiling 000141dc: Decompiler process died

*/


// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register
// WARNING: Restarted to delay deadcode elimination for space: stack

ushort __stdcall_far_allstack muldiv_s16_rounded(ushort a,ushort b,ushort c)

{
  uint uVar1;
  ushort uVar2;
  short sVar3;
  undefined2 uVar4;
  ushort uVar5;
  undefined1 *puVar6;
  bool bVar7;
  undefined1 auStack_82 [128];
  
  puVar6 = auStack_82 + 0x80;
  uVar2 = (ushort)((uint)a * (uint)b >> 0x10);
  uVar4 = (undefined2)((uint)a * (uint)b);
  uVar5 = *(ushort *)(puVar6 + 10);
  if ((uVar5 != 0) && (uVar2 < uVar5)) {
    uVar1 = CONCAT22(uVar2,uVar4);
    sVar3 = (short)(uVar1 % (uint)uVar5);
    uVar4 = (short)(uVar1 / uVar5);
    if (((!SUB21((ushort)sVar3 >> 0xf,0)) && ((ushort)(sVar3 * 2) < uVar5)) ||
       (bVar7 = b != 0xffff, b = b + 1, bVar7)) {
      return b;
    }
  }
  return 0xffff;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

short __stdcall_far_allstack_32ret muldiv_s16_rounded_3op(ushort a,ushort b,ushort c)

{
  uint uVar1;
  short sVar2;
  ushort uVar3;
  ushort in_R1;
  undefined2 uVar4;
  short sVar5;
  ushort unaff_R3;
  undefined1 *puVar6;
  undefined1 auStack_84 [128];
  
  puVar6 = auStack_84 + 0x82;
  sVar2 = (short)((uint)a * (uint)c >> 0x10);
  uVar4 = (short)((uint)a * (uint)c);
  uVar3 = (ushort)((uint)b * (uint)*(ushort *)(puVar6 + 10) >> 0x10);
  uVar4 = (short)((uint)b * (uint)*(ushort *)(puVar6 + 10));
  sVar5 = uVar3 + in_R1;
  uVar3 = sVar2 + (ushort)CARRY2(uVar3,in_R1);
  if ((*(short *)(puVar6 + 0xc) != 0) && (uVar3 < *(ushort *)(puVar6 + 0xc))) {
    uVar1 = CONCAT22(uVar3,sVar5);
    uVar4 = (undefined2)(uVar1 % (uint)*(ushort *)(puVar6 + 0xc));
    uVar4 = (short)(uVar1 / *(ushort *)(puVar6 + 0xc));
    sVar2 = (short)(CONCAT22(uVar4,unaff_R3) % (uint)*(ushort *)(puVar6 + 0xc));
    uVar4 = (short)(CONCAT22(uVar4,unaff_R3) / (uint)*(ushort *)(puVar6 + 0xc));
    if ((!SUB21((ushort)sVar2 >> 0xf,0)) && ((ushort)(sVar2 * 2) < *(ushort *)(puVar6 + 0xc))) {
      return sVar5;
    }
    if (0xfffe >= unaff_R3) {
      return sVar5 + (ushort)(0xfffe < unaff_R3);
    }
  }
  return -1;
}



ushort __stdcall_far_allstack mul_u16_x2_sat_alt(ushort a,ushort b)

{
  ushort result;
  short hi_shifted;
  int product;
  
  product = (uint)a * (uint)b;
  hi_shifted = (short)((uint)product >> 0x10) << 1;
  if ((product < 0) ||
     (result = CONCAT11((byte)hi_shifted | (byte)((uint)product >> 8) >> 7,
                        (char)((ushort)((short)product << 1) >> 8)),
     (char)((ushort)hi_shifted >> 8) != '\0')) {
    result = 0xffff;
  }
  return result;
}



ushort __stdcall_far_allstack mul_u16_sat_s16(ushort a,ushort b,ushort c)

{
  short sVar1;
  int iVar2;
  ushort uVar3;
  
  iVar2 = (uint)a * (uint)c + ((uint)b * (uint)c >> 0x10);
  sVar1 = (short)((uint)iVar2 >> 0x10) * 2;
  if ((iVar2 < 0) ||
     (uVar3 = CONCAT11((byte)sVar1 | (short)iVar2 < 0,(char)((ushort)((short)iVar2 * 2) >> 8)),
     (char)((ushort)sVar1 >> 8) != '\0')) {
    uVar3 = 0xffff;
  }
  return uVar3;
}



ushort __stdcall_far_allstack mul_u16_x2_sat(ushort a,ushort b)

{
  ushort result;
  bool carry;
  ushort lo_shifted;
  uint product;
  
  product = (uint)a * (uint)b;
  if ((0x7fffff < product) ||
     ((lo_shifted = (short)product << 1,
      result = CONCAT11((byte)((short)(product >> 0x10) << 1) | (byte)(product >> 8) >> 7,
                        (char)(lo_shifted >> 8)), (lo_shifted & 0x80) != 0 &&
      (carry = 0xfffe < result, result = result + 1, carry)))) {
    result = 0xffff;
  }
  return result;
}



// 32-bit x 16-bit -> 16-bit saturating multiply (double-word variant of mul_u16_x2_sat family).
// a_hi:a_lo form the 32-bit multiplicand, b is the 16-bit multiplier.
// 
// [PARAMS PARTIALLY FIXED 2026-08-03] a_hi and a_lo bind cleanly via CUSTOM_STORAGE at
// Stack[0x4]/[0x6] (same fix pattern as table_lookup_interp, item 24-style). b does NOT bind
// regardless of declared storage offset -- confirmed via disassembly: b is read directly as a
// memory operand by `mulxu.w @(0xa:8,FP)` (never register-loaded first), unlike a_hi/a_lo and
// unlike sibling function div_u16_sat2's third param (which IS register-loaded via mov:f.w
// before use, and binds fine). Ghidra's parameter-substitution pass appears to only rewrite
// register-loaded stack reads, not direct memory operands to mulxu/similar instructions. Tried:
// multiple storage offsets (0x8, 0xa) -- decompiler always shows b as
// *(ushort*)((int)&stack0xfffe + 10), never the named parameter. This is a genuine decompiler/
// Sleigh-semantics limitation for this instruction shape, not a wrong-offset bug. b's real value
// is at Stack[0xa]:2 if manually inspecting; left undocumented-but-correct in the signature.

short __stdcall_far_allstack_32ret mul_u16_dbl_sat(ushort a_hi,ushort a_lo,ushort b)

{
  ushort uVar1;
  short sVar2;
  short sVar3;
  uint uVar4;
  int iVar5;
  short sVar6;
  bool bVar7;
  ushort in_stack_00000008;
  
  uVar4 = (uint)a_lo * (uint)in_stack_00000008;
  iVar5 = (uint)a_hi * (uint)in_stack_00000008 + (uVar4 >> 0x10);
  uVar1 = (short)uVar4 << 1;
  sVar2 = (short)iVar5 * 2;
  sVar3 = (short)((uint)iVar5 >> 0x10) * 2;
  if (-1 < iVar5) {
    sVar6 = CONCAT11((byte)sVar3 | (short)iVar5 < 0,(char)((ushort)sVar2 >> 8));
    if (((char)((ushort)sVar3 >> 8) == '\0') &&
       (((uVar1 & 0x80) == 0 ||
        (bVar7 = 0xfffe < CONCAT11((byte)sVar2 | (byte)(uVar4 >> 8) >> 7,(char)(uVar1 >> 8)),
        sVar6 = sVar6 + (ushort)bVar7, !bVar7)))) {
      return sVar6;
    }
  }
  return -1;
}



ushort __stdcall_far_allstack mul_u16_shr8_sat(ushort a,ushort b)

{
  ushort result;
  
  result = (ushort)((uint)a * (uint)b >> 8);
  if ((char)((uint)a * (uint)b >> 0x18) != '\0') {
    result = 0xffff;
  }
  return result;
}



ushort __stdcall_far_allstack muldiv_u16_shr8_sat(ushort a,ushort b,ushort c)

{
  int iVar1;
  
  iVar1 = (uint)a * (uint)c + ((uint)b * (uint)c >> 0x10);
  if ((char)((uint)iVar1 >> 0x18) == '\0') {
    return (ushort)((uint)iVar1 >> 8);
  }
  return 0xffff;
}



ushort __stdcall_far_allstack mul_fixedpoint_8(ushort a,ushort b)

{
  ushort result;
  bool carry;
  uint product;
  
  product = (uint)a * (uint)b;
  result = (ushort)(product >> 8);
  if (((char)(product >> 0x18) != '\0') ||
     (((product & 0x80) != 0 && (carry = 0xfffe < result, result = result + 1, carry)))) {
    result = 0xffff;
  }
  return result;
}



// 32-bit x 16-bit -> 16-bit, shift-right-8 saturating multiply (shr8 variant of mul_u16_x2 family).
// a_hi:a_lo form the 32-bit multiplicand, b is the 16-bit multiplier.
// 
// [PARAMS PARTIALLY FIXED 2026-08-03] Same class of issue as sibling mul_u16_dbl_sat (0x1431f):
// a_hi/a_lo bind cleanly via CUSTOM_STORAGE at Stack[0x4]/[0x6]; b does not bind at any declared
// offset because it's read as a direct memory operand by `mulxu.w @(0xa:8,FP)` rather than
// register-loaded first. See mul_u16_dbl_sat's plate comment for full detail -- identical root
// cause, identical fix scope. b's real value is at Stack[0xa]:2.

short __stdcall_far_allstack mul_u16_shr8_sat2(ushort a_hi,ushort a_lo,ushort b)

{
  uint uVar1;
  int iVar2;
  short sVar3;
  bool bVar4;
  ushort in_stack_00000008;
  
  uVar1 = (uint)a_lo * (uint)in_stack_00000008;
  iVar2 = (uint)a_hi * (uint)in_stack_00000008 + (uVar1 >> 0x10);
  sVar3 = (short)((uint)iVar2 >> 8);
  if (((char)((uint)iVar2 >> 0x18) != '\0') ||
     (((uVar1 & 0x80) != 0 &&
      (bVar4 = 0xfffe < CONCAT11((char)iVar2,(char)(uVar1 >> 8)), sVar3 = sVar3 + (ushort)bVar4,
      bVar4)))) {
    sVar3 = -1;
  }
  return sVar3;
}



ushort __stdcall_far_allstack clamp_u8(ushort a)

{
  if (0xfe < a) {
    a = 0xff;
  }
  return a;
}



ushort __stdcall_far_allstack passthrough_or_sat(ushort flag,ushort value)

{
  if (flag != 0) {
    value = 0xffff;
  }
  return value;
}



ushort __stdcall_far_allstack clamp_u16(ushort value,ushort lo,ushort hi)

{
  ushort lo_or_hi;
  
  lo_or_hi = lo;
  if (hi <= lo) {
    lo_or_hi = hi;
    hi = lo;
  }
  if (hi < value) {
    value = hi;
  }
  else if (value < lo_or_hi) {
    value = lo_or_hi;
  }
  return value;
}



// Median-of-three selector on three 32-bit (hi,lo) word-pair values: (hi_a,lo_a) is the base value,
// (hi_b,lo_b) and (hi_c,lo_c) are two candidates. Sorts b/c so the larger lands in hi_c/lo_c, then
// picks base if it's outside the [smaller,larger] range, else clamps to the nearer bound. Only the
// hi word is returned (function is ushort, not a 32-bit return) -- confirmed via disassembly,
// matches decompile exactly.

ushort __stdcall_far_allstack
select_min_xy_triple(ushort hi_a,ushort lo_a,ushort hi_b,ushort lo_b,ushort hi_c,ushort lo_c)

{
  ushort uVar1;
  ushort uVar2;
  
  uVar1 = hi_b;
  uVar2 = lo_b;
  if ((hi_c <= hi_b) && ((hi_c < hi_b || (lo_c <= lo_b)))) {
    uVar1 = hi_c;
    uVar2 = lo_c;
    hi_c = hi_b;
    lo_c = lo_b;
  }
  if ((hi_c < hi_a) || ((hi_c <= hi_a && (lo_c < lo_a)))) {
    hi_a = hi_c;
  }
  else if ((hi_a < uVar1) || ((hi_a <= uVar1 && (lo_a < uVar2)))) {
    hi_a = uVar1;
  }
  return hi_a;
}



ushort __stdcall_far_allstack get_high_byte(ushort a)

{
  return a >> 8;
}



ushort __stdcall_far_allstack identity_passthrough_u16(ushort a)

{
  return a;
}



ushort __stdcall_far_allstack shift_left_byte_to_high(ushort a)

{
  return a << 8;
}



undefined2 __stdcall_far_allstack identity_passthrough_arg1(undefined2 a)

{
  return a;
}



char __stdcall_far_allstack round_high_byte_signed(ushort a)

{
  char result;
  bool carry;
  
  result = (char)(a >> 8);
  if (((a & 0x80) != 0) && (carry = result == -1, result = result + '\x01', carry)) {
    result = -1;
  }
  return result;
}



short __stdcall_far_allstack inc_if_signed_flag(short value,ushort flags)

{
  bool was_min;
  
  if (((flags & 0x8000) != 0) && (was_min = value == -1, value = value + 1, was_min)) {
    value = -1;
  }
  return value;
}



undefined2 __stdcall_far_allstack swap_bytes_u16(ushort a)

{
  return CONCAT11((char)a,(char)(a >> 8));
}



undefined2 __stdcall_far_allstack swap_invert_high_byte(byte a)

{
  return CONCAT11(~a,a);
}



void __stdcall_far_allstack zero_ram_range(ushort start_addr,ushort end_addr)

{
  for (; start_addr < end_addr; start_addr = start_addr + 2) {
    *(undefined2 *)start_addr = 0;
  }
  return;
}



void __stdcall_far_allstack decrement_counters_in_range(ushort start_addr,ushort end_addr)

{
  while( true ) {
    if (end_addr <= start_addr) break;
    if (*(short *)start_addr != 0) {
      *(short *)start_addr = *(short *)start_addr + -1;
    }
    start_addr = start_addr + 2;
  }
  return;
}



void __stdcall_far_allstack increment_counters_in_range(ushort start_addr,ushort end_addr)

{
  while( true ) {
    if (end_addr <= start_addr) break;
    if (*(short *)start_addr != -1) {
      *(short *)start_addr = *(short *)start_addr + 1;
    }
    start_addr = start_addr + 2;
  }
  return;
}



ushort __stdcall_far_allstack axis_interp_lerp_u8frac_256(ushort y1,ushort y0,ushort frac)

{
  int term_y0;
  ushort term_y0_lo;
  ushort term_y1_lo;
  
  if (0x100 < frac) {
    frac = 0x100;
  }
  term_y1_lo = (ushort)((uint)frac * (uint)y1);
  term_y0 = (uint)(0x100 - frac) * (uint)y0;
  term_y0_lo = (ushort)term_y0;
  return CONCAT11((char)((uint)frac * (uint)y1 >> 0x10) + (char)((uint)term_y0 >> 0x10) +
                  CARRY2(term_y1_lo,term_y0_lo),(char)(term_y1_lo + term_y0_lo >> 8));
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

short __stdcall_far_allstack_32ret weighted_blend_u8frac(ushort frac,ushort a,ushort b)

{
  short sVar1;
  ushort in_R1;
  undefined2 uVar2;
  short sVar3;
  ushort uVar4;
  short sVar5;
  ushort unaff_R3;
  undefined1 *puVar6;
  undefined1 auStack_84 [128];
  
  puVar6 = auStack_84 + 0x82;
  if (0xff < b) {
    b = 0xff;
  }
  uVar4 = 0xff - b;
  sVar1 = (short)((uint)b * (uint)frac >> 0x10);
  uVar2 = (short)((uint)b * (uint)frac);
  sVar5 = (short)((uint)uVar4 * (uint)*(ushort *)(puVar6 + 8) >> 0x10);
  uVar2 = (short)((uint)uVar4 * (uint)*(ushort *)(puVar6 + 8));
  sVar3 = in_R1 + unaff_R3;
  uVar2 = (short)(CONCAT22(sVar1 + sVar5 + (ushort)CARRY2(in_R1,unaff_R3),sVar3) / 0xff);
  return sVar3;
}



// [PROTOTYPE ADDED 2026-07-23] Missing prototype, calling convention was defaulting to broken
// __stdcall_far (unassigned param storage) -- fixed to __stdcall_far_allstack. Params inferred from
// disassembly stack offsets (FP+6/8/a/c/e) and multiply-pairing (frac*term_a_{hi,lo},
// (0x100-frac)*term_b_{hi,lo}), producing a 32-bit weighted-sum result truncated to 16 bits. EXACT
// semantic role of term_a/term_b (e.g. which represents a 32-bit axis value split hi/lo vs two
// independent 16-bit inputs) NOT independently confirmed against a caller -- names describe the
// arithmetic structure observed, not a verified physical meaning. Revisit if a caller's actual
// argument types clarify this further.

ushort __stdcall_far_allstack
weighted_blend_4term_u8frac
          (ushort term_a_hi,ushort term_a_lo,ushort term_b_lo,ushort term_b_hi,ushort frac)

{
  ushort mid_sum;
  bool carry;
  ushort term_a_hi_x_frac_carry;
  ushort term_a_lo_x_frac_lo;
  int term_b_hi_x_invfrac;
  ushort term_b_lo_x_invfrac_lo;
  
  if (0x100 < frac) {
    frac = 0x100;
  }
  term_a_lo_x_frac_lo = (ushort)((uint)frac * (uint)term_a_hi);
  term_a_hi_x_frac_carry = (ushort)((uint)frac * (uint)term_a_lo >> 0x10);
  term_b_hi_x_invfrac = (uint)(0x100 - frac) * (uint)term_b_hi;
  carry = CARRY2((ushort)((uint)frac * (uint)term_a_lo),(ushort)term_b_hi_x_invfrac);
  mid_sum = term_a_lo_x_frac_lo + term_a_hi_x_frac_carry +
            (short)((uint)term_b_hi_x_invfrac >> 0x10) + (ushort)carry;
  term_b_hi_x_invfrac = (uint)(0x100 - frac) * (uint)term_b_lo;
  term_b_lo_x_invfrac_lo = (ushort)term_b_hi_x_invfrac;
  return CONCAT11((char)((uint)frac * (uint)term_a_hi >> 0x10) +
                  CARRY2(term_a_lo_x_frac_lo,term_a_hi_x_frac_carry) + carry +
                  (char)((uint)term_b_hi_x_invfrac >> 0x10) + CARRY2(mid_sum,term_b_lo_x_invfrac_lo)
                  ,(char)(mid_sum + term_b_lo_x_invfrac_lo >> 8));
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register

short weighted_blend_2term_u8frac_div255
                (ushort param_fp1,ushort param_fp2,ushort param_fp3,ushort param_fp4,
                ushort param_fp5)

{
  short sVar1;
  ushort in_R1;
  undefined2 uVar2;
  short sVar3;
  ushort uVar4;
  ushort unaff_R3;
  ushort uVar5;
  short sVar6;
  ushort unaff_R5;
  undefined1 *puVar7;
  ushort in_stack_00000004;
  undefined1 auStack_88 [128];
  
  puVar7 = auStack_88 + 0x86;
  if (0xff < param_fp4) {
    param_fp4 = 0xff;
  }
  uVar5 = 0xff - param_fp4;
  sVar1 = (short)((uint)param_fp4 * (uint)in_stack_00000004 >> 0x10);
  uVar2 = (short)((uint)param_fp4 * (uint)in_stack_00000004);
  uVar4 = (ushort)((uint)param_fp4 * (uint)*(ushort *)(puVar7 + 8) >> 0x10);
  uVar2 = (short)((uint)param_fp4 * (uint)*(ushort *)(puVar7 + 8));
  sVar3 = in_R1 + uVar4;
  sVar1 = sVar1 + (ushort)CARRY2(in_R1,uVar4);
  sVar6 = (short)((uint)uVar5 * (uint)*(ushort *)(puVar7 + 0xc) >> 0x10);
  uVar2 = (short)((uint)uVar5 * (uint)*(ushort *)(puVar7 + 0xc));
  uVar4 = sVar3 + sVar6 + (ushort)CARRY2(unaff_R3,unaff_R5);
  sVar1 = sVar1 + (ushort)CARRY2(unaff_R3,unaff_R5);
  sVar6 = (short)((uint)uVar5 * (uint)*(ushort *)(puVar7 + 10) >> 0x10);
  uVar2 = (short)((uint)uVar5 * (uint)*(ushort *)(puVar7 + 10));
  sVar3 = uVar4 + unaff_R5;
  uVar2 = (short)(CONCAT22(sVar1 + sVar6 + (ushort)CARRY2(uVar4,unaff_R5),sVar3) / 0xff);
  return sVar3;
}



// WARNING: Restarted to delay deadcode elimination for space: stack
// Shared axis-table lookup+interpolation dispatcher, 130 call sites -- HIGH VALUE.
// Takes an EP-bank/interpolation-context word (ep_val, @Stack[0x4]) + a pointer to an
// axis_lookup_record-style struct (table, @Stack[0x6], same family as axis_lookup_interp's
// record but with an extra mode-selector byte at offset 1 and a stride byte at offset 6).
// If table[0]==3 it does a 2D bilinear-style interpolation (two calls into
// axis_interp_lerp_u8frac then a third combining call); otherwise a simple 1D lookup
// (single call).
// 
// [PROTOTYPE FIXED 2026-08-03] Earlier attempts to declare (byte+ptr) or (byte+ushort-offset)
// prototypes normally failed to bind -- Ghidra's local analysis kept re-inferring storage at
// Stack[0x7]/[0x8] (a 2-byte slot at offset 7 colliding into offset 8's slot), leaving `table`
// UNASSIGNED and the decompiler falling back to void(void) with raw in_stack_* reads. ROOT
// CAUSE: same class of issue already solved for table_lookup_indexed/table_3axis_interp_triple
// in review.md item 24 -- normal prototype-setting APIs can't override the local analysis'
// storage inference on this function shape. FIX: applied via FixTableLookupInterpStorage.java
// (ghidra scripts/), using updateFunction() with FunctionUpdateType.CUSTOM_STORAGE to force
// explicit VariableStorage(program, 0x4, 2) for ep_val and VariableStorage(program, 0x6, 2)
// for table, bypassing local-analysis inference entirely. Confirmed via force_decompile:
// table now binds as a clean, consistently-used void* throughout (table+1, +2, +4, +6),
// matching the struct trace above exactly, no split pointer, no ghost param. Return type
// resolves to ushort, matching axis_interp_lerp_u8frac's actual return (matches its two
// sibling functions above). Signature: ushort table_lookup_interp(ushort ep_val, void *table),
// __stdcall_far_2arg, CUSTOM_STORAGE.
// 
// If this program is ever re-imported/re-analyzed from scratch, re-run
// FixTableLookupInterpStorage.java (covers all three sibling functions) -- this is not part
// of Ghidra's auto-analysis pipeline, same caveat as item 25's AnnotateBankOffsetCallSites.java.

void __stdcall_far_2arg table_lookup_interp(ushort ep_val,void *table)

{
  undefined1 uVar1;
  ushort uVar2;
  ushort uVar3;
  char *pcVar4;
  ushort uVar5;
  char cVar6;
  short sVar7;
  short sVar8;
  undefined1 *puVar9;
  undefined1 uVar10;
  undefined1 uVar11;
  undefined1 auStack_a [8];
  
  uVar11 = 0;
  uVar10 = 1;
  puVar9 = auStack_a + 8;
  cVar6 = *(char *)((int)table + 1);
  uVar3 = **(ushort **)((int)table + 2);
  if (*(char *)table == '\x03') {
    uVar3 = (ushort)*(byte *)((int)table + 6);
    sVar7 = uVar3 * (**(ushort **)((int)table + 4) & 0xff);
    sVar8 = (short)table + *(short *)(puVar9 + -4) + uVar3;
    pcVar4 = (char *)(sVar8 + 7);
    sVar7 = *(short *)(puVar9 + -2);
    uVar1 = (undefined1)*(undefined2 *)(puVar9 + 6);
    uVar3 = (ushort)(byte)(*(char *)((int)sVar8 + 8) + cVar6);
    uVar5 = (ushort)(byte)(*pcVar4 + cVar6);
    *(undefined2 *)(puVar9 + -8) = *(undefined2 *)(puVar9 + 6);
    *(char **)(puVar9 + -6) = pcVar4;
    uVar3 = axis_interp_lerp_u8frac(uVar5,uVar3,sVar7);
    pcVar4 = (char *)(*(short *)(puVar9 + -6) +
                     (ushort)*(byte *)((int)*(undefined2 *)(puVar9 + 8) + 6));
    *(char **)(puVar9 + -6) = pcVar4;
    sVar7 = *(short *)(puVar9 + -2);
    uVar5 = (ushort)(byte)(pcVar4[1] + cVar6);
    uVar2 = (ushort)(byte)(cVar6 + *pcVar4);
    *(ushort *)(puVar9 + -6) = uVar3;
    uVar2 = axis_interp_lerp_u8frac(uVar2,uVar5,sVar7);
    uVar3 = **(ushort **)((int)*(undefined2 *)(puVar9 + 8) + 4);
    uVar5 = *(ushort *)(puVar9 + -6);
  }
  else {
    sVar7 = (short)table + (uVar3 & 0xff);
    uVar2 = (ushort)(byte)(*(char *)((int)sVar7 + 5) + cVar6);
    uVar5 = (ushort)(byte)(cVar6 + *(char *)((int)sVar7 + 4));
  }
  axis_interp_lerp_u8frac(uVar5,uVar2,uVar3 >> 8);
  return;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register
// [PROTOTYPE ADDED 2026-07-23] Generic ROM axis-table lookup+interpolation
// utility, 80+ call sites across virtually every ECU subsystem (isc_*, ign_*,
// egr_*, o2_*, tcu_*, knock_*, fueling_*, etc.) -- this is the ROM's shared
// "interpolate along a calibration axis" primitive.
// 
// Takes a single pointer to an axis_lookup_record (struct defined 2026-07-23,
// verified against this function's own disassembly AND cross-checked against
// a real call site, isc_openloop_target_calc @ 0x17cb8):
//   value_ptr  (offset 0) - ushort* to the RAM cell holding the X value to
//              look up, ALSO the destination the final interpolated result
//              is written back into (read once at entry, written once at exit)
//   axis_ptr   (offset 2) - ushort* to a second RAM cell, dereferenced to get
//              the actual comparison value used for the table search
//   axis_count (offset 4) - table length (clamped to 0xff/255 max)
//   axis_table (offset 6) - inline ushort[] axis breakpoint array, variable
//              length per caller (hence declared as a single ushort, not a
//              fixed-size array -- real length varies across 80+ call sites)
// 
// CAUTION -- investigated but NOT fully resolved: `ldc.b @(0x7:8,FP),EP` at
// function entry loads a byte into the EP bank register, but EP is NEVER
// referenced again anywhere in this function body (all real accesses go
// through the plain word pointer at FP+8, i.e. `record` above). This looks
// like vestigial/dead compiler-generated boilerplate (paired with the
// `ldc.b 0x0,DP` cleanup at the very end) rather than functionally
// significant banking -- but this was inferred from static analysis only,
// not verified against a live trace. If EP turns out to matter after all,
// the single-pointer prototype here would need revisiting.
// 
// Algorithm: binary-search-like walk through axis_table to bracket
// lookup_value between two adjacent breakpoints, then linear-interpolate
// the fractional position (scaled by 0xff/255) and write the combined
// {index, fraction} result back through value_ptr.

void axis_lookup_interp(byte bank,axis_lookup_record *record)

{
  uint uVar1;
  int iVar2;
  short sVar3;
  ushort count_minus_1;
  undefined2 uVar4;
  undefined2 uVar5;
  ushort uVar6;
  ushort *table_entry_ptr;
  ushort lookup_value;
  
                    // [PROTOTYPE ADDED 2026-07-23] Generic ROM axis-table lookup+interpolation
                    //    utility, 80+ call sites across virtually every ECU subsystem (isc_*,
                    // ign_*,
                    //    egr_*, o2_*, tcu_*, knock_*, fueling_*, etc.) -- this is the ROM's shared
                    //    "interpolate along a calibration axis" primitive.
                    //    
                    //    Takes a single pointer to an axis_lookup_record (struct defined
                    // 2026-07-23,
                    //    verified against this function's own disassembly AND cross-checked against
                    //    a real call site, isc_openloop_target_calc @ 0x17cb8):
                    //      value_ptr  (offset 0) - ushort* to the RAM cell holding the X value to
                    //                 look up, ALSO the destination the final interpolated result
                    //                 is written back into (read once at entry, written once at
                    // exit)
                    //      axis_ptr   (offset 2) - ushort* to a second RAM cell, dereferenced to
                    // get
                    //                 the actual comparison value used for the table search
                    //      axis_count (offset 4) - table length (clamped to 0xff/255 max)
                    //      axis_table (offset 6) - inline ushort[] axis breakpoint array, variable
                    //                 length per caller (hence declared as a single ushort, not a
                    //                 fixed-size array -- real length varies across 80+ call sites)
                    //    
                    //    CAUTION -- investigated but NOT fully resolved: `ldc.b @(0x7:8,FP),EP` at
                    //    function entry loads a byte into the EP bank register, but EP is NEVER
                    //    referenced again anywhere in this function body (all real accesses go
                    //    through the plain word pointer at FP+8, i.e. `record` above). This looks
                    //    like vestigial/dead compiler-generated boilerplate (paired with the
                    //    `ldc.b 0x0,DP` cleanup at the very end) rather than functionally
                    //    significant banking -- but this was inferred from static analysis only,
                    //    not verified against a live trace. If EP turns out to matter after all,
                    //    the single-pointer prototype here would need revisiting.
                    //    
                    //    Algorithm: binary-search-like walk through axis_table to bracket
                    //    lookup_value between two adjacent breakpoints, then linear-interpolate
                    //    the fractional position (scaled by 0xff/255) and write the combined
                    //    {index, fraction} result back through value_ptr.
                    // 
                    //    [2026-08-16] Return type fixed from undefined2 to void. The old
                    // signature's
                    //    `return in_R0;` was a decompiler phantom-register artifact (register
                    // touched
                    //    during entry stack setup, never actually assigned a meaningful value in
                    // the
                    //    function body) -- confirmed by checking all 80+ real call sites in the C
                    //    export, every one of which invokes this as a bare void call
                    //    ((*axis_lookup_interp)();) and never uses a return value. The function's
                    //    real output is written through the record/value_ptr out-parameter
                    //    (*(ushort*)*record = uVar2, last line of the body), not via return.
                    // Verified
                    //    live via force_decompile after the prototype change -- in_R0 and the
                    //    spurious return are gone, body otherwise unchanged.
  count_minus_1 = record._2_2_[2];
  if (0xfe < count_minus_1) {
    count_minus_1 = 0xff;
  }
  count_minus_1 = count_minus_1 - 1;
  uVar6 = *(ushort *)*record._2_2_ & 0xff;
  if (count_minus_1 <= (*(ushort *)*record._2_2_ & 0xff)) {
    uVar6 = count_minus_1;
  }
  lookup_value = *(ushort *)record._2_2_[1];
  table_entry_ptr = record._2_2_ + uVar6 + 3;
  if (lookup_value < *table_entry_ptr || lookup_value == *table_entry_ptr) {
    while( true ) {
      if (*table_entry_ptr <= lookup_value) break;
      if (uVar6 == 0) break;
      uVar6 = uVar6 - 1;
      table_entry_ptr = table_entry_ptr + -1;
    }
  }
  else {
    while( true ) {
      if (lookup_value < *table_entry_ptr) break;
      if (count_minus_1 <= uVar6) goto LAB_00014791;
      uVar6 = uVar6 + 1;
      table_entry_ptr = table_entry_ptr + 1;
    }
    if (uVar6 != 0) {
      uVar6 = uVar6 - 1;
      table_entry_ptr = table_entry_ptr + -1;
    }
  }
LAB_00014791:
  if (lookup_value < (ushort)record._2_2_[3]) goto LAB_000147b7;
  if (count_minus_1 == uVar6) goto LAB_000147b7;
  iVar2 = (uint)(lookup_value - *table_entry_ptr) * 0xff;
  uVar5 = (undefined2)((uint)iVar2 >> 0x10);
  uVar4 = (undefined2)iVar2;
  lookup_value = table_entry_ptr[1] - *table_entry_ptr;
  uVar1 = CONCAT22(uVar5,uVar4);
  sVar3 = (short)(uVar1 % (uint)lookup_value);
  uVar5 = (short)(uVar1 / lookup_value);
  if (SUB21((ushort)sVar3 >> 0xf,0)) {
LAB_000147b1:
    count_minus_1 = count_minus_1 + 1;
  }
  else if (lookup_value <= (ushort)(sVar3 * 2)) goto LAB_000147b1;
  uVar6 = CONCAT11((char)count_minus_1,(char)(count_minus_1 >> 8)) | uVar6;
LAB_000147b7:
  *(ushort *)*record._2_2_ = uVar6;
  return;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: Restarted to delay deadcode elimination for space: register
// WARNING: Restarted to delay deadcode elimination for space: stack

ushort __stdcall_far_allstack axis_interp_lerp_u8frac(ushort y0,ushort y1,short frac)

{
  uint uVar1;
  ushort delta_abs;
  short sVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  short delta_scaled;
  ushort uVar5;
  undefined1 *puVar6;
  undefined1 auStack_82 [128];
  
  puVar6 = auStack_82 + 0x80;
  if (frac != 0) {
    if (frac._0_1_ == '\0') {
      delta_abs = y1 - y0;
      if (y1 < y0) {
        delta_abs = -delta_abs;
      }
      uVar5 = (ushort)(y1 < y0);
      uVar4 = (undefined2)((uint)delta_abs * (uint)(ushort)frac >> 0x10);
      uVar3 = (undefined2)((uint)delta_abs * (uint)(ushort)frac);
      uVar1 = CONCAT22(uVar4,uVar3);
      sVar2 = (short)(uVar1 % 0xff);
      uVar4 = (short)(uVar1 / 0xff);
      if (0xfe < (ushort)(sVar2 * 2)) {
        delta_scaled = delta_scaled + 1;
      }
      if (uVar5 == 0) {
        sVar2 = *(short *)(puVar6 + 6) + delta_scaled;
      }
      else {
        sVar2 = *(short *)(puVar6 + 6) - delta_scaled;
      }
      *(short *)(puVar6 + -2) = sVar2;
      y0 = *(ushort *)(puVar6 + -2);
    }
    else {
      y0 = y1;
    }
  }
  return y0;
}



undefined1 __stdcall_far_allstack read_indexed_byte_f1fc(byte bank,void *array_base)

{
  undefined1 in_stack_00000005;
  
  return *(undefined1 *)(_bank + (DAT_0001f1fc & 7));
}



// WARNING: Unknown calling convention

undefined2 table_read_word_indexed_f1fc(byte bank,void *array_base)

{
  undefined1 in_stack_00000005;
  
  return *(undefined2 *)(_bank + (DAT_0001f1fc & 7) * 2);
}



void __stdcall_far_2arg table_lookup_indexed(ushort ep_val,void *table)

{
  ushort *puVar1;
  
  puVar1 = (ushort *)((short)table + (DAT_0001f1fc & 7) * 4);
  table_lookup_interp(*puVar1,(void *)puVar1[1]);
  return;
}



ushort __stdcall_far_allstack table_read_indexed(byte bank,void *array_base)

{
  undefined1 in_stack_00000005;
  
  return *(ushort *)(_bank + (DAT_0001f1fc & 7) * 4);
}



// WARNING: Restarted to delay deadcode elimination for space: stack

void __stdcall_far_2arg table_3axis_interp_triple(ushort ep_val,void *table)

{
  ushort uVar1;
  undefined1 uVar3;
  ushort uVar2;
  ushort uVar4;
  short sVar5;
  short *psVar6;
  void *pvVar7;
  short sVar8;
  undefined1 *puVar9;
  undefined1 uVar10;
  undefined1 uVar11;
  short sVar12;
  undefined1 auStack_a [8];
  
  uVar11 = 0;
  uVar10 = 1;
  puVar9 = auStack_a + 8;
  sVar5 = *(short *)((int)table + 2);
  uVar2 = **(ushort **)((int)table + 4) >> 8;
  if (*(short *)table == 3) {
    sVar5 = *(short *)((int)table + 8) * (**(ushort **)((int)table + 6) & 0xff);
    sVar8 = (short)table + *(short *)(puVar9 + -4) * 2 + uVar2 * 2;
    psVar6 = (short *)(sVar8 + 10);
    sVar12 = *(short *)(puVar9 + -2);
    uVar3 = (undefined1)*(undefined2 *)(puVar9 + 6);
    uVar2 = *(short *)((int)sVar8 + 0xc) + sVar5;
    uVar1 = *psVar6 + sVar5;
    *(undefined2 *)(puVar9 + -8) = *(undefined2 *)(puVar9 + 6);
    *(short **)(puVar9 + -6) = psVar6;
    uVar2 = axis_interp_lerp_u8frac(uVar1,uVar2,sVar12);
    psVar6 = (short *)(*(short *)(puVar9 + -6) +
                      *(short *)((int)*(undefined2 *)(puVar9 + 8) + 8) * 2);
    *(short **)(puVar9 + -6) = psVar6;
    sVar12 = *(short *)(puVar9 + -2);
    uVar1 = psVar6[1] + sVar5;
    uVar4 = *psVar6 + sVar5;
    *(ushort *)(puVar9 + -6) = uVar2;
    uVar4 = axis_interp_lerp_u8frac(uVar4,uVar1,sVar12);
    uVar2 = **(ushort **)((int)*(undefined2 *)(puVar9 + 8) + 6) >> 8;
    uVar1 = *(ushort *)(puVar9 + -6);
  }
  else {
    pvVar7 = (void *)((short)table + (**(ushort **)((int)table + 4) & 0xff) * 2);
    uVar4 = *(short *)((int)pvVar7 + 8) + sVar5;
    uVar1 = *(short *)((int)pvVar7 + 6) + sVar5;
  }
  axis_interp_lerp_u8frac(uVar1,uVar4,uVar2);
  return;
}



// WARNING: Unknown calling convention

void table_3axis_interp_triple_wrapper(byte bank,void *array_base)

{
  ushort *puVar1;
  undefined1 in_stack_00000005;
  
  puVar1 = (ushort *)(_bank + (DAT_0001f1fc & 7) * 4);
  table_3axis_interp_triple(*puVar1,(void *)puVar1[1]);
  return;
}



// WARNING: Unknown calling convention

undefined2 gear_indexed_table_lookup_f1fc(byte bank,void *array_base)

{
  undefined1 in_stack_00000005;
  
  return *(undefined2 *)(_bank + (DAT_0001f1fc & 7) * 4);
}



// [DOCUMENTED 2026-08-07, CORRECTED same session] Boot-time init: copies
// 0x100 (256) bytes from ROM bank 2:0xcf80 into RAM 0xf862 via
// memcpy_banked, AFTER subsystem_unconditional_init_chain (0x20d3c, this
// function's caller) has zeroed the whole surrounding RAM range
// (0xef64-0xf974) via zero_ram_range. So the 0xFF default (see below) is a
// deliberate cold-boot state, confirmed not a static-analysis artifact.
// 
// This is the default/seed data for the EEPROM-backed MUT logging config
// record consumed by eeprom_backup_table_write_dispatch (0x149f5), which
// in turn feeds the RequestID lists used by
// mut_configurable_reqid_backup_snapshot_5word (0x1504b) and
// mut_configurable_reqid_periodic_snapshot_8word (0x14edd).
// 
// CHECKED 2026-08-07: read ROM 0x2cf80, length 0x100 directly -- ALL 256
// BYTES ARE 0xFF (unprogrammed on this ROM). eeprom_backup_table_write_
// dispatch's checksum/terminator check (expects 0x0D at record offset
// 0x33) will fail against this blank source at cold boot.
// 
// CORRECTION: do NOT read this as "the mechanism is dead." eeprom_backup_
// table_write_dispatch is called from channel_dispatch_and_snapshot_update
// -> main_loop (0x20a80), i.e. it runs on EVERY main loop pass during
// normal operation, not just once at boot. If anything writes valid data
// (terminator 0x0D, matching checksum) into RAM 0xf862+ at runtime -- a
// real EEPROM chip, a live SCI1 upload, etc, no such writer identified in
// this project yet -- this dispatcher will pick it up on its next pass.
// The all-0xFF ROM default only describes a factory-fresh/unconfigured
// cold-boot state, not whether the mechanism is used in practice. See
// mut_verification_status.md "CORRECTION: DEAD END CALL WAS PREMATURE"
// (2026-08-07).
// 
// NOT YET DONE: find what else can write RAM 0xf862+ at runtime (search
// for an EEPROM/I2C/SPI driver in this ROM -- not yet attempted).

void init_copy_const_block_via_memcpy_banked(void)

{
  ushort uStack_4;
  
  uStack_4 = 0;
  do {
    *(undefined2 *)((int)uStack_4 + -0x7b6) = 0;
    uStack_4 = uStack_4 + 2;
  } while (uStack_4 < 2);
  memcpy_banked(2,0xcf80,0,0xf862,0x100);
  return;
}



// WARNING: Removing unreachable block (ram,0x00014b20)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// WARNING: Restarted to delay deadcode elimination for space: stack
// [DOCUMENTED 2026-08-07] Validates a checksummed config record at RAM
// 0xf814/0xf862 (terminator byte 0x0D at offset 0x33, checksum at 0x32)
// and, on a match, populates the RequestID lists consumed by
// mut_configurable_reqid_backup_snapshot_5word (0x1504b) and
// mut_configurable_reqid_periodic_snapshot_8word (0x14edd). Called from
// channel_dispatch_and_snapshot_update -> main_loop (0x20a80), i.e. runs
// every main loop pass during normal operation, not just at boot.
// 
// WORKING CONCLUSION (2026-08-07): this ECU almost certainly has no
// onboard EEPROM chip. Three independent searches (RAM-address xref
// search, whole-ROM byte-pattern sweep for the literal 0xf862 operand,
// and a structural review of every port-I/O function for a bit-banged
// serial-protocol driver) all found nothing besides
// init_copy_const_block_via_memcpy_banked (0x149c8, the boot-time loader
// of the -- all 0xFF, unprogrammed -- ROM default) that ever touches this
// record. This ECU already has substantial on-chip FLASH for non-volatile
// storage (flash_write_marked_blocks_loop etc), making a separate EEPROM
// largely redundant, and no corroborating driver/usage pattern was found
// anywhere. Treating this mechanism as VESTIGIAL -- inherited from a
// shared platform codebase, never actually populated on real hardware.
// The two consumer functions will never find valid data in practice and
// will never log anything via this path on a real vehicle.
// See mut_verification_status.md "WORKING CONCLUSION: NO EEPROM ON THIS
// ECU -- MECHANISM IS VESTIGIAL" (2026-08-07).

void eeprom_backup_table_write_dispatch(ushort param_fp1)

{
  ushort uVar1;
  bool bVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  undefined1 in_ram_0000389d;
  undefined2 in_ram_0000389e;
  ushort uVar5;
  char cVar7;
  short sVar6;
  ushort in_R2;
  ushort unaff_R3;
  ushort uVar8;
  short unaff_R5;
  short sVar9;
  byte bVar10;
  short in_stack_00000004;
  int in_stack_0000ffe0;
  ushort uStack_12;
  ushort uStack_6;
  ushort uStack_4;
  
  uVar8 = _uRam0001f818;
  uVar4 = uRam0001f819;
  uVar3 = uRam0001f818;
  bVar10 = 1;
  if (in_stack_00000004 != 0) {
    return;
  }
  uVar5 = DAT_0000f84a;
  if ((uVar5 & 0x101) != 0) {
    return;
  }
  uStack_12 = DAT_0000f84a;
  bVar2 = true;
  if ((uStack_12 & 0x200) == 0) {
    return;
  }
  if ((uStack_12 & 0x1000) != 0) {
    uStack_12 = uStack_12 | 1;
    goto LAB_00014d5b;
  }
  if ((uStack_12 & 0x800) != 0) {
    if ((uStack_12 & 0x40) == 0) {
      uStack_12 = uStack_12 | 0x1000;
    }
    else {
      uStack_12 = 0;
    }
    goto LAB_00014d5b;
  }
  if ((uStack_12 & 0x400) == 0) {
    bVar10 = 1;
    sVar9 = (*(code *)(uint)CONCAT12((char)PTR_LAB_000138a8._0_2_,PTR_LAB_000138a8._2_2_))();
    if (sVar9 == 0) {
      bVar2 = false;
    }
    else {
      sVar9 = in_stack_00000004 * 0x34;
      sVar6 = (short)&DAT_0000f814 + sVar9;
      if ((*(char *)((int)sVar6 + 0x33) == '\r') &&
         (cVar7 = byte_sum_banked('\0',(char *)CONCAT22((short)&DAT_0000f815 + sVar9,0x31),in_R2),
         cVar7 == *(char *)((int)sVar6 + 0x32))) {
        cVar7 = *(char *)((int)sVar6 + 1);
        uStack_12 = uStack_12 & 0xdfff;
        if (cVar7 == -0x7f) {
          uStack_12 = uStack_12 | 2;
        }
        else if (cVar7 == -0x7e) {
          uStack_12 = uStack_12 | 4;
        }
        else if (cVar7 == -0x7d) {
          uStack_12 = uStack_12 | 8;
        }
        else if (cVar7 == -0x7c) {
          uStack_12 = uStack_12 | 0x2002;
        }
        else if (cVar7 == -0x7b) {
          uStack_12 = uStack_12 | 0x2010;
        }
        else if (cVar7 == -0x7a) {
          uStack_12 = uStack_12 | 0x2020;
        }
        else if (cVar7 == -0x79) {
          uStack_12 = uStack_12 | 0x40;
        }
        else {
          uStack_12 = 0x100;
        }
        if (((uStack_12 & 0x2000) != 0) &&
           ((in_stack_00000004 == 0 || ((*(ushort *)((uint)bVar10 << 0x10 | 0xf84a) & 0x2000) == 0))
           )) {
          *(ushort *)((uint)bVar10 << 0x10 | 0xf5b4) = (ushort)*(byte *)((int)sVar6 + 0x2e);
          *(ushort *)((uint)bVar10 << 0x10 | 0xf5b6) = (ushort)*(byte *)((int)sVar6 + 0x2f);
          *(ushort *)((uint)bVar10 << 0x10 | 0xf5b8) = (ushort)*(byte *)((int)sVar6 + 0x30);
          *(ushort *)((uint)bVar10 << 0x10 | 0xf5ba) = (ushort)*(byte *)((int)sVar6 + 0x31);
        }
        *(undefined2 *)((int)(in_stack_00000004 << 1) + -0x7b4) = 0xffff;
        if (((uStack_12 & 0x40) != 0) &&
           (*(char *)((int)sVar6 + 3) == '\0' && *(char *)((int)sVar6 + 2) == '\0')) {
          uVar8 = CONCAT11(*(undefined1 *)((int)sVar6 + 4),*(undefined1 *)((int)sVar6 + 5));
          uStack_4 = 0x2c;
          uVar5 = sat_add_u16(uVar8,0x2c);
          if (0x100 < uVar5) {
            uStack_4 = sat_sub_u16(0x100,uVar8);
          }
          memcpy_banked(0,(short)&UNK_0000f81a + sVar9,0,(short)&DAT_0000f862 + uVar8,uStack_4);
        }
        uStack_12 = uStack_12 | 0x400;
      }
      else {
        (*(code *)(uint)CONCAT12((char)*(undefined2 *)&mut_snapshot_channel_callback_table,
                                 *(undefined2 *)((int)0x3898 + 2)))();
      }
    }
    goto LAB_00014d5b;
  }
  DAT_0001f7e1 = 5;
  if (((uStack_12 & 0x40) == 0) || (CONCAT11(uRam0001f816,uRam0001f817) != 1)) {
    if (((uStack_12 & 0x40) == 0) || (CONCAT11(uRam0001f816,uRam0001f817) != 2)) {
      uRam0001f7e2 = 0x80;
      uRam0001f7e3 = 0;
      uStack_6 = unaff_R3;
      goto LAB_00014b9c;
    }
    uRam0001f7e2 = 0;
    uRam0001f7e3 = 2;
    uRam0001f7e4 = 0;
    uRam0001f7e5 = 0;
    uVar8 = (ushort)&DAT_0000f862;
    sVar9 = 7;
    do {
      uVar8 = uVar8 >> 1;
      sVar9 = sVar9 + -1;
    } while (sVar9 != -1);
    uRam0001f7e6 = (undefined1)uVar8;
    uRam0001f7e7 = 0x62;
    uVar8 = 2;
    sVar9 = 7;
    do {
      uVar8 = uVar8 >> 1;
      if (uVar8 == 0) break;
      sVar9 = sVar9 + -1;
    } while (sVar9 != -1);
    uRam0001f7e8 = (undefined1)uVar8;
    uRam0001f7e9 = 2;
    uVar8 = 0xcf80;
    sVar9 = 7;
    uVar5 = 2;
    do {
      uVar1 = uVar8 >> 1;
      uVar8 = uVar1 | (ushort)((byte)uVar5 & 1) << 0xf;
      sVar9 = sVar9 + -1;
      uVar5 = uVar5 >> 1;
    } while (sVar9 != -1);
    uRam0001f7ea = (undefined1)uVar1;
    uRam0001f7eb = 0x80;
    uRam0001f7ec = 1;
    uRam0001f7ed = 0;
  }
  else {
    uStack_6 = 0x2c;
    uVar5 = sat_add_u16(_uRam0001f818,0x2c);
    if (0x100 < uVar5) {
      uStack_6 = sat_sub_u16(0x100,uVar8);
    }
    *(undefined1 *)((int)0xf7e0 + 2) = 0;
    *(undefined1 *)((int)0xf7e0 + 3) = 1;
    *(undefined1 *)((int)0xf7e0 + 4) = uVar3;
    *(undefined1 *)((int)0xf7e0 + 5) = uVar4;
    in_R2 = (ushort)&UNK_0000f7e6;
    in_stack_0000ffe0 = (uint)((short)&DAT_0000f862 + uVar8) << 0x10;
LAB_00014b9c:
    memcpy_banked(0,(ushort)((uint)in_stack_0000ffe0 >> 0x10),(ushort)in_stack_0000ffe0,in_R2,
                  uStack_6);
  }
  cVar7 = byte_sum_banked('\0',(char *)0xf7e10031,unaff_R5);
  *(char *)((int)0xf7e0 + 0x32) = cVar7;
  *(undefined1 *)((int)0xf7e0 + 0x33) = 0xd;
  bVar10 = 0;
  (*(code *)(uint)CONCAT12(in_ram_0000389d,in_ram_0000389e))();
  *(undefined2 *)((int)(in_stack_00000004 << 1) + -0x7b8) =
       *(undefined2 *)((uint)bVar10 << 0x10 | 0xff46);
  uStack_12 = uStack_12 | 0x800;
  bVar10 = 0;
LAB_00014d5b:
  if (bVar2) {
    *(ushort *)((int)(in_stack_00000004 << 1) + -0x7b6) = uStack_12;
  }
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack
// [PROTOTYPE ADDED 2026-07-23] Per-channel status/handler dispatcher, called
// from channel_event_or_periodic_dispatch (0x19e81) with channel_index 0 or 1
// -- a 2-channel selector (e.g. dual-channel hardware like 2x knock or 2x O2,
// consistent with "channel" naming elsewhere in this ROM). NOT the same table
// as the 0x13898 false-positive flagged elsewhere in review.md -- this is a
// separate, confirmed-live 20-byte-stride (0x14) table at base 0x3898,
// verified via disassembly (mulxu.w 0x14 / mov:i #0x3898) and confirmed live
// via 2 real call sites in channel_event_or_periodic_dispatch.
// 
// Each 20-byte table entry holds 3 function pointers used here (offsets
// 0x0/0x2, 0x8/0xa, 0xc/0xe -- CONCAT12 24-bit far-call reconstruction);
// remaining ~12 bytes of the entry's stride are untouched by this function,
// purpose not yet traced. Reads/writes a status word from a parallel array
// at base -0x7b6 (word-indexed by channel_index), gated by bit0/bit8 (0x101)
// before running the handler chain, and updates bits 0x200/0x400 in a
// function-static-looking flag word during handler execution.

void __stdcall_far_allstack channel_validity_gated_handler_dispatch(ushort channel_index)

{
  short handler_result;
  undefined1 bank_one;
  ushort uStack_6;
  
  bank_one = 1;
  if (channel_index != 0) {
    return;
  }
  uStack_6 = DAT_0000f84a;
  if ((((uStack_6 & 0x101) == 0) &&
      (handler_result =
            (*(code *)(uint)CONCAT12((char)PTR_LAB_000138a4._0_2_,PTR_LAB_000138a4._2_2_))(),
      handler_result == 0)) &&
     (((uStack_6 & 0x200) == 0 ||
      (((uStack_6 & 0x400) == 0 &&
       (handler_result =
             (*(code *)(uint)CONCAT12((char)*(undefined2 *)((int)0x3898 + 8),
                                      *(undefined2 *)((int)0x3898 + 10)))(), handler_result != 0))))
     )) {
    (*(code *)(uint)CONCAT12((char)*(undefined2 *)&mut_snapshot_channel_callback_table,
                             *(undefined2 *)((int)0x3898 + 2)))();
    uStack_6 = 0x200;
  }
  *(ushort *)((int)0 + -0x7b6) = uStack_6;
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack

void adc_backup_snapshot_write_if_time_elapsed(ushort param_fp1,ushort param_fp2)

{
  ushort uVar1;
  ushort *puVar2;
  short sVar3;
  char cVar4;
  short extraout_R2;
  short unaff_R3;
  short sVar5;
  undefined1 uVar6;
  byte bVar7;
  short in_stack_00000004;
  
  uVar6 = 1;
  uVar1 = (ushort)unaff_R3 >> 8;
  puVar2 = (ushort *)(param_fp1 * 6 + 0x38ac);
  if ((in_stack_00000004 == 0) && (sVar3 = Ram0000f848, *puVar2 <= (ushort)(DAT_0001ff46 - sVar3)))
  {
    sVar3 = (*(code *)(uint)CONCAT12((char)PTR_LAB_000138a4._0_2_,PTR_LAB_000138a4._2_2_))();
    if (sVar3 == 0) {
      sVar3 = (ushort)*(byte *)((int)puVar2 + 3) * CONCAT11((char)uVar1,*(byte *)(puVar2 + 2));
      adc_sensor_convert_multi
                ((uchar *)(uint)(ushort)((short)&UNK_0000f816 + in_stack_00000004 * 0x34),
                 &DAT_0000f7e2,(ushort)*(byte *)((int)puVar2 + 3),(ushort)*(byte *)(puVar2 + 2));
      *(undefined1 *)((int)0xf7e0 + 1) = *(undefined1 *)(puVar2 + 1);
      sVar5 = sVar3 + 1;
      cVar4 = byte_sum_banked('\0',(char *)CONCAT22((short)&UNK_0000f7e1,sVar5),unaff_R3);
      *(char *)(extraout_R2 + sVar3) = cVar4;
      *(undefined1 *)(extraout_R2 + sVar5) = 0xd;
      sVar3 = (short)&mut_snapshot_channel_callback_table + in_stack_00000004 * 0x14;
      bVar7 = 0;
      (*(code *)(uint)CONCAT12((char)*(undefined2 *)((int)sVar3 + 4),*(undefined2 *)((int)sVar3 + 6)
                              ))();
      *(undefined2 *)((int)(in_stack_00000004 << 1) + -0x7b8) =
           *(undefined2 *)((uint)bVar7 << 0x10 | 0xff46);
    }
    else {
      *(undefined2 *)((int)(in_stack_00000004 << 1) + -0x7b6) = 0;
    }
  }
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack
// [RENAMED + DOCUMENTED 2026-08-07, CORRECTED same session] Formerly
// channel_periodic_update_ringbuf_snapshot.
// 
// CORRECTION: an earlier pass this session claimed the RequestIDs used by
// this function are "fixed, hardcoded literals baked into the call sites."
// That was WRONG -- verified by disassembling the actual function body
// (ghidra:disassemble_function) rather than trusting the decompiler's
// collapsed view. The truth: each of the 8 adc_sensor_convert_single calls
// reads its RequestID from a RAM table at 0xf814 + channel*0x34 (register
// R2 walks through offsets 0x0,0x2,0x4,0x6,0x8,0xa,0xc,0xe of that record),
// NOT a literal. That RAM record is itself populated/validated at runtime
// by eeprom_backup_table_write_dispatch (0x149f5) via a checksummed
// EEPROM-style blob (terminator byte 0x0D at offset 0x33, checksum at
// 0x32, byte_sum_banked-validated) copied in from a staging buffer at
// 0xf862+. This strongly suggests the RequestID list for each logging
// "channel" is CONFIGURABLE/DOWNLOADABLE at runtime (e.g. via the same
// SCI1 serial link documented elsewhere in this project), not a fixed ROM
// constant -- i.e. this mechanism may be closer to a real "MUT data-list
// request" facility than first assessed, just indirected through an
// EEPROM-backed RAM table instead of taking the RequestID directly off the
// wire byte-by-byte.
// 
// Internal periodic BLACK-BOX/BACKUP snapshot logger, indexed by an
// internal "channel" number (param_4, one of only 2 possible values gated
// by config flags F84A/F84C). Builds an 8-entry record by calling
// adc_sensor_convert_single (0x171c3) 8 times with RequestIDs pulled from
// RAM 0xf814+channel*0x34 (see correction above), computes a checksum via
// byte_sum_banked, writes into ring buffer mut_snapshot_ringbuf_base
// (0xf7e0 + channel*0x34), then invokes a callback from
// mut_snapshot_channel_callback_table (0x3898 + channel*0x14).
// 
// Near-identical twin of mut_fixed_reqid_backup_snapshot_5word (0x1504b,
// NAME ALSO NEEDS RE-EXAMINATION per this same correction -- not yet
// re-verified this session, do that before trusting its "fixed_reqid" name
// either).
// 
// See mut_verification_status.md "MAJOR FINDING ... CORRECTED" (2026-08-07)
// for the walk-back of the original claim and what replaced it.
// 
// NOT YET DONE: read the ACTUAL runtime contents of the 0xf814 RAM table
// (not possible from a static ROM dump -- RAM is zero at rest; would need
// a live capture or to find what populates 0xf862+ at boot/config time) to
// learn the real RequestID list for each channel; trace what writes/
// uploads the 0xf862+ staging buffer (likely the real "next step" for
// understanding configurable MUT logging); re-verify
// mut_fixed_reqid_backup_snapshot_5word's RequestID source the same way.

void mut_configurable_reqid_periodic_snapshot_8word(ushort param_fp1)

{
  short sVar1;
  undefined1 extraout_var;
  undefined1 extraout_var_00;
  undefined1 extraout_var_01;
  undefined1 extraout_var_02;
  undefined1 extraout_var_03;
  undefined1 extraout_var_04;
  short sVar2;
  short sVar3;
  undefined2 *puVar4;
  undefined2 uVar5;
  char cVar6;
  short in_R2;
  undefined1 uVar7;
  byte bVar8;
  short in_stack_00000004;
  ushort uStack_4;
  
  uVar7 = 1;
  if ((in_stack_00000004 == 0) && (sVar2 = Ram0000f848, 0x243 < (ushort)(DAT_0001ff46 - sVar2))) {
    sVar2 = (*(code *)(uint)CONCAT12((char)PTR_LAB_000138a4._0_2_,PTR_LAB_000138a4._2_2_))();
    if (sVar2 == 0) {
      sVar2 = in_stack_00000004 * 0x34;
      sVar3 = (short)&mut_snapshot_ringbuf_base + sVar2;
      puVar4 = (undefined2 *)((short)&DAT_0000f7e2 + sVar2);
      uStack_4 = *(short *)((int)(in_stack_00000004 << 1) + -0x7b4) + 1;
      if (7 < uStack_4) {
        uStack_4 = 0;
      }
      sVar1 = in_stack_00000004 << 1;
      *(ushort *)((int)sVar1 + -0x7b4) = uStack_4;
      uVar5 = (*adc_sensor_convert_single)();
      *puVar4 = uVar5;
      uVar5 = (*adc_sensor_convert_single)();
      puVar4[1] = uVar5;
      (*adc_sensor_convert_single)();
      *(undefined1 *)(puVar4 + 2) = extraout_var;
      (*adc_sensor_convert_single)();
      *(undefined1 *)((int)puVar4 + 5) = extraout_var_00;
      (*adc_sensor_convert_single)();
      *(undefined1 *)(puVar4 + 3) = extraout_var_01;
      (*adc_sensor_convert_single)();
      *(undefined1 *)((int)puVar4 + 7) = extraout_var_02;
      (*adc_sensor_convert_single)();
      *(undefined1 *)(puVar4 + 4) = extraout_var_03;
      (*adc_sensor_convert_single)();
      *(undefined1 *)((int)puVar4 + 9) = extraout_var_04;
      *(byte *)((int)sVar3 + 1) = (byte)uStack_4 | 0x20;
      cVar6 = byte_sum_banked('\0',(char *)CONCAT22((short)&UNK_0000f7e1 + sVar2,0xb),in_R2);
      *(char *)((int)sVar3 + 0xc) = cVar6;
      *(undefined1 *)((int)sVar3 + 0xd) = 0xd;
      sVar2 = (short)&mut_snapshot_channel_callback_table + in_stack_00000004 * 0x14;
      bVar8 = 0;
      (*(code *)(uint)CONCAT12((char)*(undefined2 *)((int)sVar2 + 4),*(undefined2 *)((int)sVar2 + 6)
                              ))();
      *(undefined2 *)((int)sVar1 + -0x7b8) = *(undefined2 *)((uint)bVar8 << 0x10 | 0xff46);
    }
    else {
      *(undefined2 *)((int)(in_stack_00000004 << 1) + -0x7b6) = 0;
    }
  }
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack
// [RENAMED 2026-08-07, then CORRECTED same session] Formerly named
// mut_fixed_reqid_backup_snapshot_5word -- that name was WRONG, based on a
// decompiler read that hid the real addressing. Verified via direct
// disassemble_function: all 5 adc_sensor_convert_single (0x171c3) calls
// read their RequestID from RAM 0xf814 + channel*0x34 (offsets
// 0x0/0x2/0x4/0x6/0x8), NOT literals. See twin function
// mut_configurable_reqid_periodic_snapshot_8word (0x14edd) for the fuller
// writeup of the shared mechanism (same RAM record, populated/validated by
// eeprom_backup_table_write_dispatch @0x149f5 from a checksummed
// EEPROM-style blob) -- everything in that comment applies here too, just
// with 5 record entries instead of 8.
// 
// Ring buffer: mut_snapshot_ringbuf_base + channel*0x34.
// Callback table: mut_snapshot_channel_callback_table + channel*0x14.
// 
// See mut_verification_status.md "MAJOR FINDING ... CORRECTED" (2026-08-07).

void mut_configurable_reqid_backup_snapshot_5word(ushort param_fp1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined2 uVar4;
  char cVar5;
  short in_R2;
  undefined2 extraout_R2;
  undefined2 extraout_R2_00;
  undefined2 extraout_R2_01;
  undefined2 extraout_R2_02;
  undefined1 uVar6;
  byte bVar7;
  short in_stack_00000004;
  ushort uStack_4;
  
  uVar6 = 1;
  if ((in_stack_00000004 == 0) && (sVar2 = Ram0000f848, 0x243 < (ushort)(DAT_0001ff46 - sVar2))) {
    sVar2 = (*(code *)(uint)CONCAT12((char)PTR_LAB_000138a4._0_2_,PTR_LAB_000138a4._2_2_))();
    if (sVar2 == 0) {
      sVar2 = in_stack_00000004 * 0x34;
      sVar3 = (short)&mut_snapshot_ringbuf_base + sVar2;
      uStack_4 = *(short *)((int)(in_stack_00000004 << 1) + -0x7b4) + 1;
      if (2 < uStack_4) {
        uStack_4 = 0;
      }
      sVar1 = in_stack_00000004 << 1;
      *(ushort *)((int)sVar1 + -0x7b4) = uStack_4;
      uVar4 = (*adc_sensor_convert_single)();
      *(undefined2 *)((short)&DAT_0000f7e2 + sVar2) = uVar4;
      uVar4 = (*adc_sensor_convert_single)();
      *(undefined2 *)((int)extraout_R2 + 2) = uVar4;
      uVar4 = (*adc_sensor_convert_single)();
      *(undefined2 *)((int)extraout_R2_00 + 4) = uVar4;
      uVar4 = (*adc_sensor_convert_single)();
      *(undefined2 *)((int)extraout_R2_01 + 6) = uVar4;
      uVar4 = (*adc_sensor_convert_single)();
      *(undefined2 *)((int)extraout_R2_02 + 8) = uVar4;
      *(byte *)((int)sVar3 + 1) = (byte)uStack_4 | 0x30;
      cVar5 = byte_sum_banked('\0',(char *)CONCAT22((short)&UNK_0000f7e1 + sVar2,0xb),in_R2);
      *(char *)((int)sVar3 + 0xc) = cVar5;
      *(undefined1 *)((int)sVar3 + 0xd) = 0xd;
      sVar2 = (short)&mut_snapshot_channel_callback_table + in_stack_00000004 * 0x14;
      bVar7 = 0;
      (*(code *)(uint)CONCAT12((char)*(undefined2 *)((int)sVar2 + 4),*(undefined2 *)((int)sVar2 + 6)
                              ))();
      *(undefined2 *)((int)sVar1 + -0x7b8) = *(undefined2 *)((uint)bVar7 << 0x10 | 0xff46);
    }
    else {
      *(undefined2 *)((int)(in_stack_00000004 << 1) + -0x7b6) = 0;
    }
  }
  return;
}



// WARNING: This function may have set the stack pointer

void entry(void)

{
  byte bVar1;
  
  while (bVar1 = FLMCR, (bVar1 & 0x80) == 0) {
    DAT_0000fe7e = 0x5196;
    DAT_0000fe7c = 1;
    main_loop();
  }
  flash_boot_init();
  return;
}



void stub_empty_0x1519e(void)

{
  return;
}



void noop_return(void)

{
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void peripheral_register_cold_init(void)

{
  undefined1 uVar1;
  byte bVar2;
  byte bVar3;
  undefined2 uStack0004;
  undefined2 uStack0006;
  undefined2 uStack0008;
  undefined2 uStack000a;
  
  bVar3 = 1;
  (*peripheral_block_ff15_cold_init)();
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe98) = 0;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe99) = 0;
  uVar1 = 3;
  if (*(char *)((uint)bVar3 << 0x10 | 0x2e6) != '\0') {
    uVar1 = 0x23;
  }
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe82) = uVar1;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe83) = 0;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe86) = 0x30;
  bVar2 = 0x20;
  if (*(char *)((uint)bVar3 << 0x10 | 0x2e4) == '\x02') {
    bVar2 = 0x30;
  }
  if (*(char *)((uint)bVar3 << 0x10 | 0x2ed) != '\0') {
    if ((*(byte *)((uint)bVar3 << 0x10 | 0x2ed) & 4) == 0) {
      if ((*(byte *)((uint)bVar3 << 0x10 | 0x2ed) & 8) != 0) goto LAB_000151ef;
    }
    else if ((*(byte *)((uint)bVar3 << 0x10 | 0x2ed) & 8) == 0) goto LAB_000151ef;
    bVar2 = bVar2 | 0x80;
  }
LAB_000151ef:
  *(byte *)((uint)bVar3 << 0x10 | 0xfe87) = bVar2;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe8a) = 0xa0;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe8b) = 0x14;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe8e) = 0x14;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe93) = 0x40;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe96) = 0;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe97) = 0x30;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe80) = 0xff;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe81) = 0xff;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe84) = 0x3f;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe85) = 0xf8;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe88) = 0xad;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe89) = 0x15;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe8c) = 0xd4;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe91) = 0x5f;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe94) = 0;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfe95) = 0xff;
  (*init_reg_feb8_feb9)();
  (*init_state_block_f97a)();
  (*timer_block_cold_init)();
  timer_ff41_ff44_ff4a_cold_init();
  peripheral_reg_ff1d_cold_init();
  stub_empty_0x16158();
  peripheral_block_ff90_cold_init();
  uStack0004 = 0x5263;
  init_state_block_ff21();
  uStack0006 = 0x5267;
  uStack0004 = 1;
  timer_ff51_ff54_arm_init();
  uStack0008 = 0x526b;
  uStack0006 = 1;
  (*serial_fef4_cold_init)();
  uStack0008 = 0x526f;
  uStack0006 = 1;
  (*peripheral_block_fec0_fed0_cold_init)();
  uStack0008 = 0x5273;
  uStack0006 = 1;
  init_state_sentinels_f5d2();
  uStack000a = 0x5277;
  uStack0008 = 1;
  (*peripheral_block_ff70_ff88_cold_init)();
  uStack000a = 0x527b;
  uStack0008 = 1;
  (*peripheral_block_ff00_cold_init)();
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfedb) = 0x81;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfeda) = 0x60;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void peripheral_register_warm_init(void)

{
  ushort uVar1;
  byte bVar2;
  undefined2 uStack0004;
  undefined2 uStack0006;
  undefined2 uStack0008;
  undefined2 uStack000a;
  undefined2 uStack000c;
  undefined2 uStack000e;
  undefined2 uStack0010;
  
  bVar2 = 1;
  (*peripheral_block_ff15_warm_init)();
  (*noop_return_void)();
  init_flags_ff51_52();
  timer_block_cold_init_b();
  (*timer_ff41_ff42_warm_init)();
  peripheral_reg_ff1d_cold_init();
  stub_empty_0x1615a();
  uStack0004 = 0x52ae;
  peripheral_block_ff90_warm_init();
  uStack0006 = 0x52b2;
  uStack0004 = 1;
  peripheral_block_ff21_warm_init();
  uStack0008 = 0x52b6;
  uStack0006 = 1;
  timer_ff51_ff52_disarm();
  uStack000a = 0x52ba;
  uStack0008 = 1;
  serial_fef4_warm_init();
  uStack000c = 0x52be;
  uStack000a = 1;
  peripheral_fec0_fed0_state_update();
  uStack000e = 0x52c2;
  uStack000c = 1;
  (*peripheral_ff42_ff62_enable)();
  uStack000e = 0x52c6;
  uStack000c = 1;
  uVar1 = (*peripheral_block_ff70_ff88_warm_init)();
  uStack000e = 0x52ca;
  uStack000c = 1;
  peripheral_block_ff00_warm_init(uVar1);
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe98) = 0;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe99) = 0;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe80) = 0xff;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe81) = 0xff;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe84) = 0x3f;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe85) = 0xf8;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe88) = 0xad;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe89) = 0x15;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe8c) = 0xd4;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe91) = 0x5f;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe94) = 0;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfe95) = 0xff;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfedb) = 0x81;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfeda) = 0x60;
  uStack0010 = 0x5315;
  uStack000e = 1;
  (*update_flag_f9a0_bit6_cond)();
  return;
}



void update_flag_f9a0_bit6_cond(void)

{
  if ((DAT_000102e2 == '\0') || ((DAT_0001f0ea & 0x8000) != 0)) {
    pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 & 0xffbf;
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xcf;
  }
  else {
    pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 | 0x40;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void compose_status_words_f0f6_fc(void)

{
  ushort uVar1;
  
  if ((DAT_0001f0d8 & 4) == 0) {
    uVar1 = DAT_0001f0f6 | 1;
  }
  else {
    uVar1 = DAT_0001f0f6 & 0xfffe;
  }
  if ((DAT_0001fe96 & 0x80) == 0) {
    uVar1 = uVar1 | 2;
  }
  else {
    uVar1 = uVar1 & 0xfffd;
  }
  if ((DAT_0001f0d9 & 0x10) == 0) {
    uVar1 = uVar1 & 0xfff7;
  }
  else {
    uVar1 = uVar1 | 8;
  }
  if ((DAT_0001f0d9 & 4) == 0) {
    uVar1 = uVar1 | 0x10;
  }
  else {
    uVar1 = uVar1 & 0xffef;
  }
  if ((DAT_0001f0da & 8) == 0) {
    uVar1 = uVar1 & 0xffdf;
  }
  else {
    uVar1 = uVar1 | 0x20;
  }
  if ((DAT_0001f0d8 & 1) == 0) {
    uVar1 = uVar1 | 0x40;
  }
  else {
    uVar1 = uVar1 & 0xffbf;
  }
  if ((DAT_0001f0db & 1) == 0) {
    uVar1 = uVar1 & 0xff7f;
  }
  else {
    uVar1 = uVar1 | 0x80;
  }
  if ((DAT_0001f0d9 & 8) == 0) {
    uVar1 = uVar1 & 0xfeff;
  }
  else {
    uVar1 = uVar1 | 0x100;
  }
  if ((DAT_0001fe8b & 8) == 0) {
    uVar1 = uVar1 | 0x200;
  }
  else {
    uVar1 = uVar1 & 0xfdff;
  }
  if ((DAT_0001fe8e & 2) == 0) {
    uVar1 = uVar1 | 0x400;
  }
  else {
    uVar1 = uVar1 & 0xfbff;
  }
  if ((DAT_0001f0db & 4) == 0) {
    uVar1 = uVar1 & 0xf7ff;
  }
  else {
    uVar1 = uVar1 | 0x800;
  }
  if ((DAT_0001f0d8 & 8) == 0) {
    uVar1 = uVar1 & 0xefff;
  }
  else {
    uVar1 = uVar1 | 0x1000;
  }
  if ((DAT_0001f0d8 & 0x10) == 0) {
    uVar1 = uVar1 & 0xdfff;
  }
  else {
    uVar1 = uVar1 | 0x2000;
  }
  if ((DAT_0001f0db & 2) == 0) {
    uVar1 = uVar1 | 0x4000;
  }
  else {
    uVar1 = uVar1 & 0xbfff;
  }
  if ((DAT_0001f0da & 0x10) == 0) {
    DAT_0001f0f6 = uVar1 & 0x7fff;
  }
  else {
    DAT_0001f0f6 = uVar1 | 0x8000;
  }
  if ((DAT_0001f0d9 & 8) == 0) {
    uVar1 = DAT_0001f0fc & 0xfffe;
  }
  else {
    uVar1 = DAT_0001f0fc | 1;
  }
  if ((DAT_0001f0da & 4) == 0) {
    uVar1 = uVar1 & 0xfffd;
  }
  else {
    uVar1 = uVar1 | 2;
  }
  if ((DAT_0001f0da & 2) == 0) {
    uVar1 = uVar1 | 4;
  }
  else {
    uVar1 = uVar1 & 0xfffb;
  }
  if ((DAT_0001f0d9 & 8) == 0) {
    uVar1 = uVar1 | 8;
  }
  else {
    uVar1 = uVar1 & 0xfff7;
  }
  if ((DAT_0001f0db & 8) == 0) {
    uVar1 = uVar1 & 0xffef;
  }
  else {
    uVar1 = uVar1 | 0x10;
  }
  if ((DAT_0001fe8e & 8) == 0) {
    uVar1 = uVar1 & 0xffdf;
  }
  else {
    uVar1 = uVar1 | 0x20;
  }
  if ((DAT_0001f0da & 4) == 0) {
    uVar1 = uVar1 | 0x40;
  }
  else {
    uVar1 = uVar1 & 0xffbf;
  }
  if ((DAT_0001f0da & 2) == 0) {
    uVar1 = uVar1 | 0x80;
  }
  else {
    uVar1 = uVar1 & 0xff7f;
  }
  if ((DAT_0001f0d8 & 4) == 0) {
    DAT_0001f0fc = uVar1 & 0xfdff;
  }
  else {
    DAT_0001f0fc = uVar1 | 0x200;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void compose_status_word_f102(void)

{
  undefined2 in_R0;
  short sVar1;
  undefined2 in_R1;
  ushort extraout_R1;
  ushort uVar2;
  undefined2 in_R2;
  byte bVar3;
  
  bVar3 = 1;
  if ((DAT_0001fe8a & 0x10) == 0) {
    uVar2 = DAT_0001f102 | 1;
  }
  else {
    uVar2 = DAT_0001f102 & 0xfffe;
  }
  if ((P4DR & 4) == 0) {
    uVar2 = uVar2 | 2;
  }
  else {
    uVar2 = uVar2 & 0xfffd;
  }
  if ((DAT_0001f0d8 & 0x10) == 0) {
    uVar2 = uVar2 | 8;
  }
  else {
    uVar2 = uVar2 & 0xfff7;
  }
  if ((DAT_0001f0da & 1) == 0) {
    uVar2 = uVar2 | 4;
  }
  else {
    uVar2 = uVar2 & 0xfffb;
  }
  if ((DAT_0001f0d9 & 2) == 0) {
    uVar2 = uVar2 & 0xffef;
  }
  else {
    uVar2 = uVar2 | 0x10;
  }
  if ((DAT_0001fe8e & 8) == 0) {
    uVar2 = uVar2 & 0xffdf;
  }
  else {
    uVar2 = uVar2 | 0x20;
  }
  if ((DAT_0001f0d8 & 1) == 0) {
    uVar2 = uVar2 | 0x40;
  }
  else {
    uVar2 = uVar2 & 0xffbf;
  }
  if ((DAT_0001f0db & 1) == 0) {
    uVar2 = uVar2 & 0xff7f;
  }
  else {
    uVar2 = uVar2 | 0x80;
  }
  if ((DAT_0001f0d9 & 1) == 0) {
    uVar2 = uVar2 & 0xfeff;
  }
  else {
    uVar2 = uVar2 | 0x100;
  }
  if ((DAT_0001f0db & 0x10) == 0) {
    uVar2 = uVar2 & 0xfdff;
  }
  else {
    uVar2 = uVar2 | 0x200;
  }
  sVar1 = latch_flag_read_fede_bit1(in_R0,uVar2 & 0xfbff,in_R2,in_R1);
  uVar2 = extraout_R1;
  if (sVar1 != 0) {
    uVar2 = extraout_R1 | 0x400;
  }
  if ((*(byte *)((uint)bVar3 << 0x10 | 0xfe8e) & 1) == 0) {
    uVar2 = uVar2 | 0x800;
  }
  else {
    uVar2 = uVar2 & 0xf7ff;
  }
  if ((*(byte *)((uint)bVar3 << 0x10 | 0xf0da) & 0x10) == 0) {
    uVar2 = uVar2 & 0xefff;
  }
  else {
    uVar2 = uVar2 | 0x1000;
  }
  if ((*(byte *)((uint)bVar3 << 0x10 | 0xf0da) & 8) == 0) {
    uVar2 = uVar2 & 0xdfff;
  }
  else {
    uVar2 = uVar2 | 0x2000;
  }
  if ((*(byte *)((uint)bVar3 << 0x10 | 0xf0d8) & 2) == 0) {
    uVar2 = uVar2 | 0x4000;
  }
  else {
    uVar2 = uVar2 & 0xbfff;
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf102) = uVar2;
  return;
}



bool check_flag_fe96_bit7_clear(void)

{
  return (DAT_0001fe96 & 0x80) == 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar check_flag_f0d8_bit0_clear(void)

{
  return (DAT_0001f0d8 & 1) == 0;
}



bool check_flag_fe87_bit1_clear(void)

{
  return (P4DR & 2) == 0;
}



bool check_flag_fe8a_bit1_set(void)

{
  return (DAT_0001fe8a & 2) != 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly + bank-prefixed xrefs]
// adc_read_sequence_main: reads a batch of MUT-relevant ADC channels via shared helper @0x15b0d.
// Cross-checked each channel's word-write target against the MUT RequestID table @ROM 0x2fad0
// (via adc_sensor_convert_single @0x171c3). Note: xrefs to these targets require bank-prefixed
// addresses (e.g. 0x1f11e not 0xf11e) - plain address xref lookups return nothing.
// Confirmed mappings (word write addr / MUT byte read is the LOW byte of the word, i.e. addr+1):
//   - channel #0 -> word @0xF108/F10A region (CoolantTemp-related area; NOT independently traced
//     this pass, see logging.txt OPEN ITEM #4 four-way coolant conflict - still unresolved)
//   - channel #1 -> word @0xF116/F117 -> MUT AirTemp (RequestID 0x3A -> table entry 0xF117).
// VERIFIED.
//   - channel #2 -> word @0xF84E -> feeds coolant validity path (0x2142c), separate from MUT table
//   - channel #3 -> word @0xF11E/F11F -> MUT O2 Sensor (RequestID 0x13 -> table entry 0xF11F),
//     consumer o2_closed_loop_fuel_trim_compute (0x237a0). VERIFIED.
//   - channel #4 -> word @0xF122/F123 -> MUT O2 Sensor 2 (RequestID 0x3C -> table entry 0xF123),
//     consumer o2_sensor2_threshold_flag_update_f226_bit13 (0x22f16). VERIFIED.
//   - channel #0xB -> word @0xF15E/F15F -> MUT MAP/Boost (RequestID 0x38 -> table entry 0xF15F).
// VERIFIED.
//   - channel #8 -> word @0xF160/F161 -> MUT ReqID 0x39 (table entry 0xF161). CONFIRMED live
//     signal (2026-07-15 MUT sweep), physical sensor identity not yet determined - distinct from
//     the neighboring channel #0xB (MAP/Boost) word, not a shared/artifact cell.
//   - channel #0xA -> word @0xF162 region, NOT individually re-checked this pass
//   - channel #9 -> word @0xF166/F16A region, NOT individually re-checked this pass
// 6 of 8 channels now cross-verified against MUT table + consumer. Remaining 2 (channels 0/2/9/0xA
// region) not yet individually re-checked - do not assume correct without checking.

void adc_read_sequence_main(void)

{
  byte *pbVar1;
  byte extraout_var;
  byte extraout_var_00;
  byte extraout_var_01;
  byte extraout_var_02;
  byte extraout_var_03;
  byte extraout_var_04;
  byte extraout_var_05;
  byte extraout_var_06;
  byte extraout_var_07;
  byte extraout_var_08;
  byte extraout_var_09;
  undefined2 in_R0;
  ushort uVar2;
  short sVar3;
  undefined2 in_R1;
  undefined2 extraout_R1;
  undefined2 extraout_R1_00;
  undefined2 extraout_R1_01;
  undefined2 in_R2;
  undefined2 extraout_R2;
  undefined2 extraout_R2_00;
  undefined2 extraout_R2_01;
  byte bVar4;
  ushort in_SR;
  
  bVar4 = 1;
  uVar2 = noop_return_void(0,in_R1,in_R2,in_R0);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf10a) = uVar2 >> 6;
  *(ushort *)((uint)bVar4 << 0x10 | 0xf108) = uVar2 >> 8;
  (*adc_channel_convert_and_read)(1);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf116) = (ushort)extraout_var;
  (*adc_channel_convert_and_read)(2);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf84e) = (ushort)extraout_var_00;
  sVar3 = *(short *)((uint)bVar4 << 0x10 | 0xf84e);
  if (*(char *)((uint)bVar4 << 0x10 | 0x2c1) != '\0') {
    sVar3 = 0xcd;
  }
  *(short *)((uint)bVar4 << 0x10 | 0xf12e) = sVar3;
  uVar2 = in_SR & 0xfff0 | (ushort)(sVar3 == 0) << 2 | (ushort)(sVar3 < 0) << 3;
  pbVar1 = (byte *)((uint)bVar4 << 0x10 | 0xfe97);
  *pbVar1 = *pbVar1 | 1;
  (*adc_channel_convert_and_read)(3,extraout_R1,extraout_R2,uVar2);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf11e) = (ushort)extraout_var_01;
  (*adc_channel_convert_and_read)(4);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf122) = (ushort)extraout_var_02;
  uVar2 = uVar2 & 0xfff0 | (ushort)((byte)uVar2 & 1) | (ushort)((byte)(uVar2 >> 1) & 1) << 1 |
          (ushort)((byte)(uVar2 >> 2) & 1) << 2 | (ushort)((byte)(uVar2 >> 3) & 1) << 3;
  pbVar1 = (byte *)((uint)bVar4 << 0x10 | 0xfe97);
  *pbVar1 = *pbVar1 & 0xfe;
  (*adc_channel_convert_and_read)(3,extraout_R1_00,extraout_R2_00,uVar2);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf120) = (ushort)extraout_var_03;
  (*adc_channel_convert_and_read)(4);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf124) = (ushort)extraout_var_04;
  (*adc_channel_convert_and_read)(0xb);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf15e) = (ushort)extraout_var_05;
  pbVar1 = (byte *)((uint)bVar4 << 0x10 | 0xfe97);
  *pbVar1 = *pbVar1 | 1;
  (*adc_channel_convert_and_read)
            (8,extraout_R1_01,extraout_R2_01,uVar2 & 0xfff0 | (ushort)(extraout_var_05 == 0) << 2);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf160) = (ushort)extraout_var_06;
  (*adc_channel_convert_and_read)(10);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf162) = (ushort)extraout_var_07;
  (*adc_channel_convert_and_read)(9);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf16a) = (ushort)extraout_var_08;
  (*adc_channel_convert_and_read)(9);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf166) = (ushort)extraout_var_09;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// TRIAGE: only caller is subsystem_unconditional_init_chain. Calls noop_return_void(0xb) then
// stores (result>>6) into peripheral reg at bank 1 offset 0xf16e. Name is a best-effort guess based
// on the ADC-cluster location in the surrounding code (near adc_read_sequence_main/b) - verify
// against hardware manual before trusting fully.

void adc_channel_read_and_store_f16e(void)

{
  undefined2 in_R0;
  ushort uVar1;
  undefined2 in_R1;
  undefined2 in_R2;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = noop_return_void(0xb,in_R1,in_R2,in_R0);
  *(ushort *)((uint)bVar2 << 0x10 | 0xf16e) = uVar1 >> 6;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// adc_read_sequence_b: reads MUT-relevant ADC channels via shared helper
// adc_sensor_convert_single-style ADC poll routine at 0x15b0d.
//   - channel #5 -> word write @0xF13A/F13B  == MUT Battery (RequestID 0x14 -> table entry 0xF13B).
// VERIFIED.
//   - channel #7 -> word write @0xF13C/F13D  == MUT TPS (RequestID 0x17 -> table entry 0xF13D).
// VERIFIED.
// Cross-checked against MUT RequestID table @ROM 0x2fad0 (indexed via adc_sensor_convert_single
// @0x171c3, EP=2 bank, base -0x530 from EP:2 segment).
// Confirms logging.txt CONFIRMED-section TPS/Battery claims are correct under current decoder.
// 
// 2026-08-15: f13a (this function's channel-5 battery voltage) confirmed as a real charging-status
// input inside tcu_rx_main_scheduler @0x2aa36/0x2afd4: hysteresis comparator against ROM thresholds
// @0xd4e/0xd50 sets/clears f3f6 bit7. VR4 has the SAME comparator at the SAME ROM threshold
// addresses 0xd4e/0xd50 (its own battery-voltage cell is f078, gating f2d2 bit7 there) --
// byte-identical threshold addresses across both ROMs is strong cross-confirmation this is genuine
// alternator/battery-voltage-OK status logic, not coincidence. See project chat 2026-08-15.

void adc_read_sequence_b(void)

{
  byte extraout_var;
  byte extraout_var_00;
  undefined2 in_R0;
  undefined2 in_R1;
  undefined2 in_R2;
  byte bVar1;
  
  bVar1 = 1;
  noop_return_void(5,in_R1,in_R2,in_R0);
  *(ushort *)((uint)bVar1 << 0x10 | 0xf13a) = (ushort)extraout_var;
  (*adc_channel_convert_and_read)(7);
  *(ushort *)((uint)bVar1 << 0x10 | 0xf13c) = (ushort)extraout_var_00;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void adc_read_channel_6(void)

{
  byte extraout_var;
  undefined2 in_R0;
  undefined2 in_R1;
  undefined2 in_R2;
  byte bVar1;
  
  bVar1 = 1;
  noop_return_void(6,in_R1,in_R2,in_R0);
  *(ushort *)((uint)bVar1 << 0x10 | 0xf15c) = (ushort)extraout_var;
  return;
}



void zero_var_f15a(void)

{
  DAT_0001f15a = 0;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void identity_passthrough_u16_b(void)

{
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [FOUND 2026-07-22] Genuine RAM-flag-to-physical-output-pin mirror function.
// Mirrors 8 individual bits of RAM 0xf0e6 onto real H8 port data register
// pins, several gated by hardware-config bytes at ROM 0x102e4-0x102ee
// (likely per-model-variant trim/wiring options sharing this base ROM):
// 
//   f0e6 bit0 (0x01) -> P1DR bit4 (0xfe82), direct
//   f0e6 bit1 (0x02) -> P2DR bit7 (0xfe83), gated by ROM byte 0x102ee
//   f0e6 bit2 (0x04) -> P4DR bit7 (0xfe87), gated by ROM byte 0x102ed,
//                        polarity flips depending on that byte's bit3
//   f0e6 bit3 (0x08) -> PCDR bit3 (0xfe97), direct, INVERTED logic
//                        (bit clear -> pin SET)
//   f0e6 bit4 (0x10) -> P4DR bit4 (0xfe87), gated by ROM byte 0x102e4==1
//   f0e6 bit5 (0x20) -> PCDR bit6 (0xfe97), same gate as bit4
//   f0e6 bit6 (0x40) -> PCDR bit7 (0xfe97), direct
//   f0e6 bit7 (0x80) -> P1DR bit5 (0xfe82), gated by ROM byte 0x102e6==0
//   f0e6 bit13(0x2000)-> P6DR bit2 (0xfe8b), INVERTED, gated by f1f6 bit3
// 
// This is the most concrete port-level actuator/relay-driver mechanism found
// on this ROM to date -- unlike the sci1_meta_cmd_dispatch_c0_ff (0x28869)
// bit-flag chains (f510/f512/f516), which mostly terminate in further RAM
// state machines with no confirmed physical meaning, these are REAL discrete
// output pins.
// 
// Writer for bit3 traced (2026-07-22): knock_octane_f0e6_bit3_gate_dispatch
// (0x284a3) sets/clears f0e6 bit3 based on RPM threshold + the EED6/EED8/EEDA
// warmup-cluster state + a call into knock_octane_pattern_match_state_machine
// (0x28557), a bit-pattern state machine over the per-cylinder EED0-EEE8
// array family (same cluster gating f516_request_latch_f09a_calib_select's
// warmup-target selection) that looks like misfire/knock-pattern detection
// driving internal timers f51a/f51c/f51e/f04a. Circumstantial but reasonably
// strong case that PCDR bit3 (f0e6 bit3) is either a check-engine/MIL lamp
// driver or a knock-condition dash warning output -- NOT independently
// confirmed against real hardware, treat as a well-evidenced hypothesis only.
// 
// Other 8 bits (0/1/2/4/5/6/7/13): writer(s) not yet traced beyond one-shot
// init at engine_state_f498_init (0x17302, sets bit1 unconditionally at
// startup only - not diagnostically interesting). ~20 more xref sites to
// f0e6 exist across the ROM (0x17341/173d5/173db/174c9/174cf/1861d/18623/
// 187ba/187c2/18bb1/18bbb/18dbf/18dc5/18ddd/19359/1935f/19377/197ce/197d8/
// 19806/19891/19904/1998f/19e4c/27c94) -- NOT yet individually traced, good
// next-session target list for identifying the remaining 8 output pins.
// See mut_verification_status.md for the actuator-ID investigation this
// session grew out of.

void mirror_status_f0e6_to_ports(void)

{
  if (DAT_000102e6 == '\0') {
    if ((output_relay_flags_f0e6 & 0x80) == 0) {
      P1DR = P1DR & 0xdf;
    }
    else {
      P1DR = P1DR | 0x20;
    }
  }
  if ((output_relay_flags_f0e6 & 1) == 0) {
    P1DR = P1DR & 0xef;
  }
  else {
    P1DR = P1DR | 0x10;
  }
  if ((output_relay_flags_f0e6 & 8) == 0) {
    DAT_0001fe97 = DAT_0001fe97 | 8;
  }
  else {
    DAT_0001fe97 = DAT_0001fe97 & 0xf7;
  }
  if (g_egr_enable_config_flag != '\0') {
    if ((output_relay_flags_f0e6 & 2) == 0) {
      DAT_0001fe83 = DAT_0001fe83 & 0x7f;
    }
    else {
      DAT_0001fe83 = DAT_0001fe83 | 0x80;
    }
  }
  if (DAT_000102ed != 0) {
    if ((DAT_000102ed & 8) == 0) {
      if ((output_relay_flags_f0e6 & 4) == 0) {
        P4DR = P4DR | 0x80;
      }
      else {
        P4DR = P4DR & 0x7f;
      }
    }
    else if ((output_relay_flags_f0e6 & 4) == 0) {
      P4DR = P4DR & 0x7f;
    }
    else {
      P4DR = P4DR | 0x80;
    }
  }
  if (DAT_000102e4 == '\x01') {
    if ((output_relay_flags_f0e6 & 0x10) == 0) {
      P4DR = P4DR & 0xef;
    }
    else {
      P4DR = P4DR | 0x10;
    }
    if ((output_relay_flags_f0e6 & 0x20) == 0) {
      DAT_0001fe97 = DAT_0001fe97 & 0xbf;
    }
    else {
      DAT_0001fe97 = DAT_0001fe97 | 0x40;
    }
  }
  if ((output_relay_flags_f0e6 & 0x40) == 0) {
    DAT_0001fe97 = DAT_0001fe97 & 0x7f;
  }
  else {
    DAT_0001fe97 = DAT_0001fe97 | 0x80;
  }
  if ((DAT_0001f1f6 & 8) != 0) {
    if ((output_relay_flags_f0e6 & 0x2000) == 0) {
      DAT_0001fe8b = DAT_0001fe8b | 4;
    }
    else {
      DAT_0001fe8b = DAT_0001fe8b & 0xfb;
    }
  }
  return;
}



ushort mirror_status_f0ea_to_ports(ushort param_1)

{
  if ((DAT_0001f0ea & 1) == 0) {
    DAT_0001fe8e = DAT_0001fe8e & 0xbf;
  }
  else {
    DAT_0001fe8e = DAT_0001fe8e | 0x40;
  }
  if (DAT_000102c8 == '\0') {
    if ((DAT_0001f0ea & 2) == 0) {
      P4DR = P4DR | 0x40;
    }
    else {
      P4DR = P4DR & 0xbf;
    }
  }
  if ((g_status_flags_f1f2 & 0x2000) != 0) {
    if ((DAT_0001f0ea & 4) == 0) {
      P1DR = P1DR & 0xbf;
    }
    else {
      P1DR = P1DR | 0x40;
    }
    if ((DAT_0001f0ea & 8) == 0) {
      P1DR = P1DR & 0x7f;
    }
    else {
      P1DR = P1DR | 0x80;
    }
  }
  if (DAT_000102c2 != '\0') {
    if ((DAT_0001f0ea & 0x10) == 0) {
      P4DR = P4DR | 0x40;
    }
    else {
      P4DR = P4DR & 0xbf;
    }
    if ((DAT_0001f0ea & 0x20) == 0) {
      P4DR = P4DR | 0x20;
    }
    else {
      P4DR = P4DR & 0xdf;
    }
  }
  if ((DAT_0001f0ea & 0x80) == 0) {
    DAT_0001fe93 = DAT_0001fe93 & 0xfe;
  }
  else {
    DAT_0001fe93 = DAT_0001fe93 | 1;
  }
  if (DAT_000102e3 != '\0') {
    if ((DAT_0001f0ea & 0x40) == 0) {
      DAT_0001fe83 = DAT_0001fe83 & 0xbf;
    }
    else {
      DAT_0001fe83 = DAT_0001fe83 | 0x40;
    }
  }
  if (DAT_000102f3 != '\0') {
    if ((DAT_0001f0ea & 0x200) == 0) {
      DAT_0001fe83 = DAT_0001fe83 & 0xdf;
    }
    else {
      DAT_0001fe83 = DAT_0001fe83 | 0x20;
    }
    if ((DAT_0001f0ea & 0x100) == 0) {
      DAT_0001fe83 = DAT_0001fe83 & 0xef;
    }
    else {
      DAT_0001fe83 = DAT_0001fe83 | 0x10;
    }
  }
  if ((DAT_0001f1f4 & 8) != 0) {
    if ((DAT_0001f0ea & 0x400) == 0) {
      DAT_0001fe8a = DAT_0001fe8a & 0xfe;
    }
    else {
      DAT_0001fe8a = DAT_0001fe8a | 1;
    }
  }
  return param_1;
}



void update_flag_fe97_bit2_cond(void)

{
  if ((g_status_flags_f1f2 & 0x80) != 0) {
    if ((DAT_0001f0ec & 2) == 0) {
      DAT_0001fe97 = DAT_0001fe97 & 0xfb;
    }
    else {
      DAT_0001fe97 = DAT_0001fe97 | 4;
    }
  }
  return;
}



void update_flag_fe82_bit2_cond(void)

{
  if (DAT_000102eb != '\0') {
    if ((DAT_0001f0ec & 1) == 0) {
      P1DR = P1DR | 4;
    }
    else {
      P1DR = P1DR & 0xfb;
    }
  }
  return;
}



void toggle_flag_fe8b_bit4(void)

{
  DAT_0001fe8b = DAT_0001fe8b ^ 0x10;
  return;
}



void clear_flag_fe93_bit4(void)

{
  DAT_0001fe93 = DAT_0001fe93 & 0xef;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void update_port_fe82_indexed(void)

{
  P1DR = *(byte *)((int)(DAT_0001eed2 & 3) + 0x58a3) | P1DR & 0xfc;
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: ram
// WARNING: Restarted to delay deadcode elimination for space: register
// [FOUND 2026-07-26, answering "do any other timers do the same thing as
// Timer 6" question] NOT a hardware timer -- a SOFTWARE bit-banged PWM
// scheduler, running on a 48-step cycle (DAT_0001f0f0 counter, wraps at 47),
// almost certainly called from a fixed-period timer ISR (not yet identified
// which one drives the periodic call to this function). For each of 7 duty
// targets, compares the running 0-47 counter against a scaled threshold
// (value*3/8, i.e. roughly value/255 * 48-step duty ratio) and sets/clears a
// single port-register bit accordingly -- classic software PWM. Confirmed
// outputs (all real H8 port data registers per list_globals, not RAM flags):
// 
//   F448 (EGR duty, see egr_target_f448_update/egr_duty_target_calc @0x18650/
//     0x186fa) -> P1DR bit3 (0xfe82.3). Gated on ROM config byte 0x102de != 0
//     -- CONFIRMED ACTIVE on this ROM (0x102de reads 1). This is a SECOND,
//     INDEPENDENT EGR duty output, distinct from the Timer-6-hardware-PWM one
//     (F494 -> T6GR1, config 0x102e4==2, also confirmed active on this ROM --
//     see egr_position_target_f494_calc plate comment). Both mechanisms are
//     live simultaneously on this exact ROM; whether they drive the same
//     physical EGR valve redundantly, two different EGR-system components, or
//     one is dead/unused despite its config gate being satisfied is NOT
//     determined without real hardware/wiring info.
//   F452/F454/F456/F458 (all four written together by
//     o2_sensor_control_dispatch @ 0x197ce, the O2 sensor upstream/downstream
//     control-mode function) -> P2DR bits 0/1/2/3 respectively (0xfe83.0-3),
//     each independently gated on combinations of f1f2 bits 14/1/2. This
//     resolves what o2_sensor_control_dispatch's four output words physically
//     are: FOUR independent duty-cycled outputs on P2DR, almost certainly O2
//     sensor HEATER duty-cycle control (upstream/downstream heater elements,
//     possibly split further by bank or a fault-derate mode) -- consistent
//     with real O2 heater circuits typically being PWM-driven rather than
//     simple on/off.
//   F44C (wgdc_output_clamp_f44c @ 0x19110, ALREADY CONFIRMED as WGDC --
//     turbo Wastegate Duty Cycle, MUT RequestID 0x86 -> table entry 0xF44D
//     per an earlier, independently-verified session) -> PADR bit1 (0xfe93.1).
//     BEST FINDING of this pass: this connects an already MUT-confirmed real
//     actuator value to a traced physical output pin for the first time in
//     this whole investigation -- WGDC (turbo wastegate solenoid) = PADR.1,
//     software-PWM driven.
//   F45C (written by isc_target_calc_and_store @ 0x175f2, an Idle Speed
//     Control duty-solenoid path -- DISTINCT from the separate ISC STEPPER
//     MOTOR mechanism found elsewhere, isc_stepper_mode_state_reload_d/
//     isc_f402_stepper_target_calc/f406,f408 -- this ROM appears to carry
//     code for BOTH an ISC duty solenoid AND an ISC stepper motor, likely
//     alternate hardware configurations or two coexisting idle-control
//     actuators) -> PADR bit2 (0xfe93.2), gated on f1f2 bit12.
// 
// ANSWER to "do any other timers do the same thing as Timer 6": NO other H8
// hardware timer (T1-T5, T7 checked via get_xrefs_to on their GR1 compare
// registers) shows any literal-address writer anywhere in this ROM -- T2's
// GR1/CNT registers are used only as an INPUT CAPTURE source (cam/RPM
// sensing via isr_ipu_ch2ch4_input_capture, reading not writing), and T1/T3/
// T4/T5/T7 have no confirmed writer at all (either genuinely unused on this
// ROM, or a literal-addressing blind spot -- not exhaustively ruled out).
// Timer 6 remains the ONLY hardware-timer-driven PWM actuator output found.
// However, THIS software scheduler is arguably more important: it's a
// SEPARATE, parallel PWM mechanism (not a hardware timer at all) driving
// SEVEN real actuator duty outputs onto genuine port pins, including the
// first-ever physical-pin trace for an already-MUT-confirmed value (WGDC).
// See mut_verification_status.md for the full writeup and next steps.

void __stdcall_far_allstack software_pwm_output_scheduler(void)

{
  uint uVar1;
  ushort uVar2;
  ushort in_R1;
  short sVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  
  DAT_0001f0f0 = DAT_0001f0f0 + 1;
  if (0x2f < DAT_0001f0f0) {
    DAT_0001f0f0 = 0;
  }
  if (DAT_000102de != '\0') {
    sVar3 = DAT_0001f448 * 3;
    in_R1 = (in_R1 >> 3) + (ushort)((byte)(in_R1 >> 2) & 1);
    if (DAT_0001f0f0 < in_R1) {
      P1DR = P1DR | 8;
    }
    else {
      P1DR = P1DR & 0xf7;
    }
  }
  if ((g_status_flags_f1f2 & 0x1000) != 0) {
    sVar3 = DAT_0001f45c * 3;
    in_R1 = (in_R1 >> 3) + (ushort)((byte)(in_R1 >> 2) & 1);
    if (DAT_0001f0f0 < in_R1) {
      DAT_0001fe93 = DAT_0001fe93 | 4;
    }
    else {
      DAT_0001fe93 = DAT_0001fe93 & 0xfb;
    }
  }
  if ((g_status_flags_f1f2 & 0x4000) != 0) {
    sVar3 = DAT_0001f452 * 3;
    in_R1 = (in_R1 >> 3) + (ushort)((byte)(in_R1 >> 2) & 1);
    if (DAT_0001f0f0 < in_R1) {
      DAT_0001fe83 = DAT_0001fe83 | 1;
    }
    else {
      DAT_0001fe83 = DAT_0001fe83 & 0xfe;
    }
    if ((g_status_flags_f1f2 & 4) != 0) {
      sVar3 = DAT_0001f456 * 3;
      in_R1 = (in_R1 >> 3) + (ushort)((byte)(in_R1 >> 2) & 1);
      if (DAT_0001f0f0 < in_R1) {
        DAT_0001fe83 = DAT_0001fe83 | 4;
      }
      else {
        DAT_0001fe83 = DAT_0001fe83 & 0xfb;
      }
    }
    if ((g_status_flags_f1f2 & 2) != 0) {
      sVar3 = DAT_0001f454 * 3;
      in_R1 = (in_R1 >> 3) + (ushort)((byte)(in_R1 >> 2) & 1);
      if (DAT_0001f0f0 < in_R1) {
        DAT_0001fe83 = DAT_0001fe83 | 2;
      }
      else {
        DAT_0001fe83 = DAT_0001fe83 & 0xfd;
      }
      if ((g_status_flags_f1f2 & 4) != 0) {
        sVar3 = DAT_0001f458 * 3;
        in_R1 = (in_R1 >> 3) + (ushort)((byte)(in_R1 >> 2) & 1);
        if (DAT_0001f0f0 < in_R1) {
          DAT_0001fe83 = DAT_0001fe83 | 8;
        }
        else {
          DAT_0001fe83 = DAT_0001fe83 & 0xf7;
        }
      }
    }
  }
  uVar5 = (undefined2)((uint)DAT_0001f44c * 0x30 >> 0x10);
  uVar4 = (undefined2)((uint)DAT_0001f44c * 0x30);
  uVar1 = CONCAT22(uVar5,uVar4);
  uVar2 = (ushort)(uVar1 % 200);
  uVar5 = (short)(uVar1 / 200);
  if (99 < uVar2) {
    in_R1 = in_R1 + 1;
  }
  if (DAT_0001f0f0 < in_R1) {
    DAT_0001fe93 = DAT_0001fe93 | 2;
  }
  else {
    DAT_0001fe93 = DAT_0001fe93 & 0xfd;
  }
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: register
// WARNING: Unknown calling convention -- yet parameter storage is locked
// [EXISTING FUNCTION, COMMENT ADDED 2026-08-08] serial_fef9_duty_scale_and_set:
// runs only when EGR-hardware-variant config byte 0x102e4==2 (CONFIRMED
// active on this ROM). Reads radfan_pwm_duty_target_f494 (renamed from
// "EGR position target F494" -- see radfan_pwm_duty_target_f494_calc's
// plate comment for why), feeds it to ff88_set_scaled_clamped (0x16c33)
// which scales*16, clamps [1,2048], and writes T6GR1H (0xff88, real Timer
// 6 PWM duty register). Externally forceable via SCI1 actuator cmd 0xCD/
// 0xCE. See mut_verification_status.md "DISCREPANCY CHECKED..." (2026-08-08)
// for the radiator-fan-vs-EGR physical identity correction.

void serial_fef9_duty_scale_and_set(void)

{
  uint uVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (DAT_000102e4 == '\x02') {
    (*ff88_set_scaled_clamped)(radfan_pwm_duty_target_f494);
  }
  uVar1 = (uint)*(ushort *)((uint)bVar3 << 0x10 | 0xf450) * 0xfa;
  sVar2 = (short)(uVar1 / 0xff);
  if (0xfe < (ushort)((short)(uVar1 % 0xff) * 2)) {
    sVar2 = sVar2 + 1;
  }
  serial_fef9_set_clamped(sVar2);
  return;
}



// WARNING: Removing unreachable block (ram,0x00015a51)
// WARNING: Restarted to delay deadcode elimination for space: register
// Param binding confirmed correct (scale_input = Stack[0x4], matches disasm @(0x8,SP) after 2
// pushes). Return value is a genuine pass-through of R0's entry value (not explicitly set on any
// path) -- same undefined2 param_1 pass-through pattern as sibling init_ functions. Not yet
// resolved whether callers rely on this return; treat as low-priority follow-up if a caller's use
// of the return value is ever traced.

undefined2 __stdcall_far_allstack init_scale_factor_f9ac(ushort scale_input)

{
  undefined2 in_R0;
  undefined2 in_R1;
  undefined2 uVar1;
  undefined2 uVar2;
  
  if (DAT_000102e2 != '\0') {
    uVar2 = (undefined2)((uint)scale_input * 25000 >> 0x10);
    uVar1 = (undefined2)((uint)scale_input * 25000);
    uVar2 = (short)(CONCAT22(uVar2,uVar1) / 0x100);
    DAT_0001f9aa = 25000;
    DAT_0001f9ac = in_R1;
  }
  return in_R0;
}



// TRIAGE: no callers or xrefs found anywhere in the program - genuinely dead code, OR reached only
// via an indirect/computed call Ghidra hasn't resolved (e.g. a function-pointer table). Body just
// returns param_1 unchanged, ignoring param_2 - same pattern as the existing
// identity_passthrough_arg1() elsewhere in this ROM. Flagged 'unused' rather than deleted/ignored
// since embedded ECU code sometimes keeps such stubs as jump-table placeholders.

undefined2 identity_passthrough_arg1_unused(undefined2 param_1)

{
  return param_1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void peripheral_regs_ff2x_ff9x_bulk_zero_init(void)

{
  DAT_0001ff15 = DAT_0001ff15 & 0x5f;
  _DAT_0001fe80 = 0;
  _DAT_0001fe84 = 0;
  _DAT_0001fe88 = 0;
  DAT_0001fe8c = 0;
  DAT_0001fe91 = 0;
  _DAT_0001fe94 = 0;
  DAT_0001ff21 = 0;
  DAT_0001ff31 = 0;
  DAT_0001ff22 = 0;
  DAT_0001ff32 = 0;
  T1OERA = 0;
  T1OERB = 0;
  DAT_0001ff41 = 0;
  DAT_0001ff42 = 0;
  sci3_timeout_state_ff44 = 0;
  DAT_0001ff51 = 0;
  DAT_0001ff52 = 0;
  DAT_0001ff54 = 0;
  DAT_0001ff61 = 0;
  DAT_0001ff62 = 0;
  DAT_0001ff64 = 0;
  DAT_0001ff71 = 0;
  DAT_0001ff72 = 0;
  pwm_channel_track_status_ff74 = 0;
  DAT_0001ff81 = 0;
  DAT_0001ff82 = 0;
  DAT_0001ff84 = 0;
  DAT_0001ff91 = 0;
  DAT_0001ff92 = 0;
  DAT_0001ff94 = 0;
  _DAT_0001feda = 0;
  DAT_0001feb8 = 0;
  DAT_0001feb9 = 0x40;
  return;
}



void init_reg_feb8_feb9(void)

{
  DAT_0001feb8 = 0;
  DAT_0001feb9 = 0x40;
  return;
}



void noop_return_void(void)

{
  return;
}



// WARNING: Removing unreachable block (ram,0x00015b33)
// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// Shared single-channel ADC conversion helper. Called with channel number in R0.
// Selects channel via @0xFEB8 (clamped 0-11), starts conversion (bit5 @0xFEB9),
// polls completion (bit7 @0xFEB8, spins until set), reads converted byte from
// a table at (R0*2 - 0x160) and clears the busy bit. Called by adc_read_sequence_main
// (0x1556d) and adc_read_sequence_b (0x15689) for all MUT-relevant analog channel reads.
// 
// RENAMED 2026-07-23 from misleading "trap_hang" -- the old name and the
// decompiler's own "do nothing block with infinite loop"/"removing unreachable
// block" warnings are WRONG: the poll loop at 0x15b2d (`btst.b 0x7,@0xfeb8;
// beq 0x15b2d`) is a real, terminating hardware-completion poll, not an
// infinite hang -- the decompiler just can't model @0xfeb8 changing
// asynchronously from outside its visible dataflow, so it wrongly concludes
// the branch condition can never become true.
// 
// KNOWN LIMITATION: parameter `channel` is modeled as Stack[0x5] but is
// ACTUALLY passed in R0 (confirmed via disassembly -- no `link FP`/stack
// frame exists in this leaf function at all). set_variable_storage's
// programmatic API cannot retarget parameter storage to a register on this
// architecture/version; would need manual Decompiler-window retyping or a
// custom Jython/Java script (see FixEBPRegisterReuse.java pattern) to fix
// properly. Treat the current Stack[0x5] typing as a display placeholder,
// not a claim about the real ABI.

ushort __stdcall_far_allstack adc_channel_convert_and_read(uchar channel)

{
                    // WARNING: Do nothing block with infinite loop
  do {
  } while( true );
}



ushort init_state_block_f97a(ushort param_1)

{
  DAT_0001f992 = 0;
  DAT_0001f97a = 0;
  DAT_0001f97c = 0;
  DAT_0001f996 = 0xa000;
  DAT_0001f998 = SUB42(&T3GR1H,0);
  DAT_0001f99a = SUB42(&DAT_0000f97a,0);
  DAT_0001f99c = 0xc;
  DAT_0001f994 = 0xc;
  DAT_0001f99e = 0xc;
  DAT_0001ff52 = DAT_0001ff52 | 1;
  DAT_0001ff51 = DAT_0001ff51 & 0xfe | 2;
  DAT_0001f1ba = 0;
  DAT_0001f1dc = 0;
  DAT_0001f1d4 = 0;
  return param_1;
}



void init_flags_ff51_52(void)

{
  DAT_0001ff52 = DAT_0001ff52 | 1;
  DAT_0001ff51 = DAT_0001ff51 & 0xfe | 2;
  return;
}



void sci1_timing_period_tracker(ushort param_1)

{
  ushort uVar1;
  ushort uVar2;
  short sVar3;
  byte bVar4;
  
  bVar4 = 1;
  uVar2 = isr_sci1_eri_reset_timeout(param_1);
  uVar1 = *(ushort *)((uint)bVar4 << 0x10 | 0xf97c);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf1d8) = uVar1 - *(short *)((uint)bVar4 << 0x10 | 0xf97a);
  if ((((short)(uVar1 - uVar2) < 0) || (uVar1 == uVar2)) || (0x4b < uVar1 - uVar2)) {
    sVar3 = *(short *)((uint)bVar4 << 0x10 | 0xf992);
    *(short *)((uint)bVar4 << 0x10 | 0xf1d2) = sVar3 - *(short *)((uint)bVar4 << 0x10 | 0xf1d4);
    *(short *)((uint)bVar4 << 0x10 | 0xf1d4) = sVar3;
    *(ushort *)((uint)bVar4 << 0x10 | 0xf1da) = uVar2 - *(short *)((uint)bVar4 << 0x10 | 0xf97c);
  }
  else {
    sVar3 = *(short *)((uint)bVar4 << 0x10 | 0xf992) + -1;
    *(short *)((uint)bVar4 << 0x10 | 0xf1d2) = sVar3 - *(short *)((uint)bVar4 << 0x10 | 0xf1d4);
    *(short *)((uint)bVar4 << 0x10 | 0xf1d4) = sVar3;
    *(ushort *)((uint)bVar4 << 0x10 | 0xf1da) = uVar2 - *(short *)((uint)bVar4 << 0x10 | 0xf97a);
  }
  if (0x31 < *(ushort *)((uint)bVar4 << 0x10 | 0xef64)) {
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xf1d8) = 0xffff;
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xf1da) = 0xffff;
  }
  return;
}



ushort isr_sci1_eri_reset_timeout(ushort param_1)

{
  short sVar1;
  
  if (DAT_0001f99c <= DAT_0001f994) {
    DAT_0001f992 = (DAT_0001f994 - DAT_0001f99c) + DAT_0001f992;
  }
  if (DAT_0001f99c < 0xb) {
    sVar1 = (10 - DAT_0001f99c) * 2;
    DAT_0001f97a = *(undefined2 *)((int)sVar1 + -0x686);
    DAT_0001f97c = *(undefined2 *)((int)sVar1 + -0x684);
    DAT_0001f99a = SUB42(&DAT_0000f97e,0);
    DAT_0001f99c = 10;
  }
  DAT_0001f996 = 0xa000;
  DAT_0001f998 = SUB42(&T3GR1H,0);
  DAT_0001f994 = DAT_0001f99c;
  DAT_0001f99e = DAT_0001f99c;
  return param_1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void detect_sci1_param_change_reset(void)

{
  if ((DAT_0001f992 == DAT_0001f1d6) && (DAT_0001f99c == DAT_0001f99e)) {
    return;
  }
  DAT_0001f99e = DAT_0001f99c;
  DAT_0001f1d6 = DAT_0001f992;
  DAT_0001ef96 = DAT_00010f56;
  DAT_0001ef64 = 0;
  return;
}



// WARNING: Unable to track spacebase fully for stack

void isr_sci1_eri(ushort param_1)

{
  isr_sci1_eri_reset_timeout(param_1);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void timer_block_cold_init(void)

{
  DAT_0001ff20 = 6;
  DAT_0001ff40 = 6;
  DAT_0001ff50 = 6;
  DAT_0001ff60 = 2;
  DAT_0001ff30 = 0x7f;
  DAT_0001ff41 = DAT_0001ff41 & 0xcf;
  DAT_0001ff51 = DAT_0001ff51 & 0xcf;
  DAT_0001ff61 = DAT_0001ff61 & 0xcf;
  DAT_0001ff42 = DAT_0001ff42 & 0xef;
  DAT_0001ff26 = 0;
  DAT_0001ff25 = 0;
  DAT_0001ff64 = DAT_0001ff64 & 0xcf;
  DAT_0001ff6c = DAT_0001ff66 + 5000;
  DAT_0001ff63 = DAT_0001ff63 & 0xfb;
  DAT_0001ff62 = DAT_0001ff62 & 0xef | 4;
  DAT_0001ff54 = DAT_0001ff54 & 0xcf;
  DAT_0001ff52 = DAT_0001ff52 & 0xeb;
  return;
}



void timer_block_cold_init_b(void)

{
  DAT_0001ff20 = 6;
  DAT_0001ff40 = 6;
  DAT_0001ff50 = 6;
  DAT_0001ff60 = 2;
  DAT_0001ff30 = 0x7f;
  DAT_0001ff41 = DAT_0001ff41 & 0xcf;
  DAT_0001ff51 = DAT_0001ff51 & 0xcf;
  DAT_0001ff61 = DAT_0001ff61 & 0xcf;
  DAT_0001ff42 = DAT_0001ff42 & 0xef;
  DAT_0001ff25 = 0;
  DAT_0001ff64 = DAT_0001ff64 & 0xcf;
  DAT_0001ff62 = DAT_0001ff62 & 0xef | 4;
  DAT_0001ff54 = DAT_0001ff54 & 0xcf;
  DAT_0001ff52 = DAT_0001ff52 & 0xeb;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [FOUND 2026-08-07] Despite its name/vector (SCI2 error interrupt), this is
// actually functioning as a PERIODIC TIMER TICK, not a real SCI2 error
// handler. Body: reads T4DR1H (Timer 4 compare register) and T4CNTH (Timer 4
// free-running counter), reschedules T4DR1H to (current value + 200) with
// overflow/wrap handling, clears T4SRL bit2, then unconditionally calls
// tcu_rx_main_scheduler(). This is the master periodic tick that drives the
// whole scheduler chain -- confirmed as the entry point for tcu_rx_main_scheduler,
// which in turn calls sci1_protocol_state_machine every tick, which in turn
// unconditionally calls sci1_tx_response_feeder (see its plate comment) --
// i.e. this ISR is the ultimate root of the ECU's autonomous serial TX/RX
// polling loop. Likely repurposes the SCI2-ERI vector as a convenient periodic
// interrupt slot rather than for genuine SCI2 error signaling -- worth a
// rename (e.g. isr_timer4_periodic_tick_via_sci2eri_vector) but not yet
// renamed pending cross-check of whether SCI2 error conditions are ever
// actually handled here (they are not, in the current disassembly). See
// mut_verification_status.md serial/TCU section.

void isr_sci2_eri(void)

{
  byte bVar1;
  short sVar2;
  undefined2 uVar3;
  short sVar4;
  
  sVar4 = T4DR1H;
  sVar4 = sVar4 + 0x12c0;
  sVar2 = T4CNTH;
  uVar3 = T4CNTH;
  uVar3 = T4CNTH;
  if (sVar4 - sVar2 < 0) {
    sVar4 = T4CNTH;
  }
  T4DR1H = sVar4 + 200;
  bVar1 = T4SRL;
  T4SRL = bVar1 & 0xfb;
  tcu_rx_main_scheduler();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void timer_ff41_ff44_ff4a_cold_init(void)

{
  DAT_0001ff41 = DAT_0001ff41 & 0xf3;
  sci3_timeout_state_ff44 = 0xa8;
  DAT_0001ff4a = DAT_0001ff46 + 2;
  DAT_0001ff4c = DAT_0001ff4a;
  DAT_0001ff4e = DAT_0001ff4a;
  DAT_0001ff42 = DAT_0001ff42 & 0xf7 | 6;
  return;
}



void timer_ff41_ff42_warm_init(void)

{
  DAT_0001ff41 = DAT_0001ff41 & 0xf3;
  DAT_0001ff42 = DAT_0001ff42 & 0xf7 | 6;
  return;
}



// Param binding confirmed correct via get_function_variables (channel_mask=Stack[0x4],
// timeout_value=Stack[0x6], matches disasm @(0xa,SP)/@(0xc,SP) after 3 pushes). Decompiler
// pretty-print still shows raw stack0xfffa offsets -- same cosmetic display quirk documented for
// sibling init_/timer_ functions, not a binding failure. Return value is an R0 pass-through not
// explicitly set on any path, same pattern as init_scale_factor_f9ac.

undefined2 __stdcall_far_allstack sci3_timeout_set_abort(ushort channel_mask,short timeout_value)

{
  undefined2 in_R0;
  
  if (timeout_value - (DAT_0001ff46 + 5) < 0) {
    timeout_value = DAT_0001ff46 + 5;
  }
  if ((channel_mask & 1) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xfd;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xf7 | 4;
    DAT_0001ff4a = timeout_value;
  }
  if ((channel_mask & 2) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xfb;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xdf | 0x10;
    DAT_0001ff4c = timeout_value;
  }
  if ((channel_mask & 4) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xf7;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0x7f | 0x40;
    DAT_0001ff4e = timeout_value;
  }
  return in_R0;
}



// Param binding confirmed correct via get_function_variables (channel_mask=Stack[0x4],
// timeout_value=Stack[0x6]), same pattern as sibling sci3_timeout_set_abort. Interrupt-mask-save
// prologue (not a far call) -- uses __stdcall_far_allstack, not __stdcall_far.

undefined2 __stdcall_far_allstack sci3_timeout_set_retry(ushort channel_mask,short timeout_value)

{
  undefined2 in_R0;
  
  if (timeout_value - (DAT_0001ff46 + 5) < 0) {
    timeout_value = DAT_0001ff46 + 5;
  }
  if ((channel_mask & 1) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xfd;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xfb | 8;
    DAT_0001ff4a = timeout_value;
  }
  if ((channel_mask & 2) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xfb;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xef | 0x20;
    DAT_0001ff4c = timeout_value;
  }
  if ((channel_mask & 4) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xf7;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xbf | 0x80;
    DAT_0001ff4e = timeout_value;
  }
  return in_R0;
}



undefined2 __stdcall_far_allstack sci3_timeout_clear_flag(ushort channel_mask)

{
  undefined2 in_R0;
  short sVar1;
  
  sVar1 = DAT_0001ff46 + -1;
  if ((channel_mask & 1) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xfd;
    DAT_0001ff4a = sVar1;
  }
  if ((channel_mask & 2) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xfb;
    DAT_0001ff4c = sVar1;
  }
  if ((channel_mask & 4) != 0) {
    DAT_0001ff43 = DAT_0001ff43 & 0xf7;
    DAT_0001ff4e = sVar1;
  }
  return in_R0;
}



undefined2 __stdcall_far_allstack timer_ff44_ff4a_arm_channels_atomic(ushort channel_mask)

{
  undefined2 in_R0;
  
  if ((channel_mask & 1) != 0) {
    DAT_0001ff4a = DAT_0001ff46 + 2;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xf7 | 4;
  }
  if ((channel_mask & 2) != 0) {
    DAT_0001ff4c = DAT_0001ff46 + 2;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xdf | 0x10;
  }
  if ((channel_mask & 4) != 0) {
    DAT_0001ff4e = DAT_0001ff46 + 2;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0x7f | 0x40;
  }
  return in_R0;
}



undefined2 __stdcall_far_allstack timer4_oc_arm_from_flags(ushort channel_mask)

{
  undefined2 in_R0;
  
  if ((channel_mask & 1) != 0) {
    DAT_0001ff4a = DAT_0001ff46 + 2;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xfb | 8;
  }
  if ((channel_mask & 2) != 0) {
    DAT_0001ff4c = DAT_0001ff46 + 2;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xef | 0x20;
  }
  if ((channel_mask & 4) != 0) {
    DAT_0001ff4e = DAT_0001ff46 + 2;
    sci3_timeout_state_ff44 = sci3_timeout_state_ff44 & 0xbf | 0x80;
  }
  return in_R0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isr_sci3_rxi(void)

{
  byte bVar1;
  
  bVar1 = T2SRL;
  T2SRL = bVar1 & 0xfd;
  stub_empty_0x2a8d1();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isr_sci3_txi(void)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  
  bVar3 = 0;
  bVar2 = T2SRL;
  if ((bVar2 & 4) != 0) {
    bVar2 = T2SRL;
    T2SRL = bVar2 & 0xfb;
    stub_empty_0x2a8d1();
  }
  if ((*(byte *)((uint)bVar3 << 0x10 | 0xff43) & 8) != 0) {
    pbVar1 = (byte *)((uint)bVar3 << 0x10 | 0xff43);
    *pbVar1 = *pbVar1 & 0xf7;
    (*sci3_rx_process_tcu_byte)();
  }
  return;
}



byte compose_status_bits_fe86_fe8a(void)

{
  byte bVar1;
  
  bVar1 = (DAT_0001fe8a & 0x20) == 0;
  if ((DAT_0001fe86 & 0x10) == 0) {
    bVar1 = bVar1 | 2;
  }
  if ((DAT_0001fe86 & 0x20) == 0) {
    bVar1 = bVar1 | 4;
  }
  return bVar1;
}



void peripheral_block_ff15_cold_init(void)

{
  DAT_0001ff15 = 0xa0;
  DAT_0001ff1b = 0;
  DAT_0001ff1c = 0;
  DAT_0001ff14 = 0;
  DAT_0001fe9a = 0;
  DAT_0001ff10 = 0x5a00;
  DAT_0001ff1e = 0x5a00;
  DAT_0001ff1a = 0;
  DAT_0001fee0 = 0;
  DAT_0001feec = 0x71;
  DAT_0001f974 = 0;
  DAT_0001f976 = 0;
  return;
}



void peripheral_block_ff15_warm_init(void)

{
  DAT_0001ff15 = 0xa0;
  DAT_0001ff1b = 0;
  DAT_0001ff1c = 0;
  DAT_0001ff14 = 0;
  DAT_0001fe9a = 0;
  DAT_0001ff10 = 0x5a00;
  DAT_0001ff1e = 0x5a00;
  DAT_0001ff1a = 0;
  DAT_0001fee0 = 0;
  DAT_0001feec = 0x71;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// NMI interrupt service routine (ends in rte, not prts; referenced only from
// the interrupt vector table at 0x10020, not called normally). One-shot latch:
// the first time NMI fires (f974==0), captures the interrupt-pushed return
// context (PC-ish bytes at hardware-pushed SP+8/+0xa) into f974 (low byte)/
// f976 (word) -- a "record where the NMI interrupted execution" debug/fault
// trap, only armed once until something else resets f974 to 0.
// Prototype set to void(void) 2026-07-23 -- NOTE: triggered a "Unknown
// calling convention -- yet parameter storage is locked" warning, likely
// because an ISR-specific convention/locked storage was already in place;
// left as-is rather than risk destabilizing it further, since the RAM
// read/write behavior decompiles correctly regardless.

void isr_nmi(void)

{
  ushort in_stack_00000004;
  undefined2 in_stack_00000006;
  short already_latched;
  
  already_latched = DAT_0000f974;
  if (already_latched == 0) {
    DAT_0000f974 = in_stack_00000004 & 0xff;
    DAT_0000f976 = in_stack_00000006;
  }
  return;
}



void peripheral_reg_ff1d_cold_init(void)

{
  DAT_0001ff1d = 8;
  return;
}



void peripheral_reg_ff1d_warm_init(void)

{
  DAT_0001ff1d = 8;
  return;
}



bool latch_flag_read_fede_bit1(void)

{
  bool bVar1;
  
  bVar1 = (DAT_0001fede & 2) != 0;
  if (bVar1) {
    DAT_0001fede = DAT_0001fede & 0xfd;
  }
  return bVar1;
}



bool check_flag_fede_bit2_clear_if_set(void)

{
  bool bVar1;
  
  bVar1 = (DAT_0001fede & 4) != 0;
  if (bVar1) {
    DAT_0001fede = DAT_0001fede & 0xfb;
  }
  return bVar1;
}



bool latch_flag_read_fede_bit3(void)

{
  bool bVar1;
  
  bVar1 = (DAT_0001fede & 8) != 0;
  if (bVar1) {
    DAT_0001fede = DAT_0001fede & 0xf7;
  }
  return bVar1;
}



void stub_empty_0x16158(void)

{
  return;
}



void stub_empty_0x1615a(void)

{
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void propagate_flag_fe96_to_f0d8_db(void)

{
  DAT_0001f0d8 = DAT_0001fe96;
  DAT_0001f0d9 = DAT_0001fe96;
  DAT_0001f0da = DAT_0001fe96;
  DAT_0001fe97 = DAT_0001fe97 & 0xfc | 3;
  DAT_0001f0db = DAT_0001fe96;
  return;
}



void peripheral_block_ff90_cold_init(void)

{
  DAT_0001ff90 = 0x1e;
  DAT_0001ff91 = 0;
  DAT_0001ff92 = 0;
  DAT_0001ff94 = 0;
  return;
}



void peripheral_block_ff90_warm_init(void)

{
  DAT_0001ff90 = 0x1e;
  DAT_0001ff91 = 0;
  DAT_0001ff92 = 0;
  DAT_0001ff94 = 0;
  return;
}



ushort scale_counter_ff96_to_byte_clamped(void)

{
  ushort uVar1;
  ushort uVar2;
  
  uVar2 = DAT_0001ff96;
  DAT_0001ff96 = 0;
  if ((DAT_0001ff93 & 4) != 0) {
    uVar2 = 0xffff;
    DAT_0001ff93 = DAT_0001ff93 & 0xfb;
  }
  uVar1 = (ushort)((uint)uVar2 * 0x7d);
  if (((short)((uint)uVar2 * 0x7d >> 0x10) == 0) && (uVar1 < 0xff80)) {
    uVar2 = uVar1 + 0x80 >> 8;
  }
  else {
    uVar2 = 0xff;
  }
  return uVar2;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void init_state_block_ff21(void)

{
  DAT_0001ff21 = 0;
  DAT_0001ff31 = 0;
  DAT_0001ff22 = 0;
  DAT_0001ff32 = 0;
  T1OERA = 0x50;
  T1OERB = 0x55;
  DAT_0001ff2c = DAT_0001ff26 + 2;
  DAT_0001ff2e = DAT_0001ff2c;
  DAT_0001ff3c = DAT_0001ff2c;
  DAT_0001ff3e = DAT_0001ff2c;
  DAT_0001ff38 = DAT_0001ff2c;
  DAT_0001ff3a = DAT_0001ff2c;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void peripheral_block_ff21_warm_init(void)

{
  DAT_0001ff21 = 0;
  DAT_0001ff31 = 0;
  DAT_0001ff22 = 0;
  DAT_0001ff32 = 0;
  T1OERA = T1OERA & 0xf0;
  return;
}



// WARNING: Removing unreachable block (ram,0x00016389)
// WARNING: Removing unreachable block (ram,0x0001632b)
// WARNING: Removing unreachable block (ram,0x000162cd)
// WARNING: Removing unreachable block (ram,0x000162fc)
// WARNING: Removing unreachable block (ram,0x0001635a)
// WARNING: Removing unreachable block (ram,0x000163b8)
// Params confirmed correct via disassembly + get_function_variables (duration=Stack[0x4],
// channel_mask=Stack[0x6]). NOTE: contains real hardware busy-wait spin loops (beq #self on
// ff23/ff33 status bits) per channel bit set in channel_mask -- these are genuine blocking polls,
// not decompiler jump-table failures. Decompiler renders them as do{}while(true) with WARNING:
// Removing unreachable block comments because it can't see the external hardware condition that
// breaks the loop.

undefined2 __stdcall_far_allstack
multi_channel_reg_write_and_poll_ff2x_ff3x(ushort duration,ushort channel_mask)

{
  undefined2 in_R0;
  
  if (duration != 0) {
    if ((channel_mask & 1) != 0) {
      do {
      } while( true );
    }
    if ((channel_mask & 2) != 0) {
      do {
      } while( true );
    }
    if ((channel_mask & 4) != 0) {
      do {
      } while( true );
    }
    if ((channel_mask & 8) != 0) {
      do {
      } while( true );
    }
    if ((channel_mask & 0x10) != 0) {
      do {
      } while( true );
    }
    if ((channel_mask & 0x20) != 0) {
      do {
      } while( true );
    }
  }
  return in_R0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isr_adi_adc(void)

{
  byte bVar1;
  
  bVar1 = T1SRAL;
  T1SRAL = bVar1 & 0xfd;
  stub_empty_0x2a8cf();
  return;
}



undefined2
ff2a_set_clamped_to_ff26p2_atomic
          (undefined2 param_1,undefined2 param_2,undefined2 param_3,short param_4)

{
  if (param_4 - (DAT_0001ff26 + 2) < 0) {
    param_4 = DAT_0001ff26 + 2;
  }
  DAT_0001ff2a = param_4;
  return param_1;
}



byte compose_fe86_fe8a_status_nibble(void)

{
  byte bVar1;
  
  if ((DAT_0001fe8a & 4) == 0) {
    bVar1 = DAT_0001fe86 & 0xef;
  }
  else {
    bVar1 = DAT_0001fe86;
  }
  if ((DAT_0001fe8a & 8) == 0) {
    bVar1 = bVar1 & 0xdf;
  }
  return bVar1 & 0xf;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void timer_ff51_ff54_arm_init(void)

{
  DAT_0001ff51 = DAT_0001ff51 & 0xf3;
  DAT_0001ff5a = DAT_0001ff56 + 2;
  DAT_0001ff54 = DAT_0001ff54 | 0xc;
  DAT_0001ff52 = DAT_0001ff52 & 0xfd;
  return;
}



void timer_ff51_ff52_disarm(void)

{
  DAT_0001ff51 = DAT_0001ff51 & 0xf3;
  DAT_0001ff52 = DAT_0001ff52 & 0xfd;
  return;
}



void init_state_block_ff54(void)

{
  DAT_0001ff5a = DAT_0001ff56 + 2;
  DAT_0001ff54 = DAT_0001ff54 | 0xc;
  return;
}



undefined2 __stdcall_far_allstack timer_ff5a_set_clamped_armed(short timeout_value)

{
  undefined2 in_R0;
  
  if (timeout_value - (DAT_0001ff56 + 3) < 0) {
    timeout_value = DAT_0001ff56 + 3;
  }
  DAT_0001ff5a = timeout_value;
  DAT_0001ff54 = DAT_0001ff54 & 0xf7 | 4;
  return in_R0;
}



void serial_fef4_cold_init(void)

{
  if (DAT_000102c3 == '\0') {
    DAT_0001fef5 = 0x7d;
  }
  else {
    DAT_0001fef5 = 0xfa;
  }
  DAT_0001fef4 = 0x81;
  DAT_0001fef9 = 0;
  DAT_0001fef8 = 0x85;
  return;
}



void serial_fef4_warm_init(void)

{
  if (DAT_000102c3 == '\0') {
    DAT_0001fef5 = 0x7d;
  }
  else {
    DAT_0001fef5 = 0xfa;
  }
  DAT_0001fef4 = 0x81;
  DAT_0001fef8 = 0x85;
  return;
}



void serial_fef1_set_clamped(ushort param_1)

{
  if (0xf9 < param_1) {
    param_1 = 0xfa;
  }
  DAT_0001fef1 = (char)param_1;
  return;
}



void serial_fef5_set_clamped(ushort param_1)

{
  if (0xf9 < param_1) {
    param_1 = 0xfa;
  }
  DAT_0001fef5 = (char)param_1;
  return;
}



void serial_fef9_set_clamped(ushort param_1)

{
  if (0xf9 < param_1) {
    param_1 = 0xfa;
  }
  DAT_0001fef9 = (char)param_1;
  return;
}



void peripheral_block_fec0_fed0_cold_init(void)

{
  DAT_0001fec9 = 0xff;
  DAT_0001fec8 = 0;
  SCI1_SCR_0001feca = 0x70;
  DAT_0001fed1 = 0xff;
  DAT_0001fed0 = 0;
  DAT_0001fed2 = 0;
  DAT_0001fec1 = 0x1f;
  DAT_0001fec0 = 0;
  DAT_0001fec2 = 0xb0;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void peripheral_fec0_fed0_state_update(void)

{
  char cVar1;
  
  if ((DAT_0001f0f2 & 2) == 0) {
    if ((DAT_0001f0f2 & 4) == 0) {
      if ((DAT_0001f0f2 & 8) == 0) {
        cVar1 = -1;
      }
      else {
        cVar1 = '\a';
      }
    }
    else {
      cVar1 = '\x1f';
    }
  }
  else {
    cVar1 = '/';
  }
  if (cVar1 != DAT_0001fec9) {
    DAT_0001fec9 = cVar1;
  }
  if (DAT_0001fec8 != '\0') {
    DAT_0001fec8 = '\0';
  }
  if (((DAT_0001ff0d & 0x20) == 0) && ((DAT_0001ff0d & 0x40) == 0)) {
    if (SCI1_SCR_0001feca != 'p') {
      SCI1_SCR_0001feca = 'p';
    }
  }
  if (DAT_0001fed1 != -1) {
    DAT_0001fed1 = -1;
  }
  if (DAT_0001fed0 != '\0') {
    DAT_0001fed0 = '\0';
  }
  if (DAT_0001fed2 != '\0') {
    DAT_0001fed2 = '\0';
  }
  if (DAT_0001fec1 != '\x1f') {
    DAT_0001fec1 = '\x1f';
  }
  if (DAT_0001fec0 != '\0') {
    DAT_0001fec0 = '\0';
  }
  if ((DAT_0001f516 & 0xf000) == 0) {
    DAT_0001f516 = DAT_0001f516 & 0xfdff;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void serial_fec3_load_byte0_init(undefined2 param_1)

{
  uRam0001fec3 = *DAT_00016672;
  DAT_0001fec4 = DAT_0001fec4 & 0x7f;
  DAT_0001fec2 = 0xb0;
  uRam0001f4ba = CONCAT11((char)((ushort)DAT_00016672 >> 8),*DAT_00016672);
  DAT_0001f4d0 = 1;
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: ram
// WARNING: Unknown calling convention -- yet parameter storage is locked

void serial_fec3_load_next_byte(void)

{
  undefined1 *puVar1;
  
  if (DAT_0001f4d0 < 7) {
    puVar1 = *(undefined1 **)((int)(DAT_0001f4d0 << 1) + 0x6672);
    uRam0001fec3 = *puVar1;
    DAT_0001fec4 = DAT_0001fec4 & 0x7f;
    sRam0001f4ba = CONCAT11((char)((ushort)puVar1 >> 8),uRam0001fec3) + sRam0001f4ba;
    DAT_0001f4d0 = DAT_0001f4d0 + 1;
  }
  else {
    DAT_0001fec2 = DAT_0001fec2 & 0x7f;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [CONTEXT ADDED 2026-07-26] Writes a received UART byte into
// tcu_rx_confirmed_byte3_f4c2 (F4C2). "fec5" falls inside the fec0-fed5
// SCI1/2/3 UART register range confirmed elsewhere in this project (see
// f516_hibits_f520_f0f2_mode_select plate comment for the fec0-fed5 SMR/
// BRR/SCR/TDR/SSR/RDR register identification). This is the clearest direct
// evidence that the tcu_rx_main_scheduler frame-validator block (F4C4-F4CC,
// see 0x2aa36) is genuinely fed by a live UART receive path -- most likely a
// dedicated ECU<->TCU serial link, distinct from the SCI1 diagnostic
// protocol (sci1_meta_cmd_dispatch_c0_ff) traced throughout
// mut_verification_status.md. Which SCI channel (1/2/3) "fec5" specifically
// belongs to, and whether it's the same physical UART as the
// tcu_periodic_dispatch (0x2c12b) chain, not yet determined.

void serial_fec5_byte_to_fifo(undefined2 param_1)

{
  byte bVar1;
  
  if (((DAT_0001fec4 & 0x40) != 0) || (bVar1 = DAT_0001fec4, (DAT_0001fec4 & 0x38) != 0)) {
    bVar1 = DAT_0001fec4 & 0x87;
    if ((tcu_rx_frame_state_counter_f4ce < 5) && ((DAT_0001fec4 & 0x38) == 0)) {
      DAT_0001fec4 = bVar1;
      *(ushort *)((int)(tcu_rx_frame_state_counter_f4ce << 1) + -0xb3c) = (ushort)DAT_0001fec5;
      tcu_rx_frame_state_counter_f4ce = tcu_rx_frame_state_counter_f4ce + 1;
      bVar1 = DAT_0001fec4;
    }
  }
  DAT_0001fec4 = bVar1;
  return;
}



// [CONTEXT ADDED 2026-08-07] Called from
// sci1_handshake_status_byte_builder_and_f516_hibit_writer (0x28b89) and
// sci1_send_final_handshake_byte_85_and_park_phase_machine (0x28cc8) --
// transmits the byte staged in F54A (0x55/0xEF/0xF8/0x85, matching the
// publicly-documented MUT-II post-handshake status broadcast "C0 55 EF 85")
// by writing it directly to FECB (SCI1 TDR). Resets F588=0, then if F522
// bit0 armed AND F58C (length) nonzero: writes F54A to FECB, clears FECC
// bit7, sets F588=1, sets F522 bit7, zeroes F590. Sibling/counterpart to
// sci1_tx_response_feeder (0x166d7), which drains any REMAINING bytes in a
// multi-byte response (F588 continuing past 1) after this function sends the
// first. See sci1_periodic_phase_dispatch_f526 plate comment for the full
// handshake state machine this feeds.

ushort sci1_latch_and_send_f54a_handshake_byte(ushort param_1)

{
  undefined2 uVar1;
  
  DAT_0001f588 = 0;
  if (((g_sci1_rx_status_flags_f522 & 1) != 0) && (DAT_0001f58c != 0)) {
    uVar1 = Ram0000f54a;
    DAT_0001fecb = (undefined1)uVar1;
    DAT_0001fecc = DAT_0001fecc & 0x7f;
    DAT_0001f588 = 1;
    g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 | 0x80;
    DAT_0001f590 = 0;
  }
  return param_1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [FOUND 2026-08-07] Autonomous SCI1 transmit-FIFO feeder -- part of the
// answer to "does the ECU send data on its own." Called UNCONDITIONALLY every
// tick from sci1_protocol_state_machine (0x287af), which itself runs every
// scheduler tick via tcu_rx_main_scheduler <- isr_sci2_eri (a Timer-4 compare-
// match periodic tick repurposed through the SCI2-error vector, NOT a real
// SCI2 error/receive event -- see isr_sci2_eri plate comment).
// 
// Logic: checks SCI1 TDR-empty flag (FECC bit7, confirmed SCI1 SSR) AND a
// software "armed" flag (F522 bit0). If both set and the buffer index F588
// hasn't reached the length F58C (max 15 bytes), pulls the next byte from a
// ROM/RAM table at base -0xab6 (indexed by F588<<1) and writes it directly to
// FECB (confirmed SCI1 TDR) -- a genuine byte-by-byte autonomous transmit, no
// external request needed once armed.
// 
// Re-arm/advance side: f588_duty_gate_f516_bit11_set (0x28b62, reached via
// sci1_periodic_phase_dispatch_f526's jump table) clears F522 bit0 and resets
// F58C to 0 once the buffer is drained, sets F516 bit11, and advances phase
// counter F526 (0-7 cycle).
// 
// NOT YET DONE: what fills the -0xab6 buffer and F58C (length) and arms F522
// bit0 in the first place -- that's the real "what does the ECU broadcast"
// question, still open. A parallel, near-identical SCI2 mechanism exists at
// sci_fifo_f58a_advance_and_load_fed3 (0x16786) / sci_request_latch_f58a_conditional
// (0x1677c), writing to FED3 (SCI2 TDR) via F58A/F58E/F524. See mut_verification_status.md
// serial/TCU section for the receive-side chain (serial_fec5_byte_to_fifo etc.)
// this connects to.

void sci1_tx_response_feeder(void)

{
  if (((DAT_0001fecc & 0x80) != 0) && ((g_sci1_rx_status_flags_f522 & 1) != 0)) {
    if ((DAT_0001f588 < DAT_0001f58c) && (DAT_0001f588 < 0xf)) {
      DAT_0001fecb = (undefined1)*(undefined2 *)((int)(DAT_0001f588 << 1) + -0xab6);
      DAT_0001fecc = DAT_0001fecc & 0x7f;
      DAT_0001f588 = DAT_0001f588 + 1;
      g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 | 0x80;
      DAT_0001f590 = 0;
    }
  }
  return;
}



// SCI1 receive-frame byte accumulator. Confirmed 2026-07-22 while tracing the
// sci1_meta_cmd_dispatch_c0_ff (0x28869) command-byte source: on each SCI1
// receive interrupt, appends one live byte (from DAT_0001fecd, the SCI1 UART
// receive-data register) into an 11-byte buffer starting at RAM 0xf534
// (labeled sci1_rx_frame_buf_0), indexed by the running counter DAT_0001f584
// (0-10, computed store address = ((f584<<1)|0x10000)-0xacc, which equals
// 0xf534 exactly when f584==0). Buffer resets (f584=0, f590=0, f522 bit7
// cleared) on frame-boundary/error conditions (f522 bit0==0, or the f522
// bit7 "frame complete" flag already set). f522 bit5 gets set as a
// side-channel flag when fecc bits 3-5 indicate something (not yet named).
// sci1_dispatch_and_latch_response (0x2882b) later reads buffer index 0
// (0xf534) as the command byte and passes it unfiltered to the 0x28869
// dispatcher -- confirms the command byte really is live, wire-controlled
// data, not a fixed/internal value. See mut_verification_status.md "DP=2
// POINTER TABLE ... RESOLVED" section for the full chain.

ushort sci1_rx_frame_accumulator(ushort param_1)

{
  byte bVar1;
  
  bVar1 = DAT_0001fecc & 0x87;
  if ((g_sci1_rx_status_flags_f522 & 1) == 0) {
    g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 & 0xffdf;
  }
  else {
    if ((DAT_0001fecc & 0x38) != 0) {
      g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 | 0x20;
    }
    if ((g_sci1_rx_status_flags_f522 & 0x80) == 0) {
      if (DAT_0001f584 < 0xb) {
        DAT_0001fecc = bVar1;
        *(ushort *)((int)(DAT_0001f584 << 1) + -0xacc) = (ushort)DAT_0001fecd;
        g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 | 0x40;
        DAT_0001f584 = DAT_0001f584 + 1;
        DAT_0001f590 = 0;
        return param_1;
      }
      DAT_0001fecc = bVar1;
      return param_1;
    }
  }
  DAT_0001fecc = bVar1;
  DAT_0001f590 = 0;
  DAT_0001f584 = 0;
  g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 & 0xff7f;
  return param_1;
}



// [CORRECTED 2026-08-07] Was previously mislabeled "SCI1 request latch" in a
// triage note -- CORRECTED: writes to FED3 (confirmed SCI2 TDR, not SCI1's
// FECB), so this is the SCI2-side arm/latch function, sibling to
// sci_fifo_f58a_advance_and_load_fed3 (0x16786) which drains the same F58A/
// F58E/F524 state via a multi-byte table instead of this function's single
// F576 byte. Clears F58A, then if F524 bit0 set AND F58E != 0: writes F576
// directly to FED3, clears FED4 bit7, sets F58A=1, sets F524 bit7, zeroes
// F592. Reads like "arm a single pending SCI2 byte" as opposed to the table-
// driven multi-byte drain in the sibling function. Caller still not found via
// static xref -- same blind spot as sci_fifo_f58a_advance_and_load_fed3. See
// mut_verification_status.md serial/TCU section.

ushort sci_request_latch_f58a_conditional(ushort param_1)

{
  undefined2 uVar1;
  
  DAT_0001f58a = 0;
  if (((DAT_0001f524 & 1) != 0) && (DAT_0001f58e != 0)) {
    uVar1 = DAT_0000f576;
    DAT_0001fed3 = (undefined1)uVar1;
    DAT_0001fed4 = DAT_0001fed4 & 0x7f;
    DAT_0001f58a = 1;
    DAT_0001f524 = DAT_0001f524 | 0x80;
    DAT_0001f592 = 0;
  }
  return param_1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [CONTEXT ADDED 2026-08-07] SCI2 sibling of sci1_tx_response_feeder (0x166d7)
// -- same autonomous transmit-FIFO pattern, different registers/counters.
// Checks FED4 bit7 (SCI2 SSR TDRE) AND F524 bit0 (armed flag); if set and
// F58A < F58E (buffer index < length, max 7 bytes), pulls next byte from a
// table at base -0xa8a and writes to FED3 (confirmed SCI2 TDR). Not yet
// confirmed which scheduler/tick calls this function (no static xref found --
// likely reached via the same computed-call pattern tcu_rx_main_scheduler uses
// for serial_fec5_byte_to_fifo). Re-arm side: sci_request_latch_f58a_conditional
// (0x1677c) -- same shape, gates on F524 bit0 + F58E != 0, loads a single byte
// from F576 into FED3 directly rather than the table. See mut_verification_status.md
// serial/TCU section.

void sci_fifo_f58a_advance_and_load_fed3(void)

{
  if (((DAT_0001fed4 & 0x80) != 0) && ((DAT_0001f524 & 1) != 0)) {
    if ((DAT_0001f58a < DAT_0001f58e) && (DAT_0001f58a < 7)) {
      DAT_0001fed3 = (undefined1)*(undefined2 *)((int)(DAT_0001f58a << 1) + -0xa8a);
      DAT_0001fed4 = DAT_0001fed4 & 0x7f;
      DAT_0001f58a = DAT_0001f58a + 1;
      DAT_0001f524 = DAT_0001f524 | 0x80;
      DAT_0001f592 = 0;
    }
  }
  return;
}



ushort fedd5_fifo_rx_store_byte(ushort param_1)

{
  byte bVar1;
  
  bVar1 = DAT_0001fed4 & 0x87;
  if ((DAT_0001f524 & 1) == 0) {
    DAT_0001f524 = DAT_0001f524 & 0xffdf;
  }
  else {
    if ((DAT_0001fed4 & 0x38) != 0) {
      DAT_0001f524 = DAT_0001f524 | 0x20;
    }
    if ((DAT_0001f524 & 0x80) == 0) {
      if (DAT_0001f586 < 7) {
        DAT_0001fed4 = bVar1;
        *(ushort *)((int)(DAT_0001f586 << 1) + -0xa98) = (ushort)DAT_0001fed5;
        DAT_0001f524 = DAT_0001f524 | 0x40;
        DAT_0001f586 = DAT_0001f586 + 1;
        DAT_0001f592 = 0;
        return param_1;
      }
      DAT_0001fed4 = bVar1;
      return param_1;
    }
  }
  DAT_0001fed4 = bVar1;
  DAT_0001f592 = 0;
  DAT_0001f586 = 0;
  DAT_0001f524 = DAT_0001f524 & 0xff7f;
  return param_1;
}



// WARNING: Unable to track spacebase fully for stack

void isr_tpu5_tgi5a_sci1_rx_poll(ushort param_1)

{
  short sVar1;
  byte bVar2;
  
  bVar2 = DTC_DTEF;
  if ((bVar2 & 0x20) == 0) {
    sci1_rx_frame_accumulator(param_1);
  }
  else {
    bVar2 = SCI1_SSR;
    SCI1_SSR = bVar2 & 0xbf;
    bVar2 = SCI1_SSR;
    SCI1_SSR = bVar2 & 0xdf;
    bVar2 = SCI1_SSR;
    SCI1_SSR = bVar2 & 0xef;
    bVar2 = SCI1_SSR;
    SCI1_SSR = bVar2 & 0xf7;
    bVar2 = SCI1_SCR;
    SCI1_SCR = bVar2 & 0xbf;
    sVar1 = DAT_0000f9c0;
    if (sVar1 != 0) {
      g_sci1_rx_frame_error_flag_f9c2 = 1;
    }
  }
  return;
}



// WARNING: Unable to track spacebase fully for stack

void isr_tpu5_tgi5b_sci1_tie_clear(void)

{
  byte scr_value;
  
  scr_value = SCI1_SCR;
  SCI1_SCR = scr_value & 0x7f;
  return;
}



// WARNING: Unable to track spacebase fully for stack

void isr_tpu5_tci5u(void)

{
  byte bVar1;
  undefined1 uVar2;
  
  uVar2 = SCI2_SSR;
  bVar1 = SCI3_SSR;
  if ((bVar1 & 0x80) != 0) {
    (*serial_fec3_load_next_byte)();
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void init_state_sentinels_f5d2(void)

{
  DAT_0001ff41 = DAT_0001ff41 & 0xfd | 1;
  DAT_0001ff61 = DAT_0001ff61 & 0xfd | 1;
  DAT_0001ff42 = DAT_0001ff42 | 1;
  DAT_0001ff62 = DAT_0001ff62 & 0xfe;
  DAT_0001f5de = 0xffff;
  DAT_0001f5e0 = 0xffff;
  DAT_0001f5d2 = 0xffff;
  DAT_0001f5d4 = 0xffff;
  DAT_0001f5d6 = 0xffff;
  DAT_0001f5d8 = 0xffff;
  DAT_0001ef66 = 0xffff;
  DAT_0001ef68 = 0xffff;
  return;
}



void peripheral_ff42_ff62_enable(void)

{
  DAT_0001ff42 = DAT_0001ff42 | 1;
  DAT_0001ff62 = DAT_0001ff62 & 0xfe;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [RE-VERIFIED 2026-07-14] PIN MAPPING CONFIRMED for the 2026-07-13 RPM lead.
// (Note: function has since been renamed isr_ipu_ch2ch4_input_capture, correcting
// the earlier isr_sci3_eri mislabel referenced in review.md item 7.)
// 
// This function polls P5DR bit 4 (btst.b 0x4,@0xfe8a:16 at 0x1690d/0x1691e) as a
// plain GPIO input via software polling, NOT via the IPU's input-capture hardware
// path (no T2IOC1 capture register - T2GR1H/T2CNTH/T4GR1H - is touched at this
// poll site; those are read elsewhere in the function for the period-delta calc).
// 
// Per H8/539F hardware manual (H8_539F_hardware.md, Appendix D.3, Table D-19 /
// Fig 10-27): P54 is multiplexed with T2IOC1 (16-bit IPU Timer 2, Input Capture/
// Output Compare channel 1). Bit 4 of P5DR = P54. Confirmed.
// 
// This settles the open question from the 2026-07-13 session: the pin this ISR
// polls is a genuine hardware timer capture-capable pin (T2IOC1), consistent with
// a real external sensor edge input, not an arbitrary/repurposed GPIO. Supports
// (does not refute) the cam-position-sensor / coarse-RPM-source theory for this
// signal chain (P54/T2IOC1 -> period-delta -> F5DE -> F384 -> F5CA/F5CC ->
// engine_torque_pct_scale_calc @0x2184b).
// 
// [UPDATE 2026-07-15] SECOND OUTPUT TAP CONFIRMED. This ISR has two symmetric
// edge-detect branches (rising-edge block @0x16936, falling-edge block @0x169dc)
// handling alternating captures from the same 2-tooth cam signal. Each branch
// independently computes a normalized period-delta (mulxu.w 0x10 scale + signed
// rounding correction). The rising branch's result feeds F5D2/F5D4 -> F5DE ->
// F384 -> F5CA/F5CC chain (as above). The FALLING branch's result feeds
// F5D6/F5D8 directly - a previously untraced second output, written at
// 0x16a38/0x16a3c, consumed by tcu_periodic_dispatch (@0x2c12b) ->
// tcu_shift_ratio_buffer_update (@0x2c16e). Two independent downstream consumers
// (torque-scale calc and TCU shift-ratio logic) drawing period data from the same
// capture ISR is strong corroborating evidence this is a genuine engine-speed
// signal, not incidental - a real RPM source would legitimately feed both paths.
// See plate comments on tcu_shift_ratio_buffer_update and
// tcu_torque_converter_slip_calc for the downstream trace. Disassembly for the
// whole ISR is clean under the current grammar - no decode issues.
// 
// Still open: physical confirmation that P54/T2IOC1 on this ECU's pinout is wired
// to the actual camshaft position sensor per the RVR wiring diagram (this comment
// confirms the SILICON-level pin identity; the BOARD-level wiring to the physical
// sensor is a separate, still-unconfirmed step). Also open: absolute RPM
// scale/units for F5D6/F5D8 - the 0x10 divisor's real-world meaning and timer
// tick rate haven't been derived yet.

void isr_ipu_ch2ch4_input_capture(void)

{
  byte *pbVar1;
  int iVar2;
  undefined2 uVar3;
  short sVar4;
  byte bVar5;
  short sVar6;
  ushort uVar7;
  ushort uVar8;
  short sVar9;
  ushort uVar10;
  ushort uVar11;
  ushort uVar12;
  byte bVar13;
  ushort in_SR;
  ushort uVar14;
  byte in_CF;
  byte in_VF;
  byte in_ZF;
  byte in_NF;
  
  bVar13 = 0;
  uVar14 = (((in_SR & 0xfffe | (ushort)in_CF) & 0xfffd | (ushort)in_VF << 1) & 0xfffb |
           (ushort)in_ZF << 2) & 0xfff7 | (ushort)in_NF << 3;
  bVar5 = T2SRL;
  T2SRL = bVar5 & 0xfe;
  sVar6 = T2GR1H;
  uVar10 = T4GR1H;
  sVar9 = sVar6 + 0xd;
  bVar5 = T2CRL;
  if ((bVar5 & 1) == 0) {
    do {
      bVar5 = P5DR;
      if ((bVar5 & 0x10) != 0) {
        return;
      }
      sVar4 = T2CNTH;
      uVar3 = T2CNTH;
      uVar3 = T2CNTH;
    } while ((sVar9 != sVar4) && (-1 < sVar9 - sVar4));
  }
  else {
    do {
      bVar5 = P5DR;
      if ((bVar5 & 0x10) == 0) {
        return;
      }
      sVar4 = T2CNTH;
      uVar3 = T2CNTH;
      uVar3 = T2CNTH;
    } while ((sVar9 != sVar4) && (-1 < sVar9 - sVar4));
  }
  bVar5 = T2CRL;
  if ((bVar5 & 1) == 0) {
    sVar9 = DAT_0000f5e4;
    uVar14 = sVar6 - sVar9;
    if (uVar14 < 0xfe) {
      uVar3 = g_ipu_ch4_capture_settle_counter;
      uVar7 = g_ipu_ch4_capture_settle_counter;
      uVar3 = g_ipu_ch4_capture_settle_counter;
      if (uVar7 < 0x28) {
        return;
      }
    }
    uVar7 = g_ipu_ch2_capture_settle_counter;
    uVar3 = g_ipu_ch2_capture_settle_counter;
    if (uVar7 < 0x28) {
      sVar4 = DAT_0000f5e2;
      ipu_ch2ch4_period_delta_raw = sVar6 - sVar4;
      DAT_0000f5e2 = sVar6;
      sVar9 = DAT_0000f5da;
      uVar8 = uVar10 - sVar9;
      iVar2 = (uint)(ushort)(sVar6 - sVar4) * 0x10;
      uVar7 = (ushort)((uint)iVar2 >> 0x10);
      uVar12 = (ushort)iVar2;
      uVar11 = uVar12 - uVar8;
      uVar14 = -(ushort)(uVar12 < uVar8);
      if (uVar7 < uVar7 + (uVar12 < uVar8)) {
        if ((0x1f < -uVar11 && uVar11 != 0xffe0) && (uVar7 != 0)) {
          uVar7 = uVar7 - 1;
        }
      }
      else if (0x20 < uVar11) {
        uVar7 = uVar7 + 1;
      }
      DAT_0000f5d2 = uVar7;
      DAT_0000f5d4 = uVar8;
      DAT_0000f5da = uVar10;
      uVar10 = uVar8;
    }
    else {
      DAT_0000f5e2 = sVar6;
      DAT_0000f5da = uVar10;
      uVar7 = 0xffff;
      ipu_ch2ch4_period_delta_raw = 0xffff;
      DAT_0000f5d2 = 0xffff;
      DAT_0000f5d4 = 0xffff;
    }
    g_ipu_ch2_capture_settle_counter = 0;
    (*tcu_shift_torque_and_knock_mgmt)(uVar7,uVar10,uVar14,0x100);
    *(short *)((uint)bVar13 << 0x10 | 0xf0e2) =
         *(short *)((uint)bVar13 << 0x10 | 0xff46) - *(short *)((uint)bVar13 << 0x10 | 0xf5e2);
    pbVar1 = (byte *)((uint)bVar13 << 0x10 | 0xff41);
    *pbVar1 = *pbVar1 | 1;
    pbVar1 = (byte *)((uint)bVar13 << 0x10 | 0xff41);
    *pbVar1 = *pbVar1 & 0xfd;
    pbVar1 = (byte *)((uint)bVar13 << 0x10 | 0xff61);
    *pbVar1 = *pbVar1 | 1;
    pbVar1 = (byte *)((uint)bVar13 << 0x10 | 0xff61);
    *pbVar1 = *pbVar1 & 0xfd;
    return;
  }
  sVar9 = DAT_0000f5e2;
  if ((ushort)(sVar6 - sVar9) < 0xa2) {
    uVar3 = g_ipu_ch2_capture_settle_counter;
    uVar10 = g_ipu_ch2_capture_settle_counter;
    uVar3 = g_ipu_ch2_capture_settle_counter;
    if (uVar10 < 0x28) {
      return;
    }
  }
  sci1_timing_period_tracker();
  if (*(ushort *)((uint)bVar13 << 0x10 | 0xef68) < 0x28) {
    uVar10 = uVar14 - *(short *)((uint)bVar13 << 0x10 | 0xf5e4);
    *(ushort *)((uint)bVar13 << 0x10 | 0xf5e0) = uVar10;
    *(ushort *)((uint)bVar13 << 0x10 | 0xf5e4) = uVar14;
    uVar7 = sVar6 - *(short *)((uint)bVar13 << 0x10 | 0xf5dc);
    iVar2 = (uint)uVar10 * 0x10;
    uVar10 = (ushort)((uint)iVar2 >> 0x10);
    uVar14 = (ushort)iVar2;
    uVar12 = uVar14 - uVar7;
    if (uVar10 < uVar10 + (uVar14 < uVar7)) {
      if ((0x1f < -uVar12 && uVar12 != 0xffe0) && (uVar10 != 0)) {
        uVar10 = uVar10 - 1;
      }
    }
    else if (0x20 < uVar12) {
      uVar10 = uVar10 + 1;
    }
    *(ushort *)((uint)bVar13 << 0x10 | 0xf5d6) = uVar10;
    *(ushort *)((uint)bVar13 << 0x10 | 0xf5d8) = uVar7;
    *(short *)((uint)bVar13 << 0x10 | 0xf5dc) = sVar6;
  }
  else {
    *(ushort *)((uint)bVar13 << 0x10 | 0xf5e4) = uVar14;
    *(short *)((uint)bVar13 << 0x10 | 0xf5dc) = sVar6;
    *(undefined2 *)((uint)bVar13 << 0x10 | 0xf5e0) = 0xffff;
    *(undefined2 *)((uint)bVar13 << 0x10 | 0xf5d6) = 0xffff;
    *(undefined2 *)((uint)bVar13 << 0x10 | 0xf5d8) = 0xffff;
  }
  *(undefined2 *)((uint)bVar13 << 0x10 | 0xef68) = 0;
  (*tcu_shift_timing_state_machine)();
  *(short *)((uint)bVar13 << 0x10 | 0xf0e4) =
       *(short *)((uint)bVar13 << 0x10 | 0xff46) - *(short *)((uint)bVar13 << 0x10 | 0xf5e4);
  pbVar1 = (byte *)((uint)bVar13 << 0x10 | 0xff41);
  *pbVar1 = *pbVar1 | 2;
  pbVar1 = (byte *)((uint)bVar13 << 0x10 | 0xff41);
  *pbVar1 = *pbVar1 & 0xfe;
  pbVar1 = (byte *)((uint)bVar13 << 0x10 | 0xff61);
  *pbVar1 = *pbVar1 | 2;
  pbVar1 = (byte *)((uint)bVar13 << 0x10 | 0xff61);
  *pbVar1 = *pbVar1 & 0xfe;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar flag_read_ff43_bit0_inverted(void)

{
  return (DAT_0001ff43 & 1) == 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void peripheral_block_ff70_ff88_cold_init(void)

{
  DAT_0001ff70 = 6;
  DAT_0001ff71 = 2;
  DAT_0001f978 = 0;
  pwm_channel_control_flags_f9a0 = 0xeee;
  DAT_0001f9a4 = 0;
  DAT_0001f9ac = 0;
  DAT_0001f9b4 = 0;
  DAT_0001f9a8 = 0;
  DAT_0001f9b0 = 0;
  DAT_0001f9b8 = 0;
  DAT_0001f9a2 = 0xffff;
  DAT_0001f9aa = 0xffff;
  DAT_0001f9b2 = 0xffff;
  DAT_0001f9a6 = 0xffff;
  DAT_0001f9ae = 0xffff;
  DAT_0001f9b6 = 0xffff;
  DAT_0001ff7a = DAT_0001ff76 + 0x186a;
  DAT_0001ff7c = DAT_0001ff76 + 7000;
  DAT_0001ff7e = DAT_0001ff76 + 0x1e46;
  if (DAT_000102e2 == '\0') {
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xc3;
    DAT_0001ff72 = DAT_0001ff72 & 0xe8;
  }
  else {
    DAT_0001ff72 = DAT_0001ff72 & 0xec | 4;
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xf3;
  }
  if (DAT_000102fe == '\x02') {
    DAT_0001ff72 = DAT_0001ff72 | 8;
  }
  else {
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0x3f;
    DAT_0001ff72 = DAT_0001ff72 & 0xf7;
  }
  DAT_0001ff80 = 1;
  DAT_0001ff88 = 1;
  DAT_0001ff8a = 0x7ff;
  DAT_0001ff81 = 0x20;
  DAT_0001ff82 = 0;
  DAT_0001ff84 = 0;
  DAT_0001ff35 = 0;
  if (DAT_000102e4 == '\x02') {
    DAT_0001ff35 = 8;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void peripheral_block_ff70_ff88_warm_init(void)

{
  DAT_0001ff70 = 6;
  DAT_0001ff71 = 2;
  if (DAT_000102e2 == '\0') {
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xc3;
    DAT_0001ff72 = DAT_0001ff72 & 0xe8;
  }
  else {
    DAT_0001ff72 = DAT_0001ff72 & 0xec | 4;
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xf3;
  }
  if (DAT_000102fe == '\x02') {
    DAT_0001ff72 = DAT_0001ff72 | 8;
  }
  else {
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0x3f;
    DAT_0001ff72 = DAT_0001ff72 & 0xf7;
  }
  DAT_0001ff80 = 1;
  if (DAT_0001ff8a != 0x7ff) {
    DAT_0001ff8a = 0x7ff;
  }
  DAT_0001ff81 = 0x20;
  DAT_0001ff82 = 0;
  DAT_0001ff84 = 0;
  DAT_0001ff35 = 0;
  if (DAT_000102e4 == '\x02') {
    DAT_0001ff35 = 8;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f978_read_and_clear(void)

{
  ushort usValue;
  
  usValue = DAT_0001f978;
  DAT_0001f978 = 0;
  return usValue;
}



void f978_increment_on_ff73_bit0(void)

{
  if ((DAT_0001ff73 & 1) != 0) {
    DAT_0001ff73 = DAT_0001ff73 & 0xfe;
    if (DAT_0001f978 < 0xff) {
      DAT_0001f978 = DAT_0001f978 + 1;
    }
    else {
      DAT_0001f978 = 0xff;
    }
  }
  return;
}



// [EXISTING FUNCTION, COMMENT ADDED 2026-08-08] ff88_set_scaled_clamped:
// scales input*16, clamps to [1,2048], writes T6GR1H (0xff88, real H8
// Timer 6 PWM duty/compare register). Sole known caller this pass:
// serial_fef9_duty_scale_and_set (0x15a06), part of the radiator-fan PWM
// chain (renamed from "EGR" -- see radfan_pwm_duty_target_f494_calc's
// plate comment, 0x18bc2, for the correction and reasoning). See
// mut_verification_status.md "DISCREPANCY CHECKED..." (2026-08-08).

void ff88_set_scaled_clamped(short sValue)

{
  DAT_0001ff88 = sValue * 0x10;
  if (DAT_0001ff88 == 0) {
    DAT_0001ff88 = 1;
  }
  if (0x7ff < DAT_0001ff88) {
    DAT_0001ff88 = 0x800;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isr_tpu3_tgi3a(void)

{
  bool bVar1;
  undefined2 uVar2;
  short sVar3;
  ushort uScratch;
  short sVar4;
  ushort uVar5;
  byte bRegScratch;
  
  bRegScratch = T5SRL;
  T5SRL = bRegScratch & 0xfd;
  uScratch = DAT_0000f9a0;
  if ((uScratch & 4) == 0) {
    bRegScratch = T5OER;
    T5OER = bRegScratch & 0xfb;
    bRegScratch = T5OER;
    T5OER = bRegScratch & 0xf7;
    sVar3 = T5GR2H;
    sVar4 = DAT_0000f9a6;
    T5GR2H = sVar3 + sVar4;
    uScratch = DAT_0000f9a0;
    DAT_0000f9a0 = uScratch | 8;
    uScratch = DAT_0000f9a0;
    DAT_0000f9a0 = uScratch & 0xfffe;
    return;
  }
  bVar1 = false;
  uScratch = DAT_0000f9a0;
  if ((uScratch & 8) != 0) {
    uScratch = DAT_0000f9a6;
    uVar5 = DAT_0000f9a8;
    if ((uVar5 == 0) || (uScratch <= uVar5)) {
      uVar5 = uScratch >> 1;
    }
    uScratch = uScratch - uVar5;
    uVar2 = DAT_0000f9a4;
    DAT_0000f9a8 = uVar2;
    uVar2 = DAT_0000f9a2;
    DAT_0000f9a6 = uVar2;
    sVar4 = DAT_0000f9a8;
    if (sVar4 != 0) {
      bVar1 = true;
    }
    goto LAB_00016cfa;
  }
  uScratch = DAT_0000f9a0;
  DAT_0000f9a0 = uScratch | 1;
  sVar4 = DAT_0000f9a4;
  if (sVar4 == 0) {
    sVar4 = DAT_0000f9a8;
    if (sVar4 == 0) goto LAB_00016ca8;
  }
  else {
    sVar4 = DAT_0000f9a8;
    if (sVar4 != 0) {
LAB_00016ca8:
      uVar2 = DAT_0000f9a4;
      DAT_0000f9a8 = uVar2;
      uVar2 = DAT_0000f9a2;
      DAT_0000f9a6 = uVar2;
    }
  }
  uScratch = DAT_0000f9a8;
  if (uScratch != 0) {
    uVar2 = DAT_0000f9a6;
    uVar5 = DAT_0000f9a6;
    uVar2 = DAT_0000f9a6;
    if (uScratch < uVar5) goto LAB_00016cfa;
    bVar1 = true;
  }
  uScratch = DAT_0000f9a6;
  uScratch = uScratch >> 1;
LAB_00016cfa:
  sVar4 = T5GR2H;
  sVar4 = uScratch + sVar4;
  if (uScratch < 500) {
    sVar3 = T5CNTH;
    if (sVar4 - (sVar3 + 3) < 0) {
      sVar4 = sVar3 + 3;
    }
  }
  T5GR2H = sVar4;
  uScratch = DAT_0000f9a0;
  if ((uScratch & 2) == 0) {
    bVar1 = (bool)(bVar1 ^ 1);
  }
  if (bVar1) {
    bRegScratch = T5OER;
    T5OER = bRegScratch | 8;
    bRegScratch = T5OER;
    T5OER = bRegScratch & 0xfb;
  }
  else {
    bRegScratch = T5OER;
    T5OER = bRegScratch | 4;
    bRegScratch = T5OER;
    T5OER = bRegScratch & 0xf7;
  }
  uScratch = DAT_0000f9a0;
  DAT_0000f9a0 = uScratch ^ 8;
  return;
}



void pwm_ch0_period_track_update(void)

{
  ushort uHalfPeriod;
  bool bEdgeFlag;
  
  DAT_0001ff73 = DAT_0001ff73 & 0xfb;
  if ((pwm_channel_control_flags_f9a0 & 0x40) == 0) {
    pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 & 0xffef | 0x80;
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xcf;
    DAT_0001ff7c = DAT_0001ff7c + DAT_0001f9ae;
    return;
  }
  bEdgeFlag = false;
  if ((pwm_channel_control_flags_f9a0 & 0x80) != 0) {
    if ((DAT_0001f9b0 == 0) || (DAT_0001f9ae <= DAT_0001f9b0)) {
      DAT_0001f9b0 = DAT_0001f9ae >> 1;
    }
    uHalfPeriod = DAT_0001f9ae - DAT_0001f9b0;
    DAT_0001f9b0 = DAT_0001f9ac;
    DAT_0001f9ae = DAT_0001f9aa;
    if (DAT_0001f9ac != 0) {
      bEdgeFlag = true;
    }
    goto LAB_00016ddb;
  }
  pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 | 0x10;
  if (DAT_0001f9ac == 0) {
    if (DAT_0001f9b0 == 0) goto LAB_00016d89;
  }
  else if (DAT_0001f9b0 != 0) {
LAB_00016d89:
    DAT_0001f9b0 = DAT_0001f9ac;
    DAT_0001f9ae = DAT_0001f9aa;
  }
  if (DAT_0001f9b0 != 0) {
    uHalfPeriod = DAT_0001f9b0;
    if (DAT_0001f9b0 < DAT_0001f9ae) goto LAB_00016ddb;
    bEdgeFlag = true;
  }
  uHalfPeriod = DAT_0001f9ae >> 1;
LAB_00016ddb:
  DAT_0001ff7c = uHalfPeriod + DAT_0001ff7c;
  if ((uHalfPeriod < 500) && (DAT_0001ff7c - (DAT_0001ff76 + 3) < 0)) {
    DAT_0001ff7c = DAT_0001ff76 + 3;
  }
  if ((pwm_channel_control_flags_f9a0 & 0x20) == 0) {
    bEdgeFlag = (bool)(bEdgeFlag ^ 1);
  }
  if (bEdgeFlag) {
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xef | 0x20;
  }
  else {
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xdf | 0x10;
  }
  pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 ^ 0x80;
  return;
}



void pwm_ch1_period_track_update(void)

{
  ushort uHalfPeriod;
  bool bEdgeFlag;
  
  DAT_0001ff73 = DAT_0001ff73 & 0xf7;
  if ((pwm_channel_control_flags_f9a0 & 0x400) == 0) {
    pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 & 0xfeff | 0x800;
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0x3f;
    DAT_0001ff7e = DAT_0001ff7e + DAT_0001f9b6;
    return;
  }
  bEdgeFlag = false;
  if ((pwm_channel_control_flags_f9a0 & 0x800) != 0) {
    if ((DAT_0001f9b8 == 0) || (DAT_0001f9b6 <= DAT_0001f9b8)) {
      DAT_0001f9b8 = DAT_0001f9b6 >> 1;
    }
    uHalfPeriod = DAT_0001f9b6 - DAT_0001f9b8;
    DAT_0001f9b8 = DAT_0001f9b4;
    DAT_0001f9b6 = DAT_0001f9b2;
    if (DAT_0001f9b4 != 0) {
      bEdgeFlag = true;
    }
    goto LAB_00016eb7;
  }
  pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 | 0x100;
  if (DAT_0001f9b4 == 0) {
    if (DAT_0001f9b8 == 0) goto LAB_00016e65;
  }
  else if (DAT_0001f9b8 != 0) {
LAB_00016e65:
    DAT_0001f9b8 = DAT_0001f9b4;
    DAT_0001f9b6 = DAT_0001f9b2;
  }
  if (DAT_0001f9b8 != 0) {
    uHalfPeriod = DAT_0001f9b8;
    if (DAT_0001f9b8 < DAT_0001f9b6) goto LAB_00016eb7;
    bEdgeFlag = true;
  }
  uHalfPeriod = DAT_0001f9b6 >> 1;
LAB_00016eb7:
  DAT_0001ff7e = uHalfPeriod + DAT_0001ff7e;
  if ((uHalfPeriod < 500) && (DAT_0001ff7e - (DAT_0001ff76 + 3) < 0)) {
    DAT_0001ff7e = DAT_0001ff76 + 3;
  }
  if ((pwm_channel_control_flags_f9a0 & 0x200) == 0) {
    bEdgeFlag = (bool)(bEdgeFlag ^ 1);
  }
  if (bEdgeFlag) {
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0xbf | 0x80;
  }
  else {
    pwm_channel_track_status_ff74 = pwm_channel_track_status_ff74 & 0x7f | 0x40;
  }
  pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 ^ 0x800;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isr_tpu3_tgi3b(void)

{
  byte bVar1;
  byte bVar2;
  
  bVar2 = 0;
  bVar1 = T5SRL;
  if ((bVar1 & 4) != 0) {
    pwm_ch0_period_track_update();
  }
  if ((*(byte *)((uint)bVar2 << 0x10 | 0xff73) & 8) != 0) {
    pwm_ch1_period_track_update();
  }
  return;
}



bool f9a0_read_clear_bit0_atomic(void)

{
  bool bVar1;
  
  bVar1 = (pwm_channel_control_flags_f9a0 & 1) != 0;
  if (bVar1) {
    pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 & 0xfffe;
  }
  return bVar1;
}



bool f9a0_read_clear_bit4_atomic(void)

{
  bool bWasSet;
  
  bWasSet = (pwm_channel_control_flags_f9a0 & 0x10) != 0;
  if (bWasSet) {
    pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 & 0xffef;
  }
  return bWasSet;
}



bool f9a0_read_clear_bit8_atomic(void)

{
  bool bVar1;
  
  bVar1 = (pwm_channel_control_flags_f9a0 & 0x100) != 0;
  if (bVar1) {
    pwm_channel_control_flags_f9a0 = pwm_channel_control_flags_f9a0 & 0xfeff;
  }
  return bVar1;
}



void peripheral_block_ff00_cold_init(void)

{
  DAT_0001ff00 = 0;
  DAT_0001ff01 = 0;
  DAT_0001ff02 = 0x56;
  DAT_0001ff03 = 0x36;
  DAT_0001ff04 = 0;
  DAT_0001ff05 = 0x22;
  DAT_0001ff08 = 0;
  DAT_0001ff09 = 0;
  DAT_0001ff0a = 1;
  DAT_0001ff0b = 0;
  DAT_0001ff0c = 0;
  DAT_0001ff0d = 0;
  DAT_0001f9c4 = 0;
  DAT_0001f9c6 = SUB42(&DAT_0000f9c8,0);
  DAT_0001f9c8 = SUB42(&DAT_0000f9c8,0);
  DAT_0001f9ca = 1;
  DAT_0001ff52 = DAT_0001ff52 & 0xf7;
  DAT_0001ff62 = DAT_0001ff62 & 0xf5;
  DAT_0001ff54 = DAT_0001ff54 & 0x3f;
  DAT_0001ff64 = DAT_0001ff64 & 0x33;
  return;
}



ushort peripheral_block_ff00_warm_init(ushort param_1)

{
  DAT_0001ff00 = 0;
  DAT_0001ff01 = 0;
  DAT_0001ff02 = 0x56;
  DAT_0001ff03 = 0x36;
  DAT_0001ff04 = 0;
  DAT_0001ff05 = 0x22;
  if (((DAT_00010200 & 1) != 0) && ((DAT_0001f520 & 0xf8) == 0)) {
    DAT_0001ff05 = 0x72;
  }
  DAT_0001ff08 = 0;
  DAT_0001ff09 = 0;
  DAT_0001ff0a = 1;
  DAT_0001ff0b = 0;
  DAT_0001ff0c = 0;
  if (((DAT_00010200 & 1) == 0) || ((DAT_0001f520 & 0xf8) != 0)) {
    DAT_0001ff0d = 0;
  }
  else {
    DAT_0001ff0d = DAT_0001ff0d & 0x60;
  }
  DAT_0001f9c4 = 0;
  DAT_0001f9c6 = SUB42(&DAT_0000f9c8,0);
  DAT_0001f9c8 = SUB42(&DAT_0000f9c8,0);
  DAT_0001f9ca = 1;
  DAT_0001ff52 = DAT_0001ff52 & 0xf7;
  DAT_0001ff62 = DAT_0001ff62 & 0xf5;
  DAT_0001ff54 = DAT_0001ff54 & 0x3f;
  DAT_0001ff64 = DAT_0001ff64 & 0x33;
  return param_1;
}



void sci1_rx_setup(void)

{
  DAT_0001fecc = DAT_0001fecc & 0x87;
  DAT_0001f9c2 = 0;
  DAT_0001f9ba = 0x2000;
  DAT_0001f9bc = SUB42(&SCI1_RDR,0);
  DAT_0001f9be = SUB42(&DAT_0000f815,0);
  DAT_0001f9c0 = 0x33;
  SCI1_SCR_0001feca = SCI1_SCR_0001feca & 0x7b | 0x50;
  DAT_0001ff0d = DAT_0001ff0d & 0xbf | 0x20;
  return;
}



undefined2 sci1_tx_setup(undefined2 param_1)

{
  undefined2 in_stack_00000006;
  
  DAT_0001f9ba = 0x4000;
  DAT_0001f9bc = SUB42(&DAT_0000f7e2,0);
  DAT_0001f9be = SUB42(&SCI1_TDR,0);
  DAT_0001f9c0 = in_stack_00000006;
  SCI1_SCR_0001feca = SCI1_SCR_0001feca & 0xbb | 0xa0;
  DAT_0001ff0d = DAT_0001ff0d & 0xdf | 0x40;
  DAT_0001fecb = DAT_0001f7e1;
  DAT_0001fecc = DAT_0001fecc & 0x7f;
  return param_1;
}



void ff0d_clear_bits5_6(void)

{
  DAT_0001ff0d = DAT_0001ff0d & 0x9f;
  return;
}



void stub_empty_0x1715e(void)

{
  return;
}



// [PROTOTYPE ADDED 2026-07-23] Multi-sensor ADC conversion loop, processes up
// to 24 (0x18) samples. Params inferred from disassembly stack offsets
// (SP+0x16/0x1a/0x1c/0x1e, after the function's own 7-register + EP save):
//   src_ptr   - raw 16-bit ADC sample source array
//   dst_ptr   - converted byte destination array
//   count     - sample count, clamped to 24 max
//   mode_flag - if == 1, writes only 1 output byte per sample instead of 2
//               (the second byte, always written as a 0 terminator/padding,
//               is skipped when mode_flag==1 -- exact meaning of the "2nd
//               byte" mode not independently confirmed against a caller)
// Per-sample: reads raw 16-bit value, looks it up against a bound at
// DAT_00013282 with a correction table at -0x530 if in range, clamps the
// result pointer to a minimum of 0xee80, reads 1 byte (high or low byte of
// a word depending on whether raw_value > 0xbf), writes it to dst_ptr, then
// always appends a 0 byte (unless mode_flag==1 skips the data byte).
// Params/locals did not fully substitute in the pretty-printed decompiler
// output (cosmetic quirk seen elsewhere in this ROM's stack-heavy functions)
// -- the underlying type/param model is correctly applied regardless.

void __stdcall_far_allstack
adc_sensor_convert_multi(uchar *src_ptr,uchar *dst_ptr,ushort count,ushort mode_flag)

{
  undefined1 *puVar1;
  ushort count_local;
  undefined1 *raw_value;
  undefined1 *table_result;
  undefined1 *src_cursor;
  undefined1 *dst_cursor;
  undefined1 converted_byte;
  bool loop_continue;
  undefined1 in_stack_00000010;
  
  count_local = CONCAT11((undefined1)count,mode_flag._0_1_);
  if (0x18 < count_local) {
    count_local = 0x18;
  }
  if (count_local != 0) {
    src_cursor = (undefined1 *)CONCAT11(src_ptr._3_1_,dst_ptr._0_1_);
    dst_cursor = (undefined1 *)CONCAT11(dst_ptr._3_1_,count._0_1_);
    do {
      puVar1 = src_cursor;
      table_result = src_cursor + 1;
      src_cursor = src_cursor + 2;
      raw_value = (undefined1 *)CONCAT11(*table_result,*puVar1);
      table_result = raw_value;
      if (raw_value <= DAT_00013282) {
        table_result = *(undefined1 **)((int)((short)raw_value << 1) + -0x530);
      }
      if (table_result < (undefined1 *)&DAT_0000ee80) {
        table_result = (undefined1 *)&DAT_0000ee80;
      }
      converted_byte = *table_result;
      if ((undefined1 *)0xbf < raw_value) {
        converted_byte = table_result[1];
      }
      if (CONCAT11((undefined1)mode_flag,in_stack_00000010) != 1) {
        puVar1 = dst_cursor;
        dst_cursor = dst_cursor + 1;
        *puVar1 = converted_byte;
      }
      puVar1 = dst_cursor;
      dst_cursor = dst_cursor + 1;
      *puVar1 = 0;
      loop_continue = count_local != 0xffff;
      count_local = count_local - 1;
    } while (loop_continue);
  }
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// CORE MUT MECHANISM (confirmed): MUT table lookup is one level of indirection.
// For RequestID N (param, passed as high byte / R1 after swap.b): bound-check against
// @0x3282; if in range, read 16-bit RAM address from ROM table @0x2fad0 + N*2 via
// EP=2 bank-prefixed access (base offset -0x530 from EP:2 segment - i.e. table lives
// at the EP:2-banked equivalent of 0x2fad0). Floor-clamp result to 0xEE80. Return the
// BYTE at that RAM address (high byte if RequestID <= 0xBF, per cmp:i #0xbf branch,
// otherwise low byte via swap.b first - i.e. selects hi/lo byte of a word depending on
// RequestID range).
// Table @0x2fad0 decoded and spot-verified this pass for RequestIDs 0x38 (MAP/Boost),
// 0x3A (AirTemp), 0x14 (Battery), 0x17 (TPS) - all match logging.txt's claimed table
// entries exactly. Table itself is data, not yet fully typed/labeled in Ghidra.
// 
// [PROTOTYPE PASS 2026-07-24] RequestID N arrives in R1 (high byte after swap.b), not
// on the stack -- same register-parameter limitation as adc_channel_convert_and_read/
// boot_ram_block_copy (see functions_w4a51.md). set_function_prototype/set_variable_storage
// can't retarget this on H8; left undeclared (undefined2(void)) rather than commit a
// misleading stack-based signature. Return is effectively a byte (CONCAT11 high byte is
// 0 when RequestID<0xC0), typed undefined2 for the same reason.
// 
// [CALLER AUDIT 2026-08-07, CORRECTED same session] All 13 call sites enumerated via
// get_function_callers -- exactly TWO callers, both internal periodic
// snapshot/backup-logging routines:
//   - mut_configurable_reqid_backup_snapshot_5word (0x1504b) -- 5 calls
//   - mut_configurable_reqid_periodic_snapshot_8word (0x14edd) -- 8 calls
// CORRECTION: an earlier pass this session claimed both callers pass FIXED,
// HARDCODED RequestID literals. WRONG -- re-verified via direct
// disassemble_function on both callers: every RequestID is read from a RAM
// table at 0xf814+channel*0x34, itself populated/validated at runtime by
// eeprom_backup_table_write_dispatch (0x149f5) from a checksummed
// EEPROM-style config blob. So the RequestID set is CONFIGURABLE at
// runtime, not compiled-in -- likely uploaded via the same SCI1 serial
// mechanism documented elsewhere in this project, making this closer to a
// real "MUT data-list" facility than first assessed, just indirected
// through an EEPROM-backed RAM staging record instead of taking the
// RequestID directly off the wire per-byte.
// Still true regardless of the correction: cross-checked against the live
// incoming command byte F534 (3 readers, all documented, none reach this
// function) -- so whatever configures the 0xf814 RAM table, it is NOT the
// same code path that processes the live per-byte SCI1 command stream;
// it's a separate, likely session/config-time upload mechanism. See
// mut_verification_status.md "MAJOR FINDING ... CORRECTED" (2026-08-07)
// for full detail and the NOT YET DONE list (tracing what populates
// 0xf862+, the real source of the config blob).

ushort adc_sensor_convert_single(void)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 *usRequestId;
  undefined1 *usTableAddr;
  undefined2 in_stack_00000005;
  
                    // [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
                    //    CORE MUT MECHANISM (confirmed): MUT table lookup is one level of
                    // indirection.
                    //    For RequestID N (param, passed as high byte / R1 after swap.b):
                    // bound-check against
                    //    @0x3282; if in range, read 16-bit RAM address from ROM table @0x2fad0 +
                    // N*2 via
                    //    EP=2 bank-prefixed access (base offset -0x530 from EP:2 segment - i.e.
                    // table lives
                    //    at the EP:2-banked equivalent of 0x2fad0). Floor-clamp result to 0xEE80.
                    // Return the
                    //    BYTE at that RAM address (high byte if RequestID <= 0xBF, per cmp:i #0xbf
                    // branch,
                    //    otherwise low byte via swap.b first - i.e. selects hi/lo byte of a word
                    // depending on
                    //    RequestID range).
                    //    Table @0x2fad0 decoded and spot-verified this pass for RequestIDs 0x38
                    // (MAP/Boost),
                    //    0x3A (AirTemp), 0x14 (Battery), 0x17 (TPS) - all match logging.txt's
                    // claimed table
                    //    entries exactly. Table itself is data, not yet fully typed/labeled in
                    // Ghidra.
                    //    
                    //    [PROTOTYPE PASS 2026-07-24] RequestID N arrives in R1 (high byte after
                    // swap.b), not
                    //    on the stack -- same register-parameter limitation as
                    // adc_channel_convert_and_read/
                    //    boot_ram_block_copy (see functions_w4a51.md).
                    // set_function_prototype/set_variable_storage
                    //    can't retarget this on H8; left undeclared (void params) rather than
                    // commit a
                    //    misleading stack-based signature.
                    // 
                    //    [2026-08-16] Return type tightened from undefined2 to ushort: the
                    // decompiled body
                    //    (CONCAT11(uVar2,uVar1), high byte forced to 0 for RequestID<0xC0) always
                    // produces a
                    //    zero-extended byte value -- ushort is the accurate, non-misleading type,
                    // verified via
                    //    force_decompile with no change to function body. Parameter-passing
                    // limitation above is
                    //    unaffected and still applies.
                    //    
                    //    [CALLER AUDIT 2026-08-07, CORRECTED same session] All 13 call sites
                    // enumerated via
                    //    get_function_callers -- exactly TWO callers, both internal periodic
                    //    snapshot/backup-logging routines:
                    //      - mut_configurable_reqid_backup_snapshot_5word (0x1504b) -- 5 calls
                    //      - mut_configurable_reqid_periodic_snapshot_8word (0x14edd) -- 8 calls
                    //    CORRECTION: an earlier pass this session claimed both callers pass FIXED,
                    //    HARDCODED RequestID literals. WRONG -- re-verified via direct
                    //    disassemble_function on both callers: every RequestID is read from a RAM
                    //    table at 0xf814+channel*0x34, itself populated/validated at runtime by
                    //    eeprom_backup_table_write_dispatch (0x149f5) from a checksummed
                    //    EEPROM-style config blob. So the RequestID set is CONFIGURABLE at
                    //    runtime, not compiled-in -- likely uploaded via the same SCI1 serial
                    //    mechanism documented elsewhere in this project, making this closer to a
                    //    real "MUT data-list" facility than first assessed, just indirected
                    //    through an EEPROM-backed RAM staging record instead of taking the
                    //    RequestID directly off the wire per-byte.
                    //    Still true regardless of the correction: cross-checked against the live
                    //    incoming command byte F534 (3 readers, all documented, none reach this
                    //    function) -- so whatever configures the 0xf814 RAM table, it is NOT the
                    //    same code path that processes the live per-byte SCI1 command stream;
                    //    it's a separate, likely session/config-time upload mechanism. See
                    //    mut_verification_status.md "MAJOR FINDING ... CORRECTED" (2026-08-07)
                    //    for full detail and the NOT YET DONE list (tracing what populates
                    //    0xf862+, the real source of the config blob).
  usRequestId = (undefined1 *)
                CONCAT11((char)in_stack_00000005,(char)((ushort)in_stack_00000005 >> 8));
  usTableAddr = usRequestId;
  if (usRequestId <= DAT_00013282) {
    usTableAddr = *(undefined1 **)((int)((short)usRequestId << 1) + -0x530);
  }
  if (usTableAddr < (undefined1 *)&DAT_0000ee80) {
    usTableAddr = (undefined1 *)&DAT_0000ee80;
  }
  uVar1 = *usTableAddr;
  if (usRequestId < (undefined1 *)0xc0) {
    uVar2 = uVar1;
    uVar1 = 0;
  }
  else {
    uVar2 = usTableAddr[1];
  }
  return CONCAT11(uVar2,uVar1);
}



char __stdcall_far_allstack byte_sum_banked(uchar bank,char *offset,short count)

{
  char sum;
  short count_local;
  bool loop_continue;
  char *byte_ptr;
  
  sum = '\0';
  count_local = CONCAT11(offset._3_1_,count._0_1_);
  if (count_local != 0) {
    do {
      byte_ptr = offset._1_2_;
      offset._1_2_ = offset._1_2_ + 1;
      sum = sum + *byte_ptr;
      loop_continue = count_local != -1;
      count_local = count_local + -1;
    } while (loop_continue);
  }
  return sum;
}



short __stdcall_far_allstack sum_byte_array(uchar bank,uchar *offset,short count)

{
  short sum;
  short count_local;
  bool loop_continue;
  byte *byte_ptr;
  
  sum = 0;
  count_local = CONCAT11(offset._3_1_,count._0_1_);
  if (count_local != 0) {
    do {
      byte_ptr = offset._1_2_;
      offset._1_2_ = offset._1_2_ + 1;
      sum = sum + (ushort)*byte_ptr;
      loop_continue = count_local != -1;
      count_local = count_local + -1;
    } while (loop_continue);
  }
  return sum;
}



void __stdcall_far_allstack
memcpy_banked(ushort src_bank,ushort src_offset,ushort dst_bank,ushort dst_offset,short count)

{
  undefined1 *puVar1;
  bool loop_continue;
  short in_stack_0000000e;
  undefined1 *dst_cursor;
  
  if (in_stack_0000000e != 0) {
    do {
      puVar1 = (undefined1 *)dst_bank;
      dst_bank = dst_bank + 1;
      dst_cursor = (undefined1 *)count;
      count = count + 1;
      *dst_cursor = *puVar1;
      loop_continue = in_stack_0000000e != -1;
      in_stack_0000000e = in_stack_0000000e + -1;
    } while (loop_continue);
  }
  return;
}



void stub_empty_0x17300(void)

{
  return;
}



void engine_state_f498_init(void)

{
  DAT_0001f498 = DAT_0001f498 | 0xc080;
  DAT_0001f49e = DAT_00010e22;
  DAT_0001f49c = DAT_00010e20;
  DAT_0001f49a = DAT_00010e20;
  DAT_0001f4ae = DAT_0001f4ae | 0xc0;
  DAT_0001f4ac = 0xff;
  DAT_0001efec = DAT_00010e46;
  output_relay_flags_f0e6 = output_relay_flags_f0e6 | 2;
  DAT_0001f4f0 = 1;
  DAT_0001f4ec = DAT_00010eec;
  DAT_0001f466 = 0xff;
  DAT_0001f094 = DAT_00010ed6;
  return;
}



void periodic_task_dispatcher(void)

{
  byte bVar1;
  
  bVar1 = 1;
  ign_advance_f0e6_bit7_update();
  isc_f0e6_bit0_update();
  isc_target_calc_and_store();
  if (*(char *)((uint)bVar1 << 0x10 | 0x2e3) != '\0') {
    f0ea_bit6_update_from_mode_check();
  }
  egr_f0e6_bit1_update();
  egr_target_f448_update();
  egr_mode_dispatch();
  purge_f0e6_bit2_update();
  ign_advance_top_update();
  ign_advance_load_correction_f450();
  rpm_map_zone_flags_f43e_update();
  f0ea_bit7_rpm_zone_update();
  f0e6_bit6_update();
  stub_empty_193f0();
  canister_purge_mode_dispatch();
  diagnostic_status_f4b4_build();
  o2_sensor_control_dispatch();
  o2_heater_fuel_ctrl_top_update();
  return;
}



// [TRACED 2026-07-22, CORRECTED same session] Writer for output_relay_flags_f0e6
// (0xf0e6) bit7, which drives P1DR.5 (0xfe82, see mirror_status_f0e6_to_ports
// @ 0x156ce), gated on ROM config byte 0x102e6==0 (same gate the read/mirror
// side uses).
// 
// Condition source: engine_mode_f20e_f510_check (0x174a1) returns true unless
// (f20e bit4 SET) AND (f510 bit1 CLEAR).
// 
// REACHABLE FROM SCI1 DISPATCHER: f510 bit1 (mask 0x0002) IS set by
// sci1_meta_cmd_dispatch_c0_ff (0x28869) command byte 0xD8, per
// cmd_c0_d8_actuator_bit_table (0x13740) entry index 24 = 00 ff 00 02
// (byte0=0 -> f510, mask=0x0002). Verified via direct read_memory at
// 0x137a0 (double-checked after an initial miscounted eyeball-read of the
// whole table wrongly said this bit wasn't present - re-verify table entries
// individually by address, not by eyeballing a long hex dump, if revisiting).
// 
// f20e itself is an extremely widely-used general engine-mode/state flag
// word (170+ xref sites across the whole ROM) -- not practical to fully trace
// in one session. Given the function's inherited name, the SCI1-dispatcher
// reachability via cmd 0xD8, and f20e bit4's likely role as a broader
// "engine running/ignition active" condition, P1DR.5 is a plausible candidate
// for an ignition-confirm/coil-driver-adjacent output, POSSIBLY the missing
// "Ignition Timing Fix (5deg BTDC)" MUT actuator concept from the EvoScan
// profile's Mode5 list (RequestID 0x17 there, though that ID itself was
// independently found unmapped in the DP=2 table - cmd 0xD8 here is a
// different, ROM-native command byte, not the EvoScan RequestID numbering).
// UNCONFIRMED - flagged as a lead, not a fact. See mut_verification_status.md
// "PHYSICAL OUTPUT PIN TRACE" section.

void ign_advance_f0e6_bit7_update(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (DAT_000102e6 == '\0') {
    sVar2 = (*engine_mode_f20e_f510_check)();
    if (sVar2 == 0) {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
      *puVar1 = *puVar1 & 0xff7f;
    }
    else {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
      *puVar1 = *puVar1 | 0x80;
    }
  }
  else {
    ign_advance_table_lookup();
  }
  return;
}



ushort ign_advance_table_lookup(void)

{
  ushort in_ram_0000113a;
  ushort in_ram_0000113c;
  ushort in_ram_0000113e;
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  
  if (g_tps_load_axis_f10e_prev < DAT_00011130) {
    uVar2 = DAT_00011134;
  }
  else if (g_tps_load_axis_f10e_prev < DAT_00011132) {
    uVar2 = DAT_00011136;
  }
  else {
    uVar2 = DAT_00011138;
  }
  uVar3 = 3;
  uVar1 = g_tps_load_axis_f10e_prev;
  if ((g_status_flags_f20e & 0x10) == 0) {
    uVar3 = 3;
    if (((((g_status_flags_f20e & 1) == 0) && ((g_status_flags_f1fe & 8) == 0)) &&
        (uVar1 = g_tps_load_axis_f10e, DAT_0001112e < g_tps_load_axis_f10e)) &&
       (uVar1 = (ushort)((uint)uVar2 * 0x14 >> 0x10), (ushort)((uint)uVar2 * 0x14) <= DAT_0001ef70))
    {
      uVar3 = (ushort)*(byte *)((int)(DAT_0001f1de >> 4) + 0x31b7);
      uVar2 = DAT_0000f13a;
      if (uVar2 < in_ram_0000113a) {
        uVar3 = uVar3 + 1;
      }
      uVar2 = DAT_0000f13a;
      if (uVar2 < in_ram_0000113c) {
        uVar3 = uVar3 + 1;
      }
      uVar1 = DAT_0000f13a;
      if (uVar1 < in_ram_0000113e) {
        uVar3 = uVar3 + 1;
      }
    }
  }
  else if ((gear_mode_flags_f510 & 2) == 0) {
    uVar3 = 0;
  }
  if (2 < uVar3) {
    uVar3 = 3;
  }
  if (*(byte *)((int)uVar3 + 0x31b7) < 10) {
    DAT_0000f442 = (ushort)*(byte *)((int)uVar3 + 0x31b7);
  }
  else {
    DAT_0000f442 = 10;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

bool engine_mode_f20e_f510_check(void)

{
  bool bVar1;
  
  if (((g_status_flags_f20e & 0x10) == 0) || ((gear_mode_flags_f510 & 2) != 0)) {
    bVar1 = true;
  }
  else {
    bVar1 = false;
  }
  return bVar1;
}



void isc_f0e6_bit0_update(void)

{
  ushort *puVar1;
  undefined1 extraout_var;
  bool bVar2;
  byte bVar3;
  
  bVar3 = 1;
  bVar2 = knock_condition_eval();
  if (CONCAT11(extraout_var,bVar2) == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
    *puVar1 = *puVar1 & 0xfffe;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
    *puVar1 = *puVar1 | 1;
  }
  return;
}



bool knock_condition_eval(void)

{
  ushort usFlags;
  bool bVar1;
  
  bVar1 = ((g_status_flags_f0fa | g_status_flags_f0f8) & 0x10) == 0;
  if (bVar1) {
    DAT_0001efe4 = DAT_00010dbc;
  }
  if (engine_torque_pct_f17a < DAT_00010dc2) {
    DAT_0001efe6 = DAT_00010dbe;
  }
  usFlags = g_rpm_zone_flags_f43e;
  if ((g_rpm_zone_flags_f43e & 4) == 0) {
    if (DAT_00010dd0 < g_f1e6_hysteresis_gate) {
      usFlags = g_rpm_zone_flags_f43e | 4;
    }
  }
  else if (g_f1e6_hysteresis_gate <= DAT_00010dd2) {
    usFlags = g_rpm_zone_flags_f43e & 0xfffb;
  }
  if ((usFlags & 2) == 0) {
    if (DAT_00010dcc < g_adc_ch7_raw_f13c) {
      usFlags = usFlags | 2;
    }
  }
  else if (g_adc_ch7_raw_f13c <= DAT_00010dce) {
    usFlags = usFlags & 0xfffd;
  }
  if (((DAT_000102ef == 0) || ((g_status_flags_f0f8 & 0x20) != 0)) ||
     (((DAT_000102ef & 4) != 0 && (DAT_00010dd6 < g_iat_axis_f118)))) {
    DAT_0001f044 = 0;
  }
  else if (((usFlags & 4) == 0) && ((usFlags & (usFlags ^ g_rpm_zone_flags_f43e) & 2) != 0)) {
    DAT_0001f044 = DAT_00010dd4;
  }
  if ((usFlags & 1) == 0) {
    if (DAT_00010dc4 < g_adc_ch7_raw_f13c) {
      usFlags = usFlags | 1;
    }
  }
  else if (g_adc_ch7_raw_f13c <= DAT_00010dc6) {
    usFlags = usFlags & 0xfffe;
  }
  if (((((DAT_000102ef & 8) == 0) && ((g_status_flags_f0f8 & 0x20) != 0)) || ((usFlags & 1) == 0))
     || (((DAT_000102ef & 2) == 0 && (DAT_0001f044 != 0)))) {
    DAT_0001f042 = 0;
  }
  else if ((g_rpm_zone_flags_f43e & 1) == 0) {
    DAT_0001f042 = DAT_00010dc0;
  }
  g_rpm_zone_flags_f43e = usFlags;
  if ((engine_torque_pct_f17a < DAT_00010dc2 || bVar1) ||
     (((g_warmup_state_f594 & 0x80) == 0 &&
      (((DAT_0001efe4 != 0 || (DAT_0001efe6 != 0)) || ((DAT_0001f042 != 0 || (DAT_0001f044 != 0)))))
      ))) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  return bVar1;
}



void isc_target_calc_and_store(void)

{
  ushort _unused_far_pad;
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  DAT_0001f45e = DAT_0001f45c;
  if ((g_status_flags_f1f2 & 0x1000) == 0) {
    DAT_0001f45c = 0;
  }
  else {
    uVar2 = 0x80;
    if (DAT_000102e2 != '\0') {
      uVar2 = 0xff;
    }
    isc_condition_eval();
    (*isc_f03e_f45a_timer_update)();
    _unused_far_pad = (*isc_stepper_mode_dispatch)();
    uVar1 = _unused_far_pad;
    if (((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 0x10) != 0) &&
       (((*(ushort *)((uint)bVar3 << 0x10 | 0xf510) & 4) != 0 ||
        ((*(ushort *)((uint)bVar3 << 0x10 | 0xf512) & 8) != 0)))) {
      uVar1 = uVar2;
    }
    uVar1 = o2_downstream_mode64_select(_unused_far_pad,uVar1);
    if (uVar1 < uVar2) {
      *(ushort *)((uint)bVar3 << 0x10 | 0xf45c) = uVar1;
    }
    else {
      *(ushort *)((uint)bVar3 << 0x10 | 0xf45c) = uVar2;
    }
  }
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf098) = 0;
  return;
}



void isc_condition_eval(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort *puVar1;
  ushort uVar2;
  short sVar3;
  ushort extraout_R2;
  byte bVar4;
  
  bVar4 = 1;
  if ((isc_state_flags_f45a & 8) == 0) {
    if (DAT_00010db2 < engine_torque_pct_f17a) {
      isc_state_flags_f45a = isc_state_flags_f45a | 8;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_00010db4) {
    isc_state_flags_f45a = isc_state_flags_f45a & 0xfff7;
  }
  if ((isc_state_flags_f45a & 2) == 0) {
    if (DAT_00010dac < inj_pw_output_f186) {
      isc_state_flags_f45a = isc_state_flags_f45a | 2;
    }
  }
  else if (inj_pw_output_f186 <= DAT_00010dae) {
    isc_state_flags_f45a = isc_state_flags_f45a & 0xfffd;
  }
  (*axis_lookup_interp)(inj_pw_output_f186,param_2,0,2,0xd5ee,param_3);
  uVar2 = table_lookup_interp(1,(void *)0x2de4);
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf45a) & 4) == 0) {
    if (uVar2 < *(ushort *)((uint)bVar4 << 0x10 | 0xf18c)) {
      puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf45a);
      *puVar1 = *puVar1 | 4;
    }
  }
  else {
    uVar2 = mul_u16_shr8_sat(uVar2,*(ushort *)((uint)bVar4 << 0x10 | 0xdb0));
    if (*(ushort *)((uint)bVar4 << 0x10 | 0xf18c) <= uVar2) {
      puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf45a);
      *puVar1 = *puVar1 & 0xfffb;
    }
  }
  puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf45a);
  *puVar1 = *puVar1 & 0xf7ff;
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 8) == 0) {
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeffe) = *(undefined2 *)((uint)bVar4 << 0x10 | 0x1108);
  }
  uVar2 = isc_stepper_enable_check();
  if (uVar2 == 0) {
    sVar3 = (*isc_decel_fuel_cut_check)();
    uVar2 = extraout_R2;
    if (sVar3 != 0) {
      uVar2 = 0x40;
    }
  }
  else {
    uVar2 = 0x20;
  }
  *(ushort *)((uint)bVar4 << 0x10 | 0xf45a) =
       uVar2 | *(ushort *)((uint)bVar4 << 0x10 | 0xf45a) & 0xff9f;
  if (((*(ushort *)((uint)bVar4 << 0x10 | 0xf45a) & 0x840) == 0x840) &&
     ((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x80) == 0)) {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf45a);
    *puVar1 = *puVar1 | 0x1000;
  }
  else {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf45a);
    *puVar1 = *puVar1 & 0xefff;
  }
  return;
}



// WARNING: Type propagation algorithm not settling
// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_stepper_enable_check(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  ushort extraout_R2;
  ushort extraout_R2_00;
  byte bVar4;
  
  bVar4 = 1;
  if (DAT_000102e2 == '\0') {
    if (DAT_000102df == '\x01') {
      if ((((g_status_flags_f1fe & 8) == 0) ||
          ((DAT_0001110a < engine_torque_pct_f17a && (DAT_0001110c < g_adc_ch7_raw_f13c)))) &&
         ((((g_status_flags_f1fe & 8) != 0 ||
           (((isc_state_flags_f45a & 4) != 0 && ((isc_state_flags_f45a & 2) == 0)))) &&
          (((g_status_flags_f20e & 8) == 0 || (DAT_0001effe != 0)))))) {
joined_r0x00017904:
        if (((((isc_state_flags_f45a & 8) != 0) && (DAT_00010daa <= g_rpm_zone_dwell_counter_ef6a))
            && ((g_status_flags_f20e & 0x11) == 0)) && (DAT_00010da4 <= g_tps_load_axis_f10e))
        goto LAB_00017929;
      }
    }
    else if (((((g_status_flags_f1fe & 8) == 0) || ((g_status_flags_f216 & 0x10) != 0)) &&
             (((g_status_flags_f1fe & 8) != 0 ||
              ((((isc_state_flags_f45a & 4) != 0 && ((isc_state_flags_f45a & 2) == 0)) &&
               ((DAT_000102e1 == '\0' || ((g_status_flags_f216 & 0xf) != 0)))))))) &&
            ((DAT_000102e0 == '\0' || ((g_status_flags_f20e & 8) == 0)))) goto joined_r0x00017904;
LAB_00017924:
    uVar3 = 1;
  }
  else {
    uVar3 = g_status_flags_f20e & 0x800;
    if (((((g_status_flags_f1fe & 8) != 0) &&
         ((engine_torque_pct_f17a <= DAT_0001110a || (g_adc_ch7_raw_f13c <= DAT_0001110c)))) ||
        (((g_status_flags_f1fe & 8) == 0 &&
         ((((g_status_flags_f216 & 0x80) == 0 || ((isc_state_flags_f45a & 4) == 0)) ||
          ((isc_state_flags_f45a & 2) != 0)))))) ||
       (((((g_status_flags_f20e & 8) != 0 ||
          (uVar2 = shift_left_byte_to_high(DAT_0001111e),
          *(ushort *)((uint)bVar4 << 0x10 | 0xf26e) <= uVar2)) ||
         (((uVar2 = extraout_R2, uVar3 != 0 &&
           (uVar3 = sat_sub_u16(*(ushort *)((uint)bVar4 << 0x10 | 0x8a0),
                                *(ushort *)((uint)bVar4 << 0x10 | 0x1126)),
           puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf06c), uVar2 = extraout_R2_00,
           *puVar1 <= uVar3 && uVar3 != *puVar1)) ||
          (((((*(ushort *)((uint)bVar4 << 0x10 | 0xf45a) & 8) == 0 ||
             (puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xef6a),
             *puVar1 <= uVar2 && uVar2 != *puVar1)) ||
            ((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x11) != 0)) ||
           (*(ushort *)((uint)bVar4 << 0x10 | 0xf10e) < *(ushort *)((uint)bVar4 << 0x10 | 0xda4)))))
         )) || ((*(char *)((uint)bVar4 << 0x10 | 0x30a) != '\0' &&
                (((((*(ushort *)((uint)bVar4 << 0x10 | 0xf1fe) & 8) == 0 &&
                   ((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x80) == 0)) &&
                  ((*(ushort *)((uint)bVar4 << 0x10 | 0xf45a) & 0x800) == 0)) ||
                 (((((*(ushort *)((uint)bVar4 << 0x10 | 0xf1f6) & 4) != 0 &&
                    ((*(ushort *)((uint)bVar4 << 0x10 | 0xf75a) & 8) != 0)) ||
                   (((*(ushort *)((uint)bVar4 << 0x10 | 0xf1f6) & 0x80) != 0 &&
                    ((*(ushort *)((uint)bVar4 << 0x10 | 0xf75a) & 4) != 0)))) ||
                  (((*(ushort *)((uint)bVar4 << 0x10 | 0xf1f6) & 0x2000) != 0 &&
                   ((*(ushort *)((uint)bVar4 << 0x10 | 0xf75a) & 0x10) != 0))))))))))))
    goto LAB_00017924;
LAB_00017929:
    uVar3 = 0;
  }
  return uVar3;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

bool isc_decel_fuel_cut_check(void)

{
  if ((((((DAT_0001030a == '\0') && ((g_status_flags_f20e & 0x80) == 0)) ||
        ((DAT_0001030a != '\0' &&
         (((g_status_flags_f20e & 0x80) == 0 && ((isc_state_flags_f45a & 0x800) != 0)))))) ||
       (coolant_temp_scaled_f130 < DAT_00010900)) || (DAT_000108fc <= g_iat_axis_f118)) ||
     ((DAT_0001030a != '\0' &&
      ((((DAT_0001f1f6 & 0x2000) != 0 && ((DAT_0001f75a & 0x20) != 0)) ||
       (((g_status_flags_f1fe & 8) != 0 &&
        ((DAT_0001110a < engine_torque_pct_f17a && (DAT_0001110c < g_adc_ch7_raw_f13c)))))))))) {
    return true;
  }
  return false;
}



ushort isc_f03e_f45a_timer_update(ushort usInputVal)

{
  if ((g_status_flags_f20e & 0x10) == 0) {
    if (DAT_0001f03e != 0) {
      return usInputVal;
    }
    if (((isc_state_flags_f45a & 0x80) == 0) || ((isc_state_flags_f45a & 0x20) != 0)) {
      if ((isc_state_flags_f45a & 0xc0) == 0) {
        DAT_0001f03e = (short)((uint)DAT_00010da8 * 0x50);
        isc_state_flags_f45a = isc_state_flags_f45a | 0x80;
        return (ushort)((uint)DAT_00010da8 * 0x50 >> 0x10);
      }
      return isc_state_flags_f45a & 0xc0;
    }
  }
  isc_state_flags_f45a = isc_state_flags_f45a & 0xff7f;
  DAT_0001f03e = (short)((uint)DAT_00010da6 * 0x50);
  return (ushort)((uint)DAT_00010da6 * 0x50 >> 0x10);
}



ushort isc_stepper_mode_dispatch(void)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  if (DAT_000102e2 != '\0') {
    if (DAT_0001030a == '\0') {
      isc_stepper_position_calc();
      isc_stepper_secondary_position_calc();
      (*isc_openloop_target_calc)();
    }
    else {
      (*isc_f462_integrator_update)();
      (*isc_integrator_update)();
      isc_stepper_output_calc();
    }
LAB_00017a26:
    uVar1 = (*isc_dashpot_override_check)();
    return uVar1;
  }
  if (DAT_000102df == '\x01') {
    isc_f462_position_ramp();
    (*isc_openloop_stepper_scale_calc)();
    goto LAB_00017a26;
  }
  bVar3 = 1;
  if ((DAT_000102df != '\x03') && (DAT_000102df != '\0')) {
    return 0;
  }
  if (((isc_state_flags_f45a & 0x40) == 0) && ((isc_state_flags_f45a & 0xa0) != 0)) {
    DAT_0001effc = 0;
    return 0;
  }
  if (DAT_0001effc != 0) {
    return DAT_0001f45c;
  }
  uVar1 = DAT_0001f45c;
  if (DAT_0001f45c == 0) {
    if (DAT_000102df == '\0') {
      uVar2 = 0x80;
      goto LAB_00017a7e;
    }
    uVar1 = 0;
    uVar2 = DAT_00010dba;
    if (DAT_00010dba != 0) goto LAB_00017a7e;
  }
  uVar2 = sat_add_u16(uVar1,DAT_00010db6);
LAB_00017a7e:
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeffc) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xdb8);
  return uVar2;
}



ushort isc_stepper_position_calc(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  bool bVar1;
  ushort uVar2;
  ushort b;
  byte bVar3;
  ushort uStack_4;
  
  bVar3 = 1;
  uStack_4 = g_isc_stepper_integrator_f462;
  if (((g_status_flags_f20e & 8) == 0) && ((g_status_flags_f216 & 0x80) != 0)) {
    bVar1 = false;
    DAT_0001f05e = DAT_00011114;
  }
  else {
    bVar1 = true;
  }
  uVar2 = g_status_flags_f20e & 0x11;
  if (uVar2 == 0) {
    if (bVar1) {
      if (DAT_0001f05e != 0) goto LAB_00017ba8;
    }
    else {
      if ((((DAT_0001f1f6 & 0x2000) != 0) && ((DAT_0001f75a & 0x20) != 0)) &&
         (g_isc_stepper_integrator_f462 == 0)) {
        uVar2 = sat_mul_u16(DAT_0001144e,0x100);
        uVar2 = div_u16_sat(uVar2,param_3);
        uStack_4 = uVar2;
        goto LAB_00017ba8;
      }
      if ((((DAT_0001f1f6 & 0x2000) == 0) || ((DAT_0001f75a & 0x10) == 0)) ||
         (uVar2 = DAT_0001f45c, DAT_0001144e < DAT_0001f45c)) {
        if ((DAT_0001f208 & 8) != 0) {
          if ((DAT_0001f75a & 0x30) == 0) {
            if ((g_status_flags_f0f8 & 0x80) == 0) {
              uVar2 = DAT_00011102;
              b = DAT_00011102;
            }
            else {
              uVar2 = DAT_00011106;
              b = DAT_00011104;
            }
          }
          else {
            uVar2 = DAT_00011234;
            b = DAT_00011236;
          }
          if ((DAT_0001f75a & 0x20) == 0) {
            if ((((DAT_0001f75a & 0x10) == 0) && ((DAT_0001f75a & 3) == 0)) &&
               (((isc_state_flags_f45a & 0xa0) == 0 || ((isc_state_flags_f45a & 0x40) != 0)))) {
              bVar1 = true;
            }
            else {
              bVar1 = false;
            }
            if (!bVar1) {
              uVar2 = sat_sub_u16(g_isc_stepper_integrator_f462,uVar2);
              uStack_4 = uVar2;
              goto LAB_00017ba8;
            }
          }
          uVar2 = sat_add_u16(g_isc_stepper_integrator_f462,b);
          uStack_4 = uVar2;
        }
        goto LAB_00017ba8;
      }
    }
  }
  uStack_4 = 0;
LAB_00017ba8:
  if (uStack_4 < 0xff) {
    *(ushort *)((uint)bVar3 << 0x10 | 0xf462) = uStack_4;
    uVar2 = uStack_4;
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf462) = 0xff;
  }
  return uVar2;
}



ushort isc_stepper_secondary_position_calc(void)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((g_status_flags_f20e & 0x11) != 0) {
    isc_state_flags_f45a = isc_state_flags_f45a & 0xfdff;
  }
  if (((((DAT_0001f210 | g_status_flags_f20e) & 0x800) == 0) || (g_isc_stepper_integrator_f462 == 0)
      ) || (0xfe < g_isc_stepper_integrator_f462)) {
    DAT_0001f092 = DAT_0001112c;
  }
  else if (DAT_0001f092 == 0) {
    isc_state_flags_f45a = isc_state_flags_f45a | 0x200;
  }
  uVar1 = g_status_flags_f20e & 0x11;
  if (uVar1 == 0) {
    uVar1 = DAT_0001f75a & 0x30;
    uVar2 = DAT_0001f464;
    if (((uVar1 == 0) && ((isc_state_flags_f45a & 0x200) == 0)) &&
       ((isc_state_flags_f45a & 0x40) == 0)) {
      uVar1 = isc_state_flags_f45a & 0xe0;
      if ((uVar1 == 0) && ((DAT_0001f208 & 4) != 0)) {
        uVar1 = sat_add_u16(DAT_0001f464,DAT_00011128);
        uVar2 = uVar1;
      }
    }
    else if ((DAT_0001f208 & 4) != 0) {
      uVar1 = sat_sub_u16(DAT_0001f464,DAT_00011128);
      uVar2 = uVar1;
    }
  }
  else {
    uVar2 = 0xff;
  }
  if (uVar2 < 0xff) {
    *(ushort *)((uint)bVar3 << 0x10 | 0xf464) = uVar2;
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf464) = 0xff;
  }
  return uVar1;
}



ushort isc_openloop_target_calc(void)

{
  undefined2 uVar1;
  ushort uVar2;
  ushort uVar3;
  undefined2 extraout_R1;
  byte bVar4;
  ushort unaff_retaddr;
  void *table;
  
  bVar4 = 1;
  if ((isc_state_flags_f45a & 0x400) == 0) {
    if (DAT_0001110e < DAT_0001f1de) {
      isc_state_flags_f45a = isc_state_flags_f45a | 0x400;
    }
  }
  else if (DAT_0001f1de <= DAT_00011110) {
    isc_state_flags_f45a = isc_state_flags_f45a & 0xfbff;
  }
  if (((g_status_flags_f1fe & 8) == 0) && ((isc_state_flags_f45a & 0x400) != 0)) {
    isc_state_flags_f45a = isc_state_flags_f45a | 0x100;
  }
  else {
    isc_state_flags_f45a = isc_state_flags_f45a & 0xfeff;
  }
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x319c);
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf45a) & 0x100) == 0) {
    (*axis_lookup_interp)(uVar1,extraout_R1,uVar1,2,0xd784);
    (*axis_lookup_interp)();
    table = (void *)0x31a8;
  }
  else {
    (*axis_lookup_interp)();
    (*axis_lookup_interp)();
    table = (void *)0x31a0;
  }
  uVar2 = table_lookup_interp(1,table);
  uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0x1116);
  uVar2 = sat_sub_u16(uVar2,*(ushort *)((uint)bVar4 << 0x10 | 0x1116));
  uVar3 = mul_u16_shr8_sat(uVar2,uVar3);
  uVar2 = sat_add_u16(uVar3,unaff_retaddr);
  uVar3 = (*weighted_blend_u8frac)();
  if (uVar2 <= uVar3) {
    uVar3 = uVar2;
  }
  if (uVar3 < 0xff) {
    *(ushort *)((uint)bVar4 << 0x10 | 0xf48e) = uVar3;
  }
  else {
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xf48e) = 0xff;
  }
  if ((((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 8) == 0) &&
      ((*(ushort *)((uint)bVar4 << 0x10 | 0xf216) & 0x80) != 0)) &&
     (((*(char *)((uint)bVar4 << 0x10 | 0x2cb) == '\0' ||
       (*(char *)((uint)bVar4 << 0x10 | 0x31b) == '\0')) ||
      (((*(ushort *)((uint)bVar4 << 0x10 | 0xf25e) & 0x40) == 0 &&
       (*(short *)((uint)bVar4 << 0x10 | 0xefa6) == 0)))))) {
    uVar3 = (*muldiv_s16_rounded)();
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}



void isc_integrator_update(void)

{
  ushort *puVar1;
  short *psVar2;
  undefined2 uVar3;
  ushort uVar4;
  ushort a_hi;
  ushort a_lo;
  undefined2 extraout_R1;
  undefined2 extraout_R2;
  undefined2 extraout_R2_00;
  ushort a;
  byte bVar5;
  ushort in_stack_00000004;
  
  bVar5 = 1;
  if ((DAT_0001f064 == 0) && ((DAT_0001146a < DAT_0001f1c8 || (DAT_0001146c < DAT_0001f1cc)))) {
    DAT_0001f064 = DAT_00011468;
  }
  if ((((g_status_flags_f20e & 0x11) != 0) || (DAT_0001f096 == 0)) ||
     ((isc_state_flags_f45a & 0x1000) != 0)) {
    DAT_0001f478 = 0;
    DAT_0001f47a = 0;
    DAT_0001f476 = 0;
    DAT_0001f474 = 0;
    DAT_0001f472 = 0;
    g_isc_integrator_f468 = 0;
    DAT_0001f47e = 0x80;
    DAT_0001f47c = 0x80;
    DAT_0001f482 = 0x80;
    DAT_0001f486 = 0x80;
    DAT_0001f480 = 0x8080;
    DAT_0001f484 = DAT_000113f6;
    DAT_0001f48c = DAT_0001f48c & 0x3fff;
  }
  if ((isc_state_flags_f45a & 0x400) == 0) {
    if (DAT_0001110e < DAT_0001f1de) {
      isc_state_flags_f45a = isc_state_flags_f45a | 0x400;
    }
  }
  else if (DAT_0001f1de <= DAT_00011110) {
    isc_state_flags_f45a = isc_state_flags_f45a & 0xfbff;
  }
  if (((g_status_flags_f1fe & 8) == 0) && ((isc_state_flags_f45a & 0x400) != 0)) {
    isc_state_flags_f45a = isc_state_flags_f45a | 0x100;
  }
  else {
    isc_state_flags_f45a = isc_state_flags_f45a & 0xfeff;
  }
  if ((isc_state_flags_f45a & 0x100) == 0) {
    (*axis_lookup_interp)();
    (*axis_lookup_interp)();
    uVar3 = table_lookup_interp(1,(void *)0x31a8);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf46e) = uVar3;
    uVar3 = table_lookup_interp(1,(void *)0x31a8);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf46c) = uVar3;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf470) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x13f8);
  }
  else {
    (*axis_lookup_interp)();
    (*axis_lookup_interp)();
    uVar3 = table_lookup_interp(1,(void *)0x31b0);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf46e) = uVar3;
    uVar3 = table_lookup_interp(1,(void *)0x31b0);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf46c) = uVar3;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf470) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x13fa);
  }
  uVar4 = (*mul_u16_x2_sat_alt)();
  if (uVar4 < *(ushort *)((uint)bVar5 << 0x10 | 0xf46c)) {
    uVar3 = (*mul_u16_x2_sat_alt)();
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf46a) = uVar3;
    uVar3 = extraout_R2_00;
  }
  else {
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf46a) = *(undefined2 *)((uint)bVar5 << 0x10 | 0xf46c);
    uVar3 = extraout_R2;
  }
  if ((((*(ushort *)((uint)bVar5 << 0x10 | 0xf1fe) & 8) == 0) &&
      (*(short *)((uint)bVar5 << 0x10 | 0xf45c) != 0)) &&
     ((*(ushort *)((uint)bVar5 << 0x10 | 0xf45a) & 0x1000) == 0)) {
    if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf208) & 2) != 0) {
      if (*(short *)((uint)bVar5 << 0x10 | 0xf484) == 0) {
        if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf48c) & 0x2000) == 0) {
          uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0x13f6);
        }
        else {
          uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0x1466);
        }
        uVar4 = div_u32_u16_sat(*(ushort *)((uint)bVar5 << 0x10 | 0xf478),
                                *(ushort *)((uint)bVar5 << 0x10 | 0xf47a),uVar4);
        *(ushort *)((uint)bVar5 << 0x10 | 0xf474) = uVar4;
        if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf48c) & 0x4000) == 0) {
          *(undefined2 *)((uint)bVar5 << 0x10 | 0xf476) =
               *(undefined2 *)((uint)bVar5 << 0x10 | 0xf474);
          puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf48c);
          *puVar1 = *puVar1 | 0x4000;
        }
        else {
          uVar3 = (*axis_interp_lerp_u8frac_256)();
          *(undefined2 *)((uint)bVar5 << 0x10 | 0xf476) = uVar3;
        }
        if (*(short *)((uint)bVar5 << 0x10 | 0xf064) == 0) {
          *(undefined2 *)((uint)bVar5 << 0x10 | 0xf484) =
               *(undefined2 *)((uint)bVar5 << 0x10 | 0x13f6);
          puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf48c);
          *puVar1 = *puVar1 & 0xdfff;
        }
        else {
          *(undefined2 *)((uint)bVar5 << 0x10 | 0xf484) =
               *(undefined2 *)((uint)bVar5 << 0x10 | 0x1466);
          puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf48c);
          *puVar1 = *puVar1 | 0x2000;
        }
        *(undefined2 *)((uint)bVar5 << 0x10 | 0xf478) = 0;
        *(undefined2 *)((uint)bVar5 << 0x10 | 0xf47a) = 0;
        puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf48c);
        *puVar1 = *puVar1 | 0x8000;
      }
      else {
        psVar2 = (short *)((uint)bVar5 << 0x10 | 0xf484);
        *psVar2 = *psVar2 + -1;
        puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf48c);
        *puVar1 = *puVar1 & 0x7fff;
      }
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf47e) = *(undefined2 *)((uint)bVar5 << 0x10 | 0xf47c);
      muldiv_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xf476),*(ushort *)((uint)bVar5 << 0x10 | 0xf46a)
                 ,0x200);
      uVar4 = div_u16_sat(*(ushort *)((uint)bVar5 << 0x10 | 0xf46a),4);
      sat_add_u16(a,uVar4);
      uVar4 = (*clamp_u8)();
      uVar4 = sat_sub_u16(uVar4,in_stack_00000004);
      *(ushort *)((uint)bVar5 << 0x10 | 0xf47c) = uVar4;
      uVar3 = (*axis_interp_lerp_u8frac_256)();
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf480) = uVar3;
      uVar4 = get_high_byte(*(ushort *)((uint)bVar5 << 0x10 | 0xf480));
      *(ushort *)((uint)bVar5 << 0x10 | 0xf482) = uVar4;
      if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf48c) & 0x8000) != 0) {
        uVar3 = (*isc_stepper_blend_from_struct)();
        *(undefined2 *)((uint)bVar5 << 0x10 | 0xf488) = uVar3;
      }
      if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf48c) & 0x8000) == 0) {
        uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xf756);
      }
      else {
        uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xf488);
      }
      uVar4 = get_high_byte(uVar4);
      *(ushort *)((uint)bVar5 << 0x10 | 0xf486) = uVar4;
      if (*(short *)((uint)bVar5 << 0x10 | 0xf46a) == 0) {
        *(undefined2 *)((uint)bVar5 << 0x10 | 0xf472) = 0;
      }
      else {
        sat_add_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xf482),0x180);
        sat_add_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xf486),0x180);
        (*mul_u16_hi)();
        sat_add_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xf46a),0x800);
        uVar4 = (*mul_u16_hi)();
        a_hi = (*sat_sub_u16_clamp0)();
        uVar4 = div_u32_u16_sat(a_hi,a_lo,uVar4);
        *(ushort *)((uint)bVar5 << 0x10 | 0xf472) = uVar4;
      }
      uVar3 = (*sat_add_s16)();
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf478) = uVar3;
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf47a) = extraout_R1;
    }
    uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xf47c);
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0x1412);
    if ((uVar4 < *puVar1 || uVar4 == *puVar1) &&
       (*(ushort *)((uint)bVar5 << 0x10 | 0x1408) <= *(ushort *)((uint)bVar5 << 0x10 | 0xf468))) {
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf468) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x1408);
    }
    if ((((*(ushort *)((uint)bVar5 << 0x10 | 0xf208) & 0x10) != 0) &&
        (*(ushort *)((uint)bVar5 << 0x10 | 0xf46a) < *(ushort *)((uint)bVar5 << 0x10 | 0xf46c))) &&
       (*(ushort *)((uint)bVar5 << 0x10 | 0x1414) <= *(ushort *)((uint)bVar5 << 0x10 | 0xf47c))) {
      uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xf468);
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0x140e);
      if (uVar4 < *puVar1 || uVar4 == *puVar1) {
        *(short *)((uint)bVar5 << 0x10 | 0xf468) =
             *(short *)((uint)bVar5 << 0x10 | 0xf468) + *(short *)((uint)bVar5 << 0x10 | 0x1400);
      }
      else {
        *(short *)((uint)bVar5 << 0x10 | 0xf468) =
             *(short *)((uint)bVar5 << 0x10 | 0xf468) + *(short *)((uint)bVar5 << 0x10 | 0x140c);
      }
    }
    if (*(ushort *)((uint)bVar5 << 0x10 | 0x1410) <= *(ushort *)((uint)bVar5 << 0x10 | 0xf468)) {
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf468) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x1410);
    }
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf096) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x1402);
  }
  else {
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf480) = 0x8080;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf47e) = uVar3;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf47c) = uVar3;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf482) = uVar3;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf484) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x13f6);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf478) = 0;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf47a) = 0;
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf48c);
    *puVar1 = *puVar1 & 0x7fff;
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf48c);
    *puVar1 = *puVar1 & 0xdfff;
  }
  return;
}



// Also a consumer of TABLE_STATUS_WORD_PTRS_137A8 (passed by caller isc_integrator_update at
// 0x180ab): reads table entries at fixed offsets +0x30, +0x40, +0x42. Confirms this is a
// widely-shared table, not local to the knock/octane cluster.

void isc_stepper_blend_from_struct(ushort param_fp1)

{
  ushort uVar1;
  ushort value;
  byte bVar2;
  ushort unaff_retaddr;
  undefined2 in_stack_00000004;
  ushort in_stack_0000fffc;
  
  bVar2 = 1;
  if (DAT_0001f47c != 0) {
    if (DAT_0001f47e == 0) {
      sat_add_u16(0x180,DAT_0001f47c);
      uVar1 = div_u16_sat(**(ushort **)((int)in_stack_00000004 + 0x30),0x100);
      sat_add_u16(0x180,uVar1);
      goto LAB_000182a9;
    }
    sat_add_u16(0x180,DAT_0001f47c);
  }
  uVar1 = div_u16_sat(**(ushort **)((int)in_stack_00000004 + 0x30),0x100);
  sat_add_u16(0x180,uVar1);
  sat_add_u16(0x180,*(ushort *)((uint)bVar2 << 0x10 | 0xf47e));
LAB_000182a9:
  uVar1 = (*muldiv_s16_rounded)();
  sat_sub_u16(uVar1,in_stack_0000fffc);
  uVar1 = **(ushort **)((int)in_stack_00000004 + 0x40);
  value = sat_mul_u16(**(ushort **)((int)in_stack_00000004 + 0x42),0x100);
  clamp_u16(value,uVar1,unaff_retaddr);
  return;
}



ushort isc_stepper_output_calc(void)

{
  ushort uVar1;
  byte bVar2;
  ushort unaff_retaddr;
  
  bVar2 = 1;
  if ((isc_state_flags_f45a & 0x1000) == 0) {
    uVar1 = g_isc_integrator_f468;
  }
  else {
    uVar1 = DAT_000114b8;
  }
  if (g_isc_stepper_integrator_f462 == 0) {
    uVar1 = 0;
  }
  else {
    sat_mul_u16(uVar1,2);
    (*muldiv_s16_rounded)();
    sat_sub_u16(*(ushort *)((uint)bVar2 << 0x10 | 0xf46e),*(ushort *)((uint)bVar2 << 0x10 | 0xf470))
    ;
    uVar1 = (*muldiv_s16_rounded)();
    uVar1 = sat_add_u16(uVar1,unaff_retaddr);
  }
  return uVar1;
}



ushort isc_f462_integrator_update(void)

{
  bool bVar1;
  ushort uVar2;
  ushort b;
  byte bVar3;
  ushort uStack_4;
  
  bVar3 = 1;
  uStack_4 = g_isc_stepper_integrator_f462;
  if (((g_status_flags_f20e & 8) == 0) && ((g_status_flags_f216 & 0x80) != 0)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  DAT_0001f05e = 0;
  uVar2 = g_status_flags_f20e & 0x11;
  if ((uVar2 != 0) || (bVar1)) {
    uStack_4 = 0;
  }
  else if ((DAT_0001f208 & 4) != 0) {
    if ((DAT_0001f75a & 0x30) == 0) {
      if ((g_status_flags_f0f8 & 0x80) == 0) {
        uVar2 = DAT_00011102;
        b = DAT_00011102;
      }
      else {
        uVar2 = DAT_00011106;
        b = DAT_00011104;
      }
    }
    else {
      uVar2 = DAT_00011234;
      b = DAT_00011236;
    }
    if ((DAT_0001f75a & 0x20) == 0) {
      if ((((DAT_0001f75a & 0x10) == 0) && ((DAT_0001f75a & 3) == 0)) &&
         (((isc_state_flags_f45a & 0xa0) == 0 || ((isc_state_flags_f45a & 0x40) != 0)))) {
        bVar1 = true;
      }
      else {
        bVar1 = false;
      }
      if (!bVar1) {
        uVar2 = sat_sub_u16(g_isc_stepper_integrator_f462,uVar2);
        uStack_4 = uVar2;
        goto LAB_00018411;
      }
    }
    uVar2 = sat_add_u16(g_isc_stepper_integrator_f462,b);
    uStack_4 = uVar2;
  }
LAB_00018411:
  if (uStack_4 < 0xff) {
    *(ushort *)((uint)bVar3 << 0x10 | 0xf462) = uStack_4;
    uVar2 = uStack_4;
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf462) = 0xff;
  }
  return uVar2;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- decompiler shows an uninitialized/ambiguous
// in_stack_00000004 read on one branch path rather than a clean local or param. Needs
// manual disassembly trace before committing a prototype; left as default undefined2(void)
// to avoid encoding a guess.

undefined2 __stdcall_far isc_dashpot_override_check(undefined2 _unused_far_pad,undefined2 param_2)

{
  if ((g_status_flags_f1fe & 8) == 0) {
  }
  else if ((isc_state_flags_f45a & 0x40) == 0) {
    param_2 = 0;
  }
  else {
    param_2 = DAT_00011100;
  }
  return param_2;
}



ushort isc_f462_position_ramp(void)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar1 = g_status_flags_f20e & 0x11;
  if ((uVar1 == 0) && ((isc_state_flags_f45a & 0x20) == 0)) {
    uVar2 = g_isc_stepper_integrator_f462;
    if ((DAT_0001f208 & 4) != 0) {
      if (((DAT_0001f75a & 3) == 0) &&
         (((isc_state_flags_f45a & 0xa0) == 0 || ((isc_state_flags_f45a & 0x40) != 0)))) {
        uVar1 = sat_add_u16(g_isc_stepper_integrator_f462,DAT_00011102);
        uVar2 = uVar1;
      }
      else {
        uVar1 = sat_sub_u16(g_isc_stepper_integrator_f462,DAT_00011102);
        uVar2 = uVar1;
      }
    }
  }
  else {
    uVar2 = 0;
  }
  if (uVar2 < 0xff) {
    *(ushort *)((uint)bVar3 << 0x10 | 0xf462) = uVar2;
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf462) = 0xff;
  }
  return uVar1;
}



void isc_openloop_stepper_scale_calc(void)

{
  (*axis_lookup_interp)();
  (*axis_lookup_interp)();
  table_lookup_interp(1,(void *)0x31a0);
  (*muldiv_s16_rounded)();
  return;
}



void isc_activation_control_f466(void)

{
  ushort *puVar1;
  short sVar2;
  short extraout_R2;
  ushort uVar3;
  short extraout_R2_00;
  ushort uVar4;
  byte bVar5;
  
  bVar5 = 1;
  (*f9a0_read_clear_bit4_atomic)();
  if ((((*(ushort *)((uint)bVar5 << 0x10 | 0xf75c) & 1) == 0) &&
      ((*(ushort *)((uint)bVar5 << 0x10 | 0xf510) & 4) == 0)) &&
     ((*(ushort *)((uint)bVar5 << 0x10 | 0xf512) & 8) == 0)) {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf0ea);
    *puVar1 = *puVar1 & 0x7fff;
    (*update_flag_f9a0_bit6_cond)();
    if (extraout_R2 != 0) {
      uVar4 = 2;
      if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf45a) & 0x100) != 0) {
        uVar4 = 1;
      }
      sVar2 = *(short *)((uint)bVar5 << 0x10 | 0xf466);
      uVar3 = sVar2 + 1;
      if (sVar2 == -1) {
        uVar3 = 0xffff;
      }
      if (uVar4 <= uVar3) {
        uVar3 = 0;
        *(undefined2 *)((uint)bVar5 << 0x10 | 0xf460) =
             *(undefined2 *)((uint)bVar5 << 0x10 | 0xf45c);
      }
      *(ushort *)((uint)bVar5 << 0x10 | 0xf466) = uVar3;
      uVar4 = muldiv_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xf460),0x100,0xff);
      uVar4 = sat_sub_u16(uVar4,extraout_R2_00 << 8);
      if (0xff < uVar4) {
        uVar4 = 0x100;
      }
      init_scale_factor_f9ac(uVar4);
    }
  }
  else {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf0ea);
    *puVar1 = *puVar1 | 0x8000;
    (*update_flag_f9a0_bit6_cond)();
    init_scale_factor_f9ac(0);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf466) = 0xff;
  }
  return;
}



void f0ea_bit6_update_from_mode_check(void)

{
  ushort uVar1;
  undefined1 extraout_var;
  bool bVar2;
  
  bVar2 = engine_mode_f512_bit4_check();
  if (CONCAT11(extraout_var,bVar2) == 0) {
    uVar1 = g_status_flags_f0ea;
    g_status_flags_f0ea = uVar1 & 0xffbf;
  }
  else {
    uVar1 = g_status_flags_f0ea;
    g_status_flags_f0ea = uVar1 | 0x40;
  }
  return;
}



bool engine_mode_f512_bit4_check(void)

{
  bool bVar1;
  
  if (((g_status_flags_f20e & 0x10) == 0) || ((DAT_0001f512 & 0x10) == 0)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  return bVar1;
}



// [TRACED 2026-07-22] Writer for output_relay_flags_f0e6 (0xf0e6) bit1, which
// drives P2DR bit7 (0xfe83, real physical pin -- see
// mirror_status_f0e6_to_ports @ 0x156ce), gated on the SAME ROM config byte
// (0x102ee != 0) as the port-mirror side.
// 
// CONFIDENCE: HIGH -- this is a coherent, self-consistent EGR solenoid
// finding, not just a name-based guess:
// - Function name (inherited, but consistent with behavior): EGR-related.
// - Gate condition (egr_activity_condition_check @ 0x18628): enabled unless
//   RPM candidate ef90==0 (engine not running) OR (RPM > threshold
//   ef6e AND decaying counter efe8 != 0) -- i.e. disabled at very low or very
//   high RPM outside a mid-range activity window, matching how a real EGR
//   solenoid is gated in practice (no EGR at idle/high-load).
// - efe8 itself is refreshed from a ROM calibration constant (0x10e3e) when
//   RAM 0xf0f8 bit8 is clear -- consistent with a coolant-temp or warmup-gated
//   refresh (f0f8 is a broad state-flag word, not fully traced, but the
//   pattern fits a "don't run EGR until warmed up" style gate).
// - Both this writer AND the port-mirror reader use the identical ROM gate
//   byte (0x102ee), a strong internal-consistency signal that this is a real,
//   coherently-designed hardware output, not coincidental bit reuse.
// 
// IMPORTANT: this writer is DRIVEN PURELY BY INTERNAL ENGINE STATE (RPM/
// warmup), NOT by sci1_meta_cmd_dispatch_c0_ff. Checked: cmd_c0_d8_actuator_
// bit_table's f512-bit1 entry is cmd 0xC8 (00 01 ff 00 02 at 0x13760,
// confirmed via read_memory), which sets f512 bit1 -- a COMPLETELY SEPARATE
// RAM cell from f0e6 bit1 written here. This EGR output is normal automatic
// ECU control, NOT reachable/forceable via any known SCI1 diagnostic command
// on this ROM. If a real MUT/EvoScan "EGR Solenoid" actuator test exists for
// this vehicle, it does not appear to route through this mechanism -- either
// it's genuinely absent on this ROM (consistent with the earlier DP=2 table
// finding that Mode5 RequestID 0x02 "EGR Solenoid" maps to nothing), or it
// uses a mechanism not yet found.
// 
// This is currently the single BEST-EVIDENCED real actuator identification in
// the whole f0e6/port-pin trace -- unlike the P1DR.5 (0xD8/f510 bit1) lead,
// which relies on an inference from a widely-shared f20e flag, this one has
// a clean, EGR-specific, self-contained gating chain. STILL NOT independently
// confirmed against real vehicle hardware (no live/bench test performed) --
// treat as "well-evidenced hypothesis: P2DR.7 = EGR solenoid control", not
// as an absolutely proven fact. See mut_verification_status.md "PHYSICAL
// OUTPUT PIN TRACE" section.

void egr_f0e6_bit1_update(void)

{
  ushort *puVar1;
  undefined1 extraout_var;
  bool bVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((g_status_flags_f0f8 & 0x100) == 0) {
    g_egr_activity_decay_counter = g_egr_activity_decay_reload_cal;
  }
  if ((g_egr_enable_config_flag == '\0') ||
     (bVar2 = egr_activity_condition_check(), CONCAT11(extraout_var,bVar2) == 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
    *puVar1 = *puVar1 & 0xfffd;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
  *puVar1 = *puVar1 | 2;
  return;
}



bool egr_activity_condition_check(void)

{
  bool bVar1;
  
  if ((DAT_0001ef90 == 0) ||
     (((ushort)(g_rpm_threshold_scale_cal * 0x14) < DAT_0001ef6e &&
      (g_egr_activity_decay_counter != 0)))) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  return bVar1;
}



// [NEW 2026-07-15] egr_target_f448_update
// 
// ACTUATOR function, not a MUT RequestID target (F448 not present in the
// 0x2fad0 MUT table). Top-level EGR target dispatcher - performs the actual
// store to F448.
// 
// Logic: forces F448=0x80 (fixed override) when byte@0x2de is set AND both
// F20E bit4 and F510 bit4 are set (an EGR-disable/override condition pair -
// F510 also appears in the SCI1 command-dispatcher bit-flag family
// documented in review.md item 7, worth checking for a relationship).
// Otherwise calls a precondition check (0x18689); if that returns nonzero,
// forces F448=0 (EGR off). If preconditions pass, calls
// egr_duty_target_calc (0x186fa) for the computed target, passes it through
// 0x1a26e (purpose not yet traced), and stores the result to F448.
// 
// F448 = EGR actuator duty/position target.

void egr_target_f448_update(void)

{
  short sVar1;
  ushort uVar2;
  ushort extraout_R2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = 0;
  if (DAT_000102de != '\0') {
    if (((g_status_flags_f20e & 0x10) == 0) || ((gear_mode_flags_f510 & 0x10) == 0)) {
      sVar1 = (*egr_coolant_rpm_enable_check)();
      uVar2 = extraout_R2;
      if (sVar1 == 0) {
        egr_duty_target_calc();
        uVar2 = o2_downstream_correction_apply();
      }
    }
    else {
      uVar2 = 0x80;
    }
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf448) = uVar2;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

bool egr_coolant_rpm_enable_check(void)

{
  ushort usRpmThreshold;
  
  if ((g_status_flags_f1f2 & 0x800) == 0) {
    if ((ign_advance_enable_flags_f440 & 0x8000) == 0) {
      if (DAT_00010ec2 < g_tps_load_axis_f10e) {
        ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 | 0x8000;
      }
    }
    else if (g_tps_load_axis_f10e <= WORD_00010ec4) {
      ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 & 0x7fff;
    }
    if ((((g_status_flags_f20e & 0x11) == 0) && ((g_status_flags_f1fe & 8) == 0)) &&
       (DAT_00010ec0 < g_iat_axis_f118)) {
      return false;
    }
  }
  else {
    usRpmThreshold = DAT_00010ebc;
    if (DAT_0001f448 == 0) {
      usRpmThreshold = DAT_00010ebe;
    }
    if (((g_status_flags_f20e & 0x11) == 0) && (g_tps_load_axis_f10e < usRpmThreshold)) {
      return false;
    }
  }
  return true;
}



// [NEW 2026-07-15] egr_duty_target_calc
// 
// ACTUATOR function, not a MUT RequestID target (F448 not present in the
// 0x2fad0 MUT table). Computes an EGR duty target via two lookup-table
// interpolations (0x2e4a/0x2df2 selected by F0F8 bit5 and F1F2 bit11
// gating; 0x2ea2 constant table) combined via pjsr @0x142f3, with an
// optional override lookup (0x2eae) gated on F25E bit9 / EFFA / byte@0x2dd.
// Does not itself store the result - returns in R0 to its sole caller,
// egr_target_f448_update (0x18650), which performs the actual F448 write
// (see that function's plate comment for the store).
// 
// F448 = EGR position/duty target, an actuator output. Reference example
// for the actuator-driver shape (see also canister_purge_duty_calc_f4ac,
// isc_f402_stepper_target_calc).

ushort egr_duty_target_calc(void)

{
  ushort uVar1;
  byte bVar2;
  void *pvStack_4;
  
  bVar2 = 1;
  uVar1 = 0x80;
  if ((g_status_flags_f1f2 & 0x800) == 0) {
    pvStack_4 = (void *)0x2e4a;
    if ((g_status_flags_f0f8 & 0x20) != 0) {
      pvStack_4 = (void *)0x2df2;
    }
    (*axis_lookup_interp)();
    (*axis_lookup_interp)();
    table_lookup_interp(1,pvStack_4);
    table_lookup_interp(1,(void *)0x2ea2);
    uVar1 = (*mul_u16_x2_sat)();
    if ((((*(ushort *)((uint)bVar2 << 0x10 | 0xf25e) & 0x200) != 0) &&
        (*(short *)((uint)bVar2 << 0x10 | 0xeffa) == 0)) &&
       ((*(byte *)((uint)bVar2 << 0x10 | 0x2dd) & 4) == 0)) {
      uVar1 = table_lookup_interp(1,(void *)0x2eae);
      return uVar1;
    }
  }
  return uVar1;
}



void egr_mode_dispatch(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  if (DAT_000102e4 == '\0') {
    output_relay_flags_f0e6 = output_relay_flags_f0e6 & 0xffcf;
    radfan_pwm_duty_target_f494 = 0;
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xfffb;
  }
  else if (DAT_000102e4 == '\x01') {
    (*egr_sequence_control)();
  }
  else {
    radfan_pwm_duty_target_f494_calc(param_1,param_2,param_3);
  }
  return;
}



void egr_sequence_control(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  egr_valve_state_eval_f490();
  egr_f490_condition_flags_update();
  (*check_hysteresis_gate_f490)();
  uVar1 = *(ushort *)((uint)bVar2 << 0x10 | 0xf20e) & 0x11;
  if ((uVar1 != 0) ||
     (uVar1 = *(ushort *)((uint)bVar2 << 0x10 | 0xef6a),
     uVar1 < *(ushort *)((uint)bVar2 << 0x10 | 0xe9a))) {
    (*check_mode_gate_f510)(uVar1);
  }
  (*egr_f490_gear_transition_update)();
  (*egr_f0e6_valve_bits_update)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void egr_valve_state_eval_f490(void)

{
  byte *pbStack_4;
  
  if ((egr_valve_state_flags_f490 & 0x40) == 0) {
    if (bRam00012d71 < g_coolant_temp_f1e8) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x40;
    }
  }
  else if (g_coolant_temp_f1e8 <= DAT_00012d70) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xffbf;
  }
  if ((egr_valve_state_flags_f490 & 0x80) == 0) {
    if (bRam00012d73 < g_coolant_temp_f1e8) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x80;
    }
  }
  else if (g_coolant_temp_f1e8 <= bRam00012d72) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xff7f;
  }
  pbStack_4 = (byte *)0x2d70;
  if (((egr_valve_state_flags_f490 & 0x40) != 0) &&
     (pbStack_4 = (byte *)0x2d76, (egr_valve_state_flags_f490 & 0x80) != 0)) {
    pbStack_4 = (byte *)0x2d7c;
  }
  if ((egr_valve_state_flags_f490 & 0x10) == 0) {
    if (pbStack_4[1] < g_tps_load_axis_f110) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x10;
    }
  }
  else if (g_tps_load_axis_f110 <= *pbStack_4) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xffef;
  }
  if ((egr_valve_state_flags_f490 & 8) == 0) {
    if (pbStack_4[3] < g_tps_load_axis_f110) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 8;
    }
  }
  else if (g_tps_load_axis_f110 <= pbStack_4[2]) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xfff7;
  }
  if ((egr_valve_state_flags_f490 & 0x20) == 0) {
    if (pbStack_4[5] < g_tps_load_axis_f110) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x20;
    }
  }
  else if (g_tps_load_axis_f110 <= pbStack_4[4]) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xffdf;
  }
  if (((g_status_flags_f0f8 & 0x10) != 0) && ((DAT_0001f0fe & 0x10) != 0)) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x38;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [PROTOTYPE PASS 2026-07-24] NOTE: decompiler reads a raw stack0xfffe+6 value in the
// final condition (not modeled as a declared param) -- likely leftover/reused stack slot
// rather than a genuine caller argument, but not confirmed. Flagged for manual disasm
// check if this function's return value is ever found suspect at a call site.

bool egr_f490_condition_flags_update(void)

{
  bool bVar1;
  short in_stack_00000004;
  
  if ((egr_valve_state_flags_f490 & 0x2000) == 0) {
    if (DAT_00010e72 < g_coolant_temp_f1e8) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x2000;
    }
  }
  else if (g_coolant_temp_f1e8 <= DAT_00010e70) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xdfff;
  }
  if ((egr_valve_state_flags_f490 & 0x4000) == 0) {
    if (DAT_00010e76 < engine_torque_pct_f17a) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x4000;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_00010e74) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xbfff;
  }
  if ((egr_valve_state_flags_f490 & 0x8000) == 0) {
    if (DAT_00010e7a < g_tps_load_axis_f110) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x8000;
    }
  }
  else if (g_tps_load_axis_f110 <= DAT_00010e78) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0x7fff;
  }
  if (((((g_status_flags_f0f8 & 0x80) == 0) || ((egr_valve_state_flags_f490 & 0x2000) == 0)) ||
      ((egr_valve_state_flags_f490 & 0x4000) == 0)) ||
     ((in_stack_00000004 != 0 || ((egr_valve_state_flags_f490 & 0x8000) == 0)))) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  return bVar1;
}



bool check_hysteresis_gate_f490(void)

{
  if ((egr_valve_state_flags_f490 & 0x400) == 0) {
    if (DAT_00010e84 < g_tps_load_axis_f110) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x400;
    }
  }
  else if (g_tps_load_axis_f110 <= DAT_00010e82) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xfbff;
  }
  if ((egr_valve_state_flags_f490 & 0x1000) == 0) {
    if (DAT_00010e80 < g_adc_ch7_raw_f13c) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x1000;
    }
  }
  else if (g_adc_ch7_raw_f13c <= DAT_00010e7e) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xefff;
  }
  if ((egr_valve_state_flags_f490 & 0x1000) == 0) {
    DAT_0001eff6 = DAT_00010e86;
  }
  if ((((DAT_000102c7 == '\x02') && (DAT_000102e4 == '\x01')) ||
      ((egr_valve_state_flags_f490 & 0x400) == 0)) &&
     (((egr_valve_state_flags_f490 & 0x1000) != 0 && (DAT_0001eff6 != 0)))) {
    return true;
  }
  return false;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [EXISTING FUNCTION, COMMENT ADDED 2026-08-08] Shared override-mode gate,
// called from BOTH egr_sequence_control (0x187cf, genuinely-confirmed EGR
// GPIO valve mechanism) and radfan_pwm_duty_target_f494_calc (0x18bc2,
// renamed from egr_position_target_f494_calc -- see that function's plate
// comment for why "EGR" was found to be an unconfirmed label, likely
// actually a radiator fan PWM control). Being shared by both does NOT mean
// they control the same physical system -- it means this ECU variant's
// 0x102e4 config byte selects between several DIFFERENT actuator
// mechanisms that happen to share this one SCI1-override-mode check (F510
// bits 11/12, gated on F20E bit4). Returns usMode: 0 = no override, 1 =
// override low, 3 = override high (used differently by each caller). See
// mut_verification_status.md "DISCREPANCY CHECKED..." (2026-08-08).

ushort check_mode_gate_f510(void)

{
  ushort usMode;
  
  if (((g_status_flags_f20e & 0x10) == 0) || ((gear_mode_flags_f510 & 0x1800) == 0)) {
    usMode = 0;
  }
  else {
    DAT_0001eff8 = 0;
    DAT_0001eff4 = 0;
    if ((gear_mode_flags_f510 & 0x800) == 0) {
      usMode = 1;
    }
    else {
      usMode = 3;
    }
  }
  return usMode;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- decompiler shows an undeclared in_stack_00000004
// read (likely previous-gear-state param passed on stack) plus a second raw stack0xfffe+6
// read for the same purpose. Real caller-passed value, but H8 stack param modeling is
// unreliable here (same class as isc_dashpot_override_check/egr_f490_condition_flags_update).
// Left as void(void) rather than guess a stack offset; needs manual disasm trace of a
// call site to confirm.

void __stdcall_far egr_f490_gear_transition_update(undefined2 _unused_far_pad,ushort gear_bits)

{
  if (((g_status_flags_f20e & 0x11) == 0) &&
     ((((egr_valve_state_flags_f490 & 3) != 0 && ((gear_bits & 3) == 0)) ||
      (((egr_valve_state_flags_f490 & 3) == 0 && ((gear_bits & 3) != 0)))))) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 4;
  }
  else {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xfffb;
  }
  egr_valve_state_flags_f490 = gear_bits | egr_valve_state_flags_f490 & 0xfffc;
  return;
}



// [TRACED 2026-07-22] Writer for output_relay_flags_f0e6 (0xf0e6) bits 4 AND
// 5 together (mask 0x30), which drive P4DR bit4 (0xfe87) and PCDR bit6
// (0xfe97) respectively -- both real physical pins, see
// mirror_status_f0e6_to_ports @ 0x156ce, both gated on the same ROM config
// byte (0x102e4==1) that selects this "sequence control" EGR variant in the
// parent dispatcher egr_mode_dispatch (0x187a0).
// 
// CONFIDENCE: HIGH -- genuine, coherent EGR valve position-control finding:
// - Reads RAM 0xf490, an internal EGR valve-state word (bits 0/1/11), itself
//   maintained by the broader EGR state machine (egr_sequence_control @
//   0x187cf, egr_valve_state_eval_f490, egr_f490_condition_flags_update --
//   all already-named EGR functions from an earlier session, now confirmed
//   to terminate here).
// - f490 bit0 -> f0e6 bit4 (mask 0x10); f490 bit1 (gated on internal counter
//   eff8==0) -> f0e6 bit5 (mask 0x20). Reads like two-phase/two-direction
//   EGR valve drive (e.g. open/close or step-up/step-down for a duty-cycled
//   or steppered EGR valve), consistent with egr_mode_dispatch's three-way
//   hardware-variant split (0=none, 1=this two-bit sequence drive, 2=a
//   separate position-target calc via egr_position_target_f494_calc).
// - eff4/eff8 refreshed from ROM calibration constants (0x10e6a/0x10e6c)
//   when f490 bits0-1 are both clear -- consistent with a timer/duration
//   reload pattern typical of duty-cycled valve control.
// 
// Given the SAME ROM config gate (0x102e4) ties egr_mode_dispatch's variant
// selection, this writer, AND the port-mirror side together into one
// coherent, self-consistent system, this is a strong EGR valve control
// finding -- P4DR.4 / PCDR.6 = EGR valve drive outputs (exact
// open/close-vs-step semantics of the two bits not fully disambiguated).
// NOT independently confirmed against real vehicle hardware. Not reachable
// from sci1_meta_cmd_dispatch_c0_ff -- driven purely by internal EGR state
// machine, same situation as f0e6 bit1 (see egr_f0e6_bit1_update @ 0x18600).
// See mut_verification_status.md "PHYSICAL OUTPUT PIN TRACE" section.

void egr_f0e6_valve_bits_update(void)

{
  ushort usPinBits;
  
  usPinBits = 0;
  if ((egr_valve_state_flags_f490 & 3) == 0) {
    DAT_0001eff4 = DAT_00010e6a;
    DAT_0001eff8 = DAT_00010e6c;
  }
  else {
    if ((egr_valve_state_flags_f490 & 0x800) == 0) {
      if (DAT_0001eff4 != 0) goto LAB_00018bb0;
    }
    else {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xf7ff;
      DAT_0001eff4 = 0;
    }
    if ((egr_valve_state_flags_f490 & 1) != 0) {
      usPinBits = 0x10;
    }
    if (((egr_valve_state_flags_f490 & 2) != 0) && (DAT_0001eff8 == 0)) {
      usPinBits = usPinBits | 0x20;
    }
  }
LAB_00018bb0:
  output_relay_flags_f0e6 = usPinBits | output_relay_flags_f0e6 & 0xffcf;
  return;
}



// [RENAMED 2026-08-08] Formerly egr_position_target_f494_calc. Renamed
// because "EGR" was found to be an UNCONFIRMED label -- see full
// correction below and in mut_verification_status.md "DISCREPANCY
// CHECKED..." (2026-08-08).
// 
// WHAT THIS FUNCTION DOES (unchanged from prior analysis): the THIRD of 3
// hardware variants selected by ROM config byte 0x102e4 (0=none, 1=
// egr_sequence_control's GPIO drive, 2=THIS function). CONFIRMED ACTIVE on
// this ROM (0x102e4==2, read directly). Computes a target value and writes
// it to RAM F494 (now radfan_pwm_duty_target_f494), which flows:
//   F494 -> serial_fef9_duty_scale_and_set (0x15a06, runs only when
//   config==2) -> ff88_set_scaled_clamped (0x16c33): scales F494*16,
//   clamps to [1,2048] -> writes T6GR1H (0xff88), a REAL H8 Timer 6 PWM
//   duty/compare register (confirmed real hardware peripheral, not an
//   inferred RAM flag).
// 
// Externally forceable via SCI1 actuator commands 0xCD (sets F510 bit12)
// and 0xCE (sets F510 bit11), read by this function's gate
// check_mode_gate_f510 (0x18ae5): if F20E bit4 is set and either bit is
// present, this function's normal RPM-band-computed target is overridden
// with a fixed ROM calibration value from address 0xe6e instead.
// 
// CORRECTION (2026-08-08): this function and its RAM target F494 were
// previously named "egr_position_target_f494_calc" / "EGR position/duty
// target" based ONLY on being grouped alongside 2 separately-confirmed,
// GENUINELY EGR mechanisms (egr_f0e6_bit1_update -> P2DR.7 EGR solenoid,
// egr_f0e6_valve_bits_update -> P4DR.4+PCDR.6 EGR valve position) that
// share the same 0x102e4 config-byte selector. That association was NEVER
// independently verified -- this function's actual output (a Timer 6 PWM
// DUTY CYCLE register) is structurally unlike the other two confirmed EGR
// mechanisms (which drive plain GPIO bits, not PWM). Two INDEPENDENT
// external sources -- a GalantLegnum MUT-profile XML (RequestID 0x84,
// already flagged in this project as pointing at this same F494/F495 word
// and labeled "RadFans") AND EvoScan.exe's real actuator-test UI (cmd
// 0xCD/0xCE labeled "Radiator Fan Low/High") -- both independently call
// this mechanism a RADIATOR FAN, not EGR. PWM duty control is also the
// more typical mechanism for fan-speed control generally, vs GPIO/relay/
// stepper drive more typical of EGR valves on this ECU generation (matching
// the OTHER two, GENUINELY EGR, confirmed mechanisms' GPIO style).
// RENAMED accordingly: egr_position_target_f494_calc ->
// radfan_pwm_duty_target_f494_calc; global F494 -> radfan_pwm_duty_
// target_f494. Not 100% certain -- flagging as "most likely correct given
// 2 independent external sources vs 1 unconfirmed internal assumption",
// not as fully proven. See mut_verification_status.md for full reasoning.
// 
// NOT YET DONE: independently verify against a real H8/500 pinout / vehicle
// harness which physical device Timer 6's PWM output pin actually drives,
// to fully settle this rather than relying on external-source agreement
// alone.

void radfan_pwm_duty_target_f494_calc(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort *puVar1;
  short sVar2;
  ushort uVar3;
  ushort extraout_R2;
  ushort uVar4;
  byte bVar5;
  
  bVar5 = 1;
  if ((g_radfan_stage_hysteresis_flags_f492 & 1) == 0) {
    if (DAT_00010e8c < g_coolant_temp_f1e8) {
      g_radfan_stage_hysteresis_flags_f492 = g_radfan_stage_hysteresis_flags_f492 | 1;
    }
  }
  else if (g_coolant_temp_f1e8 <= DAT_00010e8a) {
    g_radfan_stage_hysteresis_flags_f492 = g_radfan_stage_hysteresis_flags_f492 & 0xfffe;
  }
  if ((g_radfan_stage_hysteresis_flags_f492 & 2) == 0) {
    if (DAT_00010e90 < g_coolant_temp_f1e8) {
      g_radfan_stage_hysteresis_flags_f492 = g_radfan_stage_hysteresis_flags_f492 | 2;
    }
  }
  else if (g_coolant_temp_f1e8 <= DAT_00010e8e) {
    g_radfan_stage_hysteresis_flags_f492 = g_radfan_stage_hysteresis_flags_f492 & 0xfffd;
  }
  if ((g_radfan_stage_hysteresis_flags_f492 & 4) == 0) {
    if (DAT_00010e94 < g_coolant_temp_f1e8) {
      g_radfan_stage_hysteresis_flags_f492 = g_radfan_stage_hysteresis_flags_f492 | 4;
    }
  }
  else if (g_coolant_temp_f1e8 <= DAT_00010e92) {
    g_radfan_stage_hysteresis_flags_f492 = g_radfan_stage_hysteresis_flags_f492 & 0xfffb;
  }
  if ((g_radfan_stage_hysteresis_flags_f492 & 8) == 0) {
    if (DAT_00010e98 < g_coolant_temp_f1e8) {
      g_radfan_stage_hysteresis_flags_f492 = g_radfan_stage_hysteresis_flags_f492 | 8;
    }
  }
  else if (g_coolant_temp_f1e8 <= DAT_00010e96) {
    g_radfan_stage_hysteresis_flags_f492 = g_radfan_stage_hysteresis_flags_f492 & 0xfff7;
  }
  radfan_speed_table_row_lookup_f496(g_coolant_temp_f1e8,param_2,param_3);
  sVar2 = (*check_hysteresis_gate_f490)();
  uVar4 = extraout_R2;
  if (sVar2 != 0) {
    uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xe88);
  }
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf1fe) & 0x11) != 0) {
    uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xe7c);
  }
  uVar3 = *(ushort *)((uint)bVar5 << 0x10 | 0xf20e) & 0x11;
  if ((uVar3 == 0) &&
     (uVar3 = *(ushort *)((uint)bVar5 << 0x10 | 0xef6a),
     *(ushort *)((uint)bVar5 << 0x10 | 0xe9a) <= uVar3)) {
    if (((*(short *)((uint)bVar5 << 0x10 | 0xf494) == 0) && (uVar4 != 0)) ||
       ((*(short *)((uint)bVar5 << 0x10 | 0xf494) != 0 && (uVar4 == 0)))) {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf490);
      *puVar1 = *puVar1 | 4;
      goto LAB_00018cb8;
    }
  }
  else {
    sVar2 = (*check_mode_gate_f510)(uVar3);
    if (sVar2 == 0) {
      uVar4 = 0;
    }
    else {
      uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xe6e);
    }
  }
  puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf490);
  *puVar1 = *puVar1 & 0xfffb;
LAB_00018cb8:
  puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xe9c);
  if ((*puVar1 <= uVar4 && uVar4 != *puVar1) && (uVar4 < *(ushort *)((uint)bVar5 << 0x10 | 0xe9e)))
  {
    uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xe9c);
  }
  *(ushort *)((uint)bVar5 << 0x10 | 0xf494) = uVar4;
  return;
}



undefined1
radfan_speed_table_row_lookup_f496(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort extraout_R2;
  ushort uVar1;
  byte bVar2;
  short sStack_4;
  
  bVar2 = 1;
  (*axis_lookup_interp)(param_1,param_2,0,2,0xd472,param_3);
  uVar1 = extraout_R2;
  if ((*(short *)((uint)bVar2 << 0x10 | 0xf0c0) != 0) &&
     (uVar1 = *(ushort *)((uint)bVar2 << 0x10 | 0xf0c0) & 0xff,
     uVar1 < *(ushort *)((uint)bVar2 << 0x10 | 0xf496))) {
    uVar1 = uVar1 + 1;
  }
  *(ushort *)((uint)bVar2 << 0x10 | 0xf496) = uVar1;
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf0f8) & 0x10) == 0) {
    if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf492) & 1) == 0) {
      sStack_4 = 0x2d04;
    }
    else if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf492) & 2) == 0) {
      sStack_4 = 0x2d10;
    }
    else {
      sStack_4 = 0x2d1c;
    }
  }
  else if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf0fe) & 0x100) == 0) {
    if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf492) & 4) == 0) {
      sStack_4 = 0x2d28;
    }
    else if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf492) & 8) == 0) {
      sStack_4 = 0x2d34;
    }
    else {
      sStack_4 = 0x2d40;
    }
  }
  else if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf492) & 4) == 0) {
    sStack_4 = 0x2d4c;
  }
  else if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf492) & 8) == 0) {
    sStack_4 = 0x2d58;
  }
  else {
    sStack_4 = 0x2d64;
  }
  return *(undefined1 *)(sStack_4 + *(short *)((uint)bVar2 << 0x10 | 0xf496));
}



// [TRACED 2026-07-22] Writer for output_relay_flags_f0e6 (0xf0e6) bit2, which
// drives P4DR bit7 (0xfe87, real physical pin -- see
// mirror_status_f0e6_to_ports @ 0x156ce), gated on the SAME ROM config byte
// (0x102ed != 0) as the port-mirror side (which also flips polarity based on
// that byte's bit3).
// 
// CONFIDENCE: HIGH -- coherent canister purge solenoid finding:
// - Gate condition (purge_enable_check @ 0x18dca): implements RPM (ef6a vs
//   ROM threshold 0x10e1c) and coolant/load-adjacent (f17a/f13e vs ROM
//   thresholds 0x10e10-0x10e1a) hysteresis, with the threshold pair itself
//   swapping depending on the CURRENT state of f0e6 bit2 (classic
//   hysteresis-band pattern) -- consistent with textbook canister-purge
//   duty-cycle control (RPM/temp gated, avoid purge at idle/cold).
// 
// CONFIRMED REACHABLE FROM SCI1 DISPATCHER, SECOND CHAIN FOUND THIS SESSION:
// purge_enable_check ALSO checks f510 bit8 (0x100) as an override -- when
// f20e bit4 is SET (a different f20e condition than the normal "not
// running" gate) AND f510 bit8 is SET, purge is force-enabled (sVar1=1)
// regardless of the RPM/temp hysteresis. f510 bit8 is set by
// sci1_meta_cmd_dispatch_c0_ff (0x28869) command byte 0xD1 (per
// cmd_c0_d8_actuator_bit_table entry 17, 00 ff 01 00 at 0x13784, confirmed
// via read_memory). So: SCI1 command 0xD1, under the right f20e state, CAN
// force the purge solenoid active -- this is genuine MUT/EvoScan-style
// actuator-test behavior, the strongest candidate found so far for a real,
// externally-triggerable actuator command on this ROM. f20e's exact bit4
// meaning is still not independently pinned down (170+ xref sites, see
// "SESSION CORRECTIONS" section), so the precise real-world trigger
// condition for cmd 0xD1 to actually work isn't 100% nailed down, but the
// mechanism itself is confirmed live end to end: cmd 0xD1 -> f510 bit8 ->
// purge_enable_check override -> f0e6 bit2 -> P4DR.7.
// 
// STRONGLY RECOMMEND prioritizing this as the next thing to verify/test if
// real hardware access is available -- of everything found in this whole
// investigation, this is the most concrete "send this SCI1 byte, this real
// pin changes" candidate. See mut_verification_status.md "PHYSICAL OUTPUT
// PIN TRACE" section for full context.

void purge_f0e6_bit2_update(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((DAT_000102ed == '\0') || (sVar2 = purge_enable_check(), sVar2 == 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
    *puVar1 = *puVar1 & 0xfffb;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
  *puVar1 = *puVar1 | 4;
  return;
}



short purge_enable_check(void)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uStack_4;
  
  uStack_4 = DAT_00010e14;
  uVar2 = DAT_00010e18;
  uVar3 = DAT_00010e10;
  if ((output_relay_flags_f0e6 & 4) != 0) {
    uStack_4 = DAT_00010e16;
    uVar2 = DAT_00010e1a;
    uVar3 = DAT_00010e12;
  }
  sVar1 = 0;
  if (((g_status_flags_f20e & 0x11) == 0) && (DAT_00010e1c < g_rpm_zone_dwell_counter_ef6a)) {
    if ((uVar2 < engine_torque_pct_f17a) && (uStack_4 < g_tps_load_trimmed_f13e)) {
      DAT_0001efee = DAT_00010e1e;
    }
    if ((g_tps_load_trimmed_f13e <= uVar3) &&
       (((engine_torque_pct_f17a <= uVar2 || (DAT_0001efee == 0)) &&
        ((g_status_flags_f0f8 & 0x2000) != 0)))) {
      return 0;
    }
  }
  else {
    if ((((g_status_flags_f20e & 0x10) != 0) && ((gear_mode_flags_f510 & 0x100) != 0)) ||
       ((g_status_flags_f20e & 0x11) == 0)) {
      sVar1 = 1;
    }
    if ((DAT_000102ed & 4) == 0) {
      return sVar1;
    }
    if (sVar1 != 0) {
      return 0;
    }
  }
  return 1;
}



void ign_advance_top_update(void)

{
  ushort unaff_FP;
  ushort unaff_retaddr;
  
  (*ign_advance_enable_flags_f440_update)();
  (*ign_advance_idle_correction_f44e)();
  ign_advance_base_calc_f860();
  wgdc_correction_integrator_update(unaff_FP);
  wgdc_output_clamp_f44c(unaff_retaddr);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void ign_advance_enable_flags_f440_update(void)

{
  byte bVar1;
  
  bVar1 = (byte)((ushort)DAT_0001f1e0 >> 8);
  if ((ign_advance_enable_flags_f440 & 4) == 0) {
    if (ram0x00010dfe < bVar1) {
      ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 | 4;
    }
  }
  else if (bVar1 <= DAT_00010e00) {
    ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 & 0xfffb;
  }
  if ((ign_advance_enable_flags_f440 & 8) == 0) {
    if (ram0x00010dfc < DAT_0001eec8) {
      ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 | 8;
    }
  }
  else if (DAT_0001eec8 <= _DAT_00010dfa) {
    ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 & 0xfff7;
  }
  if (DAT_000102f0 == '\x02') {
    ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 | 2;
  }
  else {
    if ((ign_advance_enable_flags_f440 & 2) == 0) {
      if ((ushort)DAT_00010e06 < (ushort)engine_torque_pct_f17a) {
        ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 | 2;
      }
    }
    else if ((ushort)engine_torque_pct_f17a <= (ushort)DAT_00010e08) {
      ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 & 0xfffd;
    }
    if ((ign_advance_enable_flags_f440 & 1) != 0) {
      if ((ushort)engine_torque_pct_f17a <= (ushort)DAT_00010e04) {
        ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 & 0xfffe;
      }
      goto LAB_00018f38;
    }
    if ((ushort)engine_torque_pct_f17a <= (ushort)BYTE_ARRAY_00010e02) goto LAB_00018f38;
  }
  ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 | 1;
LAB_00018f38:
  if (((((g_status_flags_f1fe & 8) == 0) && ((DAT_0001f200 & 1) == 0)) &&
      ((ign_advance_enable_flags_f440 & 4) == 0)) && ((ign_advance_enable_flags_f440 & 8) != 0)) {
    ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 & 0xffdf;
  }
  else {
    ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 | 0x20;
  }
  if (((ign_advance_enable_flags_f440 & 0x20) == 0) &&
     (((ign_advance_enable_flags_f440 & 2) != 0 || ((ign_advance_enable_flags_f440 & 1) == 0)))) {
    ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 | 0x10;
  }
  else {
    ign_advance_enable_flags_f440 = ign_advance_enable_flags_f440 & 0xffef;
  }
  if (DAT_00010df4 < inj_pw_output_f186) {
    DAT_0001f040 = DAT_00010df6;
  }
  return;
}



ushort ign_advance_base_calc_f860(void)

{
  ushort uVar1;
  byte bVar2;
  void *table;
  
  bVar2 = 1;
  (*axis_lookup_interp)();
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf0f8) & 0x20) == 0) {
    table = (void *)0x2aaa;
  }
  else {
    table = (void *)0x2a96;
  }
  uVar1 = table_lookup_interp(1,table);
  *(ushort *)((uint)bVar2 << 0x10 | 0xf860) = uVar1;
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf20e) & 0x10) == 0) {
    if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf20e) & 1) != 0) {
      return 0;
    }
  }
  else {
    uVar1 = 0;
    if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf510) & 0x40) != 0) {
      return 200;
    }
  }
  return uVar1;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// wgdc_correction_integrator_update: reads Boost Control Load Offset-adjacent RAM (0xE0A
// region) and WGDC correction tables (0x2a80/0x2a72 region, matches logging.txt claim of
// Turbo Boost Error Correction / Max Total Upward WGDC Correction tables), reads/writes
// 0xF44A. Feeds wgdc_output_clamp_f44c (0x19110). VERIFIED - matches logging.txt structure.

byte  [2] wgdc_correction_integrator_update(ushort param_fp1)

{
  undefined2 uVar1;
  short sVar2;
  ushort uVar3;
  byte abVar4 [2];
  short extraout_R2;
  byte abVar5 [2];
  byte bVar6;
  ushort unaff_retaddr;
  ushort in_stack_00000004;
  ushort lo;
  
  bVar6 = 1;
  if (((g_status_flags_f20e & 0x11) == 0) && ((ign_advance_enable_flags_f440 & 0x20) == 0)) {
    abVar5 = DAT_0001f44a;
    if ((ign_advance_enable_flags_f440 & 0x10) == 0) {
      abVar4[0] = 0;
      abVar4[1] = 0;
      if (DAT_0001f05c == (byte  [2])0x0) {
        DAT_0001f05c = BYTE_ARRAY_00010df0;
        sat_sub_u16((DAT_0001f196 >> 1) + 0x20,DAT_0001f44e + (short)BYTE_ARRAY_00010e0a);
        uVar1 = (*clamp_u8)();
        *(undefined2 *)((uint)bVar6 << 0x10 | 0xf0ba) = uVar1;
        (*axis_lookup_interp)();
        sVar2 = table_lookup_interp(1,(void *)0x2a80);
        sat_sub_u16(extraout_R2 + sVar2,0x80);
        (*axis_lookup_interp)();
        uVar3 = table_lookup_interp(1,(void *)0x2a72);
        uVar3 = clamp_u16(uVar3,*(ushort *)((uint)bVar6 << 0x10 | 0xdf2),uVar3);
        lo = 200;
        uVar3 = sat_sub_u16(in_stack_00000004 + uVar3,0x80);
        uVar3 = clamp_u16(uVar3,lo,unaff_retaddr);
        abVar4 = (byte  [2])sat_sub_u16(uVar3 + 0x80,in_stack_00000004);
        abVar5 = abVar4;
      }
      goto LAB_000190d6;
    }
  }
  else {
    abVar5[0] = 0;
    abVar5[1] = 0x80;
  }
  DAT_0001f05c = BYTE_ARRAY_00010df0;
  abVar4 = BYTE_ARRAY_00010df0;
LAB_000190d6:
  *(byte (*) [2])((uint)bVar6 << 0x10 | 0xf44a) = abVar5;
  return abVar4;
}



void ign_advance_idle_correction_f44e(void)

{
  undefined2 uVar1;
  byte bVar2;
  void *table;
  
  bVar2 = 1;
  (*axis_lookup_interp)();
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf0f8) & 0x20) == 0) {
    table = (void *)0x2ad2;
  }
  else {
    table = (void *)0x2abe;
  }
  uVar1 = table_lookup_interp(1,table);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf44e) = uVar1;
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// wgdc_output_clamp_f44c: reads 0xF44A (from wgdc_correction_integrator_update @0x18fe0),
// clamps/scales, writes final result @0xF44C/F44D. MUT RequestID 0x86 -> table entry
// 0xF44D (low byte of the word written here) = WGDC. VERIFIED - logging.txt CONFIRMED-
// section claim holds under current decoder, full writer chain confirmed end to end.
// Relationship to WGDCCorr (RequestID 0x8B -> table entry 0xF4AD, separately confirmed
// address-correct) remains UNDETERMINED per logging.txt OPEN ITEM #3 - not resolved by
// this verification pass, still open.

void wgdc_output_clamp_f44c(ushort param_fp1)

{
  ushort uVar1;
  byte bVar2;
  ushort in_stack_00000004;
  ushort b;
  
  bVar2 = 1;
  if ((ign_advance_enable_flags_f440 & 0x30) == 0) {
    b = 0x80;
    uVar1 = in_stack_00000004 + DAT_0001f44a;
  }
  else {
    uVar1 = g_status_flags_f20e & 0x11;
    if (uVar1 != 0) {
      goto LAB_00019171;
    }
    if (DAT_0001f000 != 0) {
      in_stack_00000004 = DAT_0001f44c;
      goto LAB_00019171;
    }
    DAT_0001f000 = DAT_00010df8;
    b = 0x22;
    uVar1 = DAT_0001f44c;
    if ((ign_advance_enable_flags_f440 & 0x20) == 0) {
      uVar1 = sat_add_u16(DAT_0001f44c,0x22);
      in_stack_00000004 = uVar1;
      goto LAB_00019171;
    }
  }
  uVar1 = sat_sub_u16(uVar1,b);
  in_stack_00000004 = uVar1;
LAB_00019171:
  uVar1 = o2_downstream_mode66_select(uVar1,in_stack_00000004);
  uVar1 = clamp_u16(uVar1,0,200);
  *(ushort *)((uint)bVar2 << 0x10 | 0xf44c) = uVar1;
  return;
}



void ign_advance_load_correction_f450(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  if (DAT_0001ef90 != 0) {
    if ((g_status_flags_f20e & 0x10) != 0) {
      DAT_0001f450 = DAT_00010dee;
      return;
    }
    if ((g_status_flags_f1fe & 8) == 0) {
      (*axis_lookup_interp)();
      uVar1 = table_lookup_interp(1,(void *)0x2af8);
      *(undefined2 *)((uint)bVar2 << 0x10 | 0xf450) = uVar1;
      return;
    }
  }
  DAT_0001f450 = 0;
  return;
}



void rpm_map_zone_flags_f43e_update(void)

{
  ushort usZoneBits;
  
  if ((g_status_flags_f20e & 1) != 0) {
    usZoneBits = 0;
    if (DAT_00010d7c <= g_iat_axis_f118) {
      if (DAT_00010d80 <= g_tps_load_axis_f10e) {
        usZoneBits = 0x200;
      }
    }
    if (DAT_00010d7a <= g_iat_axis_f118) {
      if (DAT_00010d7e <= g_tps_load_axis_f10e) {
        usZoneBits = usZoneBits | 0x100;
      }
    }
    g_rpm_zone_flags_f43e = usZoneBits | g_rpm_zone_flags_f43e & 0xfcff;
  }
  return;
}



void f0ea_bit7_rpm_zone_update(void)

{
  ushort uVar1;
  undefined1 extraout_var;
  bool bVar2;
  ushort extraout_R2;
  
  bVar2 = rpm_zone_enable_check();
  if (CONCAT11(extraout_var,bVar2) == 0) {
    uVar1 = g_status_flags_f0ea;
    g_status_flags_f0ea = uVar1 & 0xff7f;
  }
  else {
    uVar1 = g_status_flags_f0ea;
    g_status_flags_f0ea = uVar1 | 0x80;
  }
  uVar1 = DAT_0000f43e;
  if ((extraout_R2 & (uVar1 ^ extraout_R2) & 0x10) == 0) {
    uVar1 = DAT_0000f43e;
    DAT_0000f43e = uVar1 & 0xfff7;
  }
  else {
    uVar1 = DAT_0000f43e;
    DAT_0000f43e = uVar1 | 8;
  }
  return;
}



void f0ea_bit7_f43e_bit3_clear(void)

{
  ushort uVar1;
  
  uVar1 = g_status_flags_f0ea;
  g_status_flags_f0ea = uVar1 & 0xff7f;
  uVar1 = DAT_0000f43e;
  DAT_0000f43e = uVar1 & 0xfff7;
  return;
}



bool rpm_zone_enable_check(void)

{
  bool bVar1;
  ushort usThreshold;
  
  usThreshold = DAT_00010d78 * 0x14;
  if ((g_rpm_zone_flags_f43e & 0x200) == 0) {
    if ((g_rpm_zone_flags_f43e & 0x100) != 0) {
      usThreshold = DAT_00010d76 * 0x14;
    }
  }
  else {
    usThreshold = DAT_00010d74 * 0x14;
  }
  if (DAT_0001f26e >> 8 < DAT_00010dda) {
    usThreshold = 0;
  }
  if ((g_status_flags_f20e & 1) == 0) {
    if (((g_status_flags_f20e & 0x10) != 0) || (usThreshold < g_rpm_zone_dwell_counter_ef6a)) {
      g_rpm_zone_flags_f43e = g_rpm_zone_flags_f43e & 0xffef;
    }
  }
  else {
    g_rpm_zone_flags_f43e = g_rpm_zone_flags_f43e | 0x10;
  }
  if (((((g_status_flags_f20e & 0x10) == 0) || ((gear_mode_flags_f510 & 8) == 0)) &&
      ((((ushort)(DAT_00010ddc * 0x14) < g_rpm_zone_dwell_counter_ef6a &&
        ((g_status_flags_f21c & 1) != 0)) || ((g_rpm_zone_flags_f43e & 0x10) == 0)))) &&
     (((g_status_flags_f20e & 0x10) != 0 ||
      ((ushort)(DAT_00010d78 * 0x14) < g_rpm_zone_dwell_counter_ef6a)))) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  return bVar1;
}



// [TRACED 2026-07-22] Writer for output_relay_flags_f0e6 (0xf0e6) bit6, which
// drives PCDR bit7 (0xfe97, real physical pin, direct/non-inverted -- see
// mirror_status_f0e6_to_ports @ 0x156ce).
// 
// Gate: ign_advance_rpm_zone_enable_check (0x19364) -- RPM-zone hysteresis
// check using ROM lookup tables (0xd6e/0xd70/0xd72, selected by f43e state
// bits) compared against ef6a (RPM-adjacent), with a load-based override
// (f186) and a self-referencing hysteresis read of ITS OWN output
// (f0e6 bit6) feeding table_lookup_interp/sat_sub_u16 -- a real closed-loop
// ignition-zone control pattern (name inherited but behaviorally consistent:
// ignition-related RPM zone gating, plausibly related to a knock-zone /
// high-RPM ignition confirm output given the naming continuity with
// ign_advance_f0e6_bit7_update @ 0x173c0).
// 
// CONFIRMED REACHABLE FROM SCI1 DISPATCHER: this check ALSO tests f510 bit7
// (0x80) as part of its gate condition (bypasses the normal RPM/load check
// when f20e bit4 is set AND f510 bit7 is set). f510 bit7 is set by
// sci1_meta_cmd_dispatch_c0_ff (0x28869) command byte 0xD2 -- table entry 18,
// 00 ff 00 82 at ROM 0x13788 (CORRECTED after an initial miscount that
// wrongly said 0xD3/0x1378c -- always re-verify table entry addresses with
// read_memory before citing them, this project has now made this class of
// error three times). Mask 0x0082 sets BOTH bit1 AND bit7 simultaneously,
// resolving the "two-bit anomaly" flagged in the sci1_meta_cmd_dispatch_c0_ff
// plate comment: it's not an anomaly, cmd 0xD2 deliberately triggers TWO
// independent override checks at once (this one for bit7, and a separate
// still-unfound consumer for bit1 -- NOT the same as the bit1 traced at cmd
// 0xD8/engine_mode_f20e_f510_check, which despite similar naming reads f510
// bit1 via a DIFFERENT check; that one is triggered by cmd 0xD8's OWN table
// entry, index 24, mask 0x0002 -- the two "bit1" references are coincidental
// naming, not the same bit; re-verify carefully if revisiting).
// 
// Third SCI1-reachable output chain found this session (after cmd 0xD1's
// purge-solenoid override and cmd 0xD8's P1DR.5 chain): cmd 0xD2 -> f510
// bit7 -> ign_advance_rpm_zone_enable_check override -> f0e6 bit6 -> PCDR.7.
// Physical real-world meaning still not confirmed - same low-confidence
// "ignition-adjacent" inference as bit7/P1DR.5, not a proven component
// identity. See mut_verification_status.md "PHYSICAL OUTPUT PIN TRACE"
// section.

void f0e6_bit6_update(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = ign_advance_rpm_zone_enable_check();
  if (uVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
    *puVar1 = *puVar1 & 0xffbf;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf0e6);
    *puVar1 = *puVar1 | 0x40;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort ign_advance_rpm_zone_enable_check(void)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar1 = table_lookup_interp(1,(void *)0x2aea);
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0e6) & 0x40) == 0) {
    uVar1 = sat_sub_u16(uVar1,*(ushort *)((uint)bVar3 << 0x10 | 0xe0e));
  }
  uVar2 = *(short *)((uint)bVar3 << 0x10 | 0xd72) * 0x14;
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf43e) & 0x200) == 0) {
    if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf43e) & 0x100) != 0) {
      uVar2 = *(short *)((uint)bVar3 << 0x10 | 0xd70) * 0x14;
    }
  }
  else {
    uVar2 = *(short *)((uint)bVar3 << 0x10 | 0xd6e) * 0x14;
  }
  if ((((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 0x10) == 0) ||
      ((*(ushort *)((uint)bVar3 << 0x10 | 0xf510) & 0x80) == 0)) &&
     (((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 0x11) != 0 ||
      (((*(ushort *)((uint)bVar3 << 0x10 | 0xef6a) <= uVar2 ||
        ((*(ushort *)((uint)bVar3 << 0x10 | 0xf1fe) & 8) != 0)) ||
       (uVar1 < *(ushort *)((uint)bVar3 << 0x10 | 0xf186))))))) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



void stub_empty_193f0(void)

{
  return;
}



void canister_purge_control_flags_f4d6_update(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  
  uVar2 = 0;
  if (DAT_000102ea != '\0') {
    uVar3 = 0;
    uVar1 = DAT_0001f4d6 & 4;
    if ((DAT_000102c8 == '\0') || ((g_status_flags_f1f2 & 0x2000) != 0)) {
      uVar3 = DAT_0001f0fe & 2;
    }
    else if ((g_purge_enable_cond_f4a4 & 0x80) == 0) {
      uVar3 = tcu_rx_confirmed_byte2_f4c0 & 0x80;
    }
    uVar2 = uVar1;
    if (((DAT_0001f3f6 & 0x80) != 0) && ((DAT_0001f4d6 & 4) == 0)) {
      if (((DAT_00010e44 <= g_tps_load_axis_f10e) && (uVar2 = uVar1 | 1, uVar3 != 0)) &&
         ((uVar2 = uVar1 | 0x83, DAT_000102c8 != '\0' || ((DAT_0001f0fe & 4) != 0)))) {
        uVar2 = uVar1 | 0xc3;
      }
    }
    if ((((g_status_flags_f20e & 0x10) != 0) || ((g_status_flags_f0f8 & 0x40) != 0)) || (uVar3 == 0)
       ) {
      DAT_0001efec = DAT_00010e46;
    }
    if (((uVar2 & 0x80) != 0) && (DAT_0001efec == 0)) {
      uVar2 = uVar2 & 0xffec | 4;
    }
    if (DAT_0001ef90 == 0) {
      uVar2 = uVar2 & 0xfffb;
    }
    if ((DAT_00010e4a < DAT_0001f18e) || (DAT_00010e4c < engine_torque_pct_f17a)) {
      uVar2 = uVar2 | 0x20;
    }
  }
  DAT_0001f4d6 = uVar2 | DAT_0001f4d6 & 0xff08;
  return;
}



void canister_purge_mode_dispatch(void)

{
  if ((g_status_flags_f1f2 & 0x2000) == 0) {
    g_purge_enable_cond_f4a2 = g_purge_enable_cond_f4a2 & 0xff7f;
    DAT_0001f4ae = DAT_0001f4ae & 0xff30;
  }
  else {
    canister_purge_gear_mode_select_f4a2();
    canister_purge_duty_calc_f4ac();
  }
  return;
}



void canister_purge_gear_mode_select_f4a2(void)

{
  ushort uVar1;
  ushort extraout_R2;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar1 = canister_purge_feedback_active_check();
  uVar2 = extraout_R2;
  if (uVar1 != 0) {
    uVar2 = extraout_R2 | 0x80;
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf4a2) =
       uVar2 | *(ushort *)((uint)bVar3 << 0x10 | 0xf4a2) & 0xff7c;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort canister_purge_feedback_active_check(void)

{
  ushort uVar1;
  
  if ((((g_status_flags_f20e & 0x11) == 0) && ((DAT_0001f3f6 & 0x80) != 0)) &&
     ((tcu_rx_confirmed_byte1_f4be & 0x30) == 0x20)) {
    if ((g_purge_enable_cond_f4a4 & 0x80) != 0) {
      if (0xfe < DAT_0001f4ac) goto LAB_0001955c;
    }
    uVar1 = 1;
  }
  else {
LAB_0001955c:
    uVar1 = 0;
  }
  return uVar1;
}



// [NEW 2026-07-15, UPDATED 2026-07-26] canister_purge_duty_calc_f4ac
// 
// ACTUATOR function, not a MUT RequestID target (F4AC not present in the
// 0x2fad0 MUT table). Reference example for the general "actuator driver"
// shape used to help classify other traced cells as request-vs-actuator:
// lookup table (0x2ce8/0x2b06 constant refs via pjsr @0x14656 axis
// interpolation) -> threshold-select (pjsr @0x14000) -> clamp to 0xFF ->
// store to F4AC.
// 
// Gating: checks F4A2 bit7 and F4A4 bit7 (purge-enable conditions), and
// tcu_rx_confirmed_byte1_f4be (F4BE, RENAMED 2026-07-26) bits (mode select),
// falls back to a fixed constant @0xe5e or 0xFF (purge off) when conditions
// aren't met. Also reads tcu_rx_confirmed_byte0_f4bc (F4BC, RENAMED
// 2026-07-26) as an addend. BOTH of these inputs are checksum-confirmed
// bytes from a live TCU (transmission control unit) serial RX frame -- see
// tcu_rx_main_scheduler (0x2aa36) for the full frame validator -- meaning
// canister purge duty is genuinely influenced by data arriving over an
// ECU<->TCU link, not just internal engine-side state. Plausible real-world
// reason: purge commonly gets suspended/modulated during transmission shift
// events. Writes F0BA (MUT_E1, the shared scratch cell documented in
// review.md item 7) as a side effect via the 0x14219 calls @0x195de/0x195e7
// - one more confirmed writer to add to that cell's "many unrelated callers"
// list if not already covered.
// 
// F4AC itself = canister purge solenoid duty cycle target, an actuator
// output. STILL NOT resolved: F4AC has no confirmed physical-hardware
// consumer -- checked 2026-07-26 whether software_pwm_output_scheduler
// (0x158d7, the software bit-banged PWM driving WGDC/ISC/O2-heater/EGR
// duties onto real port pins, see mut_verification_status.md "FIFTH SCI1
// ACTUATOR-FORCE CHAIN" and the timer-sweep section) reads F4AC -- it does
// NOT. xrefs to F4AC are self-references plus one init write only. The real
// output path for canister purge duty remains untraced; note this ROM
// ALSO has a separate simple on/off purge output already fully traced
// (purge_f0e6_bit2_update -> F0E6 bit2 -> P4DR.7, SCI1-forceable via cmd
// 0xD1) -- unclear whether F4AC's duty-cycle path and the F0E6 on/off path
// drive the same physical purge solenoid (duty modulating an otherwise-on
// valve) or two different valves/stages. Not yet checked.

void canister_purge_duty_calc_f4ac(void)

{
  ushort uVar1;
  undefined2 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((g_purge_enable_cond_f4a2 & 0x80) == 0) {
    uVar1 = 0xff;
  }
  else if ((g_purge_enable_cond_f4a4 & 0x80) == 0) {
    bVar3 = 1;
    if (((tcu_rx_confirmed_byte1_f4be & 0xc0) == 0xc0) || ((tcu_rx_confirmed_byte1_f4be & 4) != 0))
    {
      uVar1 = DAT_00010e5e;
    }
    else {
      uVar1 = table_lookup_interp(1,(void *)0x2ce8);
      sat_add_u16(*(ushort *)((uint)bVar3 << 0x10 | 0xf4bc),uVar1);
      (*muldiv_s16_rounded)();
      uVar2 = (*muldiv_s16_rounded)();
      *(undefined2 *)((uint)bVar3 << 0x10 | 0xf0ba) = uVar2;
      (*axis_lookup_interp)();
      (*axis_lookup_interp)();
      uVar1 = table_lookup_interp(1,(void *)0x2b06);
    }
  }
  else {
    if (DAT_0001eff0 != 0) {
      uVar1 = DAT_0001f4ac;
      goto LAB_0001962b;
    }
    uVar1 = sat_add_u16(DAT_0001f4ac,1);
  }
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeff0) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xe68);
LAB_0001962b:
  if (uVar1 < 0xff) {
    *(ushort *)((uint)bVar3 << 0x10 | 0xf4ac) = uVar1;
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf4ac) = 0xff;
  }
  return;
}



void diagnostic_status_f4b4_build(void)

{
  ushort uVar1;
  undefined2 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  DAT_0001f4b4 = 0;
  if ((((g_diag_flags_eed6 & 0xee) != 0) || ((DAT_0001eed8 & 0xca) != 0)) ||
     ((g_diag_flags_eeda & 1) != 0)) {
    DAT_0001f4b4 = 0x80;
  }
  if ((g_status_flags_f0f8 & 0x80) != 0) {
    DAT_0001f4b4 = DAT_0001f4b4 | 0x40;
  }
  if ((DAT_0001f4d6 & 2) != 0) {
    DAT_0001f4b4 = DAT_0001f4b4 | 0x20;
  }
  if ((DAT_0001f4d6 & 1) != 0) {
    DAT_0001f4b4 = DAT_0001f4b4 | 0x10;
  }
  if ((g_status_flags_f0f8 & 2) != 0) {
    DAT_0001f4b4 = DAT_0001f4b4 | 4;
  }
  if ((DAT_0001f4ae & 0xf) != 0) {
    DAT_0001f4b4 = DAT_0001f4b4 | 2;
  }
  if ((g_status_flags_f20e & 0x1000) != 0) {
    DAT_0001f4b4 = DAT_0001f4b4 | 1;
  }
  uVar1 = DAT_00010e42 | DAT_00010e40;
  DAT_0001f4b6 = uVar1 | 4;
  if ((g_status_flags_f1f0 & 0xc) == 0) {
    if ((g_status_flags_f1f0 & 2) == 0) {
      if (((g_status_flags_f1f0 & 1) == 0) || ((g_status_flags_f1f0 & 0x20) != 0)) {
        DAT_0001f4b6 = uVar1 | 7;
      }
    }
    else {
      DAT_0001f4b6 = uVar1 | 5;
    }
  }
  else {
    DAT_0001f4b6 = uVar1 | 6;
  }
  DAT_0001f4b8 = 0;
  if ((g_purge_enable_cond_f4a4 & 0x80) != 0) {
    DAT_0001f4b8 = 0x40;
  }
  if (DAT_0001031f == '\0') {
    DAT_0001f198 = inj_pw_output_f18c;
  }
  else {
    (*muldiv_s16_rounded)();
    uVar2 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf198) = uVar2;
  }
  return;
}



void o2_sensor_control_dispatch(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort extraout_R2;
  ushort extraout_R2_00;
  ushort uVar3;
  undefined2 uVar4;
  byte bVar5;
  undefined2 uStack_6;
  undefined2 uStack_4;
  
  bVar5 = 1;
  uVar2 = 0;
  uVar4 = 0;
  uVar3 = 0;
  uStack_6 = 0;
  uStack_4 = 0;
  if ((g_status_flags_f1f2 & 0x4000) != 0) {
    if ((DAT_000102e9 == '\x03') || (DAT_000102e9 == '\x04')) {
      uVar2 = calc_load_correction_f10e();
      if (uVar2 == 1) {
        uVar4 = 0x80;
      }
      else if (uVar2 == 2) {
        uVar4 = *(undefined2 *)((uint)bVar5 << 0x10 | 0x13ec);
      }
    }
    else {
      uVar2 = o2_upstream_enable_check();
      if (uVar2 != 0) {
        uStack_6 = 0x80;
        uVar4 = 0x80;
      }
    }
    if (*(char *)((uint)bVar5 << 0x10 | 0x2e9) == '\x03') {
      uVar2 = calc_load_correction_f10e();
      if (uVar2 == 1) {
        uVar3 = 0x80;
      }
      else if (uVar2 == 2) {
        uVar3 = *(ushort *)((uint)bVar5 << 0x10 | 0x13ee);
      }
      uVar2 = extraout_R2_00;
      if (0x7f < uVar3) {
        uVar2 = extraout_R2_00 | 0x400;
      }
    }
    else {
      uVar1 = o2_downstream_enable_check();
      uVar2 = extraout_R2;
      if (uVar1 != 0) {
        uVar2 = extraout_R2 | 0xc00;
        uStack_4 = 0x80;
        uVar3 = 0x80;
      }
    }
  }
  *(ushort *)((uint)bVar5 << 0x10 | 0xf0e6) =
       uVar2 | *(ushort *)((uint)bVar5 << 0x10 | 0xf0e6) & 0xf0ff;
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf452) = uVar4;
  *(ushort *)((uint)bVar5 << 0x10 | 0xf454) = uVar3;
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf456) = uStack_6;
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf458) = uStack_4;
  return;
}



ushort o2_upstream_enable_check(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  
  uVar3 = DAT_00010ea2;
  if (DAT_000102e9 == '\x02') {
    uVar3 = DAT_00010eac;
    uVar2 = DAT_00010eae;
    if ((output_relay_flags_f0e6 & 0x100) != 0) {
      uVar3 = DAT_00010eaa;
      uVar2 = DAT_00010eb0;
    }
    if (((g_status_flags_f20e & 0x10) == 0) || ((gear_mode_flags_f510 & 0x2000) == 0)) {
      if ((g_status_flags_f20e & 0x11) != 0) {
        return 0;
      }
      if (g_tps_load_axis_f10e <= uVar2) {
        return 0;
      }
      if ((g_status_flags_f1fe & 0x11) != 0) {
        return 0;
      }
      if (g_rpm_zone_dwell_counter_ef6a <= (ushort)(DAT_00010ea8 * 0x14)) {
        return 0;
      }
      if (uVar3 <= engine_torque_pct_f17a) {
        return 0;
      }
    }
  }
  else {
    bVar4 = 1;
    if (DAT_000102e9 == '\x01') {
      if (((g_status_flags_f20e & 0x10) != 0) && ((gear_mode_flags_f510 & 0x2000) != 0)) {
        return 0;
      }
    }
    else {
      (*axis_lookup_interp)();
      uVar2 = table_lookup_interp(1,(void *)0x2d70);
      if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0e6) & 0x300) == 0) {
        uVar2 = sat_sub_u16(uVar2,*(ushort *)((uint)bVar4 << 0x10 | 0xea6));
        uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xea4);
      }
      if (((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x10) == 0) ||
         ((*(ushort *)((uint)bVar4 << 0x10 | 0xf510) & 0x2000) == 0)) {
        if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x10) != 0) {
          return 0;
        }
        if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf1fe) & 8) != 0) {
          return 0;
        }
        if (((((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 1) == 0) &&
             ((ushort)(*(short *)((uint)bVar4 << 0x10 | 0xea0) * 0x14) <
              *(ushort *)((uint)bVar4 << 0x10 | 0xef6a))) &&
            (uVar2 < *(ushort *)((uint)bVar4 << 0x10 | 0xf186))) &&
           (puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf10e), uVar3 < *puVar1 || uVar3 == *puVar1))
        {
          return 0;
        }
      }
    }
  }
  return 1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort o2_downstream_enable_check(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  
  uVar3 = DAT_00010ea2;
  if (DAT_000102e9 == '\x02') {
    uVar3 = DAT_00010eb6;
    uVar2 = DAT_00010eb8;
    if ((output_relay_flags_f0e6 & 0x400) != 0) {
      uVar3 = DAT_00010eb4;
      uVar2 = DAT_00010eba;
    }
    if (((g_status_flags_f20e & 0x10) == 0) || ((gear_mode_flags_f510 & 0x2000) == 0)) {
      if ((g_status_flags_f20e & 0x11) != 0) {
        return 0;
      }
      if (g_tps_load_axis_f10e <= uVar2) {
        return 0;
      }
      if ((g_status_flags_f1fe & 0x11) != 0) {
        return 0;
      }
      if (g_rpm_zone_dwell_counter_ef6a <= (ushort)(DAT_00010eb2 * 0x14)) {
        return 0;
      }
      if (uVar3 <= engine_torque_pct_f17a) {
        return 0;
      }
    }
  }
  else {
    bVar4 = 1;
    if (DAT_000102e9 == '\x01') {
      if (((g_status_flags_f20e & 0x10) != 0) && ((gear_mode_flags_f510 & 0x2000) != 0)) {
        return 0;
      }
    }
    else {
      (*axis_lookup_interp)();
      uVar2 = table_lookup_interp(1,(void *)0x2d70);
      if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0e6) & 0xc00) == 0) {
        uVar2 = sat_sub_u16(uVar2,*(ushort *)((uint)bVar4 << 0x10 | 0xea6));
        uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xea4);
      }
      if (((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x10) == 0) ||
         ((*(ushort *)((uint)bVar4 << 0x10 | 0xf510) & 0x2000) == 0)) {
        if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x10) != 0) {
          return 0;
        }
        if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf1fe) & 8) != 0) {
          return 0;
        }
        if (((((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 1) == 0) &&
             ((ushort)(*(short *)((uint)bVar4 << 0x10 | 0xea0) * 0x14) <
              *(ushort *)((uint)bVar4 << 0x10 | 0xef6a))) &&
            (uVar2 < *(ushort *)((uint)bVar4 << 0x10 | 0xf186))) &&
           (puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf10e), uVar3 < *puVar1 || uVar3 == *puVar1))
        {
          return 0;
        }
      }
    }
  }
  return 1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort calc_load_correction_f10e(void)

{
  ushort *puVar1;
  undefined2 uVar2;
  ushort a;
  ushort uVar3;
  byte bVar4;
  undefined2 in_stack_00000004;
  short in_stack_00000006;
  
  bVar4 = 1;
  if ((g_status_flags_f20e & 0x10) == 0) {
    if (((g_status_flags_f20e & 1) == 0) &&
       ((uVar2 = (short)((uint)DAT_00010ea0 * 0x14 >> 0x10),
        (ushort)((uint)DAT_00010ea0 * 0x14) < g_rpm_zone_dwell_counter_ef6a ||
        (uVar2 = in_stack_00000004, (**(ushort **)((int)in_stack_00000004 + 6) & 0x8000) == 0)))) {
      (*axis_lookup_interp)(uVar2);
      if (in_stack_00000006 == 0) {
        uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xea2);
        a = table_lookup_interp(1,(void *)0x2d70);
        if (*(short *)((uint)bVar4 << 0x10 | 0xf452) == 0) {
          a = sat_sub_u16(a,*(ushort *)((uint)bVar4 << 0x10 | 0xea6));
          uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xea4);
        }
      }
      else {
        uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0x13f0);
        a = table_lookup_interp(1,(void *)0x2d70);
        if (*(short *)((uint)bVar4 << 0x10 | 0xf454) == 0) {
          a = sat_sub_u16(a,*(ushort *)((uint)bVar4 << 0x10 | 0x13f4));
          uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0x13f2);
        }
      }
      if (((*(ushort *)((uint)bVar4 << 0x10 | 0xf1fe) & 8) != 0) ||
         ((a < *(ushort *)((uint)bVar4 << 0x10 | 0xf186) &&
          (puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf10e), uVar3 < *puVar1 || uVar3 == *puVar1)))
         ) goto LAB_00019ae0;
    }
    else {
      uVar3 = sat_sub_u16(DAT_0001ee8a,g_tps_load_axis_f10e_prev);
      puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0x13e6);
      if ((*puVar1 <= uVar3 && uVar3 != *puVar1) &&
         ((uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xf114),
          puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0x13e8), *puVar1 <= uVar3 && uVar3 != *puVar1 &&
          (*(ushort *)((uint)bVar4 << 0x10 | 0xf114) < *(ushort *)((uint)bVar4 << 0x10 | 0x13ea)))))
      {
        return 1;
      }
    }
    uVar3 = 2;
  }
  else {
    if ((gear_mode_flags_f510 & 0x2000) != 0) {
      return 1;
    }
LAB_00019ae0:
    uVar3 = 0;
  }
  return uVar3;
}



void o2_heater_fuel_ctrl_top_update(void)

{
  ushort uVar1;
  
  if (DAT_000102eb == '\0') {
    DAT_0001f4dc = 0xff;
    uVar1 = DAT_0000f0ec;
    DAT_0000f0ec = uVar1 | 1;
    uVar1 = sensor_validity_status_flags_f1fe;
    sensor_validity_status_flags_f1fe = uVar1 & 0xffdf;
  }
  else {
    o2_heater_timer_f088_update();
    o2_heater_iat_zone_select_f4f0();
    o2_heater_duty_f4da_calc();
    (*o2_heater_duty_downstream_correction)();
  }
  return;
}



short o2_heater_timer_f088_update(void)

{
  short sTimerVal;
  
  if (((g_status_flags_f20e & 0x11) != 0) || (sTimerVal = 0, DAT_0001f4e6 != 0)) {
    sTimerVal = DAT_00010f08 << 2;
    DAT_0001f088 = sTimerVal;
  }
  if (DAT_0001ef90 == 0) {
    g_status_flags_f1fe = g_status_flags_f1fe & 0xffdf;
  }
  else if (DAT_0001f088 == 0) {
    g_status_flags_f1fe = g_status_flags_f1fe | 0x20;
  }
  return sTimerVal;
}



void o2_heater_iat_zone_select_f4f0(void)

{
  DAT_0001f4f0 = 1;
  if (DAT_00010eea < engine_torque_pct_f17a) {
    DAT_0001f4f0 = 8;
  }
  else if (DAT_00010ee8 < engine_torque_pct_f17a) {
    DAT_0001f4f0 = 4;
  }
  else if (DAT_00010ee6 < engine_torque_pct_f17a) {
    DAT_0001f4f0 = 2;
  }
  if (DAT_00010ef4 < DAT_0001f4de) {
    DAT_0001f4ec = DAT_00010ef0;
  }
  else {
    DAT_0001f4ec = DAT_00010eec;
    if (DAT_00010ef2 < DAT_0001f4de) {
      DAT_0001f4ec = DAT_00010eee;
    }
  }
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: ram
// [VERIFIED 2026-07-22] o2_heater_duty_f4da_calc: writes F4DA (PWM-style duty
// value, clamped 0-0xFF via clamp_u8). Gated by two temperature-threshold state
// bits (F4D8 bit0/bit1, compared against 0x10F02/0x10F04/0x1017A/0x10F00 ROM
// tables), an O2 heater soak-enable check (o2_heater_soak_enable_check), and a
// force-off check (o2_heater_force_off_check, forces F4DA=0xFF when tripped).
// Also sets F046 (mode select) based on F3F0 bit4. This is an actuator output
// (O2 sensor heater duty cycle), NOT a MUT sensor read -- F4DA is not itself a
// MUT ReqID in the 0x2fad0 table, sits just outside the F4Dx/F4Ex cluster
// (ReqID 0x8C-0x93) documented in mut_verification_status.md. Referenced here
// because it was checked as a candidate producer for that cluster and ruled
// out (writes F4DA only, not any of the 8 cluster addresses).

void o2_heater_duty_f4da_calc(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort uVar2;
  int iVar1;
  undefined2 uVar3;
  short sVar4;
  byte bVar5;
  
  bVar5 = 1;
  if ((DAT_0001f4d8 & 1) == 0) {
    if (DAT_00010f02 < inj_pw_output_f186) {
      DAT_0001f4d8 = DAT_0001f4d8 | 1;
    }
  }
  else if (inj_pw_output_f186 <= DAT_00010f04) {
    DAT_0001f4d8 = DAT_0001f4d8 & 0xfffe;
  }
  if ((DAT_0001f4d8 & 2) == 0) {
    if (DAT_00010efe < engine_torque_pct_f17a) {
      DAT_0001f4d8 = DAT_0001f4d8 | 2;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_00010f00) {
    DAT_0001f4d8 = DAT_0001f4d8 & 0xfffd;
  }
  iVar1 = CONCAT22(engine_torque_pct_f17a,param_2);
  if ((DAT_000102ec != '\0') &&
     (iVar1 = CONCAT22(engine_torque_pct_f17a,param_2), engine_torque_pct_f17a <= DAT_00010f46)) {
    iVar1 = (uint)DAT_00010f48 * 0x14;
    uVar2 = (ushort)iVar1;
    if ((g_rpm_zone_dwell_counter_ef6a < uVar2) && ((g_status_flags_f20e & 0x10) == 0)) {
      if ((g_status_flags_f20e & 1) != 0) {
        iVar1 = CONCAT22(g_tps_load_axis_f10e,uVar2);
        if (g_tps_load_axis_f10e <= DAT_00010f42) {
          DAT_0001f4d8 = DAT_0001f4d8 | 0x8000;
          iVar1 = CONCAT22(g_tps_load_axis_f10e,uVar2);
        }
      }
      goto LAB_00019c44;
    }
  }
  DAT_0001f4d8 = DAT_0001f4d8 & 0x7fff;
LAB_00019c44:
  uVar3 = (undefined2)((uint)iVar1 >> 0x10);
  if (DAT_0001f046 == 0) {
    (*clamp_u8)(DAT_0001f4da + DAT_00010efa,(short)iVar1,0xff,DAT_0001f4da + DAT_00010efa,param_3);
    o2_heater_soak_enable_check();
    uVar3 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf4da) = uVar3;
    if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf3f0) & 0x10) == 0) {
      uVar3 = *(undefined2 *)((uint)bVar5 << 0x10 | 0xef6);
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf046) = uVar3;
    }
    else {
      uVar3 = *(undefined2 *)((uint)bVar5 << 0x10 | 0xef8);
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf046) = uVar3;
    }
  }
  sVar4 = (*o2_heater_force_off_check)(uVar3);
  if (sVar4 != 0) {
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf4da) = 0xff;
  }
  return;
}



void o2_heater_duty_downstream_correction(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  (*axis_lookup_interp)();
  table_lookup_interp(1,(void *)0x2dd6);
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf4dc) = uVar1;
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf20e) & 0x10) != 0) {
    uVar1 = 0xff;
    if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf512) & 1) != 0) {
      uVar1 = 0;
    }
    DAT_0000f4f4 = uVar1;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

bool o2_heater_soak_enable_check(void)

{
  bool bVar1;
  
  if (((((DAT_0001f4d8 & 1) == 0) && ((DAT_0001f4d8 & 2) == 0)) &&
      (DAT_00010f06 < g_tps_load_axis_f10e)) &&
     ((((g_status_flags_f20e & 0x11) == 0 &&
       ((ushort)(DAT_00010efc * 0x14) < g_rpm_zone_dwell_counter_ef6a)) &&
      ((g_status_flags_f1fe & 0x20) == 0)))) {
    bVar1 = true;
  }
  else {
    bVar1 = false;
  }
  return bVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

bool o2_heater_force_off_check(void)

{
  bool bVar1;
  
  if ((((g_status_flags_f20e & 0x10) == 0) &&
      ((((DAT_0001f4d8 & 0x8000) != 0 || ((g_status_flags_f20e & 1) == 0)) &&
       ((g_status_flags_f1fe & 0x20) == 0)))) &&
     ((((DAT_000102eb & 2) == 0 || ((g_status_flags_f0f8 & 0x10) == 0)) &&
      ((g_status_flags_f20e & 8) == 0)))) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  return bVar1;
}



void channel_dispatch_and_snapshot_update(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort *puVar1;
  undefined2 uVar2;
  ushort uVar3;
  byte in_ram_00000200;
  undefined2 in_ram_00000206;
  undefined2 in_ram_0000021c;
  undefined2 in_ram_0000021e;
  byte bVar4;
  ushort unaff_retaddr;
  
  uVar2 = *(undefined2 *)((int)(DAT_0001f1fc << 1) + 0x20c);
  bVar4 = 0;
  DAT_0000f5c0 = uVar2;
  DAT_0000f5c2 = in_ram_0000021c;
  DAT_0000f5c4 = in_ram_0000021e;
  DAT_0000f5c6 = in_ram_00000206;
  if ((in_ram_00000200 & 1) != 0) {
    uVar3 = DAT_0000f520;
    if ((uVar3 & 0xf8) == 0) {
      (*eeprom_backup_table_write_dispatch)(0,uVar2,param_3,0);
      if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf84a) & 3) == 3) {
        adc_backup_snapshot_write_if_time_elapsed(0,unaff_retaddr);
      }
      else if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf170) < 0x66) &&
              ((*(ushort *)((uint)bVar4 << 0x10 | 0xf84a) & 0x21) == 0x21)) {
        (*mut_configurable_reqid_backup_snapshot_5word)();
      }
    }
    else {
      DAT_0000f84a = 0;
      ff0d_clear_bits5_6();
    }
  }
  if ((*(byte *)((uint)bVar4 << 0x10 | 0x200) & 2) != 0) {
    (*eeprom_backup_table_write_dispatch)();
    if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf84c) & 3) == 3) {
      adc_backup_snapshot_write_if_time_elapsed(0,unaff_retaddr);
    }
    else if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf170) < 0x66) &&
            ((*(ushort *)((uint)bVar4 << 0x10 | 0xf84c) & 0x21) == 0x21)) {
      (*mut_configurable_reqid_backup_snapshot_5word)();
    }
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0e6) & 1) == 0) {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf5c8);
    *puVar1 = *puVar1 & 0xffef;
  }
  else {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf5c8);
    *puVar1 = *puVar1 | 0x10;
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0ea) & 1) == 0) {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf5c8);
    *puVar1 = *puVar1 & 0xfffd;
  }
  else {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf5c8);
    *puVar1 = *puVar1 | 2;
  }
  return;
}



// [DOCUMENTED 2026-08-07] Dispatcher for the two internal fixed-RequestID
// MUT snapshot loggers (see mut_fixed_reqid_backup_snapshot_5word @0x1504b
// and mut_fixed_reqid_periodic_snapshot_8word @0x14edd). Called only from
// tcu_rx_main_scheduler's periodic tick path -- confirms the whole chain is
// an internal periodic black-box logger, not a live SCI1 request/response
// handler. Selects between "channel 0" (gated by config bit DAT_00010200
// bit0, using F84A flags) and "channel 1" (gated by bit1, using F84C
// flags), each channel independently choosing between the 5-word backup
// snapshot, the 8-word periodic snapshot, or a third handler at 0x14dd8
// (not yet identified) based on bits 2/3/4 of its F84A/F84C flag word.
// See mut_verification_status.md "MAJOR FINDING: NO LIVE WIRE-DRIVEN MUT
// REQUESTID PATH EXISTS ON THIS ROM" (2026-08-07).

void channel_event_or_periodic_dispatch(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  byte bVar1;
  ushort uVar2;
  
  bVar1 = 1;
  if ((DAT_00010200 & 1) != 0) {
    if ((DAT_0001f520 & 0xf8) == 0) {
      (*channel_validity_gated_handler_dispatch)(0,param_2,param_3,0);
    }
    uVar2 = *(ushort *)((uint)bVar1 << 0x10 | 0xf84a);
    if ((uVar2 & 1) != 0) {
      if ((uVar2 & 4) == 0) {
        if ((uVar2 & 8) == 0) {
          if ((uVar2 & 0x10) != 0) {
            (*mut_configurable_reqid_periodic_snapshot_8word)();
          }
          goto LAB_00019eb6;
        }
        uVar2 = 2;
      }
      else {
        uVar2 = 1;
      }
      adc_backup_snapshot_write_if_time_elapsed(0,uVar2);
    }
  }
LAB_00019eb6:
  if ((*(byte *)((uint)bVar1 << 0x10 | 0x200) & 2) != 0) {
    (*channel_validity_gated_handler_dispatch)();
    uVar2 = *(ushort *)((uint)bVar1 << 0x10 | 0xf84c);
    if ((uVar2 & 1) != 0) {
      if ((uVar2 & 4) == 0) {
        if ((uVar2 & 8) == 0) {
          if ((uVar2 & 0x10) == 0) {
            return;
          }
          (*mut_configurable_reqid_periodic_snapshot_8word)();
          return;
        }
        uVar2 = 2;
      }
      else {
        uVar2 = 1;
      }
      adc_backup_snapshot_write_if_time_elapsed(uVar2,param_3);
    }
  }
  return;
}



void channel_data_flush_if_active(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  if ((((DAT_00010200 & 1) != 0) && ((DAT_0001f84a & 1) != 0)) && ((DAT_0001f84a & 0x20) != 0)) {
    (*mut_configurable_reqid_backup_snapshot_5word)();
  }
  uVar1 = *(ushort *)((uint)bVar2 << 0x10 | 0xf84c);
  if ((((*(byte *)((uint)bVar2 << 0x10 | 0x200) & 2) != 0) && ((uVar1 & 1) != 0)) &&
     ((uVar1 & 0x20) != 0)) {
    (*mut_configurable_reqid_backup_snapshot_5word)();
  }
  return;
}



undefined2 __stdcall_far
o2_lambda_or_passthrough_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (DAT_00010201 == -0x80) {
    raw_value = DAT_0001f5b4;
  }
  else {
  }
  return raw_value;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void o2_sensor_gain_divisor(void)

{
  if ((cal_o2_sensor_type == -0x80) && ((DAT_0001f0ec & 2) != 0)) {
    (*div_u16_rounded)();
  }
  return;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping.

undefined2 __stdcall_far
o2_wideband_mode_zero_check(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if ((cal_o2_sensor_type == '\x03') || (cal_o2_sensor_type == '\x04')) {
    raw_value = 0;
  }
  return raw_value;
}



ushort o2_wideband_cell_offset_adjust(ushort param_fp1)

{
  ushort uVar1;
  char extraout_R2;
  ushort in_stack_00000004;
  
  if (((cal_o2_sensor_type != -0x7f) && (cal_o2_sensor_type != -0x7d)) ||
     ((DAT_0001f5b8 & 0xf0) >> 4 != DAT_0001f5e8)) {
    return in_stack_00000004;
  }
  uVar1 = (*muldiv_s16_rounded)();
  if (extraout_R2 == -0x7f) {
    uVar1 = sat_sub_u16(in_stack_00000004,uVar1);
    return uVar1;
  }
  uVar1 = sat_add_u16(in_stack_00000004,uVar1);
  return uVar1;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping.

undefined2 __stdcall_far
o2_wideband_midpoint_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if ((cal_o2_sensor_type == -0x7f) || (cal_o2_sensor_type == -0x7d)) {
    raw_value = 0x80;
  }
  return raw_value;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort o2_lambda_correction_apply(void)

{
  ushort in_stack_00000004;
  
  if (DAT_00010201 == '\x01') {
    in_stack_00000004 = (*mul_fixedpoint_8)();
  }
  else if (DAT_00010201 == '\x02') {
    in_stack_00000004 = DAT_0001f5b4;
  }
  else {
  }
  return in_stack_00000004;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort o2_sensor_mode_correction_select(void)

{
  ushort uVar1;
  ushort in_stack_00000004;
  
  if (DAT_00010202 != '\x11') {
    if (DAT_00010202 == '\x12') {
      return DAT_0001f5b6;
    }
    if ((DAT_00010201 == '\x01') || (DAT_00010201 == '\x02')) {
      return 0x80;
    }
    if (DAT_00010201 != '\x03') {
      if (DAT_00010201 == '\x04') {
        return DAT_0001f5b4;
      }
      return in_stack_00000004;
    }
  }
  uVar1 = (*mul_fixedpoint_8)();
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

bool o2_wideband_lsu_mode_check(void)

{
  bool bVar1;
  
  if (((DAT_00010201 == '\x06') || (DAT_00010201 == '\a')) || (DAT_00010201 == '\b')) {
    bVar1 = true;
  }
  else {
    bVar1 = false;
  }
  return bVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort o2_mode7_8_correction_apply(void)

{
  ushort in_stack_00000004;
  
  if (DAT_00010201 == '\a') {
    in_stack_00000004 = (*mul_fixedpoint_8)();
  }
  else if (DAT_00010201 == '\b') {
    in_stack_00000004 = DAT_0001f5b4;
  }
  else {
  }
  return in_stack_00000004;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void o2_mode9_10_correction_apply(void)

{
  if (DAT_00010201 == '\t') {
    (*mul_fixedpoint_8)();
  }
  return;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping.

undefined2 __stdcall_far o2_mode11_lambda_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (DAT_00010201 == '\v') {
    raw_value = DAT_0001f5b4;
  }
  else {
  }
  return raw_value;
}



undefined2 o2_mode16_lambda_select(void)

{
  undefined2 in_stack_00000004;
  
  if (DAT_00010201 == '\x10') {
    in_stack_00000004 = DAT_0001f5b4;
  }
  else {
  }
  return in_stack_00000004;
}



bool o2_closed_loop_config_check(void)

{
  bool bVar1;
  
  if ((DAT_00010204 == '\x01') ||
     (((DAT_00010201 == '\0' && (DAT_00010202 != '\x11')) && (DAT_00010202 != '\x12')))) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  return bVar1;
}



ushort o2_sensor2_correction_apply(ushort param_fp1)

{
  ushort in_stack_00000004;
  
  if (DAT_00010202 == '\x01') {
    in_stack_00000004 = sat_sub_u16((DAT_0001f5b6 >> 1) + in_stack_00000004,0x40);
  }
  else if (DAT_00010202 == '\x02') {
    in_stack_00000004 = DAT_0001f5b6;
  }
  else {
  }
  return in_stack_00000004;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping.

undefined2 __stdcall_far o2_sensor2_mode3_midpoint(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (DAT_00010202 == '\x03') {
    raw_value = 0x80;
  }
  return raw_value;
}



bool o2_downstream_type34_check(void)

{
  bool bVar1;
  
  if ((cal_o2_sensor_type == '\x03') || (cal_o2_sensor_type == '\x04')) {
    bVar1 = true;
  }
  else {
    bVar1 = false;
  }
  return bVar1;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35), plus an unused acStack_4 artifact.
// Needs manual disasm trace before prototyping.

undefined2 __stdcall_far
o2_downstream_clamp_by_type(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (cal_o2_sensor_type == '\x03') {
    raw_value = 0xff;
  }
  else if (cal_o2_sensor_type == '\x04') {
    raw_value = 0;
  }
  return raw_value;
}



bool o2_wideband_mode82_check(void)

{
  return cal_o2_sensor_type == -0x7e;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping. NOTE: this address collides with o2_downstream_mode20_select (0x1a228) on
// the Evo5 fuzzy-match (both -> Evo5 0x1a292) -- resolve that collision before renaming
// either on Evo5, see functions_evo5.md collision list.

undefined2 __stdcall_far o2_sensor2_mode20_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (DAT_00010202 == ' ') {
    raw_value = DAT_0001f5b6;
  }
  else {
  }
  return raw_value;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern. NOTE: collides with o2_sensor2_mode20_select (0x1a212) on the Evo5 fuzzy-match
// (both -> Evo5 0x1a292) -- confirmed genuinely DIFFERENT functions on 4g63t (this one
// checks DAT_00010203 and returns f5b8; the other checks DAT_00010202 and returns f5b6),
// so on Evo5 either they were merged into one function or the match is a false positive.
// Resolve via decompile-and-compare before renaming either on Evo5.

undefined2 __stdcall_far
o2_downstream_mode20_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (cal_o2_sensor_type == ' ') {
    raw_value = DAT_0001f5b8;
  }
  else {
  }
  return raw_value;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping.

undefined2 __stdcall_far
o2_downstream_mode21_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (cal_o2_sensor_type == '!') {
    raw_value = DAT_0001f5b8;
  }
  else {
  }
  return raw_value;
}



bool o2_config4_mode16_check(void)

{
  return DAT_00010204 == '\x10';
}



bool o2_downstream_mode17_check(void)

{
  return cal_o2_sensor_type == '\x11';
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort o2_downstream_correction_apply(void)

{
  ushort in_stack_00000004;
  
  if (cal_o2_sensor_type == '\x01') {
    in_stack_00000004 = (*mul_fixedpoint_8)();
  }
  else if (cal_o2_sensor_type == '\x02') {
    in_stack_00000004 = DAT_0001f5b8;
  }
  else {
  }
  return in_stack_00000004;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping.

undefined2 __stdcall_far
o2_downstream_mode64_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (cal_o2_sensor_type == '@') {
    raw_value = DAT_0001f5b8;
  }
  else {
  }
  return raw_value;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping.

undefined2 __stdcall_far
o2_downstream_mode66_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (cal_o2_sensor_type == 'B') {
    raw_value = DAT_0001f5b8;
  }
  else {
  }
  return raw_value;
}



undefined2 o2_mode65_lambda_select(void)

{
  undefined2 in_stack_00000004;
  
  if (cal_o2_sensor_type == 'A') {
    in_stack_00000004 = DAT_0001f5b8;
  }
  else {
  }
  return in_stack_00000004;
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- same undeclared in_stack_00000004 passthrough
// pattern as o2_lambda_or_passthrough_select (0x19f35). Needs manual disasm trace before
// prototyping.

undefined2 __stdcall_far
o2_downstream_mode48_select(undefined2 _unused_far_pad,undefined2 raw_value)

{
  if (cal_o2_sensor_type == '0') {
    raw_value = DAT_0001f5b8;
  }
  else {
  }
  return raw_value;
}



void o2_trim_cell_states_reset(void)

{
  tcu_gear_shift_history_reset();
  return;
}



void fuel_trim_full_state_reset(void)

{
  tcu_diagnostic_state_cold_init();
  return;
}



void dtc_and_status_update(void)

{
  status_flags_f202_build();
  (*dtc_status_flags_f204_f206_build)();
  (*tcu_shift_flags_dispatch)();
  return;
}



void status_flags_f202_build(void)

{
  ushort usFlagBits;
  
  usFlagBits = 0;
  if ((g_status_flags_f20e & 8) != 0) {
    usFlagBits = 0x80;
  }
  if ((g_status_flags_001f25a & 0x20) != 0) {
    usFlagBits = usFlagBits | 0x40;
  }
  if ((engine_state_flags_f25e & 0x40) != 0) {
    usFlagBits = usFlagBits | 0x20;
  }
  if (DAT_0001f250 == 0) {
    usFlagBits = usFlagBits | 8;
  }
  if ((DAT_0001f730 & 0xf) != 0xf) {
    usFlagBits = usFlagBits | 2;
  }
  if ((DAT_0001f3dc & 0xf) != 0xf) {
    usFlagBits = usFlagBits | 1;
  }
  DAT_0001f202 = usFlagBits | DAT_0001f202 & 0xff10;
  return;
}



// WARNING: Removing unreachable block (ram,0x0001a484)

ushort dtc_status_flags_f204_f206_build(void)

{
  ushort usDtcBits;
  
  usDtcBits = 0;
  if (((g_iat_axis_f118 <= DAT_00010fd8) || (coolant_temp_scaled_f130 <= DAT_00010fda)) ||
     (DAT_0001f00e != 0)) {
    usDtcBits = 1;
  }
  if (g_tps_load_axis_f10e <= DAT_00010fd4) {
    usDtcBits = usDtcBits | 2;
  }
  if (g_tps_load_axis_f10e <= DAT_00010fd6) {
    usDtcBits = usDtcBits | 4;
  }
  if ((DAT_00010fd0 < g_tps_load_axis_f10e_prev) && (DAT_00010fd2 < DAT_0001f11c)) {
    usDtcBits = usDtcBits | 8;
  }
  if (DAT_0001f500 == 0) {
    usDtcBits = usDtcBits | 0x10;
  }
  if ((g_status_flags_f1fe & 8) != 0) {
    usDtcBits = usDtcBits | 0x20;
  }
  if ((g_status_flags_f1fe & 1) != 0) {
    usDtcBits = usDtcBits | 0x40;
  }
  if ((g_status_flags_f1fe & 2) != 0) {
    usDtcBits = usDtcBits | 0x80;
  }
  if ((g_status_flags_f1fe & 4) != 0) {
    usDtcBits = usDtcBits | 0x100;
  }
  DAT_0001f206 = 0;
  DAT_0001f204 = 0;
  if ((usDtcBits & 0x3e3) != 0) {
    DAT_0001f204 = 0x8000;
    DAT_0001f206 = 0x8000;
  }
  if ((usDtcBits & 0xbe3) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x4000;
  }
  if ((usDtcBits & 0x13e3) != 0) {
    DAT_0001f206 = DAT_0001f206 | 0x4000;
  }
  if ((usDtcBits & 0x3f5) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x80;
  }
  if ((usDtcBits & 0x7e3) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x40;
  }
  if ((usDtcBits & 0x3eb) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x2000;
    DAT_0001f206 = DAT_0001f206 | 0x2000;
  }
  if ((usDtcBits & 0x63e0) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x1000;
    DAT_0001f206 = DAT_0001f206 | 0x1000;
  }
  if ((usDtcBits & 0x3e3) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x20;
  }
  if ((usDtcBits & 0x3e3) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x10;
  }
  if ((usDtcBits & 0x1e3) != 0) {
    DAT_0001f204 = DAT_0001f204 | 4;
  }
  if ((usDtcBits & 0x1e0) != 0) {
    DAT_0001f204 = DAT_0001f204 | 2;
  }
  if ((usDtcBits & 0x3e3) != 0) {
    DAT_0001f204 = DAT_0001f204 | 8;
  }
  if ((usDtcBits & 0xe0) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x100;
  }
  if ((usDtcBits & 0x1e0) != 0) {
    DAT_0001f204 = DAT_0001f204 | 0x800;
    DAT_0001f206 = DAT_0001f206 | 0x800;
  }
  if ((usDtcBits & 0x1e3) != 0) {
    DAT_0001f204 = DAT_0001f204 | 1;
  }
  return usDtcBits & 0x1e0;
}



// WARNING: Control flow encountered bad instruction data
// WARNING: This function may have set the stack pointer

void flash_boot_init(undefined2 param_1)

{
  byte bVar1;
  
  do {
    bVar1 = FLMCR;
  } while ((bVar1 & 0x80) == 0);
  DAT_0000fe7e = 0x1d;
  boot_ram_block_copy(param_1,0xa76);
                    // WARNING: Bad instruction - Truncating control flow here
  halt_baddata();
}



// [INVESTIGATED 2026-07-23] Boot-time RAM block copy: copies words from a
// source pointer (arrives in R4) to a FIXED destination starting at 0xf290,
// until the source pointer reaches an end-address bound (arrives in R1).
// Real signature is effectively:
//   void boot_ram_block_copy(ushort *src_start /* R4 */, ushort *src_end /* R1 */)
// KNOWN LIMITATION: both real parameters arrive in registers (R4, R1), not on
// the stack -- no `link FP`/stack frame exists in this 4-instruction leaf
// function at all. Ghidra's decompiler shows `unaff_R4` (a register it
// can't tie to any parameter) plus a bogus stack-based param_1/param_2 pair
// that don't correspond to anything real. Same class of limitation hit on
// adc_channel_convert_and_read (0x15b0d) -- set_variable_storage's
// programmatic API can't retarget parameter storage to registers on this
// architecture; would need manual Decompiler-window retyping or a custom
// script to fix properly. Left prototype unset rather than commit a
// misleading stack-based signature.

void boot_ram_block_copy(undefined2 param_1,short param_2)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  undefined2 *puVar3;
  undefined2 *unaff_R4;
  
  puVar3 = (undefined2 *)&LAB_0000f290;
  do {
    puVar2 = unaff_R4;
    unaff_R4 = unaff_R4 + 1;
    puVar1 = puVar3;
    puVar3 = puVar3 + 1;
    *puVar1 = *puVar2;
  } while ((short)unaff_R4 - param_2 < 0 !=
           (param_2 < (short)unaff_R4 == (short)unaff_R4 - param_2 < 0));
  return;
}



void boot_main_loop(void)

{
  DAT_0001f20a._1_1_ = 0;
  boot_sci1_init(0x33);
  boot_sci1_receive_command();
  boot_sci1_receive_config_bytes();
  boot_sci1_rom_id_verify();
  sci1_boot_cmd_type_rx_validate();
  do {
    sci1_boot_cmd_dispatch();
  } while( true );
}



void boot_sci1_init(byte bFec9Init)

{
  DAT_0001fec8 = 0;
  DAT_0001fec9 = bFec9Init;
  SCI1_SCR_0001feca = 0x30;
  g_status_flags_f216 = SUB42(&DAT_0000fd00,0);
  DAT_0001f218 = SUB42(&DAT_0000fd00,0);
  return;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00020461)
// WARNING: Removing unreachable block (ram,0x000204a9)
// WARNING: Removing unreachable block (ram,0x00020470)
// WARNING: Removing unreachable block (ram,0x00020472)
// WARNING: Removing unreachable block (ram,0x00020477)
// WARNING: Removing unreachable block (ram,0x000204ae)
// WARNING: Removing unreachable block (ram,0x0002047b)
// WARNING: Removing unreachable block (ram,0x0002047d)
// WARNING: Removing unreachable block (ram,0x00020482)
// WARNING: Removing unreachable block (ram,0x000204b3)
// WARNING: Removing unreachable block (ram,0x00020486)
// WARNING: Removing unreachable block (ram,0x00020488)
// WARNING: Removing unreachable block (ram,0x0002048d)
// WARNING: Removing unreachable block (ram,0x000204b8)
// WARNING: Removing unreachable block (ram,0x00020491)
// WARNING: Removing unreachable block (ram,0x00020493)
// WARNING: Removing unreachable block (ram,0x00020498)
// WARNING: Removing unreachable block (ram,0x0002049c)
// WARNING: Removing unreachable block (ram,0x0002049e)
// WARNING: Removing unreachable block (ram,0x000204a2)
// WARNING: Removing unreachable block (ram,0x000204bd)
// WARNING: Removing unreachable block (ram,0x000204c0)
// WARNING: Removing unreachable block (ram,0x000204c9)

void boot_sci1_receive_command(void)

{
  char cVar1;
  byte bVar2;
  short extraout_R1;
  short sVar3;
  short extraout_R2;
  short sVar4;
  undefined1 *puVar5;
  byte bVar6;
  undefined1 uVar7;
  undefined1 uVar8;
  
  uVar7 = 0;
  bVar6 = 1;
  cVar1 = sci1_rx_byte_wait_loop();
  *(char *)((uint)bVar6 << 0x10 | 0xf200) = cVar1;
  if (cVar1 == '\0') {
    sci1_boot_tx_then_rx_transaction(0x10);
    return;
  }
  sci1_boot_tx_then_rx_transaction(0x61);
  sci1_boot_tx_then_rx_transaction();
  sci1_boot_tx_then_rx_transaction();
  if (*(char *)((uint)bVar6 << 0x10 | 0xf20b) != '\x01') {
    do {
                    // WARNING: Do nothing block with infinite loop
    } while( true );
  }
  bVar2 = 1;
  uVar8 = 0;
  do {
    bVar2 = sci1_rx_byte_poll_or_error(bVar2);
    sVar3 = extraout_R1;
    sVar4 = extraout_R2;
    if ((bool)uVar8) {
      sVar3 = 0x481c;
      sVar4 = 2;
    }
  } while ((sVar3 != 0) || (sVar4 != 0));
  *(undefined1 *)((uint)bVar6 << 0x10 | 0xf20b) = 0;
  *(undefined2 *)&DAT_0000fe7e = 0x40;
  boot_sci1_rom_id_verify();
  *(undefined2 *)&P1DDR = 0x43;
  sci1_boot_cmd_type_rx_validate();
  puVar5 = (undefined1 *)&P3DDR;
  do {
    *(undefined2 *)(puVar5 + -2) = 0x46;
    sci1_boot_cmd_dispatch();
    puVar5 = puVar5 + 2;
  } while( true );
}



void boot_sci1_receive_config_bytes(void)

{
  undefined1 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf201) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf202) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf203) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf204) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf205) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf206) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf207) = 0;
  if ((((DAT_0002fefa != *(char *)((uint)bVar2 << 0x10 | 0xf201)) ||
       (DAT_0002fefb != *(char *)((uint)bVar2 << 0x10 | 0xf202))) ||
      (DAT_0002fefc != *(char *)((uint)bVar2 << 0x10 | 0xf203))) ||
     (((DAT_0002fefd != *(char *)((uint)bVar2 << 0x10 | 0xf204) ||
       (DAT_0002fefe != *(char *)((uint)bVar2 << 0x10 | 0xf205))) ||
      (DAT_0002feff != *(char *)((uint)bVar2 << 0x10 | 0xf206))))) {
    *(undefined1 *)((uint)bVar2 << 0x10 | 0xf207) = 1;
  }
  return;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00020461)
// WARNING: Removing unreachable block (ram,0x000204a9)
// WARNING: Removing unreachable block (ram,0x00020470)
// WARNING: Removing unreachable block (ram,0x00020472)
// WARNING: Removing unreachable block (ram,0x00020477)
// WARNING: Removing unreachable block (ram,0x000204ae)
// WARNING: Removing unreachable block (ram,0x0002047b)
// WARNING: Removing unreachable block (ram,0x0002047d)
// WARNING: Removing unreachable block (ram,0x00020482)
// WARNING: Removing unreachable block (ram,0x000204b3)
// WARNING: Removing unreachable block (ram,0x00020486)
// WARNING: Removing unreachable block (ram,0x00020488)
// WARNING: Removing unreachable block (ram,0x0002048d)
// WARNING: Removing unreachable block (ram,0x000204b8)
// WARNING: Removing unreachable block (ram,0x00020491)
// WARNING: Removing unreachable block (ram,0x00020493)
// WARNING: Removing unreachable block (ram,0x00020498)
// WARNING: Removing unreachable block (ram,0x0002049c)
// WARNING: Removing unreachable block (ram,0x0002049e)
// WARNING: Removing unreachable block (ram,0x000204a2)
// WARNING: Removing unreachable block (ram,0x000204bd)
// WARNING: Removing unreachable block (ram,0x000204c0)
// WARNING: Removing unreachable block (ram,0x000204c9)

void boot_sci1_rom_id_verify(void)

{
  undefined2 uVar1;
  byte bVar2;
  char *pcVar3;
  short extraout_R1;
  short sVar4;
  short extraout_R2;
  short sVar5;
  undefined1 *puVar6;
  byte bVar7;
  undefined1 uVar8;
  undefined1 uVar9;
  
  uVar8 = 0;
  bVar7 = 1;
  uVar1 = sci1_rx_byte_wait_loop();
  *(char *)((uint)bVar7 << 0x10 | 0xf208) = (char)uVar1;
  pcVar3 = (char *)&DAT_0000f394;
  while( true ) {
    bVar2 = pcVar3[1];
    if (*pcVar3 == '\0') {
      sci1_boot_tx_then_rx_transaction(0x62);
      sci1_boot_tx_then_rx_transaction();
      sci1_boot_tx_then_rx_transaction();
      if (*(char *)((uint)bVar7 << 0x10 | 0xf20b) != '\x01') {
        do {
                    // WARNING: Do nothing block with infinite loop
        } while( true );
      }
      bVar2 = 1;
      uVar9 = 0;
      do {
        bVar2 = sci1_rx_byte_poll_or_error(bVar2);
        sVar4 = extraout_R1;
        sVar5 = extraout_R2;
        if ((bool)uVar9) {
          sVar4 = 0x481c;
          sVar5 = 2;
        }
      } while ((sVar4 != 0) || (sVar5 != 0));
      *(undefined1 *)((uint)bVar7 << 0x10 | 0xf20b) = 0;
      *(undefined2 *)&DAT_0000fe7e = 0x40;
      boot_sci1_rom_id_verify();
      *(undefined2 *)&P1DDR = 0x43;
      sci1_boot_cmd_type_rx_validate();
      puVar6 = (undefined1 *)&P3DDR;
      do {
        *(undefined2 *)(puVar6 + -2) = 0x46;
        sci1_boot_cmd_dispatch();
        puVar6 = puVar6 + 2;
      } while( true );
    }
    if (*pcVar3 == (char)uVar1) break;
    pcVar3 = pcVar3 + 2;
  }
  sci1_boot_tx_then_rx_transaction(uVar1);
  do {
  } while ((*(byte *)((uint)bVar7 << 0x10 | 0xfecc) & 4) == 0);
  boot_sci1_init(bVar2);
  return;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00020461)
// WARNING: Removing unreachable block (ram,0x000204a9)
// WARNING: Removing unreachable block (ram,0x00020470)
// WARNING: Removing unreachable block (ram,0x00020472)
// WARNING: Removing unreachable block (ram,0x00020477)
// WARNING: Removing unreachable block (ram,0x000204ae)
// WARNING: Removing unreachable block (ram,0x0002047b)
// WARNING: Removing unreachable block (ram,0x0002047d)
// WARNING: Removing unreachable block (ram,0x00020482)
// WARNING: Removing unreachable block (ram,0x000204b3)
// WARNING: Removing unreachable block (ram,0x00020486)
// WARNING: Removing unreachable block (ram,0x00020488)
// WARNING: Removing unreachable block (ram,0x0002048d)
// WARNING: Removing unreachable block (ram,0x000204b8)
// WARNING: Removing unreachable block (ram,0x00020491)
// WARNING: Removing unreachable block (ram,0x00020493)
// WARNING: Removing unreachable block (ram,0x00020498)
// WARNING: Removing unreachable block (ram,0x0002049c)
// WARNING: Removing unreachable block (ram,0x0002049e)
// WARNING: Removing unreachable block (ram,0x000204a2)
// WARNING: Removing unreachable block (ram,0x000204bd)
// WARNING: Removing unreachable block (ram,0x000204c0)
// WARNING: Removing unreachable block (ram,0x000204c9)

void sci1_boot_cmd_type_rx_validate(void)

{
  undefined2 uVar1;
  char cVar2;
  byte bVar3;
  short extraout_R1;
  short sVar4;
  short extraout_R2;
  short sVar5;
  undefined1 *puVar6;
  byte bVar7;
  undefined1 uVar8;
  undefined1 uVar9;
  
  uVar8 = 0;
  bVar7 = 1;
  uVar1 = sci1_rx_byte_wait_loop();
  cVar2 = (char)uVar1;
  *(char *)((uint)bVar7 << 0x10 | 0xf209) = cVar2;
  if (((cVar2 == '\x06') || (cVar2 == '\t')) || (cVar2 == '\f')) {
    sci1_boot_tx_then_rx_transaction(uVar1);
    return;
  }
  sci1_boot_tx_then_rx_transaction(0xbb);
  sci1_boot_tx_then_rx_transaction();
  sci1_boot_tx_then_rx_transaction();
  if (*(char *)((uint)bVar7 << 0x10 | 0xf20b) != '\x01') {
    do {
                    // WARNING: Do nothing block with infinite loop
    } while( true );
  }
  bVar3 = 1;
  uVar9 = 0;
  do {
    bVar3 = sci1_rx_byte_poll_or_error(bVar3);
    sVar4 = extraout_R1;
    sVar5 = extraout_R2;
    if ((bool)uVar9) {
      sVar4 = 0x481c;
      sVar5 = 2;
    }
  } while ((sVar4 != 0) || (sVar5 != 0));
  *(undefined1 *)((uint)bVar7 << 0x10 | 0xf20b) = 0;
  *(undefined2 *)&DAT_0000fe7e = 0x40;
  boot_sci1_rom_id_verify();
  *(undefined2 *)&P1DDR = 0x43;
  sci1_boot_cmd_type_rx_validate();
  puVar6 = (undefined1 *)&P3DDR;
  do {
    *(undefined2 *)(puVar6 + -2) = 0x46;
    sci1_boot_cmd_dispatch();
    puVar6 = puVar6 + 2;
  } while( true );
}


/*
Unable to decompile 'sci1_boot_cmd_dispatch'
Cause: Low-level Error: Unable to force merge of op at 0x0002042c:761
*/


// WARNING: Unable to track spacebase fully for stack
// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00020461)
// WARNING: Removing unreachable block (ram,0x000204a9)
// WARNING: Removing unreachable block (ram,0x00020470)
// WARNING: Removing unreachable block (ram,0x00020472)
// WARNING: Removing unreachable block (ram,0x00020477)
// WARNING: Removing unreachable block (ram,0x000204ae)
// WARNING: Removing unreachable block (ram,0x0002047b)
// WARNING: Removing unreachable block (ram,0x0002047d)
// WARNING: Removing unreachable block (ram,0x00020482)
// WARNING: Removing unreachable block (ram,0x000204b3)
// WARNING: Removing unreachable block (ram,0x00020486)
// WARNING: Removing unreachable block (ram,0x00020488)
// WARNING: Removing unreachable block (ram,0x0002048d)
// WARNING: Removing unreachable block (ram,0x000204b8)
// WARNING: Removing unreachable block (ram,0x00020491)
// WARNING: Removing unreachable block (ram,0x00020493)
// WARNING: Removing unreachable block (ram,0x00020498)
// WARNING: Removing unreachable block (ram,0x0002049c)
// WARNING: Removing unreachable block (ram,0x0002049e)
// WARNING: Removing unreachable block (ram,0x000204a2)
// WARNING: Removing unreachable block (ram,0x000204bd)
// WARNING: Removing unreachable block (ram,0x000204c0)
// WARNING: Removing unreachable block (ram,0x000204c9)
// [PROTOTYPE PASS 2026-07-24] SKIPPED -- complex boot function with "Unable to track
// spacebase fully"/"may have set the stack pointer" warnings, several removed unreachable
// blocks, and an infinite do-nothing tail loop. Too risky to prototype/retype without a
// manual disasm trace first; likely related to review.md item 10's spacebase-warning
// cluster. Left as-is.

void sci1_boot_security_seed_check(undefined2 param_1)

{
  byte bVar1;
  short extraout_R1;
  short sVar2;
  short extraout_R2;
  short sVar3;
  undefined1 *puVar4;
  byte bVar5;
  undefined1 uVar6;
  undefined1 uVar7;
  
  uVar6 = 0;
  bVar5 = 1;
  if ((((((char)DAT_0001f200 == -0x65) && (DAT_0001f202._0_1_ == -0x14)) &&
       ((char)DAT_0001f202 == '+')) &&
      ((DAT_0001f204._0_1_ == -0x75 && ((char)DAT_0001f204 == -0x2c)))) &&
     (DAT_0001f206._0_1_ == -0x7a)) {
    sci1_boot_tx_then_rx_transaction(param_1);
    return;
  }
  sci1_boot_tx_then_rx_transaction(0xbc);
  sci1_boot_tx_then_rx_transaction();
  sci1_boot_tx_then_rx_transaction();
  if (*(char *)((uint)bVar5 << 0x10 | 0xf20b) != '\x01') {
    do {
                    // WARNING: Do nothing block with infinite loop
    } while( true );
  }
  bVar1 = 1;
  uVar7 = 0;
  do {
    bVar1 = sci1_rx_byte_poll_or_error(bVar1);
    sVar2 = extraout_R1;
    sVar3 = extraout_R2;
    if ((bool)uVar7) {
      sVar2 = 0x481c;
      sVar3 = 2;
    }
  } while ((sVar2 != 0) || (sVar3 != 0));
  *(undefined1 *)((uint)bVar5 << 0x10 | 0xf20b) = 0;
  *(undefined2 *)&DAT_0000fe7e = 0x40;
  boot_sci1_rom_id_verify();
  *(undefined2 *)&P1DDR = 0x43;
  sci1_boot_cmd_type_rx_validate();
  puVar4 = (undefined1 *)&P3DDR;
  do {
    *(undefined2 *)(puVar4 + -2) = 0x46;
    sci1_boot_cmd_dispatch();
    puVar4 = puVar4 + 2;
  } while( true );
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00020224)
// WARNING: Removing unreachable block (ram,0x0002023b)
// WARNING: Removing unreachable block (ram,0x0002023e)
// WARNING: Removing unreachable block (ram,0x0002024a)
// WARNING: Removing unreachable block (ram,0x0002024d)
// WARNING: Removing unreachable block (ram,0x0002025b)
// WARNING: Removing unreachable block (ram,0x00020278)
// WARNING: Removing unreachable block (ram,0x0002028c)
// WARNING: Removing unreachable block (ram,0x0002027c)
// WARNING: Removing unreachable block (ram,0x00020268)
// WARNING: Removing unreachable block (ram,0x000202a1)
// WARNING: Removing unreachable block (ram,0x00020258)
// WARNING: Removing unreachable block (ram,0x00020461)
// WARNING: Removing unreachable block (ram,0x000204a9)
// WARNING: Removing unreachable block (ram,0x00020470)
// WARNING: Removing unreachable block (ram,0x00020472)
// WARNING: Removing unreachable block (ram,0x00020477)
// WARNING: Removing unreachable block (ram,0x000204ae)
// WARNING: Removing unreachable block (ram,0x0002047b)
// WARNING: Removing unreachable block (ram,0x0002047d)
// WARNING: Removing unreachable block (ram,0x00020482)
// WARNING: Removing unreachable block (ram,0x000204b3)
// WARNING: Removing unreachable block (ram,0x00020486)
// WARNING: Removing unreachable block (ram,0x00020488)
// WARNING: Removing unreachable block (ram,0x0002048d)
// WARNING: Removing unreachable block (ram,0x000204b8)
// WARNING: Removing unreachable block (ram,0x00020491)
// WARNING: Removing unreachable block (ram,0x00020493)
// WARNING: Removing unreachable block (ram,0x00020498)
// WARNING: Removing unreachable block (ram,0x0002049c)
// WARNING: Removing unreachable block (ram,0x0002049e)
// WARNING: Removing unreachable block (ram,0x000204a2)
// WARNING: Removing unreachable block (ram,0x000204bd)
// WARNING: Removing unreachable block (ram,0x000204c0)
// WARNING: Removing unreachable block (ram,0x000204c9)

void sci1_boot_flash_erase_dispatch(void)

{
  undefined2 uVar1;
  short sVar2;
  byte bVar3;
  short extraout_R1;
  short extraout_R2;
  short sVar4;
  undefined1 *puVar5;
  byte bVar6;
  undefined1 uVar7;
  undefined1 uVar8;
  
  uVar7 = 0;
  bVar6 = 1;
  if ((char)DAT_0001f208 == '\x06') {
    uVar1 = 0xf0ff;
  }
  else if ((char)DAT_0001f208 == '\t') {
    uVar1 = 0xf00;
  }
  else {
    uVar1 = 0xffff;
  }
  sVar2 = flash_erase_sectors(uVar1);
  if (sVar2 == 0) {
    sci1_boot_tx_then_rx_transaction(0xc1);
    return;
  }
  if (sVar2 == 1) {
    uVar1 = 0x65;
  }
  else {
    uVar1 = 100;
  }
  sci1_boot_tx_then_rx_transaction(uVar1);
  sci1_boot_tx_then_rx_transaction();
  sci1_boot_tx_then_rx_transaction();
  if (*(char *)((uint)bVar6 << 0x10 | 0xf20b) != '\x01') {
    do {
                    // WARNING: Do nothing block with infinite loop
    } while( true );
  }
  bVar3 = 1;
  uVar8 = 0;
  do {
    bVar3 = sci1_rx_byte_poll_or_error(bVar3);
    sVar2 = extraout_R1;
    sVar4 = extraout_R2;
    if ((bool)uVar8) {
      sVar2 = 0x481c;
      sVar4 = 2;
    }
  } while ((sVar2 != 0) || (sVar4 != 0));
  *(undefined1 *)((uint)bVar6 << 0x10 | 0xf20b) = 0;
  *(undefined2 *)&DAT_0000fe7e = 0x40;
  boot_sci1_rom_id_verify();
  *(undefined2 *)&P1DDR = 0x43;
  sci1_boot_cmd_type_rx_validate();
  puVar5 = (undefined1 *)&P3DDR;
  do {
    *(undefined2 *)(puVar5 + -2) = 0x46;
    sci1_boot_cmd_dispatch();
    puVar5 = puVar5 + 2;
  } while( true );
}



// [PROTOTYPE PASS 2026-07-24] SKIPPED -- two real pointer params arrive in R4/R5
// (unaff_R4 = start ptr, unaff_R5 = end ptr for a byte-range checksum loop), not on
// the stack. Same register-parameter limitation as adc_channel_convert_and_read/
// boot_ram_block_copy (see functions_w4a51.md) -- set_function_prototype can't retarget
// register storage on this architecture. Left as void(void).

void sci1_boot_checksum_accumulate(void)

{
  byte *unaff_R4;
  byte *unaff_R5;
  
  while( true ) {
    DAT_0001f214 = DAT_0001f214 + (ushort)*unaff_R4;
    if (unaff_R5 == unaff_R4) break;
    unaff_R4 = unaff_R4 + 1;
  }
  return;
}



void sci1_boot_flash_write_param_rx(void)

{
  undefined1 uVar1;
  char extraout_R2;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf20c) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf20e) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf20f) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf20d) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf210) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  uVar1 = sci1_rx_byte_wait_loop();
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xf211) = uVar1;
  sci1_boot_tx_then_rx_transaction();
  sci1_boot_flash_addr_range_validate
            (*(undefined1 *)((uint)bVar2 << 0x10 | 0xf20c),
             *(undefined2 *)((uint)bVar2 << 0x10 | 0xf20e),
             *(undefined1 *)((uint)bVar2 << 0x10 | 0xf20d));
  if (extraout_R2 == '\0') {
    return;
  }
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}



void scratch_pjmp_demo(void)

{
                    // WARNING: Could not recover jumptable at 0x00020377. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)(uint)CONCAT12(DAT_0001f274,DAT_0001f270))();
  return;
}



void sci1_tx_byte_blocking(uchar byte_to_send)

{
  do {
  } while ((DAT_0001fecc & 0x80) == 0);
  DAT_0001fecb = byte_to_send;
  DAT_0001fecc = DAT_0001fecc & 0x7f;
  return;
}



void sci1_boot_tx_then_rx_transaction(uchar param_1)

{
  sci1_tx_byte_blocking(param_1);
  sci1_rx_byte_wait_loop();
  return;
}



void sci1_rx_byte_wait_loop(void)

{
  byte bVar1;
  undefined1 in_CF;
  
  sci1_boot_rx_byte_enqueue();
  bVar1 = sci1_boot_rxbuf_read();
  while (!(bool)in_CF) {
    bVar1 = sci1_rx_byte_poll_or_error(bVar1);
  }
  return;
}



void sci1_boot_rx_byte_enqueue(byte param_1)

{
  undefined1 in_CF;
  
  sci1_rx_byte_poll_or_error(param_1);
  if ((bool)in_CF) {
    sci1_boot_rxbuf_write();
  }
  return;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00020461)
// WARNING: Removing unreachable block (ram,0x000204a9)
// WARNING: Removing unreachable block (ram,0x00020470)
// WARNING: Removing unreachable block (ram,0x00020472)
// WARNING: Removing unreachable block (ram,0x00020477)
// WARNING: Removing unreachable block (ram,0x000204ae)
// WARNING: Removing unreachable block (ram,0x0002047b)
// WARNING: Removing unreachable block (ram,0x0002047d)
// WARNING: Removing unreachable block (ram,0x00020482)
// WARNING: Removing unreachable block (ram,0x000204b3)
// WARNING: Removing unreachable block (ram,0x00020486)
// WARNING: Removing unreachable block (ram,0x00020488)
// WARNING: Removing unreachable block (ram,0x0002048d)
// WARNING: Removing unreachable block (ram,0x000204b8)
// WARNING: Removing unreachable block (ram,0x00020491)
// WARNING: Removing unreachable block (ram,0x00020493)
// WARNING: Removing unreachable block (ram,0x00020498)
// WARNING: Removing unreachable block (ram,0x0002049c)
// WARNING: Removing unreachable block (ram,0x0002049e)
// WARNING: Removing unreachable block (ram,0x000204a2)
// WARNING: Removing unreachable block (ram,0x000204bd)
// WARNING: Removing unreachable block (ram,0x000204c0)
// WARNING: Removing unreachable block (ram,0x000204c9)

void sci1_boot_rxbuf_write(undefined1 param_1,undefined2 param_2,undefined2 param_3)

{
  byte bVar1;
  short extraout_R1;
  short sVar2;
  short extraout_R2;
  short sVar3;
  undefined1 *puVar4;
  byte bVar5;
  undefined1 uVar6;
  undefined1 uVar7;
  
  uVar6 = 0;
  bVar5 = 1;
  *g_status_flags_f216 = param_1;
  g_status_flags_f216 =
       (undefined1 *)
       CONCAT11((char)((ushort)g_status_flags_f216 >> 8),(char)g_status_flags_f216 + '\x01');
  if ((undefined1 *)DAT_0001f218 != g_status_flags_f216) {
    return;
  }
  sci1_boot_tx_then_rx_transaction(0x65,g_status_flags_f216,DAT_0001f218,param_3);
  sci1_boot_tx_then_rx_transaction();
  sci1_boot_tx_then_rx_transaction();
  if (*(char *)((uint)bVar5 << 0x10 | 0xf20b) != '\x01') {
    do {
                    // WARNING: Do nothing block with infinite loop
    } while( true );
  }
  bVar1 = 1;
  uVar7 = 0;
  do {
    bVar1 = sci1_rx_byte_poll_or_error(bVar1);
    sVar2 = extraout_R1;
    sVar3 = extraout_R2;
    if ((bool)uVar7) {
      sVar2 = 0x481c;
      sVar3 = 2;
    }
  } while ((sVar2 != 0) || (sVar3 != 0));
  *(undefined1 *)((uint)bVar5 << 0x10 | 0xf20b) = 0;
  *(undefined2 *)&DAT_0000fe7e = 0x40;
  boot_sci1_rom_id_verify();
  *(undefined2 *)&P1DDR = 0x43;
  sci1_boot_cmd_type_rx_validate();
  puVar4 = (undefined1 *)&P3DDR;
  do {
    *(undefined2 *)(puVar4 + -2) = 0x46;
    sci1_boot_cmd_dispatch();
    puVar4 = puVar4 + 2;
  } while( true );
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar sci1_boot_rxbuf_read(void)

{
  uchar uVar1;
  
  uVar1 = '\0';
  if (DAT_0001f218 != g_status_flags_f216) {
    uVar1 = *DAT_0001f218;
    DAT_0001f218 = (uchar *)CONCAT11((char)((ushort)DAT_0001f218 >> 8),(char)DAT_0001f218 + '\x01');
  }
  return uVar1;
}



byte sci1_rx_byte_poll_or_error(byte param_1)

{
  byte bVar1;
  
  if ((DAT_0001fecc & 0x10) != 0) {
    bVar1 = sci1_boot_handshake_and_bulk_rx(0xa1);
    return bVar1;
  }
  if ((DAT_0001fecc & 0x20) != 0) {
    bVar1 = sci1_boot_handshake_and_bulk_rx(0xa3);
    return bVar1;
  }
  if ((DAT_0001fecc & 8) != 0) {
    bVar1 = sci1_boot_handshake_and_bulk_rx(0xa2);
    return bVar1;
  }
  if ((DAT_0001fecc & 0x40) != 0) {
    DAT_0001fecc = DAT_0001fecc & 0xbf;
    param_1 = DAT_0001fecd;
  }
  return param_1;
}



void sci1_boot_tx_buf_drain(char param_1,undefined1 *param_2)

{
  undefined1 *extraout_R1;
  char extraout_R2;
  undefined1 uVar1;
  
  uVar1 = 1;
  do {
    sci1_boot_tx_then_rx_transaction(*param_2,param_2 + 1,param_1);
    param_1 = extraout_R2 + -1;
    param_2 = extraout_R1;
  } while (extraout_R2 != '\x01');
  return;
}



// WARNING: Unable to track spacebase fully for stack
// WARNING: This function may have set the stack pointer
// WARNING: Removing unreachable block (ram,0x00020461)
// WARNING: Removing unreachable block (ram,0x000204a9)
// WARNING: Removing unreachable block (ram,0x00020470)
// WARNING: Removing unreachable block (ram,0x00020472)
// WARNING: Removing unreachable block (ram,0x00020477)
// WARNING: Removing unreachable block (ram,0x000204ae)
// WARNING: Removing unreachable block (ram,0x0002047b)
// WARNING: Removing unreachable block (ram,0x0002047d)
// WARNING: Removing unreachable block (ram,0x00020482)
// WARNING: Removing unreachable block (ram,0x000204b3)
// WARNING: Removing unreachable block (ram,0x00020486)
// WARNING: Removing unreachable block (ram,0x00020488)
// WARNING: Removing unreachable block (ram,0x0002048d)
// WARNING: Removing unreachable block (ram,0x000204b8)
// WARNING: Removing unreachable block (ram,0x00020491)
// WARNING: Removing unreachable block (ram,0x00020493)
// WARNING: Removing unreachable block (ram,0x00020498)
// WARNING: Removing unreachable block (ram,0x0002049c)
// WARNING: Removing unreachable block (ram,0x0002049e)
// WARNING: Removing unreachable block (ram,0x000204a2)
// WARNING: Removing unreachable block (ram,0x000204bd)
// WARNING: Removing unreachable block (ram,0x000204c0)
// WARNING: Removing unreachable block (ram,0x000204c9)
// HIGH VALUE, 161 call sites. Boot-handshake function with two genuine non-return exit paths (not
// decompiler bugs): (1) if the handshake byte @0xf20b never becomes 1, it spins forever at 0x2043d
// (real infinite loop -- correctly flagged); (2) on successful handshake, unconditional `bra
// 0x2003d`
// tail-jump at 0x2045e into boot_main_loop (0x20030), not a `rts`.
// 
// BOUNDARY FIXED 2026-07-24: this function's body previously extended to 0x204cd, incorrectly
// absorbing ~0x6d bytes of unrelated dead code (0x20461-0x204cd) that had zero real inbound
// references from this function's actual control flow. That code has been split off into its own
// function, sci1_boot_flash_addr_range_validate (0x20461) -- a flash-write address-range validator,
// likely reached via indirect dispatch rather than a direct call. Body now correctly ends at
// 0x20460,
// right after the tail-jump. No stack params to bind (void is correct) -- the earlier decompiler
// mess
// was a structural boundary problem, now resolved, not a typing problem.

void sci1_boot_handshake_and_bulk_rx(void)

{
  byte bVar1;
  short extraout_R1;
  short sVar2;
  short extraout_R2;
  short sVar3;
  undefined1 *puVar4;
  byte bVar5;
  undefined1 uVar6;
  undefined1 uVar7;
  
  uVar6 = 0;
  bVar5 = 1;
  sci1_boot_tx_then_rx_transaction();
  sci1_boot_tx_then_rx_transaction();
  sci1_boot_tx_then_rx_transaction();
  if (*(char *)((uint)bVar5 << 0x10 | 0xf20b) != '\x01') {
    do {
                    // WARNING: Do nothing block with infinite loop
    } while( true );
  }
  bVar1 = 1;
  uVar7 = 0;
  do {
    bVar1 = sci1_rx_byte_poll_or_error(bVar1);
    sVar2 = extraout_R1;
    sVar3 = extraout_R2;
    if ((bool)uVar7) {
      sVar2 = 0x481c;
      sVar3 = 2;
    }
  } while ((sVar2 != 0) || (sVar3 != 0));
  *(undefined1 *)((uint)bVar5 << 0x10 | 0xf20b) = 0;
  *(undefined2 *)&DAT_0000fe7e = 0x40;
  boot_sci1_rom_id_verify();
  *(undefined2 *)&P1DDR = 0x43;
  sci1_boot_cmd_type_rx_validate();
  puVar4 = (undefined1 *)&P3DDR;
  do {
    *(undefined2 *)(puVar4 + -2) = 0x46;
    sci1_boot_cmd_dispatch();
    puVar4 = puVar4 + 2;
  } while( true );
}



// WARNING: This function may have set the stack pointer

void sci1_boot_flash_write_block_loop
               (undefined2 param_1,undefined2 param_2,undefined1 param_3,char *param_4)

{
  byte *pbVar1;
  char *pcVar2;
  byte bVar3;
  ushort uVar4;
  short sVar5;
  undefined2 uVar6;
  undefined2 extraout_R1;
  ushort param2;
  undefined2 unaff_R3;
  undefined2 unaff_R5;
  byte bVar7;
  undefined2 unaff_retaddr;
  undefined2 uVar8;
  char *in_stack_00000002;
  
  bVar7 = 1;
  DAT_0001f26c = (char)param_1;
  DAT_0001f268 = param_2;
  DAT_0001f26a = unaff_R3;
  DAT_0001f26d = param_3;
  uVar4 = sci1_boot_build_row_addr_frame
                    (param_1,DAT_0001f26c,(char *)CONCAT22(unaff_R5,unaff_retaddr));
  sci1_boot_tx_buf_drain(uVar4,(short)&DAT_0000f21a);
  while( true ) {
    while( true ) {
      uVar4 = *(ushort *)((uint)bVar7 << 0x10 | 0xf268);
      bVar3 = *(byte *)((uint)bVar7 << 0x10 | 0xf26c);
      if (0xffdf < uVar4) break;
      pbVar1 = (byte *)((uint)bVar7 << 0x10 | 0xf26d);
      if ((*pbVar1 <= bVar3) &&
         ((bVar3 != *pbVar1 || (*(ushort *)((uint)bVar7 << 0x10 | 0xf26a) <= uVar4 + 0x20))))
      goto LAB_00020577;
      sVar5 = sci1_boot_build_data_frame
                        (CONCAT11(*(undefined1 *)((uint)bVar7 << 0x10 | 0xf26c),0x20),
                         in_stack_00000002,*(ushort *)((uint)bVar7 << 0x10 | 0xf268));
      *(undefined2 *)((uint)bVar7 << 0x10 | 0xf268) = extraout_R1;
      sci1_boot_tx_buf_drain(sVar5,(short)&DAT_0000f21a);
    }
    if (*(byte *)((uint)bVar7 << 0x10 | 0xf26d) <= bVar3) break;
    uVar4 = *(ushort *)((uint)bVar7 << 0x10 | 0xf268);
    sVar5 = sci1_boot_build_data_frame
                      (CONCAT11(*(undefined1 *)((uint)bVar7 << 0x10 | 0xf26c),-(char)uVar4),
                       in_stack_00000002,uVar4);
    uVar6 = sci1_boot_tx_buf_drain(sVar5,(short)&DAT_0000f21a);
    *(undefined2 *)((uint)bVar7 << 0x10 | 0xf268) = 0;
    pcVar2 = (char *)((uint)bVar7 << 0x10 | 0xf26c);
    *pcVar2 = *pcVar2 + '\x01';
    uVar8 = 0x56f;
    uVar4 = sci1_boot_build_row_addr_frame(uVar6,*(char *)((uint)bVar7 << 0x10 | 0xf26c),param_4);
    in_stack_00000002 = (char *)CONCAT22(uVar8,0x575);
    sci1_boot_tx_buf_drain(uVar4,(short)&DAT_0000f21a);
  }
LAB_00020577:
  uVar4 = *(ushort *)((uint)bVar7 << 0x10 | 0xf268);
  sVar5 = sci1_boot_build_data_frame
                    (CONCAT11(*(undefined1 *)((uint)bVar7 << 0x10 | 0xf26c),
                              ((char)*(undefined2 *)((uint)bVar7 << 0x10 | 0xf26a) - (char)uVar4) +
                              '\x01'),in_stack_00000002,uVar4);
  uVar4 = sci1_boot_tx_buf_drain(sVar5,(short)&DAT_0000f21a);
  uVar4 = sci1_boot_build_eof_frame(uVar4,param2,0xf21a);
  sci1_boot_tx_buf_drain(uVar4,(short)&DAT_0000f21a);
  return;
}



// WARNING: Unable to track spacebase fully for stack

short sci1_boot_build_data_frame(ushort cmd_and_len,char *payload,ushort tx_offset)

{
  char *pcVar1;
  char *pcVar2;
  undefined1 *in_R2;
  char *pcVar3;
  char *pcVar4;
  char cVar5;
  char cVar6;
  bool bVar7;
  
  *in_R2 = 0x3a;
  cVar5 = (char)cmd_and_len;
  in_R2[1] = cVar5;
  cVar6 = (char)(tx_offset >> 8);
  in_R2[2] = cVar6;
  cVar6 = cVar5 + cVar6 + (char)tx_offset;
  in_R2[3] = (char)tx_offset;
  in_R2[4] = 0;
  pcVar3 = in_R2 + 5;
  do {
    pcVar4 = pcVar3;
    pcVar1 = (char *)tx_offset;
    tx_offset = tx_offset + 1;
    cVar6 = cVar6 + *pcVar1;
    *pcVar4 = *pcVar1;
    bVar7 = cVar5 != -1;
    cVar5 = cVar5 + -1;
    pcVar3 = pcVar4 + 1;
  } while (bVar7);
  pcVar3 = pcVar4 + 2;
  pcVar4[1] = -cVar6;
  pcVar4 = (char *)&DAT_0000f8c1;
  cVar5 = '\x02';
  do {
    pcVar2 = pcVar4;
    pcVar4 = pcVar4 + 1;
    pcVar1 = pcVar3;
    pcVar3 = pcVar3 + 1;
    *pcVar1 = *pcVar2;
    bVar7 = cVar5 != -1;
    cVar5 = cVar5 + -1;
  } while (bVar7);
  return (cmd_and_len & 0xff) + 8;
}



ushort sci1_boot_build_row_addr_frame(undefined2 param_1,char param_2,char *param_3)

{
  char *pcVar1;
  char *pcVar2;
  char *in_R2;
  char *pcVar3;
  char *pcVar4;
  char cVar5;
  bool bVar6;
  
  pcVar3 = (char *)&DAT_0000f8a0;
  cVar5 = '\x05';
  do {
    pcVar4 = in_R2;
    pcVar1 = pcVar3;
    pcVar3 = pcVar3 + 1;
    *pcVar4 = *pcVar1;
    bVar6 = cVar5 != -1;
    cVar5 = cVar5 + -1;
    in_R2 = pcVar4 + 1;
  } while (bVar6);
  cVar5 = DAT_0001f8a5 + param_2 * '\x10';
  pcVar4[1] = param_2 * '\x10';
  pcVar4[2] = '\0';
  pcVar3 = pcVar4 + 4;
  pcVar4[3] = -cVar5;
  pcVar4 = (char *)&DAT_0000f8c1;
  cVar5 = '\x02';
  do {
    pcVar2 = pcVar4;
    pcVar4 = pcVar4 + 1;
    pcVar1 = pcVar3;
    pcVar3 = pcVar3 + 1;
    *pcVar1 = *pcVar2;
    bVar6 = cVar5 != -1;
    cVar5 = cVar5 + -1;
  } while (bVar6);
  return 10;
}



// WARNING: Control flow encountered bad instruction data

undefined2 switchD_00028b50::sci1_boot_switch_case4_frag_20640(void)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined2 unaff_R4;
  char cVar4;
  short unaff_FP;
  bool bVar5;
  bool in_NF;
  undefined1 *unaff_retaddr;
  
                    // [2026-08-16] Investigated the halt_baddata() decompiler warning here. NOT a
                    // truncated-function
                    //    bug (unlike the sci1_boot_cmd_dispatch fix from 2026-08-09) -- this is a
                    // genuinely tiny,
                    //    correctly-bounded 6-byte function (0x20640-0x20645: bpl/nop/ldm.w), a
                    // switch-case landing pad
                    //    for the jump table in sci1_post_handshake_status_broadcast_dispatch
                    // (switchD_00028b50), reached
                    //    via a live COMPUTED_JUMP xref from 0x28b50.
                    // 
                    //    Root cause of halt_baddata(): the `bpl` tests the N flag from context set
                    // up before the jump;
                    //    if plus, branches to 0x20843 (real code, outside this function). If minus,
                    // falls through with
                    //    NO explicit branch/rts through `nop; ldm.w @SP+,(R2)` directly into the
                    // next byte, which is the
                    //    entry point of the adjacent, separately-named, already-correct function
                    //    sci1_boot_build_eof_frame (0x20646-0x2065a, called normally elsewhere from
                    //    sci1_boot_flash_write_block_loop @0x20599). The decompiler cannot cleanly
                    // express a
                    //    fall-through-into-another-function's-entry-point control flow shape, so it
                    // synthesizes a
                    //    phantom `if(in_NF)` branch (pulling in sci1_boot_build_eof_frame's body)
                    // for the taken path and
                    //    halt_baddata()s on the untaken path instead of showing the real
                    // fallthrough.
                    // 
                    //    CONCLUSION: benign, intentional size-optimized H8 code (shared
                    // micro-epilogue immediately
                    //    preceding the next routine). No fix applied -- correcting this would
                    // require manually
                    //    overriding the decompiler's control-flow model for a 3-byte fallthrough,
                    // which isn't worth the
                    //    risk of miscategorizing genuinely different code elsewhere. Left as-is;
                    // the halt_baddata() can
                    //    be safely ignored/dismissed if it resurfaces in a bookmark/error scan.
  if (in_NF) {
    puVar3 = (undefined1 *)&DAT_0000f8bb;
    cVar4 = '\b';
    do {
      puVar2 = puVar3;
      puVar3 = puVar3 + 1;
      puVar1 = unaff_retaddr;
      unaff_retaddr = unaff_retaddr + 1;
      *puVar1 = *puVar2;
      bVar5 = cVar4 != -1;
      cVar4 = cVar4 + -1;
    } while (bVar5);
    return 8;
  }
  *(undefined2 *)(unaff_FP + 0x1d) = unaff_R4;
                    // WARNING: Bad instruction - Truncating control flow here
  halt_baddata();
}



ushort sci1_boot_build_eof_frame(ushort param1,ushort param2,ushort tx_offset)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  char cVar4;
  bool bVar5;
  
  puVar3 = (undefined1 *)&DAT_0000f8bb;
  cVar4 = '\b';
  do {
    puVar2 = puVar3;
    puVar3 = puVar3 + 1;
    puVar1 = (undefined1 *)tx_offset;
    tx_offset = tx_offset + 1;
    *puVar1 = *puVar2;
    bVar5 = cVar4 != -1;
    cVar4 = cVar4 + -1;
  } while (bVar5);
  return 8;
}



// WARNING: Removing unreachable block (ram,0x00020779)

byte sci1_colon_frame_dispatch(byte param_1)

{
  char cVar1;
  byte bVar2;
  
  bVar2 = 1;
  DAT_0001f27d = 0;
  DAT_0001f281 = 0;
  DAT_0001f276 = 0;
  DAT_0001f278 = 0;
  DAT_0001f27a = 0;
  DAT_0001f26e = 0;
  DAT_0001f270 = 0;
  DAT_0001f272 = 0;
  DAT_0001f274 = 0;
  DAT_0001f275 = 0;
  DAT_0001f27c = param_1;
  while (cVar1 = sci1_rx_byte_wait_loop(), *(char *)((uint)bVar2 << 0x10 | 0xf27d) == '\0') {
    if (cVar1 == ':') {
      *(undefined1 *)((uint)bVar2 << 0x10 | 0xf27d) = 1;
      *(undefined1 *)((uint)bVar2 << 0x10 | 0xf280) = 0;
    }
  }
  *(char *)((uint)bVar2 << 0x10 | 0xf280) = *(char *)((uint)bVar2 << 0x10 | 0xf280) + cVar1;
                    // 2026-08-09: DP traced to reset (entry@0x1517c: ldc.b 0x0,DP, written once
                    // unconditionally, never touched again on the SCI1 bootloader path -- see
                    // sci1_boot_cmd_dispatch plate comment). With DP=0, spSegment(DP,X)=X for both
                    // this load's EA and the 0x206cb jmp target. Real table read addresses for the
                    // three actual callers: param_1=0->0xfb2c, param_1=1->0xf92e,
                    // param_1=2->0xf930. All three are valid RAM addresses (RAM seg 0xee80-0xffff)
                    // but genuinely unbacked in the static image and never written by any code in
                    // this ROM (confirmed via get_xrefs_to) -- table contents are
                    // runtime/external-SCI1-link-populated, not statically recoverable.
                    // Investigation closed at this ceiling; see review9.md follow-up sessions for
                    // full trace.
                    // WARNING: Could not recover jumptable at 0x000206cb. Too many branches
                    // WARNING: Treating indirect jump as call
                    // TRIED: p-code emulation (EmulatorHelper) to resolve this jump table at
                    // runtime -- fails with 'Unimplemented CALLOTHER pcodeop (spSegment)' on the
                    // EP-relative load at 0x206c3 itself. This is the H8/539F segmented/banked
                    // addressing primitive, which Ghidra's stock emulator does not implement.
                    // Emulation is not viable here without writing a custom spSegment CALLOTHER
                    // handler AND still guessing the real runtime EP/DP bank value -- not attempted
                    // further. Static byte-pattern search for 0xf92e/0xda0/0xfb2a/0xfb2c also came
                    // up empty (see review notes). Table base genuinely unrecoverable without live
                    // hardware/real trace data.
  bVar2 = (*(code *)((short)&UNK_0000f260 +
                    *(short *)((int)((ushort)(byte)(*(char *)((uint)bVar2 << 0x10 | 0xf27d) - 1) <<
                                    1) + -0x6d2)))();
  return bVar2;
}



// WARNING: Removing unreachable block (ram,0x00020779)
// TRIAGE: no direct callers found - almost certainly reached via an indirect/computed jump table
// (decompiler explicitly warns "Could not recover jumptable at 0x206cb, treating indirect jump as
// call"). Sits between sci1_colon_frame_dispatch and sci1_boot_ihex_data_byte_store in the
// bootloader command area. Loops reading bytes via sci1_rx_byte_wait_loop() until a ':'
// start-of-record marker is seen (or a field-count flag at f27d is already set), accumulates a
// checksum-like byte at f280, then dispatches through a jump table indexed by (f27d-1) to a byte
// offset computed from a small lookup table at -0x6d2 in bank1. Consistent with parsing/dispatching
// Intel-HEX record fields over the bootloader serial link. Confidence is moderate - verify the jump
// table contents manually in the listing.

undefined2 sci1_boot_ihex_colon_record_decode_dispatch(undefined1 param_1,undefined2 param_2)

{
  char cVar2;
  undefined2 uVar1;
  byte bVar3;
  
  bVar3 = 1;
  *(undefined1 *)((int)param_2 + -0xd88) = param_1;
  DAT_0001f27e = (char)param_2 + 1;
  if (DAT_0001f27e < 3) {
    bVar3 = (byte)((ushort)DAT_0001f278 >> 8);
    DAT_0001f278 = DAT_0001f278 << 4;
    DAT_0001f27a = (((bVar3 >> 7) << 1 | (bVar3 & 0x7f) >> 6) << 1 | (bVar3 & 0x3f) >> 5) << 1 |
                   (bVar3 & 0x1f) >> 4;
    DAT_0001f27d = DAT_0001f27d + '\x01';
    uVar1 = sci1_colon_frame_dispatch_next_record();
    return uVar1;
  }
  while (cVar2 = sci1_rx_byte_wait_loop(), *(char *)((uint)bVar3 << 0x10 | 0xf27d) == '\0') {
    if (cVar2 == ':') {
      *(undefined1 *)((uint)bVar3 << 0x10 | 0xf27d) = 1;
      *(undefined1 *)((uint)bVar3 << 0x10 | 0xf280) = 0;
    }
  }
  *(char *)((uint)bVar3 << 0x10 | 0xf280) = *(char *)((uint)bVar3 << 0x10 | 0xf280) + cVar2;
                    // WARNING: Could not recover jumptable at 0x000206cb. Too many branches
                    // WARNING: Treating indirect jump as call
  uVar1 = (*(code *)((short)&UNK_0000f260 +
                    *(short *)((int)((ushort)(byte)(*(char *)((uint)bVar3 << 0x10 | 0xf27d) - 1) <<
                                    1) + -0x6d2)))();
  return uVar1;
}



// WARNING: Removing unreachable block (ram,0x00020779)
// Re-entrant continuation of sci1_colon_frame_dispatch (0x20664) for the
// next colon-prefixed IHEX record: increments the frame counter (0xf27d)
// then repeats the same colon-wait loop and re-dispatches through the same
// DP-banked jump table at -0x6d2 (0x206c3/0x206cb -- see that function's
// plate/decompiler comments for the 2026-08-09 DP=0 trace and resolved
// table addresses, which apply identically here). Called from within
// sci1_colon_frame_dispatch's own jump-table body and from
// sci1_boot_ihex_colon_record_decode_dispatch -- this is how the SCI1
// bootloader processes each subsequent ':'-framed IHEX line after the
// first.

undefined1 sci1_colon_frame_dispatch_next_record(void)

{
  char cVar1;
  undefined1 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  DAT_0001f27d = DAT_0001f27d + '\x01';
  while (cVar1 = sci1_rx_byte_wait_loop(), *(char *)((uint)bVar3 << 0x10 | 0xf27d) == '\0') {
    if (cVar1 == ':') {
      *(undefined1 *)((uint)bVar3 << 0x10 | 0xf27d) = 1;
      *(undefined1 *)((uint)bVar3 << 0x10 | 0xf280) = 0;
    }
  }
  *(char *)((uint)bVar3 << 0x10 | 0xf280) = *(char *)((uint)bVar3 << 0x10 | 0xf280) + cVar1;
                    // WARNING: Could not recover jumptable at 0x000206cb. Too many branches
                    // WARNING: Treating indirect jump as call
  uVar2 = (*(code *)((short)&UNK_0000f260 +
                    *(short *)((int)((ushort)(byte)(*(char *)((uint)bVar3 << 0x10 | 0xf27d) - 1) <<
                                    1) + -0x6d2)))();
  return uVar2;
}



// WARNING: Unable to track spacebase fully for stack

void f26e_cluster_accumulator_unrelated_to_ihex(byte param_1)

{
  byte bVar1;
  ushort uVar2;
  byte *pbVar3;
  
  uVar2 = DAT_0001f27e + DAT_0001f276;
  pbVar3 = (byte *)(uVar2 + DAT_0001f278);
  bVar1 = DAT_0001f27a + CARRY2((ushort)DAT_0001f27e,DAT_0001f276) + CARRY2(uVar2,DAT_0001f278);
  DAT_0001f26e = DAT_0001f26e + (ushort)param_1;
  if (DAT_0001f27c == '\0') {
    if (((byte)(bVar1 + 1) < 2) && (1 < (byte)(bVar1 + 1))) {
      flash_vpp_pulse_handshake_echo_verify((ushort)param_1,0xffff);
    }
  }
  else if (DAT_0001f27c == '\x01') {
    *pbVar3 = param_1;
    if ((DAT_0001f281 & 1) == 0) {
      DAT_0001f281 = DAT_0001f281 | 1;
      DAT_0001f270 = pbVar3;
      DAT_0001f274 = bVar1;
    }
    if ((DAT_0001f275 <= bVar1) && ((bVar1 != DAT_0001f275 || (DAT_0001f272 <= pbVar3)))) {
      DAT_0001f272 = pbVar3;
      DAT_0001f275 = bVar1;
    }
  }
  return;
}



void flash_erase_sectors(undefined2 param_1)

{
  short sVar1;
  byte bVar2;
  
  bVar2 = 1;
  DAT_0001ff14 = 0;
  sVar1 = 10;
  DAT_0001fee0 = 0x40;
  do {
    sVar1 = sVar1 + -1;
  } while (sVar1 != -1);
  DAT_0001fee2 = param_1;
  flash_erase_sector_loop();
  if (*(short *)((uint)bVar2 << 0x10 | 0xfee2) != 0) {
    flash_write_or_verify(*(undefined2 *)((uint)bVar2 << 0x10 | 0xfee2));
  }
  sVar1 = 100;
  *(undefined1 *)((uint)bVar2 << 0x10 | 0xfee0) = 0;
  do {
    sVar1 = sVar1 + -1;
  } while (sVar1 != -1);
  return;
}



short flash_write_or_verify(undefined2 param_1)

{
  short sVar1;
  ushort uVar2;
  short extraout_R1;
  short sVar3;
  byte bVar4;
  
  bVar4 = 1;
  DAT_0001fee2 = param_1;
  sVar1 = flash_write_marked_blocks_loop();
  if (sVar1 == 0) {
    uVar2 = 1;
    sVar1 = 0x15b2;
    do {
      sVar3 = sVar1;
      if (uVar2 < 4) {
        sVar3 = sVar1 * 2;
      }
      flash_timing_delay(sVar1);
      flash_erase_sector_loop();
      if (*(short *)((uint)bVar4 << 0x10 | 0xfee2) == 0) {
        sVar1 = 0;
        goto LAB_0002087f;
      }
      uVar2 = extraout_R1 + 1;
      sVar1 = sVar3;
    } while (uVar2 < 0x25b);
    sVar1 = 2;
  }
LAB_0002087f:
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xfee2) = 0;
  return sVar1;
}



// TRIAGE: no direct callers found (likely reached indirectly from the flash-write command path near
// flash_write_or_verify/flash_byte_program_verify). Iterates a 16-bit mask at fee2; for each set
// bit, walks a per-block {byte,count} record and calls flash_byte_program_verify() in a loop over
// that block's byte range, returning early on nonzero (error) status. Reads as "write/verify every
// flash block flagged in the fee2 bitmask". Verify bit-to-block mapping manually before relying on
// this name.

ushort flash_write_marked_blocks_loop(void)

{
  ushort uVar1;
  undefined2 *puVar2;
  undefined2 *extraout_R2;
  undefined2 unaff_R3;
  ushort uVar3;
  undefined2 unaff_R4;
  uchar param1;
  ushort uVar4;
  byte bVar5;
  char *verify_addr;
  
  bVar5 = 1;
  verify_addr = (char *)CONCAT22(unaff_R3,unaff_R4);
  puVar2 = (undefined2 *)&DAT_0000fc74;
  uVar3 = 0;
  do {
    if ((*(ushort *)((uint)bVar5 << 0x10 | 0xfee2) & 1 << uVar3) != 0) {
      param1 = (uchar)*puVar2;
      uVar4 = puVar2[1];
      while( true ) {
        uVar1 = flash_byte_program_verify(param1,verify_addr);
        if (uVar1 != 0) {
          return uVar1;
        }
        puVar2 = extraout_R2;
        if ((ushort)extraout_R2[2] <= uVar4) break;
        uVar4 = uVar4 + 1;
      }
    }
    puVar2 = puVar2 + 3;
    uVar3 = uVar3 + 1;
  } while (uVar3 < 0x10);
  return 0;
}



ushort flash_byte_program_verify(uchar param1,char *verify_addr)

{
  char *in_R1;
  ushort uVar1;
  short sVar2;
  
  uVar1 = 1;
  while( true ) {
    *in_R1 = '\0';
    sVar2 = 0x31;
    do {
      sVar2 = sVar2 + -1;
    } while (sVar2 != -1);
    DAT_0001fee0 = 0x40;
    DAT_0001ff10 = 0xa500;
    sVar2 = 8;
    do {
      sVar2 = sVar2 + -1;
    } while (sVar2 != -1);
    if (*in_R1 == '\0') break;
    uVar1 = uVar1 + 1;
    if (0x10 < uVar1) {
      return 1;
    }
  }
  DAT_0001fee0 = 0x40;
  DAT_0001ff10 = 0xa500;
  return 0;
}



void flash_timing_delay(short delay_count)

{
  short sVar1;
  
  do {
    delay_count = delay_count + -1;
  } while (delay_count != -1);
  DAT_0001fee0 = 0x40;
  DAT_0001ff10 = 0xa500;
  sVar1 = 8;
  do {
    sVar1 = sVar1 + -1;
  } while (sVar1 != -1);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void flash_erase_sector_loop(void)

{
  undefined2 uVar1;
  undefined2 *puVar2;
  char *pcVar3;
  short sVar4;
  short sVar5;
  
  sVar4 = 8;
  do {
    sVar4 = sVar4 + -1;
  } while (sVar4 != -1);
  puVar2 = (undefined2 *)&DAT_0000fc74;
  sVar4 = 0;
  do {
    if ((DAT_0001fee2 & 1 << sVar4) != 0) {
      uVar1 = *puVar2;
      pcVar3 = (char *)puVar2[1];
      while( true ) {
        sVar5 = 4;
        *pcVar3 = -1;
        do {
          sVar5 = sVar5 + -1;
        } while (sVar5 != -1);
        if (*pcVar3 != -1) goto LAB_00020978;
        if ((char *)puVar2[2] <= pcVar3) break;
        pcVar3 = pcVar3 + 1;
      }
      DAT_0001fee2 = DAT_0001fee2 & ~(1 << sVar4);
    }
LAB_00020978:
    puVar2 = puVar2 + 3;
    sVar4 = sVar4 + 1;
    if ((byte)sVar4 < 0xf) {
      sVar4 = 8;
      DAT_0001fee0 = 0x40;
      do {
        sVar4 = sVar4 + -1;
      } while (sVar4 != -1);
      return;
    }
  } while( true );
}



// TRIAGE: sole caller is sci1_boot_ihex_data_byte_store, so this runs once per received ihex data
// byte during flash programming. Toggles a control register at bank1 offset fee0 through a
// 0x40/0x41/0x44 sequence (fixed delay loops between each write - pattern typical of a
// programming-voltage/strobe pin sequence), sends the byte via sci1_boot_rx_byte_enqueue-style
// echo, and compares the echoed byte against the original up to 0x10 retries, returning true
// (failure) if the retry count is exceeded. Named for the strobe-and-verify behavior; the "Vpp"
// assumption should be checked against the H8/539F flash programming section of the hardware
// manual.

bool flash_vpp_pulse_handshake_echo_verify(char param_1,undefined2 param_2,undefined1 param_3)

{
  ushort uVar1;
  ushort extraout_R2;
  char *unaff_R3;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar1 = 1;
  DAT_0001fee2 = param_2;
  *unaff_R3 = param_1;
  while( true ) {
    sVar2 = 0x2f;
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xff10) = 0xa57a;
    *(undefined1 *)((uint)bVar3 << 0x10 | 0xfee0) = 0x41;
    if ((*(byte *)((uint)bVar3 << 0x10 | 0xfecc) & 0x40) == 0) {
      do {
        sVar2 = sVar2 + -1;
      } while (sVar2 != -1);
    }
    else {
      param_1 = sci1_boot_rx_byte_enqueue();
      sVar2 = 0x1a;
      do {
        sVar2 = sVar2 + -1;
        uVar1 = extraout_R2;
      } while (sVar2 != -1);
    }
    *(undefined1 *)((uint)bVar3 << 0x10 | 0xfee0) = 0x40;
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xff10) = 0xa500;
    sVar2 = 8;
    *(undefined1 *)((uint)bVar3 << 0x10 | 0xfee0) = 0x44;
    do {
      sVar2 = sVar2 + -1;
    } while (sVar2 != -1);
    if ((param_1 == *unaff_R3) || (uVar1 = uVar1 + 1, 0x10 < uVar1)) break;
    sVar2 = 8;
    *(undefined1 *)((uint)bVar3 << 0x10 | 0xfee0) = 0x40;
    do {
      sVar2 = sVar2 + -1;
    } while (sVar2 != -1);
  }
  sVar2 = 8;
  *(undefined1 *)((uint)bVar3 << 0x10 | 0xfee0) = 0x40;
  do {
    sVar2 = sVar2 + -1;
  } while (sVar2 != -1);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xfee2) = 0;
  return 0x10 < uVar1;
}



void main_loop(void)

{
  short sVar1;
  byte bVar2;
  
  cold_init_dispatch();
  do {
    bVar2 = 0;
    f516_hibits_f520_f0f2_mode_select();
    (*status_word_consumer_periodic)();
    channel_dispatch_and_snapshot_update();
    main_loop_engine_control_update();
    main_control_loop_update();
    engine_periodic_correction_master_dispatch();
    isc_f33e_periodic_dispatch_group_b();
    isc_stepper_position_dispatch();
    (*periodic_task_dispatcher)();
    (*engine_warmup_periodic_dispatch)();
    dtc_and_status_update();
    mirror_status_f0e6_to_ports();
    serial_fef9_duty_scale_and_set();
    sVar1 = *(short *)((uint)bVar2 << 0x10 | 0xff46);
    *(short *)((uint)bVar2 << 0x10 | 0xf0de) = sVar1 - *(short *)((uint)bVar2 << 0x10 | 0xf0e0);
    *(short *)((uint)bVar2 << 0x10 | 0xf0e0) = sVar1;
  } while( true );
}



void cold_init_dispatch(void)

{
  byte bVar1;
  
  bVar1 = 1;
  peripheral_cold_init_wrapper_set_ef7a();
  gear_state_config_loader_f1fc();
  subsystem_cold_init();
  (*subsystem_unconditional_init_chain)();
  if (*(short *)((uint)bVar1 << 0x10 | 0xef98) != 0) {
    (*toggle_flag_fe8b_bit4)();
  }
  return;
}



void subsystem_cold_init(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = check_cold_init_complete_sentinel_ef62();
  if (uVar1 != 0) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xef62) = 0xa5a5;
    stub_empty_0x1519e();
    init_ee82_ee8a_from_rom_const();
    tcu_gear_ratio_state_init();
    inj_channel_state_init();
    init_state_block_ee98_80_call_wrapper();
    octane_level_init_or_force_from_gear_table();
    isc_stepper_position_reinit();
    stub_empty_0x17300();
    startup_phase_reset_wrapper();
    o2_trim_cell_states_reset();
    stub_empty_0x28f80();
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xef62) = 0x5a5a;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort check_cold_init_complete_sentinel_ef62(void)

{
  short sVar1;
  byte bVar2;
  
  bVar2 = 1;
  if ((((((g_knock_octane_trim_ee98 < DAT_00010912) || (DAT_00010910 < g_knock_octane_trim_ee98)) ||
        (DAT_0001ee9a < DAT_00010912)) ||
       (((DAT_00010910 < DAT_0001ee9a || (DAT_0001ee9c < DAT_00010912)) ||
        ((DAT_00010910 < DAT_0001ee9c ||
         ((DAT_0001eeca < DAT_00010c72 || (DAT_00010c70 < DAT_0001eeca)))))))) ||
      (((DAT_0001eecc < DAT_00010c72 ||
        ((((((DAT_00010c70 < DAT_0001eecc || (DAT_0001eece < DAT_00010c72)) ||
            (DAT_00010c70 < DAT_0001eece)) ||
           ((sVar1 = (*swap_invert_high_byte)(), sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeed4)
            || (sVar1 = (*swap_invert_high_byte)(),
               sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeed6))))) ||
          ((sVar1 = (*swap_invert_high_byte)(), sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeed8) ||
           ((sVar1 = (*swap_invert_high_byte)(), sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeeda)
            || (sVar1 = (*swap_invert_high_byte)(),
               sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeedc))))))) ||
         (sVar1 = (*swap_invert_high_byte)(), sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeede)))))
       || (((sVar1 = (*swap_invert_high_byte)(), sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeee0)
            || (sVar1 = (*swap_invert_high_byte)(),
               sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeee2))) ||
           (sVar1 = (*swap_invert_high_byte)(), sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeee4))))
       ))) || (((sVar1 = (*swap_invert_high_byte)(),
                sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeee6) ||
                (sVar1 = (*swap_invert_high_byte)(),
                sVar1 != *(short *)((uint)bVar2 << 0x10 | 0xeee8))) ||
               (*(short *)((uint)bVar2 << 0x10 | 0xef62) != 0x5a5a)))) {
    return 1;
  }
  return 0;
}



void subsystem_unconditional_init_chain(void)

{
  byte bVar1;
  
  bVar1 = 1;
  zero_ram_range(0xef64,0xf974);
  gear_state_config_loader_f1fc();
  (*adc_read_sequence_main)();
  adc_channel_read_and_store_f16e();
  (*adc_read_sequence_b)();
  (*adc_read_channel_6)();
  identity_passthrough_u16_b();
  propagate_flag_fe96_to_f0d8_db();
  (*compose_status_word_f102)();
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf104) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xf102);
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf106) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xf102);
  (*compose_status_words_f0f6_fc)();
  noop_return();
  post_init_status_compose_and_ef98_signal();
  (*throttle_idle_state_cold_init)();
  (*ecu_openloop_state_reset)();
  (*f2f0_f2f8_ring_and_f27x_f2cc_periodic_init)();
  tcu_slip_state_block_cold_init();
  isc_stepper_mode_reinit_dispatch();
  engine_state_f498_init();
  warmup_state_f594_f0e6_init();
  fuel_trim_full_state_reset();
  o2_wideband_state_cold_init();
  init_copy_const_block_via_memcpy_banked();
  return;
}



void peripheral_cold_init_wrapper_set_ef7a(void)

{
  byte bVar1;
  
  bVar1 = 1;
  peripheral_register_cold_init();
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xef7a) = 0x14;
  return;
}



void status_word_consumer_periodic(void)

{
  byte bVar1;
  
  bVar1 = 1;
  toggle_flag_fe8b_bit4();
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xef7a) = 0x14;
  peripheral_register_warm_init();
  if (*(short *)((uint)bVar1 << 0x10 | 0xefa2) != 0) {
    if (*(short *)((uint)bVar1 << 0x10 | 0xef90) != 0) goto LAB_00020dc6;
  }
  init_state_block_ff21();
  (*init_state_block_f97a)();
  init_state_sentinels_f5d2();
  o2_wideband_state_warm_init();
  bVar1 = 0;
LAB_00020dc6:
  if ((*(short *)((uint)bVar1 << 0x10 | 0xefa4) == 0) &&
     ((*(short *)((uint)bVar1 << 0x10 | 0xefa2) == 0 ||
      ((*(ushort *)((uint)bVar1 << 0x10 | 0xf0f8) & 0x40) == 0)))) {
    (*timer4_oc_arm_from_flags)();
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void init_ee82_ee8a_from_rom_const(void)

{
  DAT_0001ee82 = 0x80;
  DAT_0001ee8a = DAT_00010f68;
  return;
}



void post_init_status_compose_and_ef98_signal(void)

{
  short sVar1;
  byte bVar2;
  
  bVar2 = 1;
  DAT_0001ef6c = 0xffff;
  g_rpm_zone_dwell_counter_ef6a = 0xffff;
  DAT_0001f10c = DAT_0001f10a;
  calc_f110_f10e_via_table();
  calc_f118_via_table();
  (*compose_status_word_f0f8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf0fa) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf0f8);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf100) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf0fe);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xef90) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xbfe);
  sVar1 = (*check_flag_fe96_bit7_clear)();
  if (sVar1 != 0) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xef98) = 8;
  }
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf29c) = 0x80;
  (*table_select_scale_fanout_f1be_f1c2)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xef92) = *(undefined2 *)((uint)bVar2 << 0x10 | 0x9c8);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf22a) = *(undefined2 *)((uint)bVar2 << 0x10 | 0x89a);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf22c) = *(undefined2 *)((uint)bVar2 << 0x10 | 0x89a);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf184) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xaea);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf154) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf13c);
  return;
}



void table_select_scale_fanout_f1be_f1c2(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  short sVar1;
  undefined2 uVar2;
  byte bVar3;
  undefined4 uVar4;
  
  bVar3 = 1;
  if ((g_status_flags_f0f8 & 0x20) == 0) {
    uVar4 = CONCAT22(0xda8c,param_3);
  }
  else {
    uVar4 = CONCAT22(0xda6c,param_3);
  }
  table_read_indexed((byte)((uint)uVar4 >> 0x18),(void *)((uint)uVar4 >> 8));
  sVar1 = (*clamp_u8)();
  *(short *)((uint)bVar3 << 0x10 | 0xf1be) = sVar1 << 3;
  uVar2 = (*mul_u16_x2_sat)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1c2) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1b6) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1be);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1bc) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1be);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1c4) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1c2);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1c6) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1c2);
  return;
}



void main_loop_engine_control_update(void)

{
  gear_state_config_loader_f1fc();
  (*cond_reset_and_latch_f10e_f118_prev)();
  (*periodic_status_compose_and_stall_check)();
  (*sensor_adc_update)();
  throttle_target_ramp_update();
  (*throttle_body_update)();
  lambda_enable_flag_update();
  fueling_lambda_update();
  fueling_wideband_output_scale();
  fueling_coolant_inj_offset_calc();
  (*rpm_load_zone_counter_update)();
  return;
}



// gear_state_config_loader_f1fc: derives a 3-bit TCU gear-config index (F1FA) from
// F15A/F15B, which is then used to select values from ROM lookup tables (base 0x222,
// 0x262, 0x272, 0x282) driving F1FC/F1F0/F1F2/F1F4.
// 
// MUT SWEEP FINDING (2026-07-15): F15A/F15B (MUT ReqID 0x3B) is CONFIRMED DEAD for
// MUT-reading purposes. This function calls zero_var_f15a() (0x156c2) and then
// immediately reads the same word back - no other function anywhere in the ROM writes
// F15A (confirmed via whole-image byte-pattern search, only 2 hits total: the zero and
// this read). So F1FA is always 0 here; the gear-config selection is structurally a
// no-op / always picks index 0. Flagged in mut_verification_status.md as the same class
// of finding as the confirmed-dead MUT_83/0xF0BB cell - do not log MUT ReqID 0x3B.
// Open question (not investigated this session): whether this is legacy/vestigial code
// from an earlier ROM revision that did write F15A, or an intentional always-index-0
// simplification.

void gear_state_config_loader_f1fc(void)

{
  short sVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  zero_var_f15a();
  uVar2 = *(ushort *)((uint)bVar3 << 0x10 | 0xf15a);
  sVar1 = 4;
  do {
    uVar2 = uVar2 >> 1;
    if (uVar2 == 0) break;
    sVar1 = sVar1 + -1;
  } while (sVar1 != -1);
  *(ushort *)((uint)bVar3 << 0x10 | 0xf1fa) = uVar2 & 7;
  if (*(short *)((uint)bVar3 << 0x10 | 0x20a) == 0) {
    uVar2 = *(ushort *)((uint)bVar3 << 0x10 | 0x208);
  }
  else {
    uVar2 = *(ushort *)((int)(*(short *)((uint)bVar3 << 0x10 | 0xf1fa) << 1) + 0x222);
    bVar3 = 0;
  }
  if (7 < uVar2) {
    uVar2 = 0;
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf1fc) = uVar2;
  DAT_0000f1f0 = *(undefined2 *)((int)(*(short *)((uint)bVar3 << 0x10 | 0xf1fc) << 1) + 0x262);
  sVar1 = DAT_0000f1fc;
  DAT_0000f1f2 = *(undefined2 *)((int)(sVar1 << 1) + 0x272);
  sVar1 = DAT_0000f1fc;
  DAT_0000f1f4 = *(undefined2 *)((int)(sVar1 << 1) + 0x282);
  DAT_0000f1f8 = 0;
  DAT_0000f1f6 = 0;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort cond_reset_and_latch_f10e_f118_prev(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f0f8 & 0x40) != 0) {
    DAT_0001ef6c = 0;
  }
  if ((g_status_flags_f20e & 1) != 0) {
    g_rpm_zone_dwell_counter_ef6a = 0;
  }
  uVar1 = 0;
  if ((g_status_flags_f20e & 0x11) != 0) {
    DAT_0001ef70 = 0;
    g_tps_load_axis_f10e_prev = g_tps_load_axis_f10e;
    DAT_0001f11c = g_iat_axis_f118;
    uVar1 = g_iat_axis_f118;
  }
  return uVar1;
}



void periodic_status_compose_and_stall_check(void)

{
  byte bVar1;
  
  bVar1 = 1;
  (*compose_status_words_f0f6_fc)();
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf0fa) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xf0f8);
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf100) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xf0fe);
  (*compose_status_word_f0f8)();
  crank_stall_detect_update();
  return;
}



void compose_status_word_f0f8(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  
  bVar4 = 1;
  uVar3 = DAT_0001f0f6;
  if (DAT_000102c6 == '\0') {
    uVar3 = DAT_0001f0f6 & 0xbbff;
  }
  if ((DAT_000102ed & 2) == 0) {
    uVar3 = uVar3 | 0x2000;
  }
  if (DAT_000102c3 == '\0') {
    uVar3 = uVar3 & 0xfdff;
  }
  uVar2 = DAT_0001f0fc;
  if (DAT_000102e5 == '\0') {
    uVar2 = DAT_0001f0fc & 0xffef;
  }
  if ((g_status_flags_f20e & 0x11) == 0) {
    if (DAT_0001ef6c < DAT_00010bf8) goto LAB_00021043;
  }
  else {
LAB_00021043:
    uVar3 = uVar3 | 0x20;
  }
  if (g_tps_load_axis_f10e <= DAT_00010bfc) {
    uVar3 = uVar3 & 0xfff7;
  }
  if ((egr_valve_state_flags_f490 & 0x100) == 0) {
    if (DAT_00010dca < g_tps_load_axis_f110) {
      egr_valve_state_flags_f490 = egr_valve_state_flags_f490 | 0x100;
    }
  }
  else if (g_tps_load_axis_f110 <= DAT_00010dc8) {
    egr_valve_state_flags_f490 = egr_valve_state_flags_f490 & 0xfeff;
  }
  uVar1 = g_status_flags_f20e & 0x11;
  if ((g_status_flags_f20e & 0x11) == 0) {
    uVar1 = g_rpm_zone_dwell_counter_ef6a;
    if ((DAT_00010bfa <= g_rpm_zone_dwell_counter_ef6a) &&
       ((egr_valve_state_flags_f490 & 0x100) == 0)) goto LAB_00021091;
  }
  uVar3 = uVar3 & 0xffef;
LAB_00021091:
  if (DAT_000102c7 == '\x02') {
    uVar2 = (*fuel_cut_rpm_offset_select)(uVar1,param_2,uVar3,uVar3,uVar2,param_3);
    uVar3 = (*fuel_cut_enable_check)();
  }
  if ((*(char *)((uint)bVar4 << 0x10 | 0x2c7) == '\0') || ((uVar3 & 0x10) == 0)) {
    uVar3 = uVar3 & 0xfffe;
    uVar2 = uVar2 & 0xfeff;
  }
  if (*(char *)((uint)bVar4 << 0x10 | 0x2c4) != '\0') {
    uVar3 = uVar3 & 0xefff;
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf510) & 1) != 0) {
    uVar3 = uVar3 & 0xf7ff | 0x1000;
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf512) & 0x40) != 0) {
    uVar3 = uVar3 | 0x1800;
  }
  uVar3 = uVar3 & 0xfffb;
  if (*(short *)((uint)bVar4 << 0x10 | 0xf1ec) != 0) {
    uVar3 = uVar3 | 4;
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf1f4) & 8) != 0) {
    if (((*(ushort *)((uint)bVar4 << 0x10 | 0xee88) & 1) == 0) ||
       ((*(ushort *)((uint)bVar4 << 0x10 | 0xf1fe) & 0x40) != 0)) {
      uVar3 = uVar3 & 0xff7f;
    }
    else {
      uVar3 = uVar3 | 0x80;
    }
  }
  *(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) = uVar3;
  *(ushort *)((uint)bVar4 << 0x10 | 0xf0fe) = uVar2;
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack

ushort fuel_cut_enable_check(ushort param_fp1)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  ushort in_stack_00000004;
  
  bVar3 = 1;
  if (((cal_fuel_cut_enable_flag != '\0') && (DAT_00010d30 < DAT_0001f436)) ||
     ((cal_fuel_cut_enable_flag == '\0' && (DAT_00010d2e < coolant_temp_scaled_f130)))) {
    DAT_0001f066 = DAT_00010d3a;
  }
  uVar2 = get_high_byte(DAT_0001f4e8);
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf216) & 0x2000) == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xd3e);
    if (*puVar1 <= uVar2 && uVar2 != *puVar1) {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf216);
      *puVar1 = *puVar1 | 0x2000;
    }
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xd40);
    if (uVar2 < *puVar1 || uVar2 == *puVar1) {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf216);
      *puVar1 = *puVar1 & 0xdfff;
    }
  }
  if (((in_stack_00000004 & 1) != 0) &&
     (((uVar2 = *(ushort *)((uint)bVar3 << 0x10 | 0xf118),
       puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xd36), !(uVar2 < *puVar1 || uVar2 == *puVar1) ||
       (((*(char *)((uint)bVar3 << 0x10 | 0x30d) != '\0' &&
         ((*(ushort *)((uint)bVar3 << 0x10 | 0xf216) & 0x2000) != 0)) ||
        (*(short *)((uint)bVar3 << 0x10 | 0xf066) == 0)))) ||
      ((*(ushort *)((uint)bVar3 << 0x10 | 0xf10e) < *(ushort *)((uint)bVar3 << 0x10 | 0xd38) ||
       (uVar2 = *(short *)((uint)bVar3 << 0x10 | 0xd32) * 0x14,
       puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xef6a), !(uVar2 < *puVar1 || uVar2 == *puVar1)))))
     )) {
    in_stack_00000004 = in_stack_00000004 & 0xfffe;
  }
  return in_stack_00000004;
}



// WARNING: Restarted to delay deadcode elimination for space: stack

ushort __stdcall_far
fuel_cut_rpm_offset_select(undefined2 _unused_far_pad,ushort param_2,ushort param_3)

{
  ushort uVar1;
  
  if ((((param_2 & 0x10) == 0) || ((ushort)(DAT_00010d34 * 0x14) <= g_rpm_zone_dwell_counter_ef6a))
     && ((param_2 & 1) == 0)) {
    uVar1 = param_3 & 0xfeff;
  }
  else {
    uVar1 = param_3 | 0x100;
  }
  return uVar1;
}



void crank_stall_detect_update(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((g_status_flags_f0f8 & 2) != 0) {
    DAT_0001ef90 = DAT_00010bfe;
  }
  if (DAT_0001ef90 == 0) {
    DAT_0001ef6e = 0;
  }
  sVar2 = (*check_flag_fe96_bit7_clear)();
  if (sVar2 != 0) {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xef98) = 0x118;
  }
  if (4 < *(ushort *)((uint)bVar3 << 0x10 | 0xef6e)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf224);
    *puVar1 = *puVar1 | 0x100;
  }
  return;
}



void sensor_adc_update(void)

{
  (*adc_read_sequence_main)();
  (*map_tps_axis_update)();
  iat_axis_update();
  coolant_temp_validity_and_scale();
  manifold_heat_enable_flag_update();
  tps_delta_calc();
  (*iac_position_integrator)();
  return;
}



void map_tps_axis_update(void)

{
  byte bVar1;
  
  bVar1 = 1;
  if (DAT_00010f5c <= DAT_0001f108) {
    if (DAT_0001f108 <= DAT_00010f5a) {
      g_status_flags_f1fe = g_status_flags_f1fe & 0xfffe;
      goto LAB_00021237;
    }
  }
  g_status_flags_f1fe = g_status_flags_f1fe | 1;
LAB_00021237:
  (*tps_range_clamp_update)();
  calc_f110_f10e_via_table();
  (*axis_lookup_interp)();
  (*axis_lookup_interp)();
  (*axis_lookup_interp)();
  if ((*(short *)((uint)bVar1 << 0x10 | 0xef90) == 0) &&
     ((*(ushort *)((uint)bVar1 << 0x10 | 0xf224) & 0x400) != 0)) {
    *(undefined2 *)((uint)bVar1 << 0x10 | 0xee8a) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xf10e);
  }
  return;
}



void tps_range_clamp_update(void)

{
  ushort *puVar1;
  ushort uVar2;
  undefined2 uVar3;
  short in_ram_00000f68;
  ushort uVar4;
  ushort value;
  ushort unaff_R3;
  byte bVar5;
  
  uVar2 = *(ushort *)((int)((0xa0 - DAT_00010f66) * 2) + 0x33a4);
  uVar4 = sensor_validity_status_flags_f1fe;
  value = DAT_0001f10a;
  if ((uVar4 & 0x11) != 0) {
    value = *(ushort *)((int)((0xa0 - in_ram_00000f68) * 2) + 0x33a4);
  }
  bVar5 = 0;
  uVar4 = DAT_0000f20e;
  if ((uVar4 & 0x10) == 0) {
    uVar4 = g_rpm_load_zone_flags_f208;
    if ((uVar4 & 8) == 0) goto LAB_000212fc;
    uVar3 = DAT_0000f10c;
    uVar4 = DAT_0000f10c;
    uVar4 = sat_sub_u16(uVar4,0xc);
    value = clamp_u16(value,uVar4,unaff_R3);
    if (((*(ushort *)((uint)bVar5 << 0x10 | 0xf224) & 0x1000) != 0) && (uVar2 < value)) {
      value = uVar2;
    }
  }
  *(ushort *)((uint)bVar5 << 0x10 | 0xf10c) = value;
LAB_000212fc:
  if (uVar2 < *(ushort *)((uint)bVar5 << 0x10 | 0xf10c)) {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf224);
    *puVar1 = *puVar1 & 0xefff;
  }
  else {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf224);
    *puVar1 = *puVar1 | 0x1000;
  }
  return;
}



void calc_f110_f10e_via_table(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  g_axis_lookup_input_scratch_f0ba = DAT_0001f10a;
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x32f8);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf112) = uVar1;
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf1fe) & 0x11) == 0) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf110) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf112);
  }
  else {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf110) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf68);
  }
  if (*(short *)((uint)bVar2 << 0x10 | 0xf10c) == *(short *)((uint)bVar2 << 0x10 | 0xf10a)) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf10e) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf112);
  }
  else {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf0ba) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf10c);
    (*axis_lookup_interp)();
    uVar1 = table_lookup_interp(1,(void *)0x32f8);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf10e) = uVar1;
  }
  return;
}



void iat_axis_update(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  if (DAT_00010f6c <= g_iat_raw_adc_f116) {
    if (g_iat_raw_adc_f116 <= DAT_00010f6a) {
      g_status_flags_f1fe = g_status_flags_f1fe & 0xfffd;
      goto LAB_000213b4;
    }
  }
  g_status_flags_f1fe = g_status_flags_f1fe | 2;
LAB_000213b4:
  calc_f118_via_table();
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x1d7a);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf298) = uVar1;
  return;
}



void calc_f118_via_table(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x34e6);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf11a) = uVar1;
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf1fe) & 2) == 0) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf118) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf11a);
  }
  else {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf118) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf6e);
  }
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// coolant_temp_validity_and_scale: reads 0xF12E, writes 0xF130, computes final value into
// 0xF29A. This is the fourth, independently-traced coolant path logging.txt describes
// (F84E -> F12E -> F130 -> F29A). CONFIRMED: F84E is written by adc_read_sequence_main
// channel 2 (@0x1556d, word write 0xF84E), F12E is written elsewhere (not re-traced this
// pass) and read here, feeding this function's F130/F29A output.
// STATUS: this path does NOT overlap MUT profile's claimed Coolant Temp addresses
// (0xF109/0xF10F, RequestID 0x07/0x10) - no writer found for 0xF109 under any bank prefix
// checked (0x0-0x1). Consistent with logging.txt's own caveat that the MUT Coolant Temp
// claim is "address arithmetic only, NOT independently traced" and that FOUR different
// coolant-temp claims exist in the codebase (RVR_base.xml F0C4/F0C8, MUT F109/F10F, this
// F84E/F12E/F130/F29A path, and presumably one more) with NONE reconciled. This pass does
// NOT resolve OPEN ITEM #4 - still open, do not treat any single coolant path as
// authoritative.

void coolant_temp_validity_and_scale(void)

{
  ushort *puVar1;
  short sVar2;
  undefined2 uVar3;
  byte bVar4;
  
  bVar4 = 1;
  sVar2 = (*coolant_temp_range_check)();
  if (sVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf1fe);
    *puVar1 = *puVar1 & 0xfffb;
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xf130) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xf12e);
  }
  else {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf1fe);
    *puVar1 = *puVar1 | 4;
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xf130) = 0xcd;
  }
  uVar3 = (*mul_fixedpoint_8)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf29a) = uVar3;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort coolant_temp_range_check(void)

{
  if (mut_battery_voltage_f13a < DAT_000109c6) {
    DAT_0001ef92 = DAT_000109c8;
  }
  if (((DAT_0001ef92 == 0) && (DAT_000109c4 <= DAT_0001f12e)) && (DAT_0001f12e <= DAT_000109c2)) {
    return 0;
  }
  return 1;
}



void manifold_heat_enable_flag_update(void)

{
  if ((DAT_0001f1f4 & 8) == 0) {
    if ((((g_status_flags_f0f8 & 0x80) == 0) || (g_adc_ch7_raw_f13c <= DAT_00010f70)) &&
       (DAT_00010f72 <= g_adc_ch7_raw_f13c)) goto LAB_000214e2;
  }
  else {
    if ((((DAT_00010abc < inj_pw_output_f186) || (g_adc_ch7_raw_f13c <= DAT_00010abe)) ||
        ((g_status_flags_f1fe & 8) != 0)) && (DAT_00010ac0 <= g_adc_ch7_raw_f13c)) {
      DAT_0001f068 = DAT_00010ac2;
LAB_000214e2:
      g_status_flags_f1fe = g_status_flags_f1fe & 0xffbf;
      return;
    }
    if (DAT_0001f068 != 0) {
      return;
    }
  }
  g_status_flags_f1fe = g_status_flags_f1fe | 0x40;
  return;
}



// [EXISTING FUNCTION, CROSS-REF ADDED 2026-08-08] tps_delta_calc: writes
// RAM F158 (write site at 0x2152f). Confirmed consumer this session:
// tcu_dtc_plausibility_check_and_fault_dispatch (0x2bc0f, renamed from
// tcu_dtc_status_check_dispatch) reads F158 as part of a multi-condition
// DTC/plausibility check, alongside engine_torque_pct_f17a and
// coolant_temp_scaled_f130. See that function's plate comment and
// mut_verification_status.md "MAJOR RECONCILIATION: EVOSCAN SOURCE + ROM
// ANALYSIS CONFIRM A REAL DTC SUBSYSTEM" (2026-08-08) for the full DTC
// subsystem writeup, including the EvoScan-confirmed MUT-II DTC RequestID
// sequence (0x3B/0x3C/0x3D) this function's output may ultimately feed
// into (not yet confirmed at the bit level).

void tps_delta_calc(void)

{
  undefined2 uVar1;
  ushort a;
  ushort uVar2;
  undefined2 extraout_R2;
  byte bVar3;
  
  uVar1 = DAT_0000f14e;
  DAT_0000f14c = uVar1;
  uVar1 = DAT_0000f152;
  DAT_0000f150 = uVar1;
  DAT_0000f152 = 0;
  DAT_0000f14e = 0;
  bVar3 = 0;
  uVar2 = g_rpm_load_zone_flags_f208;
  if ((uVar2 & 1) != 0) {
    uVar2 = g_tps_load_current_f13c;
    a = DAT_0000f154;
    uVar2 = sat_sub_u16(a,uVar2);
    *(ushort *)((uint)bVar3 << 0x10 | 0xf158) = uVar2;
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf154) = extraout_R2;
  }
  return;
}



void iac_position_integrator(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  ushort extraout_R2;
  byte bVar4;
  
  bVar4 = 1;
  uVar3 = idle_closed_loop_ready_check();
  if (uVar3 == 0) {
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xef94) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xcc8);
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 & 0xefff;
  }
  else {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 | 0x1000;
  }
  uVar3 = extraout_R2;
  if (*(short *)((uint)bVar4 << 0x10 | 0xef94) == 0) {
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf13e);
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xef94) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xcc8);
    if (uVar2 < 0x22) {
      if (uVar2 < 0x21) {
        uVar3 = extraout_R2 + 1;
      }
    }
    else {
      uVar3 = extraout_R2 - 1;
    }
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf1f4) & 8) != 0) {
    uVar3 = sat_sub_u16(0xa1,*(ushort *)((uint)bVar4 << 0x10 | 0xee86));
  }
  uVar3 = clamp_u16(uVar3,*(ushort *)((uint)bVar4 << 0x10 | 0xcce),
                    *(ushort *)((uint)bVar4 << 0x10 | 0xccc));
  *(ushort *)((uint)bVar4 << 0x10 | 0xee82) = uVar3;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort idle_closed_loop_ready_check(void)

{
  ushort uVar1;
  
  if (((((g_status_flags_f20e & 0x11) == 0) && ((g_status_flags_f3f0 & 0x10) != 0)) &&
      ((g_status_flags_f0f8 & 0x10) == 0)) &&
     (((DAT_00010cc0 < g_tps_load_axis_f10e && (DAT_0001f14c <= DAT_00010cc6)) &&
      ((DAT_00010cc4 <= g_adc_ch7_raw_f13c && (g_adc_ch7_raw_f13c <= DAT_00010cc2)))))) {
    if (g_idle_rpm_actual_f178 < g_idle_target_rpm_f400) {
      uVar1 = g_idle_target_rpm_f400 - g_idle_rpm_actual_f178;
    }
    else {
      uVar1 = g_idle_rpm_actual_f178 - g_idle_target_rpm_f400;
    }
    if (uVar1 <= DAT_00010cca) {
      return 1;
    }
  }
  return 0;
}



void throttle_target_ramp_update(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  ushort extraout_R2;
  ushort uVar4;
  byte bVar5;
  
  bVar5 = 1;
  uVar2 = table_lookup_interp(1,(void *)0x1d98);
  if (*(char *)((uint)bVar5 << 0x10 | 0x309) == '\0') goto LAB_00021752;
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf20e) & 0x11) == 0) {
    if ((*(short *)((uint)bVar5 << 0x10 | 0xf2bc) == 0) &&
       (((((*(ushort *)((uint)bVar5 << 0x10 | 0xf0fa) &
            (*(ushort *)((uint)bVar5 << 0x10 | 0xf0f8) ^ *(ushort *)((uint)bVar5 << 0x10 | 0xf0fa))
           & 0x80) != 0 ||
          ((*(ushort *)((uint)bVar5 << 0x10 | 0xf0fa) &
            (*(ushort *)((uint)bVar5 << 0x10 | 0xf0f8) ^ *(ushort *)((uint)bVar5 << 0x10 | 0xf0fa))
           & 0x20) != 0)) ||
         ((*(char *)((uint)bVar5 << 0x10 | 0x30b) != '\0' &&
          ((*(ushort *)((uint)bVar5 << 0x10 | 0xf0f8) &
            (*(ushort *)((uint)bVar5 << 0x10 | 0xf0f8) ^ *(ushort *)((uint)bVar5 << 0x10 | 0xf0fa))
           & 8) != 0)))) &&
        (((puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf29c), *puVar1 <= uVar2 && uVar2 != *puVar1
          || (*(short *)((uint)bVar5 << 0x10 | 0xf2a0) != 0)) ||
         (*(short *)((uint)bVar5 << 0x10 | 0xf2c6) != 0)))))) {
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf056) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x13e2);
LAB_000216c5:
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2be) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x13e0);
    }
    else {
      if (*(short *)((uint)bVar5 << 0x10 | 0xf056) != 0) goto LAB_000216c5;
      if ((0x80 < *(ushort *)((uint)bVar5 << 0x10 | 0xf2be)) &&
         ((*(ushort *)((uint)bVar5 << 0x10 | 0xf208) & 2) != 0)) {
        uVar2 = sat_sub_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xf2be),
                            *(ushort *)((uint)bVar5 << 0x10 | 0x13de));
        *(ushort *)((uint)bVar5 << 0x10 | 0xf2be) = uVar2;
      }
    }
    if (*(ushort *)((uint)bVar5 << 0x10 | 0xf2be) < 0x80) {
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2be) = 0x80;
    }
    uVar2 = throttle_ramp_enable_check();
    if ((uVar2 == 0) || (*(short *)((uint)bVar5 << 0x10 | 0xf2be) != 0x80)) {
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2bc) = 0xff;
    }
    else if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf208) & 2) != 0) {
      uVar2 = sat_sub_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xf2bc),
                          *(ushort *)((uint)bVar5 << 0x10 | 0x13e4));
      *(ushort *)((uint)bVar5 << 0x10 | 0xf2bc) = uVar2;
    }
  }
  else {
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2bc) = 0;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2be) = 0x80;
  }
  table_lookup_interp(1,(void *)0x1d98);
  uVar2 = (*weighted_blend_u8frac)();
LAB_00021752:
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf20e) & 2) == 0) {
    uVar4 = 0x80;
    uVar3 = sat_sub_u16(uVar2,0x80);
    uVar2 = *(ushort *)((uint)bVar5 << 0x10 | 0xf17a);
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xa96);
    if (*puVar1 <= uVar2 && uVar2 != *puVar1) {
      (*axis_lookup_interp)();
      uVar4 = table_lookup_interp(1,(void *)0x1d86);
      uVar3 = extraout_R2;
    }
    (*muldiv_s16_rounded)((short)((uint)uVar3 * (uint)uVar4 >> 0x10));
    uVar2 = (*clamp_u8)();
  }
  *(ushort *)((uint)bVar5 << 0x10 | 0xf29c) = uVar2;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort throttle_ramp_enable_check(void)

{
  ushort uVar1;
  
  if (((((g_status_flags_f0f8 & 0x80) == 0) || ((g_status_flags_f0f8 & 0x20) == 0)) ||
      ((DAT_0001eee2 & 1) != 0)) ||
     (((g_status_flags_f0f8 & 4) != 0 ||
      ((DAT_0001030b != '\0' && ((g_status_flags_f0f8 & 8) != 0)))))) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



void throttle_body_update(void)

{
  if ((g_status_flags_f20e & 0x10) == 0) {
    (*engine_torque_pct_scale_calc)();
    throttle_body_pos_error_calc();
  }
  else {
    (*engine_torque_scale_state_reset)();
  }
  throttle_param_table_select();
  (*axis_lookup_interp)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void engine_torque_scale_state_reset(void)

{
  candidate_rpm_period_hist_newest = 0xffff;
  DAT_0000f182 = 0;
  candidate_rpm_axis_bkpt_lo = 0;
  DAT_0000f17a = 0;
  DAT_0000f178 = 0;
  DAT_0000f170 = 0;
  DAT_0000f17c = 0;
  DAT_0000f172 = 0;
  DAT_0000f174 = 0;
  DAT_0000f17e = 0x80;
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// engine_torque_pct_scale_calc: reads 0xF5CA/F5CC (confirmed engine torque values, no RPM
// input anywhere in this function), computes via div_s32_s16_rounded-style calls (0x14151),
// writes intermediate results to 0xF172/F170/F178, final output @0xF17A (write instruction
// at 0x218b8 - exact ROM address match to logging.txt's citation). CONFIRMS logging.txt's
// REFUTED claim: 0xF17A/F17B is NOT RPM, it's purely torque/load-derived (F5CA/F5CC via
// rounded division + clamp), computed with no RPM input at all. VERIFIED under current
// decoder - RPM's real location remains unknown, do not resume searching here
// (logging.txt OPEN ITEM #1, still open).
// 
// [CROSS-REF ADDED 2026-08-07] Output global renamed DAT_0001f17a ->
// engine_torque_pct_f17a. This is the single most heavily-consumed derived
// value in the ROM (~86 read sites found via xref search) -- it's the
// primary load/threshold input for closedloop_eligibility_check,
// purge_enable_check, isc_condition_eval, warmup_complete_gate_check,
// knock_condition_eval, cranking_detect_check, o2_upstream_enable_check,
// egr_f490_condition_flags_update, and many more, plus it's one of the
// fields packed into the periodic SCI1 status frame built by
// sci1_periodic_status_frame_build_f54a_f566 (0x28d7d, field F550). No
// static WRITE xref existed to F17A before this session traced it here --
// worth keeping in mind that this global's writer wasn't visible to a
// routine xref sweep, in case other "no writer found" cells in this project
// have the same cause.

void engine_torque_pct_scale_calc(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  uVar1 = candidate_rpm_period_hist_newest;
  uVar1 = DAT_0000f5cc;
  bVar2 = 0;
  uVar1 = (*div_s32_s16_rounded)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf172) = uVar1;
  uVar1 = (*div_s32_s16_rounded)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf170) = uVar1;
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf178) = uVar1;
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf17a) = uVar1;
  return;
}



void throttle_body_pos_error_calc(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  if (g_rpm_zone_dwell_counter_ef6a < DAT_000109dc) {
    DAT_0001f174 = 0;
    DAT_0001f17e = 0x80;
  }
  else {
    sat_sub_u16(DAT_0001f172,g_throttle_body_pos_target_f170);
    uVar1 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf174) = uVar1;
    if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf208) & 4) == 0) {
      return;
    }
    sat_sub_u16(*(short *)((uint)bVar2 << 0x10 | 0xf17a) + 0x80,
                *(ushort *)((uint)bVar2 << 0x10 | 0xf17c));
    uVar1 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf17e) = uVar1;
  }
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf17c) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xf17a);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void throttle_param_table_select(void)

{
  if ((g_status_flags_f0f8 & 4) == 0) {
    DAT_0001f184 = DAT_00010aea;
  }
  else {
    DAT_0001f184 = DAT_00010aec;
  }
  return;
}



void lambda_enable_flag_update(void)

{
  ushort *puVar1;
  undefined1 extraout_var;
  uchar uVar3;
  short sVar2;
  byte bVar4;
  
  bVar4 = 1;
  uVar3 = lambda_enable_inhibit_check();
  if (CONCAT11(extraout_var,uVar3) == 0) {
    sVar2 = (*o2_wideband_lsu_mode_check)();
    if (sVar2 == 0) {
      puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf1fe);
      *puVar1 = *puVar1 & 0xfff7;
      goto LAB_0002195c;
    }
  }
  puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf1fe);
  *puVar1 = *puVar1 | 8;
LAB_0002195c:
  (*lambda_correction_output_calc)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar lambda_enable_inhibit_check(void)

{
  if ((g_status_flags_f20e & 0x11) != 0) {
    DAT_0001ef96 = DAT_00010f56;
  }
  return DAT_0001ef96 == 0;
}



void lambda_correction_output_calc(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  if ((g_status_flags_f20e & 0x10) == 0) {
    uVar1 = DAT_0000f1b6;
    uVar1 = DAT_0000f5e0;
    bVar2 = 0;
    uVar1 = (*muldiv_s16_rounded)();
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf1e0) = uVar1;
    (*div_u16_rounded)();
    uVar1 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf1de) = uVar1;
  }
  else {
    DAT_0001f1de = 0;
    DAT_0001f1e0 = 0;
  }
  return;
}



void fueling_lambda_update(void)

{
  fueling_base_injection_calc();
  (*fueling_accel_enrichment_calc)();
  (*fueling_openloop_output_copy)();
  (*fueling_throttle_zone_flags_update)();
  (*fueling_inj_target_select)();
  (*fueling_inj_accum_clamp_drain)();
  (*fueling_inj_pw_calc)();
  (*axis_lookup_interp)();
  return;
}



void fueling_base_injection_calc(void)

{
  short sVar1;
  undefined2 uVar2;
  byte bVar3;
  void *table;
  
  bVar3 = 1;
  (*axis_lookup_interp)();
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0) {
    table = (void *)0xda8c;
  }
  else {
    table = (void *)0xda6c;
  }
  table_lookup_indexed(2,table);
  table_lookup_interp(1,(void *)0x18a0);
  table_lookup_interp(1,(void *)0x1894);
  o2_mode9_10_correction_apply();
  (*muldiv_s16_rounded)();
  sVar1 = (*clamp_u8)();
  *(short *)((uint)bVar3 << 0x10 | 0xf1be) = sVar1 << 3;
  uVar2 = (*mul_u16_x2_sat)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1c2) = uVar2;
  return;
}



ushort fueling_accel_enrichment_calc(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = g_status_flags_f20e & 0x11;
  if (uVar2 == 0) {
    uVar1 = (undefined2)((uint)DAT_0001081a * 0x75);
    (*identity_passthrough_arg1)
              ((short)((uint)DAT_0001081a * 0x75 >> 0x10),uVar1,param_3,uVar1,
               g_throttle_body_pos_target_f170 << 1,DAT_0001f29a * DAT_0001f298);
    (*div_u32_u16_rounded)();
    uVar2 = (*div_s32_s16_rounded)();
    *(ushort *)((uint)bVar3 << 0x10 | 0xf1c0) = uVar2;
  }
  else {
    DAT_0001f1c0 = 0;
  }
  return uVar2;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void fueling_openloop_output_copy(void)

{
  undefined2 uVar1;
  
  if ((g_status_flags_f20e & 0x10) != 0) {
    uVar1 = DAT_0000f1be;
    DAT_0000f1b6 = uVar1;
    uVar1 = DAT_0000f1be;
    DAT_0000f1bc = uVar1;
    uVar1 = DAT_0000f1c2;
    DAT_0000f1c4 = uVar1;
    uVar1 = DAT_0000f1c2;
    DAT_0000f1c6 = uVar1;
  }
  return;
}



void fueling_throttle_zone_flags_update(void)

{
  ushort uVar1;
  undefined2 in_ram_00000a62;
  undefined2 in_ram_00000a68;
  
  if ((g_status_flags_f224 & 0x4000) == 0) {
    if (DAT_00010a5c < g_tps_load_trimmed_f13e) {
      g_status_flags_f224 = g_status_flags_f224 | 0x4000;
    }
  }
  else if (g_tps_load_trimmed_f13e <= DAT_00010a5e) {
    g_status_flags_f224 = g_status_flags_f224 & 0xbfff;
  }
  if (((g_status_flags_f224 & 0x4000) == 0) || (engine_torque_pct_f17a <= DAT_00010a60)) {
    uVar1 = DAT_0000f224;
    DAT_0000f224 = uVar1 & 0x7fff;
    DAT_0000f012 = in_ram_00000a62;
    DAT_0000f014 = in_ram_00000a68;
    return;
  }
  g_status_flags_f224 = g_status_flags_f224 | 0x8000;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void fueling_inj_target_select(void)

{
  ushort uVar1;
  
  uVar1 = 0;
  if ((g_warmup_state_f594 & 0x80) == 0) {
    if (g_rpm_zone_dwell_counter_ef6a < DAT_00010aa4) {
LAB_00021bc6:
      DAT_0001f1b4 = DAT_0001080e;
      goto LAB_00021bce;
    }
    if ((g_status_flags_f216 & 0x100) != 0) goto LAB_00021ba9;
    if ((g_status_flags_f3f0 & 0x10) == 0) goto LAB_00021bc6;
    DAT_0001f1b4 = BYTE_ARRAY_0001080c;
  }
  else {
LAB_00021ba9:
    DAT_0001f1b4 = DAT_00010da2;
  }
  uVar1 = 0x200;
LAB_00021bce:
  if (DAT_00010306 == '\0') {
    uVar1 = 0;
    if ((ushort)DAT_0001f1b4 <= (ushort)DAT_00010958) {
      uVar1 = 0x200;
    }
  }
  g_status_flags_f226 = uVar1 | g_status_flags_f226 & 0xfdff;
  return;
}



void fueling_inj_accum_clamp_drain(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 0;
  uVar1 = DAT_0000f1ca;
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf1c8) = uVar1;
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf1cc) = uVar1;
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf1ce) = 0;
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf1ca) = 0;
  return;
}



void fueling_inj_pw_calc(void)

{
  ushort a;
  ushort a_00;
  undefined2 uVar1;
  ushort b;
  ushort extraout_R1;
  byte bVar2;
  ushort b_00;
  
  bVar2 = 1;
  a_00 = (*mul_u16_hi)();
  uVar1 = (*inc_if_signed_flag)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf190) = uVar1;
  (*inc_if_signed_flag)();
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf186) = uVar1;
  mul_u16_sat_s16(a_00,b,*(ushort *)((uint)bVar2 << 0x10 | 0xf29a));
  b_00 = extraout_R1;
  uVar1 = (*inc_if_signed_flag)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf194) = uVar1;
  (*inc_if_signed_flag)();
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf18a) = uVar1;
  a = *(ushort *)((uint)bVar2 << 0x10 | 0xf298);
  mul_u16_sat_s16(a,b_00,a);
  uVar1 = (*inc_if_signed_flag)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf196) = uVar1;
  (*inc_if_signed_flag)();
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf18c) = uVar1;
  mul_u16_sat_s16(a_00,b,*(ushort *)((uint)bVar2 << 0x10 | 0xf298));
  uVar1 = (*inc_if_signed_flag)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf192) = uVar1;
  (*inc_if_signed_flag)();
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf188) = uVar1;
  return;
}



void fueling_wideband_output_scale(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort extraout_R1;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = (*div_u16_rounded)();
  if (uVar2 < 0xfe) {
    *(ushort *)((uint)bVar3 << 0x10 | 0xf1e6) = uVar2;
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf1e6) = 0xfe;
  }
  uVar2 = (*mul_u16_hi)();
  uVar2 = passthrough_or_sat(uVar2 >> 2,
                             (extraout_R1 >> 1 | (ushort)((byte)uVar2 & 1) << 0xf) >> 1 |
                             (ushort)((byte)(uVar2 >> 1) & 1) << 0xf);
  *(ushort *)((uint)bVar3 << 0x10 | 0xf242) = uVar2;
  if (*(short *)((uint)bVar3 << 0x10 | 0xef90) == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf224);
    *puVar1 = *puVar1 & 0xfff7;
  }
  else if (*(ushort *)((uint)bVar3 << 0x10 | 0x10de) <= *(ushort *)((uint)bVar3 << 0x10 | 0xf1e6)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf224);
    *puVar1 = *puVar1 | 8;
  }
  return;
}



void fueling_coolant_inj_offset_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = DAT_0001089a;
  if (coolant_temp_scaled_f130 <= DAT_0001089c) {
    uVar1 = sat_sub_u16(DAT_0001089a,DAT_0001089e);
  }
  *(ushort *)((uint)bVar2 << 0x10 | 0xf22c) = uVar1;
  *(ushort *)((uint)bVar2 << 0x10 | 0xf22a) = uVar1;
  return;
}



void rpm_load_zone_counter_update(void)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  uVar1 = g_rpm_load_zone_flags_f208;
  g_rpm_load_zone_flags_f208 = uVar1 & 0xffc0;
  uVar2 = g_status_flags_f20a;
  uVar1 = g_rpm_load_zone_flags_f208;
  g_rpm_load_zone_flags_f208 = uVar1 | uVar2 & 0x3f;
  uVar1 = g_status_flags_f20a;
  g_status_flags_f20a = uVar1 & 0xffc0;
  bVar3 = 0;
  uVar1 = g_rpm_load_zone_flags_f208;
  if ((uVar1 & 1) != 0) {
    increment_counters_in_range(0xef6a,0xef74);
    (*decrement_counters_in_range)();
  }
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf208) & 2) != 0) {
    increment_counters_in_range(0xef74,0xef74);
    increment_counters_in_range(0xef74,0xef74);
    (*decrement_counters_in_range)();
    (*decrement_counters_in_range)();
  }
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf208) & 4) != 0) {
    increment_counters_in_range(0xef74,0xef74);
    (*decrement_counters_in_range)();
  }
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf208) & 8) != 0) {
    increment_counters_in_range(0xef74,0xef7a);
    (*decrement_counters_in_range)();
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [VERIFIED 2026-07-22] inj_channel_state_init: unconditionally sets 6 RAM cells
// (EE8C/EE8E/EE90/EE92/EE94/EE96 -- MUT ReqID 0x60-0x65, per-table entries
// MUT_60_entry..MUT_65_entry at ROM 0x2fb90) to the neutral value 0x8080 (no
// gating conditions, no per-cylinder differentiation in this function). Matches
// the shape of a per-cylinder fuel-trim/balance array (6 entries, centered at
// 0x80=neutral for what may be a signed offset scale), but this is INFERENCE,
// not confirmed semantics.
// No runtime/per-cycle updater has been found despite checking every *inj*- and
// *cyl*-named function in the ROM (fueling_inj_pw_calc, fueling_inj_target_select,
// fueling_inj_accum_clamp_drain, per_cyl_knock_flag_update_cyl_a/b -- none touch
// this address range). Static xref search also returns zero hits (consistent
// with this ROM's known indirect/bank-prefixed addressing blind spot).
// 
// [RECONFIRMED 2026-08-07] Re-checked with current disassembly using the
// correct 4-byte write encoding (1D EE <hi> <lo> for mov:g.w Rn,@addr:16)
// rather than unreliable 2-byte search -- confirms this is genuinely the ONLY
// writer to all 6 cells anywhere in the ROM. Also checked the 33-byte
// unanalyzed gap immediately preceding this function (0x21edf-0x21eff): it is
// pure 0xFF padding, not orphaned/hidden code. CLOSING this open question: no
// runtime updater exists in this ROM. These 6 cells are init-only, static for
// the life of the ECU -- treat MUT ReqID 0x60-0x65 as effectively dead/
// unused-feature reads (always return 0x8080), not live sensor or actuator
// values. See mut_verification_status.md EE8C-EE96 cluster entry.

void inj_channel_state_init(void)

{
  DAT_0001ee96 = 0x8080;
  DAT_0001ee94 = 0x8080;
  DAT_0001ee92 = 0x8080;
  DAT_0001ee90 = 0x8080;
  DAT_0001ee8e = 0x8080;
  DAT_0001ee8c = 0x8080;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void ecu_openloop_state_reset(void)

{
  g_status_flags_f20e = 0x10;
  DAT_0001f228 = 0xffff;
  return;
}



void main_control_loop_update(void)

{
  control_state_latch_prev();
  diag_flag_clear_and_param_reload();
  f216_bit6_update_from_hysteresis_gate();
  (*f33e_bit0_update_from_rpm_load_window)();
  f216_bit4_update_table_1cc7();
  (*f216_low_nibble_update_table_1cc3)();
  (*f218_shift_in_f216_low_nibble)();
  (*f216_bit8_update_from_recovery_chain)();
  (*tcu_link_confirmed_flag_f226_bit7_update)();
  f226_bit8_update_from_f4c0_check();
  f25e_bit6_update_from_decel_fuel_cut();
  aux_load_enable_flag_update();
  (*post_start_enrich_timer_update)();
  o2_heater_ready_flag_update();
  mode_state_update();
  f224_bit10_update_from_f20e_check();
  return;
}



void control_state_latch_prev(void)

{
  DAT_0001f212 = g_status_flags_f20e;
  DAT_0001f214 = DAT_0001f210;
  DAT_0001f220 = g_status_flags_f21c;
  DAT_0001f222 = DAT_0001f21e;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void diag_flag_clear_and_param_reload(void)

{
  DAT_0001ef54 = DAT_0001ef54 & 0xfbff;
  DAT_0001f0b6 = DAT_000113d2;
  return;
}



void f216_bit6_update_from_hysteresis_gate(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((DAT_000102c5 == '\0') || (uVar2 = f1e6_hysteresis_gate_check(), uVar2 == 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf216);
    *puVar1 = *puVar1 & 0xffbf;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf216);
  *puVar1 = *puVar1 | 0x40;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f1e6_hysteresis_gate_check(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f216 & 0x800) == 0) {
    if (DAT_00010cee < g_f1e6_hysteresis_gate) {
      g_status_flags_f216 = g_status_flags_f216 | 0x800;
    }
  }
  else if (g_f1e6_hysteresis_gate <= DAT_00010cf0) {
    g_status_flags_f216 = g_status_flags_f216 & 0xf7ff;
  }
  if (((((g_status_flags_f216 & 0x800) == 0) || ((g_status_flags_f0f8 & 0x8000) == 0)) ||
      ((DAT_0001f102 & 0x1000) == 0)) || ((g_status_flags_f0f8 & 0x20) == 0)) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



void f33e_bit0_update_from_rpm_load_window(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  sVar2 = (*rpm_load_window_condition_check)();
  if (sVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 & 0xfffe;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 | 1;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort rpm_load_window_condition_check(void)

{
  if (((((g_status_flags_f0f8 & 2) == 0) || ((g_status_flags_f20e & 0x11) != 0)) ||
      (DAT_00010b70 <= engine_torque_pct_f17a)) ||
     ((mut_battery_voltage_f13a <= DAT_00010b76 || (DAT_00010b74 <= mut_battery_voltage_f13a)))) {
    return 0;
  }
  return 1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f216_bit4_update_table_1cc7(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  ushort uVar4;
  
  uVar4 = (ushort)((g_status_flags_f216 & 0x10) != 0);
  if ((g_status_flags_f0f8 & 0x20) == 0) {
    uVar4 = uVar4 + 2;
  }
  uVar1 = DAT_0000f17a;
  uVar2 = DAT_0000f17a;
  uVar3 = DAT_0000f17a;
  if (*(byte *)((int)uVar4 + 0x1cc7) < uVar2 || *(byte *)((int)uVar4 + 0x1cc7) == uVar1) {
    uVar4 = DAT_0000f216;
    DAT_0000f216 = uVar4 | 0x10;
  }
  else {
    uVar4 = DAT_0000f216;
    DAT_0000f216 = uVar4 & 0xffef;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f216_low_nibble_update_table_1cc3(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  char in_ram_000002c1;
  ushort uVar4;
  undefined2 uVar5;
  ushort uStack_4;
  
  uStack_4 = 0;
  uVar4 = (ushort)*(byte *)((int)(ushort)((g_status_flags_f216 & 3) != 0) + 0x1cc3);
  uVar1 = DAT_0000f1de;
  uVar2 = DAT_0000f1de;
  uVar5 = DAT_0000f1de;
  if (uVar4 < uVar2 || uVar4 == uVar1) {
    if (in_ram_000002c1 == '\0') {
      uVar5 = 2;
      uVar1 = DAT_0000f216;
      if ((uVar1 & 2) != 0) {
        uVar5 = 3;
      }
      uVar1 = DAT_0000f1de;
      uVar2 = DAT_0000f1de;
      uVar3 = DAT_0000f1de;
      if (*(byte *)((int)uVar5 + 0x1cc3) < uVar2 || *(byte *)((int)uVar5 + 0x1cc3) == uVar1) {
        uStack_4 = 2;
        goto LAB_000220d5;
      }
    }
    uStack_4 = 1;
  }
LAB_000220d5:
  uVar1 = DAT_0000f216;
  DAT_0000f216 = uStack_4 | uVar1 & 0xfff0;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f218_shift_in_f216_low_nibble(void)

{
  ushort uVar1;
  
  uVar1 = g_status_flags_f216 & 0xf;
  if (1 < uVar1) {
    uVar1 = 2;
  }
  DAT_0001f218 = uVar1 | DAT_0001f218 << 4;
  return;
}



void f216_bit8_update_from_recovery_chain(void)

{
  ushort uVar1;
  ushort extraout_R2;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  f224_threshold_tier_and_change_flag();
  (*stub_zero_0x22193)();
  stub_zero_0x22193();
  uVar1 = efa0_max_hold_and_recovered_check();
  uVar2 = extraout_R2;
  if (uVar1 != 0) {
    uVar2 = extraout_R2 | 0x100;
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf216) =
       uVar2 | *(ushort *)((uint)bVar3 << 0x10 | 0xf216) & 0xf8ff;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f224_threshold_tier_and_change_flag(void)

{
  ushort uVar1;
  
  uVar1 = 0;
  if (DAT_0001f242 < DAT_00010d92) {
    uVar1 = 0x40;
  }
  if (DAT_0001f242 < DAT_00010d90) {
    uVar1 = uVar1 | 0x20;
  }
  if (DAT_0001f242 < DAT_00010d8e) {
    uVar1 = uVar1 | 0x10;
  }
  if (((g_status_flags_f224 ^ uVar1) & 0x70) != 0) {
    uVar1 = uVar1 | 0x80;
  }
  g_status_flags_f224 = uVar1 | g_status_flags_f224 & 0xff0f;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort stub_zero_0x22193(void)

{
  return 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar ef9e_latch_from_multi_threshold_check(void)

{
  if (((((g_status_flags_f0f8 & 0x20) == 0) || (DAT_00010d9a <= DAT_0001f14c)) ||
      ((ushort)(DAT_00010d9e * 4) <= g_throttle_body_pos_target_f170)) ||
     (((DAT_00010d94 <= g_f1e6_hysteresis_gate || (g_f1e6_hysteresis_gate <= DAT_00010d96)) ||
      (((g_status_flags_f224 & 0x40) != 0 || (DAT_00010d9c <= inj_pw_output_f186)))))) {
    g_ef9e_latch_value = g_efa0_max_hold_override_cal;
  }
  return g_ef9e_latch_value == 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort efa0_max_hold_and_recovered_check(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f224 & 0x10) == 0) {
    uVar1 = g_ef9e_latch_value;
  }
  else {
    uVar1 = g_efa0_max_hold_override_cal;
  }
  if (g_efa0_max_hold_value < uVar1) {
    g_efa0_max_hold_value = uVar1;
  }
  if ((g_efa0_max_hold_value == 0) && ((g_status_flags_f224 & 0x10) == 0)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [RENAMED + DOCUMENTED 2026-07-26, was f226_bit7_update_from_f4c2_check]
// Sets/clears status flag F226 bit7 (0x80) based on data arriving over the
// TCU serial RX link (see tcu_rx_main_scheduler @ 0x2aa36 for the full frame
// validator this depends on).
// 
// Clears F226 bit7 if: ROM config byte 0x102fd == 0, OR F4A4 bit7 is set
// (the same flag that also gates canister_purge_duty_calc_f4ac -- likely a
// general "purge/TCU-feature disabled" master flag), OR g_status_flags_f20e
// bit3 is clear.
// 
// Otherwise, sets F226 bit7 (once, sticky -- only sets if not already set)
// when tcu_rx_confirmed_byte3_f4c2 (F4C2, the checksum-confirmed 4th byte of
// the TCU RX frame) has its bits 0x60 equal to 0x40 exactly (i.e. bit6 set,
// bit5 clear, regardless of other bits).
// 
// Best working hypothesis: a "TCU link confirmed / handshake OK" status bit,
// gated on the config byte that enables TCU communication at all and on the
// same purge-feature-enable flag used elsewhere -- but not proven against
// real hardware. F226's other bits not characterized in this project.
// See tcu_rx_main_scheduler (0x2aa36) plate comment for the full serial
// frame context.

void tcu_link_confirmed_flag_f226_bit7_update(void)

{
  if (((DAT_000102fd == '\0') || ((g_purge_enable_cond_f4a4 & 0x80) != 0)) ||
     ((g_status_flags_f20e & 8) == 0)) {
    g_status_flags_f226 = g_status_flags_f226 & 0xff7f;
  }
  else if (((g_status_flags_f226 & 0x80) == 0) && ((tcu_rx_confirmed_byte3_f4c2 & 0x60) == 0x40)) {
    g_status_flags_f226 = g_status_flags_f226 | 0x80;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f226_bit8_update_from_f4c0_check(void)

{
  if ((((DAT_00010305 == '\0') || ((g_purge_enable_cond_f4a4 & 0x80) != 0)) ||
      ((tcu_rx_confirmed_byte2_f4c0 & 7) == 0)) ||
     (DAT_000113ce < (tcu_rx_confirmed_byte2_f4c0 & 7))) {
    g_status_flags_f226 = g_status_flags_f226 & 0xfeff;
  }
  else {
    g_status_flags_f226 = g_status_flags_f226 | 0x100;
  }
  return;
}



void f25e_bit6_update_from_decel_fuel_cut(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((DAT_000102cb == '\0') || (uVar2 = decel_fuel_cut_eval(), uVar2 == 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
    *puVar1 = *puVar1 & 0xffbf;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
  *puVar1 = *puVar1 | 0x40;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xefa6) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xa5a);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort decel_fuel_cut_eval(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  
  if ((engine_state_flags_f25e & 0x10) == 0) {
    if (DAT_00010a54 < inj_pw_output_f186) {
      engine_state_flags_f25e = engine_state_flags_f25e | 0x10;
    }
  }
  else if (inj_pw_output_f186 <= DAT_00010a56) {
    engine_state_flags_f25e = engine_state_flags_f25e & 0xffef;
  }
  table_lookup_interp(1,(void *)0x1ccc);
  bVar4 = 0;
  (*check_flag_f0f8_bit2_clear)();
  uVar2 = (*clamp_u8)();
  uVar3 = (*clamp_u8)();
  if (((*(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) & 0xb0) == 0x80) &&
     (uVar3 < *(ushort *)((uint)bVar4 << 0x10 | 0xf17a))) {
    if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0fa) & 0x80) == 0) {
      puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf25e);
      *puVar1 = *puVar1 | 0x20;
    }
    if (((((*(ushort *)((uint)bVar4 << 0x10 | 0xf25e) & 0x10) == 0) &&
         (uVar3 = *(short *)((uint)bVar4 << 0x10 | 0xa58) * 0x14,
         puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xef6a), uVar3 < *puVar1 || uVar3 == *puVar1)) &&
        (*(ushort *)((uint)bVar4 << 0x10 | 0xf17a) <= uVar2)) &&
       ((*(ushort *)((uint)bVar4 << 0x10 | 0xf25e) & 0x20) != 0)) {
      return 1;
    }
  }
  else {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf25e);
    *puVar1 = *puVar1 & 0xffdf;
  }
  return 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar check_flag_f0f8_bit2_clear(void)

{
  return (g_status_flags_f0f8 & 4) == 0;
}



void aux_load_enable_flag_update(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (((DAT_000102dd & 1) == 0) || (uVar2 = aux_load_condition_check(), uVar2 == 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
    *puVar1 = *puVar1 & 0xfdff;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
  *puVar1 = *puVar1 | 0x200;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort aux_load_condition_check(void)

{
  if ((g_status_flags_f20e & 4) == 0) {
    DAT_0001effa = DAT_00010ee4;
  }
  if (((((DAT_000102dd & 2) == 0) || ((g_status_flags_f0f8 & 0x80) != 0)) &&
      ((g_status_flags_f20e & 4) != 0)) && (DAT_00010ee0 <= engine_torque_pct_f17a)) {
    return 1;
  }
  return 0;
}



void post_start_enrich_timer_update(void)

{
  short sVar1;
  
  if (((g_status_flags_f20e & 0x10) == 0) && ((DAT_0001f102 & 0x40) != 0)) {
    if ((DAT_0001f102 & 0x2000) != 0) {
      g_post_start_enrich_timer = g_post_start_enrich_timer_reload_cal;
      goto LAB_00022403;
    }
  }
  else {
    g_post_start_enrich_timer = g_post_start_enrich_timer_reload_cal;
  }
  g_post_start_enrich_timer_secondary = g_post_start_enrich_timer_reload_cal;
LAB_00022403:
  sVar1 = 2;
  if ((g_status_flags_f20e & 0x10) == 0) {
    if (((g_purge_enable_cond_f4a4 & 4) != 0) ||
       ((((DAT_0001f102 & 0x40) != 0 && ((DAT_0001f102 & 0x2000) == 0)) &&
        (g_post_start_enrich_timer == 0)))) {
      sVar1 = 1;
    }
  }
  else {
    sVar1 = 0;
  }
  if (sVar1 == 1) {
    g_status_flags_f224 = g_status_flags_f224 | 4;
  }
  else if (sVar1 == 0) {
    g_status_flags_f224 = g_status_flags_f224 & 0xfffb;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void o2_heater_ready_flag_update(void)

{
  if ((g_status_flags_f226 & 1) == 0) {
    if (DAT_00010a72 < g_coolant_temp_f1e8) {
      g_status_flags_f226 = g_status_flags_f226 | 1;
    }
  }
  else if (g_coolant_temp_f1e8 <= DAT_00010a74) {
    g_status_flags_f226 = g_status_flags_f226 & 0xfffe;
  }
  if ((((DAT_0001f1f4 & 2) == 0) || ((g_status_flags_f226 & 1) == 0)) ||
     (engine_torque_pct_f17a <= DAT_00010a76)) {
    g_status_flags_f226 = g_status_flags_f226 & 0xfffd;
  }
  else {
    g_status_flags_f226 = g_status_flags_f226 | 2;
  }
  return;
}



void mode_state_update(void)

{
  openloop_mode_bit_update();
  (*cranking_mode_bit_update)();
  (*warmup_mode_state_machine)();
  f20e_bit1_mirror_from_f1fe_bit3();
  engine_mode_dispatch_and_f240_edge_counter();
  (*f20e_f210_clear_chain_and_o2_closedloop_gate)();
  o2_sensor2_threshold_flag_update_f226_bit13();
  return;
}



void openloop_mode_bit_update(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = closedloop_inhibit_check();
  if (uVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 & 0xffef;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 | 0x10;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort closedloop_inhibit_check(void)

{
  ushort uVar1;
  
  if ((((DAT_0001f212 & 0x20) == 0) || (DAT_0001efa2 == 0)) || (DAT_0001ef90 == 0)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



void cranking_mode_bit_update(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  sVar2 = (*cranking_detect_check)();
  if (sVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 & 0xfffe;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 | 1;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar cranking_detect_check(void)

{
  bool bVar1;
  ushort uVar2;
  
  if ((DAT_0001f212 & 0x10) != 0) {
    if (DAT_00010834 < g_tps_load_axis_f10e) {
      engine_state_flags_f25e = engine_state_flags_f25e & 0xdfff;
    }
    else {
      engine_state_flags_f25e = engine_state_flags_f25e | 0x2000;
    }
  }
  if (g_tps_load_axis_f10e < DAT_00010864) {
    engine_state_flags_f25e = engine_state_flags_f25e | 0x1000;
  }
  else {
    engine_state_flags_f25e = engine_state_flags_f25e & 0xefff;
  }
  if ((g_status_flags_f0f8 & 0x40) == 0) {
    bVar1 = false;
  }
  else {
    uVar2 = DAT_00010828;
    if ((DAT_0001f212 & 1) == 0) {
      uVar2 = DAT_00010824;
    }
    if ((engine_state_flags_f25e & 0x2000) != 0) {
      uVar2 = uVar2 + DAT_0001082c;
    }
    bVar1 = engine_torque_pct_f17a < uVar2;
  }
  return bVar1;
}



void warmup_mode_state_machine(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  short sVar1;
  ushort extraout_R2;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  closedloop_eligibility_check(param_1,param_2,0,param_3);
  rpm_sync_ready_check();
  (*check_flag_fe96_bit7_clear)();
  (*closedloop_fuel_ready_check)();
  o2_temp_ready_check();
  (*warmup_ign_advance_correction_calc)();
  (*f216_bits_f188_f17a_update_and_warmup_gate)();
  sVar1 = (*warmup_complete_gate_check)();
  uVar2 = extraout_R2;
  if (sVar1 != 0) {
    uVar2 = extraout_R2 | 0xc;
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf20e) =
       uVar2 | *(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 0xfcf3;
  return;
}



ushort closedloop_eligibility_check(void)

{
  ushort *puVar1;
  ushort uVar2;
  short sVar3;
  byte bVar4;
  
  bVar4 = 1;
  if (DAT_000102cc == '\0') {
    return 0;
  }
  if ((g_status_flags_f1f0 & 0x20) != 0) {
    return 0;
  }
  if ((g_status_flags_f1f0 & 1) == 0) {
    return 0;
  }
  if ((((g_status_flags_f20e & 0x10) == 0) && (g_f1e6_hysteresis_gate <= DAT_00010a52)) &&
     ((g_status_flags_f0f8 & 0x80) == 0)) {
    if (DAT_0001f076 != 0) goto LAB_00022663;
  }
  else {
    DAT_0001ef74 = 0;
    DAT_0001f076 = 0;
  }
  if ((((g_status_flags_f0f8 & 0x20) != 0) && (DAT_00010a46 < g_tps_load_axis_f10e)) &&
     (((DAT_0001eee2 & 1) == 0 &&
      (((g_status_flags_f1fe & 8) == 0 && (DAT_00010a42 <= engine_torque_pct_f17a)))))) {
    uVar2 = div_u32_u16_sat(0x1d,0x4c00,(ushort)BYTE_ARRAY_00010886);
    if (uVar2 + *(short *)((uint)bVar4 << 0x10 | 0xa44) * -8 <
        *(ushort *)((uint)bVar4 << 0x10 | 0xf170)) goto LAB_00022663;
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf18c);
    if (*(char *)((uint)bVar4 << 0x10 | 0x2cc) == '\x01') {
      uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf186);
    }
    if (uVar2 < *(ushort *)((uint)bVar4 << 0x10 | 0xa48)) goto LAB_00022663;
  }
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xef74) = 0;
LAB_00022663:
  if (*(char *)((uint)bVar4 << 0x10 | 0x2cc) == '\x01') {
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xa4e);
  }
  else {
    (*axis_lookup_interp)();
    sVar3 = table_lookup_interp(1,(void *)0x1f70);
    uVar2 = sVar3 << 2;
  }
  if (uVar2 < *(ushort *)((uint)bVar4 << 0x10 | 0xef74)) {
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xf076) = 0xffff;
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf224) & 0x200) == 0) {
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf17a);
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xa4a);
    if (*puVar1 <= uVar2 && uVar2 != *puVar1) {
      puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf224);
      *puVar1 = *puVar1 | 0x200;
    }
  }
  else {
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf17a);
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xa4c);
    if (uVar2 < *puVar1 || uVar2 == *puVar1) {
      puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf224);
      *puVar1 = *puVar1 & 0xfdff;
    }
  }
  if (*(short *)((uint)bVar4 << 0x10 | 0xf076) == 0) {
    return 0;
  }
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xef74) = 0;
  return *(ushort *)((uint)bVar4 << 0x10 | 0xf224) & 0x200;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort rpm_sync_ready_check(void)

{
  ushort ready;
  
  if (((DAT_0001ef6c < DAT_00010896) || ((g_diag_flags_eed6 & 0x80) == 0)) ||
     ((DAT_0001f5ee & 1) != 0)) {
    ready = 0;
  }
  else {
    ready = 1;
  }
  return ready;
}



bool closedloop_fuel_ready_check(void)

{
  short sVar1;
  ushort extraout_R2;
  byte bVar2;
  
  bVar2 = 1;
  if (((g_status_flags_f20e & 0x11) != 0) ||
     (sVar1 = table_lookup_indexed(2,(void *)0xda4c), extraout_R2 < (ushort)(sVar1 << 2))) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf016) = *(undefined2 *)((uint)bVar2 << 0x10 | 0x894);
  }
  return *(short *)((uint)bVar2 << 0x10 | 0xf016) == 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void o2_temp_ready_check(void)

{
  byte _unused_far_pad [2];
  undefined2 mode;
  
  if ((g_status_flags_f224 & 0x2000) == 0) {
    if ((ushort)BYTE_ARRAY_00010888 < (ushort)g_coolant_temp_f1e8) {
      g_status_flags_f224 = g_status_flags_f224 | 0x2000;
    }
  }
  else if ((ushort)g_coolant_temp_f1e8 <= (ushort)DAT_0001088a) {
    g_status_flags_f224 = g_status_flags_f224 & 0xdfff;
  }
  if ((g_status_flags_f1f2 & 0x40) != 0) {
    _unused_far_pad = g_coolant_temp_f1e8;
    if (((g_status_flags_f224 & 0x2000) == 0) ||
       (_unused_far_pad = engine_torque_pct_f17a,
       (ushort)engine_torque_pct_f17a <= (ushort)DAT_00010892)) {
      mode = 0;
    }
    else {
      mode = 1;
    }
    o2_ready_timer_update(_unused_far_pad,mode);
  }
  return;
}



short __stdcall_far o2_ready_timer_update(undefined2 _unused_far_pad,short mode)

{
  if (DAT_000102ca == '\0') {
    return mode;
  }
  if (((DAT_0001f212 & 0x11) != 0) || (g_tps_load_axis_f10e < DAT_0001088c)) {
    DAT_0001f228 = 0xffff;
    return mode;
  }
  if (mode == 0) {
    DAT_0001f018 = DAT_0001088e;
    if (engine_torque_pct_f17a < DAT_00010892) {
      DAT_0001f018 = DAT_0001088e;
      DAT_0001f228 = 0xffff;
      return 0;
    }
    if (DAT_0001f01a == 0) {
      DAT_0001f018 = DAT_0001088e;
      DAT_0001f228 = 0xffff;
      return 0;
    }
  }
  else if (DAT_0001f018 == 0) {
    DAT_0001f01a = DAT_00010890;
  }
  else {
    DAT_0001f01a = 0;
    if (mode == 0) {
      mode = 1;
    }
    else {
      mode = 0;
    }
  }
  DAT_0001f228 = DAT_000132b6;
  return mode;
}



void warmup_ign_advance_correction_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  table_lookup_interp(1,(void *)0x1ccc);
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf0f8) & 0x80) == 0) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf06a) = *(undefined2 *)((uint)bVar2 << 0x10 | 0x872);
  }
  if (((*(ushort *)((uint)bVar2 << 0x10 | 0xf3f0) & 0x20) == 0) &&
     ((*(ushort *)((uint)bVar2 << 0x10 | 0xf25a) & 0x4000) != 0)) {
    uVar1 = g_status_flags_f25a;
    g_status_flags_f25a = uVar1 & 0xbfff;
  }
  (*check_flag_f0f8_bit2_clear)();
  (*clamp_u8)();
  return;
}



void warmup_correction_f25a_f0f8_f212_calc(void)

{
  ushort uVar1;
  short sVar2;
  short unaff_R3;
  short unaff_FP;
  byte bVar3;
  undefined1 uVar4;
  
  uVar4 = 0;
  uVar1 = g_status_flags_f25a;
  g_status_flags_f25a = uVar1 & 0xbfff;
  uVar1 = DAT_0000f0f8;
  if (((uVar1 & 0x20) == 0) || (uVar1 = DAT_0000f212, (uVar1 & 4) != 0)) {
    unaff_R3 = unaff_R3 + 1;
  }
  *(short *)(unaff_FP + -2) = *(short *)(unaff_FP + -2) + (ushort)*(byte *)((int)unaff_R3 + 0x1cd8);
  bVar3 = 0;
  sVar2 = (*check_flag_f0f8_bit2_clear)();
  if (sVar2 != 0) {
    *(short *)(unaff_FP + -2) = *(short *)(unaff_FP + -2) + *(short *)((uint)bVar3 << 0x10 | 0x874);
  }
  (*clamp_u8)();
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: ram
// WARNING: Restarted to delay deadcode elimination for space: stack
// Params confirmed correct via get_function_variables (_unused_far_pad=<UNASSIGNED>,
// threshold=Stack[0x4]) despite __stdcall_far / far-call offset root cause fix applied. Decompiler
// signature/body text intermittently reverts to showing void() and raw stack0xfffe+6 -- this is a
// display-cache anomaly (force_decompile does not clear it), not a binding failure; storage is
// verified correct.

ushort __stdcall_far
f216_bits_f188_f17a_update_and_warmup_gate(undefined2 _unused_far_pad,ushort threshold)

{
  ushort uVar1;
  
  if ((g_status_flags_f216 & 0x80) == 0) {
    if (DAT_0001087c < DAT_0001f188) {
      g_status_flags_f216 = g_status_flags_f216 | 0x80;
    }
  }
  else if (DAT_0001f188 <= DAT_0001087a) {
    g_status_flags_f216 = g_status_flags_f216 & 0xff7f;
  }
  if ((g_status_flags_f216 & 0x8000) == 0) {
    if (DAT_000108ce < engine_torque_pct_f17a) {
      g_status_flags_f216 = g_status_flags_f216 | 0x8000;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_000108d0) {
    g_status_flags_f216 = g_status_flags_f216 & 0x7fff;
  }
  if ((g_status_flags_f216 & 0x80) != 0) {
    DAT_0001f026 = DAT_00010876 * 0x14;
  }
  if ((((DAT_000102c9 == '\0') || ((g_status_flags_f0f8 & 4) != 0)) &&
      ((g_status_flags_f216 & 0x40) == 0)) &&
     ((((g_status_flags_f1fe & 8) == 0 && (threshold < engine_torque_pct_f17a)) &&
      (DAT_0001f026 == 0)))) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort warmup_complete_gate_check(void)

{
  ushort uVar1;
  ushort in_stack_00000004;
  
  if ((((in_stack_00000004 < engine_torque_pct_f17a) && ((g_status_flags_f216 & 0x40) == 0)) &&
      ((ushort)(DAT_0001086a * 0x14) <= g_rpm_zone_dwell_counter_ef6a)) &&
     ((((g_status_flags_f3f0 & 0x20) == 0 &&
       ((g_status_flags_f0f8 & g_status_flags_f0fa & 0x80) != 0)) && (DAT_0001f23c == 0)))) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f20e_bit1_mirror_from_f1fe_bit3(void)

{
  if ((g_status_flags_f1fe & 8) == 0) {
    g_status_flags_f20e = g_status_flags_f20e & 0xfffd;
  }
  else {
    g_status_flags_f20e = g_status_flags_f20e | 2;
  }
  return;
}



void engine_mode_dispatch_and_f240_edge_counter(void)

{
  ushort *puVar1;
  short *psVar2;
  ushort uVar3;
  short sVar4;
  byte bVar5;
  ushort in_stack_00000004;
  
  bVar5 = 1;
  (*f240_counter_reset_and_f074_table_load)();
  (*f21c_bit12_update_dispatcher)();
  f21c_bit1_update_dispatcher();
  uVar3 = (*f224_bits01_update_and_efa8_edge_latch)();
  status_word_composite_multibit_update_via_table(uVar3);
  struct_bit11_toggle_from_gate_check(in_stack_00000004);
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf222) = 0;
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf21e) = 0;
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf20e) & 0x800) == 0) {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 & 0xff7f;
  }
  else {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 | 0x80;
  }
  if (((*(char *)((uint)bVar5 << 0x10 | 0x2cd) == '\x02') &&
      ((*(ushort *)((uint)bVar5 << 0x10 | 0xf20e) & 0x80) != 0)) &&
     ((*(ushort *)((uint)bVar5 << 0x10 | 0xf220) &
       (*(ushort *)((uint)bVar5 << 0x10 | 0xf21c) ^ *(ushort *)((uint)bVar5 << 0x10 | 0xf220)) &
      0x200) != 0)) {
    sVar4 = *(short *)((uint)bVar5 << 0x10 | 0xf240) + 1;
    *(short *)((uint)bVar5 << 0x10 | 0xf240) = sVar4;
    if (sVar4 == 0) {
      psVar2 = (short *)((uint)bVar5 << 0x10 | 0xf240);
      *psVar2 = *psVar2 + -1;
    }
    uVar3 = *(ushort *)((uint)bVar5 << 0x10 | 0xf240);
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0x8b4);
    if (uVar3 < *puVar1 || uVar3 == *puVar1) {
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf07e) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x8b0);
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf23e) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x8b2);
    }
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f240_counter_reset_and_f074_table_load(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = 0;
  if ((DAT_0001f212 & 0x11) != 0) {
    DAT_0001f07e = DAT_000108b0;
    DAT_0001f23e = DAT_000108b2;
    DAT_0001f240 = 0;
    uVar1 = DAT_000108b2;
  }
  if ((DAT_0001f212 & 1) != 0) {
    DAT_0001f0b0 = 0;
    DAT_0001f0ae = 0;
    uVar1 = table_lookup_interp(1,(void *)0x1d4c);
    *(ushort *)((uint)bVar2 << 0x10 | 0xf074) = uVar1;
  }
  return uVar1;
}



void f21c_bit12_update_dispatcher(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  (*f21c_bit9_hysteresis_and_f23e_table_ramp)();
  (*f21c_bit8_hysteresis_from_table_variant)();
  sVar2 = (*f21c_bit12_source_condition_check)();
  if (sVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf21c);
    *puVar1 = *puVar1 & 0xefff;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf21c);
    *puVar1 = *puVar1 | 0x1000;
  }
  return;
}



void f21c_bit9_hysteresis_and_f23e_table_ramp(void)

{
  ushort *puVar1;
  short *psVar2;
  ushort a;
  ushort extraout_R2;
  ushort uVar3;
  ushort uVar4;
  byte bVar5;
  ushort uStack_4;
  
  bVar5 = 1;
  uVar4 = inj_pw_output_f186;
  if ((g_status_flags_f1f0 & 1) == 0) {
    if (DAT_000108aa <= coolant_temp_scaled_f130) {
      uVar4 = inj_pw_output_f18c;
    }
  }
  (*axis_lookup_interp)();
  a = table_lookup_interp(1,(void *)0x1cf8);
  uStack_4 = sat_sub_u16(a,*(ushort *)((uint)bVar5 << 0x10 | 0x8ac));
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf21c) & 0x200) == 0) {
    if (a < uVar4) {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf21c);
      *puVar1 = *puVar1 | 0x200;
    }
  }
  else if (uVar4 <= uStack_4) {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf21c);
    *puVar1 = *puVar1 & 0xfdff;
  }
  if (*(char *)((uint)bVar5 << 0x10 | 0x2cd) != '\0') {
    uVar3 = extraout_R2;
    if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf21c) & 1) != 0) {
      uVar3 = uStack_4;
    }
    if (uVar3 < uVar4) {
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf07e) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x8b0);
      if (((*(ushort *)((uint)bVar5 << 0x10 | 0xf208) & 8) != 0) &&
         (*(short *)((uint)bVar5 << 0x10 | 0xf23e) != 0)) {
        psVar2 = (short *)((uint)bVar5 << 0x10 | 0xf23e);
        *psVar2 = *psVar2 + -1;
      }
      if (*(short *)((uint)bVar5 << 0x10 | 0xf23e) != 0) {
        a = table_lookup_interp(1,(void *)0x1d0c);
        uStack_4 = sat_sub_u16(a,*(ushort *)((uint)bVar5 << 0x10 | 0x8ac));
      }
    }
    else if (*(short *)((uint)bVar5 << 0x10 | 0xf07e) == 0) {
      uVar3 = *(ushort *)((uint)bVar5 << 0x10 | 0x8b2);
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf23e);
      if ((uVar3 < *puVar1 || uVar3 == *puVar1) || (*(short *)((uint)bVar5 << 0x10 | 0x8b0) == 0)) {
        *(ushort *)((uint)bVar5 << 0x10 | 0xf23e) = uVar3;
      }
      else {
        psVar2 = (short *)((uint)bVar5 << 0x10 | 0xf23e);
        *psVar2 = *psVar2 + 1;
      }
      *(undefined2 *)((uint)bVar5 << 0x10 | 0xf07e) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x8b0);
    }
  }
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf21c) & 1) == 0) {
    if (a < uVar4) {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf21c);
      *puVar1 = *puVar1 | 1;
    }
  }
  else if (uVar4 <= uStack_4) {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf21c);
    *puVar1 = *puVar1 & 0xfffe;
  }
  return;
}



void f21c_bit8_hysteresis_from_table_variant(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  void *table;
  
  bVar3 = 1;
  (*axis_lookup_interp)();
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0) {
    table = (void *)0x1d38;
  }
  else {
    table = (void *)0x1d24;
  }
  uVar2 = table_lookup_interp(1,table);
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf21c) & 0x100) == 0) {
    if (uVar2 < *(ushort *)((uint)bVar3 << 0x10 | 0xf13e)) {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf21c);
      *puVar1 = *puVar1 | 0x100;
    }
  }
  else {
    uVar2 = sat_sub_u16(uVar2,*(ushort *)((uint)bVar3 << 0x10 | 0x8b6));
    if (*(ushort *)((uint)bVar3 << 0x10 | 0xf13e) <= uVar2) {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf21c);
      *puVar1 = *puVar1 & 0xfeff;
    }
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f21c_bit12_source_condition_check(void)

{
  ushort uVar1;
  
  if (((((((g_status_flags_f1f2 & 0x10) == 0) || ((g_status_flags_f21c & 1) != 0)) ||
        ((g_status_flags_f21c & 0x100) != 0)) ||
       (((g_status_flags_f20e & 0x1b) != 0 || ((g_status_flags_001f25a & 0x20) != 0)))) ||
      (((DAT_0001f340 & 1) != 0 || (((g_status_flags_f216 & 0x400) != 0 || (DAT_0001f074 != 0))))))
     || (((engine_state_flags_f25e & 0x100) != 0 || ((DAT_000102cb != '\0' && (DAT_0001efa6 != 0))))
        )) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



void f21c_bit1_update_dispatcher(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = f21c_bit1_source_condition_check();
  if (uVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf21c);
    *puVar1 = *puVar1 & 0xfffd;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf21c);
    *puVar1 = *puVar1 | 2;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f21c_bit1_source_condition_check(void)

{
  ushort uVar1;
  
  if (((g_status_flags_f21c & 0x1000) == 0) && ((g_status_flags_f216 & 0x80) != 0)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



void f224_bits01_update_and_efa8_edge_latch(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort in_ram_000008e8;
  undefined2 in_ram_000008ea;
  ushort uVar3;
  
  uVar2 = g_status_flags_f224;
  if ((g_status_flags_f224 & 1) == 0) {
    if (DAT_000108e6 < g_tps_load_axis_f10e) {
      g_status_flags_f224 = g_status_flags_f224 | 1;
    }
  }
  else if (g_tps_load_axis_f10e <= DAT_000108e4) {
    g_status_flags_f224 = g_status_flags_f224 & 0xfffe;
  }
  uVar3 = DAT_0000f1b6;
  uVar1 = DAT_0000f1bc;
  if (uVar3 < uVar1) {
    uVar3 = uVar1 - uVar3;
  }
  else {
    uVar3 = uVar3 - uVar1;
  }
  if (0xfe < uVar3) {
    uVar3 = 0xff;
  }
  if (uVar3 < in_ram_000008e8) {
    uVar3 = DAT_0000f224;
    DAT_0000f224 = uVar3 & 0xfffd;
  }
  else {
    uVar3 = DAT_0000f224;
    DAT_0000f224 = uVar3 | 2;
  }
  uVar3 = DAT_0000f224;
  uVar1 = DAT_0000f224;
  if ((uVar1 & (uVar3 ^ uVar2) & 2) != 0) {
    g_f224_bit1_edge_latch_value_efa8 = in_ram_000008ea;
  }
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack
// Table-driven (arg = pointer to TABLE_STATUS_WORD_PTRS_137A8-style table, e.g. 0x37a8 from
// engine_mode_dispatch_and_f240_edge_counter). Reads table entries at offsets
// +4(idx2),+6(idx3=f21c-equivalent),+8(idx4),+0xe(idx7),+0x12(idx9),+0x22(idx17),+0x24(idx18),+0x26(idx19).
// Computes an 8-bit composite status word (bits 0,2,4,5,6,7,0xd,0xe) via threshold comparisons
// against fixed constants at 0x898/0x8a0/0x8a2/0x8e0/0x8e2, then writes the result back to table
// entry +6. This is the generic/master version of the individually-named f21c_bit* functions found
// earlier this session (f21c_bit1_update_dispatcher, f21c_bit8_hysteresis_from_table_variant,
// f21c_bit9_hysteresis_and_f23e_table_ramp, f21c_bit12_source_condition_check) -- strong candidate
// for cross-ROM fuzzy signature matching since only the table-pointer immediate would differ
// between builds.

void status_word_composite_multibit_update_via_table(ushort table_ptr_low16)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  undefined2 in_stack_00000004;
  
  uVar3 = **(ushort **)((int)in_stack_00000004 + 6);
  uVar1 = **(ushort **)((int)in_stack_00000004 + 0xe);
  if (((DAT_0001f212 & 0x10) != 0) ||
     ((ushort)(DAT_000108a2 * 0x14) < g_rpm_zone_dwell_counter_ef6a)) {
    uVar3 = uVar3 & 0xff67;
    if (**(ushort **)((int)in_stack_00000004 + 0x12) <= uVar1) {
      uVar3 = uVar3 | 0x80;
    }
    if (DAT_00010898 <= uVar1) {
      uVar3 = uVar3 | 0x10;
    }
    if ((DAT_0001f212 & 0x10) == 0) {
      uVar2 = **(ushort **)((int)in_stack_00000004 + 8) ^ uVar3;
      if ((uVar2 & 0x10) == 0) {
        if ((**(ushort **)((int)in_stack_00000004 + 4) & 0x800) == 0) goto LAB_00022d8a;
        if (**(short **)((int)in_stack_00000004 + 0x22) == 0) {
          uVar3 = uVar3 | 0x4000;
        }
      }
      else {
        uVar3 = uVar3 & 0xbfff;
LAB_00022d8a:
        **(undefined2 **)((int)in_stack_00000004 + 0x22) = DAT_000108a0;
      }
      if ((uVar2 & 0x80) == 0) {
        if ((**(ushort **)((int)in_stack_00000004 + 4) & 0x800) != 0) {
          if ((**(ushort **)((int)in_stack_00000004 + 8) & 0x2000) != 0) {
            **(undefined2 **)((int)in_stack_00000004 + 0x24) = DAT_000108a0;
          }
          if (**(short **)((int)in_stack_00000004 + 0x24) == 0) {
            uVar3 = uVar3 | 0x20;
          }
          goto LAB_00022dd3;
        }
      }
      else {
        **(undefined2 **)((int)in_stack_00000004 + 0x24) = DAT_000108a0;
        uVar3 = uVar3 & 0x7fdf;
      }
      uVar3 = uVar3 | 0x2000;
      goto LAB_00022dd9;
    }
    uVar3 = uVar3 | 0xc020;
  }
LAB_00022dd3:
  uVar3 = uVar3 & 0xdfff;
LAB_00022dd9:
  if (uVar1 < **(ushort **)((int)in_stack_00000004 + 0x12)) {
    uVar3 = uVar3 & 0xfffb;
  }
  else {
    uVar3 = uVar3 | 4;
  }
  if (((**(ushort **)((int)in_stack_00000004 + 8) ^ uVar3) & 4) != 0) {
    if (((g_status_flags_f224 & 1) == 0) || (DAT_0001efa8 != 0)) {
      **(undefined2 **)((int)in_stack_00000004 + 0x26) = DAT_000108e2;
    }
    else {
      **(undefined2 **)((int)in_stack_00000004 + 0x26) = DAT_000108e0;
    }
  }
  if (**(short **)((int)in_stack_00000004 + 0x26) == 0) {
    if ((uVar3 & 4) == 0) {
      uVar3 = uVar3 & 0xffbf;
    }
    else {
      uVar3 = uVar3 | 0x40;
    }
  }
  **(ushort **)((int)in_stack_00000004 + 6) = uVar3;
  return;
}



// Table-driven consumer of TABLE_STATUS_WORD_PTRS_137A8: reads table entry +2 (idx1,
// f20e-equivalent), calls f21c_bit1_and_struct_bit5_gate_check, sets/clears bit11 (0x800) of that
// entry. Name already accurate pre-table-discovery; confirmed consistent with the generic
// table-consumer family (status_word_bit3_conditional_update_via_table,
// status_word_composite_multibit_update_via_table).

void struct_bit11_toggle_from_gate_check(ushort param_fp1)

{
  ushort *puVar1;
  ushort uVar2;
  undefined1 uVar3;
  undefined2 in_stack_00000004;
  
  uVar3 = 1;
  uVar2 = f21c_bit1_and_struct_bit5_gate_check();
  puVar1 = *(ushort **)((int)in_stack_00000004 + 2);
  if (uVar2 == 0) {
    *puVar1 = *puVar1 & 0xf7ff;
  }
  else {
    *puVar1 = *puVar1 | 0x800;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f21c_bit1_and_struct_bit5_gate_check(void)

{
  undefined2 in_stack_00000004;
  byte abStack_4 [2];
  
  if (((g_status_flags_f21c & 2) != 0) && ((**(ushort **)((int)in_stack_00000004 + 6) & 0x20) == 0))
  {
    if ((g_status_flags_f0f8 & 0x20) == 0) {
      abStack_4 = DAT_0001150e;
    }
    else {
      abStack_4 = BYTE_ARRAY_000108a4;
    }
    if (((ushort)abStack_4 <= (ushort)g_tps_load_axis_f10e) &&
       ((**(ushort **)((int)in_stack_00000004 + 0x56) & 0xf0) == 0)) {
      return 1;
    }
  }
  return 0;
}



void f20e_f210_clear_chain_and_o2_closedloop_gate(void)

{
  ushort *puVar1;
  undefined1 extraout_var;
  bool bVar2;
  ushort extraout_R2;
  ushort uVar3;
  ushort uVar4;
  byte bVar5;
  
  bVar5 = 1;
  uVar3 = g_status_flags_f20e;
  uVar4 = DAT_0001f210;
  if ((g_status_flags_f20e & 0x10) != 0) {
    uVar3 = g_status_flags_f20e & 0xf472;
    uVar4 = DAT_0001f210 & 0xf7ff;
  }
  if ((uVar3 & 0x100) != 0) {
    uVar3 = uVar3 & 0xf56a;
    uVar4 = uVar4 & 0xf7ff;
  }
  if ((uVar3 & 1) != 0) {
    uVar3 = uVar3 & 0xf463;
    uVar4 = uVar4 & 0xf7ff;
  }
  if ((uVar3 & 0x200) != 0) {
    uVar3 = uVar3 & 0xf66a;
    uVar4 = uVar4 & 0xf7ff;
  }
  if ((uVar3 & 4) != 0) {
    uVar3 = uVar3 & 0xf46e;
    uVar4 = uVar4 & 0xf7ff;
  }
  if ((uVar3 & 2) != 0) {
    uVar4 = uVar4 & 0xf7ff;
  }
  bVar2 = o2_closed_loop_config_check();
  uVar3 = extraout_R2;
  if (CONCAT11(extraout_var,bVar2) != 0) {
    uVar3 = extraout_R2 & 0xf77f;
    uVar4 = uVar4 & 0xf7ff;
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf21c);
    *puVar1 = *puVar1 | 0x1000;
  }
  *(ushort *)((uint)bVar5 << 0x10 | 0xf20e) = uVar3;
  *(ushort *)((uint)bVar5 << 0x10 | 0xf210) = uVar4;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly + bank-prefixed xrefs]
// o2_sensor2_threshold_flag_update_f226_bit13: reads MUT O2 Sensor 2 value @0xF122
// (bank-prefixed write from adc_read_sequence_main channel 4, confirmed via xref to
// 0x1f122). MUT RequestID 0x3C -> table entry 0xF123 (low byte of the 0xF122/F123
// word). VERIFIED - logging.txt CONFIRMED-section claim holds under current decoder.

void o2_sensor2_threshold_flag_update_f226_bit13(void)

{
  if ((g_status_flags_f20e & 8) == 0) {
    if ((DAT_0001031d == '\0') || ((g_status_flags_f224 & 4) == 0)) {
      DAT_0001f022 = DAT_000108a6;
    }
    else {
      DAT_0001f022 = DAT_00011478;
    }
  }
  else if (DAT_0001f022 == 0) {
    if (DAT_0001031d == '\0') {
      DAT_0001f024 = DAT_000108a8;
    }
    else {
      DAT_0001f024 = DAT_0001147c;
    }
  }
  if ((((DAT_0001031d == '\0') || (DAT_0001f024 == 0)) || (DAT_0001f01e != 0)) ||
     (DAT_0001147a <= DAT_0001f122)) {
    g_status_flags_f226 = g_status_flags_f226 & 0xdfff;
    return;
  }
  g_status_flags_f226 = g_status_flags_f226 | 0x2000;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f224_bit10_update_from_f20e_check(void)

{
  if ((g_status_flags_f20e & 0x11) == 0) {
    g_status_flags_f224 = g_status_flags_f224 | 0x400;
  }
  return g_status_flags_f20e & 0x11;
}



void init_state_block_ee98_80_call_wrapper(void)

{
  init_state_block_ee98_80();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void init_state_block_ee98_80(void)

{
  DAT_0001eeb2 = 0x80;
  DAT_0001eeb0 = 0x80;
  DAT_0001eeae = 0x80;
  DAT_0001ee9c = 0x80;
  DAT_0001ee9a = 0x80;
  g_knock_octane_trim_ee98 = 0x80;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// TRIAGE NOTE: copies DAT_00010956 into f2f0/f2f2/f2f4/f2f6/f2f8 (5 identical writes -- looks like
// resetting a small ring/history buffer to a common init value), sets f25a=0x10, computes f278/f276
// from 0x131e4<<8 and f27c/f27a from TABLE_2D_0x131d4[4]<<8, and f2cc from (0x11df0 *
// unnamed_ram_param)>>2, returning the high word of that same product. Called early in the periodic
// dispatch chain -- likely a per-cycle re-init of a small state/history block feeding f27x/f2cc.
// Not renamed: the in_ram_00000800 parameter source and TABLE_2D_000131D4 identity are unconfirmed.
// Confidence: low.

void f2f0_f2f8_ring_and_f27x_f2cc_periodic_init(void)

{
  short in_ram_00000800;
  
  DAT_0001f2f8 = DAT_00010956;
  DAT_0001f2f2 = DAT_00010956;
  DAT_0001f2f0 = DAT_00010956;
  DAT_0001f2f6 = DAT_00010956;
  DAT_0001f2f4 = DAT_00010956;
  g_status_flags_001f25a = 0x10;
  DAT_0000f278 = (ushort)DAT_000131e4 << 8;
  DAT_0000f276 = (ushort)DAT_000131e4 << 8;
  DAT_0000f27c = (ushort)DAT_000131d8 << 8;
  DAT_0000f27a = (ushort)DAT_000131d8 << 8;
  DAT_0000f2cc = (ushort)DAT_00011df0 * in_ram_00000800 >> 2;
  return;
}



void engine_periodic_correction_master_dispatch(void)

{
  knock_octane_trim_master_dispatch();
  knock_octane_secondary_dispatch();
  (*f0ba_f2d0_o2_mode7_8_correction_calc)();
  f2d2_composite_correction_calc();
  f2ce_f2cc_o2_mode11_correction_calc();
  injpw_airvol_reset_on_fuelcut();
  f24e_f254_f970_f972_reset_on_f20e_bit4();
  f2e0_f2ee_correction_dispatch();
  f310_f316_composite_calc();
  f250_f252_channel_snapshot_update();
  f31c_bit_toggle_pulse();
  f25a_f2xx_state_mirror_update();
  f856_f858_clamped_calc();
  return;
}



void knock_octane_trim_master_dispatch(void)

{
  undefined2 uVar1;
  byte bVar2;
  ushort in_stack_00000004;
  ushort in_stack_00000006;
  
  bVar2 = 1;
  (*f25e_gate_chain_dispatch_b)();
  f28e_f292_knock_lambda_composite_calc();
  (*f2a0_f2c6_dual_timer_decay_and_f2c8_blend_dispatch)();
  f2b6_knock_retard_state_select();
  (*f2ca_table_override_from_f114_gate)();
  o2_closed_loop_fuel_trim_compute(in_stack_00000004);
  f266_knock_octane_composite_trim_compute(in_stack_00000006);
  (*muldiv_s16_rounded)
            ((short)((uint)*(ushort *)((uint)bVar2 << 0x10 | 0xf2a4) *
                     (uint)*(ushort *)((uint)bVar2 << 0x10 | 0xf29c) >> 0x10));
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf5be) = uVar1;
  return;
}



void f25e_gate_chain_dispatch_b(void)

{
  (*f25e_gate_chain_dispatch_a)();
  (*knock_octane_dual_axis_blend_far_trampoline)();
  return;
}



void f25e_gate_chain_dispatch_a(void)

{
  (*f07c_multi_channel_threshold_gate_check)();
  (*f25e_bit10_update_from_f216_gate)();
  f25e_bit11_update_from_f224_gate();
  (*status_word_bit3_conditional_update_via_table)();
  f25e_bit8_update_from_f110_gate();
  (*f25e_bit14_update_from_f186_f17a_gate)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// Checks 4 channel pairs (f17a/0x0d84, f186/0x0d82, f10e/0x0d86, f118/0x0d88); if any pair fails
// its <= comparison, writes 2*0x0d8a into f07c. Pattern matches sibling threshold-gate functions
// (rpm_load_window_condition_check etc). Confidence: medium.

void f07c_multi_channel_threshold_gate_check(void)

{
  if ((((engine_torque_pct_f17a <= DAT_00010d84) || (inj_pw_output_f186 <= DAT_00010d82)) ||
      (g_tps_load_axis_f10e <= DAT_00010d86)) || (g_iat_axis_f118 <= DAT_00010d88)) {
    DAT_0001f07c = DAT_00010d8a << 1;
  }
  return;
}



// If flag f1f2 bit4 clear, calls f216_bit5_hysteresis_gate_check and sets f25e bit10 (0x400) on
// nonzero result; otherwise clears f25e bit10. Confidence: medium-high (mirrors 000231da/f224 pair
// exactly).

void f25e_bit10_update_from_f216_gate(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (((g_status_flags_f1f2 & 0x10) == 0) &&
     (sVar2 = (*f216_bit5_hysteresis_gate_check)(), sVar2 != 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
    *puVar1 = *puVar1 | 0x400;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
  *puVar1 = *puVar1 & 0xfbff;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// Hysteresis gate: sets/clears f216 bit5 (0x20) by comparing 0x09cc/0x09ce against f18c; returns 1
// only when bit5 clear, low 5 bits of f216 all clear, and 0x09ca < f10e. Called by
// f25e_bit10_update_from_f216_gate. Confidence: medium.

ushort f216_bit5_hysteresis_gate_check(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f216 & 0x20) == 0) {
    if (DAT_000109cc < inj_pw_output_f18c) {
      g_status_flags_f216 = g_status_flags_f216 | 0x20;
    }
  }
  else if (inj_pw_output_f18c <= DAT_000109ce) {
    g_status_flags_f216 = g_status_flags_f216 & 0xffdf;
  }
  if ((((g_status_flags_f216 & 0x20) == 0) && ((g_status_flags_f216 & 0x1f) == 0)) &&
     (DAT_000109ca < g_tps_load_axis_f10e)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



// If flag f1f4 bit0 set, calls f224_bit11_hysteresis_timer_and_gate_check and sets f25e bit11
// (0x800) on nonzero result; otherwise clears f25e bit11. Mirrors f25e_bit10_update_from_f216_gate
// structurally.

void f25e_bit11_update_from_f224_gate(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (((DAT_0001f1f4 & 1) == 0) ||
     (uVar2 = f224_bit11_hysteresis_timer_and_gate_check(), uVar2 == 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
    *puVar1 = *puVar1 & 0xf7ff;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
  *puVar1 = *puVar1 | 0x800;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// Uses axis_lookup_interp/table_lookup_interp(0x14656) against DAT_0001f2b2, compares result to
// f18c/f10e, runs a saturating bank1 counter at 0xefb4 (capped 0..0xff) gated by f224 bit11 (0x800)
// and 0x09e8. Sets/clears f224 bit11 on threshold crossing. Returns 1 only when f10e>=0x09e2, f20e
// bit1 clear, and f224 bit11 set -- an armed hysteresis/ramp-timer gate, likely EGR or ISC related
// given proximity to isc_/egr_ functions. Confidence: medium (semantics of table 0x1f64 axis not
// confirmed).

ushort f224_bit11_hysteresis_timer_and_gate_check(void)

{
  ushort *puVar1;
  undefined2 in_R0;
  ushort uVar2;
  undefined2 uVar3;
  ushort uVar4;
  undefined2 uVar5;
  undefined2 in_R1;
  ushort extraout_R2;
  ushort extraout_R2_00;
  byte bVar6;
  
  bVar6 = 1;
  (*axis_lookup_interp)(in_R0,in_R1,DAT_0001f2b2,2,0xd2aa);
  uVar2 = table_lookup_interp(1,(void *)0x1f64);
  uVar3 = table_lookup_interp(1,(void *)0x1f64);
  uVar4 = sat_sub_u16(uVar2,*(ushort *)((uint)bVar6 << 0x10 | 0x9e0));
  if ((*(ushort *)((uint)bVar6 << 0x10 | 0xf224) & 0x800) == 0) {
    if (uVar2 < *(ushort *)((uint)bVar6 << 0x10 | 0xf18c)) {
      puVar1 = (ushort *)((uint)bVar6 << 0x10 | 0xf224);
      *puVar1 = *puVar1 | 0x800;
    }
  }
  else if (*(ushort *)((uint)bVar6 << 0x10 | 0xf18c) <= uVar4) {
    puVar1 = (ushort *)((uint)bVar6 << 0x10 | 0xf224);
    *puVar1 = *puVar1 & 0xf7ff;
  }
  uVar2 = extraout_R2;
  if ((*(ushort *)((uint)bVar6 << 0x10 | 0xf224) & 0x800) == 0) {
    if (*(short *)((uint)bVar6 << 0x10 | 0xefb6) == 0) {
      uVar5 = table_lookup_interp(1,(void *)0x1f64);
      *(undefined2 *)((uint)bVar6 << 0x10 | 0xefb6) = uVar5;
      uVar2 = extraout_R2_00;
    }
  }
  else {
    if (*(ushort *)((uint)bVar6 << 0x10 | 0xf17a) < *(ushort *)((uint)bVar6 << 0x10 | 0x9e8)) {
      if (*(short *)((uint)bVar6 << 0x10 | 0xefb4) != 0) goto LAB_0002329d;
      uVar2 = extraout_R2 + 1;
      if (uVar2 < 0x100) goto LAB_00023297;
    }
    uVar2 = 0xff;
  }
LAB_00023297:
  *(undefined2 *)((uint)bVar6 << 0x10 | 0xefb4) = uVar3;
LAB_0002329d:
  *(ushort *)((uint)bVar6 << 0x10 | 0xf2b2) = uVar2;
  if (((*(ushort *)((uint)bVar6 << 0x10 | 0xf10e) < *(ushort *)((uint)bVar6 << 0x10 | 0x9e2)) ||
      ((*(ushort *)((uint)bVar6 << 0x10 | 0xf20e) & 2) != 0)) ||
     ((*(ushort *)((uint)bVar6 << 0x10 | 0xf224) & 0x800) == 0)) {
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}



// WARNING: Restarted to delay deadcode elimination for space: stack
// CORRECTED (was flagged low-confidence as per-cylinder struct): the stack param (e.g. 0x37a8) is a
// pointer to TABLE_STATUS_WORD_PTRS_137A8, an array of addresses of various status/flag words
// (f20e,f21c,f25e,f224,f266,f2a4,ee8c,f070...). This function reads table entry #5 (offset+10 baked
// into the code = f25e), applies a 4-way threshold gate (0000102f5 / f1f2 bit4 / f20e bit4+bit0 /
// f10e,f118 vs 0x0d58,0x0d5a), and sets/clears bit3 of that word. GENERIC/TEMPLATE PATTERN: the
// table pointer is the only real argument; the target entry offset is compiled in. This looks like
// compiler-generated boilerplate that would recur near-identically (modulo the table-address
// immediate) across other ROM builds -- good candidate for cross-ROM fuzzy signature matching.

void __stdcall_far
status_word_bit3_conditional_update_via_table(undefined2 _unused_far_pad,undefined2 table_offset)

{
  ushort uVar1;
  
  uVar1 = **(ushort **)((int)table_offset + 10);
  if (DAT_000102f5 != '\0') {
    if ((((g_status_flags_f20e & 0x10) == 0) &&
        ((**(ushort **)((int)table_offset + 2) & 0x800) == 0)) &&
       ((g_status_flags_f1f2 & 0x10) != 0 ||
        g_rpm_zone_dwell_counter_ef6a < (ushort)(DAT_00010d60 * 0x14))) {
      if ((g_status_flags_f20e & 1) == 0) goto LAB_00023326;
      if ((DAT_00010d58 < g_tps_load_axis_f10e) && (DAT_00010d5a < g_iat_axis_f118)) {
        uVar1 = uVar1 | 8;
        goto LAB_00023326;
      }
    }
  }
  uVar1 = uVar1 & 0xfff7;
LAB_00023326:
  **(ushort **)((int)table_offset + 10) = uVar1;
  return;
}



void f25e_bit8_update_from_f110_gate(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = f110_f17a_threshold_gate_check();
  if (uVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
    *puVar1 = *puVar1 & 0xfeff;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf25e);
    *puVar1 = *puVar1 | 0x100;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f110_f17a_threshold_gate_check(void)

{
  ushort uVar1;
  
  uVar1 = DAT_000109ee;
  if ((engine_state_flags_f25e & 0x100) != 0) {
    uVar1 = DAT_000109f0;
  }
  if ((g_tps_load_axis_f110 < uVar1) || (engine_torque_pct_f17a < DAT_000109ec)) {
    return 0;
  }
  return 1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f25e_bit14_update_from_f186_f17a_gate(void)

{
  if ((((g_status_flags_f1f2 & 0x80) == 0) || (inj_pw_output_f186 < DAT_00010b4c)) ||
     (engine_torque_pct_f17a < DAT_00010b4a)) {
    engine_state_flags_f25e = engine_state_flags_f25e & 0xbfff;
    return;
  }
  engine_state_flags_f25e = engine_state_flags_f25e | 0x4000;
  return;
}



// DECOMPILER ARTIFACT SUSPECTED: decompiles as an unconditional infinite self-call with no base
// case (FUN_00023397 calling FUN_00023397). Almost certainly a far-call target mis-resolved due to
// the known CP-register limitation in the README (CP not modeled as a SLEIGH context var). Needs
// disassembly-level check of the actual call target bytes before naming/trusting. DO NOT rename
// until re-analyzed post CP_ctx fix.

void knock_octane_dual_axis_blend_far_trampoline(void)

{
  (*knock_octane_dual_axis_table_blend_f2a8)();
  return;
}



// TRIAGE NOTE: writes f0ba from f110_f17a_threshold_gate_check() [now-named]; then does 2x
// axis_lookup_interp + 2x table_lookup_indexed (table selected by f0f8 bit5), optionally
// weighted_blend_u8frac if f1f2 bit7 set; final result -> f2a8. Likely a knock/octane correction
// table blend feeding the f2a0-f2c8 cluster. Not renamed: table identities
// (0xd2e0/0xd302/0xd9cc/0xd9ac/0xd98c/0xd96c) not confirmed against EcuFlash defs. Confidence:
// low-medium.

void knock_octane_dual_axis_table_blend_f2a8(void)

{
  ushort uVar1;
  undefined2 uVar2;
  byte bVar3;
  void *pvVar4;
  
  bVar3 = 1;
  uVar1 = f190_f196_select_by_f1f0_f21c_gate();
  *(ushort *)((uint)bVar3 << 0x10 | 0xf0ba) = uVar1;
  (*axis_lookup_interp)();
  (*axis_lookup_interp)();
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0) {
    pvVar4 = (void *)0xd9cc;
  }
  else {
    pvVar4 = (void *)0xd9ac;
  }
  uVar2 = table_lookup_indexed(2,pvVar4);
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf1f2) & 0x80) != 0) {
    if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0) {
      pvVar4 = (void *)0xd98c;
    }
    else {
      pvVar4 = (void *)0xd96c;
    }
    table_lookup_indexed(2,pvVar4);
    uVar2 = (*weighted_blend_u8frac)();
  }
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2a8) = uVar2;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f190_f196_select_by_f1f0_f21c_gate(void)

{
  ushort uVar1;
  
  if (((g_status_flags_f1f0 & 0x11) == 0) && ((g_status_flags_f21c & 2) != 0)) {
    uVar1 = DAT_0001f196;
  }
  else {
    uVar1 = DAT_0001f190;
  }
  return uVar1;
}



// TRIAGE NOTE: computes f292 (table lookup + 0x818 offset), f296 via o2_lambda_correction_apply,
// f0ba (table*0xcd scaled via div_u16_rounded), f294 (table lookup), f290 (muldiv of f294*f296 vs
// f292), f28e (muldiv of read_indexed_byte_f1fc()*0x808 vs f290). Reads/writes overlap with
// f2a8_correction_table_blend area -- likely knock/lambda composite correction calc chain feeding
// 0002392c. Not renamed: too many unconfirmed table refs (0xd218/0xd248/0xd20a/0xd9ac-family).
// Confidence: low.

void f28e_f292_knock_lambda_composite_calc(void)

{
  short sVar1;
  ushort uVar2;
  undefined2 uVar3;
  byte bVar4;
  undefined4 uStack0008;
  
  bVar4 = 1;
  (*axis_lookup_interp)();
  sVar1 = table_lookup_interp(1,(void *)0x1800);
  *(short *)((uint)bVar4 << 0x10 | 0xf292) = sVar1 + *(short *)((uint)bVar4 << 0x10 | 0x818);
  table_lookup_interp(1,(void *)0x181a);
  uVar2 = o2_lambda_correction_apply();
  *(ushort *)((uint)bVar4 << 0x10 | 0xf296) = uVar2;
  uVar2 = table_lookup_interp(1,(void *)0x1834);
  uVar3 = (*div_u16_rounded)((short)((uint)uVar2 * 0xcd >> 0x10));
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf0ba) = uVar3;
  (*axis_lookup_interp)();
  (*axis_lookup_interp)();
  uVar3 = table_lookup_interp(1,(void *)0x1840);
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf294) = uVar3;
  uVar3 = (*muldiv_s16_rounded)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf290) = uVar3;
  uStack0008 = CONCAT22(0x18ab,uStack0008._2_2_);
  read_indexed_byte_f1fc(0x18,uStack0008._1_2_);
  uVar3 = (*muldiv_s16_rounded)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf28e) = uVar3;
  return;
}



void f2a0_f2c6_dual_timer_decay_and_f2c8_blend_dispatch(void)

{
  short *psVar1;
  undefined2 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((g_status_flags_f20e & 0x11) != 0) {
    uVar2 = table_lookup_interp(1,(void *)0x1db4);
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2a2) = uVar2;
    uVar2 = table_lookup_interp(1,(void *)0x1db4);
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2c0) = uVar2;
    uVar2 = table_lookup_interp(1,(void *)0x1db4);
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2c2) = uVar2;
    uVar2 = table_lookup_interp(1,(void *)0x1db4);
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2c4) = uVar2;
  }
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 0x10) == 0) {
    if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 1) != 0) {
      uVar2 = table_lookup_interp(1,(void *)0x1da6);
      *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2a0) = uVar2;
      uVar2 = table_lookup_interp(1,(void *)0x1da6);
      *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2c6) = uVar2;
    }
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2a0) = 0;
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2c6) = 0;
  }
  if ((*(short *)((uint)bVar3 << 0x10 | 0xf2a0) != 0) &&
     (*(short *)((uint)bVar3 << 0x10 | 0xefba) == 0)) {
    psVar1 = (short *)((uint)bVar3 << 0x10 | 0xf2a0);
    *psVar1 = *psVar1 + -1;
    efba_timer_target_select_from_f2a0_thresholds();
  }
  if ((*(short *)((uint)bVar3 << 0x10 | 0xf2c6) != 0) &&
     (*(short *)((uint)bVar3 << 0x10 | 0xefbc) == 0)) {
    psVar1 = (short *)((uint)bVar3 << 0x10 | 0xf2c6);
    *psVar1 = *psVar1 + -1;
    efbc_timer_target_select_from_f2c6_thresholds();
  }
  if (*(char *)((uint)bVar3 << 0x10 | 0x309) == '\0') {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2c8) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2a0);
  }
  else {
    uVar2 = (*weighted_blend_u8frac)();
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2c8) = uVar2;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void efba_timer_target_select_from_f2a0_thresholds(void)

{
  if (DAT_00010309 == '\0') {
    if (DAT_0001f2a0 <= DAT_0001083c) {
      g_efba_timer_target = DAT_0001f2a2;
      return;
    }
  }
  else if (DAT_0001f2a0 <= DAT_0001083a) {
    if (DAT_0001083c < DAT_0001f2a0) {
      g_efba_timer_target = DAT_0001f2c2;
      return;
    }
    g_efba_timer_target = DAT_0001f2a2;
    return;
  }
  g_efba_timer_target = DAT_00010842;
  if (g_tps_load_axis_f10e <= DAT_00010846) {
    g_efba_timer_target = DAT_00010844;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void efbc_timer_target_select_from_f2c6_thresholds(void)

{
  if (DAT_00010848 < DAT_0001f2c6) {
    g_efbc_timer_target = DAT_0001084c;
    if (g_tps_load_axis_f10e <= DAT_00010850) {
      g_efbc_timer_target = DAT_0001084e;
    }
  }
  else {
    g_efbc_timer_target = DAT_0001f2c0;
    if (DAT_0001084a < DAT_0001f2c6) {
      g_efbc_timer_target = DAT_0001f2c4;
    }
  }
  return;
}



// TRIAGE NOTE: complex condition-code (SR/CF) manipulation gated on DAT_000102ce and f20e bits,
// comparing f10e/000109da, f17a/0000109de, f174/0000109d8; falls back to a lookup at 0x1f08 indexed
// by f29e (capped at 8) when f20e bit2 clear and f0f8 bits 0x80/0x10 in a specific state. Writes
// f2b6. Reads like a knock-window / ignition-retard state machine. Not renamed: condition-flag
// semantics (in_SR/in_CF) not fully traced. Confidence: low.

ushort f2b6_knock_retard_state_select(ushort param_1)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = 0x80;
  if (DAT_000102ce == '\0') goto LAB_0002374d;
  param_1 = g_status_flags_f20e & 0x11;
  if (param_1 == 0) {
    if ((g_status_flags_f20e & 4) != 0) {
      param_1 = g_tps_load_axis_f10e;
      if (DAT_000109da < g_tps_load_axis_f10e) {
        param_1 = engine_torque_pct_f17a;
        if (engine_torque_pct_f17a < DAT_000109de) {
          param_1 = DAT_0001f174;
          if (DAT_000109d8 < DAT_0001f174) {
            DAT_0000f29e = 8;
            bVar3 = 0;
          }
          goto LAB_0002371b;
        }
      }
      DAT_0000f29e = 0;
      bVar3 = 0;
    }
  }
  else {
    DAT_0000f29e = 0;
    bVar3 = 0;
  }
LAB_0002371b:
  if (((((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 4) == 0) &&
       ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x80) != 0)) &&
      ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x10) == 0)) &&
     (uVar1 = *(ushort *)((uint)bVar3 << 0x10 | 0xf29e),
     *(short *)((uint)bVar3 << 0x10 | 0xf29e) != 0)) {
    uVar2 = 8;
    if (uVar1 < 8) {
      uVar2 = uVar1;
    }
    uVar2 = (ushort)*(byte *)((int)uVar2 + 0x1f08);
    bVar3 = 0;
  }
LAB_0002374d:
  *(ushort *)((uint)bVar3 << 0x10 | 0xf2b6) = uVar2;
  return param_1;
}



// TRIAGE NOTE: if DAT_0001031e set and f20e bits 0x11 clear, and f114 >= 0x11482 and f1fe bit3
// clear, does axis_lookup_interp then table_lookup_interp(0x14656) to override default 0x80; result
// -> f2ca. Simple threshold-gated table override, similar shape to efba/efbc selectors above.
// Confidence: medium -- could rename next pass as f2ca_table_override_from_f114_gate.

ushort f2ca_table_override_from_f114_gate(ushort param_1)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = 0x80;
  if ((DAT_0001031e != '\0') && (param_1 = g_status_flags_f20e & 0x11, param_1 == 0)) {
    param_1 = g_tps_load_axis_f10e_prev;
    if ((DAT_00011482 <= g_tps_load_axis_f10e_prev) && ((g_status_flags_f1fe & 8) == 0)) {
      (*axis_lookup_interp)();
      param_1 = table_lookup_interp(1,(void *)0x1d86);
      uVar1 = param_1;
    }
  }
  *(ushort *)((uint)bVar2 << 0x10 | 0xf2ca) = uVar1;
  return param_1;
}



// WARNING: Restarted to delay deadcode elimination for space: stack
// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly + bank-prefixed xrefs]
// o2_closed_loop_fuel_trim_compute: reads MUT O2 Sensor value @0xF11E (bank-prefixed
// write from adc_read_sequence_main channel 3, confirmed via xref to 0x1f11e - plain
// xref lookup on 0xf11e returns nothing, must use bank-prefixed address per the
// "invisible to plain xref" lesson in logging.txt).
// MUT RequestID 0x13 -> table entry 0xF11F (low byte of the 0xF11E/F11F word written
// here) = O2 Sensor. VERIFIED - logging.txt CONFIRMED-section claim holds under current
// decoder. Decompiler output for this function is currently untrustworthy (stack-var-as-
// address artifacts, CONCAT12/ZEXT24 garbage) - use disassembly for further analysis here,
// not decompile_function.

void o2_closed_loop_fuel_trim_compute(ushort param_fp1)

{
  undefined1 extraout_var;
  bool bVar1;
  ushort uVar2;
  byte bVar3;
  ushort unaff_retaddr;
  undefined2 in_stack_00000004;
  
  bVar3 = 1;
  if ((**(ushort **)((int)in_stack_00000004 + 2) & 0x800) == 0) {
    if ((engine_state_flags_f25e & 0x400) == 0) {
      if (((engine_state_flags_f25e & 0x4000) != 0) &&
         (bVar1 = o2_downstream_type34_check(), CONCAT11(extraout_var,bVar1) == 0)) {
        uVar2 = sat_sub_u16(*(ushort *)((uint)bVar3 << 0x10 | 0xf3a2),
                            *(ushort *)((uint)bVar3 << 0x10 | 0xb46));
        get_high_byte(uVar2 * *(short *)((uint)bVar3 << 0x10 | 0xb48));
      }
      if ((*(char *)((uint)bVar3 << 0x10 | 0x2cf) != '\0') &&
         (*(short *)((uint)bVar3 << 0x10 | 0xf07c) == 0)) {
        (*mul_u16_x2_sat)();
      }
    }
    else {
      uVar2 = DAT_0001f11e;
      if (DAT_0001f11e < DAT_000109d6) {
        uVar2 = 0x80;
      }
      clamp_u16(uVar2,DAT_000109d2,DAT_000109d4);
      (*clamp_u8)();
    }
    if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf226) & 2) != 0) {
      (*clamp_u8)();
    }
    if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf25e) & 0x800) != 0) {
      f0ba_f2b2_threshold_blend_calc(unaff_retaddr);
    }
    if ((**(ushort **)((int)in_stack_00000004 + 10) & 8) != 0) {
      sat_sub_u16(*(short *)((uint)bVar3 << 0x10 | 0xd60) * 0x14,
                  *(ushort *)((uint)bVar3 << 0x10 | 0xef6a));
      (*muldiv_s16_rounded)();
    }
  }
  uVar2 = o2_sensor_mode_correction_select();
  **(ushort **)((int)in_stack_00000004 + 0x28) = uVar2;
  return;
}



// TRIAGE NOTE: if 0000109e4<=f17a, writes f0ba from param +
// axis_lookup_interp/table_lookup(0x14656); then if result <= param, blends via
// weighted_blend_u8frac using f2b2. Part of the same f0ba/f2b2 correction-blend family as
// f224_bit11_hysteresis_timer_and_gate_check. Not renamed: exact physical meaning of the blend
// inputs unconfirmed. Confidence: low-medium.

byte  [2] f0ba_f2b2_threshold_blend_calc(ushort param_fp1)

{
  byte abVar1 [2];
  byte in_stack_00000004 [2];
  
  abVar1 = BYTE_ARRAY_000109e6;
  if ((ushort)BYTE_ARRAY_000109e4 <= (ushort)engine_torque_pct_f17a) {
    g_axis_lookup_input_scratch_f0ba = in_stack_00000004;
    (*axis_lookup_interp)();
    abVar1 = (byte  [2])table_lookup_interp(1,(void *)0x1f68);
  }
  if ((ushort)abVar1 <= (ushort)in_stack_00000004) {
    in_stack_00000004 = (byte  [2])(*weighted_blend_u8frac)();
  }
  return in_stack_00000004;
}



// WARNING: Heritage AFTER dead removal. Example location: s0xfffa : 0x00023955
// WARNING: Restarted to delay deadcode elimination for space: stack
// WARNING: Exceeded maximum restarts with more pending
// CORRECTED (was flagged low-confidence): stack param 0x37a8 -> TABLE_STATUS_WORD_PTRS_137A8; this
// function's final store targets table entry #21 (offset+0x2a baked in = f266), same
// table-pointer-argument pattern as status_word_bit3_conditional_update_via_table. Computes a
// composite knock/octane trim from f298/f29a/f28e/f2b6/f2c8/f2be/f2bc/f482 via muldiv_u16_scaled,
// mul_u16_sat_s16, sat_add_u16, inc_if_signed_flag, gated by flags 0x31e/0x309/0x30a -- result
// written to f266 through the table indirection. Confidence: medium-high on data flow, output
// register confirmed via table entry math.

void f266_knock_octane_composite_trim_compute(ushort param_fp1)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  ushort b;
  short sVar4;
  ushort extraout_R1;
  ushort extraout_R1_00;
  short sVar5;
  ushort b_00;
  ushort extraout_R1_01;
  ushort extraout_R1_02;
  byte bVar6;
  undefined2 in_stack_00000004;
  ushort in_stack_0000fffa;
  ushort uStack_4;
  
  bVar6 = 1;
  (*identity_passthrough_arg1)();
  uVar1 = muldiv_u16_scaled(in_stack_0000fffa,uStack_4,
                            *(short *)((uint)bVar6 << 0x10 | 0xf29a) *
                            *(short *)((uint)bVar6 << 0x10 | 0xf298),0x4000);
  if (*(char *)((uint)bVar6 << 0x10 | 0x31e) == '\0') {
    uVar2 = *(ushort *)((uint)bVar6 << 0x10 | 0xf2b6);
    sVar4 = 6;
    do {
      uVar2 = uVar2 << 1;
      if (uVar2 == 0) break;
      sVar4 = sVar4 + -1;
    } while (sVar4 != -1);
    uVar1 = muldiv_u16_scaled(uVar1,b,uVar2,0x4000);
    uVar2 = *(short *)((uint)bVar6 << 0x10 | 0xf2c8) * 2 + 0x80;
    uStack_4 = extraout_R1;
  }
  else {
    uVar1 = mul_u16_sat_s16(uVar1,b,*(ushort *)((uint)bVar6 << 0x10 | 0xf2b6));
    uVar2 = (*muldiv_s16_rounded)
                      ((short)((uint)*(ushort *)((uint)bVar6 << 0x10 | 0xf29c) *
                               (uint)*(ushort *)((uint)bVar6 << 0x10 | 0xf2ca) >> 0x10));
    if (0x80 < uVar2) {
      sVar4 = *(short *)((uint)bVar6 << 0x10 | 0xf2ca);
      sVar5 = 6;
      do {
        sVar4 = sVar4 << 1;
        if (sVar4 == 0) break;
        sVar5 = sVar5 + -1;
      } while (sVar5 != -1);
    }
    uVar2 = (*muldiv_s16_rounded)();
    uStack_4 = extraout_R1_00;
  }
  uVar1 = mul_u16_sat_s16(uVar1,uStack_4,uVar2);
  uStack_4 = b_00;
  if (*(char *)((uint)bVar6 << 0x10 | 0x309) != '\0') {
    uVar1 = mul_u16_sat_s16(uVar1,b_00,*(ushort *)((uint)bVar6 << 0x10 | 0xf2be));
    uStack_4 = extraout_R1_01;
  }
  if (*(char *)((uint)bVar6 << 0x10 | 0x30a) != '\0') {
    uVar2 = sat_add_u16(*(ushort *)((uint)bVar6 << 0x10 | 0xf482),0x180);
    uVar1 = muldiv_u16_scaled(uVar1,uStack_4,uVar2,uVar1);
    uStack_4 = extraout_R1_02;
  }
  mul_u16_sat_s16(uVar1,uStack_4,**(ushort **)((int)in_stack_00000004 + 0x28));
  uVar3 = (*inc_if_signed_flag)();
  **(undefined2 **)((int)in_stack_00000004 + 0x2a) = uVar3;
  return;
}



void knock_octane_secondary_dispatch(void)

{
  ushort in_stack_00000002;
  
  knock_octane_secondary_dispatch_sub();
  status_word_table_subdispatch_group_a(in_stack_00000002);
  return;
}



void knock_octane_secondary_dispatch_sub(void)

{
  byte bVar1;
  
  bVar1 = 1;
  octane_trim_f262_f264_table_select();
  knock_octane_trim_gate_dispatch();
  if (*(char *)((uint)bVar1 << 0x10 | 0x2c1) != '\0') {
    (*knock_octane_trim_ramp_update)();
  }
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: ram

void octane_trim_f262_f264_table_select(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  char in_ram_0000031e;
  ushort in_ram_00001422;
  short sVar1;
  ushort uVar2;
  short extraout_R1;
  short extraout_R1_00;
  short extraout_R1_01;
  short extraout_R1_02;
  short extraout_R2;
  short extraout_R2_00;
  short extraout_R2_01;
  undefined2 unaff_R3;
  short sVar3;
  undefined2 unaff_R4;
  byte bVar4;
  undefined1 uVar5;
  undefined1 uVar6;
  undefined1 uVar7;
  byte bStack_5;
  ushort uStack_4;
  
  bVar4 = 1;
  uVar7 = (undefined1)((ushort)unaff_R4 >> 8);
  uVar6 = (undefined1)((ushort)unaff_R3 >> 8);
  uVar5 = (undefined1)((ushort)param_3 >> 8);
  sVar3 = 0x18;
  sVar1 = 0;
  if ((g_status_flags_f216 & 0x1f) != 0) {
    DAT_0001efb0 = DAT_000108d2;
    sVar1 = DAT_000108d2;
  }
  if (((DAT_0001031d == '\0') && (DAT_0001f024 != 0)) ||
     ((DAT_0001031d != '\0' && ((g_status_flags_f226 & 0x2000) != 0)))) {
    uStack_4 = (ushort)DAT_00011d5c;
    bStack_5 = DAT_00011d5d;
    bVar4 = 0;
  }
  else if (DAT_0001efb0 == 0) {
    uStack_4 = (ushort)DAT_00011d5e;
    bStack_5 = DAT_00011d5f;
    if ((in_ram_0000031e != '\0') &&
       (uVar2 = map_tps_derived_threshold_f10e, uVar2 < in_ram_00001422)) {
      uStack_4 = (ushort)DAT_0001321a;
      bStack_5 = DAT_0001321b;
    }
    bVar4 = 0;
    sVar3 = 0;
    uVar2 = DAT_0000f0f8;
    if ((uVar2 & 0x20) == 0) {
      sVar3 = 0xc;
    }
  }
  else {
    sVar3 = (*calc_staged_index_f0f8)(sVar1,param_2,param_3,(char)param_3);
    uVar2 = table_read_indexed(0xda,(void *)CONCAT11(0xac,uVar5));
    uStack_4 = (ushort)*(byte *)(extraout_R1 + sVar3);
    uVar2 = table_read_indexed(0xda,(void *)CONCAT11(0xcc,uVar6));
    bStack_5 = *(byte *)(extraout_R1_00 + extraout_R2);
    uVar5 = 0;
    if ((*(char *)((uint)bVar4 << 0x10 | 0x309) != '\0') &&
       (*(ushort *)((uint)bVar4 << 0x10 | 0xf10e) < *(ushort *)((uint)bVar4 << 0x10 | 0x1420))) {
      uVar2 = table_read_indexed(0xda,(void *)CONCAT11(0xac,uVar7));
      uStack_4 = (ushort)*(byte *)(extraout_R1_01 + extraout_R2_00);
      uVar2 = table_read_indexed(0xda,(void *)CONCAT11(0xcc,uVar5));
      bStack_5 = *(byte *)(extraout_R1_02 + extraout_R2_01);
    }
    sVar3 = sVar3 << 1;
  }
  *(ushort *)((uint)bVar4 << 0x10 | 0xf262) = uStack_4;
  *(ushort *)((uint)bVar4 << 0x10 | 0xf264) = (ushort)bStack_5;
  *(short *)((uint)bVar4 << 0x10 | 0xf5bc) = sVar3;
  return;
}



short calc_staged_index_f0f8(void)

{
  short sVar1;
  short sVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uStack_4;
  
  sVar1 = 0;
  uStack_4 = DAT_000108b8;
  uVar3 = DAT_000108bc;
  uVar4 = DAT_000108ba;
  if ((g_status_flags_f0f8 & 0x20) == 0) {
    uStack_4 = DAT_000108be;
    sVar1 = 6;
    uVar3 = DAT_000108c2;
    uVar4 = DAT_000108c0;
  }
  if (uStack_4 <= inj_pw_output_f18c) {
    sVar1 = sVar1 + 1;
  }
  sVar2 = sVar1;
  if ((engine_torque_pct_f17a < uVar4) || (sVar2 = sVar1 + 2, engine_torque_pct_f17a < uVar3)) {
    return sVar2;
  }
  return sVar1 + 4;
}



void knock_octane_trim_gate_dispatch(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = knock_octane_trim_gate_condition_check();
  if (uVar1 != 0) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xefae) = *(undefined2 *)((uint)bVar2 << 0x10 | 0x90a);
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort knock_octane_trim_gate_condition_check(void)

{
  if (((((((DAT_0001f218 ^ DAT_0001f218 >> 4) & 0xf) == 0) && ((g_status_flags_f20e & 0x80) != 0))
       && ((g_status_flags_f1fe & 0x17) == 0)) &&
      ((((g_status_flags_f21c & 0x4000) == 0 && (DAT_000108fa <= g_tps_load_axis_f10e)) &&
       ((g_iat_axis_f118 < DAT_000108fc &&
        ((DAT_00010902 <= DAT_0001f188 && (DAT_00010900 <= coolant_temp_scaled_f130)))))))) &&
     (((((coolant_temp_scaled_f130 < DAT_000108fe &&
         (((g_status_flags_f216 & 0x700) == 0 && (DAT_0001f034 == 0)))) &&
        (((cal_fuel_cut_enable_flag != '\0' || ((g_status_flags_f1f2 & 0x20) == 0)) ||
         (((g_status_flags_f216 & 0x10) == 0 && ((DAT_0001f218 & 0xf) == 0)))))) &&
       ((cal_fuel_cut_enable_flag == '\0' ||
        ((g_tps_load_trimmed_f13e < UNK_00010906 || (UNK_00010904 < g_tps_load_trimmed_f13e)))))) &&
      ((((g_status_flags_f1f2 & 0x1000) == 0 || (DAT_0001f45c == 0)) &&
       (((((DAT_0001f0ea & 0x80) == 0 && ((g_status_flags_f21c & 0x200) == 0)) &&
         (DAT_0001f2dc == 0)) && (DAT_0001f1cc <= DAT_0001f2f8)))))))) {
    return 0;
  }
  return 1;
}



void status_word_table_subdispatch_group_a(ushort param_fp1)

{
  ushort in_R2;
  
  (*f276_f27a_dual_channel_compute_via_table)();
  (*f26e_o2_closed_loop_output_blend_via_table)();
  f286_knock_octane_secondary_trim_compute_via_table(in_R2);
  (*f27e_table_indexed_clamp_compute_via_table)();
  return;
}



// Table-driven (TABLE_STATUS_WORD_PTRS_137A8): reads f21c-equivalent (entry+6) bit14 to select
// between two table_lookup_interp results (tables 0x31e0/0x31d4) or a fallback constant at 0x8c4;
// writes results to entries +0x40 (f276) and +0x42 (f27a). f276/f27a were previously init'd to 0 by
// init_state_block_ee98_80_call_wrapper's callee (0x23027) -- this is their real per-cycle compute.

void f276_f27a_dual_channel_compute_via_table(ushort param_fp1)

{
  short sVar1;
  byte bVar2;
  byte bVar3;
  undefined2 in_stack_00000004;
  
  bVar3 = 1;
  sVar1 = table_lookup_interp(1,(void *)0x31e0);
  bVar2 = table_lookup_interp(1,(void *)0x31d4);
  if ((**(ushort **)((int)in_stack_00000004 + 6) & 0x4000) != 0) {
    bVar2 = (byte)*(undefined2 *)((uint)bVar3 << 0x10 | 0x8c4);
  }
  **(short **)((int)in_stack_00000004 + 0x40) = sVar1 << 8;
  **(short **)((int)in_stack_00000004 + 0x42) = (ushort)bVar2 << 8;
  return;
}



// Table-driven: entry+0x30 (f26e) is the write target, entry+0x32 (f272) a secondary. Calls
// o2_closed_loop_config_check then branches on FUN_00023fc7's return (2/4/8/0x10) to pick scaled
// constants at 0x8c4/0x8c6/0x8c8/0x8ca, else clears to 0x8080 default. Final override path checks
// entry+2 (f20e) bit11 and entry+4 (f220) bit11. Consistent with existing o2_* naming family.

void f26e_o2_closed_loop_output_blend_via_table(ushort param_fp1)

{
  ushort *puVar1;
  ushort *puVar2;
  undefined1 extraout_var;
  bool bVar4;
  short sVar3;
  ushort uVar5;
  byte bVar6;
  undefined2 in_stack_00000004;
  
  bVar6 = 1;
  puVar1 = *(ushort **)((int)in_stack_00000004 + 0x30);
  puVar2 = *(ushort **)((int)in_stack_00000004 + 0x32);
  bVar4 = o2_closed_loop_config_check();
  if (CONCAT11(extraout_var,bVar4) == 0) {
    sVar3 = (*o2_closed_loop_mode_selector_via_table)();
    if (sVar3 == 2) {
      *puVar1 = *(short *)((uint)bVar6 << 0x10 | 0x8c8) * 0x100 + 0x80;
    }
    else {
      if (sVar3 == 4) goto LAB_00023f15;
      if (sVar3 == 8) {
        *puVar1 = *(short *)((uint)bVar6 << 0x10 | 0x8c6) * 0x100 + 0x80;
      }
      else if (sVar3 == 0x10) {
        *puVar1 = *(short *)((uint)bVar6 << 0x10 | 0x8ca) * 0x100 + 0x80;
      }
    }
  }
  else {
LAB_00023f15:
    *puVar1 = 0x8080;
  }
  if ((**(ushort **)((int)in_stack_00000004 + 2) & 0x800) != 0) {
    if ((**(ushort **)((int)in_stack_00000004 + 4) & 0x800) == 0) {
      *puVar1 = *puVar2;
    }
    *puVar2 = *puVar1;
    return;
  }
  bVar4 = false;
  uVar5 = 0x8080;
  if ((*(ushort *)((uint)bVar6 << 0x10 | 0xf20e) & 0x10) == 0) {
    if ((*(char *)((uint)bVar6 << 0x10 | 0x2cb) == '\0') ||
       (*(short *)((uint)bVar6 << 0x10 | 0xefa6) == 0)) {
      if ((**(ushort **)((int)in_stack_00000004 + 10) & 8) != 0) {
        uVar5 = *(short *)((uint)bVar6 << 0x10 | 0xd5e) * 0x100 + 0x80;
        if (**(ushort **)((int)in_stack_00000004 + 0x40) < uVar5) {
          uVar5 = **(ushort **)((int)in_stack_00000004 + 0x40);
        }
        goto LAB_00023fb9;
      }
      if (((*(ushort *)((uint)bVar6 << 0x10 | 0xf1f0) & 1) == 0) ||
         ((*(ushort *)((uint)bVar6 << 0x10 | 0xf216) & 0x80) != 0)) goto LAB_00023fb6;
    }
    bVar4 = true;
  }
  else {
LAB_00023fb6:
    uVar5 = 0x8080;
  }
LAB_00023fb9:
  if (!bVar4) {
    *puVar2 = uVar5;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// Table-driven mode selector: entry+2(f20e) bit11 -> return 1 (closed-loop override); else checks
// f10e range vs entry+6(f21c) bit0x8000, entry+0x56 bits 0xf0, f43e bit3, f0f8 bit5-selected
// threshold; then f216 bits 0x80/0x8000 disambiguate. Returns enum code 1/2/4/8/0x10 consumed as a
// switch selector by f26e_o2_closed_loop_output_blend_via_table.

ushort o2_closed_loop_mode_selector_via_table(void)

{
  undefined2 in_stack_00000004;
  byte abStack_4 [2];
  
  if ((**(ushort **)((int)in_stack_00000004 + 2) & 0x800) != 0) {
    return 1;
  }
  if ((g_status_flags_f0f8 & 0x20) == 0) {
    abStack_4 = DAT_0001150e;
  }
  else {
    abStack_4 = BYTE_ARRAY_000108a4;
  }
  if (((((ushort)abStack_4 < (ushort)g_tps_load_axis_f10e) && ((g_status_flags_f21c & 0x1000) == 0))
      && ((**(ushort **)((int)in_stack_00000004 + 6) & 0x8000) == 0)) &&
     (((g_rpm_zone_flags_f43e & 8) == 0 &&
      ((**(ushort **)((int)in_stack_00000004 + 0x56) & 0xf0) == 0)))) {
    if ((g_status_flags_f216 & 0x80) != 0) {
      return 8;
    }
    if ((g_status_flags_f216 & 0x8000) != 0) {
      if ((ushort)g_tps_load_axis_f10e < (ushort)DAT_000108cc) {
        return 0x10;
      }
      return 2;
    }
  }
  return 4;
}



// Table-driven: writes entry+0x2c (f286). Gated by entry+2 (f20e) bit11; branches on knock-cut
// flags (0x1031d, f024, f226 bit0x2000), fuel-mode flags (0x10309/0x1030c), and table_read_indexed
// / calc_staged_index_f0f8 lookups; final clamp_u8 or sat_sub_u16. Sibling of
// f266_knock_octane_composite_trim_compute (same shared-table family, parallel output register
// f286). Confidence: medium on exact physical meaning, high on data flow/table linkage.

void f286_knock_octane_secondary_trim_compute_via_table(ushort param_fp1)

{
  short sVar1;
  byte *extraout_R1;
  short extraout_R1_00;
  undefined2 unaff_R3;
  ushort uVar2;
  byte bVar3;
  undefined2 in_stack_00000004;
  byte bank;
  undefined1 uVar4;
  undefined1 uVar5;
  undefined1 uStack_7;
  byte *pbStack_6;
  ushort uStack_4;
  
  bVar3 = 1;
  uVar5 = (undefined1)((ushort)unaff_R3 >> 8);
  if ((**(ushort **)((int)in_stack_00000004 + 2) & 0x800) == 0) {
    uVar2 = 0x80;
  }
  else {
    if (((DAT_0001031d == '\0') && (DAT_0001f024 != 0)) ||
       ((DAT_0001031d != '\0' && ((g_status_flags_f226 & 0x2000) != 0)))) {
      if (DAT_0001031d == '\0') {
        uStack_4 = DAT_000108d6;
      }
      else {
        uStack_4 = DAT_0001147e;
      }
    }
    else if ((g_status_flags_f216 & 0x100) == 0) {
      if ((DAT_00010309 == '\0') || (DAT_0001030c == '\0')) {
        if ((g_status_flags_f0f8 & 0x20) == 0) {
          bank = 0xdb;
          uVar4 = 0xc;
        }
        else {
          bank = 0xda;
          uVar4 = 0xec;
        }
        uVar2 = table_read_indexed(bank,(void *)CONCAT11(uVar4,uVar5));
        pbStack_6 = extraout_R1;
        if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf216) & 0x1f) != 0) {
          pbStack_6 = extraout_R1 + 1;
        }
        uStack_7 = (undefined1)uVar2;
        uStack_4 = (ushort)*pbStack_6;
      }
      else {
        uVar4 = 0x40;
        sVar1 = (*calc_staged_index_f0f8)();
        uVar2 = table_read_indexed(0xda,(void *)CONCAT11(0xec,uVar4));
        uStack_4 = (ushort)*(byte *)(extraout_R1_00 + sVar1);
        table_read_indexed(0xda,(void *)CONCAT11(0xec,uVar5));
      }
    }
    else {
      uStack_4 = DAT_000108d4;
    }
    if ((**(ushort **)((int)in_stack_00000004 + 6) & 0x40) == 0) {
      uVar2 = (*clamp_u8)();
    }
    else {
      uVar2 = sat_sub_u16(0x80,uStack_4);
    }
  }
  **(ushort **)((int)in_stack_00000004 + 0x2c) = uVar2;
  return;
}



// Table-driven: writes entry+0x2e (f27e). Calls FUN_00024207(table) then FUN_000242c1(table); on
// failure writes fallback 0x80. Otherwise selects an index from f218 low nibble (gated by 0x2c1
// flag and f1f2 bit5), looks up a value from a small table at entry+0x34 indexed by that selector,
// clamp_u16's it, then overrides with 0x80 if f21c bit9 (0x200) set per entry+2 (f20e) bit11
// branch. Sibling of f276_f27a_dual_channel_compute_via_table in style.

void f27e_table_indexed_clamp_compute_via_table(ushort param_fp1)

{
  ushort uVar1;
  short sVar2;
  ushort uVar3;
  byte bVar4;
  undefined2 in_stack_00000004;
  
  bVar4 = 1;
  (*eeac_adaptive_trim_learn_update_via_table)();
  sVar2 = (*f1fe_bit3_clear_and_f1f2_bit4_set_check)();
  if (sVar2 == 0) {
    **(undefined2 **)((int)in_stack_00000004 + 0x2e) = 0x80;
    return;
  }
  if ((*(char *)((uint)bVar4 << 0x10 | 0x2c1) == '\0') &&
     ((*(ushort *)((uint)bVar4 << 0x10 | 0xf1f2) & 0x20) != 0)) {
    uVar3 = 0;
  }
  else {
    uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xf218) & 0xf;
  }
  uVar3 = clamp_u16(*(ushort *)(*(short *)((int)in_stack_00000004 + 0x34) + uVar3 * 2),
                    *(ushort *)((uint)bVar4 << 0x10 | 0x910),
                    *(ushort *)((uint)bVar4 << 0x10 | 0x912));
  if ((**(ushort **)((int)in_stack_00000004 + 2) & 0x800) == 0) {
    if ((*(char *)((uint)bVar4 << 0x10 | 0x2c1) != '\0') ||
       ((*(ushort *)((uint)bVar4 << 0x10 | 0xf1f2) & 0x20) == 0)) {
      uVar1 = *(ushort *)((uint)bVar4 << 0x10 | 0xf21c);
      goto joined_r0x000241eb;
    }
    if ((((*(ushort *)((uint)bVar4 << 0x10 | 0xf216) & 0x10) == 0) &&
        ((*(ushort *)((uint)bVar4 << 0x10 | 0xf218) & 0xf) == 0)) &&
       ((*(ushort *)((uint)bVar4 << 0x10 | 0xf21c) & 2) != 0)) goto LAB_000241f0;
  }
  else {
    uVar1 = *(ushort *)((uint)bVar4 << 0x10 | 0xf21c);
joined_r0x000241eb:
    if ((uVar1 & 0x200) == 0) goto LAB_000241f0;
  }
  uVar3 = 0x80;
LAB_000241f0:
  **(ushort **)((int)in_stack_00000004 + 0x2e) = uVar3;
  return;
}



// Table-driven: writes entry+0x3a (idx29=eeac) and adjusts a small indexed array at entry+0x34
// (indexed by f218 low nibble). Gated by DAT_0001efae (reset-to-0x80 override) and f208 bit0;
// nudges the eeac cell via sat_add_u16/sat_sub_u16 toward a target derived from entry+0x3a's
// current value and DAT_0001090c/090e -- classic saturating adaptive-learn pattern (long-term
// trim/knock-learn style). On o2_closed_loop_config_check()==false, re-clamps via clamp_u16 into
// the entry+0x34 array; on true, falls back to init_state_block_ee98_80.

void eeac_adaptive_trim_learn_update_via_table(ushort param_fp1)

{
  ushort uVar1;
  undefined1 extraout_var;
  bool bVar3;
  ushort uVar2;
  ushort uVar4;
  byte bVar5;
  undefined2 in_stack_00000004;
  
  bVar5 = 1;
  uVar1 = DAT_0001f218 & 0xf;
  uVar2 = *(ushort *)(*(short *)((int)in_stack_00000004 + 0x34) + uVar1 * 2);
  if (DAT_0001efae == 0) {
    if ((DAT_0001f208 & 1) != 0) {
      uVar4 = **(ushort **)((int)in_stack_00000004 + 0x30) & 0xff00;
      if (uVar4 != 0x8000) {
        uVar2 = **(short **)((int)in_stack_00000004 + 0x3a) + uVar2 * 0x100;
        if (uVar4 < 0x8001) {
          uVar2 = sat_sub_u16(uVar2,DAT_0001090c);
        }
        else {
          uVar2 = sat_add_u16(uVar2,DAT_0001090e);
        }
        **(ushort **)((int)in_stack_00000004 + 0x3a) = uVar2 & 0xff;
        uVar2 = uVar2 >> 8;
      }
    }
  }
  else {
    **(undefined2 **)((int)in_stack_00000004 + 0x3a) = 0x80;
  }
  bVar3 = o2_closed_loop_config_check();
  if (CONCAT11(extraout_var,bVar3) == 0) {
    uVar2 = clamp_u16(uVar2,*(ushort *)((uint)bVar5 << 0x10 | 0x910),
                      *(ushort *)((uint)bVar5 << 0x10 | 0x912));
    *(ushort *)(*(short *)((int)in_stack_00000004 + 0x34) + uVar1 * 2) = uVar2;
  }
  else {
    init_state_block_ee98_80();
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f1fe_bit3_clear_and_f1f2_bit4_set_check(void)

{
  ushort uVar1;
  
  if (((g_status_flags_f1fe & 8) == 0) && ((g_status_flags_f1f2 & 0x10) != 0)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



// WARNING: Heritage AFTER dead removal. Example location: s0xfffa : 0x00024375
// WARNING: Restarted to delay deadcode elimination for space: stack
// WARNING: Exceeded maximum restarts with more pending

void f0ba_f2d0_o2_mode7_8_correction_calc(void)

{
  undefined2 uVar1;
  ushort uVar2;
  ushort b;
  byte bVar3;
  
  bVar3 = 1;
  table_lookup_interp(1,(void *)0x2986);
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf0ba) = uVar1;
  (*axis_lookup_interp)();
  (*axis_lookup_interp)();
  table_lookup_interp(1,(void *)0x1dc2);
  uVar2 = o2_mode7_8_correction_apply();
  *(ushort *)((uint)bVar3 << 0x10 | 0xf2da) = uVar2;
  uVar2 = (*identity_passthrough_arg1)
                    ((short)((uint)*(ushort *)((uint)bVar3 << 0x10 | 0xf2da) *
                             (uint)*(ushort *)((uint)bVar3 << 0x10 | 0x814) >> 0x10));
  muldiv_u16_scaled(uVar2,b,*(short *)((uint)bVar3 << 0x10 | 0xf298) *
                            *(short *)((uint)bVar3 << 0x10 | 0xf29a),b);
  (*muldiv_u16_shr8_sat)();
  (*muldiv_u16_shr8_sat)();
  uVar1 = (*inc_if_signed_flag)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf2d0) = uVar1;
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack

void f2d2_composite_correction_calc(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  ushort c;
  ushort a;
  ushort uVar4;
  ushort b;
  ushort extraout_R1;
  ushort b_00;
  ushort extraout_R1_00;
  byte bVar5;
  short sVar6;
  byte bVar7;
  ushort uStack_e;
  ushort uStack_c;
  
  uVar2 = table_1e4a_lookup_with_f2d6_d8_reset();
  uVar3 = f07a_default_reset_return_0x80();
  c = (*f2b4_threshold_table_lookup)();
  (*axis_lookup_interp)();
  table_lookup_interp(1,(void *)0x1e3a);
  a = (*identity_passthrough_arg1)();
  bVar7 = 0;
  uVar4 = (*identity_passthrough_arg1)();
  uVar2 = muldiv_u16_scaled(a,b,uVar2 * uVar3,0x4000);
  sVar6 = 6;
  do {
    c = c << 1;
    if (c == 0) break;
    sVar6 = sVar6 + -1;
  } while (sVar6 != -1);
  uStack_e = muldiv_u16_scaled(uVar2,b_00,c,0x4000);
  uStack_c = extraout_R1_00;
  if ((uStack_e <= uVar4) && ((uStack_e != uVar4 || (extraout_R1_00 < extraout_R1)))) {
    uStack_e = uVar4;
    uStack_c = extraout_R1;
  }
  muldiv_u16_scaled(uStack_e,uStack_c,
                    *(short *)((uint)bVar7 << 0x10 | 0xf29a) *
                    *(short *)((uint)bVar7 << 0x10 | 0x860),0x200);
  uStack_c = (*inc_if_signed_flag)();
  if ((*(ushort *)((uint)bVar7 << 0x10 | 0xf20e) & 1) == 0) {
    *(ushort *)((uint)bVar7 << 0x10 | 0xf25e) = *(ushort *)((uint)bVar7 << 0x10 | 0xf25e) & 0x7ffb;
    uStack_c = sat_mul_u16(uStack_c,4);
  }
  else {
    bVar5 = (byte)*(undefined2 *)((uint)bVar7 << 0x10 | 0x85c);
    if ((*(ushort *)((uint)bVar7 << 0x10 | 0xf25e) & 0x8000) != 0) {
      bVar5 = (byte)*(undefined2 *)((uint)bVar7 << 0x10 | 0x85e);
    }
    if (uStack_c >> 1 < (ushort)bVar5 * 0x100) {
      bVar5 = (byte)*(undefined2 *)((uint)bVar7 << 0x10 | 0x854);
      if ((*(ushort *)((uint)bVar7 << 0x10 | 0xf25e) & 4) != 0) {
        bVar5 = (byte)*(undefined2 *)((uint)bVar7 << 0x10 | 0x858);
      }
      if (uStack_c < (ushort)bVar5 * 0x100) {
        puVar1 = (ushort *)((uint)bVar7 << 0x10 | 0xf25e);
        *puVar1 = *puVar1 & 0xfffb;
        uStack_c = sat_mul_u16(uStack_c,4);
      }
      else {
        puVar1 = (ushort *)((uint)bVar7 << 0x10 | 0xf25e);
        *puVar1 = *puVar1 | 4;
      }
      puVar1 = (ushort *)((uint)bVar7 << 0x10 | 0xf25e);
      *puVar1 = *puVar1 & 0x7fff;
    }
    else {
      *(ushort *)((uint)bVar7 << 0x10 | 0xf25e) = *(ushort *)((uint)bVar7 << 0x10 | 0xf25e) | 0x8004
      ;
      uStack_c = uStack_c >> 1;
    }
  }
  *(ushort *)((uint)bVar7 << 0x10 | 0xf2d2) = uStack_c;
  return;
}



ushort table_1e4a_lookup_with_f2d6_d8_reset(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f20e & 0x10) != 0) {
    DAT_0000f2d6 = 0;
    DAT_0000f2d8 = 0;
  }
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x1e4a);
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f07a_default_reset_return_0x80(void)

{
  if (((g_status_flags_f20e & 0x10) != 0) || ((g_status_flags_f0f8 & 0x40) == 0)) {
    DAT_0001f07a = DAT_00010838;
  }
  return 0x80;
}



ushort f2b4_threshold_table_lookup(void)

{
  ushort *puVar1;
  ushort uVar2;
  short sVar3;
  ushort uVar4;
  bool bVar5;
  
  bVar5 = (g_status_flags_f0f8 & 0x40) != 0;
  if (!bVar5) {
    DAT_0000f2b4 = 0;
  }
  if (*(char *)((uint)bVar5 << 0x10 | 0x2f4) != '\0') {
    uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xf2b4);
    if (*(char *)((uint)bVar5 << 0x10 | 0x2f4) == '\x02') {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0x836);
      if (uVar4 < *puVar1 || uVar4 == *puVar1) {
        (*axis_lookup_interp)();
        sVar3 = table_lookup_interp(1,(void *)0x1e6a);
        return sVar3 + 0x80;
      }
    }
    else {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0x836);
      if ((uVar4 < *puVar1 || uVar4 == *puVar1) &&
         (uVar2 = *(ushort *)((uint)bVar5 << 0x10 | 0xf10e),
         puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0x866), uVar2 < *puVar1 || uVar2 == *puVar1)) {
        uVar4 = uVar4 >> 3;
        if (0x12 < uVar4) {
          uVar4 = 0x13;
        }
        return (ushort)*(byte *)((int)uVar4 + 0x1e56);
      }
    }
  }
  return 0x80;
}



void f2ce_f2cc_o2_mode11_correction_calc(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x1dec);
  uVar1 = o2_mode11_lambda_select(uVar1,uVar1);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf2ce) = uVar1;
  uVar1 = (*muldiv_s16_rounded)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf2cc) = uVar1;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// injpw_airvol_reset_on_fuelcut: gated on 0xF25A bit-mask 0x28 (matches logging.txt's
// claimed fuel-cut condition), zeroes 0xF970/F972 (InjPulseWidth/AirVol) plus 0xF250/F252.
// VERIFIED - logging.txt CONFIRMED-section claim holds under current decoder.
// 
// MUT SWEEP FOLLOW-UP (2026-07-15): the F970 and F972 resets here are single ushort
// stores, corroborating fuel_pw_and_airvol_compute's finding that F970/F971 and F972/F973
// are each one 16-bit word with no independent low/high-byte writer. Resolves MUT ReqIDs
// 0x2A (F971) and 0x2B (F972) from BLANK to CONFIRMED - see mut_verification_status.md
// and review.md item 7b.

ushort injpw_airvol_reset_on_fuelcut(void)

{
  if ((g_status_flags_001f25a & 0x28) != 0) {
    DAT_0000f252 = 0;
    DAT_0000f250 = 0;
    DAT_0000f972 = 0;
    DAT_0000f970 = 0;
  }
  return g_status_flags_001f25a & 0x28;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f24e_f254_f970_f972_reset_on_f20e_bit4(void)

{
  if ((g_status_flags_f20e & 0x10) != 0) {
    DAT_0000f252 = 0;
    DAT_0000f250 = 0;
    DAT_0000f24e = 0;
    DAT_0000f254 = 0;
    DAT_0000f972 = 0;
    DAT_0000f970 = 0;
  }
  return;
}



void f2e0_f2ee_correction_dispatch(void)

{
  f2e0_f2ec_composite_correction_calc();
  f2ea_f2ee_composite_correction_calc();
  stub_empty_0x2495a();
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: stack

void f2e0_f2ec_composite_correction_calc(void)

{
  ushort *puVar1;
  ushort uVar2;
  undefined2 uVar3;
  short sVar4;
  ushort value;
  byte bVar5;
  ushort uStack_6;
  ushort uStack_4;
  
  bVar5 = 1;
  uStack_6 = 0;
  uStack_4 = 0;
  if ((g_status_flags_f20e & 1) != 0) {
    uVar3 = table_lookup_interp(1,(void *)0x1f16);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf304) = uVar3;
    uVar3 = table_lookup_interp(1,(void *)0x1f16);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf30c) = uVar3;
  }
  sVar4 = *(short *)((uint)bVar5 << 0x10 | 0xf304);
  if ((*(char *)((uint)bVar5 << 0x10 | 0x31e) != '\0') &&
     ((*(ushort *)((uint)bVar5 << 0x10 | 0xf742) & 4) == 0)) {
    sVar4 = *(short *)((uint)bVar5 << 0x10 | 0xf30c);
  }
  puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xef6a);
  if ((ushort)(sVar4 * 0x50) < *puVar1 || sVar4 * 0x50 == *puVar1) {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 & 0xdfff;
  }
  else {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 | 0x2000;
  }
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf0f8) & 0x40) == 0) {
    table_lookup_interp(1,(void *)0x1e86);
    (*muldiv_s16_rounded_3op)();
    table_lookup_interp(1,(void *)0x1e7a);
    uStack_6 = (*muldiv_s16_rounded_3op)();
    uStack_4 = value;
    if (*(char *)((uint)bVar5 << 0x10 | 0x30e) == '\0') {
      uStack_4 = passthrough_or_sat(uStack_6,value);
      uStack_6 = 0;
    }
  }
  *(ushort *)((uint)bVar5 << 0x10 | 0xf2e6) = uStack_6;
  *(ushort *)((uint)bVar5 << 0x10 | 0xf2e8) = uStack_4;
  uVar3 = table_lookup_interp(1,(void *)0x1e94);
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2ec) = uVar3;
  sVar4 = table_lookup_interp(1,(void *)0x1f22);
  *(ushort *)((uint)bVar5 << 0x10 | 0xf2e0) =
       ((ushort)(sVar4 * *(short *)((uint)bVar5 << 0x10 | 0x812)) >> 3) + 1 >> 1;
  uVar3 = (*mul_u16_x2_sat)();
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2e4) = uVar3;
  uVar2 = *(ushort *)((uint)bVar5 << 0x10 | 0xf10e);
  puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xaa6);
  if ((*puVar1 <= uVar2 && uVar2 != *puVar1) &&
     ((*(ushort *)((uint)bVar5 << 0x10 | 0xf0f8) & 0x80) == 0)) {
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xefb8) = *(undefined2 *)((uint)bVar5 << 0x10 | 0xaa8);
  }
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2fa) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x966);
  return;
}



// WARNING: Heritage AFTER dead removal. Example location: s0xfffa : 0x000248dd
// WARNING: Restarted to delay deadcode elimination for space: stack
// WARNING: Exceeded maximum restarts with more pending

void f2ea_f2ee_composite_correction_calc(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  undefined2 in_ram_0000096a;
  ushort uVar1;
  short sVar2;
  short sVar3;
  ushort b;
  ushort b_00;
  undefined2 uVar4;
  byte bVar5;
  
  uVar4 = 0;
  bVar5 = (g_status_flags_f20e & 1) == 0;
  if (!(bool)bVar5) {
    g_f2ea_correction_calc_gate_ref_efb2 = in_ram_0000096a;
  }
  if (*(short *)((uint)bVar5 << 0x10 | 0xefb2) == 0) {
    uVar1 = (*identity_passthrough_arg1)();
    sVar2 = table_lookup_interp(1,(void *)0x1f3c);
    uVar1 = muldiv_u16_scaled(uVar1,b,sVar2 * *(short *)((uint)bVar5 << 0x10 | 0xf29a),param_3);
    sVar2 = table_lookup_interp(1,(void *)0x1f4a);
    sVar3 = table_lookup_interp(1,(void *)0x1f30);
    muldiv_u16_scaled(uVar1,b_00,sVar3 * sVar2,uVar1);
    uVar4 = (*inc_if_signed_flag)();
  }
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2ea) = uVar4;
  uVar4 = table_lookup_interp(1,(void *)0x1f5c);
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2ee) = uVar4;
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2e2) = *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2e0);
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf2fc) = *(undefined2 *)((uint)bVar5 << 0x10 | 0x968);
  return;
}



void stub_empty_0x2495a(void)

{
  return;
}



void f310_f316_composite_calc(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  table_lookup_interp(1,(void *)0x1ed6);
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf20e) & 0x2000) != 0) {
    if (*(char *)((uint)bVar2 << 0x10 | 0x30e) == '\0') {
      (*mul_u16_x2_sat)();
    }
    else {
      (*muldiv_s16_rounded)();
    }
  }
  table_lookup_interp(1,(void *)0x1ee2);
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf314) = uVar1;
  (*mul_u16_x2_sat)();
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf310) = uVar1;
  uVar1 = table_lookup_interp(1,(void *)0x1ec8);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf316) = uVar1;
  return;
}



void f250_f252_channel_snapshot_update(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  ushort uVar1;
  short sVar2;
  ushort in_SR;
  ushort uStack_2;
  
  if (DAT_00010828 <= engine_torque_pct_f17a) {
    engine_state_flags_f25e = engine_state_flags_f25e & 0xfffd;
  }
  if (((g_status_flags_f20e & 0x10) != 0) && ((engine_state_flags_f25e & 2) == 0)) {
    engine_state_flags_f25e = engine_state_flags_f25e & 0xfffe;
  }
  if (g_tps_load_axis_f10e < DAT_00010aa2) {
    engine_state_flags_f25e = engine_state_flags_f25e & 0xff7f;
  }
  else {
    engine_state_flags_f25e = engine_state_flags_f25e | 0x80;
  }
  if (((g_status_flags_f20e & 1) != 0) && ((engine_state_flags_f25e & 1) == 0)) {
    if ((engine_state_flags_f25e & 0x80) == 0) {
      uStack_2 = DAT_00010852;
    }
    else {
      uStack_2 = param_3;
      uStack_2 = table_3axis_interp_triple(1,(void *)0x1ef0);
    }
    sVar2 = (*f2cc_offset_accumulate)();
    uStack_2 = in_SR & 0xfff0 | (ushort)((undefined1 *)&DAT_0000fffd < &uStack_2) |
               (ushort)(sVar2 == 0) << 2 | (ushort)(sVar2 < 0) << 3;
    DAT_0000f252 = sVar2;
    DAT_0000f250 = sVar2;
    (*channel_enable_mask_build_and_dispatch_ff2x_ff3x)();
    uVar1 = DAT_0000f25e;
    DAT_0000f25e = uVar1 | 3;
  }
  return;
}


/*
Unable to decompile 'f31c_bit_toggle_pulse'
Cause: Exception while decompiling 00024a8e: process: timeout

*/


void f31c_bit6_set_then_clear_pulse(void)

{
  ushort uVar1;
  ushort unaff_R3;
  
  uVar1 = g_f31c_toggle_pulse;
  g_f31c_toggle_pulse = uVar1 | unaff_R3;
  uVar1 = g_f31c_toggle_pulse;
  g_f31c_toggle_pulse = uVar1 & 0xffbf;
  noop_return_after_f31c_pulse();
  return;
}



void noop_return_after_f31c_pulse(void)

{
  return;
}



void f25a_f2xx_state_mirror_update(void)

{
  ushort uVar1;
  char in_ram_0000030a;
  ushort uVar2;
  undefined2 uVar3;
  
  uVar2 = g_status_flags_f20e & 0x89f;
  if ((engine_state_flags_f25e & 4) != 0) {
    uVar2 = uVar2 | 0x8000;
  }
  if ((engine_state_flags_f25e & 0x8000) != 0) {
    uVar2 = uVar2 | 0x200;
  }
  uVar3 = DAT_0001f2d2;
  if ((g_status_flags_f20e & 1) == 0) {
    uVar3 = DAT_0001f2d0;
  }
  DAT_0000f2d4 = uVar3;
  uVar1 = g_status_flags_f25a;
  g_status_flags_f25a = uVar2 | uVar1 & 0x7560;
  uVar2 = DAT_0000f25c;
  DAT_0000f25c = DAT_0001f210 & 0x800 | uVar2 & 0xf7ff;
  uVar3 = DAT_0000f266;
  DAT_0000f268 = uVar3;
  uVar3 = DAT_0000f26a;
  DAT_0000f26c = uVar3;
  uVar3 = DAT_0000f27e;
  DAT_0000f280 = uVar3;
  uVar3 = DAT_0000f282;
  DAT_0000f284 = uVar3;
  uVar3 = DAT_0000f286;
  DAT_0000f288 = uVar3;
  uVar3 = DAT_0000f28a;
  DAT_0000f28c = uVar3;
  if ((in_ram_0000030a != '\0') && (uVar2 = DAT_0000f48c, (uVar2 & 0x8000) != 0)) {
    uVar3 = DAT_0000f488;
    DAT_0000f756 = uVar3;
    uVar3 = DAT_0000f488;
    DAT_0000f26e = uVar3;
    uVar2 = DAT_0000f48c;
    DAT_0000f48c = uVar2 & 0x7fff;
  }
  return;
}



void f856_f858_clamped_calc(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  ushort uVar1;
  ushort uVar2;
  ushort value;
  byte bVar3;
  ushort unaff_retaddr;
  
  bVar3 = 1;
  uVar1 = get_high_byte(DAT_0001f270);
  uVar1 = sat_sub_u16(uVar1 + *(short *)((uint)bVar3 << 0x10 | 0xf28a),param_3);
  uVar2 = get_high_byte(*(ushort *)((uint)bVar3 << 0x10 | 0xf26e));
  sat_sub_u16(uVar2 + *(short *)((uint)bVar3 << 0x10 | 0xf286),unaff_retaddr);
  uVar1 = clamp_u16(uVar1,0xff,0);
  *(ushort *)((uint)bVar3 << 0x10 | 0xf858) = uVar1;
  uVar1 = clamp_u16(value,0xff,0);
  *(ushort *)((uint)bVar3 << 0x10 | 0xf856) = uVar1;
  return;
}



void octane_level_init_or_force_from_gear_table
               (undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  undefined1 extraout_var;
  undefined1 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  byte bVar4;
  
  bVar4 = 1;
  uVar2 = 0;
  uVar3 = 0;
  if ((g_status_flags_f1f2 & 0x100) == 0) {
    uVar1 = read_indexed_byte_f1fc(0x28,(void *)(CONCAT22(0x2882,param_3) >> 8));
    uVar2 = CONCAT11(extraout_var,uVar1);
  }
  else if ((DAT_0001f0fe & 1) != 0) {
    uVar3 = 6;
    uVar2 = 0xff;
  }
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xeec6) = uVar2;
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xeec8) = uVar2;
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf3a4) = uVar3;
  return;
}



void tcu_slip_state_block_cold_init(void)

{
  byte bVar1;
  
  DAT_0001f350 = 0xa0;
  DAT_0001efbe = DAT_00010b5e;
  DAT_0000f380 = (ushort)DAT_0001280c << 4;
  bVar1 = 0;
  DAT_0000f37e = (ushort)DAT_00012828;
  tcu_slip_control_init();
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf374) = 0x80;
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf3b2) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xb1a);
  return;
}



void tcu_slip_control_init(void)

{
  ushort uVar1;
  undefined2 in_ram_00000b14;
  byte bVar2;
  
  DAT_0000f3ae = 0x1000;
  DAT_0000f3ac = 0x1000;
  DAT_0000f3aa = in_ram_00000b14;
  uVar1 = DAT_0000f0ec;
  DAT_0000f0ec = uVar1 & 0xfffd;
  bVar2 = 0;
  uVar1 = DAT_0000f3a4;
  DAT_0000f3a4 = uVar1 & 0xfeff;
  (*update_flag_fe97_bit2_cond)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf3ba) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xb58);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf3bc) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xb52);
  return;
}



void isc_f33e_periodic_dispatch_group_b(void)

{
  mirror_f18a_to_f18e();
  f33e_bit6_update_from_table_da2c_threshold();
  f33e_bit7_update_from_f0f8_gate();
  (*octane_level_gate_and_reset_dispatch)();
  f33e_multi_bit_update_dispatch();
  canister_purge_control_flags_f4d6_update();
  (*isc_f342_composite_correction_calc)();
  isc_f350_correction_calc();
  isc_f3b2_select();
  isc_f380_f37e_correction_calc();
  isc_f360_f362_correction_calc();
  return;
}



void mirror_f18a_to_f18e(void)

{
  DAT_0001f18e = DAT_0001f18a;
  return;
}



void f33e_bit6_update_from_table_da2c_threshold(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = table_lookup_indexed(2,(void *)0xda2c);
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf18e);
  if (uVar2 < *puVar1 || uVar2 == *puVar1) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 | 0x40;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 & 0xffbf;
  }
  return;
}



void f33e_bit7_update_from_f0f8_gate(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = f0f8_bit12_active_bit11_clear_check();
  if (uVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 & 0xff7f;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 | 0x80;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f0f8_bit12_active_bit11_clear_check(void)

{
  ushort uVar1;
  
  if (((g_status_flags_f0f8 & 0x1000) == 0) || ((g_status_flags_f0f8 & 0x800) != 0)) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



void octane_level_gate_and_reset_dispatch(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f1f2 & 0x80) == 0) {
    DAT_0000f3a2 = 0;
    uVar1 = DAT_0000f200;
    DAT_0000f200 = uVar1 & 0xfffe;
    g_knock_event_counter_eec6 = 0;
    g_octane_level_mut_0x27 = 0;
  }
  else {
    (*tcu_slip_and_ff54_reinit_on_f20e_bit4)();
    (*per_cyl_knock_flag_update_cyl_a)();
    per_cyl_knock_flag_update_cyl_b();
    (*f3a4_bit10_update_dispatch)();
    f3ba_f3bc_f3a4_hysteresis_update();
    f3a4_bit8_hysteresis_update();
    knock_event_counter_update_eec6();
    (*knock_counter_condition_override_eec6)();
    (*octane_level_compute_from_knock_counter)();
    (*efc2_threshold_update)();
  }
  return;
}



void tcu_slip_and_ff54_reinit_on_f20e_bit4(void)

{
  if ((g_status_flags_f20e & 0x10) != 0) {
    tcu_slip_control_init();
    init_state_block_ff54();
  }
  return;
}



void per_cyl_knock_flag_update_cyl_a(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  sVar2 = (*tcu_slip_duty_gain_condition_check)();
  if (sVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 & 0xffbf;
    DAT_0000f3a2 = 0;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 | 0x40;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort tcu_slip_duty_gain_condition_check(void)

{
  if ((((g_status_flags_f20e & 0x11) == 0) && ((g_status_flags_f33e & 0x80) == 0)) &&
     (DAT_00010b06 <= engine_torque_pct_f17a)) {
    return 1;
  }
  return 0;
}



void per_cyl_knock_flag_update_cyl_b(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = f3a4_bit12_13_update_and_check();
  if (uVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 & 0xff7f;
    uVar2 = g_f3a2_hold_min_timer_ef88;
    g_f3a2_hold_min_timer_ef88 = uVar2 & 3;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 | 0x80;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f3a4_bit12_13_update_and_check(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = table_lookup_interp(1,(void *)0x2832);
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf186);
  if (uVar2 < *puVar1 || uVar2 == *puVar1) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 | 0x1000;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 & 0xefff;
  }
  if ((((*(ushort *)((uint)bVar3 << 0x10 | 0xf1fe) & 8) == 0) &&
      ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x80) == 0)) &&
     ((*(ushort *)((uint)bVar3 << 0x10 | 0xf3a4) & 0x1000) != 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 | 0x2000;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 & 0xdfff;
  }
  if (((*(ushort *)((uint)bVar3 << 0x10 | 0xf1fe) & 8) == 0) &&
     ((*(ushort *)((uint)bVar3 << 0x10 | 0xf3a4) & 0x1000) == 0)) {
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}



void f3a4_bit10_update_dispatch(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  sVar2 = (*f0ec_bit1_threshold_gate_check)();
  if (sVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 & 0xfbff;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3a4);
    *puVar1 = *puVar1 | 0x400;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f0ec_bit1_threshold_gate_check(void)

{
  ushort uVar1;
  
  uVar1 = DAT_00010f88;
  if ((DAT_0001f0ec & 2) != 0) {
    uVar1 = DAT_00010f86;
  }
  if ((engine_torque_pct_f17a < uVar1) || (inj_pw_output_f186 < DAT_00010f84)) {
    return 0;
  }
  return 1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f3ba_f3bc_f3a4_hysteresis_update(void)

{
  DAT_0001f3ba = DAT_00010b58;
  if ((knock_per_cyl_flags_f3a4 & 0x800) == 0) {
    if (DAT_00010b54 < engine_torque_pct_f17a) {
      knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 | 0x800;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_00010b56) {
    knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 & 0xf7ff;
  }
  if ((knock_per_cyl_flags_f3a4 & 0x800) == 0) {
    DAT_0001f3bc = DAT_00010b52;
  }
  else {
    DAT_0001f3bc = DAT_00010b50;
  }
  if ((engine_torque_pct_f17a < DAT_00010b5c) || (DAT_00010b5a < engine_torque_pct_f17a)) {
    knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 & 0xfdff;
  }
  else {
    knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 | 0x200;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f3a4_bit8_hysteresis_update(void)

{
  if ((knock_per_cyl_flags_f3a4 & 0x100) == 0) {
    if (DAT_00010b34 < engine_torque_pct_f17a) {
      knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 | 0x100;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_00010b30) {
    knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 & 0xfeff;
  }
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// knock_event_counter_update_eec6: reads Knock Sum @0xF3A2 (already independently confirmed,
// see tcu_shift_torque_and_knock_mgmt @0x28fff), hysteresis-compares against ROM constants
// @0xB3E/0xB40, increments/decrements/clamps 0xEEC6 event counter. Feeds
// octane_level_compute_from_knock_counter (0x252d3). Chain matches logging.txt CONFIRMED
// claim exactly. VERIFIED.

void knock_event_counter_update_eec6(void)

{
  ushort *puVar1;
  short *psVar2;
  ushort uVar3;
  short sVar4;
  byte bVar5;
  bool bVar6;
  
  bVar5 = 1;
  uVar3 = f3a4_bit6_gate_condition_check();
  if (uVar3 != 0) {
    uVar3 = *(ushort *)((uint)bVar5 << 0x10 | 0xf3a2);
    bVar6 = *(ushort *)((uint)bVar5 << 0x10 | 0xf3a2) < *(ushort *)((uint)bVar5 << 0x10 | 0xb3e);
    if (((bVar6) ||
        (puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xb40), *puVar1 <= uVar3 && uVar3 != *puVar1)) &&
       (*(short *)((uint)bVar5 << 0x10 | 0xefc0) == 0)) {
      if (bVar6) {
        sVar4 = *(short *)((uint)bVar5 << 0x10 | 0xeec6) + 1;
        *(short *)((uint)bVar5 << 0x10 | 0xeec6) = sVar4;
        if (sVar4 == 0) {
          psVar2 = (short *)((uint)bVar5 << 0x10 | 0xeec6);
          *psVar2 = *psVar2 + -1;
        }
        *(undefined2 *)((uint)bVar5 << 0x10 | 0xefc0) = *(undefined2 *)((uint)bVar5 << 0x10 | 0xb42)
        ;
      }
      else {
        if (*(short *)((uint)bVar5 << 0x10 | 0xeec6) != 0) {
          psVar2 = (short *)((uint)bVar5 << 0x10 | 0xeec6);
          *psVar2 = *psVar2 + -1;
        }
        *(undefined2 *)((uint)bVar5 << 0x10 | 0xefc0) = *(undefined2 *)((uint)bVar5 << 0x10 | 0xb44)
        ;
      }
      uVar3 = clamp_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xeec6),0,0xff);
      *(ushort *)((uint)bVar5 << 0x10 | 0xeec6) = uVar3;
    }
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f3a4_bit6_gate_condition_check(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((((g_tps_load_axis_f10e < DAT_00010b3c) || ((DAT_0001f200 & 1) != 0)) ||
      ((g_status_flags_f1fe & 8) != 0)) ||
     (((knock_per_cyl_flags_f3a4 & 0x40) == 0 ||
      (uVar2 = table_lookup_interp(1,(void *)0x2840),
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf186), *puVar1 <= uVar2 && uVar2 != *puVar1)))) {
    return 0;
  }
  return 1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void knock_counter_condition_override_eec6(void)

{
  ushort uVar1;
  
  uVar1 = knock_per_cyl_flags_f3a4;
  if ((g_status_flags_f1f2 & 0x100) != 0) {
    if ((DAT_0001f0fe & 1) == 0) {
      knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 & 0xfffb;
    }
    else {
      knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 | 4;
    }
    if (((knock_per_cyl_flags_f3a4 ^ uVar1) & 4) != 0) {
      DAT_0001efbe = DAT_00010b5e;
    }
    if (DAT_0001efbe == 0) {
      if ((knock_per_cyl_flags_f3a4 & 4) == 0) {
        knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 & 0xfffd;
      }
      else if ((knock_per_cyl_flags_f3a4 & 2) == 0) {
        knock_per_cyl_flags_f3a4 = knock_per_cyl_flags_f3a4 | 2;
        DAT_0001eec6 = 0xff;
      }
    }
    if ((knock_per_cyl_flags_f3a4 & 2) == 0) {
      DAT_0001eec6 = 0;
    }
  }
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// octane_level_compute_from_knock_counter: reads 0xEEC6 event counter (from
// knock_event_counter_update_eec6 @0x251c4, gated by 0xF1FE bit3 / 0xF200 bit0),
// writes result @0xEEC8/EEC9. MUT RequestID 0x27 -> table entry 0xEEC9 (low byte
// of the word written here) = Octane Level. VERIFIED - logging.txt CONFIRMED-section
// claim holds under current decoder. Full chain (Knock Sum -> event counter -> octane
// level) independently re-verified end to end.

void octane_level_compute_from_knock_counter(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = (*o2_downstream_clamp_by_type)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xeec8) = uVar1;
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// efc2_threshold_update (formerly FUN_000252f9 in logging.txt): gates on 0xF17E magnitude
// threshold, writes constant @0xB24 to 0xEFC2. Second writer confirmed at 0x2b676 inside
// tcu_rx_main_scheduler, gated purely on throttle-position delta (0xF13C current vs 0xF142
// previous TPS reading, threshold @0xB20). NOT knock-related in either writer - both are
// throttle/crank-transient delta detectors. CONFIRMS logging.txt's REFUTED/MISLABELED
// claim: knock_flag (MUT RequestID 0x6F -> RAM 0xEFC2/EFC3) is a genuine mislabel inherited
// from the stock Evo profile, VERIFIED under current decoder.

void efc2_threshold_update(void)

{
  undefined2 in_ram_00000b24;
  
  if (0x7f < DAT_0001f17e) {
    if (DAT_00010b22 <= DAT_0001f17e - 0x80) {
      g_delta_threshold_calib_load_efc2 = in_ram_00000b24;
    }
  }
  return;
}



void f33e_multi_bit_update_dispatch(void)

{
  f33e_bit2_update_dispatch();
  f33e_bit3_update_dispatch();
  (*f33e_bit12_clear)();
  f20e_bit15_update_dispatch();
  f33e_bit4_update_dispatch();
  return;
}



void f33e_bit2_update_dispatch(void)

{
  ushort *puVar1;
  undefined2 in_ram_00000bbc;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (DAT_000102d4 != '\0') {
    uVar2 = f33e_bit2_source_condition_check();
    if (uVar2 != 0) {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
      *puVar1 = *puVar1 | 4;
      DAT_0000f378 = in_ram_00000bbc;
      goto LAB_00025372;
    }
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
  *puVar1 = *puVar1 & 0xfffb;
LAB_00025372:
  (*f37a_correction_calc_from_f33e_bit2_or_f378)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f33e_bit2_source_condition_check(void)

{
  if (((((DAT_00010bac < DAT_0001f14c) && (DAT_0001f18e <= DAT_00010bb2)) &&
       (((g_status_flags_f0f8 & 0x20) != 0 || (DAT_000102d4 == '\x02')))) &&
      ((((DAT_00010bae <= g_tps_load_axis_f10e && (DAT_00010bb0 <= g_f1e6_hysteresis_gate)) &&
        (DAT_00010bb4 <= engine_torque_pct_f17a)) &&
       ((engine_torque_pct_f17a <= DAT_00010bb6 && (DAT_0001f242 >> 1 < DAT_00010bb8)))))) &&
     (DAT_00010bba < DAT_0001f242 >> 1)) {
    return 1;
  }
  return 0;
}



void f37a_correction_calc_from_f33e_bit2_or_f378
               (undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort uVar1;
  undefined2 uVar2;
  ushort extraout_R2;
  byte bVar3;
  
  bVar3 = 1;
  if (((g_status_flags_f33e & 4) != 0) || (DAT_0001f378 != 0)) {
    (*axis_lookup_interp)(param_1,param_2,0x80,2,0xd180,param_3);
    table_lookup_interp(1,(void *)0x284e);
    if (*(char *)((uint)bVar3 << 0x10 | 0x2c1) == '\0') {
      (*axis_lookup_interp)();
      table_lookup_interp(1,(void *)0x2858);
    }
    uVar1 = table_lookup_interp(1,(void *)0x2860);
    (*muldiv_s16_rounded)((short)((uint)uVar1 * (uint)extraout_R2 >> 0x10));
    uVar2 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf37a) = uVar2;
  }
  return;
}



void f33e_bit3_update_dispatch(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((DAT_000102d1 == '\0') || (uVar2 = f33e_bit5_update_and_condition_check(), uVar2 == 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 & 0xfff7;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
  *puVar1 = *puVar1 | 8;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f33e_bit5_update_and_condition_check(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  if (DAT_00010bc6 < engine_torque_pct_f17a) {
    g_status_flags_f33e = g_status_flags_f33e | 0x20;
  }
  else {
    g_status_flags_f33e = g_status_flags_f33e & 0xffdf;
  }
  if ((((DAT_00010bc0 < g_tps_load_axis_f10e) && (DAT_00010bc2 <= engine_torque_pct_f17a)) &&
      (engine_torque_pct_f17a <= DAT_00010bc4)) &&
     ((uVar1 = table_lookup_interp(1,(void *)0x286c),
      uVar1 < *(ushort *)((uint)bVar2 << 0x10 | 0xf18e) &&
      (*(ushort *)((uint)bVar2 << 0x10 | 0xf13e) < *(ushort *)((uint)bVar2 << 0x10 | 0xbbe))))) {
    return 1;
  }
  return 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f33e_bit12_clear(void)

{
  g_status_flags_f33e = g_status_flags_f33e & 0xefff;
  return;
}



undefined2 f33e_bit11_bit10_hysteresis_gate_check(void)

{
  undefined2 uVar1;
  
  if ((g_status_flags_f33e & 0x800) == 0) {
    if (DAT_00010ba4 < coolant_temp_scaled_f130) {
      g_status_flags_f33e = g_status_flags_f33e | 0x800;
    }
  }
  else if (coolant_temp_scaled_f130 <= DAT_00010ba2) {
    g_status_flags_f33e = g_status_flags_f33e & 0xf7ff;
  }
  if ((g_status_flags_f33e & 0x400) == 0) {
    if (DAT_00010ba6 < g_tps_load_axis_f10e) {
      g_status_flags_f33e = g_status_flags_f33e | 0x400;
    }
  }
  else if (g_tps_load_axis_f10e <= DAT_00010ba8) {
    g_status_flags_f33e = g_status_flags_f33e & 0xfbff;
  }
  if (((inj_pw_output_f186 < PTR_vec_trapa4_00010b9e._2_2_) || ((g_status_flags_f33e & 0x800) == 0))
     || ((g_status_flags_f33e & 0x400) == 0)) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



void f20e_bit15_update_dispatch(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (((g_status_flags_f1f2 & 0x400) == 0) ||
     (uVar2 = f20e_bit15_source_condition_check(), uVar2 == 0)) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf20e);
    *puVar1 = *puVar1 & 0x7fff;
    return;
  }
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf20e);
  *puVar1 = *puVar1 | 0x8000;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f20e_bit15_source_condition_check(void)

{
  if (((((((g_status_flags_f0f8 & 0x80) == 0) && ((g_status_flags_f0f8 & 0x20) != 0)) &&
        (engine_torque_pct_f17a < DAT_00010b7a)) &&
       ((DAT_00010b7c < engine_torque_pct_f17a && (DAT_00010b82 < g_tps_load_axis_f10e)))) &&
      ((DAT_00010d96 < g_f1e6_hysteresis_gate &&
       (((isc_stepper_position_eed0 & 4) == 0 && ((g_status_flags_f20e & 0x11) == 0)))))) &&
     ((DAT_0001f242 < DAT_00010b7e && (DAT_00010b80 < DAT_0001f242)))) {
    return 1;
  }
  return 0;
}



void f33e_bit4_update_dispatch(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = f33e_bit1_hysteresis_gate_check();
  if (uVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 & 0xffef;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf33e);
    *puVar1 = *puVar1 | 0x10;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f33e_bit1_hysteresis_gate_check(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f33e & 2) == 0) {
    if (DAT_00010aee < engine_torque_pct_f17a) {
      g_status_flags_f33e = g_status_flags_f33e | 2;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_00010af0) {
    g_status_flags_f33e = g_status_flags_f33e & 0xfffd;
  }
  if (((((g_status_flags_f0f8 & 0x80) == 0) || ((g_status_flags_f33e & 2) != 0)) ||
      ((isc_stepper_position_eed0 & 4) != 0)) || ((g_status_flags_f20e & 0x19) != 0)) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



void f33e_bit13_bit11_clear(void)

{
  g_status_flags_f33e = g_status_flags_f33e & 0x9fff;
  return;
}



void isc_f342_composite_correction_calc(void)

{
  short sVar1;
  short extraout_R2;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  
  bVar4 = 1;
  isc_f344_correction_calc();
  (*isc_f36a_f36c_target_calc)();
  if (*(char *)((uint)bVar4 << 0x10 | 0x2f9) != '\0') {
    (*isc_f37c_ramp_target_calc)();
  }
  (*isc_f352_table_lookup)();
  (*isc_f354_table_lookup_conditional)();
  isc_f356_correction_calc();
  (*isc_f358_correction_calc)();
  isc_f35c_correction_calc();
  sVar1 = (*isc_f35a_correction_calc)();
  uVar3 = extraout_R2 + sVar1 + 0x7d80;
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf358) < 0x80) &&
     (uVar2 = *(short *)((uint)bVar4 << 0x10 | 0xaf4) + 0x8000, uVar3 < uVar2)) {
    uVar3 = uVar2;
  }
  sVar1 = (*isc_f374_correction_calc)();
  uVar3 = sat_sub_u16((uVar3 + sVar1) - 0x80,0x8000);
  uVar2 = (*isc_f36e_f370_target_calc)();
  if (uVar2 <= uVar3) {
    uVar3 = uVar2;
  }
  if ((*(short *)((uint)bVar4 << 0x10 | 0xf35a) != 0x80) &&
     (uVar3 < *(ushort *)((uint)bVar4 << 0x10 | 0xb94))) {
    uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xb94);
  }
  if (((*(ushort *)((uint)bVar4 << 0x10 | 0xf33e) & 0x80) != 0) ||
     ((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x11) != 0)) {
    uVar3 = *(short *)((uint)bVar4 << 0x10 | 0xac6) + 0x14;
  }
  uVar2 = 10;
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf1f2) & 0x2000) != 0) {
    uVar2 = table_lookup_interp(1,(void *)0x2cf4);
  }
  uVar3 = clamp_u16(uVar3,uVar2,0x46);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf342) = uVar3;
  return;
}



void isc_f344_correction_calc(void)

{
  undefined2 uVar1;
  byte bVar2;
  ushort unaff_retaddr;
  ushort in_stack_00000002;
  
  bVar2 = 1;
  isc_f34e_f34a_correction_calc();
  isc_f34c_correction_calc(unaff_retaddr);
  (*isc_f4d6_gated_offset_calc)();
  uVar1 = o2_sensor2_correction_apply(in_stack_00000002);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf344) = uVar1;
  return;
}



ushort isc_f34e_f34a_correction_calc(void)

{
  undefined2 uVar1;
  ushort a;
  ushort b;
  short extraout_R2;
  short extraout_R2_00;
  short sVar2;
  byte bVar3;
  void *pvVar4;
  ushort uStack_4;
  
  bVar3 = 1;
  uVar1 = (*isc_f35e_correction_calc)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf0ba) = uVar1;
  (*axis_lookup_interp)();
  (*axis_lookup_interp)();
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0) {
    table_lookup_indexed(2,(void *)0xdbcc);
    pvVar4 = (void *)0xda0c;
  }
  else {
    table_lookup_indexed(2,(void *)0xdbac);
    pvVar4 = (void *)0xd9ec;
  }
  uStack_4 = table_lookup_indexed(2,pvVar4);
  *(short *)((uint)bVar3 << 0x10 | 0xf346) = extraout_R2;
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf1f2) & 0x80) == 0) {
    a = *(ushort *)((uint)bVar3 << 0x10 | 0xf346);
    sVar2 = extraout_R2;
  }
  else {
    if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0) {
      pvVar4 = (void *)0xdb8c;
    }
    else {
      pvVar4 = (void *)0xdb6c;
    }
    uVar1 = table_lookup_indexed(2,pvVar4);
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf348) = uVar1;
    a = (*weighted_blend_u8frac)();
    sVar2 = extraout_R2_00;
  }
  if ((*(char *)((uint)bVar3 << 0x10 | 0x2fa) != '\0') &&
     ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0)) {
    (*axis_lookup_interp)();
    (*axis_lookup_interp)();
    b = table_lookup_interp(1,(void *)0x27a8);
    uStack_4 = sat_add_u16(a,b);
    a = (*clamp_u8)();
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf34e) = a;
  sat_sub_u16(sVar2 + uStack_4,0x80);
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf34a) = uVar1;
  return *(ushort *)((uint)bVar3 << 0x10 | 0xf34e);
}



short isc_f35e_correction_calc(void)

{
  short sVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((g_status_flags_f1fe & 8) == 0) {
    table_lookup_interp(1,(void *)0x27b0);
    sVar1 = (*mul_fixedpoint_8)((short)((uint)*(ushort *)((uint)bVar3 << 0x10 | 0xf366) *
                                        (uint)*(ushort *)((uint)bVar3 << 0x10 | 0xae0) >> 0x10));
    uVar2 = select_value_f194_f196();
    sVar1 = uVar2 + sVar1;
  }
  else {
    sVar1 = table_lookup_indexed(2,(void *)0xda2c);
    sVar1 = sVar1 << 1;
  }
  *(short *)((uint)bVar3 << 0x10 | 0xf35e) = sVar1;
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort select_value_f194_f196(void)

{
  ushort uVar1;
  
  if ((DAT_00010f6e < g_iat_axis_f118) || (DAT_00010304 != '\0')) {
    uVar1 = DAT_0001f194;
  }
  else {
    uVar1 = DAT_0001f196;
  }
  return uVar1;
}



ushort isc_f34c_correction_calc(ushort param_fp1)

{
  undefined2 uVar1;
  ushort uVar2;
  byte bVar3;
  ushort in_stack_00000004;
  
  bVar3 = 1;
  if ((g_status_flags_f20e & 0x11) == 0) {
    if ((g_status_flags_f0fa & (g_status_flags_f0f8 ^ g_status_flags_f0fa) & 0x80) == 0) {
      if (DAT_0001f05a == 0) {
        if (DAT_0001f030 == 0) {
          DAT_0001f030 = DAT_000113dc << 1;
          uVar1 = (*clamp_u8)();
          *(undefined2 *)((uint)bVar3 << 0x10 | 0xf3ee) = uVar1;
        }
      }
      else {
        DAT_0001f030 = DAT_000113dc << 1;
      }
    }
    else {
      DAT_0001f05a = DAT_000113d8;
      DAT_0001f3ee = 0;
    }
  }
  else {
    DAT_0001f3ee = 0xff;
  }
  if (((*(ushort *)((uint)bVar3 << 0x10 | 0xf1f2) & 0x200) != 0) &&
     (*(ushort *)((uint)bVar3 << 0x10 | 0xb78) <= *(ushort *)((uint)bVar3 << 0x10 | 0xef6a))) {
    (*axis_lookup_interp)();
    table_lookup_interp(1,(void *)0x27fc);
    uVar2 = (*weighted_blend_u8frac)();
    if (uVar2 <= in_stack_00000004) {
      in_stack_00000004 = uVar2;
    }
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf34c) = in_stack_00000004;
  return in_stack_00000004;
}



// [CHECKED 2026-07-22] isc_f4d6_gated_offset_calc: gated by isc_f4d6_gate_condition_check
// and a >0x13 threshold check, computes a saturated 2x-multiply offset. Return
// value only (in_stack_00000004) -- decompiler shows no direct RAM store in this
// function body; caller not traced this session. Checked as a candidate producer
// for the F4Dx/F4Ex MUT ReqID 0x8C-0x93 cluster (mut_verification_status.md) and
// ruled out -- no write to any of F84F/EEFB/F4DF/F4DD/F4E5/F4E7/F4EB/F4DB found here.

ushort isc_f4d6_gated_offset_calc(ushort param_fp1)

{
  short sVar1;
  ushort in_stack_00000004;
  
  if (((DAT_000102ea != '\0') && (sVar1 = (*isc_f4d6_gate_condition_check)(), sVar1 != 0)) &&
     (0x13 < in_stack_00000004)) {
    sVar1 = (*mul_u16_x2_sat)();
    in_stack_00000004 = sVar1 + 0x14;
  }
  return in_stack_00000004;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f4d6_gate_condition_check(void)

{
  if (((((g_status_flags_f1f2 & 0x2000) == 0) || (DAT_0001f374 == 0x80)) &&
      ((g_status_flags_f0f8 & 0x40) == 0)) &&
     ((DAT_00010e48 <= DAT_0001ef6c && ((DAT_0001f4d6 & 0x23) == 0x23)))) {
    return 1;
  }
  return 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isc_f36a_f36c_target_calc(void)

{
  undefined2 in_ram_00000afc;
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((g_status_flags_f20e & 0x10) == 0) {
    if (DAT_00010b02 <= g_idle_rpm_actual_f178) {
      if (DAT_0001f14c < DAT_000109ba) {
        if (DAT_0001f1c8 < DAT_00010b00) goto LAB_00025a97;
      }
    }
  }
  DAT_0000f36a = 0x80;
  bVar3 = 0;
LAB_00025a97:
  if (((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) != 0) &&
     ((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 4) != 0)) {
    uVar2 = *(short *)((uint)bVar3 << 0x10 | 0xafa) + 0x14;
    uVar1 = sat_sub_u16(*(ushort *)((uint)bVar3 << 0x10 | 0xf344),
                        *(ushort *)((uint)bVar3 << 0x10 | 0xaf8));
    if (uVar1 < uVar2) {
      DAT_0000f36a = uVar2;
    }
    else {
      DAT_0000f36a = uVar1;
    }
    DAT_0000f36c = in_ram_00000afc;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isc_f37c_ramp_target_calc(void)

{
  ushort c;
  ushort uVar1;
  byte bVar2;
  ushort in_stack_0000fff8;
  
  bVar2 = 1;
  uVar1 = 0x80;
  if ((((((DAT_000102c8 == '\0') || (g_rpm_zone_dwell_counter_ef6a <= (ushort)(DAT_00010bda * 0x14))
         ) || (g_tps_load_axis_f10e < DAT_00010bdc)) ||
       (((g_status_flags_f0f8 & 0x80) == 0 || (g_idle_rpm_actual_f178 < DAT_00010bde)))) ||
      (((g_purge_enable_cond_f4a4 & 0x80) != 0 ||
       (((tcu_rx_confirmed_byte2_f4c0 & 0x20) == 0 || ((DAT_0001f4b4 & 0x80) != 0)))))) ||
     (((tcu_rx_confirmed_byte1_f4be & 0xc0) != 0 || ((g_status_flags_f0f8 & 0x20) != 0)))) {
    if (g_isc_ramp_direction_flag == 0) {
      DAT_0001ef72 = 0;
    }
    else {
      DAT_0001ef72 = 0xffff;
    }
    goto LAB_00025be5;
  }
  if (DAT_0001ef72 <= DAT_00010be0 + DAT_00010be2) {
    g_isc_ramp_direction_flag = DAT_00010be6;
  }
  uVar1 = DAT_0001ef72;
  if (DAT_0001ef72 < DAT_00010be0) {
    c = DAT_00010be4;
LAB_00025bb7:
    uVar1 = muldiv_u16(in_stack_0000fff8,uVar1,c);
  }
  else {
    if (DAT_00010be2 != 0) {
      in_stack_0000fff8 = DAT_00010be0 + DAT_00010be2;
      c = sat_sub_u16(in_stack_0000fff8,DAT_0001ef72);
      goto LAB_00025bb7;
    }
    uVar1 = 0;
  }
  uVar1 = sat_sub_u16(*(ushort *)((uint)bVar2 << 0x10 | 0xf34e),uVar1);
LAB_00025be5:
  if (uVar1 < 0x14) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf37c) = 0x14;
  }
  else {
    *(ushort *)((uint)bVar2 << 0x10 | 0xf37c) = uVar1;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isc_f352_table_lookup(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = table_lookup_interp(1,(void *)0x27cc);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf352) = uVar1;
  return;
}



ushort isc_f354_table_lookup_conditional(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = 0x80;
  if (cal_fuel_cut_enable_flag == '\0') {
    (*axis_lookup_interp)();
    uVar1 = table_lookup_interp(1,(void *)0x27e4);
  }
  *(ushort *)((uint)bVar2 << 0x10 | 0xf354) = uVar1;
  return uVar1;
}



ushort isc_f356_correction_calc(void)

{
  ushort uVar1;
  ushort uVar2;
  short sVar3;
  ushort extraout_R2;
  ushort a;
  byte bVar4;
  
  bVar4 = 1;
  uVar1 = table_lookup_interp(1,(void *)0x27ee);
  uVar2 = extraout_R2;
  if (uVar1 < *(ushort *)((uint)bVar4 << 0x10 | 0xf18e)) {
    uVar2 = table_lookup_interp(1,(void *)0x27d8);
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf226) & 2) != 0) {
    uVar2 = sat_sub_u16(uVar2,*(ushort *)((uint)bVar4 << 0x10 | 0xa78));
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf33e) & 0x1000) != 0) {
    uVar2 = sat_sub_u16(uVar2,*(ushort *)((uint)bVar4 << 0x10 | 0xbaa));
  }
  if (((*(char *)((uint)bVar4 << 0x10 | 0x2d2) != '\0') &&
      (*(ushort *)((uint)bVar4 << 0x10 | 0xb96) <= *(ushort *)((uint)bVar4 << 0x10 | 0xf18e))) &&
     (*(ushort *)((uint)bVar4 << 0x10 | 0xb9a) <= *(ushort *)((uint)bVar4 << 0x10 | 0xf10e))) {
    if (*(ushort *)((uint)bVar4 << 0x10 | 0xf10e) < *(ushort *)((uint)bVar4 << 0x10 | 0xb98)) {
      uVar1 = *(ushort *)((uint)bVar4 << 0x10 | 0xb9e);
    }
    else {
      uVar1 = *(ushort *)((uint)bVar4 << 0x10 | 0xb9c);
    }
    sat_sub_u16(uVar2,uVar1);
  }
  sVar3 = (*f33e_bit15_hysteresis_update)();
  uVar2 = a;
  if (sVar3 != 0) {
    uVar2 = sat_sub_u16(a,*(ushort *)((uint)bVar4 << 0x10 | 0x14c4));
  }
  *(ushort *)((uint)bVar4 << 0x10 | 0xf356) = uVar2;
  return uVar2;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f33e_bit15_hysteresis_update(void)

{
  if ((g_status_flags_f33e & 0x8000) == 0) {
    if (DAT_000114bc < engine_torque_pct_f17a) {
      g_status_flags_f33e = g_status_flags_f33e | 0x8000;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_000114ba) {
    g_status_flags_f33e = g_status_flags_f33e & 0x7fff;
  }
  return 0;
}



void isc_f358_correction_calc(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  undefined2 uVar5;
  byte bVar6;
  
  bVar6 = 1;
  if ((g_status_flags_f33e & 0x10) == 0) {
    if (((g_status_flags_f20e & 0x8000) == 0) ||
       (uVar4 = sat_sub_u16(g_throttle_body_pos_target_f170,(DAT_0001f182 >> 3) + 1 >> 1),
       puVar1 = (ushort *)((uint)bVar6 << 0x10 | 0xb86), uVar4 < *puVar1 || uVar4 == *puVar1))
    goto LAB_00025e0b;
    uVar4 = (*mul_fixedpoint_8)();
  }
  else {
    uVar3 = (*mul_u16_x2_sat)();
    uVar4 = (*(ushort *)((uint)bVar6 << 0x10 | 0xf180) >> 3) + 1 >> 1;
    if (uVar3 < uVar4) {
      uVar3 = uVar4;
    }
    uVar2 = *(ushort *)((uint)bVar6 << 0x10 | 0xf170);
    uVar4 = (*mul_fixedpoint_8)();
    if (((*(ushort *)((uint)bVar6 << 0x10 | 0xf0f8) & 0x20) == 0) &&
       (*(char *)((uint)bVar6 << 0x10 | 0x2fa) != '\0')) {
      if (*(ushort *)((uint)bVar6 << 0x10 | 0xae8) <= uVar4) {
        uVar4 = *(ushort *)((uint)bVar6 << 0x10 | 0xae8);
      }
    }
    else if (*(ushort *)((uint)bVar6 << 0x10 | 0xae6) <= uVar4) {
      uVar4 = *(ushort *)((uint)bVar6 << 0x10 | 0xae6);
    }
    if (uVar2 <= uVar3) {
      (*clamp_u8)();
      goto LAB_00025e0b;
    }
  }
  sat_sub_u16(0x80,uVar4);
LAB_00025e0b:
  uVar5 = (*o2_sensor2_mode3_midpoint)();
  *(undefined2 *)((uint)bVar6 << 0x10 | 0xf358) = uVar5;
  return;
}



ushort isc_f35a_correction_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = 0x80;
  if ((((DAT_000102d3 != '\0') && (g_rpm_zone_dwell_counter_ef6a < DAT_00010b8c)) &&
      (DAT_00010b8a <= g_tps_load_axis_f10e)) && ((g_status_flags_f0f8 & 0x80) != 0)) {
    sat_sub_u16(g_throttle_body_pos_target_f170,DAT_00010b8e);
    uVar1 = (*mul_fixedpoint_8)();
    if (*(ushort *)((uint)bVar2 << 0x10 | 0xb90) <= uVar1) {
      uVar1 = *(ushort *)((uint)bVar2 << 0x10 | 0xb90);
    }
    uVar1 = sat_sub_u16(0x80,uVar1);
  }
  *(ushort *)((uint)bVar2 << 0x10 | 0xf35a) = uVar1;
  return uVar1;
}



ushort isc_f374_correction_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = 0x80;
  if ((g_status_flags_f1f2 & 0x2000) != 0) {
    if ((((g_purge_enable_cond_f4a2 & 0x80) == 0) || ((g_purge_enable_cond_f4a4 & 0x80) != 0)) ||
       ((tcu_rx_confirmed_byte1_f4be & 0xc0) == 0)) {
      uVar1 = DAT_0001f376;
    }
    else {
      uVar1 = select_value_f194_f196();
      *(ushort *)((uint)bVar2 << 0x10 | 0xf0ba) = uVar1;
      (*axis_lookup_interp)();
      (*axis_lookup_interp)();
      table_lookup_interp(1,(void *)0x2b94);
      uVar1 = (*muldiv_s16_rounded)();
      DAT_0000f376 = uVar1;
      bVar2 = 0;
    }
    uVar1 = sat_sub_u16(0x80,uVar1);
  }
  *(ushort *)((uint)bVar2 << 0x10 | 0xf374) = uVar1;
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f36e_f370_target_calc(void)

{
  undefined2 in_ram_00000bd4;
  undefined2 in_ram_00000bd6;
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  if ((DAT_000102d5 != '\0') && ((g_status_flags_f20e & 0x10) == 0)) {
    if (DAT_00010bca <= DAT_0001f18e) {
      if (DAT_0001f18e <= DAT_00010bc8) {
        if (DAT_00010bce <= engine_torque_pct_f17a) {
          if ((engine_torque_pct_f17a <= DAT_00010bcc) && ((g_status_flags_f0f8 & 0x80) != 0)) {
            if ((g_status_flags_f0f8 & 0x20) != 0) {
              if ((DAT_00010bd0 <= g_tps_load_axis_f10e) && ((g_status_flags_f0f8 & 4) != 0)) {
                uVar1 = sat_sub_u16(DAT_0001f17e,0x80);
                if (*(ushort *)((uint)bVar2 << 0x10 | 0xbd2) <= uVar1) {
                  DAT_0000f370 = in_ram_00000bd6;
                  DAT_0000f36e = in_ram_00000bd4;
                  bVar2 = 0;
                }
              }
            }
            goto LAB_00025fe8;
          }
        }
      }
    }
  }
  DAT_0000f36e = 0x80;
  bVar2 = 0;
LAB_00025fe8:
  return *(ushort *)((uint)bVar2 << 0x10 | 0xf36e);
}



void isc_f350_correction_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = (*div_u16_rounded)();
  uVar1 = sat_sub_u16(*(short *)((uint)bVar2 << 0x10 | 0xac4) + 0x39,uVar1);
  *(ushort *)((uint)bVar2 << 0x10 | 0xf350) = uVar1;
  return;
}



void isc_f3b2_select(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  short sVar1;
  undefined2 extraout_R2;
  undefined2 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  sVar1 = (*o2_wideband_mode82_check)(param_1,param_2,BYTE_ARRAY_00010b1a,param_3);
  uVar2 = extraout_R2;
  if (sVar1 != 0) {
    uVar2 = *(undefined2 *)((uint)bVar3 << 0x10 | 0xb1a);
  }
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf3b2) = uVar2;
  return;
}



void isc_f380_f37e_correction_calc(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  short sVar4;
  ushort uVar5;
  ushort a;
  byte bVar6;
  
  uVar2 = DAT_0001f5ca;
  uVar1 = DAT_00010adc;
  bVar6 = 1;
  (*axis_lookup_interp)();
  uVar3 = table_lookup_interp(1,(void *)0x2824);
  *(undefined2 *)((uint)bVar6 << 0x10 | 0xf37e) = uVar3;
  sVar4 = table_lookup_interp(1,(void *)0x2808);
  uVar5 = sVar4 << 4;
  if (uVar2 <= uVar1) {
    table_lookup_interp(1,(void *)0x2816);
    uVar5 = (*mul_fixedpoint_8)();
    uVar5 = sat_sub_u16(a,uVar5);
  }
  *(ushort *)((uint)bVar6 << 0x10 | 0xf380) = uVar5;
  *(ushort *)((uint)bVar6 << 0x10 | 0xf85e) = uVar5 >> 4;
  return;
}



void isc_f360_f362_correction_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = (*mul_fixedpoint_8)();
  uVar1 = sat_sub_u16(0x51,uVar1);
  *(ushort *)((uint)bVar2 << 0x10 | 0xf360) = uVar1;
  *(ushort *)((uint)bVar2 << 0x10 | 0xf362) =
       (short)&DAT_0000ffec + *(short *)((uint)bVar2 << 0x10 | 0xf360) & 0xff;
  return;
}



void isc_stepper_position_reinit(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  DAT_0001eece = DAT_00010c74;
  DAT_0001eecc = DAT_00010c74;
  DAT_0001eeca = DAT_00010c74;
  isc_stepper_position_eed0 = 0x80;
  uVar1 = (*swap_invert_high_byte)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xeed4) = uVar1;
  return;
}



void isc_stepper_mode_reinit_dispatch(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort *puVar1;
  undefined2 uVar2;
  ushort uVar3;
  byte bVar4;
  
  bVar4 = 1;
  if ((isc_stepper_position_eed0 & 0xa0) == 0) {
    if ((DAT_0001eed4 & 0xff) == DAT_00010cbc) {
      DAT_0001eed2 = 3;
    }
    isc_stepper_position_eed0 = 0x40;
  }
  else {
    DAT_0001eed2 = 3;
    uVar2 = (*swap_invert_high_byte)(isc_stepper_position_eed0 & 0xa0,param_2,param_3,0);
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeed4) = uVar2;
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeed0) = 0x80;
  }
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf036) = 1;
  uVar3 = get_high_byte(*(ushort *)((uint)bVar4 << 0x10 | 0xeeca));
  *(ushort *)((uint)bVar4 << 0x10 | 0xf412) = uVar3;
  puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf3f4);
  *puVar1 = *puVar1 | 1;
  calc_f40e_threshold_select();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xef76) = 0xffff;
  return;
}



void isc_stepper_position_dispatch(void)

{
  short sVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  isc_periodic_update_dispatch();
  isc_stepper_state_dispatch();
  isc_stepper_target_dispatch();
  sVar1 = (*o2_downstream_mode17_check)();
  uVar2 = 0;
  if ((sVar1 != 0) && (uVar2 = *(ushort *)((uint)bVar3 << 0x10 | 0xeed0) & 0xb0, uVar2 == 0)) {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf408) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf5b8);
    uVar2 = idle_stepper_table_lookup_wrapper();
    *(ushort *)((uint)bVar3 << 0x10 | 0xf406) = uVar2;
    *(ushort *)((uint)bVar3 << 0x10 | 0xeed0) = *(ushort *)((uint)bVar3 << 0x10 | 0xeed0) & 0xffc0;
    uVar2 = *(ushort *)((uint)bVar3 << 0x10 | 0xf3f0) & 0xff4a;
    *(ushort *)((uint)bVar3 << 0x10 | 0xf3f0) = uVar2;
  }
  (*isc_f406_clamp_dispatch)(uVar2);
  (*mirror_f406_f408_to_f40a_f40c)();
  return;
}



void isc_periodic_update_dispatch(void)

{
  isc_f436_source_select_and_reset();
  (*isc_f416_f438_reset_on_f20e_bit4)();
  (*idle_target_rpm_compute_f400)();
  f3f0_f42e_correction_calc();
  (*isc_f426_f03a_decay_calc)();
  (*f4de_f4e2_octane_correction_calc)();
  multi_channel_update_max_dispatch();
  calc_f40e_threshold_select();
  isc_f036_f424_ramp_calc();
  isc_f416_f438_dispatch();
  stub_empty_0x26b8b();
  (*isc_f422_correction_calc)();
  isc_f084_f3f0_bit8_update();
  isc_f42a_table_lookup();
  (*isc_f42c_correction_calc)();
  f490_bits01_or_default_select();
  isc_f3f0_bit3_and_f434_update();
  stub_empty_0x26e76();
  return;
}



void isc_f436_source_select_and_reset(void)

{
  if (cal_fuel_cut_enable_flag == '\x01') {
    DAT_0001f436 = g_knock_octane_trim_ee98;
  }
  else {
    DAT_0001f436 = DAT_0001ee9a;
  }
  if ((g_status_flags_f0f8 & 0x80) == 0) {
    DAT_0001efe2 = DAT_00010c86;
  }
  DAT_0001f43a = 0;
  return;
}



void isc_f416_f438_reset_on_f20e_bit4(void)

{
  byte bVar1;
  
  bVar1 = 1;
  if ((g_status_flags_f20e & 0x10) != 0) {
    (*f3fa_f3fe_target_select_by_f0f8_bit5)();
    *(undefined2 *)((uint)bVar1 << 0x10 | 0xf416) = 0;
    *(undefined2 *)((uint)bVar1 << 0x10 | 0xf438) = 0;
    *(ushort *)((uint)bVar1 << 0x10 | 0xf3f4) = *(ushort *)((uint)bVar1 << 0x10 | 0xf3f4) & 0xffe1;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f3fa_f3fe_target_select_by_f0f8_bit5(void)

{
  if ((g_status_flags_f0f8 & 0x20) == 0) {
    DAT_0001f3fe = DAT_00010c68;
    DAT_0001f3fa = DAT_00010c68;
  }
  else {
    DAT_0001f3fe = DAT_00010c66;
    DAT_0001f3fa = DAT_00010c64;
  }
  DAT_0001f3fc = DAT_0001f3fe;
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// idle_target_rpm_compute_f400: coolant-temp-indexed table lookup (tables @0xd8ec/0xd90c
// selected by A/C flag 0xF0F8 bit5), floor clamp against 0xF41A, A/C trim offset added
// (0xD14/0xD16 depending on flag state), writes result @0xF400/F401. MUT RequestID 0x24
// -> table entry 0xF401 (low byte of the word written here) = TargetIdleRPM. VERIFIED -
// logging.txt CONFIRMED-section claim holds under current decoder, structure matches
// exactly (coolant-indexed table, A/C flag selects pair, floor clamp + trim offset).

void idle_target_rpm_compute_f400(void)

{
  ushort uVar1;
  undefined2 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((g_status_flags_f0f8 & 0x20) == 0) {
    uVar1 = table_lookup_indexed(2,(void *)0xd90c);
  }
  else {
    uVar1 = table_lookup_indexed(2,(void *)0xd8ec);
    if (((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x4000) != 0) &&
       (uVar1 < *(ushort *)((uint)bVar3 << 0x10 | 0xd24))) {
      uVar1 = *(ushort *)((uint)bVar3 << 0x10 | 0xd24);
    }
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xf418) = uVar1;
  (*isc_f41a_correction_calc)();
  uVar2 = (*clamp_u8)();
  uVar2 = o2_downstream_mode21_select(uVar2,uVar2);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf400) = uVar2;
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: ram

void isc_f41a_correction_calc(ushort param_1)

{
  undefined2 uVar1;
  short extraout_R1;
  short extraout_R2;
  byte bVar2;
  short sVar3;
  
  bVar2 = 1;
  if ((g_status_flags_f20e & 0x10) != 0) {
    isc_state_flags_f3f4 = isc_state_flags_f3f4 | 0x20;
    DAT_0001f034 = 0;
  }
  if ((g_status_flags_f20e & 1) != 0) {
    if (DAT_00010d68 < g_iat_axis_f118) {
      isc_state_flags_f3f4 = isc_state_flags_f3f4 | 0x20;
    }
    else {
      isc_state_flags_f3f4 = isc_state_flags_f3f4 & 0xffdf;
    }
    if (DAT_00010d62 < g_tps_load_axis_f10e) {
      param_1 = (ushort)((uint)DAT_00010d66 * 0x50 >> 0x10);
      DAT_0001f034 = (short)((uint)DAT_00010d66 * 0x50);
    }
    else {
      DAT_0001f034 = 0;
      param_1 = g_tps_load_axis_f10e;
    }
  }
  if ((DAT_0001f034 == 0) || ((g_status_flags_f0f8 & 0x20) == 0)) {
    DAT_0001f41a = 0;
  }
  else {
    if ((g_status_flags_f0f8 & 0x10) == 0) {
      sVar3 = DAT_00010d66 * 0x50;
      (*muldiv_s16_rounded)();
      bVar2 = 0;
      table_read_indexed(0xd8,(void *)(CONCAT22(0xd8ec,sVar3) >> 8));
      param_1 = (ushort)*(byte *)((int)(extraout_R1 + extraout_R2) + 3);
    }
    else if ((DAT_000102c7 == '\x02') && ((g_status_flags_f0f8 & 1) != 0)) {
      param_1 = (*muldiv_s16_rounded)();
    }
    uVar1 = (*clamp_u8)(param_1);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf41a) = uVar1;
  }
  return;
}



ushort f3f0_f42e_correction_calc(ushort param_1)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (DAT_000102da == '\0') {
    DAT_0001f42e = 0;
  }
  else {
    if ((g_status_flags_f0fa & (g_status_flags_f0f8 ^ g_status_flags_f0fa) & 8) != 0) {
      DAT_0001ef76 = 0;
    }
    if (g_f1e6_hysteresis_gate < DAT_00010c1a) {
      g_status_flags_f3f0 = g_status_flags_f3f0 & 0xfbff;
    }
    else {
      g_status_flags_f3f0 = g_status_flags_f3f0 | 0x400;
    }
    if ((g_status_flags_f0fa & (g_status_flags_f0f8 ^ g_status_flags_f0fa) & 8) != 0) {
      if ((g_status_flags_f3f0 & 0x400) == 0) {
        g_status_flags_f3f0 = g_status_flags_f3f0 & 0xfdff;
      }
      else {
        g_status_flags_f3f0 = g_status_flags_f3f0 | 0x200;
      }
    }
    if (((g_status_flags_f3f0 & 0x200) == 0) && ((g_status_flags_f0f8 & 0x18) == 0)) {
      if ((g_status_flags_f0f8 & 0x20) == 0) {
        uVar1 = DAT_00010c14;
      }
      else {
        uVar1 = DAT_00010c12;
      }
      param_1 = 0;
      if (uVar1 < DAT_0001ef76) {
        g_status_flags_f3f0 = g_status_flags_f3f0 & 0xf7ff | 0x1000;
      }
      else {
        g_status_flags_f3f0 = g_status_flags_f3f0 | 0x1800;
      }
    }
    else {
      param_1 = g_status_flags_f3f0 & 0xe7ff;
      g_status_flags_f3f0 = param_1;
    }
    if ((g_status_flags_f0f8 & 8) == 0) {
      uVar1 = DAT_0001f42e;
    }
    else {
      DAT_0001efda = DAT_00010c0e;
      param_1 = DAT_00010c0e;
      if ((g_status_flags_f0f8 & 0x20) == 0) {
        uVar1 = DAT_00010c0c;
      }
      else {
        uVar1 = DAT_00010c0a;
      }
    }
    if (DAT_0001efda == 0) {
      if ((g_status_flags_f3f0 & 0x1800) == 0x1000) {
        if ((g_status_flags_f0f8 & 0x20) == 0) {
          DAT_0001efda = DAT_00010c1e;
        }
        else {
          DAT_0001efda = DAT_00010c1c;
        }
      }
      else {
        DAT_0001efda = DAT_00010c0e;
      }
      param_1 = sat_sub_u16(uVar1,DAT_00010c10);
      uVar1 = param_1;
    }
    if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf3f0) & 0x800) != 0) {
      if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0) {
        uVar2 = *(ushort *)((uint)bVar3 << 0x10 | 0xc18);
      }
      else {
        uVar2 = *(ushort *)((uint)bVar3 << 0x10 | 0xc16);
      }
      if (uVar1 < uVar2) {
        uVar1 = uVar2;
      }
    }
    *(ushort *)((uint)bVar3 << 0x10 | 0xf42e) = uVar1;
  }
  return param_1;
}



ushort isc_f426_f03a_decay_calc(ushort param_1)

{
  byte bVar1;
  
  bVar1 = 1;
  if (DAT_0001f03a == 0) {
    if (DAT_0001f426 != 0) {
      DAT_0001f426 = DAT_0001f426 + -1;
    }
    DAT_0001f03a = DAT_00010d1a;
    param_1 = DAT_00010d1a;
  }
  if (((g_status_flags_f0f8 & 0x20) == 0) && (param_1 = g_status_flags_f20e & 0x11, param_1 == 0)) {
    param_1 = g_status_flags_f0fa & (g_status_flags_f0f8 ^ g_status_flags_f0fa);
    if ((param_1 & 0x20) != 0) {
      DAT_0001f03a = DAT_00010d18;
      (*axis_lookup_interp)();
      param_1 = table_lookup_interp(1,(void *)0x292a);
      *(ushort *)((uint)bVar1 << 0x10 | 0xf426) = param_1;
    }
  }
  else {
    DAT_0001f03a = 0;
    DAT_0001f426 = 0;
  }
  return param_1;
}



// [VERIFIED 2026-07-22] f4de_f4e2_octane_correction_calc: writes F4DE (interpolated
// correction value, two axis_lookup_interp calls, zeroed when F20E bit0/bit4 set)
// and F4E0/F4E2 (a mirrored/shifted copy pair with saturating div/mul against
// F0A/F0C/F0E thresholds), gated on a config flag (0x102eb) and o2_config4_mode16_check.
// Confirms F4DE/F4E2 (already-named MUT table addresses) plus reveals F4E0 as a
// third address in the same write group (not itself a listed MUT ReqID in the
// 0x2fad0 table). Checked as a candidate producer for the separate F4Dx/F4Ex MUT
// ReqID 0x8C-0x93 cluster (mut_verification_status.md) and ruled out -- none of
// F84F/EEFB/F4DF/F4DD/F4E5/F4E7/F4EB/F4DB are touched by this function.

void f4de_f4e2_octane_correction_calc(void)

{
  ushort *puVar1;
  int iVar2;
  undefined2 uVar3;
  ushort uVar4;
  short sVar5;
  ushort uVar6;
  ushort uVar7;
  byte bVar8;
  
  bVar8 = 1;
  if (DAT_000102eb == '\0') {
    DAT_0001f4de = 0;
  }
  else {
    g_axis_lookup_input_scratch_f0ba = DAT_0001f180 >> 4;
    (*axis_lookup_interp)();
    (*axis_lookup_interp)();
    if ((*(ushort *)((uint)bVar8 << 0x10 | 0xf20e) & 0x11) == 0) {
      uVar3 = table_lookup_interp(1,(void *)0x2d74);
      *(undefined2 *)((uint)bVar8 << 0x10 | 0xf4de) = uVar3;
    }
    else {
      *(undefined2 *)((uint)bVar8 << 0x10 | 0xf4de) = 0;
    }
    *(undefined2 *)((uint)bVar8 << 0x10 | 0xf4e2) = *(undefined2 *)((uint)bVar8 << 0x10 | 0xf4e0);
    iVar2 = (uint)*(ushort *)((uint)bVar8 << 0x10 | 0xf0e) *
            (uint)*(ushort *)((uint)bVar8 << 0x10 | 0xf4e2);
    uVar4 = (ushort)((uint)iVar2 >> 0x10);
    uVar7 = (short)iVar2 + *(short *)((uint)bVar8 << 0x10 | 0xf0a);
    uVar6 = 0;
    if (*(short *)((uint)bVar8 << 0x10 | 0xf4e2) != 0) {
      iVar2 = (uint)(*(short *)((uint)bVar8 << 0x10 | 0xf4e2) - 1) *
              (uint)*(ushort *)((uint)bVar8 << 0x10 | 0xf0e);
      uVar4 = (ushort)((uint)iVar2 >> 0x10);
      uVar6 = (short)iVar2 + *(short *)((uint)bVar8 << 0x10 | 0xf0c);
    }
    puVar1 = (ushort *)((uint)bVar8 << 0x10 | 0xf4de);
    if (uVar7 < *puVar1 || uVar7 == *puVar1) {
      uVar6 = 0;
      uVar4 = *(ushort *)((uint)bVar8 << 0x10 | 0xf4de);
      if (*(ushort *)((uint)bVar8 << 0x10 | 0xf0a) <= uVar4) {
        uVar4 = div_u16_sat(*(short *)((uint)bVar8 << 0x10 | 0xf4de) -
                            *(short *)((uint)bVar8 << 0x10 | 0xf0a),
                            *(ushort *)((uint)bVar8 << 0x10 | 0xf0e));
        uVar6 = uVar4 + 1;
      }
    }
    else {
      puVar1 = (ushort *)((uint)bVar8 << 0x10 | 0xf4de);
      if (uVar6 < *puVar1 || uVar6 == *puVar1) {
        uVar6 = *(ushort *)((uint)bVar8 << 0x10 | 0xf4e0);
      }
      else {
        uVar6 = 0;
        uVar4 = *(ushort *)((uint)bVar8 << 0x10 | 0xf4de);
        if (*(ushort *)((uint)bVar8 << 0x10 | 0xf0c) <= uVar4) {
          uVar4 = div_u16_sat(*(short *)((uint)bVar8 << 0x10 | 0xf4de) -
                              *(short *)((uint)bVar8 << 0x10 | 0xf0c),
                              *(ushort *)((uint)bVar8 << 0x10 | 0xf0e));
          uVar6 = uVar4 + 1;
        }
      }
    }
    if (uVar6 < *(ushort *)((uint)bVar8 << 0x10 | 0xf10)) {
      *(ushort *)((uint)bVar8 << 0x10 | 0xf4e0) = uVar6;
    }
    else {
      uVar4 = *(ushort *)((uint)bVar8 << 0x10 | 0xf10);
      *(ushort *)((uint)bVar8 << 0x10 | 0xf4e0) = uVar4;
    }
    sVar5 = o2_config4_mode16_check(uVar4);
    if (sVar5 == 0) {
      return;
    }
  }
  *(undefined2 *)((uint)bVar8 << 0x10 | 0xf4e2) = 0;
  *(undefined2 *)((uint)bVar8 << 0x10 | 0xf4e0) = 0;
  return;
}



void multi_channel_update_max_dispatch(void)

{
  ushort *puVar1;
  ushort extraout_R2;
  ushort extraout_R2_00;
  ushort extraout_R2_01;
  ushort extraout_R2_02;
  ushort extraout_R2_03;
  ushort extraout_R2_04;
  ushort extraout_R2_05;
  ushort extraout_R2_06;
  ushort extraout_R2_07;
  ushort extraout_R2_08;
  ushort extraout_R2_09;
  ushort extraout_R2_10;
  ushort extraout_R2_11;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = g_status_flags_f0fa ^ g_status_flags_f0f8;
  if ((uVar2 & 8) != 0) {
    (*update_max)();
    uVar2 = extraout_R2;
  }
  if ((uVar2 & 0x10) != 0) {
    (*update_max)();
    uVar2 = extraout_R2_00;
  }
  if ((uVar2 & 0x20) != 0) {
    (*update_max)();
    uVar2 = extraout_R2_01;
  }
  if (*(ushort *)((uint)bVar3 << 0x10 | 0xc3c) <= *(ushort *)((uint)bVar3 << 0x10 | 0xf14c)) {
    (*update_max)();
    uVar2 = extraout_R2_02;
  }
  if ((uVar2 & 0x200) != 0) {
    (*update_max)();
    uVar2 = extraout_R2_03;
  }
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x80) == 0) {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xefd6) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xc42);
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xefd8) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xc48);
  }
  else if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf400) >> 2) +
           *(short *)((uint)bVar3 << 0x10 | 0xc44) < *(ushort *)((uint)bVar3 << 0x10 | 0xf17a)) {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xefd6) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xc42);
  }
  if (((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 0x11) != 0) ||
     ((*(ushort *)((uint)bVar3 << 0x10 | 0xf340) & 1) != 0)) {
    (*update_max)();
    uVar2 = extraout_R2_04;
  }
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf3f0) & 0x20) != 0) {
    (*update_max)();
    uVar2 = extraout_R2_05;
  }
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf216) & 0x40) != 0) {
    (*update_max)();
    uVar2 = extraout_R2_06;
  }
  if (*(char *)((uint)bVar3 << 0x10 | 0x2e4) != '\0') {
    if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf490) & 4) != 0) {
      (*update_max)();
      uVar2 = extraout_R2_07;
    }
    if ((((*(char *)((uint)bVar3 << 0x10 | 0x2e4) == '\x01') &&
         ((*(ushort *)((uint)bVar3 << 0x10 | 0xf490) & 3) != 0)) &&
        ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x10) == 0)) &&
       (*(short *)((uint)bVar3 << 0x10 | 0xf048) != 0)) {
      (*update_max)();
      uVar2 = extraout_R2_08;
    }
  }
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf25e) & 0x200) != 0) {
    (*update_max)();
    uVar2 = extraout_R2_09;
  }
  if ((uVar2 & 0x4000) != 0) {
    (*update_max)();
    uVar2 = extraout_R2_10;
  }
  if ((*(char *)((uint)bVar3 << 0x10 | 0x2eb) != '\0') &&
     (*(short *)((uint)bVar3 << 0x10 | 0xf4e0) != *(short *)((uint)bVar3 << 0x10 | 0xf4e2))) {
    if (*(ushort *)((uint)bVar3 << 0x10 | 0xf18) <= *(ushort *)((uint)bVar3 << 0x10 | 0xef6a)) {
      (*update_max)();
      uVar2 = extraout_R2_11;
    }
    if (*(ushort *)((uint)bVar3 << 0x10 | 0xf03c) < *(ushort *)((uint)bVar3 << 0x10 | 0xf14)) {
      *(undefined2 *)((uint)bVar3 << 0x10 | 0xf03c) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf14);
    }
  }
  if (((uVar2 & 1) != 0) &&
     (uVar2 = *(short *)((uint)bVar3 << 0x10 | 0xd3c) * 4,
     puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf03c), *puVar1 <= uVar2 && uVar2 != *puVar1)) {
    *(short *)((uint)bVar3 << 0x10 | 0xf03c) = *(short *)((uint)bVar3 << 0x10 | 0xd3c) << 2;
  }
  if ((*(short *)((uint)bVar3 << 0x10 | 0xf42e) != 0) &&
     ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 8) == 0)) {
    (*update_max)();
  }
  if ((*(char *)((uint)bVar3 << 0x10 | 0x2f7) != '\0') &&
     (*(short *)((uint)bVar3 << 0x10 | 0xf434) != 0)) {
    (*update_max)();
  }
  return;
}



void __stdcall_far update_max(undefined2 _unused_far_pad,ushort value)

{
  if (DAT_0001efd4 <= value) {
    DAT_0001efd4 = value;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void calc_f40e_threshold_select(void)

{
  if (g_tps_load_axis_f10e < DAT_00010d56) {
    DAT_0001f40e = DAT_00010d54;
  }
  else {
    DAT_0001f40e = DAT_00010d52;
  }
  return;
}



ushort idle_stepper_table_lookup_wrapper(void)

{
  ushort uVar1;
  byte bVar2;
  undefined2 in_stack_00000004;
  
  bVar2 = 1;
  (*axis_lookup_interp)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf0ba) = in_stack_00000004;
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x2992);
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f036_f424_ramp_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  if (((g_status_flags_f20e & 1) == 0) && ((isc_stepper_position_eed0 & 0xa0) == 0)) {
    uVar1 = 0;
    if ((DAT_0001f424 != 0) && (DAT_0001f036 == 0)) {
      DAT_0001f424 = DAT_0001f424 + -1;
      uVar1 = table_lookup_interp(1,(void *)0x2944);
      *(ushort *)((uint)bVar2 << 0x10 | 0xf036) = uVar1;
    }
  }
  else {
    uVar1 = table_lookup_interp(1,(void *)0x2938);
    *(ushort *)((uint)bVar2 << 0x10 | 0xf424) = uVar1;
  }
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf594) & 0x80) != 0) {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf424) = 0;
  }
  return uVar1;
}



void isc_f416_f438_dispatch(void)

{
  f3f0_bit6_clear();
  (*isc_f416_f438_correction_calc)();
  (*f3f4_bit7_update_dispatch)();
  isc_f438_correction_calc();
  (*stub_empty_0x26b8b)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void f3f0_bit6_clear(void)

{
  g_status_flags_f3f0 = g_status_flags_f3f0 & 0xffbf;
  return;
}



undefined2 f3f0_bit13_14_15_hysteresis_gate_check(void)

{
  undefined2 uVar1;
  
  if ((g_status_flags_f3f0 & 0x2000) == 0) {
    if (DAT_00010c90 < g_tps_load_axis_f10e) {
      g_status_flags_f3f0 = g_status_flags_f3f0 | 0x2000;
    }
  }
  else if (g_tps_load_axis_f10e <= DAT_00010c92) {
    g_status_flags_f3f0 = g_status_flags_f3f0 & 0xdfff;
  }
  if ((g_status_flags_f3f0 & 0x4000) == 0) {
    if (DAT_00010c94 < g_tps_load_trimmed_f13e) {
      g_status_flags_f3f0 = g_status_flags_f3f0 | 0x4000;
    }
  }
  else if (g_tps_load_trimmed_f13e <= DAT_00010c96) {
    g_status_flags_f3f0 = g_status_flags_f3f0 & 0xbfff;
  }
  if ((g_status_flags_f3f0 & 0x8000) == 0) {
    if (DAT_00010c98 < g_f1e6_hysteresis_gate) {
      g_status_flags_f3f0 = g_status_flags_f3f0 | 0x8000;
    }
  }
  else if (g_f1e6_hysteresis_gate <= DAT_00010c9a) {
    g_status_flags_f3f0 = g_status_flags_f3f0 & 0x7fff;
  }
  if ((((g_status_flags_f3f0 & 0x2000) == 0) && ((g_status_flags_f3f0 & 0x4000) != 0)) &&
     ((g_status_flags_f3f0 & 0x8000) != 0)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



void isc_f416_f438_correction_calc(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 _unused_far_pad;
  ushort uVar1;
  ushort extraout_R2;
  ushort extraout_R2_00;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  _unused_far_pad = (*f0f8_bit_based_constant_select)(param_1,param_2,DAT_0001efc6,param_3);
  uVar1 = o2_downstream_mode20_select(_unused_far_pad,_unused_far_pad);
  if ((extraout_R2 == 0) || (uVar2 = extraout_R2, uVar1 < extraout_R2)) {
    uVar2 = uVar1;
  }
  if (*(short *)((uint)bVar3 << 0x10 | 0xefc6) == 0) {
    uVar1 = sat_sub_u16(*(ushort *)((uint)bVar3 << 0x10 | 0xf416),
                        *(ushort *)((uint)bVar3 << 0x10 | 0xc80));
    *(ushort *)((uint)bVar3 << 0x10 | 0xf416) = uVar1;
    uVar1 = sat_sub_u16(*(ushort *)((uint)bVar3 << 0x10 | 0xf438),
                        *(ushort *)((uint)bVar3 << 0x10 | 0xc80));
    *(ushort *)((uint)bVar3 << 0x10 | 0xf438) = uVar1;
    uVar2 = extraout_R2_00;
  }
  *(ushort *)((uint)bVar3 << 0x10 | 0xefc6) = uVar2;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f0f8_bit_based_constant_select(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f0f8 & 0x80) == 0) {
    if ((g_status_flags_f0f8 & 0x20) == 0) {
      uVar1 = DAT_00010c7a;
    }
    else {
      uVar1 = DAT_00010c78;
    }
  }
  else if ((g_status_flags_f0f8 & 0x20) == 0) {
    uVar1 = DAT_00010c7e;
  }
  else {
    uVar1 = DAT_00010c7c;
  }
  return uVar1;
}



void f3f4_bit7_update_dispatch(void)

{
  ushort *puVar1;
  short sVar2;
  byte bVar3;
  
  bVar3 = 1;
  sVar2 = (*efc8_zero_condition_check)();
  if (sVar2 == 0) {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3f4);
    *puVar1 = *puVar1 & 0xff7f;
  }
  else {
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3f4);
    *puVar1 = *puVar1 | 0x80;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar efc8_zero_condition_check(void)

{
  if ((g_adc_ch7_raw_f13c < DAT_00010d46) || (DAT_00010d48 < inj_pw_output_f186)) {
    DAT_0001efc8 = DAT_00010d4a;
  }
  return DAT_0001efc8 == 0;
}



void isc_f438_correction_calc(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort raw_value;
  byte bVar3;
  void *pvStack_4;
  
  bVar3 = 1;
  if ((((g_status_flags_f0f8 & 0x80) == 0) && ((isc_state_flags_f3f4 & 0x80) == 0)) &&
     (DAT_00010c76 < g_idle_rpm_actual_f178)) {
    uVar2 = g_idle_rpm_actual_f178;
    if (((g_status_flags_f1f2 & 0x2000) == 0) || ((g_purge_enable_cond_f4a2 & 0x80) == 0)) {
      if ((g_status_flags_f3f0 & 0x40) == 0) {
        if ((g_status_flags_f0f8 & 0x20) == 0) {
          if ((g_status_flags_f0f8 & 0x10) == 0) {
            pvStack_4 = (void *)0x2902;
          }
          else {
            pvStack_4 = (void *)0x291a;
          }
        }
        else if ((g_status_flags_f0f8 & 0x10) == 0) {
          pvStack_4 = (void *)0x28f6;
        }
        else {
          pvStack_4 = (void *)0x290e;
        }
        (*axis_lookup_interp)();
        uVar2 = table_lookup_interp(1,pvStack_4);
        raw_value = uVar2;
      }
      else {
        raw_value = DAT_00010ca2;
      }
    }
    else {
      raw_value = DAT_00010e66;
    }
    uVar2 = o2_sensor2_mode20_select(uVar2,raw_value);
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf438);
    if (*puVar1 <= uVar2 && uVar2 != *puVar1) {
      *(ushort *)((uint)bVar3 << 0x10 | 0xf438) = uVar2;
    }
  }
  return;
}



void stub_empty_0x26b8b(void)

{
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isc_f428_condition_calc(void)

{
  if (DAT_000102d7 != '\0') {
    if ((g_status_flags_f0f8 & 0x10) != 0) {
      DAT_0001efd2 = DAT_00010c2e;
    }
    if ((((g_status_flags_f0f8 & 0x10) == 0) && (DAT_0001efd2 != 0)) &&
       (DAT_00010c28 <= g_tps_load_axis_f10e)) {
      DAT_0001f428 = DAT_00010c2a;
      return;
    }
  }
  DAT_0001f428 = 0;
  return;
}



void isc_f422_correction_calc(void)

{
  ushort *puVar1;
  ushort uVar2;
  ushort extraout_R2;
  byte bVar3;
  
  bVar3 = 1;
  if (DAT_000102d8 == '\0') {
    DAT_0001f422 = 0;
    g_status_flags_f3f2 = g_status_flags_f3f2 & 0xff7f;
  }
  else {
    uVar2 = isc_f3f2_bit6_hysteresis_gate_check();
    if (uVar2 == 0) {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3f2);
      *puVar1 = *puVar1 & 0xff7f;
      uVar2 = extraout_R2;
    }
    else {
      puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3f2);
      *puVar1 = *puVar1 | 0x80;
      uVar2 = table_lookup_interp(1,(void *)0x2926);
      *(undefined2 *)((uint)bVar3 << 0x10 | 0xefdc) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xd02);
    }
    if (*(short *)((uint)bVar3 << 0x10 | 0xefdc) == 0) {
      *(undefined2 *)((uint)bVar3 << 0x10 | 0xefdc) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xd02);
      uVar2 = sat_sub_u16(uVar2,*(ushort *)((uint)bVar3 << 0x10 | 0xd04));
    }
    *(ushort *)((uint)bVar3 << 0x10 | 0xf422) = uVar2;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f3f2_bit6_hysteresis_gate_check(void)

{
  ushort uVar1;
  ushort uVar2;
  
  if ((g_status_flags_f0f8 & 0x10) == 0) {
    uVar1 = DAT_00010cfa;
    uVar2 = DAT_00010cf8;
  }
  else {
    uVar1 = DAT_00010cfe;
    uVar2 = DAT_00010cfc;
  }
  if ((g_status_flags_f3f2 & 0x40) == 0) {
    if (uVar2 < DAT_0001f188) {
      g_status_flags_f3f2 = g_status_flags_f3f2 | 0x40;
    }
  }
  else if (DAT_0001f188 <= uVar1) {
    g_status_flags_f3f2 = g_status_flags_f3f2 & 0xffbf;
  }
  if (((((g_status_flags_f0f8 & 0x80) == 0) ||
       (((DAT_000102d8 != '\x02' && ((g_status_flags_f0f8 & 0x20) == 0)) ||
        ((g_status_flags_f20e & 8) == 0)))) ||
      (((g_tps_load_axis_f10e <= DAT_00010d00 || ((g_status_flags_f3f0 & 0x20) != 0)) ||
       ((g_status_flags_f0f8 & 4) == 0)))) || ((g_status_flags_f3f2 & 0x40) != 0)) {
    return 0;
  }
  return 1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isc_f084_f3f0_bit8_update(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  
  if (DAT_000102d9 != '\0') {
    if (cal_fuel_cut_enable_flag == '\0') {
      uVar1 = coolant_temp_scaled_f130;
      uVar2 = DAT_00010d0c;
      uVar3 = DAT_00010d0a;
    }
    else {
      uVar1 = DAT_0001f436;
      uVar2 = DAT_00010d08;
      uVar3 = DAT_00010d06;
    }
    if (uVar2 < uVar1) {
      DAT_0001f084 = DAT_00010d0e;
    }
    else if (DAT_0001f084 == 0) {
      g_status_flags_f3f0 = g_status_flags_f3f0 | 0x100;
    }
    if (uVar1 < uVar3) {
      return;
    }
  }
  g_status_flags_f3f0 = g_status_flags_f3f0 & 0xfeff;
  return;
}



void isc_f42a_table_lookup(void)

{
  undefined2 uVar1;
  byte bVar2;
  void *table;
  
  bVar2 = 1;
  if (cal_fuel_cut_enable_flag == '\0') {
    (*axis_lookup_interp)();
    if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf0f8) & 0x20) == 0) {
      table = (void *)0x2954;
    }
    else {
      table = (void *)0x2954;
    }
  }
  else {
    (*axis_lookup_interp)();
    if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf0f8) & 0x20) == 0) {
      table = (void *)0x2950;
    }
    else {
      table = (void *)0x2950;
    }
  }
  uVar1 = table_lookup_interp(1,table);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf42a) = uVar1;
  return;
}



void isc_f42c_correction_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = isc_f3f0_bit2_hysteresis_gate_check();
  if (uVar1 == 0) {
    if (*(short *)((uint)bVar2 << 0x10 | 0xefde) != 0) {
      return;
    }
    uVar1 = sat_sub_u16(*(ushort *)((uint)bVar2 << 0x10 | 0xf42c),
                        *(ushort *)((uint)bVar2 << 0x10 | 0xcb4));
    *(ushort *)((uint)bVar2 << 0x10 | 0xf42c) = uVar1;
  }
  else {
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf42c) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xcae);
  }
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xefde) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xcb6);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f3f0_bit2_hysteresis_gate_check(void)

{
  if (((g_status_flags_f0f8 & (g_status_flags_f0f8 ^ g_status_flags_f0fa) & 0x10) != 0) ||
     ((g_status_flags_f0fa & (g_status_flags_f0f8 ^ g_status_flags_f0fa) & 0x20) != 0)) {
    DAT_0001efcc = DAT_00010cb0;
  }
  if ((DAT_0001efcc == 0) && (DAT_00010cac < g_tps_load_axis_f10e)) {
    if ((g_idle_rpm_actual_f178 < DAT_00010caa) && ((g_status_flags_f20e & 0x11) == 0)) {
      if (DAT_00010cb2 <= DAT_0001ef70) {
        if ((g_status_flags_f3f0 & 4) == 0) {
          DAT_0001efce = 0;
        }
        g_status_flags_f3f0 = g_status_flags_f3f0 | 4;
        goto LAB_00026df0;
      }
    }
  }
  g_status_flags_f3f0 = g_status_flags_f3f0 & 0xfffb;
LAB_00026df0:
  return g_status_flags_f3f0 & 4;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f490_bits01_or_default_select(void)

{
  ushort uVar1;
  
  uVar1 = egr_valve_state_flags_f490 & 3;
  if ((egr_valve_state_flags_f490 & 3) == 0) {
    DAT_0001f048 = DAT_00010c26;
    uVar1 = DAT_00010c26;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isc_f3f0_bit3_and_f434_update(void)

{
  ushort uVar1;
  
  uVar1 = (DAT_0001f180 >> 3) + 1 >> 1;
  if (g_throttle_body_pos_target_f170 <= uVar1) {
    if (DAT_00010d22 <= uVar1 - g_throttle_body_pos_target_f170) {
      g_status_flags_f3f0 = g_status_flags_f3f0 | 8;
      goto LAB_00026e38;
    }
  }
  g_status_flags_f3f0 = g_status_flags_f3f0 & 0xfff7;
LAB_00026e38:
  if (((g_status_flags_f0f8 & 0x20) == 0) && ((g_status_flags_f0f8 & 0x80) != 0)) {
    if ((g_status_flags_f3f0 & 8) != 0) {
      DAT_0001efe0 = DAT_00010d1c;
    }
  }
  else {
    DAT_0001efe0 = 0;
  }
  if ((DAT_000102f7 == '\0') || (DAT_0001efe0 == 0)) {
    DAT_0001f434 = 0;
  }
  else {
    DAT_0001f434 = DAT_00010d1e;
  }
  return;
}



void stub_empty_0x26e76(void)

{
  return;
}



void isc_stepper_state_dispatch(void)

{
  ushort *puVar1;
  ushort uVar2;
  short sVar3;
  byte bVar4;
  
  bVar4 = 1;
  uVar2 = isc_stepper_position_reached_check();
  if (uVar2 != 0) {
    (*isc_stepper_state_reinit_and_target_select)();
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xeed0) & 0x20) != 0) {
    isc_stepper_output_state_machine_eed4();
  }
  if ((((*(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) & 0x40) != 0) &&
      (*(short *)((uint)bVar4 << 0x10 | 0xef90) != 0)) ||
     ((*(ushort *)((uint)bVar4 << 0x10 | 0xeed0) & 0x20) != 0)) {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xeed0);
    *puVar1 = *puVar1 & 0xffbf;
  }
  if (((*(ushort *)((uint)bVar4 << 0x10 | 0xeed0) & 0x20) == 0) &&
     (((*(ushort *)((uint)bVar4 << 0x10 | 0xeed0) & 0x80) != 0 ||
      ((*(short *)((uint)bVar4 << 0x10 | 0xef90) == 0 &&
       ((*(ushort *)((uint)bVar4 << 0x10 | 0xeed0) & 0x40) == 0)))))) {
    (*isc_stepper_mode_state_reload_b)();
  }
  uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xeed0) & 0xa0;
  if ((uVar2 == 0) && (*(short *)((uint)bVar4 << 0x10 | 0xef90) == 0)) {
    uVar2 = (*isc_stepper_mode_state_reload_c)(0);
  }
  else {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xeed0);
    *puVar1 = *puVar1 & 0xffef;
  }
  if ((((*(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 2) == 0) ||
      (uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf20e) & 0x11, uVar2 != 0)) ||
     (uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xeed0) & 0xb0, uVar2 != 0)) {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xeed0);
    *puVar1 = *puVar1 & 0xfff7;
  }
  else {
    uVar2 = (*isc_stepper_mode_state_reload_d)(0);
  }
  sVar3 = (*isc_efd0_gate_condition_check)(uVar2);
  if (sVar3 == 0) {
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xeed0);
    *puVar1 = *puVar1 & 0xfffb;
  }
  else {
    isc_f408_f406_stepper_target_calc();
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_stepper_position_reached_check(void)

{
  ushort *puVar1;
  ushort uVar2;
  short sVar3;
  ushort uVar4;
  short extraout_R2;
  byte bVar5;
  
  bVar5 = 1;
  sVar3 = (*swap_invert_high_byte)();
  if ((((extraout_R2 == sVar3) &&
       (uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xc70),
       *(ushort *)((uint)bVar5 << 0x10 | 0xeeca) <= *(ushort *)((uint)bVar5 << 0x10 | 0xc70))) &&
      (*(ushort *)((uint)bVar5 << 0x10 | 0xeecc) <= uVar4)) &&
     (((*(ushort *)((uint)bVar5 << 0x10 | 0xeece) <= uVar4 &&
       (uVar4 = *(ushort *)((uint)bVar5 << 0x10 | 0xc72),
       uVar2 = *(ushort *)((uint)bVar5 << 0x10 | 0xc72),
       puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xeeca), uVar2 < *puVar1 || uVar2 == *puVar1)) &&
      ((puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xeecc), uVar4 < *puVar1 || uVar4 == *puVar1 &&
       (puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xeece), uVar4 < *puVar1 || uVar4 == *puVar1))))))
  {
    uVar4 = 0;
  }
  else {
    uVar4 = 1;
  }
  return uVar4;
}



void isc_stepper_state_reinit_and_target_select(void)

{
  isc_stepper_position_eed0 = 0x20;
  DAT_0001eece = DAT_00010c74;
  DAT_0001eecc = DAT_00010c74;
  DAT_0001eeca = DAT_00010c74;
  (*f3fa_f3fe_target_select_by_f0f8_bit5)();
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// isc_stepper_output_state_machine_eed4: closed-loop idle stepper state machine, gated
// by 0xEED0 status bits, writes stepper position @0xEED4/EED5. MUT RequestID 0x16 ->
// table entry 0xEED5 (low byte of the word written here) = ISCSteps. VERIFIED -
// logging.txt CONFIRMED-section claim holds under current decoder.

void isc_stepper_output_state_machine_eed4(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  undefined2 uVar4;
  undefined2 extraout_R1;
  undefined2 extraout_R1_00;
  ushort uVar5;
  byte bVar6;
  ushort in_SR;
  byte in_CF;
  byte bVar7;
  
  uVar3 = isc_stepper_position_eed0 & 3;
  bVar7 = uVar3 != 0;
  if (!(bool)bVar7) {
    in_SR = (in_SR & 0xfffe | (ushort)in_CF) & 0xfff1;
    bVar6 = 0;
    uVar3 = (*swap_invert_high_byte)();
    *(ushort *)((uint)bVar6 << 0x10 | 0xeed4) = uVar3;
    uVar2 = isc_stepper_position_eed0;
    isc_stepper_position_eed0 = uVar2 | 2;
    param_2 = extraout_R1;
  }
  if ((*(ushort *)((uint)bVar7 << 0x10 | 0xeed0) & 2) != 0) {
    if ((*(ushort *)((uint)bVar7 << 0x10 | 0xeed4) & 0xff) == 0) {
      puVar1 = (ushort *)((uint)bVar7 << 0x10 | 0xeed0);
      *puVar1 = *puVar1 & 0xfffd;
      puVar1 = (ushort *)((uint)bVar7 << 0x10 | 0xeed0);
      *puVar1 = *puVar1 | 1;
    }
    else {
      *(undefined2 *)((uint)bVar7 << 0x10 | 0xf406) = 0;
      uVar3 = f3f0_target_state_reset_and_reload(param_3);
      param_2 = extraout_R1_00;
    }
  }
  if ((*(ushort *)((uint)bVar7 << 0x10 | 0xeed0) & 1) != 0) {
    uVar5 = *(ushort *)((uint)bVar7 << 0x10 | 0xeed4) & 0xff;
    puVar1 = (ushort *)((uint)bVar7 << 0x10 | 0xcbe);
    uVar2 = *puVar1;
    if (uVar5 == uVar2) {
      bVar7 = 0;
      uVar4 = (*swap_invert_high_byte)
                        (uVar3,param_2,uVar5,0,
                         in_SR & 0xfff0 | (ushort)(uVar5 < *puVar1) |
                         (ushort)((short)*puVar1 < (short)uVar5 == (short)(uVar5 - uVar2) < 0) << 1
                         | (ushort)(uVar5 == uVar2) << 2 | (ushort)((short)(uVar5 - uVar2) < 0) << 3
                        );
      *(undefined2 *)((uint)bVar7 << 0x10 | 0xeed4) = uVar4;
      uVar3 = isc_stepper_position_eed0;
      isc_stepper_position_eed0 = uVar3 & 0xffdc;
    }
    else {
      *(undefined2 *)((uint)bVar7 << 0x10 | 0xf406) = *(undefined2 *)((uint)bVar7 << 0x10 | 0xcbe);
      f3f0_target_state_reset_and_reload(param_3);
    }
  }
  return;
}



void f3f0_target_state_reset_and_reload(ushort param_fp1)

{
  ushort *puVar1;
  byte bVar2;
  
  bVar2 = 1;
  (*update_max)();
  puVar1 = (ushort *)((uint)bVar2 << 0x10 | 0xf3f4);
  *puVar1 = *puVar1 | 1;
  *(ushort *)((uint)bVar2 << 0x10 | 0xf3f0) = *(ushort *)((uint)bVar2 << 0x10 | 0xf3f0) & 0xff4a;
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf416) = 0;
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf438) = 0;
  (*f3fa_f3fe_target_select_by_f0f8_bit5)();
  return;
}



void isc_stepper_mode_state_reload_b(void)

{
  ushort *puVar1;
  undefined2 in_ram_00000cba;
  undefined2 uVar2;
  byte bVar3;
  ushort unaff_retaddr;
  
  bVar3 = 1;
  isc_stepper_position_eed0 = isc_stepper_position_eed0 | 0x80;
  if ((DAT_0001eed4 & 0xff) == DAT_00010cb8) {
    bVar3 = 0;
    DAT_0000f40a = in_ram_00000cba;
    uVar2 = DAT_0000f40a;
    uVar2 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xeed4) = uVar2;
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xeed0);
    *puVar1 = *puVar1 & 0xff7f;
    puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xeed0);
    *puVar1 = *puVar1 | 0x40;
  }
  else {
    f3f0_target_state_reset_and_reload(unaff_retaddr);
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf406) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xcb8);
  }
  return;
}



void isc_stepper_mode_state_reload_c(void)

{
  byte bVar1;
  ushort unaff_retaddr;
  
  bVar1 = 1;
  isc_stepper_position_eed0 = isc_stepper_position_eed0 & 0xff50 | 0x10;
  f3f0_target_state_reset_and_reload(unaff_retaddr);
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf406) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xcbc);
  return;
}



void isc_stepper_mode_state_reload_d(void)

{
  ushort uVar1;
  byte bVar2;
  ushort unaff_retaddr;
  
  bVar2 = 1;
  isc_stepper_position_eed0 = isc_stepper_position_eed0 & 0xff48 | 8;
  f3f0_target_state_reset_and_reload(unaff_retaddr);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf408) = *(undefined2 *)((uint)bVar2 << 0x10 | 0xd42);
  uVar1 = idle_stepper_table_lookup_wrapper();
  *(ushort *)((uint)bVar2 << 0x10 | 0xf406) = uVar1;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar isc_efd0_gate_condition_check(void)

{
  if (((((g_status_flags_f20e & 0x11) != 0) || ((isc_stepper_position_eed0 & 0xb8) != 0)) ||
      ((g_status_flags_f0f8 & 0x24) != 0x20)) || ((g_status_flags_f0f8 & 0x1800) != 0x1800)) {
    DAT_0001efd0 = DAT_00010d44;
  }
  return DAT_0001efd0 == 0;
}



void isc_f408_f406_stepper_target_calc(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  undefined2 uVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  isc_stepper_position_eed0 = isc_stepper_position_eed0 | 4;
  f3f0_target_state_reset_and_reload(param_3);
  table_lookup_indexed(2,(void *)0xd92c);
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf408) = uVar1;
  uVar2 = idle_stepper_table_lookup_wrapper();
  *(ushort *)((uint)bVar3 << 0x10 | 0xf406) = uVar2;
  return;
}



void isc_stepper_target_dispatch(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if ((isc_stepper_position_eed0 & 0xbc) == 0) {
    f3f0_bit0_update();
    uVar2 = f3f4_bit0_update_and_check();
    if (uVar2 == 0) {
      if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf20e) & 0x11) == 0) {
        uVar2 = isc_efd4_gate_condition_check();
        if (uVar2 == 0) {
          puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3f0);
          *puVar1 = *puVar1 & 0xffef;
          *(undefined2 *)((uint)bVar3 << 0x10 | 0xefca) =
               *(undefined2 *)((uint)bVar3 << 0x10 | 0xc6c);
          (*isc_f408_composite_dispatch)();
        }
        else {
          puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf3f0);
          *puVar1 = *puVar1 | 0x10;
          (*isc_stepper_correction_dispatch_b)();
        }
      }
      else {
        isc_f408_from_f402_dispatch();
      }
    }
    else {
      (*isc_stepper_trim_dispatch)();
    }
    uVar2 = idle_stepper_table_lookup_wrapper();
    *(ushort *)((uint)bVar3 << 0x10 | 0xf406) = uVar2;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f3f0_bit0_update(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f20e & 0x11) == 0) {
    g_status_flags_f3f0 = g_status_flags_f3f0 & 0xfffe;
    uVar1 = 0;
  }
  else {
    uVar1 = g_status_flags_f3f0 & 0xff4b;
    g_status_flags_f3f0 = uVar1 | 1;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f3f4_bit0_update_and_check(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f20e & 0x11) == 0) {
    isc_state_flags_f3f4 = isc_state_flags_f3f4 | 1;
  }
  else if ((DAT_0001eed4 & 0xff) == DAT_0001f40a) {
    isc_state_flags_f3f4 = isc_state_flags_f3f4 & 0xfffe;
  }
  if (((g_status_flags_f20e & 1) == 0) || ((isc_state_flags_f3f4 & 1) != 0)) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



void isc_stepper_trim_dispatch(void)

{
  (*isc_f402_stepper_target_calc)();
  (*f414_trim_calc_gated_by_f0f8_f3f0)();
  isc_f408_correction_calc();
  return;
}



// [NEW 2026-07-15] isc_f402_stepper_target_calc
// 
// ACTUATOR function, not itself a MUT RequestID target (F402 not present
// in the 0x2fad0 MUT table), but directly upstream of two that are:
// computes the core ISC stepper target (lookup table @0x28ea via pjsr
// @0x14656, plus F42A + F43A correction terms, saturated via pjsr
// @0x1442d), stores to F402. F402 then feeds isc_eeca_eecc_eece_correction_calc
// (0x27a81, resolves MUT ReqID 0x08/0x0A) and isc_f3fa_f3fc_f3fe_correction_calc
// (0x27990, same mode-shadow idiom, targets not currently MUT IDs) - see
// those functions' plate comments.
// 
// F402 = ISC stepper motor position/target, actuator output.

void isc_f402_stepper_target_calc(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  table_lookup_interp(1,(void *)0x28ea);
  uVar1 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf402) = uVar1;
  return;
}



void f414_trim_calc_gated_by_f0f8_f3f0(void)

{
  short sVar1;
  undefined2 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  if (((g_status_flags_f0f8 & 0x10) == 0) && ((g_status_flags_f3f0 & 1) == 0)) {
    sVar1 = table_lookup_interp(1,(void *)0x297a);
    sat_sub_u16(*(short *)((uint)bVar3 << 0x10 | 0xf402) + *(short *)((uint)bVar3 << 0x10 | 0xf412),
                sVar1 + 0x80);
    uVar2 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf414) = uVar2;
  }
  else {
    DAT_0001f414 = 0;
  }
  return;
}



void isc_f408_correction_calc(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort b;
  ushort extraout_R2;
  ushort extraout_R2_00;
  ushort unaff_FP;
  byte bVar3;
  bool bVar4;
  void *table;
  
  bVar3 = 1;
  if (DAT_0001f414 < DAT_0001f40c) {
    uVar1 = DAT_0001f40c;
  }
  else {
    uVar1 = DAT_0001f414;
  }
  if (DAT_0001efce == 0) {
    bVar4 = g_idle_rpm_actual_f178 < g_idle_target_rpm_f400;
    if (bVar4) {
      g_axis_lookup_input_scratch_f0ba = g_idle_target_rpm_f400 - g_idle_rpm_actual_f178;
    }
    else {
      g_axis_lookup_input_scratch_f0ba = g_idle_rpm_actual_f178 - g_idle_target_rpm_f400;
    }
    uVar1 = f0f8_bit5_constant_select();
    uVar2 = mul_u16_shr8_sat(*(ushort *)((uint)bVar3 << 0x10 | 0xf400),uVar1);
    uVar1 = extraout_R2;
    if (uVar2 < *(ushort *)((uint)bVar3 << 0x10 | 0xf0ba)) {
      *(undefined2 *)((uint)bVar3 << 0x10 | 0xefce) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xc5e);
      (*axis_lookup_interp)();
      if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x20) == 0) {
        table = (void *)0x296c;
      }
      else {
        table = (void *)0x295e;
      }
      b = table_lookup_interp(1,table);
      uVar2 = *(ushort *)((uint)bVar3 << 0x10 | 0xf40a);
      uVar1 = extraout_R2_00;
      if (bVar4) {
        if (*(ushort *)((uint)bVar3 << 0x10 | 0xf40e) <= uVar2) goto LAB_00027395;
      }
      else {
        if (uVar2 == 0) goto LAB_00027395;
        sat_sub_u16(*(ushort *)((uint)bVar3 << 0x10 | 0xf40c),b);
      }
      uVar1 = isc_stepper_target_clamp_calc(unaff_FP);
    }
  }
LAB_00027395:
  *(ushort *)((uint)bVar3 << 0x10 | 0xf408) = uVar1;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f0f8_bit5_constant_select(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f0f8 & 0x20) == 0) {
    uVar1 = DAT_00010c54;
  }
  else {
    uVar1 = DAT_00010c52;
  }
  return uVar1;
}



ushort isc_stepper_target_clamp_calc(ushort param_fp1)

{
  ushort lo;
  ushort value;
  byte bVar1;
  ushort in_stack_00000004;
  
  bVar1 = 1;
  if ((g_status_flags_f3f0 & 1) == 0) {
    if (DAT_0001f414 < in_stack_00000004) {
    }
    else {
      in_stack_00000004 = DAT_0001f414;
    }
  }
  else {
    sat_sub_u16(DAT_0001f402 + DAT_00010c46,0x80);
    lo = sat_sub_u16(*(short *)((uint)bVar1 << 0x10 | 0xf402) +
                     *(short *)((uint)bVar1 << 0x10 | 0xc64),0x80);
    in_stack_00000004 = clamp_u16(value,lo,lo);
  }
  return in_stack_00000004;
}



void isc_f408_from_f402_dispatch(void)

{
  byte bVar1;
  
  bVar1 = 1;
  (*isc_f402_stepper_target_calc)();
  (*f414_trim_calc_gated_by_f0f8_f3f0)();
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf408) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xf402);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_efd4_gate_condition_check(void)

{
  ushort uVar1;
  
  if (((((g_status_flags_f0f8 & 0x80) == 0) || ((g_status_flags_f0f8 & 4) != 0)) ||
      (DAT_0001efd4 != 0)) ||
     ((((DAT_0001f422 != 0 || (DAT_0001f426 != 0)) || ((DAT_0001efd6 != 0 && (DAT_0001efd8 != 0))))
      || (g_idle_rpm_actual_f178 <= DAT_00010c4a)))) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



void isc_stepper_correction_dispatch_b(void)

{
  (*isc_f402_composite_correction_calc)();
  (*f414_trim_calc_gated_by_f0f8_f3f0)();
  isc_f408_correction_calc();
  (*isc_f3fa_f3fe_dispatch)();
  isc_eeca_update_dispatch();
  return;
}



void isc_f402_composite_correction_calc(void)

{
  ushort *puVar1;
  ushort uVar2;
  undefined2 uVar3;
  undefined2 extraout_R1;
  ushort extraout_R2;
  ushort uVar4;
  ushort extraout_R2_00;
  short extraout_R2_01;
  byte bVar5;
  
  bVar5 = 1;
  (*isc_f41c_table_lookup)();
  isc_f41e_table_lookup();
  puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf3f4);
  *puVar1 = *puVar1 & 0xfffd;
  (*isc_f420_correction_calc)();
  uVar2 = f216_bit6_constant_select();
  uVar4 = extraout_R2;
  if (extraout_R2 < uVar2) {
    uVar4 = uVar2;
  }
  uVar4 = uVar4 + *(short *)((uint)bVar5 << 0x10 | 0xf42a) +
          *(short *)((uint)bVar5 << 0x10 | 0xf424);
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf0f8) & 0x80) == 0) {
    uVar2 = sat_sub_u16(*(ushort *)((uint)bVar5 << 0x10 | 0xf438),
                        *(ushort *)((uint)bVar5 << 0x10 | 0xc82));
    uVar4 = extraout_R2_00;
    if (uVar2 < *(ushort *)((uint)bVar5 << 0x10 | 0xf416)) goto LAB_00027505;
LAB_00027500:
    *(ushort *)((uint)bVar5 << 0x10 | 0xf416) = uVar2;
  }
  else {
    if (*(short *)((uint)bVar5 << 0x10 | 0xefe2) == 0) {
      if (*(ushort *)((uint)bVar5 << 0x10 | 0xf438) < *(ushort *)((uint)bVar5 << 0x10 | 0xf416))
      goto LAB_00027505;
    }
    else {
      uVar2 = uVar4 + *(short *)((uint)bVar5 << 0x10 | 0xc84);
      if (*(ushort *)((uint)bVar5 << 0x10 | 0xf438) <= uVar2) goto LAB_00027500;
    }
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf416) = *(undefined2 *)((uint)bVar5 << 0x10 | 0xf438);
  }
LAB_00027505:
  if (uVar4 < *(ushort *)((uint)bVar5 << 0x10 | 0xf416)) {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf3f0);
    *puVar1 = *puVar1 | 0x20;
  }
  else {
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf3f0);
    *puVar1 = *puVar1 & 0xffdf;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf416) = 0;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf438) = 0;
  }
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf25e) & 0x200) != 0) {
    table_lookup_interp(1,(void *)0x2ebc);
  }
  isc_f42e_or_default_select();
  isc_f038_constant_select();
  f0f8_bit9_constant_or_calc_select();
  f490_bits01_constant_select();
  uVar4 = f3f0_bit8_constant_select();
  (*f430_clear)(uVar4,extraout_R1,extraout_R2_01 + uVar4);
  f430_clear();
  f0f8_masked_constant_select();
  uVar3 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf402) = uVar3;
  return;
}



ushort isc_f41c_table_lookup(void)

{
  short sVar1;
  byte bVar2;
  void *table;
  
  bVar2 = 1;
  if ((g_status_flags_f0f8 & 0x20) == 0) {
    table = (void *)0xd94c;
  }
  else {
    table = (void *)0xd92c;
  }
  sVar1 = table_lookup_indexed(2,table);
  *(short *)((uint)bVar2 << 0x10 | 0xf41c) = sVar1 + *(short *)((uint)bVar2 << 0x10 | 0xf43a);
  return *(ushort *)((uint)bVar2 << 0x10 | 0xf41c);
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void isc_f41e_table_lookup(void)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  void *table;
  
  bVar4 = 1;
  if ((g_status_flags_f0f8 & 0x10) == 0) {
    uVar3 = 0;
  }
  else {
    if ((g_status_flags_f0f8 & 1) == 0) {
      if ((g_status_flags_f0f8 & 0x20) == 0) {
        table = (void *)0x28d2;
      }
      else {
        table = (void *)0x28ba;
      }
    }
    else if ((g_status_flags_f0f8 & 0x20) == 0) {
      table = (void *)0x28de;
    }
    else {
      table = (void *)0x28c6;
    }
    sVar1 = table_lookup_interp(1,table);
    uVar3 = sVar1 + *(short *)((uint)bVar4 << 0x10 | 0xf43a);
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) & 0x4024) == 0x4020) {
    sVar1 = *(short *)((uint)bVar4 << 0x10 | 0xd28);
    if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) & 0x10) != 0) {
      sVar1 = *(short *)((uint)bVar4 << 0x10 | 0xd26);
    }
    uVar2 = sVar1 + *(short *)((uint)bVar4 << 0x10 | 0xf43a);
    if (uVar3 < uVar2) {
      uVar3 = uVar2;
    }
  }
  *(ushort *)((uint)bVar4 << 0x10 | 0xf41e) = uVar3;
  return;
}



undefined2 f3f4_bit2_3_4_hysteresis_gate_check(void)

{
  undefined2 uVar1;
  
  if ((isc_state_flags_f3f4 & 0x10) == 0) {
    if (DAT_00010cd4 < g_f1e6_hysteresis_gate) {
      isc_state_flags_f3f4 = isc_state_flags_f3f4 | 0x10;
    }
  }
  else if (g_f1e6_hysteresis_gate <= DAT_00010cd6) {
    isc_state_flags_f3f4 = isc_state_flags_f3f4 & 0xffef;
  }
  if ((isc_state_flags_f3f4 & 8) == 0) {
    if (DAT_00010cd8 < engine_torque_pct_f17a) {
      isc_state_flags_f3f4 = isc_state_flags_f3f4 | 8;
    }
  }
  else if (engine_torque_pct_f17a <= DAT_00010cda) {
    isc_state_flags_f3f4 = isc_state_flags_f3f4 & 0xfff7;
  }
  if ((isc_state_flags_f3f4 & 4) == 0) {
    if (DAT_00010cde < g_tps_load_axis_f10e) {
      isc_state_flags_f3f4 = isc_state_flags_f3f4 | 4;
    }
  }
  else if (g_tps_load_axis_f10e <= DAT_00010cdc) {
    isc_state_flags_f3f4 = isc_state_flags_f3f4 & 0xfffb;
  }
  if (((isc_state_flags_f3f4 & 0x1c) == 0x1c) && ((g_status_flags_f0f8 & 0xa0) == 0x80)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



undefined2 f0f8_gated_constant_select_f3f4_neighbor(void)

{
  undefined2 uVar1;
  
  if ((g_status_flags_f0f8 & 0x10) == 0) {
    uVar1 = DAT_00010ce0;
  }
  else if ((g_status_flags_f0f8 & 1) == 0) {
    uVar1 = DAT_00010ce2;
  }
  else {
    uVar1 = DAT_00010ce4;
  }
  return uVar1;
}



ushort isc_f420_correction_calc(void)

{
  undefined2 uVar1;
  undefined2 in_R2;
  byte bVar2;
  
  bVar2 = 1;
  if (DAT_0001f034 == 0) {
    DAT_0001f420 = 0;
  }
  else {
    (*muldiv_s16_rounded)();
    if ((*(char *)((uint)bVar2 << 0x10 | 0x2c7) == '\x02') &&
       ((*(ushort *)((uint)bVar2 << 0x10 | 0xf0f8) & 1) != 0)) {
      bVar2 = 0;
    }
    else {
      bVar2 = 0;
      table_read_indexed(0xd9,(void *)(CONCAT22(0xd92c,in_R2) >> 8));
    }
    uVar1 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf420) = uVar1;
  }
  return *(ushort *)((uint)bVar2 << 0x10 | 0xf420);
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f216_bit6_constant_select(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f216 & 0x40) == 0) {
    uVar1 = 0;
  }
  else if ((g_status_flags_f0f8 & 0x10) == 0) {
    uVar1 = DAT_00010ce6;
  }
  else {
    uVar1 = DAT_00010cea;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f42e_or_default_select(void)

{
  ushort uVar1;
  
  if (DAT_000102da == '\0') {
    if ((g_status_flags_f0f8 & 8) == 0) {
      uVar1 = 0;
    }
    else {
      uVar1 = DAT_00010c08;
    }
  }
  else {
    uVar1 = DAT_0001f42e;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f038_constant_select(void)

{
  if (DAT_000102db != '\0') {
    if ((g_status_flags_f0f8 & 0x80) == 0) {
      DAT_0001f038 = 0;
    }
    else {
      if (((g_status_flags_f20e & 4) != 0) && (DAT_0001f29e != 0)) {
        DAT_0001f038 = DAT_00010cf6;
      }
      if (DAT_0001f038 != 0) {
        if ((g_status_flags_f0f8 & 0x10) == 0) {
          return DAT_00010cf2;
        }
        return DAT_00010cf4;
      }
    }
  }
  return 0;
}



undefined2
f0f8_bit9_constant_or_calc_select(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  
  if (DAT_000102eb == '\0') {
    if ((g_status_flags_f0f8 & 0x200) == 0) {
      uVar1 = 0;
    }
    else {
      uVar1 = DAT_00010c20;
    }
  }
  else {
    uVar1 = (undefined2)((uint)DAT_0001f4e0 * (uint)DAT_00010f12);
    uVar1 = (*clamp_u8)((short)((uint)DAT_0001f4e0 * (uint)DAT_00010f12 >> 0x10),uVar1,0,uVar1,
                        param_3);
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f490_bits01_constant_select(void)

{
  if (DAT_000102e4 == '\x01') {
    if (((egr_valve_state_flags_f490 & 3) == 0) || ((g_status_flags_f0f8 & 0x10) != 0)) {
      return 0;
    }
    if (DAT_0001f048 != 0) {
      return DAT_00010c24;
    }
  }
  else {
    if (DAT_000102e4 != '\x02') {
      return 0;
    }
    if (radfan_pwm_duty_target_f494 == 0) {
      return 0;
    }
    if ((g_status_flags_f0f8 & 0x10) != 0) {
      return 0;
    }
  }
  return DAT_00010c22;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f3f0_bit8_constant_select(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f3f0 & 0x100) == 0) {
    uVar1 = 0;
  }
  else if ((g_status_flags_f0f8 & 0x20) == 0) {
    uVar1 = DAT_00010d12;
  }
  else {
    uVar1 = DAT_00010d10;
  }
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f0f8_masked_constant_select(void)

{
  ushort uVar1;
  
  if ((DAT_000102c6 == '\0') || ((g_status_flags_f0f8 & 0x414) != 0x404)) {
    uVar1 = 0;
  }
  else {
    uVar1 = DAT_00010d2a;
  }
  return uVar1;
}



short isc_f416_f3f0_bit5_gate_check(ushort param_fp1)

{
  ushort uVar1;
  ushort in_stack_00000004;
  
  if (g_isc_correction_f416 != 0) {
    if ((g_status_flags_f3f0 & 0x40) == 0) {
      if ((g_status_flags_f0f8 & 0x10) == 0) {
        uVar1 = DAT_00010c9c;
      }
      else {
        uVar1 = DAT_00010c9e;
      }
    }
    else {
      uVar1 = DAT_00010ca0;
    }
    if (in_stack_00000004 < uVar1) {
      g_status_flags_f3f0 = g_status_flags_f3f0 | 0x20;
      return g_isc_correction_f416;
    }
  }
  g_status_flags_f3f0 = g_status_flags_f3f0 & 0xffdf;
  return 0;
}



void isc_f3fa_f3fe_dispatch(void)

{
  ushort *puVar1;
  ushort uVar2;
  short sVar3;
  byte bVar4;
  
  bVar4 = 1;
  sVar3 = (*isc_f406_ramp_gate_condition_check)();
  if (sVar3 != 0) {
    isc_f3fa_f3fc_f3fe_correction_calc();
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xc60) <= *(ushort *)((uint)bVar4 << 0x10 | 0xf178)) &&
     (uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf1e6),
     puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xc62), *puVar1 <= uVar2 && uVar2 != *puVar1)) {
    (*f3fa_f3fe_target_select_by_f0f8_bit5)();
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f406_ramp_gate_condition_check(void)

{
  if (((((g_status_flags_f3f0 & 0x10) == 0) || ((g_status_flags_f3f0 & 1) != 0)) ||
      (DAT_0001f424 != 0)) ||
     ((((g_status_flags_f0f8 & 8) != 0 || (DAT_0001f03c != 0)) ||
      ((g_isc_stepper_target_f406 == 0 || (DAT_0001f40e <= g_isc_stepper_target_f406)))))) {
    return 0;
  }
  return 1;
}



// [NEW 2026-07-15, UPDATED same session] isc_f3fa_f3fc_f3fe_correction_calc
// 
// Resolves MUT RequestID 0x09 (F3FB) and 0x0B (F3FD) - see
// mut_verification_status.md master table.
// 
// Same mode-shadow idiom as isc_eeca_eecc_eece_correction_calc (0x27a81,
// which resolves MUT ReqID 0x08/0x0A) - computes one ISC correction term
// (lookup table via pjsr @0x144b4, combined with F402 and F408+0x100 via
// two pjsr @0x14036 calls and a final pjsr @0x1442d saturate), then writes
// the SAME 16-bit value to exactly one of three shadow words based on
// identical gating logic:
//   - F0F8 bit4 == 0                                -> F3FA/F3FB word
//   - F0F8 bit4 == 1, byte@0x2c7 != 1                -> F3FC/F3FD word
//   - F0F8 bit4 == 1, F0F8 bit0 == 1, byte@0x2c7 == 1 -> F3FE/F3FF word (not a MUT ID)
// 
// CONFIRMED (2026-07-15): MUT ReqID 0x09/0x0B genuinely read F3FB/F3FD, the
// LOW byte of the F3FA/F3FC words (big-endian chip) - verified against
// adc_sensor_convert_single (0x171c3)'s single-byte fetch for ReqID<=0xBF.
// This is NOT a scraper artifact (contrast the F5C0 cluster in review.md
// item 7, where odd-offset rows were confirmed artifacts) - the 0x2fad0
// table's entries for these IDs genuinely decode to the low-byte address.

void isc_f3fa_f3fc_f3fe_correction_calc(void)

{
  ushort uVar1;
  undefined2 uVar2;
  byte bVar3;
  ushort unaff_retaddr;
  
  bVar3 = 1;
  uVar1 = (*select_const_by_flags_eeca)();
  get_high_byte(uVar1);
  uVar1 = sat_sub_u16(*(short *)((uint)bVar3 << 0x10 | 0xf408) + 0x100,
                      *(ushort *)((uint)bVar3 << 0x10 | 0xf402));
  sat_sub_u16(uVar1,unaff_retaddr);
  uVar2 = (*clamp_u8)();
  if ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 0x10) == 0) {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf3fa) = uVar2;
  }
  else if ((*(char *)((uint)bVar3 << 0x10 | 0x2c7) == '\x01') &&
          ((*(ushort *)((uint)bVar3 << 0x10 | 0xf0f8) & 1) != 0)) {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf3fe) = uVar2;
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf3fc) = uVar2;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort select_const_by_flags_eeca(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f0f8 & 0x10) == 0) {
    uVar1 = DAT_0001eeca;
  }
  else if ((DAT_000102c7 == '\x01') && ((g_status_flags_f0f8 & 1) != 0)) {
    uVar1 = DAT_0001eece;
  }
  else {
    uVar1 = DAT_0001eecc;
  }
  return uVar1;
}



void isc_eeca_update_dispatch(void)

{
  short sVar1;
  
  sVar1 = isc_eeca_update_gate_condition_check();
  if (sVar1 != 0) {
    (*isc_eeca_eecc_eece_correction_calc)();
  }
  return;
}



undefined2
isc_eeca_update_gate_condition_check(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar4;
  undefined2 uVar5;
  ushort extraout_R2;
  byte bVar6;
  
  uVar3 = DAT_0001f40e;
  bVar6 = 1;
  sVar4 = (*isc_f406_ramp_gate_condition_check)(param_1,param_2,g_isc_stepper_target_f406,param_3);
  if ((((((sVar4 == 0) || ((*(ushort *)((uint)bVar6 << 0x10 | 0xf0ea) & 0x80) != 0)) ||
        (*(short *)((uint)bVar6 << 0x10 | 0xf034) != 0)) ||
       ((uVar2 = *(ushort *)((uint)bVar6 << 0x10 | 0xf10e),
        puVar1 = (ushort *)((uint)bVar6 << 0x10 | 0xc6e), uVar2 < *puVar1 || uVar2 == *puVar1 ||
        ((*(ushort *)((uint)bVar6 << 0x10 | 0xf0f8) & 0x1000) != 0)))) ||
      ((*(char *)((uint)bVar6 << 0x10 | 0x2c6) != '\0' &&
       ((*(ushort *)((uint)bVar6 << 0x10 | 0xf0f8) & 0x4000) != 0)))) ||
     (((uVar3 <= extraout_R2 || (extraout_R2 == 0)) ||
      (*(short *)((uint)bVar6 << 0x10 | 0xefca) != 0)))) {
    uVar5 = 0;
  }
  else {
    uVar5 = 1;
  }
  return uVar5;
}



// [NEW 2026-07-15] isc_eeca_eecc_eece_correction_calc
// 
// Resolves MUT RequestID 0x08 (EECA) and 0x0A (EECC) - see
// mut_verification_status.md master table.
// 
// Computes a single ISC correction term (lookup table @0x144b4 + F402 offset,
// clamped against F408, scaled via 0x1405c/0x14036/0x14000), then writes that
// SAME value to exactly one of three shadow RAM cells depending on mode flags:
//   - F0F8 bit4 == 0                                -> EECA  (MUT ReqID 0x08)
//   - F0F8 bit4 == 1, byte@0x2c7 != 1                -> EECC  (MUT ReqID 0x0A)
//   - F0F8 bit4 == 1, F0F8 bit0 == 1, byte@0x2c7 == 1 -> EECE  (not a MUT ID)
// 
// MUT reading 0x08 vs 0x0A is just reading whichever mode happens to be
// active, not two independent values. Sole caller: isc_eeca_update_dispatch
// (0x27a20).
// 
// Same mode-select idiom (F0F8 bit4/bit0 + byte@0x2c7) also appears in
// isc_f3fa_f3fc_f3fe_correction_calc (0x27990) for a different ISC term
// (F3FA/F3FC/F3FE, not currently MUT IDs) - worth checking for more
// instances of this pattern when tracing other BLANK MUT rows adjacent to
// confirmed ISC cells.

void isc_eeca_eecc_eece_correction_calc(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  ushort *puVar1;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  bool bVar5;
  bool bVar6;
  
  bVar4 = 1;
  uVar2 = (*select_const_by_flags_eeca)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xefca) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xc6a);
  uVar3 = get_high_byte(uVar2);
  uVar3 = sat_sub_u16(uVar3 + *(short *)((uint)bVar4 << 0x10 | 0xf402),param_3);
  puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf408);
  bVar5 = *puVar1 <= uVar3;
  bVar6 = uVar3 != *puVar1;
  if (bVar5 && bVar6) {
    uVar3 = uVar3 - *(short *)((uint)bVar4 << 0x10 | 0xf408);
  }
  else {
    uVar3 = *(short *)((uint)bVar4 << 0x10 | 0xf408) - uVar3;
  }
  uVar3 = sat_mul_u16(uVar3,3);
  if (bVar5 && bVar6) {
    uVar2 = sat_sub_u16(uVar2,uVar3);
  }
  else {
    uVar2 = sat_add_u16(uVar2,uVar3);
  }
  uVar2 = clamp_u16(uVar2,*(ushort *)((uint)bVar4 << 0x10 | 0xc72),
                    *(ushort *)((uint)bVar4 << 0x10 | 0xc70));
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) & 0x10) == 0) {
    *(ushort *)((uint)bVar4 << 0x10 | 0xeeca) = uVar2;
  }
  else if ((*(char *)((uint)bVar4 << 0x10 | 0x2c7) == '\x01') &&
          ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) & 1) != 0)) {
    *(ushort *)((uint)bVar4 << 0x10 | 0xeece) = uVar2;
  }
  else {
    *(ushort *)((uint)bVar4 << 0x10 | 0xeecc) = uVar2;
  }
  return;
}



void isc_f408_composite_dispatch(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  ushort unaff_FP;
  byte bVar4;
  
  bVar4 = 1;
  (*isc_f402_composite_correction_calc)();
  (*f414_trim_calc_gated_by_f0f8_f3f0)();
  (*isc_f3fa_f3fe_dispatch)();
  uVar1 = isc_f3fa_f3fc_f3fe_select();
  uVar2 = (*select_const_by_flags_eeca)();
  uVar2 = get_high_byte(uVar2);
  sat_sub_u16(*(short *)((uint)bVar4 << 0x10 | 0xf402) + uVar2 + uVar1,unaff_FP);
  uVar3 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf408) = uVar3;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort isc_f3fa_f3fc_f3fe_select(void)

{
  ushort uVar1;
  
  if ((g_status_flags_f0f8 & 0x10) == 0) {
    uVar1 = DAT_0001f3fa;
  }
  else if ((DAT_000102c7 == '\x01') && ((g_status_flags_f0f8 & 1) != 0)) {
    uVar1 = DAT_0001f3fe;
  }
  else {
    uVar1 = DAT_0001f3fc;
  }
  return uVar1;
}



void isc_f406_clamp_dispatch(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = isc_stepper_position_eed0 & 0xa0;
  if (uVar1 == 0) {
    uVar1 = clamp_u16(g_isc_stepper_target_f406,0,DAT_0001f40e);
    *(ushort *)((uint)bVar2 << 0x10 | 0xf406) = uVar1;
  }
  (*isc_f412_f082_stall_timer_calc)(uVar1);
  return;
}



undefined1 isc_table_read_byte_clamped_2eca(ushort param_fp1)

{
  ushort in_stack_00000004;
  ushort uStack_4;
  
  uStack_4 = in_stack_00000004;
  if (DAT_00012eca <= in_stack_00000004) {
    uStack_4 = DAT_00012eca;
  }
  return *(undefined1 *)((int)uStack_4 + 0x2eca);
}



void isc_f412_f082_stall_timer_calc(void)

{
  ushort *puVar1;
  short *psVar2;
  ushort uVar3;
  short sVar4;
  byte bVar5;
  
  bVar5 = 1;
  uVar3 = get_high_byte(DAT_00010c70);
  if (uVar3 < *(ushort *)((uint)bVar5 << 0x10 | 0xf412)) {
    uVar3 = get_high_byte(*(ushort *)((uint)bVar5 << 0x10 | 0xeeca));
    *(ushort *)((uint)bVar5 << 0x10 | 0xf412) = uVar3;
  }
  if (((((*(ushort *)((uint)bVar5 << 0x10 | 0xf0f8) & 0x18) == 0) &&
       ((*(ushort *)((uint)bVar5 << 0x10 | 0xf3f0) & 0x10) != 0)) &&
      ((*(ushort *)((uint)bVar5 << 0x10 | 0xf3f0) & 1) == 0)) &&
     ((*(short *)((uint)bVar5 << 0x10 | 0xf406) != 0 &&
      (*(ushort *)((uint)bVar5 << 0x10 | 0xf408) <= *(ushort *)((uint)bVar5 << 0x10 | 0xf414))))) {
    uVar3 = *(ushort *)((uint)bVar5 << 0x10 | 0xf180);
    sVar4 = 4;
    do {
      uVar3 = uVar3 >> 1;
      if (uVar3 == 0) break;
      sVar4 = sVar4 + -1;
    } while (sVar4 != -1);
    if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf400) <= uVar3) &&
       (uVar3 = uVar3 - *(short *)((uint)bVar5 << 0x10 | 0xf400),
       puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xcd2), *puVar1 <= uVar3 && uVar3 != *puVar1)) {
      if (*(short *)((uint)bVar5 << 0x10 | 0xf082) != 0) {
        return;
      }
      if (*(short *)((uint)bVar5 << 0x10 | 0xf412) != 0) {
        psVar2 = (short *)((uint)bVar5 << 0x10 | 0xf412);
        *psVar2 = *psVar2 + -1;
      }
    }
  }
  *(undefined2 *)((uint)bVar5 << 0x10 | 0xf082) = *(undefined2 *)((uint)bVar5 << 0x10 | 0xcd0);
  return;
}



void mirror_f406_f408_to_f40a_f40c(void)

{
  DAT_0001f40a = g_isc_stepper_target_f406;
  DAT_0001f40c = g_isc_stepper_base_f408;
  return;
}



void startup_phase_reset_wrapper(void)

{
  (*startup_phase_reset_eed6_eefa_bulk)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [FOUND 2026-07-26, during output_relay_flags_f0e6 writer sweep] Second
// CONFIRMED writer of f0e6 bit3 (mask 0x08), alongside
// knock_octane_f0e6_bit3_gate_dispatch (0x284a3, the RPM/knock-pattern-gated
// ongoing control). This one sets bit3 UNCONDITIONALLY as part of a warmup-
// state init (also sets f09a from ROM calib 0x10f50, and f594=0x41). Same
// init-then-steady-state-override pattern as f0e6 bit1 (engine_state_f498_init
// unconditional vs egr_f0e6_bit1_update ongoing control, see 0x18600) -- not a
// conflict, just cold-init default followed by real gated control once running.
// 
// f0e6 bit3 drives PCDR.3 (0xfe97... check exact port via
// mirror_status_f0e6_to_ports @ 0x156ce), INVERTED (bit clear = pin set).
// Physical identity still unconfirmed -- plausible MIL/check-engine or
// knock-warning lamp candidate given knock_octane_f0e6_bit3_gate_dispatch's
// RPM-threshold/knock-pattern gating, but not proven. This bit was previously
// MISLABELED as "bit3 = EGR solenoid" in review.md item 1 (an old miscount) --
// corrected 2026-07-26, EGR solenoid is actually bit1. See review.md item 1
// and mut_verification_status.md "PHYSICAL OUTPUT PIN TRACE" section for full
// bit map and context.

void warmup_state_f594_f0e6_init(void)

{
  DAT_0001f09a = DAT_00010f50;
  g_warmup_state_f594 = 0x41;
  output_relay_flags_f0e6 = output_relay_flags_f0e6 | 8;
  return;
}



void engine_warmup_periodic_dispatch(void)

{
  (*startup_phase_reset_eed6_cluster)();
  (*eeea_bit0_update_on_efa2)();
  (*knock_sensor_diagnostic_update)();
  f516_request_latch_f09a_calib_select();
  engine_warmup_dispatch_b();
  (*eee0_eeee_diag_flags_reset_dispatch)();
  (*knock_octane_f0e6_bit3_gate_dispatch)();
  return;
}



// [IMPORTED FROM OLD XML - SESSION 10, 2026-07-03 - NOT YET RE-VERIFIED under current Sleigh
// grammar, review before trusting] Startup-phase reset (gated on EF6E < 4, a boot/warmup phase
// counter). Zeroes/masks EED6, EED8 (Load1B low word), EEDA, EEDC, EEDE, F502, F09A(<-init val
// 0x10f50), EF78, F1FE bit4. This is a RESET path for the EED6-EEDE cluster, NOT the steady-state
// Load1B producer - the real per-cycle Load1B compute site is STILL UNIDENTIFIED (open item as of
// session 10). See also startup_phase_reset_eed6_eefa_bulk (0x28413) for the larger related
// bulk-clear. Original source: old-Sleigh XML export, logging.txt session 10.

void startup_phase_reset_eed6_cluster(void)

{
  ushort *puVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  byte bVar4;
  
  bVar4 = 1;
  if (DAT_0001ef6e < 4) {
    DAT_0001eeec = DAT_0001eeec & 0xffd5;
    DAT_0001eeea = DAT_0001eeea & 0xff95 | 1;
    DAT_0001f502 = 0;
    DAT_0001f09a = DAT_00010f50;
    DAT_0001ef78 = 0;
    uVar2 = (*swap_invert_high_byte)();
    uVar3 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeed6) = uVar3;
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeed8) = uVar2;
    uVar3 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeeda) = uVar3;
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeedc) = uVar2;
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeede) = uVar2;
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf1fe);
    *puVar1 = *puVar1 & 0xffef;
  }
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void eeea_bit0_update_on_efa2(void)

{
  if (DAT_0001efa2 == 0) {
    DAT_0001eeea = DAT_0001eeea | 1;
  }
  else if (((DAT_0001eeea & 1) != 0) || ((g_status_flags_f20e & 1) != 0)) {
    DAT_0001eeea = DAT_0001eeea & 0xfffe;
    DAT_0001ef78 = 0;
  }
  return;
}



void knock_sensor_diagnostic_update(void)

{
  ushort *puVar1;
  ushort uVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  byte bVar7;
  
  bVar7 = 1;
  if (DAT_0001ef6e < 0x21) {
    if ((g_status_flags_f20e & 0x10) != 0) {
      if (DAT_0001f108 < 5) {
        uVar2 = g_warmup_state_f594;
        if (0xf5 < mut_battery_voltage_f13a) goto LAB_00027db2;
      }
    }
    uVar2 = g_warmup_state_f594 & 0xffbf;
  }
  else {
    if ((g_warmup_state_f594 & 1) != 0) {
      if (g_coolant_temp_f1e8 < 0x96) {
        g_warmup_state_f594 = g_warmup_state_f594 & 0xffbf;
      }
      if ((g_warmup_state_f594 & 0x40) != 0) {
        g_warmup_state_f594 = g_warmup_state_f594 | 0x80;
      }
    }
    uVar2 = g_warmup_state_f594 & 0xffbe;
    if (mut_battery_voltage_f13a < 0xb1) {
      uVar2 = g_warmup_state_f594 & 0xff3e;
    }
  }
LAB_00027db2:
  g_warmup_state_f594 = uVar2;
  if (((g_warmup_state_f594 & 0x80) != 0) && ((g_warmup_state_f594 & 2) == 0)) {
    DAT_0001ef7a = 0x3c;
    sVar3 = (*sum_byte_array)();
    sVar4 = (*sum_byte_array)();
    sVar5 = (*sum_byte_array)();
    sVar6 = (*sum_byte_array)();
    *(short *)((uint)bVar7 << 0x10 | 0xf596) = sVar3 + sVar4 + sVar5 + sVar6;
    puVar1 = (ushort *)((uint)bVar7 << 0x10 | 0xf594);
    *puVar1 = *puVar1 | 2;
  }
  return;
}



// [FOUND 2026-07-15] Consumer of the sci1_meta_cmd_dispatch_c0_ff (0x28869)
// actuator-request bits on @0xf516.
// 
// Latch state machine gated on @0xeeea bit7:
// - Not latched: if (f516 bit6) OR (ram byte @0x205 != 0) OR (f594 bit7):
//   latch on (eeea |= 0x80), and unless mode f502==1, load output register
//   @0xf09a from calibration value @0xf052.
// - Latched: if (ram @0x205==0) AND ((f516 bit5==0) OR (ef6e > 3)): latch off
//   (eeea &= ~0x80), load @0xf09a from calibration value @0xf050 instead.
// - Every call unconditionally clears @0xf516 bits 2, 5, 6, 8 on exit.
// 
// This confirms f516 bits 2/5/6/8 are ONE-SHOT REQUEST flags: dispatcher
// commands C3(bit6)/C4(bits6+8)/FA(bits2+5) set them, this function consumes
// and clears them each call. f09a looks like a selector between two
// calibration/setpoint values (f050 vs f052) - plausibly two alternate
// target/shift maps. Physical meaning of f050/f052/f09a still unconfirmed.
// See mut_verification_status.md dispatcher section.

void f516_request_latch_f09a_calib_select(void)

{
  ushort uVar1;
  undefined2 uVar2;
  short sVar3;
  char in_ram_00000205;
  undefined2 in_ram_00000f50;
  undefined2 in_ram_00000f52;
  
  uVar1 = g_f09a_calib_select_latch_flags_eeea;
  if ((uVar1 & 0x80) == 0) {
    uVar1 = g_f516_request_latch;
    if ((((uVar1 & 0x40) != 0) || (in_ram_00000205 != '\0')) ||
       (uVar1 = DAT_0000f594, (uVar1 & 0x80) != 0)) {
      uVar1 = g_f09a_calib_select_latch_flags_eeea;
      g_f09a_calib_select_latch_flags_eeea = uVar1 | 0x80;
      sVar3 = DAT_0000f502;
      uVar2 = DAT_0000f502;
      uVar2 = DAT_0000f502;
      if (sVar3 != 1) {
        DAT_0000f09a = in_ram_00000f52;
      }
    }
  }
  else if (in_ram_00000205 == '\0') {
    uVar1 = g_f516_request_latch;
    if ((uVar1 & 0x20) == 0) {
      uVar2 = g_f09a_calib_select_release_counter_ef6e;
      uVar1 = g_f09a_calib_select_release_counter_ef6e;
      uVar2 = g_f09a_calib_select_release_counter_ef6e;
      if (3 < uVar1) goto LAB_00027e73;
    }
    uVar1 = g_f09a_calib_select_latch_flags_eeea;
    g_f09a_calib_select_latch_flags_eeea = uVar1 & 0xff7f;
    DAT_0000f09a = in_ram_00000f50;
  }
LAB_00027e73:
  uVar1 = g_f516_request_latch;
  g_f516_request_latch = uVar1 & 0xff9f;
  uVar1 = g_f516_request_latch;
  g_f516_request_latch = uVar1 & 0xfefb;
  return;
}



void engine_warmup_dispatch_b(void)

{
  f502_state_table_dispatch();
  (*eee4_eeda_bit6_update_from_f0f8_bit12)();
  return;
}



void f502_state_table_dispatch(undefined2 param_1)

{
  byte bVar1;
  ushort uVar2;
  ushort *puVar3;
  byte bVar4;
  byte *pbStack_8;
  
  bVar4 = 1;
  f502_state_index_update(param_1,0);
  pbStack_8 = (byte *)(*(short *)((uint)bVar4 << 0x10 | 0xf502) * 6 + 0x369a);
  uVar2 = (*(code *)(uint)CONCAT12((char)*(undefined2 *)(pbStack_8 + 2),
                                   *(undefined2 *)(pbStack_8 + 4)))();
  if (uVar2 == 1) {
    bVar1 = *pbStack_8;
    uVar2 = (*swap_invert_high_byte)();
    *(ushort *)((short)&DAT_0000eed6 + (ushort)bVar1 * 2) = uVar2;
  }
  else if (uVar2 == 0) {
    bVar1 = *pbStack_8;
    puVar3 = (ushort *)((short)&DAT_0000eed6 + (ushort)bVar1 * 2);
    if (*(short *)((uint)bVar4 << 0x10 | 0xf09a) == 0) {
      uVar2 = (*swap_invert_high_byte)();
      *puVar3 = uVar2;
      uVar2 = (*swap_invert_high_byte)();
      *(ushort *)((short)&DAT_0000eee0 + (ushort)bVar1 * 2) = uVar2;
    }
    else {
      uVar2 = (ushort)pbStack_8[1] & *puVar3;
      if (uVar2 == 0) {
        return;
      }
    }
  }
  f502_state_index_update(uVar2,1);
  return;
}



void __stdcall_far f502_state_index_update(undefined2 _unused_far_pad,ushort advance_flag)

{
  ushort uVar1;
  ushort uVar2;
  
  uVar2 = DAT_0001f502;
  if (advance_flag != 0) {
    uVar2 = DAT_0001f502 + 1;
  }
  uVar1 = 0x14;
  if (DAT_0001ef78 < DAT_00010f4e) {
    uVar1 = 0xb;
  }
  if (uVar1 < uVar2) {
    uVar2 = 0;
  }
  if (uVar2 != DAT_0001f502) {
    if (((DAT_0001eeea & 0x80) == 0) || (uVar2 == 1)) {
      DAT_0001f09a = DAT_00010f50;
    }
    else {
      DAT_0001f09a = DAT_00010f52;
    }
  }
  DAT_0001f502 = uVar2;
  return;
}



undefined2 f17a_load_zone_and_ef96_f1d8_check(void)

{
  undefined2 uVar1;
  
  if (DAT_00010f54 < engine_torque_pct_f17a) {
    if ((DAT_0001ef96 == 0) || ((ushort)(DAT_00010f58 * 0x200) <= DAT_0001f1d8)) {
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 2;
  }
  return uVar1;
}



// f502_state_table_dispatch state 12 handler (mask 0002). Unconditionally returns 2 (deny/hold) --
// no reads, no side effects. Part of the 16-function batch resolved by
// H8StateTableDispatchAnalyzer, see review8.md.

undefined2 f502_state12_const_deny2_stub(void)

{
  return 2;
}



// f502_state_table_dispatch state 1 handler (mask 0040). If DAT_0001efa2==0: returns 2 if
// g_status_flags_f0f8 bit6 (0x40) clear, else 0. If DAT_0001efa2!=0: returns 1. Sibling of
// phase_dispatch_efa2_f500_f5ee_gate_check (state 2), which shares the DAT_0001efa2 top-level gate.
// Part of the 16-function batch resolved by H8StateTableDispatchAnalyzer, see review8.md.

undefined2 f0f8_bit6_efa2_phase_gate_check(void)

{
  undefined2 uVar1;
  
  if (DAT_0001efa2 == 0) {
    if ((g_status_flags_f0f8 & 0x40) == 0) {
      uVar1 = 2;
    }
    else {
      uVar1 = 0;
    }
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



undefined2 phase_dispatch_efa2_f500_f5ee_gate_check(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined2 extraout_R2;
  
  uVar2 = 2;
  if (DAT_0001efa2 != 0) {
    if (DAT_0001f500 != 0) {
      if ((DAT_0001f5ee & 1) != 0) {
        return 1;
      }
      sVar1 = (*check_flag_f0d8_bit0_clear)();
      uVar2 = extraout_R2;
      if (sVar1 != 0) goto LAB_00028034;
    }
    uVar2 = 0;
  }
LAB_00028034:
  return uVar2;
}



// f502_state_table_dispatch state 3 handler (mask 0101). This was already a defined function before
// this session (unnamed FUN_00028038) -- H8StateTableDispatchAnalyzer only added the missing
// COMPUTED_CALL reference to it, did not create it. If DAT_0001f1ec==0: returns 0 when
// engine_torque_pct_f17a and multiple other zone bounds/flags (DAT_00010f74/f76/f4e/ef78,
// g_status_flags_f0f8 bit7, g_status_flags_f20e bit1, DAT_0001f25a bit5) all clear/in-range, else
// 2. If DAT_0001f1ec!=0: returns 1. Part of the state-table batch, see review8.md.

undefined2 f1ec_torque_f17a_multi_zone_gate_check(void)

{
  undefined2 uVar1;
  
  if (DAT_0001f1ec == 0) {
    if (((((g_status_flags_f0f8 & 0x80) == 0) && (DAT_00010f74 < engine_torque_pct_f17a)) &&
        (DAT_00010f76 < inj_pw_output_f186)) &&
       (((DAT_00010f4e <= DAT_0001ef78 && ((g_status_flags_f20e & 2) == 0)) &&
        ((g_status_flags_001f25a & 0x20) == 0)))) {
      return 0;
    }
    uVar1 = 2;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}



undefined2 f17a_f13c_load_zone_and_f514_gate_check(void)

{
  if ((((DAT_0001efa2 == 0) || (DAT_00010f7c <= engine_torque_pct_f17a)) ||
      (DAT_00010f7e <= g_adc_ch7_raw_f13c)) || (DAT_0001f514 != -1)) {
    return 2;
  }
  if ((DAT_0001f200 & 2) == 0) {
    return 1;
  }
  return 0;
}



// f502_state_table_dispatch state 5 handler (mask 0140). Returns (DAT_0001f340 & 1) == 0 as bool.
// Simplest handler in the table besides the constant stubs. Part of the 16-function batch resolved
// by H8StateTableDispatchAnalyzer, see review8.md.

bool f340_bit0_clear_check(void)

{
  return (DAT_0001f340 & 1) == 0;
}



// f502_state_table_dispatch state 6 handler (mask 0180). Unconditionally returns 1 -- no reads, no
// side effects. Part of the 16-function batch resolved by H8StateTableDispatchAnalyzer, see
// review8.md.

undefined2 f502_state6_const_hold1_stub(void)

{
  return 1;
}



// f502_state_table_dispatch state 7 handler (mask 0201). Unconditionally returns 1 -- no reads, no
// side effects. Part of the 16-function batch resolved by H8StateTableDispatchAnalyzer, see
// review8.md.

undefined2 f502_state7_const_hold1_stub(void)

{
  return 1;
}



// f502_state_table_dispatch state 8 handler (mask 0020). Largest/most complex handler in the table
// (116 bytes). Conditionally writes DAT_0001f00c = DAT_00010f64 * 0x50 (a timer/count reload, gated
// on g_status_flags_f20e bit4, DAT_0001f10e vs DAT_00010f66, DAT_0001f10a vs DAT_0001f10c), then
// evaluates DAT_0001f1fe bits 0x10/0x1 combined with DAT_00010f4e/DAT_0001ef78 and DAT_0001f09a to
// decide 0/1/2, with a side effect of setting DAT_0001f1fe bit4 (0x10) in one path. Worth a closer
// look if this state's transition timing turns out to matter. Part of the 16-function batch
// resolved by H8StateTableDispatchAnalyzer, see review8.md.

undefined2 f1fe_f00c_timer_calc_gate_check(void)

{
  undefined2 uVar1;
  
  uVar1 = 2;
  if ((((g_status_flags_f20e & 0x10) != 0) || (g_tps_load_axis_f10e != DAT_00010f66)) ||
     (DAT_0001f10a <= DAT_0001f10c)) {
    DAT_0001f00c = DAT_00010f64 * 0x50;
  }
  if ((g_status_flags_f1fe & 0x10) == 0) {
    if (((g_status_flags_f1fe & 1) != 0) && (DAT_00010f4e <= DAT_0001ef78)) {
      uVar1 = 0;
      if (DAT_0001f09a == 0) {
        g_status_flags_f1fe = g_status_flags_f1fe | 0x10;
      }
      goto LAB_0002814d;
    }
    if ((g_status_flags_f20e & 0x10) != 0) goto LAB_0002814d;
    if (DAT_0001f00c != 0) {
      if (DAT_00010f66 < g_tps_load_axis_f10e) {
        return 1;
      }
      goto LAB_0002814d;
    }
  }
  uVar1 = 0;
LAB_0002814d:
  return uVar1;
}



// f502_state_table_dispatch state 9 handler (mask 0110). Returns (DAT_0001f1fe & 0x20) == 0 as
// bool. Side effect: when that bit IS set (false path), also clears DAT_0001f09a to 0 -- the same
// DAT_0001f09a read (not written) by f1fe_f00c_timer_calc_gate_check (state 8), suggesting these
// two states cooperate around a shared timer/counter reset. Part of the 16-function batch resolved
// by H8StateTableDispatchAnalyzer, see review8.md.

bool f1fe_bit5_clear_reset_f09a_check(void)

{
  bool bVar1;
  
  bVar1 = (g_status_flags_f1fe & 0x20) == 0;
  if (!bVar1) {
    DAT_0001f09a = 0;
  }
  return bVar1;
}



// f502_state_table_dispatch state 13 handler (mask 0104). If DAT_0001f1f2 bit7 (0x80) set: returns
// 2 if g_status_flags_f3a4 bit10 (0x400) clear, else returns 0 if DAT_0001f200 bit0 (0x1) set.
// Falls through to return 1 otherwise. Part of the 16-function batch resolved by
// H8StateTableDispatchAnalyzer, see review8.md.

undefined2 f1f2_bit7_f3a4_bit10_gate_check(void)

{
  if ((g_status_flags_f1f2 & 0x80) != 0) {
    if ((knock_per_cyl_flags_f3a4 & 0x400) == 0) {
      return 2;
    }
    if ((DAT_0001f200 & 1) != 0) {
      return 0;
    }
  }
  return 1;
}



// f502_state_table_dispatch state 14 handler (mask 0004). Returns (DAT_0001f1fe & 2) == 0 as bool
// -- the third distinct DAT_0001f1fe bit checked across this table (see also state 9's bit5 and
// state 15's bit6), consistent with DAT_0001f1fe being a multi-bit status/condition register this
// whole subsystem consults. Part of the 16-function batch resolved by H8StateTableDispatchAnalyzer,
// see review8.md.

bool f1fe_bit1_clear_check(void)

{
  return (g_status_flags_f1fe & 2) == 0;
}



// f502_state_table_dispatch state 15 handler (mask 0008). Returns (DAT_0001f1fe & 0x40) == 0 as
// bool. See f1fe_bit1_clear_check and f1fe_bit5_clear_reset_f09a_check for sibling bit checks on
// the same status word. Part of the 16-function batch resolved by H8StateTableDispatchAnalyzer, see
// review8.md.

bool f1fe_bit6_clear_check(void)

{
  return (g_status_flags_f1fe & 0x40) == 0;
}



// f502_state_table_dispatch state 16 handler (mask 0102). If DAT_0001ef92==0: returns 1 if
// DAT_00010f7a <= DAT_0001f12e < DAT_00010f78 (in-range check), else 0. If DAT_0001ef92!=0: returns
// 2. Part of the 16-function batch resolved by H8StateTableDispatchAnalyzer, see review8.md.

undefined2 ef92_f12e_range_gate_check(void)

{
  undefined2 uVar1;
  
  if (DAT_0001ef92 == 0) {
    if ((DAT_00010f7a <= DAT_0001f12e) && (DAT_0001f12e < DAT_00010f78)) {
      return 1;
    }
    uVar1 = 0;
  }
  else {
    uVar1 = 2;
  }
  return uVar1;
}



// f502_state_table_dispatch state 17 handler (mask 0204). If DAT_000102ea != 0 (a byte flag,
// unusual address -- worth checking what else touches it): returns 2 if g_status_flags_f20e bit4
// (0x10) OR g_status_flags_f0f8 bit5 (0x20) set, else 0 if DAT_0001f4d6 bit2 (0x4) set. Falls
// through to return 1 otherwise. Part of the 16-function batch resolved by
// H8StateTableDispatchAnalyzer, see review8.md.

undefined2 flag102ea_f4d6_bit2_gate_check(void)

{
  if (DAT_000102ea != '\0') {
    if (((g_status_flags_f20e & 0x10) != 0) || ((g_status_flags_f0f8 & 0x20) != 0)) {
      return 2;
    }
    if ((DAT_0001f4d6 & 4) != 0) {
      return 0;
    }
  }
  return 1;
}



// f502_state_table_dispatch state 18 handler (mask 0208). Unconditionally returns 1 -- no reads, no
// side effects. Part of the 16-function batch resolved by H8StateTableDispatchAnalyzer, see
// review8.md.

undefined2 f502_state18_const_hold1_stub(void)

{
  return 1;
}



// f502_state_table_dispatch state 19 handler (mask 0210). Returns 1 unless DAT_0001f1f2 bit13
// (0x2000) is set AND (DAT_0001f4ae & 0xa) is nonzero, in which case returns 0. Near-identical
// shape to state 20's handler (mask 0x5 instead of 0xa on the same DAT_0001f4ae) -- likely two
// related sub-conditions on the same byte. Part of the 16-function batch resolved by
// H8StateTableDispatchAnalyzer, see review8.md.

undefined2 f1f2_bit13_f4ae_mask0a_gate_check(void)

{
  undefined2 uVar1;
  
  if (((g_status_flags_f1f2 & 0x2000) == 0) || ((DAT_0001f4ae & 10) == 0)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



// f502_state_table_dispatch state 20 handler (mask 0220). Returns 1 unless DAT_0001f1f2 bit13
// (0x2000) is set AND (DAT_0001f4ae & 0x5) is nonzero, in which case returns 0. See
// f1f2_bit13_f4ae_mask0a_gate_check (state 19) -- same shape, different mask on DAT_0001f4ae. Part
// of the 16-function batch resolved by H8StateTableDispatchAnalyzer, see review8.md.

undefined2 f1f2_bit13_f4ae_mask05_gate_check(void)

{
  undefined2 uVar1;
  
  if (((g_status_flags_f1f2 & 0x2000) == 0) || ((DAT_0001f4ae & 5) == 0)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



// f502_state_table_dispatch state 10 handler (mask 0120). Unconditionally returns 1 -- no reads, no
// side effects. Part of the 16-function batch resolved by H8StateTableDispatchAnalyzer, see
// review8.md.

undefined2 f502_state10_const_hold1_stub(void)

{
  return 1;
}



// f502_state_table_dispatch state 11 handler (mask 0301, last record in the first sub-table).
// Returns 1 unless DAT_0001f1f4 bit2 (0x4) is set AND both DAT_0001eedc bit0 (0x1) and DAT_0001f202
// bit1 (0x2) are clear, in which case returns 0. Part of the 16-function batch resolved by
// H8StateTableDispatchAnalyzer, see review8.md.

undefined2 f1f4_eedc_f202_gate_check(void)

{
  undefined2 uVar1;
  
  if (((DAT_0001f1f4 & 4) == 0) || (((DAT_0001eedc & 1) == 0 && ((DAT_0001f202 & 2) == 0)))) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



void eee4_eeda_bit6_update_from_f0f8_bit12(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  if ((g_status_flags_f0f8 & 0x1000) == 0) {
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xeee4) = uVar1;
  }
  else {
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xeee4) = uVar1;
  }
  uVar1 = (*swap_invert_high_byte)();
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xeeda) = uVar1;
  return;
}



// [FOUND 2026-07-15] Reads @0xf516 bit7 near the end: if set, calls
// startup_phase_reset_eed6_eefa_bulk (0x28413) - the bulk zero of the entire
// EED6-EEFA RAM cluster (includes EED8 = Load1B, see mut_verification_status.md
// item 1). Always clears @0xf516 bits 1 and 7 on exit (bit1's setter is still
// unknown - not present in sci1_meta_cmd_dispatch_c0_ff's table or hardcoded
// cases either).
// 
// This makes f516 bit7 - set by dispatcher command 0xFC
// (sci1_meta_cmd_dispatch_c0_ff, 0x28869) - a LIVE, on-demand trigger for this
// bulk diagnostic/RAM-axis-pointer reset, not just a startup-time call. In
// other words: command 0xFC over SCI1 can force a Load1B/diag-flag reset at
// runtime. Also gates several other EEE0/EEE4/EEDA/EED6/EEE2/EEE6/EEE8 flag
// toggles via swap_invert_high_byte earlier in the function - not yet
// individually decoded.

void eee0_eeee_diag_flags_reset_dispatch(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  undefined2 uVar1;
  short sVar2;
  ushort extraout_R2;
  ushort extraout_R2_00;
  ushort extraout_R2_01;
  ushort uVar3;
  byte bVar4;
  ushort in_SR;
  ushort uStack_4;
  undefined2 uStack_2;
  
  bVar4 = 1;
  uVar3 = g_status_flags_f1f2;
  uStack_2 = param_3;
  if ((g_status_flags_f1f2 & 0x8000) != 0x8000) {
    DAT_0001eeee = 0;
    uStack_4 = DAT_0001eee0 & 0xfffe;
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeee0) = uVar1;
    uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeed6) & 0xfffe;
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeed6) = uVar1;
    uVar3 = extraout_R2;
  }
  if ((uVar3 & 0x8004) != 0x8004) {
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeef0) = 0;
    uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeee4) & 0xfffd;
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeee4) = uVar1;
    uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeeda) & 0xfffd;
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeeda) = uVar1;
    uVar3 = extraout_R2_00;
  }
  if ((uVar3 & 0x8002) != 0x8002) {
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeef2) = 0;
    uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeee4) & 0xff7f;
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeee4) = uVar1;
    uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeeda) & 0xff7f;
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeeda) = uVar1;
    uVar3 = extraout_R2_01;
  }
  if ((uVar3 & 0x8006) != 0x8006) {
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeef4) = 0;
    uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeee0) & 0xffef;
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeee0) = uVar1;
    uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeed6) & 0xffef;
    uVar1 = (*swap_invert_high_byte)();
    *(undefined2 *)((uint)bVar4 << 0x10 | 0xeed6) = uVar1;
  }
  uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeee0) & 0xff;
  uVar1 = (*swap_invert_high_byte)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xeee0) = uVar1;
  uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeee2) & 0xff;
  uVar1 = (*swap_invert_high_byte)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xeee2) = uVar1;
  uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeee4) & 0xff;
  uVar1 = (*swap_invert_high_byte)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xeee4) = uVar1;
  uStack_4 = *(ushort *)((uint)bVar4 << 0x10 | 0xeee6) & 1;
  uVar1 = (*swap_invert_high_byte)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xeee6) = uVar1;
  uStack_4 = 0;
  sVar2 = (*swap_invert_high_byte)();
  *(short *)((uint)bVar4 << 0x10 | 0xeee8) = sVar2;
  uStack_4 = in_SR & 0xfff0 | (ushort)((undefined1 *)&DAT_0000fffd < &uStack_4) |
             (ushort)(sVar2 == 0) << 2 | (ushort)(sVar2 < 0) << 3;
  bVar4 = 0;
  uVar3 = g_f516_request_latch;
  if ((uVar3 & 0x80) != 0) {
    (*startup_phase_reset_eed6_eefa_bulk)();
  }
  *(ushort *)((uint)bVar4 << 0x10 | 0xf516) = *(ushort *)((uint)bVar4 << 0x10 | 0xf516) & 0xff7d;
  return;
}



// [IMPORTED FROM OLD XML - SESSION 10, 2026-07-03 - NOT YET RE-VERIFIED under current Sleigh
// grammar, review before trusting] Bulk reset/init routine - zeroes the ENTIRE RAM cluster
// EED6-EEFA
// (EED6,EED8[Load1B],EEDA,EEDC,EEDE,EEE0,EEE2,EEE4,EEE6,EEE8,EEEE,EEF0,EEF2,EEF4,EEF6,EEF8,EEFA)
// plus F5AC, and masks F4AE/F4B0/F1FE/EEEA/F4D6 flag bits. NOTE: this EEDx-EEFx range overlaps
// several "efXX RAM axis pointer" cells already flagged as unreconciled in the MT-specific XML's
// UNKNOWN TABLES notes (e.g. eff4=Load, effc=Coolant, efea=RPM-shaped-axis-data) - this confirms
// those are all part of one shared init/reset block, consistent with them being live RAM index
// variables rather than independently-sourced sensor values. Like startup_phase_reset_eed6_cluster
// (0x27cb7), this is a RESET path, not a per-cycle producer. Load1B's real steady-state writer is
// STILL OPEN (unresolved as of session 10). Original source: old-Sleigh XML export, logging.txt
// session 10.

void startup_phase_reset_eed6_eefa_bulk(void)

{
  ushort uVar1;
  undefined2 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = (*swap_invert_high_byte)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeed6) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeed8) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeeda) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeedc) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeede) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeee0) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeee2) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeee4) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeee6) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeee8) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeeee) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeef0) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeef2) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeef4) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeef6) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf5ac) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeef8) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeefa) = 0;
  uVar1 = g_tcu_purge_hysteresis_status_flags_f4ae;
  g_tcu_purge_hysteresis_status_flags_f4ae = uVar1 & 0xfff0;
  uVar1 = g_tcu_purge_duty_gate_byte_f4b0;
  g_tcu_purge_duty_gate_byte_f4b0 = uVar1 & 0xfff0;
  uVar1 = sensor_validity_status_flags_f1fe;
  sensor_validity_status_flags_f1fe = uVar1 & 0xffcf;
  uVar1 = g_f09a_calib_select_latch_flags_eeea;
  g_f09a_calib_select_latch_flags_eeea = uVar1 & 0xffbf;
  uVar1 = DAT_0000f4d6;
  DAT_0000f4d6 = uVar1 & 0xfffb;
  return;
}



// WARNING: Restarted to delay deadcode elimination for space: ram

ushort knock_octane_f0e6_bit3_gate_dispatch(ushort param_1,ushort param_2,undefined2 param_3)

{
  ushort *puVar1;
  bool bVar2;
  ushort uVar3;
  ushort uVar4;
  byte bVar5;
  
  bVar5 = 1;
  bVar2 = false;
  if (((DAT_0001f526 & 0x80) != 0) && (param_1 = DAT_0001f520 & 0xc0, param_1 != 0)) {
    DAT_0001f518 = DAT_0001f518 | 0x80;
  }
  if (DAT_0001ef90 == 0) {
    DAT_0001f518 = DAT_0001f518 & 0xff7f;
  }
  else {
    if ((g_warmup_state_f594 & 0x80) == 0) {
      param_1 = (ushort)((uint)g_rpm_threshold_scale_cal * 0x14 >> 0x10);
      param_2 = (ushort)((uint)g_rpm_threshold_scale_cal * 0x14);
      bVar2 = false;
      if ((DAT_0001ef6e < param_2) || ((g_status_flags_f0f8 & 0x1000) != 0)) {
LAB_0002852c:
        uVar3 = 1;
      }
      else {
        if (((DAT_0001f0f6 & 0x800) != 0) && ((DAT_0001f1ec == 0 && ((DAT_0001f518 & 0x80) == 0))))
        goto LAB_000284f8;
        uVar4 = 0xff;
        uVar3 = 0xf3;
        if ((g_status_flags_f1f0 & 0x40) != 0) {
          uVar4 = 0xee;
          uVar3 = 0x71;
        }
        if ((((g_diag_flags_eed6 & uVar4) != 0) || (param_1 = DAT_0001eed8 & 0xee, param_1 != 0)) ||
           ((g_diag_flags_eeda & uVar3) != 0)) goto LAB_0002852c;
        uVar3 = 0;
      }
    }
    else {
LAB_000284f8:
      param_1 = (*knock_octane_pattern_match_state_machine)(param_1,param_2,param_3,param_3);
      uVar3 = param_1;
      bVar2 = true;
    }
    if (uVar3 != 0) {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf0e6);
      *puVar1 = *puVar1 | 8;
      goto LAB_00028541;
    }
  }
  puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf0e6);
  *puVar1 = *puVar1 & 0xfff7;
LAB_00028541:
  if (!bVar2) {
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf04a) = 0;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf51c) = 0;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf51e) = 0x500;
  }
  return param_1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uchar knock_octane_pattern_match_state_machine(void)

{
  char cVar1;
  undefined2 uVar2;
  byte bVar3;
  byte bStack_6;
  byte bStack_4;
  
  if (DAT_0001f04a != 0) goto LAB_00028692;
  if (((DAT_0001eee8 | DAT_0001eee6 | g_diag_flags_eee4 | DAT_0001eee2 | DAT_0001eee0) & 0xff) == 0)
  {
    DAT_0001f51a = 10;
    DAT_0001f04a = 0x14;
    DAT_0001f51c = 0;
    DAT_0001f51e = 0x500;
    goto LAB_00028692;
  }
  cVar1 = (char)DAT_0001f51c;
  DAT_0001f51c = DAT_0001f51c & 0xff;
  if (cVar1 == '\0') {
    bStack_6 = (byte)((ushort)DAT_0001f51e >> 8);
    bStack_4 = (byte)DAT_0001f51e;
    uVar2 = *(undefined2 *)((int)((ushort)bStack_6 << 1) + -0x1120);
    do {
      bStack_4 = bStack_4 << 1;
      if ((bStack_4 == 0) || ((char)uVar2 == '\0')) {
        bStack_4 = 1;
        bStack_6 = bStack_6 + 1;
        if (4 < bStack_6) {
          bStack_6 = 0;
        }
        uVar2 = *(undefined2 *)((int)((ushort)bStack_6 << 1) + -0x1120);
      }
    } while ((bStack_4 & (byte)uVar2) == 0);
    bVar3 = 0;
    if (bStack_4 == 1) {
LAB_000285ff:
      DAT_0001f51c = (ushort)*(byte *)((ushort)bStack_6 * 8 + 0x3718 + (ushort)bVar3);
    }
    else {
      if (bStack_4 == 2) {
LAB_000285fd:
        bVar3 = bVar3 + 1;
        goto LAB_000285ff;
      }
      if (bStack_4 == 4) {
LAB_000285fb:
        bVar3 = bVar3 + 1;
        goto LAB_000285fd;
      }
      if (bStack_4 == 8) {
LAB_000285f9:
        bVar3 = bVar3 + 1;
        goto LAB_000285fb;
      }
      if (bStack_4 == 0x10) {
LAB_000285f7:
        bVar3 = bVar3 + 1;
        goto LAB_000285f9;
      }
      if (bStack_4 == 0x20) {
LAB_000285f5:
        bVar3 = bVar3 + 1;
        goto LAB_000285f7;
      }
      if (bStack_4 == 0x40) {
LAB_000285f3:
        bVar3 = bVar3 + 1;
        goto LAB_000285f5;
      }
      if (bStack_4 == 0x80) {
        bVar3 = 1;
        goto LAB_000285f3;
      }
      DAT_0001f51c = 0;
    }
    DAT_0001f51e = CONCAT11(bStack_6,bStack_4);
  }
  cVar1 = (char)DAT_0001f51c;
  if (cVar1 != '\0') {
    if ((DAT_0001f51c & 0xf0) == 0) {
      if (cVar1 != '\0') {
        DAT_0001f51c = (ushort)(byte)(cVar1 - 1);
      }
      if ((DAT_0001f51c & 0xf) != 0) {
        DAT_0001f51a = 0x14;
        DAT_0001f04a = 0x28;
        goto LAB_00028692;
      }
      DAT_0001f51a = 0x78;
    }
    else {
      DAT_0001f51c = (ushort)(byte)(cVar1 - 0x10U);
      if ((cVar1 - 0x10U & 0xf0) != 0) {
        DAT_0001f51a = 0x14;
        DAT_0001f04a = 0x50;
        goto LAB_00028692;
      }
      DAT_0001f51a = 0x50;
    }
    DAT_0001f04a = 0x8c;
  }
LAB_00028692:
  return DAT_0001f51a < DAT_0001f04a;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked
// [FOUND 2026-07-15] Second, separate consumer of sci1_meta_cmd_dispatch_c0_ff
// (0x28869) actuator-request bits on @0xf516 - reads the HIGH bits (11/12/13/15,
// masks 0x800/0x1000/0x2000/0x8000), NOT the low bits (2/5/6/8) that
// f516_request_latch_f09a_calib_select (0x27e1a) consumes.
// 
// Clears f516 &= 0x7FF (top 5 bits) on entry, then derives a mode value into
// @0xf520 (0x80 if f594 bit7 set OR f516 bit15; 0 if f516 bit11; 0x20 if
// f516 bit13; 0x08 if f516 bit12; else 0), then a 4-bit sub-mode into
// @0xf0f2 based on f520's bits, while @0xf0f4 is hardcoded to 8.
// 
// CORRECTED 2026-07-22: initially hypothesized this drove a PWM-style dual-
// speed fan output based on peripheral_fec0_fed0_state_update's graduated
// mode values. REFUTED after checking the actual register labels: fec0-fec5/
// fec8-fecd/fed0-fed5 are SCI3/SCI1/SCI2 SMR/BRR/SCR/TDR/SSR/RDR (confirmed
// via list_globals) - this is UART BAUD RATE / SERIAL CONTROL reconfiguration,
// not an actuator or fan driver. f0f2's mode values (1/2/4/8) select between
// baud-rate/serial-config presets applied simultaneously across SCI1/2/3,
// most likely a slow-init-handshake vs fast-K-line-comms speed switch typical
// of MUT/ISO-style diagnostic protocols. NOT an actuator mechanism - do not
// cite this as a fan/relay/solenoid candidate.
// 
// OPEN QUESTION (unchanged): no static writer found anywhere for f516 bits
// 11/12/13/15 - sci1_meta_cmd_dispatch_c0_ff's C0-FF table only ever sets
// bits 0-8. Given the corrected finding above, these bits are more likely
// tied to protocol/comms timing/handshake state than to any actuator command
// - worth checking sci1_protocol_state_machine's own internal state transitions
// rather than assuming a hidden actuator dispatcher exists.

void f516_hibits_f520_f0f2_mode_select(void)

{
  ushort uVar1;
  ushort uVar2;
  byte in_ram_00000200;
  ushort uVar3;
  
  uVar1 = g_f516_request_latch;
  uVar3 = g_f516_request_latch;
  g_f516_request_latch = uVar3 & 0x7ff;
  uVar3 = DAT_0000f520;
  uVar3 = uVar3 & 0xf8;
  uVar2 = DAT_0000f594;
  if ((uVar2 & 0x80) == 0) {
    if ((uVar1 & 0x800) == 0) {
      if ((uVar1 & 0x8000) != 0) goto LAB_0002873e;
      if ((uVar1 & 0x2000) == 0) {
        if ((uVar1 & 0x1000) != 0) {
          uVar3 = 8;
        }
      }
      else {
        uVar3 = 0x20;
      }
    }
    else {
      uVar3 = 0;
    }
  }
  else {
LAB_0002873e:
    uVar3 = 0x80;
  }
  uVar1 = DAT_0000f520;
  DAT_0000f520 = uVar3 | uVar1 & 0xff07;
  uVar3 = DAT_0000f520;
  if ((uVar3 & 0x10) == 0) {
    uVar3 = DAT_0000f520;
    if ((uVar3 & 0xa8) != 0) {
      uVar3 = 4;
      goto LAB_0002878f;
    }
    uVar3 = DAT_0000f520;
    if ((uVar3 & 0x40) != 0) {
      uVar3 = 2;
      goto LAB_0002878f;
    }
    if ((in_ram_00000200 & 1) != 0) {
      uVar3 = 8;
      goto LAB_0002878f;
    }
  }
  uVar3 = 1;
LAB_0002878f:
  uVar1 = DAT_0000f0f2;
  DAT_0000f0f2 = uVar3 | uVar1 & 0xfff0;
  uVar3 = DAT_0000f0f4;
  DAT_0000f0f4 = uVar3 & 0xfff0 | 8;
  return;
}



// [FOUND 2026-07-14] SCI1 protocol state machine. Feeds command bytes into
// sci1_dispatch_and_latch_response (0x2882b) -> sci1_meta_cmd_dispatch_c0_ff
// (0x28869), the real actuator/command dispatcher (separate from the MUT
// table read path at 0x2fad0). See plate comment at 0x28869 and
// mut_verification_status.md "REAL COMMAND DISPATCHER FOUND" section.

void sci1_protocol_state_machine(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  ushort *puVar1;
  ushort uVar2;
  short sVar3;
  undefined2 extraout_R1;
  ushort extraout_R2;
  byte bVar4;
  
  bVar4 = 1;
  sVar3 = (*f00e_f520_f526_gate_condition_check)(param_1,param_2,0x80,param_3);
  if (sVar3 != 0) {
    (*f510_f512_f00e_f514_reset)();
  }
  (*sci1_tx_response_feeder)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf04c) = 0x14;
  if ((extraout_R2 & *(ushort *)((uint)bVar4 << 0x10 | 0xf594)) == 0) {
    if (((*(short *)((uint)bVar4 << 0x10 | 0xf04c) == 0) ||
        (*(short *)((uint)bVar4 << 0x10 | 0xef90) == 0)) ||
       (((extraout_R2 & *(ushort *)((uint)bVar4 << 0x10 | 0xf526)) != 0 &&
        (2000 < *(ushort *)((uint)bVar4 << 0x10 | 0xf590))))) {
      *(undefined2 *)((uint)bVar4 << 0x10 | 0xf526) = 0;
    }
  }
  else {
    *(ushort *)((uint)bVar4 << 0x10 | 0xf526) = extraout_R2;
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf522);
    *puVar1 = *puVar1 | 1;
  }
  uVar2 = extraout_R2 & *(ushort *)((uint)bVar4 << 0x10 | 0xf526);
  if (uVar2 == 0) {
    (*sci1_post_handshake_status_broadcast_dispatch)(0);
  }
  else if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf520) & extraout_R2) == 0) {
    if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf520) & 0x20) != 0) {
      (*f526_state_reset_dispatch)(uVar2,extraout_R1,0);
    }
  }
  else {
    (*sci1_dispatch_and_latch_response)();
  }
  return;
}



// [FOUND 2026-07-14] Reads a command byte from RAM 0xf534 and passes it to
// sci1_meta_cmd_dispatch_c0_ff (0x28869) - the real actuator/command dispatcher,
// separate from the MUT table read path. See plate comment at 0x28869 and
// mut_verification_status.md "REAL COMMAND DISPATCHER FOUND" section.
// Caller: sci1_protocol_state_machine (0x287af).

void sci1_dispatch_and_latch_response(void)

{
  short sVar1;
  byte bVar2;
  
  bVar2 = 1;
  if (((g_sci1_rx_status_flags_f522 & 0x40) != 0) && ((g_sci1_rx_status_flags_f522 & 0x20) == 0)) {
    sVar1 = (*sci1_meta_cmd_dispatch_c0_ff)();
    if (sVar1 != -1) {
      *(short *)((uint)bVar2 << 0x10 | 0xf54a) = sVar1;
      *(undefined2 *)((uint)bVar2 << 0x10 | 0xf58c) = 1;
      (*sci1_latch_and_send_f54a_handshake_byte)();
    }
  }
  *(ushort *)((uint)bVar2 << 0x10 | 0xf522) = *(ushort *)((uint)bVar2 << 0x10 | 0xf522) & 0xff9f;
  return;
}



// [FOUND 2026-07-14, UPDATED 2026-07-15, 2026-07-22 (x2), 2026-07-26 (x3)] Real command-byte
// dispatcher for SCI1, separate from the MUT table (0x2fad0)/
// adc_sensor_convert_single (0x171c3) read path used throughout
// mut_verification_status.md.
// 
// - Command byte < 0xC0 (boundary check @0x28870): looks up a WORD via
//   eaw_disp16 addressing on Rn_banked(R0), DP=2 -- i.e. effective address
//   = ((DP<<16)|R0) + (-0x530), a genuine 24-bit banked ROM read (grammar
//   traced 2026-07-22 via h8539f.slaspec's eaw_disp16/Rn_banked
//   constructors). Table region labeled sci1_cmd_lt0xc0_ptrtable_unprogrammed
//   @ 0x1fad0. RESOLVED 2026-07-22: entire possible table span (cmd 0x00-0xBF,
//   0x1FAD0-0x1FC4E) plus padding (0x1F800-0x1FC90, ~1200 bytes) is ALL 0xFF
//   -- completely UNPROGRAMMED/blank on this ROM (RVR_1998_x3 4g63t 21000011
//   md352553.hex), confirmed as real in-range reads, not address errors. Dead
//   code path on THIS ROM. NOT checked on the other two ROM files in
//   test/rvr/roms/. REACHABILITY CONFIRMED 2026-07-22: caller
//   sci1_dispatch_and_latch_response (0x2882b) invokes unconditionally on
//   command-byte value. Command byte read from RAM 0xf534 (labeled
//   sci1_rx_frame_buf_0), traced to sci1_rx_frame_accumulator (0x16716,
//   entry point) which fills it live from the SCI1 UART RX register as bytes
//   arrive on the wire.
// 
// - Command byte 0xC0-0xD8 inclusive (excl. C3/C4): TABLE-DRIVEN via
//   cmd_c0_d8_actuator_bit_table @ ROM 0x13740, (cmd-0xC0)*4 byte offset.
//   Table is 25 entries (0xC0-0xD8), ends right after the 0xD8 entry. Byte0
//   selects f512(1)/f510(0), bytes[2:3] are a 16-bit big-endian mask.
// 
//   REAL GATE MECHANISM (decompiled directly 2026-07-26, more authoritative
//   than the byte-table alone): cmd values 0xCA/0xCB are UNCONDITIONALLY
//   skipped by an explicit top-level `cmd != 0xCA && cmd != 0xCB` check --
//   they never reach the table logic at all. For every OTHER command in the
//   C0-D8 table range, the write only takes effect if BOTH (f00e==0) AND
//   (f25a bit4 SET) at the moment the byte is processed -- otherwise the
//   function falls through to a no-op (echoes existing f516, returns 0xFF)
//   regardless of the table's mask. Applies to ALL non-CA/CB table entries,
//   e.g. cmd 0xD1's purge override requires f25a bit4 set on top of the
//   downstream f20e/f510 conditions already documented. f25a's meaning is
//   not yet independently characterized.
// 
//   TABLE FULLY RE-VERIFIED 2026-07-26 (previous CC-D8 region was uniformly
//   off-by-one -- confirmed via direct per-entry read_memory at every address,
//   not eyeballing a combined hex dump):
//     C0-C4: f512, mask 0 (stub/unimplemented, base offset confirmed correct
//       -- re-checked C0 itself: 01 FF 00 00)
//     C5-C9: f512, mask 0x10,0x08,0x04,0x02,0x01 (clean descending sweep)
//     CA-CB: f510, mask 0, ALSO unconditionally hard-skipped (see above)
//     CC:    f510, mask 0x2000 (bit13) -- was misattributed to CD in all
//       prior notes (this plate comment and review.md item 1 both said
//       "CD sets f510 bit13"; CC was called a stub). Confirmed 0x13770 =
//       00 FF 20 00.
//     CD:    f510, mask 0x1000 (bit12) -- was called 0x2000/CC's value
//     CE:    f510, mask 0x0800 (bit11)
//     CF:    f510, mask 0x0400 (bit10)
//     D0:    f510, mask 0x0200 (bit9)
//     D1:    f510, mask 0x0100 (bit8) -- unchanged, matches purge-override
//       finding (purge_enable_check reads f510 bit8)
//     D2:    f510, mask 0x0082 (bit7 + bit1) -- unchanged mask value, but
//       now understood correctly: bit7 is D2's OWN expected slot in the
//       descending sweep (13,12,11,10,9,8,7...); the extra bit1 is a
//       deliberate SECOND effect, not an anomaly (see D8 below).
//     D3:    f510, mask 0x0040 (bit6) -- CONFIRMED 0x1378c = 00 FF 00 40.
//       Previously this slot was wrongly called "the D3 two-bit entry
//       (0x0082)" -- that value actually belongs to D2, one slot earlier.
//     D4:    f510, mask 0x0020 (bit5) -- confirmed 0x13790
//     D5:    f510, mask 0x0010 (bit4) -- confirmed 0x13794
//     D6:    f510, mask 0x0008 (bit3) -- confirmed 0x13798
//     D7:    f510, mask 0x0004 (bit2) -- confirmed 0x1379c
//     D8:    f510, mask 0x0002 (bit1) -- confirmed 0x137a0
// 
//   BOTH previously-flagged "table anomalies" are RESOLVED and DISSOLVE
//   simultaneously once correctly aligned: there is no D8 pattern-break and
//   no true D3 dual-bit oddity. The real structure is one perfectly clean,
//   uninterrupted 13-entry descending bit sweep from CC (bit13) through D8
//   (bit1), with exactly ONE deliberate deviation: cmd 0xD2 sets its own
//   expected bit (bit7) PLUS a duplicate of D8's bit (bit1) as a secondary
//   effect -- i.e. cmd 0xD2 forces two independent override checks at once
//   (bit7's ignition-zone check AND bit1's still-unidentified consumer),
//   exactly as previously hypothesized, just now correctly attributed to D2
//   rather than "D3's anomaly" and "D8's break" as two separate unexplained
//   things.
// 
//   Real disassembly confirms table applies to cmd < 0xD9 i.e. through
//   0xD8 inclusive, then 0xD9 is a SEPARATE explicit branch (not
//   table-driven, not subject to the f25a gate): sets f510 |= 1 (bit0)
//   directly if f00e==0. C3/C4 are also hardcoded: C3-> f512 bit6 (gated
//   f00e==0, latches f00e=0xffff), C4-> f516 bits6+8 (UNCONDITIONAL).
// 
// - Full hardcoded (non-table) case list: C3->f512 bit6, C4->f516 bits6+8,
//   D9->f510 bit0, CA/CB->unconditionally skipped, DA-DF (<0xE0, excl D9,
//   gated f00e==0 AND f1ec==0)->clears a computed bit in f514 via a
//   shift-loop keyed on (cmd - 0xD9), EC-EF->per-gear 4-byte struct at ROM
//   0x232 (gear_indexed_4byte_struct_ec_ef, byte-reversed per gear index
//   f1fc), F3->calls f510_f512_f00e_f514_reset (bulk reset), F8->zeros f526
//   (gated f00e==0), F9->no-op read (gated f00e==0), FA->f516 bits2+5 (gated
//   f594 bit7==0 AND ram 0x205==0), FB->f516 bit6, FC->f516 bit7, FD->
//   toggles f522 bit2, returns high/low byte of f596 alternately, FE/FF->
//   per-gear word at ROM 0x252 (gear_indexed_word_table_fe_ff), high/low
//   byte respectively.
// 
// - ALL f516 bits accounted for, including bit1/bit12 (CLOSED 2026-07-26,
//   full method in mut_verification_status.md -- exhaustive
//   search_byte_patterns sweep, 27 hits manually inspected, none set bit1
//   or bit12; same literal-addressing-blind-spot caveat as item 1's f0e6
//   bit13).
// 
// MODE5 ACTUATOR REQUESTID CONCLUSION (2026-07-22): the RVR EvoScan XML
// profile's 8 Mode5 (Actuator Test) RequestIDs do NOT map onto anything
// live on this ROM. This EvoScan RequestID numbering is UNRELATED to this
// dispatcher's own native command byte values (0xC0-0xFF).
// 
// NOT YET DONE: characterize f25a bit4 (the gate covering the whole
// table-driven range) and f510 bit1's OTHER consumer (D2's secondary
// effect, separate from D8's own primary bit1 consumer already traced to
// engine_mode_f20e_f510_check); reader for f510/f512 bits corresponding to
// CC-D0 (bits 8-13, now confirmed set by cmd CC-D1) -- not yet traced
// forward the way bit1/bit7/bit8 were; whether this dispatcher is reachable
// from SCI3 or only SCI1; whether either table is programmed on the other
// two ROM files in test/rvr/roms/. See mut_verification_status.md "REAL
// COMMAND DISPATCHER FOUND" / "DP=2 POINTER TABLE ... RESOLVED" /
// "PHYSICAL OUTPUT PIN TRACE" / "~20 UNTRACED ... CLOSED" for full details.
// Caller: sci1_dispatch_and_latch_response (0x2882b).

ushort sci1_meta_cmd_dispatch_c0_ff(ushort param_fp1)

{
  ushort uVar1;
  short sVar2;
  ushort uVar3;
  byte *pbVar4;
  ushort in_stack_00000004;
  ushort uStack_6;
  
  uStack_6 = 0;
  if (in_stack_00000004 < 0xc0) {
    return (ushort)**(byte **)((int)(in_stack_00000004 << 1) + -0x530);
  }
  uVar3 = DAT_0001f516;
  if ((in_stack_00000004 != 0xca) && (in_stack_00000004 != 0xcb)) {
    if (in_stack_00000004 == 0xc4) {
      DAT_0001f516 = DAT_0001f516 | 0x140;
      goto LAB_00028aca;
    }
    if (in_stack_00000004 == 0xc3) {
      if (DAT_0001f00e == 0) {
        DAT_0001f512 = DAT_0001f512 | 0x40;
LAB_00028927:
        DAT_0001f00e = -1;
        goto LAB_00028aca;
      }
    }
    else if (in_stack_00000004 < 0xd9) {
      if ((DAT_0001f00e == 0) && ((g_status_flags_001f25a & 0x10) != 0)) {
        sVar2 = ((short)&T2CRH + in_stack_00000004) * 4;
        uVar1 = *(ushort *)((int)(sVar2 + 0x3740) + 2);
        if (uVar1 != 0) {
          sVar2 = (short)*(char *)((int)sVar2 + 0x3740) << 1;
          *(ushort *)((int)sVar2 + -0xaf0) = *(ushort *)((int)sVar2 + -0xaf0) | uVar1;
          if ((g_warmup_state_f594 & 0x80) != 0) {
            DAT_0001f00e = 8;
            goto LAB_00028aca;
          }
LAB_00028961:
          DAT_0001f00e = 0xf0;
          goto LAB_00028aca;
        }
      }
    }
    else if (in_stack_00000004 == 0xd9) {
      if (DAT_0001f00e == 0) {
        gear_mode_flags_f510 = gear_mode_flags_f510 | 1;
        goto LAB_00028927;
      }
    }
    else if (in_stack_00000004 < 0xe0) {
      if ((DAT_0001f00e == 0) && (DAT_0001f1ec == 0)) {
        uVar3 = 1;
        sVar2 = (short)&TMDRA + in_stack_00000004;
        if (in_stack_00000004 != 0xd9) {
          do {
            uVar3 = uVar3 << 1;
            if (uVar3 == 0) break;
            sVar2 = sVar2 + -1;
          } while (sVar2 != -1);
        }
        DAT_0001f514 = DAT_0001f514 & ~uVar3;
        goto LAB_00028961;
      }
    }
    else {
      if ((0xeb < in_stack_00000004) && (in_stack_00000004 < 0xf0)) {
        pbVar4 = (byte *)(DAT_0001f1fc * 4 + 0x232);
        if (in_stack_00000004 == 0xec) {
          return (ushort)pbVar4[3];
        }
        if (in_stack_00000004 == 0xed) {
          return (ushort)pbVar4[2];
        }
        if (in_stack_00000004 == 0xee) {
          return (ushort)pbVar4[1];
        }
        if (in_stack_00000004 == 0xef) {
          return (ushort)*pbVar4;
        }
        goto LAB_00028aca;
      }
      if (in_stack_00000004 == 0xf3) {
        (*f510_f512_f00e_f514_reset)();
        goto LAB_00028aca;
      }
      if (0xf7 < in_stack_00000004) {
        if (in_stack_00000004 == 0xf8) {
          if (DAT_0001f00e == 0) {
            DAT_0001f526 = 0;
            goto LAB_00028aca;
          }
        }
        else if (in_stack_00000004 == 0xf9) {
          if (DAT_0001f00e == 0) goto LAB_00028aca;
        }
        else {
          if (in_stack_00000004 != 0xfa) {
            if (in_stack_00000004 == 0xfb) {
              DAT_0001f516 = DAT_0001f516 | 0x40;
            }
            else if (in_stack_00000004 == 0xfc) {
              DAT_0001f516 = DAT_0001f516 | 0x80;
            }
            else {
              if (in_stack_00000004 == 0xfd) {
                if ((g_warmup_state_f594 & 0x80) == 0) {
                  return uRam00010220;
                }
                if ((g_sci1_rx_status_flags_f522 & 4) == 0) {
                  g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 | 4;
                  return DAT_0001f596 >> 8;
                }
                g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 & 0xfffb;
                return DAT_0001f596 & 0xff;
              }
              if (in_stack_00000004 == 0xfe) {
                return *(ushort *)((int)(DAT_0001f1fc << 1) + 0x252) >> 8;
              }
              if (in_stack_00000004 == 0xff) {
                return *(ushort *)((int)(DAT_0001f1fc << 1) + 0x252) & 0xff;
              }
            }
            goto LAB_00028aca;
          }
          uVar3 = DAT_0001f516 | 4;
          if (((g_warmup_state_f594 & 0x80) == 0) && (cRam00010205 == '\0')) {
            DAT_0001f516 = DAT_0001f516 | 0x24;
            goto LAB_00028aca;
          }
        }
      }
    }
  }
  DAT_0001f516 = uVar3;
  uStack_6 = 0xff;
LAB_00028aca:
  return uStack_6;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f00e_f520_f526_gate_condition_check(void)

{
  ushort uVar1;
  
  if (((((DAT_0001f00e == 0) || ((DAT_0001f520 & 0xc0) == 0)) || ((DAT_0001f526 & 0x80) == 0)) ||
      (((g_status_flags_001f25a & 0x10) == 0 &&
       (((gear_mode_flags_f510 & 0x3ffe) != 0 || ((DAT_0001f512 & 0x1f) != 0)))))) ||
     ((DAT_0001f1ec != 0 && ((DAT_0001f514 & 0x3f) != 0x3f)))) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



void f510_f512_f00e_f514_reset(void)

{
  gear_mode_flags_f510 = 0;
  DAT_0001f512 = 0;
  DAT_0001f00e = 0;
  DAT_0001f514 = 0xffff;
  return;
}



// WARNING: Control flow encountered bad instruction data
// WARNING: Removing unreachable block (ram,0x000250f2)
// [RESOLVED 2026-08-07] "halt_baddata()" decompiler warnings on cases 0/2/6
// are CONFIRMED SPURIOUS -- read raw disassembly directly at every jump-table
// target (8 entries at 0x28b52-0x28b60, all landing 0x28b62-0x28cc8) and every
// single one is clean, valid H8 code with no bad instructions anywhere. The
// decompiler's switch() reconstruction for this indexed jump (via
// `mov:g.w @(-0x74ae:16,R4),R4` / `jmp @R4`, EP=2 bank-relative) is simply
// wrong -- ignore the decompiled body of this function entirely; trust only
// the raw disassembly at each of the 8 targets below.
// 
// MAJOR FINDING: this state machine is the source of the MUT-II protocol's
// well-documented post-handshake status broadcast. Public MUT-II protocol
// documentation (K-line, 5-baud init) confirms the ECU sends bytes
// "C0 55 EF 85" before it starts responding to MUT commands. This maps almost
// exactly onto the ISO-9141/K-line keyword-protocol shape (sync byte, then
// keyword byte(s), then address echo):
//   - 0x55 = the universal K-line SYNC byte (case 3, gated on a 40-tick timer
//     + F516 bit9)
//   - 0xEF / 0xF8 / (0x08,0x08) = the KEYWORD byte(s), selected by case 5 from
//     F520's protocol-mode value (F520 bit7 -> 0xEF, matching the public doc
//     exactly; F520 bit5 -> 0xF8; else -> two bytes 0x08 0x08) -- three
//     variants beyond the single public-doc value, likely covering different
//     MUT-II sub-protocol/speed negotiations
//   - 0x85 = sent by case 7/idx7, matches the public doc's 4th byte exactly
//     (likely an ECU/address echo), then parks the machine (F526=0x80) and
//     hands control to normal MUT command processing
// See sci1_handshake_status_byte_builder_and_f516_hibit_writer (0x28b89) plate
// comment for the full precise per-case breakdown of cases 1-6 including the
// F520/F516 mode-selection logic. All bytes are staged in F54A (single-byte
// cases) or F54A+F54C (two-byte case) and transmitted via
// sci1_latch_and_send_f54a_handshake_byte (0x166cd) into FECB (SCI1 TDR),
// which is the downstream half of the autonomous-TX chain documented on
// sci1_tx_response_feeder (0x166d7).
// 
// Per-target index (raw disassembly, all confirmed clean):
//   idx0 (0x28b62) f588_duty_gate_f516_bit11_set -- rearm/phase-advance, shares
//     F58C duty-period register with case 5's byte-count field
//   idx1 (0x28b89) case 1 -- init F528/F52A/F52C/F52E, arm F530=10
//   idx2 (0x28ba8) case 2 -- F516 bits 13/15 writer, mode-lock state machine
//   idx3 (0x28c5a) case 3 -- SENDS 0x55 (sync byte)
//   idx4 (0x28c78) case 4 -- inter-byte timing spacer (F590>=4 gate)
//   idx5 (0x28c83) case 5 -- SENDS 0xEF / 0xF8 / (0x08,0x08) (keyword byte(s))
//   idx6 (0x28cba) case 6 -- inter-byte timing spacer, clears F584
//   idx7 (0x28cc8) sci1_send_final_handshake_byte_85_and_park_phase_machine --
//     SENDS 0x85, parks phase machine, hands off to sci1_dispatch_and_latch_response
// 
// Called from sci1_protocol_state_machine (0x287af) only when F526 bit7 is
// clear (link not yet fully up / still in handshake-broadcast mode). See
// mut_verification_status.md serial/TCU section.

undefined2
sci1_post_handshake_status_broadcast_dispatch
          (undefined2 param_1,undefined2 param_2,undefined1 *param_3)

{
  char *pcVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined2 uVar4;
  short sVar5;
  undefined2 extraout_R2;
  undefined1 *puVar6;
  undefined2 unaff_R3;
  char cVar7;
  undefined2 unaff_R5;
  short unaff_FP;
  bool bVar8;
  
  if (DAT_0001f532 != -1) {
    DAT_0001f532 = DAT_0001f532 + 1;
  }
  if (7 < DAT_0001f526) {
    DAT_0001f526 = 0;
    return param_1;
  }
  sVar5 = *(short *)((int)(DAT_0001f526 << 1) + -0x74ae);
  switch(DAT_0001f526) {
  case 0:
    uVar4 = CONCAT11((char)((ushort)unaff_R3 >> 8),
                     *(byte *)((int)unaff_R3 + 0x43f3) ^ (byte)unaff_R3);
    pcVar1 = (char *)((int)uVar4 + 0x61f1);
    *pcVar1 = *pcVar1 + '\x02';
    *(ushort *)((int)unaff_R5 + -0x1167) = *(ushort *)((int)unaff_R5 + -0x1167) ^ 0x4000;
    *(undefined2 *)((short)&UNK_0000ffee + unaff_FP) = uVar4;
    *(undefined2 *)((short)&DAT_0000fff2 + unaff_FP) = unaff_R5;
                    // WARNING: Bad instruction - Truncating control flow here
    halt_baddata();
  case 2:
                    // WARNING: Bad instruction - Truncating control flow here
    halt_baddata();
  case 3:
    return 0;
  case 4:
    if (-1 < sVar5) {
      *(short *)(unaff_FP + 0x1d) = sVar5;
                    // WARNING: Bad instruction - Truncating control flow here
      halt_baddata();
    }
    puVar6 = (undefined1 *)&DAT_0000f8bb;
    cVar7 = '\b';
    do {
      puVar3 = puVar6;
      puVar6 = puVar6 + 1;
      puVar2 = param_3;
      param_3 = param_3 + 1;
      *puVar2 = *puVar3;
      bVar8 = cVar7 != -1;
      cVar7 = cVar7 + -1;
    } while (bVar8);
    return 8;
  case 5:
    return param_1;
  case 6:
                    // WARNING: Bad instruction - Truncating control flow here
    halt_baddata();
  case 7:
    lambda_enable_flag_update();
    fueling_lambda_update();
    fueling_wideband_output_scale();
    fueling_coolant_inj_offset_calc();
    uVar4 = (*rpm_load_zone_counter_update)();
    return uVar4;
  }
  uVar4 = 2;
  if (DAT_0001efa2 != 0) {
    if (DAT_0001f500 != 0) {
      if ((DAT_0001f5ee & 1) != 0) {
        return 1;
      }
      sVar5 = (*check_flag_f0d8_bit0_clear)();
      uVar4 = extraout_R2;
      if (sVar5 != 0) goto LAB_00028034;
    }
    uVar4 = 0;
  }
LAB_00028034:
  return uVar4;
}



// WARNING: Removing unreachable block (ram,0x00028c83)
// WARNING: Removing unreachable block (ram,0x00028c91)
// WARNING: Removing unreachable block (ram,0x00028c97)
// WARNING: Removing unreachable block (ram,0x00028ca4)
// WARNING: Removing unreachable block (ram,0x00028c89)
// WARNING: Removing unreachable block (ram,0x00028c5a)
// WARNING: Removing unreachable block (ram,0x00028c61)
// WARNING: Removing unreachable block (ram,0x00028c67)
// WARNING: Removing unreachable block (ram,0x00028c9d)
// WARNING: Removing unreachable block (ram,0x00028cb3)
// WARNING: Removing unreachable block (ram,0x00028c6e)
// WARNING: Removing unreachable block (ram,0x00028c75)
// WARNING: Removing unreachable block (ram,0x00028b89)
// WARNING: Removing unreachable block (ram,0x00028b90)
// WARNING: Removing unreachable block (ram,0x00028ba8)
// WARNING: Removing unreachable block (ram,0x00028bb4)
// WARNING: Removing unreachable block (ram,0x00028bae)
// WARNING: Removing unreachable block (ram,0x00028bb8)
// WARNING: Removing unreachable block (ram,0x00028bc4)
// WARNING: Removing unreachable block (ram,0x00028bc8)
// WARNING: Removing unreachable block (ram,0x00028bd3)
// WARNING: Removing unreachable block (ram,0x00028bdf)
// WARNING: Removing unreachable block (ram,0x00028bd9)
// WARNING: Removing unreachable block (ram,0x00028be3)
// WARNING: Removing unreachable block (ram,0x00028be9)
// WARNING: Removing unreachable block (ram,0x00028bed)
// WARNING: Removing unreachable block (ram,0x00028bf3)
// WARNING: Removing unreachable block (ram,0x00028bfd)
// WARNING: Removing unreachable block (ram,0x00028c04)
// WARNING: Removing unreachable block (ram,0x00028c08)
// WARNING: Removing unreachable block (ram,0x00028c14)
// WARNING: Removing unreachable block (ram,0x00028c1b)
// WARNING: Removing unreachable block (ram,0x00028c33)
// WARNING: Removing unreachable block (ram,0x00028c3b)
// WARNING: Removing unreachable block (ram,0x00028c4a)
// WARNING: Removing unreachable block (ram,0x00028d1b)
// WARNING: Removing unreachable block (ram,0x00028c25)
// WARNING: Removing unreachable block (ram,0x00028c4f)
// WARNING: Removing unreachable block (ram,0x00028c78)
// WARNING: Removing unreachable block (ram,0x00028c80)
// WARNING: Removing unreachable block (ram,0x00028cba)
// WARNING: Removing unreachable block (ram,0x00028cc2)
// [FOUND 2026-07-15] CONFIRMED clean in disassembly (verified, not decompiler
// noise): `bset.w @0xf516:16, 0xb` - this is the missing WRITER for f516 bit11
// that f516_hibits_f520_f0f2_mode_select (0x28700) consumes (see its plate
// comment / dispatcher notes). Resolves that open question.
// 
// Logic: if (@0xf522 bit0 == 0) OR (@0xf588 >= @0xf58c) [duty/period counter
// elapsed]: clear f522 bit0, zero f58c, set f516 bit11, and if @0xf102 bit5 is
// set, advance the phase counter @0xf526 by 1.
// 
// f526 is the same phase-index consumed by the switch in FUN_00028b2f's
// case-0..7 block - this function is a computed-jump branch target reached
// FROM inside that function (address 0x28b2f), i.e. Ghidra has split what is
// really one logical routine into these pieces because of an unresolved jump
// table there. Sibling branch: f528_f52e_zero_f526_advance_alt_branch
// (0x28b89), same phase-advance idea gated on the opposite f102 bit5 state.
// Not yet named/traced: what f102 bit5, f522 bit0, f526's phase meaning, or
// f588/f58c actually represent physically (duty-cycle test sequencer is a
// reasonable guess given the pattern, unconfirmed).

void f588_duty_gate_f516_bit11_set(void)

{
  if (((g_sci1_rx_status_flags_f522 & 1) == 0) || (DAT_0001f58c <= DAT_0001f588)) {
    g_sci1_rx_status_flags_f522 = g_sci1_rx_status_flags_f522 & 0xfffe;
    DAT_0001f58c = 0;
    DAT_0001f516 = DAT_0001f516 | 0x800;
    if ((DAT_0001f102 & 0x20) != 0) {
      DAT_0001f526 = DAT_0001f526 + 1;
    }
  }
  return;
}



// WARNING: Removing unreachable block (ram,0x00028c83)
// WARNING: Removing unreachable block (ram,0x00028c91)
// WARNING: Removing unreachable block (ram,0x00028c97)
// WARNING: Removing unreachable block (ram,0x00028ca4)
// WARNING: Removing unreachable block (ram,0x00028c89)
// WARNING: Removing unreachable block (ram,0x00028c5a)
// WARNING: Removing unreachable block (ram,0x00028c61)
// WARNING: Removing unreachable block (ram,0x00028c67)
// WARNING: Removing unreachable block (ram,0x00028c9d)
// WARNING: Removing unreachable block (ram,0x00028cb3)
// WARNING: Removing unreachable block (ram,0x00028c6e)
// WARNING: Removing unreachable block (ram,0x00028c75)
// WARNING: Removing unreachable block (ram,0x00028ba8)
// WARNING: Removing unreachable block (ram,0x00028bb4)
// WARNING: Removing unreachable block (ram,0x00028bae)
// WARNING: Removing unreachable block (ram,0x00028bb8)
// WARNING: Removing unreachable block (ram,0x00028bc4)
// WARNING: Removing unreachable block (ram,0x00028bc8)
// WARNING: Removing unreachable block (ram,0x00028bd3)
// WARNING: Removing unreachable block (ram,0x00028bdf)
// WARNING: Removing unreachable block (ram,0x00028bd9)
// WARNING: Removing unreachable block (ram,0x00028be3)
// WARNING: Removing unreachable block (ram,0x00028be9)
// WARNING: Removing unreachable block (ram,0x00028bed)
// WARNING: Removing unreachable block (ram,0x00028bf3)
// WARNING: Removing unreachable block (ram,0x00028bfd)
// WARNING: Removing unreachable block (ram,0x00028c04)
// WARNING: Removing unreachable block (ram,0x00028c08)
// WARNING: Removing unreachable block (ram,0x00028c14)
// WARNING: Removing unreachable block (ram,0x00028c1b)
// WARNING: Removing unreachable block (ram,0x00028c33)
// WARNING: Removing unreachable block (ram,0x00028c3b)
// WARNING: Removing unreachable block (ram,0x00028c4a)
// WARNING: Removing unreachable block (ram,0x00028d1b)
// WARNING: Removing unreachable block (ram,0x00028c25)
// WARNING: Removing unreachable block (ram,0x00028c4f)
// WARNING: Removing unreachable block (ram,0x00028c78)
// WARNING: Removing unreachable block (ram,0x00028c80)
// WARNING: Removing unreachable block (ram,0x00028cba)
// WARNING: Removing unreachable block (ram,0x00028cc2)
// [RESOLVED 2026-08-07] Confirmed physical meaning: this is the MUT-II K-line
// handshake KEYWORD BYTE SELECTOR, gated on F520's protocol-mode value (set by
// f516_hibits_f520_f0f2_mode_select, 0x28700). This function contains cases
// 1-6 (of 8) from sci1_post_handshake_status_broadcast_dispatch's (0x28b2f)
// switch on F526.
// 
// Case 1 (@0x28b89): if F102 bit5==0: zero F528/F52A/F52C/F52E, set F530=10,
//   advance phase.
// Case 2 (@0x28ba8): tracks F52A/F52C/F52E as up/down counters gated on F102
//   bit5 and (F102^F106) bit5; once F52E reaches 0x50(80): sets/clears F528
//   bit9 from F52A bit15, decrements/right-shifts F530 countdown; once
//   F530==0 AND F52C<5: reset phase to 0 (abort). Once F530==0:
//     - if F528==0x0200: F516 |= 0x8200 (bits 9 and 15)
//     - elif F528==0x0220: F516 |= 0x2200 (bits 9 and 13), advance
//     - else: reset phase to 0 (abort)
//   WRITER for F516 bits 13/15 (both also set bit9) -- confirmed unrelated to
//   the SCI1 command dispatcher, purely this handshake sequencer's internal
//   state.
// Case 3 (@0x28c5a): *** SENDS THE SYNC BYTE 0x55 *** -- gated on elapsed
//   counter F532>=0x28(40 ticks) AND F516 bit9 set (written by case 2 above,
//   confirming the sync byte only fires after the case-2 state machine has
//   locked onto a mode). Matches ISO-9141/K-line convention: 0x55 is the
//   universal K-line sync byte, sent before the two protocol keyword bytes.
// Case 4 (@0x28c78): trivial gate on F590>=4 elapsed ticks, abort or advance
//   only (inter-byte timing spacer).
// Case 5 (@0x28c83): *** SENDS THE KEYWORD BYTE(S) *** -- reads F520 (set by
//   f516_hibits_f520_f0f2_mode_select from F594 bit7/F516 bits 11/12/13/15):
//     - F520 bit7 set (F594 bit7 or F516 bit15) -> F54A = 0xEF, F58C=1 (1 byte)
//     - else F520 bit5 set (F516 bit13)         -> F54A = 0xF8, F58C=1 (1 byte)
//     - else (F520 bit3 clear-path / F516 bit11) -> F54A=0x08, F54C=0x08,
//       F58C=2 (TWO bytes: 0x08 0x08)
//   Confirms three distinct keyword-byte responses depending on which
//   protocol/comms mode F516's high bits selected -- likely corresponds to
//   MUT-II variant/speed negotiation (the public MUT-II doc's documented
//   "EF" keyword byte matches the F520-bit7 case exactly; 0xF8 and the 0x08/
//   0x08 pair are additional variants not covered by that public doc, possibly
//   TCU-address or alternate-baud negotiation paths). Calls
//   sci1_latch_and_send_f54a_handshake_byte (0x166cd) to transmit.
// Case 6 (@0x28cba): trivial gate on F590>=4, clears F584, abort or advance.
// 
// See sci1_post_handshake_status_broadcast_dispatch (0x28b2f) plate comment
// for the full 8-case picture including case 7 (sends the documented final
// 0x85 byte and parks the machine) and case 0
// (f588_duty_gate_f516_bit11_set, the rearm/period logic sharing F58C with
// case 5 here). F54A/F54C are confirmed as the TX staging registers feeding
// sci1_latch_and_send_f54a_handshake_byte / sci1_tx_response_feeder, NOT
// actuator/duty-cycle output targets as an earlier hypothesis suggested.

void sci1_handshake_status_byte_builder_and_f516_hibit_writer(void)

{
  if ((DAT_0001f102 & 0x20) == 0) {
    DAT_0001f528 = 0;
    DAT_0001f52a = 0;
    DAT_0001f52c = 0;
    DAT_0001f52e = 0;
    DAT_0001f530 = 10;
    DAT_0001f526 = DAT_0001f526 + 1;
  }
  return;
}



// idx7 target of the sci1_periodic_phase_dispatch_f526 (0x28b2f) 8-entry jump table -- the "small
// remaining gap" flagged in that function's 2026-07-15 plate comment. Function was undefined (raw
// bytes only, no xref-driven auto-creation despite the table entry pointing here). Created + named
// 2026-07-21.
// 
// Body: gates on f520 bits 0x88/0x20. If f520&0x88==0 and f520&0x20==0: normal phase-advance path
// -- resets f526 to 0 if f590>0x14 (wrap/timeout), returns early if f590<0xa or f584!=1 or
// f534!=0xf7 (not-yet-ready gates), otherwise increments f526 (advance to next phase). If f520&0x20
// set: resets f598/f59a first, then falls through. If f520&0x88 set (or after the f598/f59a reset):
// sets f54a=0x85, f58c=1, calls latch_request_f588(f520&0x88) function pointer, and force-sets
// f526=0x80 -- 0x80 is out of the normal 0-7 phase range (gated by `cmp:i #0x7:16,R4 / bhi` in the
// dispatcher), so this looks like a deliberate abort/fault sentinel rather than a valid next phase,
// distinct from the normal increment path above.

void sci1_send_final_handshake_byte_85_and_park_phase_machine(void)

{
  byte bVar1;
  
  bVar1 = 1;
  if ((DAT_0001f520 & 0x88) == 0) {
    if ((DAT_0001f520 & 0x20) == 0) {
      if (0x14 < DAT_0001f590) {
        DAT_0001f526 = 0;
        return;
      }
      if (DAT_0001f590 < 10) {
        return;
      }
      if (DAT_0001f584 != 1) {
        return;
      }
      if (DAT_0001f534 == 0xf7) {
        DAT_0001f526 = DAT_0001f526 + 1;
        return;
      }
      return;
    }
    DAT_0001f598 = 0x50;
    DAT_0001f59a = 0;
  }
  DAT_0001f54a = 0x85;
  DAT_0001f58c = 1;
  (*sci1_latch_and_send_f54a_handshake_byte)(DAT_0001f520 & 0x88);
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf526) = 0x80;
  return;
}



// [UPDATED 2026-08-07] Phase-counter (F526) housekeeping + periodic frame
// trigger, called every tick from sci1_protocol_state_machine (0x287af).
// Advances/resets F526 based on F522 bits 5-6 and a stall counter F59a
// (times out after 10 ticks if F534 sits at 0xf8). Every 200 ticks (F598
// countdown), if F526 has reached phase 0x80: sets F58C=15 (frame length),
// calls sci1_periodic_status_frame_build_f54a_f566 (0x28d7d) to PACK the
// 14-byte status frame + checksum into F54A-F566, then calls
// sci1_latch_and_send_f54a_handshake_byte (0x166cd) to start transmitting
// it. This is the periodic (not request-driven) broadcast path, distinct
// from sci1_dispatch_and_latch_response's command-triggered path -- both
// converge on the same F54A staging buffer and the same send/feeder
// mechanism. See mut_verification_status.md "packs and sends" trace.

void f526_state_reset_dispatch(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = g_sci1_rx_status_flags_f522 & 0x60;
  if (uVar1 == 0) goto LAB_00028d4e;
  if ((g_sci1_rx_status_flags_f522 & 0x20) == 0) {
    if (DAT_0001f534 != 0xf8) goto LAB_00028d3b;
  }
  else {
LAB_00028d3b:
    uVar1 = DAT_0001f59a + 1;
    DAT_0001f59a = uVar1;
    if (uVar1 < 10) goto LAB_00028d4e;
  }
  DAT_0001f526 = 0;
LAB_00028d4e:
  if (DAT_0001f598 == 0) {
    DAT_0001f598 = 200;
    if (DAT_0001f526 == 0x80) {
      DAT_0001f58c = 0xf;
      (*sci1_periodic_status_frame_build_f54a_f566)(uVar1);
      (*sci1_latch_and_send_f54a_handshake_byte)();
    }
  }
  *(ushort *)((uint)bVar2 << 0x10 | 0xf522) = *(ushort *)((uint)bVar2 << 0x10 | 0xf522) & 0xff9f;
  return;
}



// [UPDATED 2026-08-07] Builds the 14-byte periodic status/handshake frame at
// F54A-F564 plus a checksum byte at F566 (sum of all 14 words, &0xff). This
// is the "pack" step feeding the autonomous SCI1 TX chain: called only from
// f526_state_reset_dispatch (0x28d23) when phase counter F526==0x80, which
// also sets F58C=15 (frame length) right before calling this, then triggers
// sci1_latch_and_send_f54a_handshake_byte (0x166cd) to start transmission;
// sci1_tx_response_feeder (0x166d7) drains the remaining bytes on later
// ticks by reading a ROM/RAM table at -0xab6 (NOT confirmed to be this
// frame directly -- see NOT YET DONE below).
// 
// Field sources (offset : source : status):
//   F54A : (RAM 0x102c1 != 0) as 0/1                    -- boolean status bit, untraced
//   F54C : packed bitfield from F4B6/F4B8/tcu_rx_confirmed_byte2_f4c0/F4B4
//          -- CONFIRMED link to TCU RX frame fields (see mut_verification_status.md
//          "TCU SERIAL RX FRAME FOUND" section)
//   F54E : raw copy of DAT_0001f18c                      -- TRACED 2026-08-07: written by
//          fueling_inj_pw_calc (0x21c2f, called from fueling_lambda_update). F18C is one
//          of four sibling cells (F186/F188/F18A/F18C) each computed as
//          clamp_u8(inc_if_signed_flag(mul_u16_sat_s16(...))) -- clamped per-cylinder/
//          per-bank injector pulse-width term. NOT renamed -- which of the 4 siblings
//          maps to which bank/cylinder isn't confirmed yet.
//   F550 : raw copy of engine_torque_pct_f17a             -- CONFIRMED/RESOLVED 2026-08-07:
//          renamed from DAT_0001f17a. Written by engine_torque_pct_scale_calc (0x2184b,
//          write at 0x218b8, called from throttle_body_update): computes
//          clamp_u8(div_s32_s16_rounded(F5CA/F5CC-derived torque values)) -- a torque
//          percentage/scale value with NO RPM input anywhere in the computation. Already
//          investigated in an earlier session (logging.txt OPEN ITEM #1): an old "is this
//          RPM?" theory was REFUTED there, torque-based identity CONFIRMED. RPM's real
//          location is still unknown -- do not resume that search here.
//   F552/F554/F55A : ALL THREE DERIVE FROM ONE muldiv_s16_rounded_3op() CALL --
//          CHAIN FULLY TRACED 2026-08-07 (operands found via raw disassembly at
//          0x28dd6-0x28dfb; decompiler hid the pushed args). Call is
//          muldiv_s16_rounded_3op(accum_latch_100tick_f33c, accum_latch_100tick_f33a,
//          ROM_word@0x808, 0x246). The raw result is saturated via passthrough_or_sat(),
//          split hi/lo into F552/F554, and the SAME result is ALSO passed to clamp_u8()
//          for F55A (not an independent field, just a clamped view of the same value).
// 
//          accum_latch_100tick_f33a/f33c (renamed from DAT_0001f33a/DAT_0001f33c) RESOLVED
//          this session: earlier "zero other xrefs" search was checking the wrong EP/bank
//          prefix (0001 instead of 0000) -- same address, different bank tag, real xrefs
//          exist under the 0000-prefixed form. They are written INSIDE
//          tcu_rx_main_scheduler (0x2aa36, write site ~0x2af3e), as a straight latch:
//              DAT_0000f33a = DAT_0000f336;
//              DAT_0000f33c = DAT_0000f338;
//              DAT_0000f336 = 0;
//              DAT_0000f338 = 0;
//          gated on a 100-tick reload timer (EF8E). So F33A/F33C are a SNAPSHOT-AND-RESET
//          of accumulator F336/F338, taken once every 100 scheduler ticks, immediately
//          zeroing the accumulator for the next window -- i.e. F33A/F33C hold "whatever
//          F336/F338 accumulated over the last 100-tick window."
//          F336/F338 themselves are written inside fuel_pw_and_airvol_compute (0x29fba,
//          write site ~0x2a77f) following a mul_u16_hi/sat_add_s16 accumulation sequence
//          near the end of that function's fuel/air computation. Exact physical meaning
//          of the F336/F338 accumulator itself NOT YET fully pinned down this session
//          (fuel_pw_and_airvol_compute is a large function; only the write-site tail was
//          inspected here) -- but strong circumstantial case for an integrated fuel-
//          delivery or airflow quantity given the parent function's scope, consistent
//          with a "per-100-tick-window average/rate" being what ultimately lands in
//          F552/F554/F55A of this frame. ROM word @0x808 and constant 0x246 are a
//          fixed scale/divisor pair for that averaging -- ROM@0x808 not readable from
//          this context (likely EP/bank-relative), not resolved.
//   F556 : raw copy of mut_battery_voltage_f13a           -- CONFIRMED 2026-08-07: this is
//          MUT Battery voltage, RequestID 0x14 (see adc_read_sequence_b @0x15689 plate
//          comment, already verified in an earlier session against the MUT table @0x2fad0).
//          Renamed DAT_0001f13a -> mut_battery_voltage_f13a.
//   F558 : raw copy of coolant_temp_scaled_f130            -- CONFIRMED 2026-08-07: renamed
//          from DAT_0001f130. This is the validated/scaled coolant temperature from the
//          F84E->F12E->F130->F29A chain documented in coolant_temp_validity_and_scale's
//          plate comment (0x21414). NOTE per that comment this path does NOT reconcile
//          with the MUT profile's claimed coolant temp address (F109/F10F) -- still an
//          open reconciliation question, but F130 itself is a real, traced coolant value.
//   F55C : raw copy of DAT_0001f10e                       -- TRACED 2026-08-07: written by
//          calc_f110_f10e_via_table (0x21314, called from map_tps_axis_update and
//          post_init_status_compose_and_ef98_signal). Table/axis-interpolation output,
//          paired with F110/F112, using F10A/F10C as axis inputs -- consistent with a
//          MAP or TPS-derived interpolated load value. NOT renamed yet -- need to confirm
//          which axis/table before committing a name. Note a sibling function
//          calc_load_correction_f10e (0x199f0) also exists and touches F10E-adjacent
//          territory -- not yet cross-checked against this writer, worth a look.
//   F55E/F560/F562/F564 : always zero (reserved/padding, 4 words)
//   F566 : checksum = sum(F54A..F564 as words) & 0xff
// 
// SUMMARY: ALL 14 fields now traced to a concrete source. With F550=Engine
// TorquePct(CONFIRMED), F556=Battery(CONFIRMED), F558=CoolantTemp
// (CONFIRMED), F54E=InjectorPW(traced), F552/554/55A=100-tick-averaged
// fuel/air accumulator from fuel_pw_and_airvol_compute(traced, physical
// meaning of the underlying F336/F338 accumulator itself still open), and
// F55C=table/axis output(traced), this frame is a compact PERIODIC SNAPSHOT
// of key engine channels -- not a generic handshake broadcast, and not the
// request-driven Mode-2 MUT-table path (adc_sensor_convert_single/0x2fad0),
// but a fixed-format bundle reusing real, independently-confirmed sensor/
// MUT cells, including one (Battery, ReqID 0x14) with a direct known MUT
// identity. Sent automatically every 200 ticks once phase counter F526
// reaches 0x80.
// 
// NOT YET DONE:
// - Pin down the exact physical meaning of the F336/F338 accumulator inside
//   fuel_pw_and_airvol_compute (only the write-site tail was inspected;
//   the full function is large and multi-purpose).
// - Read/resolve ROM word @0x808 (not accessible from this context directly
//   -- likely bank/EP-relative) and confirm the 0x246 scale constant's role.
// - Confirm exact identity/name for F18C (which injector PW sibling) and
//   F10E (which table/axis; check calc_load_correction_f10e for overlap).
// - Confirm/deny whether sci1_tx_response_feeder's -0xab6 table is this
//   same frame or a separate buffer.
// - Trace the actual Mode-2 MUT RequestID response path (via
//   adc_sensor_convert_single) onto the wire -- still not found, separate
//   from this periodic-frame chain.
// - REMINDER FOR FUTURE SESSIONS: when a static xref search shows "zero
//   other references" for a cell, double check the EP/bank-address PREFIX
//   (0000 vs 0001, as seen here and with F17A) before concluding the writer
//   doesn't exist -- it may just be indexed under a different bank tag for
//   the same physical RAM address.

void sci1_periodic_status_frame_build_f54a_f566(void)

{
  ushort uVar1;
  undefined2 uVar2;
  ushort value;
  byte bVar3;
  
  bVar3 = 1;
  DAT_0001f54a = (ushort)(cal_fuel_cut_enable_flag != '\0');
  DAT_0001f54c = (DAT_0001f4b6 & 4) << 1 |
                 (DAT_0001f4b8 & 0x80) >> 1 |
                 DAT_0001f4b8 & 0x30 | tcu_rx_confirmed_byte2_f4c0 & 7 | DAT_0001f4b4 & 0x80;
  DAT_0001f54e = inj_pw_output_f18c;
  DAT_0001f550 = engine_torque_pct_f17a;
  uVar1 = (*muldiv_s16_rounded_3op)();
  uVar1 = passthrough_or_sat(uVar1,value);
  *(ushort *)((uint)bVar3 << 0x10 | 0xf552) = uVar1 >> 8;
  *(ushort *)((uint)bVar3 << 0x10 | 0xf554) = uVar1 & 0xff;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf556) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf13a);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf558) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf130);
  uVar2 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf55a) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf55c) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf10e);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf55e) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf560) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf562) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf564) = 0;
  *(ushort *)((uint)bVar3 << 0x10 | 0xf566) =
       *(short *)((uint)bVar3 << 0x10 | 0xf54a) + *(short *)((uint)bVar3 << 0x10 | 0xf54c) +
       *(short *)((uint)bVar3 << 0x10 | 0xf54e) + *(short *)((uint)bVar3 << 0x10 | 0xf550) +
       *(short *)((uint)bVar3 << 0x10 | 0xf552) + *(short *)((uint)bVar3 << 0x10 | 0xf554) +
       *(short *)((uint)bVar3 << 0x10 | 0xf556) + *(short *)((uint)bVar3 << 0x10 | 0xf558) +
       *(short *)((uint)bVar3 << 0x10 | 0xf55a) + *(short *)((uint)bVar3 << 0x10 | 0xf55c) +
       *(short *)((uint)bVar3 << 0x10 | 0xf55e) + *(short *)((uint)bVar3 << 0x10 | 0xf560) +
       *(short *)((uint)bVar3 << 0x10 | 0xf562) + *(short *)((uint)bVar3 << 0x10 | 0xf564) & 0xff;
  return;
}



void isc_f35c_correction_calc(void)

{
  ushort uVar1;
  byte bVar2;
  
  bVar2 = 1;
  uVar1 = sat_sub_u16(0x80,0);
  *(ushort *)((uint)bVar2 << 0x10 | 0xf35c) = uVar1;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f430_clear(void)

{
  DAT_0001f430 = 0;
  return 0;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ushort f432_clear(void)

{
  DAT_0001f432 = 0;
  return 0;
}



void stub_empty_0x28f80(void)

{
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void o2_wideband_state_cold_init(void)

{
  DAT_0001f5f2 = 5;
  DAT_0001f384 = 0xffff;
  DAT_0001f3d0 = 0;
  DAT_0001f3da = DAT_00011028;
  DAT_0001f3d8 = DAT_00011028;
  DAT_0001f3d6 = DAT_00011028;
  DAT_0001f3dc = 0xffff;
  DAT_0001f5ea = 3;
  DAT_0001f500 = DAT_00010f80;
  DAT_0001f5e6 = 0xffff;
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void o2_wideband_state_warm_init(void)

{
  DAT_0001f5d0 = 0xffff;
  DAT_0001f5ce = 0xffff;
  DAT_0001f5cc = 0xffff;
  DAT_0001f5ca = 0xffff;
  DAT_0001f5f2 = 5;
  DAT_0001f5ee = 0;
  DAT_0001f5ea = 3;
  DAT_0001f500 = DAT_00010f80;
  DAT_0001f384 = 0xffff;
  DAT_0001f3d0 = 0;
  DAT_0001f340 = DAT_0001f340 & 0xff7f;
  DAT_0001f2b0 = DAT_00010aa0;
  return;
}



// WARNING: Removing unreachable block (ram,0x00029aa4)
// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly + bank-prefixed xrefs]
//    tcu_shift_torque_and_knock_mgmt (0x28fff-0x29c31): confirmed live as ONE merged function
//    matching logging.txt's "MAJOR STRUCTURAL FIX" claim (Ghidra's earlier mis-split into
//    tcu_shift_torque_mgmt/knock_process_main etc. is NOT present in current analysis - this
//    is a single function from 0x28fff to 0x29c31, prts at 0x29c31).
//    
//    KNOCK CLUSTER - all items re-verified via direct xrefs this pass:
//    - Knock Sum (0xF3A2/F3A3, RequestID 0x26): CONFIRMED. Writes @0x2916f/0x29177/0x298dd/
//      0x298f3/0x298fd/0x29a3a/0x29a44, gated by 0xF1F2 bit7 (confirmed @0x297e3).
//    - knock_adc (0xF3A8/A9, RequestID 0x6A -> table 0xF3A9): CONFIRMED, write @0x29849.
//    - knock_var (0xF962/963, RequestID 0x6C -> table 0xF963): CONFIRMED, write @0x29a48.
//    - knock_base (0xF966/967, RequestID 0x6B -> table 0xF967): CONFIRMED, write @0x29a58.
//    - knock_change (0xF968/969, RequestID 0x6D -> table 0xF969): CONFIRMED, writes @0x298ae/
//      0x298b8. NOTE: address is F968/969, distinct from the F962-966 cluster - do not confuse
//      with knock_var/base despite proximity.
//    - knock_dynamics (0xF3B6/B7, RequestID 0x6E -> table 0xF3B7): CONFIRMED, writes @0x29886/
//      0x298aa/0x298b4.
//    All 6 knock RequestIDs (Sum/ADC/Var/Base/Change/Dynamics) now fully re-verified under
//    current decoder - logging.txt CONFIRMED-section claims hold.
//    
//    Knock Voltage (0xF15C/F15D, MUT RequestID 0x30): PARTIALLY VERIFIED ONLY.
//    Read confirmed at 0x297ee, immediately after the 0xF1F2 bit7 gate. HOWEVER: no writer
//    found for 0xF15C/F15D under bank prefixes 0x0-0x3. logging.txt's "written by knock/TCU
//    handler" claim NOT CONFIRMED for the write side. Needs further investigation.
//    
//    TCU Shift Torque Cmd Lo/Hi (ROM 0x1328e/0x13296, gear-indexed via (gear&3)<<1 where gear
//    comes from 0xF3CC&3): CONFIRMED. @0x290a8/0x290b2 reads ROM 0x328e-offset table, writes
//    RAM 0xF39E; @0x290b6/0x290ba reads ROM 0x3296-offset table, writes RAM 0xF3A0. ROM bytes
//    at 0x1328e verified as [00 02 00 01 00 02 00 01 00 01 00 02 00 01 00 02] - matches
//    logging.txt's claim of small values (1-2), confirmed NOT a data error. SCI3 TX usage
//    claim not independently re-traced this pass.
// 
// [ADDED 2026-07-13, RPM investigation continued from isr_ipu_ch2ch4_input_capture / 0x168e3]
//    Consumes g_ipu_ch2ch4_period_delta_raw (0xF5DE) two separate ways - do not conflate:
//    1) F5DE copied directly to F384, then pushed through a 4-deep shift register each call:
//         F384 -> candidate_rpm_period_hist_newest(F5CA) -> F5CC -> F5CE -> F5D0
//       candidate_rpm_period_hist_newest is then divided by fixed constant 0x1D4 (468) via
//       div_s32_s16_rounded, and the result is run through axis_interp_lerp_u8frac_256 TWICE,
//       writing candidate_rpm_axis_bkpt_lo (0xF180) and 0xF182. This shape (divide by fixed
//       constant -> table-axis interpolation) matches an RPM/engine-speed breakpoint lookup,
//       but is NOT yet independently confirmed by name/units - treat "rpm" in these labels as
//       a working hypothesis, not fact. Next step to confirm: pull the ROM axis-table bytes
//       feeding this interpolation and check for round-number RPM-shaped breakpoints.
//    2) Separately, F5DE>>3 accumulates into F4EE (gated by flag byte @0x2eb), later scaled via
//       mul_u16_hi(40000, F4DC-related) then div_s32_s16_rounded into F4F4. This is a DIFFERENT
//       derived value from (1) above - likely a smoothed/averaged metric, possibly duty-cycle
//       or dwell-adjacent rather than RPM. Do not merge this thread with the F5CA/axis-interp
//       one when writing up conclusions.

void tcu_shift_torque_and_knock_mgmt
               (undefined2 param_1,undefined2 param_2,ushort param_3,undefined2 param_4,
               ushort param_5)

{
  ushort *puVar1;
  short *psVar2;
  undefined1 extraout_var;
  short sVar3;
  ushort uVar4;
  ushort uVar5;
  undefined2 uVar6;
  char cVar7;
  short extraout_R2;
  ushort uVar8;
  ushort extraout_R2_00;
  ushort uVar9;
  short sVar10;
  ushort unaff_R5;
  byte bVar11;
  ushort in_SR;
  ushort uVar12;
  byte in_CF;
  byte in_VF;
  byte in_ZF;
  byte in_NF;
  
  uVar12 = (((in_SR & 0xfffe | (ushort)in_CF) & 0xfffd | (ushort)in_VF << 1) & 0xfffb |
           (ushort)in_ZF << 2) & 0xfff7 | (ushort)in_NF << 3;
  bVar11 = 0;
  sVar3 = DAT_0000f5e6;
  DAT_0000f5e6 = sVar3 << 1;
  uVar8 = unaff_R5;
  sVar3 = (*check_flag_fe87_bit1_clear)();
  if (sVar3 != 0) {
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf5e6);
    *puVar1 = *puVar1 | 1;
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf5ee) & 1) == 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf5e8) = 1;
  }
  else {
    psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf5e8);
    *psVar2 = *psVar2 + 1;
    if (3 < *(ushort *)((uint)bVar11 << 0x10 | 0xf5e8)) {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf5e8) = 0;
    }
  }
  uVar5 = *(ushort *)((int)(*(short *)((uint)bVar11 << 0x10 | 0xf5e8) << 1) + 0x3284) ^
          *(ushort *)((uint)bVar11 << 0x10 | 0xf5e6);
  sVar3 = (*check_flag_f0d8_bit0_clear)();
  if (sVar3 == 0) {
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0x328c) & uVar5) == 0) {
      if ((uVar5 & 0xf) == 0) {
        puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf5ee);
        *puVar1 = *puVar1 | 1;
      }
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf5f0) = 2;
    }
    else if (*(short *)((uint)bVar11 << 0x10 | 0xf5f0) == 0) {
      uVar5 = *(ushort *)((uint)bVar11 << 0x10 | 0xf5e6) & 0xf;
      if ((uVar5 != 0) && (uVar5 != 0xf)) goto LAB_00029088;
    }
    else {
      psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf5f0);
      *psVar2 = *psVar2 + -1;
    }
  }
  else {
LAB_00029088:
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf5ee);
    *puVar1 = *puVar1 & 0xfffe;
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf5ee) & 1) == 0) {
    uVar5 = *(ushort *)((uint)bVar11 << 0x10 | 0xf5e6);
  }
  else {
    uVar5 = *(ushort *)((int)(*(short *)((uint)bVar11 << 0x10 | 0xf5e8) << 1) + 0x3284);
  }
  sVar3 = (uVar5 & 3) << 1;
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf39e) = *(undefined2 *)((int)sVar3 + 0x328e);
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3a0) = *(undefined2 *)((int)sVar3 + 0x3296);
  *(short *)((uint)bVar11 << 0x10 | 0xf3d2) = *(short *)((uint)bVar11 << 0x10 | 0xf3d2) << 1;
  sVar3 = (*check_flag_fe8a_bit1_set)();
  if (sVar3 != 0) {
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3d2);
    *puVar1 = *puVar1 | 1;
  }
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf384) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf5de);
  uVar5 = *(ushort *)((uint)bVar11 << 0x10 | 0xf384);
  uVar9 = (*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 5) << 1;
  uVar4 = uVar9 | 0x35;
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf33e) & 0x80) != 0) {
    uVar4 = uVar9 | 0x75;
  }
  if (*(ushort *)((uint)bVar11 << 0x10 | 0xad2) <= uVar5) {
    uVar9 = uVar4 & 0xffdf;
    if (0x1080 < uVar5) {
      uVar9 = uVar4 & 0xffdb;
    }
    uVar4 = uVar9;
    if (0x1305 < uVar5) {
      uVar4 = uVar4 & 0xffef;
    }
    uVar9 = 0x3a00;
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 1) != 0) {
      uVar9 = 0x4900;
    }
    if (uVar9 <= uVar5) {
      uVar4 = uVar4 & 0xfffe;
    }
  }
  *(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) = uVar4;
  puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3ce);
  *puVar1 = *puVar1 & 0xfffe;
  if (*(short *)((uint)bVar11 << 0x10 | 0xefa4) == 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3cc) = 0;
    uVar5 = unaff_R5;
LAB_0002935d:
    tcu_shift_state_reset(uVar12);
  }
  else {
    sVar3 = (*check_flag_f0d8_bit0_clear)();
    if (((sVar3 == 0) && ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 0x40) == 0)) &&
       ((*(ushort *)((uint)bVar11 << 0x10 | 0xf5ee) & 1) != 0)) {
      uVar9 = *(ushort *)((uint)bVar11 << 0x10 | 0xf3a2);
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xb1a);
      if (*puVar1 <= uVar9 && uVar9 != *puVar1) {
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3a2) = 0;
      }
      uVar9 = *(ushort *)((uint)bVar11 << 0x10 | 0xf3a2);
      (*o2_wideband_mode_zero_check)();
      if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf33e) & 8) != 0) &&
         ((*(ushort *)((uint)bVar11 << 0x10 | 0xf5ee) & 1) != 0)) {
        if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf33e) & 0x20) == 0) {
          bVar11 = 0;
        }
        else {
          bVar11 = 0;
        }
      }
      uVar9 = o2_wideband_cell_offset_adjust(uVar9);
      if (*(ushort *)((uint)bVar11 << 0x10 | 0xad4) <= uVar9) {
        uVar9 = *(ushort *)((uint)bVar11 << 0x10 | 0xad4);
      }
      uVar9 = uVar9 + 0x2a;
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf38c);
      if (uVar9 < *puVar1 || uVar9 == *puVar1) {
        unaff_R5 = *(ushort *)((uint)bVar11 << 0x10 | 0xf38e);
        uVar12 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf38c),
                             *(ushort *)((uint)bVar11 << 0x10 | 0xad0));
        if (uVar9 < uVar12) {
          *(ushort *)((uint)bVar11 << 0x10 | 0xf38c) = uVar12;
        }
        else {
          *(ushort *)((uint)bVar11 << 0x10 | 0xf38c) = uVar9;
        }
        if (uVar9 < *(ushort *)((uint)bVar11 << 0x10 | 0xf38e)) {
          uVar12 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf38e),
                               *(ushort *)((uint)bVar11 << 0x10 | 0xad0));
          if (uVar9 < uVar12) {
            *(ushort *)((uint)bVar11 << 0x10 | 0xf38e) = uVar12;
          }
          else {
            *(ushort *)((uint)bVar11 << 0x10 | 0xf38e) = uVar9;
          }
        }
        else {
          uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf38c);
          *(ushort *)((uint)bVar11 << 0x10 | 0xf38e) = uVar12;
        }
        uVar9 = o2_wideband_midpoint_select(uVar12,0xe);
        uVar12 = 0x9258;
        param_3 = unaff_R5;
        uVar9 = sat_sub_u16(unaff_R5,uVar9);
        puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf38c);
        if (*puVar1 <= uVar9 && uVar9 != *puVar1) {
          *(ushort *)((uint)bVar11 << 0x10 | 0xf38c) = uVar9;
        }
      }
      else {
        uVar4 = *(short *)((uint)bVar11 << 0x10 | 0xf38c) + *(short *)((uint)bVar11 << 0x10 | 0xace)
        ;
        if (uVar4 < uVar9) {
          uVar9 = uVar4;
        }
        *(ushort *)((uint)bVar11 << 0x10 | 0xf38e) = uVar9;
        *(ushort *)((uint)bVar11 << 0x10 | 0xf38c) = uVar9;
      }
      *(short *)((uint)bVar11 << 0x10 | 0xf364) =
           (short)&UNK_0000ffd6 + *(short *)((uint)bVar11 << 0x10 | 0xf38c);
    }
    else {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf38c) = 0xca;
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf38e) = 0xca;
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf364) = 0xa0;
    }
    sVar3 = (*check_flag_f0d8_bit0_clear)();
    if (((sVar3 != 0) && (*(ushort *)((uint)bVar11 << 0x10 | 0xf13a) < 0xff)) ||
       ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 1) == 0)) {
LAB_00029356:
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3cc) = 0;
      uVar5 = unaff_R5;
      goto LAB_0002935d;
    }
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 2) == 0) {
      uVar12 = 0x92ac;
      sVar3 = (*tcu_f384_f388_hysteresis_gate_check)();
      if (sVar3 == 0) {
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3cc) = 1;
        uVar5 = unaff_R5;
        goto LAB_0002935d;
      }
      goto LAB_00029356;
    }
    uVar9 = *(ushort *)((uint)bVar11 << 0x10 | 0xf388);
    if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 0x20) == 0) && (uVar5 < 0x1d4c)) {
      sVar3 = 5;
      uVar4 = uVar5;
      do {
        uVar4 = uVar4 >> 1;
        if (uVar4 == 0) break;
        sVar3 = sVar3 + -1;
      } while (sVar3 != -1);
      if (uVar5 < uVar9) {
        if (uVar9 - uVar5 < uVar4) goto LAB_00029330;
        uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xac8);
        uVar4 = (*mul_u16_x2_sat_alt)();
        uVar9 = uVar5;
        uVar5 = sat_sub_u16(uVar5,uVar4);
      }
      else {
        if (uVar5 - uVar9 < uVar4) goto LAB_00029330;
        uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xaca);
        uVar4 = (*mul_u16_x2_sat_alt)();
        uVar9 = uVar5;
        uVar5 = sat_add_u16(uVar5,uVar4);
      }
    }
    else {
LAB_00029330:
      uVar9 = uVar5;
    }
    *(ushort *)((uint)bVar11 << 0x10 | 0xf38a) = uVar5;
    unaff_R5 = uVar5;
    if ((0x186a < uVar9) && (*(ushort *)((uint)bVar11 << 0x10 | 0xf38c) < 0xcb)) {
      uVar12 = 0x9349;
      sVar3 = (*tcu_f384_f388_hysteresis_gate_check)();
      if (sVar3 != 0) goto LAB_00029356;
    }
    if (((uVar5 & 0x8000) != 0) || (uVar5 < 0x146)) goto LAB_00029356;
    uVar12 = muldiv_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf38c),uVar5,0x200);
    uVar12 = sat_sub_u16(uVar12,param_3);
    *(ushort *)((uint)bVar11 << 0x10 | 0xf390) = uVar12;
    uVar12 = muldiv_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf38e),uVar5,0x200);
    uVar9 = 0x93ab;
    uVar12 = sat_sub_u16(uVar12,uVar8);
    *(ushort *)((uint)bVar11 << 0x10 | 0xf392) = uVar12;
    uVar12 = (*compose_status_bits_fe86_fe8a)();
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf39e) & uVar12) == 0) {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3d0) = 2;
      if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 0xc) == 0xc) {
        sVar3 = *(short *)((uint)bVar11 << 0x10 | 0xf382);
      }
      else if (uVar5 < 0x1130) {
        sVar3 = *(short *)((uint)bVar11 << 0x10 | 0xf380);
      }
      else {
        uVar12 = (short)&isc_stepper_position_eed0 + uVar5 >> 4;
        if (0xca < *(ushort *)((uint)bVar11 << 0x10 | 0xf38c)) {
          uVar12 = muldiv_u16((short)&T1GR4H + *(short *)((uint)bVar11 << 0x10 | 0xf38c),uVar5,0x400
                             );
          if (uVar12 < 0x100) {
            uVar12 = extraout_R2 + uVar12;
          }
          else {
            uVar12 = extraout_R2 + 0x100;
          }
        }
        sVar3 = uVar12 + *(short *)((uint)bVar11 << 0x10 | 0xf380);
      }
      sVar3 = (*(short *)((uint)bVar11 << 0x10 | 0xf5e2) + *(short *)((uint)bVar11 << 0x10 | 0xf390)
              ) - sVar3;
      sVar10 = *(short *)((uint)bVar11 << 0x10 | 0xf398) + *(short *)((uint)bVar11 << 0x10 | 0xad8);
      if ((sVar3 - sVar10 & 0x8000U) != 0) {
        sVar3 = sVar10;
      }
      if ((sVar3 - *(short *)((uint)bVar11 << 0x10 | 0xff46) & 0x8000U) != 0) {
        sVar3 = *(short *)((uint)bVar11 << 0x10 | 0xff46);
      }
      *(short *)((uint)bVar11 << 0x10 | 0xf396) = sVar3;
      (*sci3_timeout_set_abort)();
    }
    else {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3d0) = 1;
      (*sci3_timeout_clear_flag)();
    }
    *(short *)((uint)bVar11 << 0x10 | 0xf398) =
         *(short *)((uint)bVar11 << 0x10 | 0xf5e2) + *(short *)((uint)bVar11 << 0x10 | 0xf390);
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf38c) < 0xca) ||
       (((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 0x40) == 0 &&
        ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 0x10) != 0)))) {
      if (*(short *)((uint)bVar11 << 0x10 | 0xf3d0) != 2) {
        if ((*(short *)((uint)bVar11 << 0x10 | 0xf398) - *(short *)((uint)bVar11 << 0x10 | 0xff46) &
            0x8000U) != 0) {
          *(undefined2 *)((uint)bVar11 << 0x10 | 0xf398) =
               *(undefined2 *)((uint)bVar11 << 0x10 | 0xff46);
        }
        (*sci3_timeout_set_retry)();
      }
    }
    else {
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3ce);
      *puVar1 = *puVar1 | 1;
    }
    if (*(ushort *)((uint)bVar11 << 0x10 | 0xf38c) < 200) {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf394) = 0;
    }
    else if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3cc) & 0x40) == 0) {
      uVar12 = muldiv_u16((short)&T1GR3L + *(short *)((uint)bVar11 << 0x10 | 0xf38c),uVar5,0x200);
      uVar12 = sat_sub_u16(uVar12,uVar9);
      if (uVar12 == 0) {
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf394) = 1;
      }
      else {
        *(ushort *)((uint)bVar11 << 0x10 | 0xf394) = uVar12;
      }
    }
    else {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf394) = 0x32;
    }
    sVar3 = (uVar5 + *(short *)((uint)bVar11 << 0x10 | 0xf5e2) +
            *(short *)((uint)bVar11 << 0x10 | 0xf392)) - *(short *)((uint)bVar11 << 0x10 | 0xf382);
    sVar10 = *(short *)((uint)bVar11 << 0x10 | 0xf398) + *(short *)((uint)bVar11 << 0x10 | 0xad8);
    if ((sVar3 - sVar10 & 0x8000U) != 0) {
      sVar3 = sVar10;
    }
    *(short *)((uint)bVar11 << 0x10 | 0xf39a) = sVar3;
  }
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf388) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf384);
  uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf384);
  sVar3 = 6;
  do {
    uVar12 = uVar12 >> 1;
    if (uVar12 == 0) break;
    sVar3 = sVar3 + -1;
  } while (sVar3 != -1);
  *(ushort *)((uint)bVar11 << 0x10 | 0xf39c) = uVar12;
  bVar11 = 0;
  g_tcu_shift_timing_state_flag_efa4 = 0x33;
  uVar6 = DAT_0000f5ce;
  DAT_0000f5d0 = uVar6;
  uVar6 = DAT_0000f5cc;
  DAT_0000f5ce = uVar6;
  uVar6 = candidate_rpm_period_hist_newest;
  DAT_0000f5cc = uVar6;
  uVar6 = DAT_0000f384;
  candidate_rpm_period_hist_newest = uVar6;
  uVar6 = candidate_rpm_period_hist_newest;
  uVar6 = (*div_s32_s16_rounded)();
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 1) == 0) {
    uVar6 = (*axis_interp_lerp_u8frac_256)();
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf180) = uVar6;
    uVar6 = (*axis_interp_lerp_u8frac_256)();
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf182) = uVar6;
  }
  else {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf180) = uVar6;
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf182) = uVar6;
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf36c) != 0) {
    psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf36c);
    *psVar2 = *psVar2 + -1;
  }
  if ((*(short *)((uint)bVar11 << 0x10 | 0xf36c) == 0) &&
     (*(ushort *)((uint)bVar11 << 0x10 | 0xf36a) < 0x80)) {
    psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf36a);
    *psVar2 = *psVar2 + 1;
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf36c) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xafe);
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf368) != 0) {
    psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf368);
    *psVar2 = *psVar2 + -1;
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf368) == 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf366) = 0;
  }
  uVar12 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf376),
                       *(ushort *)((uint)bVar11 << 0x10 | 0xe60));
  *(ushort *)((uint)bVar11 << 0x10 | 0xf376) = uVar12;
  if (*(short *)((uint)bVar11 << 0x10 | 0xf370) != 0) {
    psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf370);
    *psVar2 = *psVar2 + -1;
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf372) != 0) {
    psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf372);
    *psVar2 = *psVar2 + -1;
  }
  if ((*(short *)((uint)bVar11 << 0x10 | 0xf370) == 0) &&
     (*(short *)((uint)bVar11 << 0x10 | 0xf372) == 0)) {
    if (*(ushort *)((uint)bVar11 << 0x10 | 0xf36e) < 0x80) {
      psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf36e);
      *psVar2 = *psVar2 + 1;
    }
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf372) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xbd8);
  }
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2d8) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2d6);
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2d6) = 0x3c0;
  sVar3 = *(short *)((uint)bVar11 << 0x10 | 0xf2b4) + 1;
  *(short *)((uint)bVar11 << 0x10 | 0xf2b4) = sVar3;
  if (sVar3 == 0) {
    psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf2b4);
    *psVar2 = *psVar2 + -1;
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 0x800) != 0) {
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf21c) & 0x40) == 0) {
      uVar12 = sat_add_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf26e),
                           *(short *)((uint)bVar11 << 0x10 | 0xf264) << 2);
    }
    else {
      uVar12 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf26e),
                           *(short *)((uint)bVar11 << 0x10 | 0xf262) << 2);
    }
    uVar12 = clamp_u16(uVar12,*(ushort *)((uint)bVar11 << 0x10 | 0xf276),
                       *(ushort *)((uint)bVar11 << 0x10 | 0xf27a));
    *(ushort *)((uint)bVar11 << 0x10 | 0xf26e) = uVar12;
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 0x200) != 0) {
    fuel_pw_and_airvol_compute();
  }
  if (*(char *)((uint)bVar11 << 0x10 | 0x2eb) != '\0') {
    *(ushort *)((uint)bVar11 << 0x10 | 0xf4ee) =
         *(short *)((uint)bVar11 << 0x10 | 0xf4ee) +
         (*(ushort *)((uint)bVar11 << 0x10 | 0xf5de) >> 3);
    if (*(short *)((uint)bVar11 << 0x10 | 0xf4f2) != 0) {
      psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf4f2);
      *psVar2 = *psVar2 + -1;
    }
    if (*(short *)((uint)bVar11 << 0x10 | 0xf4f2) == 0) {
      uVar6 = scale_counter_ff96_to_byte_clamped();
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf4e4) = uVar6;
      uVar12 = (*muldiv_s16_rounded)();
      if (0xfe < uVar12) {
        uVar12 = 0xff;
      }
      *(ushort *)((uint)bVar11 << 0x10 | 0xf4e6) = uVar12;
      if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 1) == 0) {
        uVar12 = (*axis_interp_lerp_u8frac_256)();
        *(ushort *)((uint)bVar11 << 0x10 | 0xf4e8) = uVar12;
        cVar7 = round_high_byte_signed(uVar12);
        *(ushort *)((uint)bVar11 << 0x10 | 0xf4ea) = CONCAT11(extraout_var,cVar7);
      }
      else {
        *(ushort *)((uint)bVar11 << 0x10 | 0xf4e8) = uVar12 << 8;
        *(ushort *)((uint)bVar11 << 0x10 | 0xf4ea) = uVar12;
      }
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf4ee) = 0;
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf4f2) =
           *(undefined2 *)((uint)bVar11 << 0x10 | 0xf4f0);
    }
    sVar3 = -1;
    if (*(ushort *)((uint)bVar11 << 0x10 | 0xf4dc) < 0xff) {
      (*mul_u16_hi)();
      sVar3 = (*div_s32_s16_rounded)();
    }
    if (sVar3 == 0) {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf4f4) = 1;
    }
    else {
      *(short *)((uint)bVar11 << 0x10 | 0xf4f4) = sVar3;
    }
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf0ec);
    *puVar1 = *puVar1 | 1;
    (*update_flag_fe82_bit2_cond)();
  }
  (*stub_empty_0x2c129)();
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf1f2) & 0x80) == 0) {
    return;
  }
  (*adc_read_channel_6)();
  uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf15c);
  if (uVar12 < 0x65) {
    if (uVar12 < 0x5d) {
      uVar12 = (*mul_fixedpoint_8)();
    }
    else {
      sVar3 = (*mul_fixedpoint_8)();
      uVar12 = sVar3 + 0x31;
    }
  }
  else {
    sVar3 = (*mul_fixedpoint_8)();
    uVar12 = sVar3 + 0x37;
  }
  if (0xfe < uVar12) {
    uVar12 = 0xff;
  }
  uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf3a8);
  *(ushort *)((uint)bVar11 << 0x10 | 0xf3a8) = uVar12;
  if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf3a4) & 0x400) == 0) ||
     ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3a6) & 1) != 0)) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3b6) = 0;
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf968) = 0;
  }
  else {
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3a8);
    if (uVar8 < *puVar1 || uVar8 == *puVar1) {
      uVar8 = 0;
    }
    else {
      uVar8 = uVar8 - *(short *)((uint)bVar11 << 0x10 | 0xf3a8);
    }
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf0ec) & 2) == 0) {
      uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf8a);
    }
    else {
      uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf8c);
    }
    if (uVar8 < uVar12) {
      if (*(short *)((uint)bVar11 << 0x10 | 0xf3b6) != -1) {
        psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf3b6);
        *psVar2 = *psVar2 + 1;
      }
      if (*(ushort *)((uint)bVar11 << 0x10 | 0xf8e) <= *(ushort *)((uint)bVar11 << 0x10 | 0xf3b6)) {
        puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf200);
        *puVar1 = *puVar1 | 1;
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3b6) =
             *(undefined2 *)((uint)bVar11 << 0x10 | 0xf8e);
      }
    }
    else {
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf200);
      *puVar1 = *puVar1 & 0xfffe;
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3b6) = 0;
    }
    *(ushort *)((uint)bVar11 << 0x10 | 0xf968) = uVar8;
  }
  init_state_block_ff54();
  uVar12 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf3a8),
                       *(ushort *)((uint)bVar11 << 0x10 | 0xf3aa));
  *(ushort *)((uint)bVar11 << 0x10 | 0xf3b0) = uVar12;
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3a4) & 0x40) == 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3a2) = 0;
  }
  else {
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf200) & 1) == 0) {
      if (*(short *)((uint)bVar11 << 0x10 | 0xf3b0) == 0) {
        uVar12 = 0;
      }
      else {
        sVar3 = (*muldiv_s16_rounded)();
        uVar12 = sVar3 + 1;
        if (0xfe < uVar12) {
          uVar12 = 0xff;
        }
      }
      if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3a4) & 0x200) == 0) {
        (*tcu_torque_state_reset)();
        uVar12 = extraout_R2_00;
      }
      else {
        psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf3be);
        *psVar2 = *psVar2 + 1;
        if (*(ushort *)((uint)bVar11 << 0x10 | 0xf3ba) <= *(ushort *)((uint)bVar11 << 0x10 | 0xf3be)
           ) {
          *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3be) =
               *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3ba);
          *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3c4) = 0x8000;
        }
        psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf3c0);
        *psVar2 = *psVar2 + 1;
        if (*(ushort *)((uint)bVar11 << 0x10 | 0xf3ba) <= *(ushort *)((uint)bVar11 << 0x10 | 0xf3c0)
           ) {
          *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3c0) =
               *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3ba);
          *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3c6) = 0x8000;
        }
        *(short *)((uint)bVar11 << 0x10 | 0xf3ca) = *(short *)((uint)bVar11 << 0x10 | 0xf3ca) << 1;
        if ((*(ushort *)((uint)bVar11 << 0x10 | 0xb4e) <= uVar12) &&
           (puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3bc),
           uVar12 < *puVar1 || uVar12 == *puVar1)) {
          puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3ca);
          *puVar1 = *puVar1 | 1;
          if (*(short *)((uint)bVar11 << 0x10 | 0xf5ea) == *(short *)((uint)bVar11 << 0x10 | 0xf3c4)
             ) {
            *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3be) = 0;
          }
          else {
            if (*(short *)((uint)bVar11 << 0x10 | 0xf5ea) !=
                *(short *)((uint)bVar11 << 0x10 | 0xf3c6)) {
              if ((*(char *)((uint)bVar11 << 0x10 | 0x2d0) == '\0') ||
                 ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3ca) & 0x10) != 0)) {
                if (*(short *)((uint)bVar11 << 0x10 | 0xf3c4) == -0x8000) {
                  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3c4) =
                       *(undefined2 *)((uint)bVar11 << 0x10 | 0xf5ea);
                  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3be) = 0;
                  cVar7 = *(char *)((uint)bVar11 << 0x10 | 0x2d0);
                }
                else {
                  if (*(short *)((uint)bVar11 << 0x10 | 0xf3c6) != -0x8000) goto LAB_00029a10;
                  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3c6) =
                       *(undefined2 *)((uint)bVar11 << 0x10 | 0xf5ea);
                  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3c0) = 0;
                  cVar7 = *(char *)((uint)bVar11 << 0x10 | 0x2d0);
                }
                if (cVar7 != '\0') goto LAB_00029a08;
              }
              goto LAB_00029a10;
            }
            *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3c0) = 0;
          }
LAB_00029a08:
          uVar12 = 0;
        }
      }
LAB_00029a10:
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3b4) =
           *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3b2);
      if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3a4) & 0x80) == 0) {
        uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xb0e);
      }
      else {
        uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xb0c);
      }
      if (uVar8 <= uVar12) {
        uVar12 = uVar8;
      }
      uVar8 = *(short *)((uint)bVar11 << 0x10 | 0xf3a2) + uVar12;
      if (uVar8 < *(ushort *)((uint)bVar11 << 0x10 | 0xf3b4)) {
        *(ushort *)((uint)bVar11 << 0x10 | 0xf3a2) = uVar8;
      }
      else {
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3a2) =
             *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3b4);
      }
      goto LAB_00029a48;
    }
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3a4) & 0x80) == 0) {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3a2) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xb28)
      ;
    }
    else {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3a2) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xb26)
      ;
    }
  }
  (*tcu_torque_state_reset)();
  uVar12 = 0;
LAB_00029a48:
  *(ushort *)((uint)bVar11 << 0x10 | 0xf962) = uVar12;
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf964) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3ae);
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf966) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3aa);
  uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf3ae) >> 8;
  puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3a8);
  if (uVar12 < *puVar1 || uVar12 == *puVar1) {
    if (*(short *)((uint)bVar11 << 0x10 | 0xf3b8) != -1) {
      psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf3b8);
      *psVar2 = *psVar2 + 1;
    }
  }
  else {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3b8) = 0;
  }
  sVar3 = 0x20;
  if (*(short *)((uint)bVar11 << 0x10 | 0xefc2) == 0) {
    if (*(short *)((uint)bVar11 << 0x10 | 0xf3b0) == 0) {
      uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf3b8);
      if (*(ushort *)((uint)bVar11 << 0x10 | 0xb62) <= uVar12) {
        sVar3 = 0x10;
      }
    }
    else {
      sVar3 = 0x100;
    }
    if ((sVar3 != 0x10) && (sVar3 != 0x20)) {
      uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf3a8);
    }
  }
  (*identity_passthrough_arg1)(uVar12);
  (*mul_u16_hi)();
  (*sat_add_s16)();
  uVar6 = (*inc_if_signed_flag)();
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3ac) = uVar6;
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf0ec) & 2) == 0) {
    if (*(ushort *)((uint)bVar11 << 0x10 | 0xb2c) <= *(ushort *)((uint)bVar11 << 0x10 | 0xf3aa)) {
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3a6);
      *puVar1 = *puVar1 | 0x10;
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3de) =
           *(undefined2 *)((uint)bVar11 << 0x10 | 0xf17a);
    }
  }
  else {
    uVar12 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf3de),
                         *(ushort *)((uint)bVar11 << 0x10 | 0xb2e));
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf17a);
    if ((*puVar1 <= uVar12 && uVar12 != *puVar1) ||
       (*(ushort *)((uint)bVar11 << 0x10 | 0xf3aa) < *(ushort *)((uint)bVar11 << 0x10 | 0xb2a))) {
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3a6);
      *puVar1 = *puVar1 & 0xffef;
    }
  }
  puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3a6);
  *puVar1 = *puVar1 & 0xfffe;
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf3a6) & 0x10) == 0) {
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf0ec) & 2) != 0) {
      uVar12 = sat_mul_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf3ac),3);
      *(ushort *)((uint)bVar11 << 0x10 | 0xf3ac) = uVar12;
      uVar12 = sat_mul_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf3ae),3);
      *(ushort *)((uint)bVar11 << 0x10 | 0xf3ae) = uVar12;
      uVar12 = sat_mul_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf3aa),3);
      uVar12 = clamp_u16(uVar12,1,param_5);
      *(ushort *)((uint)bVar11 << 0x10 | 0xf3aa) = uVar12;
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3a6);
      *puVar1 = *puVar1 | 1;
    }
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf0ec);
    *puVar1 = *puVar1 & 0xfffd;
  }
  else {
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf0ec) & 2) == 0) {
      uVar6 = (*div_u16_rounded)();
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3ac) = uVar6;
      uVar6 = (*div_u16_rounded)();
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf3ae) = uVar6;
      uVar5 = 0xff;
      uVar8 = 3;
      uVar12 = (*div_u16_rounded)();
      uVar12 = clamp_u16(uVar12,uVar8,uVar5);
      *(ushort *)((uint)bVar11 << 0x10 | 0xf3aa) = uVar12;
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf3a6);
      *puVar1 = *puVar1 | 1;
    }
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf0ec);
    *puVar1 = *puVar1 | 2;
  }
  (*update_flag_fe97_bit2_cond)();
  return;
}



bool tcu_f384_f388_hysteresis_gate_check(void)

{
  ushort uVar1;
  ushort uVar2;
  
  if (DAT_0001f384 < DAT_0001f388) {
    uVar2 = DAT_0001f388 - DAT_0001f384;
  }
  else {
    uVar2 = DAT_0001f384 - DAT_0001f388;
  }
  uVar1 = mul_u16_shr8_sat(DAT_0001f384,DAT_00010acc);
  return uVar1 <= uVar2;
}



void tcu_shift_state_reset(ushort param_fp1)

{
  byte bVar1;
  short in_stack_00000004;
  
  bVar1 = 1;
  if (in_stack_00000004 == 1) {
    DAT_0001f38a = DAT_0001f384;
    timer_ff44_ff4a_arm_channels_atomic(DAT_0001f39e);
  }
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf3d0) = 0;
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf394) = 0;
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf38c) = 0xca;
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf38e) = 0xca;
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf364) = 0xa0;
  return;
}



void tcu_torque_state_reset(void)

{
  DAT_0001f3c2 = DAT_0001f3ba;
  DAT_0001f3c0 = DAT_0001f3ba;
  DAT_0001f3be = DAT_0001f3ba;
  DAT_0001f3c8 = 0x8000;
  DAT_0001f3c6 = 0x8000;
  DAT_0001f3c4 = 0x8000;
  DAT_0001f3ca = 0;
  return;
}



void tcu_shift_timing_state_machine(void)

{
  ushort *puVar1;
  short *psVar2;
  undefined2 in_ram_00000a9a;
  short sVar3;
  ushort uVar4;
  undefined2 extraout_R1;
  undefined2 extraout_R1_00;
  undefined2 extraout_R1_01;
  undefined2 extraout_R1_02;
  undefined2 uVar5;
  undefined2 extraout_R1_03;
  ushort uVar6;
  ushort extraout_R2;
  ushort extraout_R2_00;
  short sVar7;
  short extraout_R2_01;
  byte bVar8;
  ushort in_SR;
  ushort uVar9;
  byte in_CF;
  
  uVar9 = (in_SR & 0xfffe | (ushort)in_CF) & 0xfff1 | 8;
  bVar8 = 0;
  sVar3 = DAT_0000f5e6;
  DAT_0000f5e6 = sVar3 << 1;
  sVar3 = (*check_flag_fe87_bit1_clear)();
  if (sVar3 != 0) {
    puVar1 = (ushort *)((uint)bVar8 << 0x10 | 0xf5e6);
    *puVar1 = *puVar1 | 1;
  }
  uVar4 = *(ushort *)((uint)bVar8 << 0x10 | 0xf5e0);
  *(ushort *)((uint)bVar8 << 0x10 | 0xf386) = uVar4;
  uVar6 = *(ushort *)((uint)bVar8 << 0x10 | 0xf386);
  if ((*(short *)((uint)bVar8 << 0x10 | 0xefa4) == 0) || (uVar6 = uVar6 & 0x8000, uVar6 != 0)) {
    *(undefined2 *)((uint)bVar8 << 0x10 | 0xf3cc) = 0;
  }
  else if ((*(ushort *)((uint)bVar8 << 0x10 | 0xf3cc) & 2) != 0) {
    if (*(short *)((uint)bVar8 << 0x10 | 0xf394) != 0) {
      uVar4 = (*compose_status_bits_fe86_fe8a)();
      uVar4 = *(ushort *)((uint)bVar8 << 0x10 | 0xf39e) & uVar4;
      if (uVar4 == 0) {
        if (*(short *)((uint)bVar8 << 0x10 | 0xf3d0) == 2) {
          sVar3 = ((*(short *)((uint)bVar8 << 0x10 | 0xf5e4) + extraout_R2) -
                  *(short *)((uint)bVar8 << 0x10 | 0xf382)) -
                  *(short *)((uint)bVar8 << 0x10 | 0xf39c);
          if ((sVar3 - *(short *)((uint)bVar8 << 0x10 | 0xff46) & 0x8000U) != 0) {
            uVar5 = *(undefined2 *)((uint)bVar8 << 0x10 | 0xff46);
            uVar4 = (*sci3_timeout_set_abort)();
            *(undefined2 *)((uint)bVar8 << 0x10 | 0xf396) = uVar5;
            goto LAB_00029d8e;
          }
          uVar4 = (*sci3_timeout_set_abort)();
          *(short *)((uint)bVar8 << 0x10 | 0xf396) = sVar3;
          uVar9 = *(short *)((uint)bVar8 << 0x10 | 0xf398) -
                  *(short *)((uint)bVar8 << 0x10 | 0xf5e4);
          if (uVar9 < extraout_R2_00) {
            uVar4 = extraout_R2_00 - uVar9;
            uVar9 = uVar4;
          }
          else {
            uVar9 = uVar9 - extraout_R2_00;
          }
          if (((*(ushort *)((uint)bVar8 << 0x10 | 0xf3ce) & 1) != 0) ||
             (puVar1 = (ushort *)((uint)bVar8 << 0x10 | 0xf39c),
             *puVar1 <= uVar9 && uVar9 != *puVar1)) {
            uVar4 = *(short *)((uint)bVar8 << 0x10 | 0xf5e4) + extraout_R2_00;
            *(ushort *)((uint)bVar8 << 0x10 | 0xf398) = uVar4;
          }
        }
      }
      else if (*(short *)((uint)bVar8 << 0x10 | 0xf3d0) != 0) {
        uVar9 = *(short *)((uint)bVar8 << 0x10 | 0xf398) - *(short *)((uint)bVar8 << 0x10 | 0xf5e4);
        if (uVar9 < extraout_R2) {
          uVar9 = extraout_R2 - uVar9;
        }
        else {
          uVar9 = uVar9 - extraout_R2;
        }
        if (((*(ushort *)((uint)bVar8 << 0x10 | 0xf3ce) & 1) != 0) ||
           (puVar1 = (ushort *)((uint)bVar8 << 0x10 | 0xf39c), *puVar1 <= uVar9 && uVar9 != *puVar1)
           ) {
LAB_00029d8e:
          uVar4 = (*tcu_shift_timer_arm)(uVar4);
        }
      }
    }
    if ((*(ushort *)((uint)bVar8 << 0x10 | 0xf5ee) & 1) == 0) {
LAB_00029df7:
      tcu_shift_timer_reset(uVar4);
    }
    else if (((*(ushort *)((uint)bVar8 << 0x10 | 0xf3cc) & 0x40) == 0) &&
            (*(ushort *)((uint)bVar8 << 0x10 | 0xf38c) < 200)) {
      uVar4 = (*compose_status_bits_fe86_fe8a)();
      uVar4 = *(ushort *)((uint)bVar8 << 0x10 | 0xf39e) & uVar4;
      if ((uVar4 != 0) && (*(short *)((uint)bVar8 << 0x10 | 0xf3d0) != 0)) goto LAB_00029df7;
    }
    (*tcu_f382_correction_calc)();
    uVar9 = mul_u16_shr8_sat(0x9c,*(ushort *)((uint)bVar8 << 0x10 | 0xf386));
    sVar3 = (uVar9 + *(short *)((uint)bVar8 << 0x10 | 0xf5e4) +
            *(short *)((uint)bVar8 << 0x10 | 0xf392)) - *(short *)((uint)bVar8 << 0x10 | 0xf382);
    sVar7 = *(short *)((uint)bVar8 << 0x10 | 0xf398) + *(short *)((uint)bVar8 << 0x10 | 0xad8);
    uVar9 = sVar3 - sVar7 & 0x8000;
    if (uVar9 != 0) {
      sVar3 = sVar7;
    }
    uVar9 = (*compose_status_bits_fe86_fe8a)(uVar9);
    uVar9 = *(ushort *)((uint)bVar8 << 0x10 | 0xf39e) & uVar9;
    uVar5 = extraout_R1_01;
    if (uVar9 == 0) {
      if (*(short *)((uint)bVar8 << 0x10 | 0xf3d0) != 2) {
        *(undefined2 *)((uint)bVar8 << 0x10 | 0xf3d0) = 0;
        uVar9 = (*sci3_timeout_set_abort)();
        uVar5 = extraout_R1_02;
      }
    }
    else if (*(short *)((uint)bVar8 << 0x10 | 0xf3d0) == 0) goto LAB_00029e60;
    *(short *)((uint)bVar8 << 0x10 | 0xf39a) = sVar3;
    goto LAB_00029e60;
  }
  tcu_shift_timer_reset(uVar4,extraout_R1,uVar6,uVar9);
  uVar9 = (*tcu_f382_correction_calc)();
  uVar5 = extraout_R1_00;
LAB_00029e60:
  puVar1 = (ushort *)((uint)bVar8 << 0x10 | 0xf3ce);
  *puVar1 = *puVar1 & 0xfffe;
  puVar1 = (ushort *)((uint)bVar8 << 0x10 | 0xf340);
  *puVar1 = *puVar1 | 0x80;
  if ((*(ushort *)((uint)bVar8 << 0x10 | 0xf1f2) & 0x80) != 0) {
    if ((*(short *)((uint)bVar8 << 0x10 | 0xf398) - *(short *)((uint)bVar8 << 0x10 | 0xf5e4) &
        0x8000U) == 0) {
      sVar3 = *(short *)((uint)bVar8 << 0x10 | 0xf398);
    }
    else {
      sVar3 = *(short *)((uint)bVar8 << 0x10 | 0xf5e4);
    }
    uVar9 = timer_ff5a_set_clamped_armed(sVar3 + 0x19);
    uVar5 = extraout_R1_03;
  }
  bVar8 = 0;
  sVar3 = g_f4a4_clear_hold_gate_ef90;
  if ((sVar3 == 0) || (sVar3 = g_tcu_shift_timing_timer_gate_efa2, sVar3 == 0)) {
    DAT_0000f5ee = 0;
    uVar9 = DAT_0000f5e6;
    uVar9 = uVar9 | 0xfffe;
    DAT_0000f5e6 = uVar9;
  }
  sVar7 = (*check_flag_f0d8_bit0_clear)(uVar9,uVar5,in_ram_00000a9a);
  sVar3 = extraout_R2_01;
  if (sVar7 != 0) {
    sVar3 = extraout_R2_01 << 1;
  }
  *(short *)((uint)bVar8 << 0x10 | 0xefa2) = sVar3;
  if ((*(ushort *)((uint)bVar8 << 0x10 | 0xf0f8) & 0x80) == 0) {
    if ((*(ushort *)((uint)bVar8 << 0x10 | 0xf0f8) & 0x20) == 0) {
      *(undefined2 *)((uint)bVar8 << 0x10 | 0xf23c) = *(undefined2 *)((uint)bVar8 << 0x10 | 0x86e);
    }
    else if ((*(ushort *)((uint)bVar8 << 0x10 | 0xf224) & 4) == 0) {
      *(undefined2 *)((uint)bVar8 << 0x10 | 0xf23c) = *(undefined2 *)((uint)bVar8 << 0x10 | 0x870);
    }
    else {
      *(undefined2 *)((uint)bVar8 << 0x10 | 0xf23c) = *(undefined2 *)((uint)bVar8 << 0x10 | 0x86c);
    }
  }
  else if ((*(short *)((uint)bVar8 << 0x10 | 0xf23c) != 0) &&
          (psVar2 = (short *)((uint)bVar8 << 0x10 | 0xf23c), sVar3 = *psVar2, *psVar2 = sVar3 + -1,
          sVar3 == -1)) {
    puVar1 = (ushort *)((uint)bVar8 << 0x10 | 0xf25a);
    *puVar1 = *puVar1 | 0x4000;
  }
  if (((*(ushort *)((uint)bVar8 << 0x10 | 0xf25a) & 4) == 0) &&
     (*(short *)((uint)bVar8 << 0x10 | 0xf29e) != 0)) {
    psVar2 = (short *)((uint)bVar8 << 0x10 | 0xf29e);
    *psVar2 = *psVar2 + -1;
  }
  if (*(short *)((uint)bVar8 << 0x10 | 0xf378) != 0) {
    psVar2 = (short *)((uint)bVar8 << 0x10 | 0xf378);
    *psVar2 = *psVar2 + -1;
  }
  tcu_periodic_timer_update();
  fuel_pw_and_airvol_compute();
  *(undefined2 *)((uint)bVar8 << 0x10 | 0xf5ec) = *(undefined2 *)((uint)bVar8 << 0x10 | 0xf5ea);
  stub_empty_0x2c129();
  channel_data_flush_if_active();
  return;
}



void tcu_shift_timer_reset(void)

{
  byte bVar1;
  
  bVar1 = 1;
  (*timer4_oc_arm_from_flags)();
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf3d0) = 0;
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf398) = *(undefined2 *)((uint)bVar1 << 0x10 | 0xff46);
  return;
}



void tcu_shift_timer_arm(void)

{
  undefined2 extraout_R2;
  byte bVar1;
  
  bVar1 = 1;
  DAT_0001f3d0 = 1;
  (*sci3_timeout_set_retry)();
  *(undefined2 *)((uint)bVar1 << 0x10 | 0xf398) = extraout_R2;
  return;
}



void tcu_f382_correction_calc(void)

{
  ushort *puVar1;
  ushort uVar2;
  byte bVar3;
  
  bVar3 = 1;
  uVar2 = (*mul_u16_x2_sat_alt)();
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xf380);
  if (uVar2 < *puVar1 || uVar2 == *puVar1) {
    *(ushort *)((uint)bVar3 << 0x10 | 0xf382) = uVar2;
  }
  else {
    *(undefined2 *)((uint)bVar3 << 0x10 | 0xf382) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf380);
  }
  return;
}



// [RE-VERIFIED 2026-07-12 under current Sleigh grammar, live disassembly]
// fuel_pw_and_airvol_compute: large fuel/air computation function. Confirmed writes:
//   - 0xF970 (@0x2a6c1) = InjPulseWidth, MUT RequestID 0x29 -> table entry 0xF970. VERIFIED.
//   - 0xF972 (@0x2a6d7) = AirVol, MUT RequestID 0x2C -> table entry 0xF973 (low byte of
//     0xF972/F973 word). VERIFIED.
// Gated on RAM 0xF17A (@0x2a67c) vs ROM threshold @0x806 as claimed in logging.txt - NOTE:
// 0xF17A itself is the subject of logging.txt's UNRESOLVED RPM-identity investigation
// (REFUTED as RPM, confirmed instead to be engine_torque_pct_scale_calc output per
// logging.txt OPEN ITEMS #1) - this function's gating on F17A is real and confirmed, but
// do not read that as F17A being RPM.
// Reset path: injpw_airvol_reset_on_fuelcut (0x24680) - see that function's comment.
// 
// MUT SWEEP FOLLOW-UP (2026-07-15): both 0xF970 and 0xF972 are written here as single
// 16-bit stores, not separate byte writes. This confirms MUT ReqID 0x2A (F971) is simply
// the low byte of the F970/F971 InjPulseWidth word, and ReqID 0x2B (F972) is simply the
// high byte of the F972/F973 AirVol word - same idiom as the F3FA/F3FB and F3FC/F3FD
// mode-shadow pairs found elsewhere in this project. Both rows updated from BLANK/POINTER
// to CONFIRMED in mut_verification_status.md; see review.md item 7b.

void fuel_pw_and_airvol_compute(void)

{
  uint uVar1;
  ushort *puVar2;
  short *psVar3;
  ushort uVar4;
  undefined2 uVar5;
  ushort uVar6;
  short sVar7;
  ushort value;
  undefined2 extraout_R1;
  ushort value_00;
  undefined2 extraout_R1_00;
  undefined2 extraout_R2;
  ushort uVar8;
  ushort uVar9;
  char cVar10;
  char extraout_R2_00;
  char extraout_R2_01;
  byte bVar11;
  short in_stack_00000004;
  ushort uVar12;
  ushort uStack_6;
  ushort uStack_4;
  
  bVar11 = 1;
  if (in_stack_00000004 != 0) {
    if ((((engine_state_flags_f25e & 0x80) == 0) || ((g_status_flags_001f25a & 1) == 0)) ||
       ((DAT_0001f2b0 == 0 && ((DAT_0001f5ee & 2) != 0)))) {
      cVar10 = '\x02';
    }
    else {
      cVar10 = '\0';
    }
    uStack_6 = DAT_0001f24e;
    goto LAB_0002a66f;
  }
  if ((ushort)DAT_0001f5ca < (ushort)BYTE_ARRAY_00010886) {
    g_status_flags_001f25a = g_status_flags_001f25a | 0x20;
  }
  else {
    g_status_flags_001f25a = g_status_flags_001f25a & 0xffdf;
  }
  uVar4 = muldiv_u16(DAT_0001f1da,0x9c,DAT_0001f1d8);
  if (0x9b < uVar4) {
    uVar4 = 0x9c;
  }
  uVar5 = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1d2);
  (*mul_u16_hi)(uVar5,0,uVar4,uVar5,0x9c,0,*(undefined2 *)((uint)bVar11 << 0x10 | 0xf1dc));
  uVar4 = (*sat_sub_u16_clamp0)();
  uVar4 = passthrough_or_sat(uVar4,value);
  *(ushort *)((uint)bVar11 << 0x10 | 0xf1ba) = uVar4;
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1dc) = extraout_R2;
  uVar4 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1ba);
  uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1b6);
  *(ushort *)((uint)bVar11 << 0x10 | 0xf850) = uVar4;
  if (uVar4 < uVar12) {
    uVar8 = uVar12 - uVar4;
    *(ushort *)((uint)bVar11 << 0x10 | 0xf854) = uVar8;
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf852) = 0;
  }
  else {
    uVar8 = uVar4 - uVar12;
    *(ushort *)((uint)bVar11 << 0x10 | 0xf852) = uVar8;
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf854) = 0;
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf224) & 0x8000) != 0) {
    if (uVar4 < uVar12) {
      if ((*(short *)((uint)bVar11 << 0x10 | 0xf014) == 0) &&
         (*(ushort *)((uint)bVar11 << 0x10 | 0xa6a) <= uVar8)) {
        uVar4 = sat_sub_u16(uVar12,*(ushort *)((uint)bVar11 << 0x10 | 0xa6c));
        *(ushort *)((uint)bVar11 << 0x10 | 0xf1ba) = uVar4;
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf014) =
             *(undefined2 *)((uint)bVar11 << 0x10 | 0xa68);
      }
    }
    else if ((*(short *)((uint)bVar11 << 0x10 | 0xf012) == 0) &&
            (*(ushort *)((uint)bVar11 << 0x10 | 0xa64) <= uVar8)) {
      uVar4 = sat_add_u16(uVar12,*(ushort *)((uint)bVar11 << 0x10 | 0xa66));
      *(ushort *)((uint)bVar11 << 0x10 | 0xf1ba) = uVar4;
    }
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf1de) < *(ushort *)((uint)bVar11 << 0x10 | 0x81c)) &&
     (uVar4 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf1b6),
                          *(ushort *)((uint)bVar11 << 0x10 | 0xf1ba)),
     *(ushort *)((uint)bVar11 << 0x10 | 0x81e) <= uVar4)) {
    uVar4 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf1b6),
                        *(ushort *)((uint)bVar11 << 0x10 | 0x820));
    *(ushort *)((uint)bVar11 << 0x10 | 0xf1ba) = uVar4;
  }
  if (*(ushort *)((uint)bVar11 << 0x10 | 0xf1ba) < *(ushort *)((uint)bVar11 << 0x10 | 0xf1c0)) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1ba) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1c0);
  }
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1bc) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1b6);
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1c6) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1c4);
  (*identity_passthrough_arg1)();
  uVar5 = (*weighted_blend_4term_u8frac)();
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1b6) = uVar5;
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1b8) = extraout_R1;
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 2) != 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1b6) = 0;
  }
  uVar4 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1b6);
  uVar12 = 0xa164;
  uVar5 = (*mul_u16_x2_sat)();
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1c4) = uVar5;
  if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 1) != 0) ||
     (uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1b6),
     puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0xf1be), *puVar2 <= uVar8 && uVar8 != *puVar2)) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1b6) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1be);
  }
  if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 1) != 0) ||
     (uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1c4),
     puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0xf1c2), *puVar2 <= uVar8 && uVar8 != *puVar2)) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1c4) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1c2);
  }
  if (*(ushort *)((uint)bVar11 << 0x10 | 0xf1b6) < *(ushort *)((uint)bVar11 << 0x10 | 0xf1bc)) {
    uVar8 = *(short *)((uint)bVar11 << 0x10 | 0xf1bc) - *(short *)((uint)bVar11 << 0x10 | 0xf1b6);
    puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0xf1ce);
    if (*puVar2 <= uVar8 && uVar8 != *puVar2) {
      *(ushort *)((uint)bVar11 << 0x10 | 0xf1ce) = uVar8;
    }
  }
  else {
    uVar8 = *(short *)((uint)bVar11 << 0x10 | 0xf1b6) - *(short *)((uint)bVar11 << 0x10 | 0xf1bc);
    puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0xf1ca);
    if (*puVar2 <= uVar8 && uVar8 != *puVar2) {
      *(ushort *)((uint)bVar11 << 0x10 | 0xf1ca) = uVar8;
    }
  }
  uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1c4);
  uVar6 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1c6);
  if (uVar8 < uVar6) {
    uVar9 = uVar6 - uVar8;
  }
  else {
    uVar9 = uVar8 - uVar6;
  }
  if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf226) & 0x200) == 0) &&
     (uVar8 < *(ushort *)((uint)bVar11 << 0x10 | 0xf2e4))) {
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf31c) & 0x80) == 0) {
      uStack_4 = *(ushort *)((uint)bVar11 << 0x10 | 0x960);
    }
    else {
      uStack_4 = *(ushort *)((uint)bVar11 << 0x10 | 0x95e);
    }
    uVar9 = muldiv_u16(uVar9,uStack_4,*(ushort *)((uint)bVar11 << 0x10 | 0x962));
    if (uVar8 < uVar6) {
      uVar4 = 0xa232;
      uVar12 = 2;
      uVar8 = sat_sub_u16(uVar8,uVar9);
      *(ushort *)((uint)bVar11 << 0x10 | 0xf1d0) = uVar8;
    }
    else {
      uVar4 = 0xa225;
      uVar12 = 2;
      uVar8 = sat_add_u16(uVar8,uVar9);
      *(ushort *)((uint)bVar11 << 0x10 | 0xf1d0) = uVar8;
    }
    if (*(ushort *)((uint)bVar11 << 0x10 | 0xf2e4) <= *(ushort *)((uint)bVar11 << 0x10 | 0xf1d0)) {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf1d0) =
           *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2e4);
    }
  }
  else {
    *(ushort *)((uint)bVar11 << 0x10 | 0xf1d0) = uVar8;
  }
  cVar10 = '\x01';
  if (*(short *)((uint)bVar11 << 0x10 | 0xf2dc) != 0) {
    uVar4 = 2;
    uVar8 = mul_u16_shr8_sat(*(ushort *)((uint)bVar11 << 0x10 | 0xf2dc),
                             *(ushort *)((uint)bVar11 << 0x10 | 0xf2ec));
    uVar6 = *(short *)((uint)bVar11 << 0x10 | 0xf2dc) - uVar8;
    puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0x96c);
    if (uVar6 < *puVar2 || uVar6 == *puVar2) {
      if (*(short *)((uint)bVar11 << 0x10 | 0xf2fe) == 0) {
        uVar8 = sat_sub_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf2dc),1);
        *(ushort *)((uint)bVar11 << 0x10 | 0xf2dc) = uVar8;
        cVar10 = extraout_R2_01;
      }
      else {
        cVar10 = '\0';
      }
    }
    else {
      *(ushort *)((uint)bVar11 << 0x10 | 0xf2dc) = uVar8;
      cVar10 = extraout_R2_00;
    }
  }
  if (cVar10 != '\0') {
    uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf10e);
    puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0x96e);
    if (uVar8 < *puVar2 || uVar8 == *puVar2) {
      uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf10e);
      puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0x974);
      if (uVar8 < *puVar2 || uVar8 == *puVar2) {
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2fe) =
             *(undefined2 *)((uint)bVar11 << 0x10 | 0x976);
      }
      else {
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2fe) =
             *(undefined2 *)((uint)bVar11 << 0x10 | 0x972);
      }
    }
    else {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2fe) = *(undefined2 *)((uint)bVar11 << 0x10 | 0x970)
      ;
    }
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf2fe) != 0) {
    psVar3 = (short *)((uint)bVar11 << 0x10 | 0xf2fe);
    *psVar3 = *psVar3 + -1;
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf302) != 0) {
    psVar3 = (short *)((uint)bVar11 << 0x10 | 0xf302);
    *psVar3 = *psVar3 + -1;
  }
  if (*(ushort *)((uint)bVar11 << 0x10 | 0xf1b6) < *(ushort *)((uint)bVar11 << 0x10 | 0xf2e0)) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf302) = *(undefined2 *)((uint)bVar11 << 0x10 | 0x954);
  }
  *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2de) = 0;
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf226) & 0x200) == 0) {
    uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1b6);
    sVar7 = 6;
    do {
      uVar8 = uVar8 >> 1;
      if (uVar8 == 0) break;
      sVar7 = sVar7 + -1;
    } while (sVar7 != -1);
    if (0xb < uVar8) {
      uVar8 = 0xc;
    }
    DAT_0000f2f8 = (ushort)*(byte *)((int)uVar8 + 0x1ea4);
    uVar5 = DAT_0000f2f8;
    DAT_0000f2f2 = uVar5;
    uVar5 = DAT_0000f2f8;
    DAT_0000f2f0 = uVar5;
    uVar8 = (ushort)*(byte *)((int)uVar8 + 0x1eb1);
    bVar11 = 0;
  }
  else {
    uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0x956);
    *(ushort *)((uint)bVar11 << 0x10 | 0xf2f8) = uVar8;
    *(ushort *)((uint)bVar11 << 0x10 | 0xf2f2) = uVar8;
    *(ushort *)((uint)bVar11 << 0x10 | 0xf2f0) = uVar8;
  }
  *(ushort *)((uint)bVar11 << 0x10 | 0xf2f6) = uVar8;
  *(ushort *)((uint)bVar11 << 0x10 | 0xf2f4) = uVar8;
  uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 0x13;
  if (uVar8 == 0) {
    sVar7 = (*check_flag_fe96_bit7_clear)(0);
    uVar8 = 0;
    if (((sVar7 == 0) || (uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 0x28, uVar8 != 0)) ||
       ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25e) & 0x40) != 0)) {
      *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2dc) = 0;
    }
    else {
      uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1b6);
      if (uVar8 < *(ushort *)((uint)bVar11 << 0x10 | 0xf1bc)) {
        uVar8 = *(ushort *)((uint)bVar11 << 0x10 | 0xf1b6);
        if (uVar8 < *(ushort *)((uint)bVar11 << 0x10 | 0xf2e2)) {
          uVar6 = *(short *)((uint)bVar11 << 0x10 | 0xf1bc) -
                  *(short *)((uint)bVar11 << 0x10 | 0xf1b6);
          if (*(ushort *)((uint)bVar11 << 0x10 | 0xf2fc) <= uVar6) {
            uVar6 = *(ushort *)((uint)bVar11 << 0x10 | 0xf2fc);
          }
          if (*(short *)((uint)bVar11 << 0x10 | 0xf028) == 0) {
            uVar9 = *(ushort *)((uint)bVar11 << 0x10 | 0xf2f2);
          }
          else {
            uVar9 = *(ushort *)((uint)bVar11 << 0x10 | 0xf2f6);
          }
          if (uVar9 < uVar6) {
            if (*(ushort *)((uint)bVar11 << 0x10 | 0xf2de) <= uVar6) {
              *(ushort *)((uint)bVar11 << 0x10 | 0xf2de) = uVar6;
            }
            *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2dc) = 0;
          }
        }
      }
      else if (*(short *)((uint)bVar11 << 0x10 | 0xf302) != 0) {
        uVar6 = *(short *)((uint)bVar11 << 0x10 | 0xf1b6) -
                *(short *)((uint)bVar11 << 0x10 | 0xf1bc);
        if (*(ushort *)((uint)bVar11 << 0x10 | 0xf2fa) <= uVar6) {
          uVar6 = *(ushort *)((uint)bVar11 << 0x10 | 0xf2fa);
        }
        if (*(short *)((uint)bVar11 << 0x10 | 0xf028) == 0) {
          uVar9 = *(ushort *)((uint)bVar11 << 0x10 | 0xf2f0);
        }
        else {
          uVar9 = *(ushort *)((uint)bVar11 << 0x10 | 0xf2f4);
        }
        if (uVar9 < uVar6) {
          if (*(ushort *)((uint)bVar11 << 0x10 | 0xf2dc) <= uVar6) {
            *(ushort *)((uint)bVar11 << 0x10 | 0xf2dc) = uVar6;
          }
          *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2de) = 0;
        }
      }
      if (((((*(ushort *)((uint)bVar11 << 0x10 | 0xf1f4) & 8) != 0) &&
           ((*(ushort *)((uint)bVar11 << 0x10 | 0xee88) & 1) != 0)) ||
          (((*(ushort *)((uint)bVar11 << 0x10 | 0xf1f4) & 8) == 0 &&
           ((*(ushort *)((uint)bVar11 << 0x10 | 0xf102) & 0x80) != 0)))) &&
         (*(short *)((uint)bVar11 << 0x10 | 0xefb8) != 0)) {
        *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2dc) = 0;
      }
      if (*(short *)((uint)bVar11 << 0x10 | 0xefb2) != 0) goto LAB_0002a43d;
    }
  }
  else {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2dc) = 0;
LAB_0002a43d:
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2de) = 0;
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf2dc) == 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf256) = 0;
  }
  else {
    uVar12 = 0xa461;
    uVar8 = muldiv_u16_scaled(*(ushort *)((uint)bVar11 << 0x10 | 0xf2e6),
                              *(ushort *)((uint)bVar11 << 0x10 | 0xf2e8),
                              *(ushort *)((uint)bVar11 << 0x10 | 0xf2dc),0x400);
    uVar4 = 2;
    uVar8 = passthrough_or_sat(uVar8,value_00);
    *(ushort *)((uint)bVar11 << 0x10 | 0xf256) = uVar8;
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf2de) == 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf258) = 0;
  }
  else {
    uVar4 = 0xa493;
    uVar12 = 2;
    uVar8 = muldiv_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf2de),
                       *(ushort *)((uint)bVar11 << 0x10 | 0xf2ea),0x400);
    *(ushort *)((uint)bVar11 << 0x10 | 0xf258) = uVar8;
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf594) & 0x80) != 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf256) = 0;
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf258) = 0;
  }
  sVar7 = (*check_flag_fe96_bit7_clear)(uVar8);
  if ((sVar7 == 0) || ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 0x38) != 0)) {
    uStack_6 = 0;
  }
  else if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 1) == 0) &&
          ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 3) == 0)) {
    if (*(char *)((uint)bVar11 << 0x10 | 0x2c1) == '\0') {
      uVar8 = 2;
      uVar4 = get_high_byte(*(ushort *)((uint)bVar11 << 0x10 | 0xf26e));
      uVar4 = uVar4 + *(short *)((uint)bVar11 << 0x10 | 0xf288) +
              *(short *)((uint)bVar11 << 0x10 | 0xf280) + 0x80;
    }
    else {
      uVar8 = *(short *)((uint)bVar11 << 0x10 | 0x8da) << 2;
      uVar6 = get_high_byte(*(ushort *)((uint)bVar11 << 0x10 | 0xf26e));
      uVar6 = sat_sub_u16((uVar6 + *(short *)((uint)bVar11 << 0x10 | 0xf288)) * 2 +
                          *(short *)((uint)bVar11 << 0x10 | 0xf280),uVar8);
      uVar8 = 2;
      uVar4 = clamp_u16(uVar6,uVar12,uVar4);
    }
    uVar12 = 0x400;
    uVar4 = muldiv_u16(*(ushort *)((uint)bVar11 << 0x10 | 0xf268),uVar4,0x200);
    uStack_6 = muldiv_u16(uVar4,uVar12,uVar8);
    if (*(short *)((uint)bVar11 << 0x10 | 0xf256) != 0) {
      uStack_6 = sat_add_u16(uStack_6,*(ushort *)((uint)bVar11 << 0x10 | 0xf256));
    }
    if ((*(short *)((uint)bVar11 << 0x10 | 0xf258) != 0) &&
       (uStack_6 = sat_sub_u16(uStack_6,*(ushort *)((uint)bVar11 << 0x10 | 0xf258)), uStack_6 == 0))
    {
      uStack_6 = 1;
    }
  }
  else {
    uStack_6 = *(ushort *)((uint)bVar11 << 0x10 | 0xf2d4);
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25e) & 0x40) == 0) {
LAB_0002a5cf:
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf2aa) = 0;
  }
  else {
    uVar1 = (uint)bVar11;
    bVar11 = 0;
    if (*(char *)((int)*(undefined2 *)(uVar1 << 0x10 | 0xf2aa) + 0x1f11) == '\0') {
      uStack_6 = 0;
    }
    uVar5 = DAT_0000f2aa;
    sVar7 = DAT_0000f2aa;
    uVar5 = DAT_0000f2aa;
    if (sVar7 != -1) {
      sVar7 = DAT_0000f2aa;
      DAT_0000f2aa = sVar7 + 1;
    }
    uVar5 = DAT_0000f2aa;
    uVar4 = DAT_0000f2aa;
    uVar5 = DAT_0000f2aa;
    if (3 < uVar4) goto LAB_0002a5cf;
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf5f4) != -1) {
    psVar3 = (short *)((uint)bVar11 << 0x10 | 0xf5f4);
    *psVar3 = *psVar3 + 1;
  }
  if (1 < *(ushort *)((uint)bVar11 << 0x10 | 0xf5f4)) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf5f4) = 0;
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf5ee) & 2) != 0) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf5f4) = 1;
  }
  if ((((*(ushort *)((uint)bVar11 << 0x10 | 0xf25e) & 0x80) == 0) ||
      ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 1) == 0)) ||
     ((*(short *)((uint)bVar11 << 0x10 | 0xf2b0) == 0 &&
      ((*(ushort *)((uint)bVar11 << 0x10 | 0xf5ee) & 2) != 0)))) {
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf25a) & 0x8000) == 0) {
      if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf5ee) & 2) == 0) &&
         ((*(ushort *)((uint)bVar11 << 0x10 | 0xf5ee) & 6) == 0)) {
        cVar10 = '\x02';
        uStack_6 = uStack_6 >> 2;
      }
      else {
        cVar10 = '\x01';
      }
    }
    else {
      cVar10 = '\x02';
    }
  }
  else {
    cVar10 = '\0';
  }
  if (*(short *)((uint)bVar11 << 0x10 | 0xf2b0) != 0) {
    psVar3 = (short *)((uint)bVar11 << 0x10 | 0xf2b0);
    *psVar3 = *psVar3 + -1;
  }
  *(ushort *)((uint)bVar11 << 0x10 | 0xf24e) = uStack_6;
LAB_0002a66f:
  if ((cVar10 == '\0') || (uStack_6 == 0)) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf254) = 0;
  }
  else {
    uVar12 = 0;
    uVar4 = *(ushort *)((uint)bVar11 << 0x10 | 0xf17a);
    puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0x806);
    if (uVar4 < *puVar2 || uVar4 == *puVar2) {
      uVar12 = *(ushort *)((uint)bVar11 << 0x10 | 0x804);
      sVar7 = 5;
      do {
        uVar12 = uVar12 << 1;
        if (uVar12 == 0) break;
        sVar7 = sVar7 + -1;
      } while (sVar7 != -1);
    }
    uVar8 = (ushort)&DAT_0000fde8;
    uVar4 = (*f2cc_offset_accumulate)();
    uVar4 = clamp_u16(uVar4,uVar8,uVar12);
    *(ushort *)((uint)bVar11 << 0x10 | 0xf254) = uVar4;
    *(ushort *)((uint)bVar11 << 0x10 | 0xf250) = uVar4;
    uVar12 = sat_mul_u16(uVar4,4);
    *(ushort *)((uint)bVar11 << 0x10 | 0xf970) = uVar12;
    *(ushort *)((uint)bVar11 << 0x10 | 0xf252) = uVar4;
    uVar4 = sat_mul_u16(uVar4,4);
    *(ushort *)((uint)bVar11 << 0x10 | 0xf972) = uVar4;
    puVar2 = (ushort *)((uint)bVar11 << 0x10 | 0xf31c);
    *puVar2 = *puVar2 & 0xffbf;
    (*channel_enable_mask_build_and_dispatch_ff2x_ff3x)();
    uVar4 = 0;
    do {
      uVar12 = uVar4 - 1;
      uVar8 = uVar4;
      while (uVar8 != 0xffff) {
        uVar12 = uVar12 - 1;
        uVar8 = uVar12;
      }
      uVar4 = uVar4 + 1;
    } while (uVar4 < 4);
    (*mul_u16_hi)();
    uVar5 = (*sat_add_s16)();
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf336) = uVar5;
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf338) = extraout_R1_00;
  }
  diag_status_f200_f50e_update();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void tcu_periodic_timer_update(void)

{
  bool bVar1;
  
  bVar1 = (DAT_0001f5e6 & 5) == 1;
  if (bVar1) {
    DAT_0001f5f2 = 0;
  }
  if (DAT_0001f5f2 != 0) {
    DAT_0001f5f2 = DAT_0001f5f2 + -1;
  }
  if (DAT_0001f5f2 == 0) {
    DAT_0001f5ee = DAT_0001f5ee | 6;
  }
  if (DAT_0001f500 != 0) {
    DAT_0001f500 = DAT_0001f500 + -1;
  }
  if (bVar1) {
    DAT_0001f500 = DAT_00010f80;
  }
  DAT_0001f5ea = DAT_0001f5ea + 1;
  if ((3 < DAT_0001f5ea) || (bVar1)) {
    DAT_0001f5ea = 0;
  }
  return;
}



void f2cc_offset_accumulate(ushort param_fp1)

{
  byte bVar1;
  ushort in_stack_00000004;
  
  bVar1 = 1;
  if (in_stack_00000004 < 0x209) {
    bVar1 = 0;
    in_stack_00000004 =
         sat_add_u16(in_stack_00000004,(ushort)*(byte *)((int)(in_stack_00000004 >> 3) + 0x1df7));
  }
  sat_add_u16(in_stack_00000004,*(ushort *)((uint)bVar1 << 0x10 | 0xf2cc));
  return;
}



void channel_enable_mask_build_and_dispatch_ff2x_ff3x(ushort param_fp1,ushort param_fp2)

{
  ushort uVar1;
  short in_stack_00000004;
  
  uVar1 = DAT_0001f3dc & DAT_0001f514 & param_fp1 & 0xf;
  if (((g_status_flags_f1f2 & 0x40) != 0) && (DAT_000102ca != '\0')) {
    uVar1 = DAT_0001f228 & uVar1;
  }
  if ((DAT_0001f1f4 & 4) != 0) {
    uVar1 = DAT_0001f730 & uVar1;
  }
  if ((in_stack_00000004 != 0) && (uVar1 != 0)) {
    (*multi_channel_reg_write_and_poll_ff2x_ff3x)();
  }
  return;
}



ushort diag_status_f200_f50e_update(void)

{
  ushort *puVar1;
  ushort uVar2;
  undefined2 uVar3;
  short sVar4;
  undefined2 extraout_R1;
  ushort extraout_R2;
  undefined2 unaff_R3;
  byte bVar5;
  
  bVar5 = 1;
  uVar3 = compose_fe86_fe8a_status_nibble();
  sVar4 = latch_flag_read_fede_bit3(uVar3,extraout_R1,uVar3,unaff_R3);
  if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf50e) &
      *(ushort *)((uint)bVar5 << 0x10 | 0xf50c) &
      (extraout_R2 ^ *(ushort *)((uint)bVar5 << 0x10 | 0xf50c))) != 0) {
    if (sVar4 == 0) {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf200);
      *puVar1 = *puVar1 | 2;
    }
    else {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf200);
      *puVar1 = *puVar1 & 0xfffd;
      *(short *)((uint)bVar5 << 0x10 | 0xf50e) = *(short *)((uint)bVar5 << 0x10 | 0xf50e) << 1;
    }
  }
  uVar2 = *(ushort *)((uint)bVar5 << 0x10 | 0xf50e) & 0xf;
  if (uVar2 == 0) {
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf50e) = 1;
  }
  *(ushort *)((uint)bVar5 << 0x10 | 0xf50c) = extraout_R2;
  return uVar2;
}



void stub_empty_0x2a8cf(void)

{
  return;
}



void stub_empty_0x2a8d1(void)

{
  return;
}



void sci3_rx_process_tcu_byte(ushort param_fp1)

{
  byte bVar1;
  short in_stack_00000004;
  
  bVar1 = 1;
  (*sci3_timeout_clear_flag)();
  if (in_stack_00000004 == *(short *)((uint)bVar1 << 0x10 | 0xf39e)) {
    if (*(short *)((uint)bVar1 << 0x10 | 0xf3d0) == 2) {
      *(undefined2 *)((uint)bVar1 << 0x10 | 0xf3d0) = 1;
      if ((*(ushort *)((uint)bVar1 << 0x10 | 0xf3ce) & 1) == 0) {
        (*sci3_timeout_set_retry)();
      }
    }
    else if ((*(short *)((uint)bVar1 << 0x10 | 0xf3d0) == 1) &&
            ((*(ushort *)((uint)bVar1 << 0x10 | 0xf3ce) & 1) == 0)) {
      *(undefined2 *)((uint)bVar1 << 0x10 | 0xf3d0) = 0;
      (*sci3_timeout_set_abort)();
    }
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address
// WARNING: Unknown calling convention -- yet parameter storage is locked

void tcu_gear_ratio_state_init(void)

{
  DAT_0001ee84 = DAT_00010a7c;
  DAT_0001ee86 = _DAT_00010aae;
  DAT_0001ee88 = 1;
  return;
}



void throttle_idle_state_cold_init(void)

{
  ushort *puVar1;
  undefined2 uVar2;
  byte bVar3;
  
  bVar3 = 1;
  DAT_0001f1e4 = 0xffff;
  if ((DAT_0001f102 & 2) != 0) {
    DAT_0001f20a = DAT_0001f20a | 0x8000;
  }
  DAT_0001ef8c = 6;
  DAT_0001f20a = DAT_0001f20a | 1;
  DAT_0001ef8e = 100;
  sat_sub_u16(g_adc_ch7_raw_f13c + DAT_0001ee82,0x80);
  uVar2 = (*clamp_u8)();
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf13e) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf140) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf13e);
  uVar2 = *(undefined2 *)((uint)bVar3 << 0x10 | 0xf13c);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf14a) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf148) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf146) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf144) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf142) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf31a) = uVar2;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xeff2) = 0x14;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf4f8) = 0x80;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf4fc) = 0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf4fa) = 0xffff;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf4ae) = 0xc0;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf4a8) = 0x18;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf514) = 0xffff;
  puVar1 = (ushort *)((uint)bVar3 << 0x10 | 0xee88);
  *puVar1 = *puVar1 | 1;
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xf16c) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xaae);
  *(undefined2 *)((uint)bVar3 << 0x10 | 0xef9a) = *(undefined2 *)((uint)bVar3 << 0x10 | 0xab2);
  return;
}



// WARNING: Removing unreachable block (ram,0x0002b20e)
// WARNING: Removing unreachable block (ram,0x0002b222)
// WARNING: Removing unreachable block (ram,0x0002b226)
// WARNING: Removing unreachable block (ram,0x0002b230)
// WARNING: Removing unreachable block (ram,0x0002b239)
// WARNING: Removing unreachable block (ram,0x0002b243)
// WARNING: Removing unreachable block (ram,0x0002b24c)
// WARNING: Removing unreachable block (ram,0x0002b252)
// WARNING: Removing unreachable block (ram,0x0002b258)
// WARNING: Removing unreachable block (ram,0x0002b28a)
// WARNING: Removing unreachable block (ram,0x0002b295)
// WARNING: Removing unreachable block (ram,0x0002b299)
// WARNING: Removing unreachable block (ram,0x0002b2a3)
// WARNING: Removing unreachable block (ram,0x0002b2a9)
// WARNING: Removing unreachable block (ram,0x0002b2ad)
// WARNING: Removing unreachable block (ram,0x0002b2b3)
// WARNING: Removing unreachable block (ram,0x0002b2b9)
// WARNING: Removing unreachable block (ram,0x0002b2bd)
// WARNING: Removing unreachable block (ram,0x0002b2c7)
// WARNING: Removing unreachable block (ram,0x0002b2d0)
// WARNING: Removing unreachable block (ram,0x0002b2d6)
// WARNING: Removing unreachable block (ram,0x0002b2dc)
// WARNING: Removing unreachable block (ram,0x0002b2e6)
// WARNING: Removing unreachable block (ram,0x0002b2f4)
// WARNING: Removing unreachable block (ram,0x0002b25c)
// WARNING: Removing unreachable block (ram,0x0002b263)
// WARNING: Removing unreachable block (ram,0x0002b272)
// WARNING: Removing unreachable block (ram,0x0002b309)
// WARNING: Removing unreachable block (ram,0x0002b30d)
// [RE-VERIFIED 2026-07-14 under current Sleigh grammar, live disassembly]
// tcu_rx_main_scheduler: (continuing prior 2026-07-12 re-verification of this function)
// EFEA (0xEFEA) claim CONFIRMED. Traced the F13C/F140/DE8 gating block (~0x2b0af-0x2b118):
// EFEA is either cleared to 0 or set to a fixed ROM constant (@0xDE8), gated by comparisons
// of F13C (current cycle value) against F140 (previous cycle value) and several DDE/DE0/DE2/DE6
// threshold constants. EFEA is only ever read as a boolean (tst.w @0xEFEA, != 0) at 0x2b106,
// never as a magnitude. This confirms logging.txt's REFUTED characterization: EFEA is a
// throttle/load-transient hold-flag, structurally identical to the already-confirmed EFC2
// mislabel, NOT a continuously-updated RPM value. RPM's real location remains unknown.
// 
// F0C0 claim NOT independently re-verifiable this session: logging.txt records only "one odd
// COMPUTED_CALL xref, no support either way" with no address given. No function or label
// references 0xF0C0 in the live program; a raw byte-pattern search for F0C0 returns 150+ hits,
// nearly all landing inside dense calibration/scaling-table data regions (0x11xxx-0x13xxx,
// 0x2dxxx), not code - same false-positive risk previously seen with MUT_83/MUT_E1. Without
// the original xref address there is no trail to re-derive. Status stays as logging.txt left
// it: unsupported, inconclusive.
// 
// [FOUND 2026-07-26] TCU (transmission control unit) SERIAL RX FRAME VALIDATOR
// (~0x2b160-0x2b1cc, guarded by f1f2 bit13 OR ram-bit 0x2c8, and
// tcu_rx_frame_state_counter_f4ce==5): a small checksummed receive frame --
// tcu_rx_frame_raw_byte0/1/2/3 (F4C4/F4C6/F4C8/F4CA) plus
// tcu_rx_frame_checksum_f4cc, validated via a simple additive checksum
// ((byte0+byte1+byte2+byte3)&0xFF == checksum). On a checksum match, the four
// raw bytes are latched into tcu_rx_confirmed_byte0-3_f4bc/f4be/f4c0/f4c2 --
// the "confirmed good" copies actually consumed elsewhere -- and
// tcu_rx_frame_timeout_counter_f4d2 is reset to 0. If tcu_rx_confirmed_byte2's
// (F4C0) low 3 bits are nonzero, sets F4A4 bit2 (a mode/request flag also
// gating canister_purge_duty_calc_f4ac and egr-adjacent logic elsewhere).
// tcu_rx_frame_timeout_counter_f4d2 increments each pass this block runs and
// forces re-init (via serial_fec3_load_byte0_init) if it exceeds ~0x31 or
// certain f0f8/f3f6 state changes -- a standard serial-link timeout/resync
// mechanism.
// 
// CONFIRMED GENUINELY SERIAL (not just an internal RAM protocol): one of the
// confirmed-copy cells, tcu_rx_confirmed_byte3_f4c2, is ALSO written directly
// by serial_fec5_byte_to_fifo (0x166bf) -- "fec5" sits inside the fec0-fed5
// SCI1/2/3 UART register range already confirmed elsewhere in this
// investigation (see f516_hibits_f520_f0f2_mode_select plate comment), so
// this is a real UART receive path, most likely a dedicated ECU<->TCU serial
// link distinct from the SCI1 diagnostic protocol
// (sci1_meta_cmd_dispatch_c0_ff) traced everywhere else in
// mut_verification_status.md.
// 
// DOWNSTREAM CONSUMERS (both confirmed, not hypothesized):
//   - tcu_rx_confirmed_byte0_f4bc (F4BC) and tcu_rx_confirmed_byte1_f4be
//     (F4BE) are read directly inside canister_purge_duty_calc_f4ac (0x19560)
//     as an addend and a mode-gate respectively -- i.e. data arriving over
//     this TCU link genuinely influences canister purge duty calculation,
//     not just transmission bookkeeping. Plausible real-world reason: purge
//     commonly gets suspended/modulated during transmission shift events.
//   - tcu_rx_confirmed_byte3_f4c2 (F4C2) is read by
//     tcu_link_confirmed_flag_f226_bit7_update (0x2223d, renamed 2026-07-26
//     from the descriptive-but-mechanism-only f226_bit7_update_from_f4c2_check),
//     which sets status flag F226 bit7 when (F4C2 & 0x60)==0x40, gated on ROM
//     config byte 0x102fd and the same F4A4 bit7 purge-enable flag. Likely a
//     "TCU link confirmed/handshake OK" status bit, not itself an actuator.
// 
// NOT YET DONE: characterize what tcu_rx_frame_raw_byte0/1_f4c4/f4c6
// (F4C4/F4C6, i.e. the OTHER two confirmed bytes, F4BC's and F4BE's raw
// sources) represent -- likely TCU-reported gear/shift-state or torque-
// converter data given the "tcu_" naming context of the parent function and
// neighboring tcu_shift_ratio_buffer_update/tcu_torque_converter_slip_calc
// functions documented elsewhere; identify which SCI channel (SCI2 vs SCI3)
// "fec5" actually belongs to and whether it's the same physical UART as the
// isr_ipu_ch2ch4_input_capture-adjacent TCU dispatch chain
// (tcu_periodic_dispatch @ 0x2c12b) found earlier in this project. See
// mut_verification_status.md for cross-references to the canister-purge and
// EGR actuator work this connects to.

void tcu_rx_main_scheduler(void)

{
  ushort *puVar1;
  short *psVar2;
  bool bVar3;
  char in_ram_000002c8;
  char in_ram_000002eb;
  ushort in_ram_000009bc;
  undefined2 in_ram_00000a98;
  ushort uVar4;
  short sVar5;
  undefined2 uVar6;
  ushort uVar7;
  short sVar8;
  short sVar9;
  char cVar10;
  ushort extraout_R2;
  ushort extraout_R2_00;
  ushort extraout_R2_01;
  ushort extraout_R2_02;
  ushort extraout_R2_03;
  ushort extraout_R2_04;
  ushort uVar11;
  byte bVar12;
  
  bVar12 = 0;
  sVar5 = DAT_0000f73c;
  DAT_0000f73c = sVar5 + 1;
  sVar5 = DAT_0000f2d6;
  if (sVar5 != 0) {
    sVar5 = DAT_0000f2d6;
    DAT_0000f2d6 = sVar5 + -1;
  }
  propagate_flag_fe96_to_f0d8_db();
  *(undefined2 *)((uint)bVar12 << 0x10 | 0xf106) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf104);
  *(undefined2 *)((uint)bVar12 << 0x10 | 0xf104) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf102);
  (*compose_status_word_f102)();
  uVar4 = (*f978_increment_on_ff73_bit0)();
  if (((((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x40) != 0) &&
       ((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 1) != 0)) &&
      ((*(ushort *)((uint)bVar12 << 0x10 | 0xf340) & 0x80) != 0)) &&
     (*(ushort *)((uint)bVar12 << 0x10 | 0xf13a) < 0xff)) {
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xf3cc) = 0;
    puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf3ce);
    *puVar1 = *puVar1 & 0xfffe;
    sVar5 = (*flag_read_ff43_bit0_inverted)();
    uVar4 = 0;
    if (sVar5 != 0) {
      uVar4 = timer_ff44_ff4a_arm_channels_atomic(*(ushort *)((uint)bVar12 << 0x10 | 0xf39e));
    }
  }
  if (*(char *)((uint)bVar12 << 0x10 | 0x2c2) != '\0') {
    if (*(short *)((uint)bVar12 << 0x10 | 0xf4fc) != 0) {
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf4fc);
      *psVar2 = *psVar2 + -1;
    }
    *(short *)((uint)bVar12 << 0x10 | 0xf4fa) = *(short *)((uint)bVar12 << 0x10 | 0xf4fa) << 1;
    if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x10) != 0) {
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf4fa);
      *puVar1 = *puVar1 | 1;
    }
    uVar4 = (ushort)((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x4000) != 0);
    if (uVar4 == ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4f8) & 0x20) != 0)) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4fe) = 0;
    }
    else if (*(short *)((uint)bVar12 << 0x10 | 0xf4fe) != -1) {
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf4fe);
      *psVar2 = *psVar2 + 1;
    }
    if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4f8) & 0x20) == 0) {
      if (*(ushort *)((uint)bVar12 << 0x10 | 0xf4fe) < 0x29) {
        if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4f8) & 0x80) == 0) {
          if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4f8) & 0x40) == 0) {
            sVar5 = *(short *)((uint)bVar12 << 0x10 | 0xf4fa);
            if ((sVar5 != 0) && (sVar5 != -1)) goto LAB_0002ab5b;
          }
          else {
            if (*(short *)((uint)bVar12 << 0x10 | 0xf4fa) != -1) {
LAB_0002ab5b:
              cVar10 = '\0';
              goto LAB_0002ab5d;
            }
            if (*(short *)((uint)bVar12 << 0x10 | 0xf4fc) != 0) {
              cVar10 = '\x03';
              goto LAB_0002ab5d;
            }
          }
LAB_0002ab57:
          cVar10 = '\x02';
        }
        else if (*(short *)((uint)bVar12 << 0x10 | 0xf4fc) == 0) {
          if (*(short *)((uint)bVar12 << 0x10 | 0xf4fe) != 0) goto LAB_0002ab2c;
          cVar10 = '\x04';
        }
        else {
          cVar10 = '\x01';
        }
      }
      else {
LAB_0002ab2c:
        cVar10 = '\x06';
      }
    }
    else {
      if (0x28 < *(ushort *)((uint)bVar12 << 0x10 | 0xf4fe)) goto LAB_0002ab57;
      cVar10 = '\x05';
    }
LAB_0002ab5d:
    if (cVar10 == '\x06') {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4f8) = 0x20;
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4fe) = 0;
LAB_0002ab8c:
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
      *puVar1 = *puVar1 | 0x10;
    }
    else {
      if (cVar10 != '\x05') {
        if (cVar10 == '\x02') {
          *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4fc) = 8;
        }
        else if (cVar10 != '\x01') {
          if (cVar10 == '\x04') {
            *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4fa) = 0xffff;
            *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4fc) = 0xc;
          }
          else if (cVar10 != '\x03') {
            *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4f8) = 0;
            puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
            *puVar1 = *puVar1 & 0xffef;
            puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
            *puVar1 = *puVar1 | 0x20;
            goto LAB_0002abc2;
          }
          *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4f8) = 0x40;
          goto LAB_0002abac;
        }
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4f8) = 0x80;
        goto LAB_0002ab8c;
      }
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4f8) = 0x20;
LAB_0002abac:
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
      *puVar1 = *puVar1 & 0xffef;
    }
    puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
    *puVar1 = *puVar1 & 0xffdf;
  }
LAB_0002abc2:
  uVar7 = DAT_0000f1f2;
  if ((uVar7 & 0x2000) != 0) {
    uVar4 = g_tcu_purge_duty_mode_select_f4a2;
    uVar4 = uVar4 & 3;
    if (uVar4 == 0) {
      sVar5 = g_tcu_purge_duty_target_f4a8;
      if (sVar5 != 0) {
        sVar5 = g_tcu_purge_duty_target_f4a8;
        g_tcu_purge_duty_target_f4a8 = sVar5 + -1;
      }
      uVar4 = g_tcu_purge_duty_target_f4a8;
      uVar7 = g_tcu_purge_duty_current_f4aa;
      uVar11 = g_tcu_purge_duty_current_f4aa;
      uVar6 = g_tcu_purge_duty_current_f4aa;
      if (uVar4 < uVar11 || uVar4 == uVar7) {
        uVar7 = g_status_flags_f0ea;
        g_status_flags_f0ea = uVar7 & 0xfffb;
        uVar7 = g_status_flags_f0ea;
        g_status_flags_f0ea = uVar7 | 8;
      }
    }
  }
  uVar7 = DAT_0000f1f2;
  if (((uVar7 & 0x2000) != 0) || (in_ram_000002c8 != '\0')) {
    serial_fec5_byte_to_fifo(uVar4);
  }
  if (in_ram_000002eb != '\0') {
    bVar12 = 0;
    sVar5 = DAT_0000f4f4;
    if (sVar5 == 0) {
      uVar4 = DAT_0000f0ec;
      DAT_0000f0ec = uVar4 & 0xfffe;
    }
    else {
      uVar4 = DAT_0000f0ec;
      DAT_0000f0ec = uVar4 | 1;
    }
    (*update_flag_fe82_bit2_cond)();
    if (*(short *)((uint)bVar12 << 0x10 | 0xf4f4) != 0) {
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf4f4);
      *psVar2 = *psVar2 + -1;
    }
  }
  bVar12 = 0;
  uVar4 = DAT_0000f73c;
  if ((uVar4 & 1) == 0) {
    sVar5 = DAT_0000f1ec;
    if (sVar5 != 0) {
      sVar5 = DAT_0000f1ec;
      DAT_0000f1ec = sVar5 + -1;
    }
    sVar5 = DAT_0000f1e2;
    if (sVar5 != 0) {
      sVar5 = DAT_0000f1e2;
      DAT_0000f1e2 = sVar5 + -1;
    }
    sVar5 = DAT_0000f1ee;
    if (sVar5 != 0) {
      sVar5 = DAT_0000f1ee;
      DAT_0000f1ee = sVar5 + -1;
    }
    uVar4 = g_status_flags_f20a;
    uVar7 = g_status_word_f102;
    if ((uVar7 & 2) == 0) {
      uVar4 = uVar4 & 0x7fff;
    }
    else {
      uVar4 = uVar4 | 0x8000;
    }
    uVar7 = g_status_flags_f20a;
    if (((uVar7 ^ uVar4) & 0x8000) == 0) {
      sVar5 = DAT_0000f1e2;
      if (sVar5 == 0) {
        DAT_0000f1e4 = 0xffff;
      }
    }
    else {
      DAT_0000f1ec = in_ram_00000a98;
      if ((uVar4 & 0x8000) != 0) {
        uVar4 = DAT_0000f1e2;
        uVar4 = sat_sub_u16(0xe2,uVar4);
        *(ushort *)((uint)bVar12 << 0x10 | 0xf1e4) = uVar4;
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf1e2) = 0xe2;
        uVar4 = extraout_R2;
      }
    }
    *(ushort *)((uint)bVar12 << 0x10 | 0xf20a) = uVar4;
    if (*(short *)((uint)bVar12 << 0x10 | 0xf1ee) == 0) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf1ee) = 0x11b;
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf1ea) =
           *(undefined2 *)((uint)bVar12 << 0x10 | 0xf1e8);
      uVar6 = (*f978_read_and_clear)();
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf1e8) = uVar6;
    }
    if (*(short *)((uint)bVar12 << 0x10 | 0xf590) != -1) {
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf590);
      *psVar2 = *psVar2 + 1;
    }
    if (*(short *)((uint)bVar12 << 0x10 | 0xf592) != -1) {
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf592);
      *psVar2 = *psVar2 + 1;
    }
    if (*(short *)((uint)bVar12 << 0x10 | 0xf598) != 0) {
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf598);
      *psVar2 = *psVar2 + -1;
    }
    (*sci1_protocol_state_machine)();
  }
  if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf73c) & 3) == 0) {
    (*adc_read_sequence_b)(0);
    sat_sub_u16(*(short *)((uint)bVar12 << 0x10 | 0xf13c) +
                *(short *)((uint)bVar12 << 0x10 | 0xee82),0x80);
    uVar6 = (*clamp_u8)();
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xf13e) = uVar6;
    if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf1f4) & 8) != 0) {
      uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf13c);
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xaae);
      if (*puVar1 <= uVar4 && uVar4 != *puVar1) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xee88);
        *puVar1 = *puVar1 & 0xfffb;
      }
      if (((((*(ushort *)((uint)bVar12 << 0x10 | 0xf25a) & 0x11) == 0) &&
           (uVar7 = *(ushort *)((uint)bVar12 << 0x10 | 0xef6a),
           puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xab6), *puVar1 <= uVar7 && uVar7 != *puVar1))
          && (uVar7 = *(ushort *)((uint)bVar12 << 0x10 | 0xf13a),
             puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xab4), *puVar1 <= uVar7 && uVar7 != *puVar1
             )) && (((*(ushort *)((uint)bVar12 << 0x10 | 0xab0) <= uVar4 &&
                     (puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xaae),
                     uVar4 < *puVar1 || uVar4 == *puVar1)) &&
                    ((*(ushort *)((uint)bVar12 << 0x10 | 0xf0f8) & 4) == 0)))) {
        bVar3 = true;
        if ((((*(ushort *)((uint)bVar12 << 0x10 | 0xee88) & 2) == 0) &&
            ((*(ushort *)((uint)bVar12 << 0x10 | 0xf73c) & 0x7f) == 0)) &&
           (puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xee86), uVar4 < *puVar1 || uVar4 == *puVar1)
           ) {
          uVar4 = sat_sub_u16(*(ushort *)((uint)bVar12 << 0x10 | 0xee86),
                              *(ushort *)((uint)bVar12 << 0x10 | 0xaba));
          if (uVar4 < extraout_R2_00) {
            *(ushort *)((uint)bVar12 << 0x10 | 0xee86) = extraout_R2_00;
          }
          else {
            *(ushort *)((uint)bVar12 << 0x10 | 0xee86) = uVar4;
          }
        }
      }
      else {
        bVar3 = false;
      }
      uVar7 = sat_add_u16(*(ushort *)((uint)bVar12 << 0x10 | 0xf16c),
                          *(ushort *)((uint)bVar12 << 0x10 | 0xab8));
      uVar4 = extraout_R2_01;
      if (((bVar3) && (*(ushort *)((uint)bVar12 << 0x10 | 0xf16c) <= extraout_R2_01)) &&
         (extraout_R2_01 <= uVar7)) {
        if (*(short *)((uint)bVar12 << 0x10 | 0xef9a) == 0) {
          *(undefined2 *)((uint)bVar12 << 0x10 | 0xef9a) =
               *(undefined2 *)((uint)bVar12 << 0x10 | 0xab2);
          if (*(ushort *)((uint)bVar12 << 0x10 | 0xee86) <
              *(ushort *)((uint)bVar12 << 0x10 | 0xf16c)) {
            if ((*(ushort *)((uint)bVar12 << 0x10 | 0xee88) & 4) != 0) goto LAB_0002ae42;
            *(ushort *)((uint)bVar12 << 0x10 | 0xee88) =
                 *(ushort *)((uint)bVar12 << 0x10 | 0xee88) | 6;
            uVar7 = sat_add_u16(*(ushort *)((uint)bVar12 << 0x10 | 0xee86),1);
            uVar4 = extraout_R2_02;
            if (uVar7 < *(ushort *)((uint)bVar12 << 0x10 | 0xf16c)) {
              *(ushort *)((uint)bVar12 << 0x10 | 0xee86) = uVar7;
              goto LAB_0002ae3e;
            }
          }
          else {
            puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xee88);
            *puVar1 = *puVar1 | 2;
          }
          *(undefined2 *)((uint)bVar12 << 0x10 | 0xee86) =
               *(undefined2 *)((uint)bVar12 << 0x10 | 0xf16c);
          goto LAB_0002ae3e;
        }
      }
      else {
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xef9a) =
             *(undefined2 *)((uint)bVar12 << 0x10 | 0xab2);
LAB_0002ae3e:
        *(ushort *)((uint)bVar12 << 0x10 | 0xf16c) = uVar4;
      }
LAB_0002ae42:
      uVar4 = sat_add_u16(*(ushort *)((uint)bVar12 << 0x10 | 0xee86),
                          *(ushort *)((uint)bVar12 << 0x10 | 0xaaa));
      uVar7 = sat_sub_u16(uVar4,*(ushort *)((uint)bVar12 << 0x10 | 0xaac));
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xee88) & 8) == 0) {
        if (uVar4 < extraout_R2_03) {
          puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xee88);
          *puVar1 = *puVar1 | 8;
        }
      }
      else if (extraout_R2_03 <= uVar7) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xee88);
        *puVar1 = *puVar1 & 0xfff7;
      }
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf1fe) & 0x40) == 0) {
        if ((*(ushort *)((uint)bVar12 << 0x10 | 0xee88) & 8) == 0) {
          puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xee88);
          *puVar1 = *puVar1 | 1;
LAB_0002aea6:
          puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
          *puVar1 = *puVar1 | 0x400;
          goto LAB_0002aeb0;
        }
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xee88);
        *puVar1 = *puVar1 & 0xfffe;
      }
      else {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xee88);
        *puVar1 = *puVar1 & 0xfffe;
        uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf13c);
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xabe);
        if (*puVar1 <= uVar4 && uVar4 != *puVar1) goto LAB_0002aea6;
      }
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
      *puVar1 = *puVar1 & 0xfbff;
    }
LAB_0002aeb0:
    increment_counters_in_range(0xef64,0xef6a);
    uVar4 = (*decrement_counters_in_range)();
    if (*(short *)((uint)bVar12 << 0x10 | 0xef8c) == 0) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xef8c) = 5;
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf73e);
      *psVar2 = *psVar2 + 1;
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
      *puVar1 = *puVar1 | 1;
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf73e) & 1) == 0) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
        *puVar1 = *puVar1 | 2;
      }
      uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf73e) & 3;
      if (uVar4 == 0) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
        *puVar1 = *puVar1 | 4;
      }
    }
    if (*(short *)((uint)bVar12 << 0x10 | 0xef8e) == 0) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xef8e) = 100;
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf740);
      *psVar2 = *psVar2 + 1;
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
      *puVar1 = *puVar1 | 8;
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf740) & 1) == 0) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
        *puVar1 = *puVar1 | 0x10;
      }
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf740) & 3) == 0) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
        *puVar1 = *puVar1 | 0x20;
      }
      uVar4 = DAT_0000f336;
      uVar6 = DAT_0000f338;
      DAT_0000f33a = uVar4;
      DAT_0000f33c = uVar6;
      DAT_0000f336 = 0;
      DAT_0000f338 = 0;
      bVar12 = 0;
    }
    if (*(short *)((uint)bVar12 << 0x10 | 0xef98) == 0) {
      bVar12 = 0;
      peripheral_regs_ff2x_ff9x_bulk_zero_init(uVar4);
    }
    if (*(short *)((uint)bVar12 << 0x10 | 0xef7a) != 0) {
      (*toggle_flag_fe8b_bit4)();
    }
    if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf1f2) & 0x80) != 0) {
      if (*(short *)((uint)bVar12 << 0x10 | 0xef88) == 0) {
        sVar5 = DAT_0000f3a2;
        if (sVar5 != 0) {
          sVar5 = DAT_0000f3a2;
          DAT_0000f3a2 = sVar5 + -1;
        }
        bVar12 = 0;
      }
      uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xb1c);
      if (*(short *)((uint)bVar12 << 0x10 | 0xefc2) != 0) {
        uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xb1e);
      }
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf3a4) & 0x80) == 0) {
        uVar4 = 2;
      }
      if ((*(short *)((uint)bVar12 << 0x10 | 0xef88) == 0) ||
         (uVar4 < *(ushort *)((uint)bVar12 << 0x10 | 0xef88))) {
        *(ushort *)((uint)bVar12 << 0x10 | 0xef88) = uVar4;
      }
    }
    if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf3f6) & 0x80) == 0) {
                    // Battery voltage (f13a, confirmed MUT Battery/RequestID 0x14 via
                    // adc_read_sequence_b @0x15689) hysteresis comparator: sets f3f6 bit7 when f13a
                    // > ROM threshold @0xd50 (this instruction), clears it when f13a <= ROM
                    // threshold @0xd4e (see 0x2afc4 just above). Same pattern independently found
                    // in VR4 at 0x2b4ab/0xf078/0xf2d2 bit7 -- VR4 uses the SAME ROM threshold
                    // addresses 0xd4e/0xd50 (byte-identical, not just same offset pattern), strong
                    // cross-confirmation this is a real, shared charging-system/battery-voltage
                    // status flag, not coincidental structural similarity. Likely
                    // "battery/alternator voltage OK" status bit. See project chat 2026-08-15.
      uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf13a);
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xd50);
      if (*puVar1 <= uVar4 && uVar4 != *puVar1) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf3f6);
        *puVar1 = *puVar1 | 0x80;
      }
    }
    else {
      uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf13a);
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xd4e);
      if (uVar4 < *puVar1 || uVar4 == *puVar1) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf3f6);
        *puVar1 = *puVar1 & 0xff7f;
      }
    }
    if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf3f6) & 0x80) == 0) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf3f8) = 0;
    }
    else {
      uVar4 = 3;
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xeed0) & 0x80) != 0) {
        uVar4 = 4;
      }
      if (*(short *)((uint)bVar12 << 0x10 | 0xf3f8) != -1) {
        psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf3f8);
        *psVar2 = *psVar2 + 1;
      }
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf3f8);
      if (((uVar4 < *puVar1 || uVar4 == *puVar1) &&
          (sVar5 = (*swap_invert_high_byte)(), sVar5 == *(short *)((uint)bVar12 << 0x10 | 0xeed4)))
         && (extraout_R2_04 != *(ushort *)((uint)bVar12 << 0x10 | 0xf40a))) {
        if (extraout_R2_04 < *(ushort *)((uint)bVar12 << 0x10 | 0xf40a)) {
          psVar2 = (short *)((uint)bVar12 << 0x10 | 0xeed2);
          *psVar2 = *psVar2 + 1;
        }
        else {
          psVar2 = (short *)((uint)bVar12 << 0x10 | 0xeed2);
          *psVar2 = *psVar2 + -1;
        }
        bVar12 = 0;
        uVar6 = (*swap_invert_high_byte)();
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xeed4) = uVar6;
        (*update_port_fe82_indexed)();
        bVar12 = 0;
        DAT_0000f3f8 = 1;
      }
    }
    uVar7 = 0;
    uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf13e);
    if (*(char *)((uint)bVar12 << 0x10 | 0x2f8) == '\x01') {
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf20a) & 0x1000) == 0) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xde2);
        if (*puVar1 <= uVar4 && uVar4 != *puVar1) goto LAB_0002b08b;
      }
      else {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xde4);
        if (uVar4 < *puVar1 || uVar4 == *puVar1) goto LAB_0002b091;
      }
    }
    else if (uVar4 < *(ushort *)((uint)bVar12 << 0x10 | 0xde2)) {
LAB_0002b091:
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
      *puVar1 = *puVar1 & 0xefff;
    }
    else {
LAB_0002b08b:
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
      *puVar1 = *puVar1 | 0x1000;
    }
    if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf25a) & 0x10) == 0) {
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf17a) < *(ushort *)((uint)bVar12 << 0x10 | 0xdea))
         || (*(ushort *)((uint)bVar12 << 0x10 | 0xf186) < *(ushort *)((uint)bVar12 << 0x10 | 0xdec))
         ) {
        if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf1f4) & 8) == 0) {
          uVar7 = *(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x80;
        }
        else {
          uVar7 = *(ushort *)((uint)bVar12 << 0x10 | 0xee88) & 1;
        }
      }
      if (uVar4 < *(ushort *)((uint)bVar12 << 0x10 | 0xdde)) {
        if (uVar4 < *(ushort *)((uint)bVar12 << 0x10 | 0xde0)) {
          if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf20a) & 0x1000) == 0) {
            if (*(ushort *)((uint)bVar12 << 0x10 | 0xf72) <=
                *(ushort *)((uint)bVar12 << 0x10 | 0xf13c)) goto LAB_0002b118;
          }
          else if (*(short *)((uint)bVar12 << 0x10 | 0xefea) != 0) {
LAB_0002b118:
            uVar7 = 1;
          }
        }
        else {
          if (*(ushort *)((uint)bVar12 << 0x10 | 0xf140) <= uVar4) goto LAB_0002b0e1;
          if (*(ushort *)((uint)bVar12 << 0x10 | 0xde6) <=
              *(short *)((uint)bVar12 << 0x10 | 0xf140) - uVar4) {
            uVar7 = 1;
            *(undefined2 *)((uint)bVar12 << 0x10 | 0xefea) =
                 *(undefined2 *)((uint)bVar12 << 0x10 | 0xde8);
          }
        }
      }
      else if (*(ushort *)((uint)bVar12 << 0x10 | 0xf140) <= uVar4) goto LAB_0002b0e1;
    }
    else {
LAB_0002b0e1:
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xefea) = 0;
    }
    if (uVar7 == 0) {
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
      *puVar1 = *puVar1 & 0xfffe;
    }
    else {
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
      *puVar1 = *puVar1 | 1;
    }
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xf140) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf13e);
    if ((((*(ushort *)((uint)bVar12 << 0x10 | 0xf1f2) & 0x2000) != 0) ||
        (*(char *)((uint)bVar12 << 0x10 | 0x2c8) != '\0')) &&
       (*(short *)((uint)bVar12 << 0x10 | 0xef8a) == 0)) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xef8a) = 4;
      uVar4 = *(short *)((uint)bVar12 << 0x10 | 0xf4d2) + 1;
      *(ushort *)((uint)bVar12 << 0x10 | 0xf4d2) = uVar4;
      if (uVar4 == 0) {
        psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf4d2);
        *psVar2 = *psVar2 + -1;
      }
      if ((((*(ushort *)((uint)bVar12 << 0x10 | 0xf4a4) & 0x80) == 0) &&
          (*(short *)((uint)bVar12 << 0x10 | 0xf4ce) == 5)) &&
         (uVar4 = *(short *)((uint)bVar12 << 0x10 | 0xf4c4) +
                  *(short *)((uint)bVar12 << 0x10 | 0xf4c6) +
                  *(short *)((uint)bVar12 << 0x10 | 0xf4c8) +
                  *(short *)((uint)bVar12 << 0x10 | 0xf4ca) & 0xff,
         uVar4 == *(ushort *)((uint)bVar12 << 0x10 | 0xf4cc))) {
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4bc) =
             *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4c4);
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4be) =
             *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4c6);
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4c0) =
             *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4c8);
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4c2) =
             *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4ca);
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4d2) = 0;
        uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf4c0) & 7;
        if (uVar4 != 0) {
          puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf4a4);
          *puVar1 = *puVar1 | 4;
        }
      }
      if ((((*(ushort *)((uint)bVar12 << 0x10 | 0xf0f8) & 2) == 0) ||
          ((*(ushort *)((uint)bVar12 << 0x10 | 0xf0f8) & 0x40) != 0)) ||
         ((*(ushort *)((uint)bVar12 << 0x10 | 0xf3f6) & 0x80) == 0)) {
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xeff2) = 0x14;
LAB_0002b1d0:
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4d2) = 0;
      }
      else if (*(short *)((uint)bVar12 << 0x10 | 0xeff2) != 0) goto LAB_0002b1d0;
      if (0x31 < *(ushort *)((uint)bVar12 << 0x10 | 0xf4d2)) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf4a4);
        *puVar1 = *puVar1 | 0x80;
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4bc) = 0x80;
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4c0) = 0;
        *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4c2) = 0;
      }
      if (*(short *)((uint)bVar12 << 0x10 | 0xef90) == 0) {
        uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf4a4) & 0xff3e;
        *(ushort *)((uint)bVar12 << 0x10 | 0xf4a4) = uVar4;
      }
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4ce) = 0;
      serial_fec3_load_byte0_init(uVar4);
    }
    detect_sci1_param_change_reset();
  }
  if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf73c) & 7) != 0) goto LAB_0002b6ea;
  if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf1f2) & 0x2000) != 0) {
    if (*(short *)((uint)bVar12 << 0x10 | 0xf4b2) != 0) {
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf4b2);
      *psVar2 = *psVar2 + -1;
    }
    if (((*(ushort *)((uint)bVar12 << 0x10 | 0xf4be) & 0xc0) != 0xc0) &&
       ((*(short *)((uint)bVar12 << 0x10 | 0xf4b2) != 0 ||
        ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4a6) & 0x80) == 0)))) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4b2) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xe5c)
      ;
    }
    if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4a2) & 3) == 0) {
      if (((*(ushort *)((uint)bVar12 << 0x10 | 0xf3f6) & 0x80) != 0) &&
         ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4ae) & 0x20) != 0)) {
        uVar7 = 0;
        uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf4a8);
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf4aa);
        if (uVar4 < *puVar1 || uVar4 == *puVar1) {
          if (8 < (ushort)(*(short *)((uint)bVar12 << 0x10 | 0xf4aa) -
                          *(short *)((uint)bVar12 << 0x10 | 0xf4a8))) {
            uVar7 = 6;
          }
        }
        else {
          uVar7 = 3;
          if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4a6) & 0x80) != 0) {
            uVar7 = 0xc;
          }
        }
        if (uVar7 != 0) {
          uVar4 = 0;
          if (((uVar7 & 8) != 0) && ((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x100) == 0)) {
            uVar4 = 8;
          }
          if (((uVar7 & 4) != 0) && ((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x200) == 0)) {
            uVar4 = uVar4 | 4;
          }
          if (((uVar7 & 2) != 0) && ((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x100) != 0)) {
            uVar4 = uVar4 | 2;
          }
          if (((uVar7 & 1) != 0) && ((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x200) != 0)) {
            uVar4 = uVar4 | 1;
          }
          *(ushort *)((uint)bVar12 << 0x10 | 0xf4ae) =
               *(ushort *)((uint)bVar12 << 0x10 | 0xf4ae) |
               *(ushort *)((uint)bVar12 << 0x10 | 0xf4b0) & uVar4;
          *(ushort *)((uint)bVar12 << 0x10 | 0xf4b0) = uVar4;
          if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4ae) & 0x80) == 0) {
            puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf4ae);
            *puVar1 = *puVar1 & 0xffbf;
          }
          else {
            puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf4ae);
            *puVar1 = *puVar1 & 0xff7f;
          }
        }
      }
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf224) & 0x100) != 0) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf4ae);
        *puVar1 = *puVar1 | 0x20;
      }
      if (((*(ushort *)((uint)bVar12 << 0x10 | 0xf4ae) & 0xc0) == 0) ||
         ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4ae) & 0x20) == 0)) {
        if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4a2) & 0x80) == 0) {
          uVar4 = 0x18;
          uVar7 = 0;
          uVar11 = 0;
        }
        else {
          if (*(ushort *)((uint)bVar12 << 0x10 | 0xf13e) <
              *(ushort *)((uint)bVar12 << 0x10 | 0xf4ac)) {
            uVar11 = 0;
            uVar7 = *(short *)((uint)bVar12 << 0x10 | 0xf4ac) -
                    *(short *)((uint)bVar12 << 0x10 | 0xf13e);
            uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xe52);
          }
          else {
            uVar11 = 0x80;
            uVar7 = *(short *)((uint)bVar12 << 0x10 | 0xf13e) -
                    *(short *)((uint)bVar12 << 0x10 | 0xf4ac);
            uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xe54);
          }
          cVar10 = '\0';
          if (uVar4 < uVar7) {
            if (*(short *)((uint)bVar12 << 0x10 | 0xf4a8) == 0) {
              cVar10 = '\x02';
            }
            else if (uVar11 == *(ushort *)((uint)bVar12 << 0x10 | 0xf4a6)) {
              cVar10 = '\x01';
            }
          }
          if (cVar10 == '\0') {
            uVar4 = 0;
            uVar7 = 0x18;
          }
          else {
            if (cVar10 != '\x01') {
              *(ushort *)((uint)bVar12 << 0x10 | 0xf0be) = uVar7;
              (*axis_lookup_interp)();
              if (uVar11 == 0) {
                uVar7 = table_lookup_interp(1,(void *)0x2cbc);
                if (*(short *)((uint)bVar12 << 0x10 | 0xf4b2) == 0) {
                  uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xe5a);
                }
                else {
                  uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xe56);
                }
              }
              else {
                uVar7 = table_lookup_interp(1,(void *)0x2cd2);
                uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xe58);
              }
              if (uVar11 != *(ushort *)((uint)bVar12 << 0x10 | 0xf4a6)) {
                uVar7 = sat_sub_u16(uVar7,uVar4);
              }
              uVar4 = 0x18;
              goto LAB_0002b525;
            }
            uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf4a8);
            uVar7 = *(ushort *)((uint)bVar12 << 0x10 | 0xf4aa);
          }
          uVar11 = *(ushort *)((uint)bVar12 << 0x10 | 0xf4a6);
        }
      }
      else {
        uVar4 = 0x18;
        uVar7 = 0;
        uVar11 = 0x80;
      }
LAB_0002b525:
      *(ushort *)((uint)bVar12 << 0x10 | 0xf4a8) = uVar4;
      *(ushort *)((uint)bVar12 << 0x10 | 0xf4aa) = uVar7;
      *(ushort *)((uint)bVar12 << 0x10 | 0xf4a6) = uVar11;
      if ((uVar7 < 0x18) && (uVar7 < uVar4)) {
        if ((uVar11 & 0x80) == 0) {
          puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
          *puVar1 = *puVar1 & 0xfffb;
          puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
          *puVar1 = *puVar1 & 0xfff7;
        }
        else {
          puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
          *puVar1 = *puVar1 | 4;
          puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
          *puVar1 = *puVar1 | 8;
        }
      }
    }
    else {
      if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf4a2) & 2) == 0) {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
        *puVar1 = *puVar1 & 0xfffb;
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
        *puVar1 = *puVar1 | 8;
      }
      else {
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
        *puVar1 = *puVar1 | 4;
        puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
        *puVar1 = *puVar1 & 0xfff7;
      }
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4a8) = 0;
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4aa) = 0;
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4a6) = 0;
    }
  }
  if ((*(char *)((uint)bVar12 << 0x10 | 0x2ea) != '\0') &&
     (*(char *)((uint)bVar12 << 0x10 | 0x2c8) == '\0')) {
    uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf4d6);
    if ((uVar4 & 0x10) == 0) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf4d4) = 0;
    }
    else if (*(short *)((uint)bVar12 << 0x10 | 0xf4d4) != 0) {
      psVar2 = (short *)((uint)bVar12 << 0x10 | 0xf4d4);
      *psVar2 = *psVar2 + -1;
    }
    if ((uVar4 & 0x13) == 0) {
LAB_0002b58c:
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
      *puVar1 = *puVar1 & 0xfffd;
    }
    else {
      if ((uVar4 & 2) == 0) {
        if (*(short *)((uint)bVar12 << 0x10 | 0xf4d4) != 0) goto LAB_0002b596;
        if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf0ea) & 2) != 0) goto LAB_0002b58c;
      }
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf0ea);
      *puVar1 = *puVar1 | 2;
    }
  }
LAB_0002b596:
  if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf1f2) & 0x80) != 0) {
    bVar12 = 0;
    uVar6 = DAT_0000f3ae;
    (*mul_u16_hi)();
    uVar6 = (*div_s32_s16_rounded)();
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xf3ae) = uVar6;
    sVar8 = (*o2_sensor_gain_divisor)();
    sVar5 = (*o2_lambda_or_passthrough_select)();
    sVar9 = 4;
    do {
      sVar5 = sVar5 << 1;
      if (sVar5 == 0) break;
      sVar9 = sVar9 + -1;
    } while (sVar9 != -1);
    (*mul_u16_hi)();
    sVar5 = (*inc_if_signed_flag)();
    uVar4 = clamp_u16(sVar5 + sVar8,0xff,1);
    *(ushort *)((uint)bVar12 << 0x10 | 0xf3aa) = uVar4;
    bVar12 = 0;
  }
  if (*(ushort *)((uint)bVar12 << 0x10 | 0xf13c) < *(ushort *)((uint)bVar12 << 0x10 | 0xf142)) {
    uVar4 = *(short *)((uint)bVar12 << 0x10 | 0xf142) - *(short *)((uint)bVar12 << 0x10 | 0xf13c);
    puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf152);
    if (*puVar1 <= uVar4 && uVar4 != *puVar1) {
      *(ushort *)((uint)bVar12 << 0x10 | 0xf152) = uVar4;
    }
    uVar4 = 0;
  }
  else {
    uVar4 = *(short *)((uint)bVar12 << 0x10 | 0xf13c) - *(short *)((uint)bVar12 << 0x10 | 0xf142);
    puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf14e);
    if (*puVar1 <= uVar4 && uVar4 != *puVar1) {
      *(ushort *)((uint)bVar12 << 0x10 | 0xf14e) = uVar4;
    }
  }
  if (*(ushort *)((uint)bVar12 << 0x10 | 0xb20) <= uVar4) {
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xefc2) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xb24);
  }
  if ((((*(ushort *)((uint)bVar12 << 0x10 | 0xf33e) & 0x40) == 0) &&
      (*(ushort *)((uint)bVar12 << 0x10 | 0xade) <= uVar4)) &&
     (*(ushort *)((uint)bVar12 << 0x10 | 0xf366) <= uVar4)) {
    *(ushort *)((uint)bVar12 << 0x10 | 0xf366) = uVar4;
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xf368) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xae2);
  }
  if (*(ushort *)((uint)bVar12 << 0x10 | 0xf14a) < *(ushort *)((uint)bVar12 << 0x10 | 0xf13c)) {
    uVar4 = *(short *)((uint)bVar12 << 0x10 | 0xf13c) - *(short *)((uint)bVar12 << 0x10 | 0xf14a);
  }
  else {
    uVar4 = *(short *)((uint)bVar12 << 0x10 | 0xf14a) - *(short *)((uint)bVar12 << 0x10 | 0xf13c);
  }
  if (*(ushort *)((uint)bVar12 << 0x10 | 0xa6e) <= uVar4) {
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xf028) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xa70);
  }
  *(undefined2 *)((uint)bVar12 << 0x10 | 0xf14a) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf148);
  *(undefined2 *)((uint)bVar12 << 0x10 | 0xf148) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf146);
  *(undefined2 *)((uint)bVar12 << 0x10 | 0xf146) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf144);
  *(undefined2 *)((uint)bVar12 << 0x10 | 0xf144) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf142);
  *(undefined2 *)((uint)bVar12 << 0x10 | 0xf142) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf13c);
LAB_0002b6ea:
  if (*(short *)((uint)bVar12 << 0x10 | 0xef86) == 0) {
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xf312) = 0;
  }
  if ((*(ushort *)((uint)bVar12 << 0x10 | 0xf73c) & 7) == 0) {
    uVar4 = sat_sub_u16(*(ushort *)((uint)bVar12 << 0x10 | 0xf13c),
                        *(ushort *)((uint)bVar12 << 0x10 | 0xf31a));
    *(ushort *)((uint)bVar12 << 0x10 | 0xf318) = uVar4;
    puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
    *puVar1 = *puVar1 & 0xdfff;
    if ((((*(ushort *)((uint)bVar12 << 0x10 | 0x9ba) <= *(ushort *)((uint)bVar12 << 0x10 | 0xf318))
         && (*(ushort *)((uint)bVar12 << 0x10 | 0xf31a) < *(ushort *)((uint)bVar12 << 0x10 | 0xf316)
            )) && ((*(ushort *)((uint)bVar12 << 0x10 | 0xf312) <
                    *(ushort *)((uint)bVar12 << 0x10 | 0xf310) &&
                   ((*(ushort *)((uint)bVar12 << 0x10 | 0xf25a) & 0x11) == 0)))) &&
       ((((*(ushort *)((uint)bVar12 << 0x10 | 0xf1f4) & 8) != 0 &&
         ((*(ushort *)((uint)bVar12 << 0x10 | 0xee88) & 1) == 0)) ||
        (((*(ushort *)((uint)bVar12 << 0x10 | 0xf1f4) & 8) == 0 &&
         ((*(ushort *)((uint)bVar12 << 0x10 | 0xf102) & 0x80) == 0)))))) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xef86) = *(undefined2 *)((uint)bVar12 << 0x10 | 0x9b8)
      ;
      puVar1 = (ushort *)((uint)bVar12 << 0x10 | 0xf20a);
      *puVar1 = *puVar1 | 0x2000;
      uVar4 = *(ushort *)((uint)bVar12 << 0x10 | 0xf318) >> 2;
      if (7 < uVar4) {
        uVar4 = 8;
      }
      bVar12 = 0;
      sVar5 = DAT_0000f314;
      uVar4 = (ushort)*(byte *)((int)uVar4 + 0x1ebe) * sVar5;
      sVar5 = 6;
      do {
        uVar4 = uVar4 >> 1;
        if (uVar4 == 0) break;
        sVar5 = sVar5 + -1;
      } while (sVar5 != -1);
      if (in_ram_000009bc <= uVar4) {
        uVar4 = in_ram_000009bc;
      }
      DAT_0000f30e = uVar4;
      sVar5 = DAT_0000f312;
      DAT_0000f312 = sVar5 + uVar4;
      if (uVar4 != 0) {
        sVar5 = 5;
        do {
          uVar4 = uVar4 << 1;
          if (uVar4 == 0) break;
          sVar5 = sVar5 + -1;
        } while (sVar5 != -1);
        (*f2cc_offset_accumulate)();
        uVar6 = DAT_0000f5ec;
        compose_fe86_fe8a_status_nibble();
        (*channel_enable_mask_build_and_dispatch_ff2x_ff3x)();
        bVar12 = 0;
      }
    }
    *(undefined2 *)((uint)bVar12 << 0x10 | 0xf31a) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xf13c);
    if (*(ushort *)((uint)bVar12 << 0x10 | 0x9ba) <= *(ushort *)((uint)bVar12 << 0x10 | 0xf318)) {
      *(undefined2 *)((uint)bVar12 << 0x10 | 0xf028) = *(undefined2 *)((uint)bVar12 << 0x10 | 0xa70)
      ;
    }
  }
  if (*(char *)((uint)bVar12 << 0x10 | 0x2e2) != '\0') {
    isc_activation_control_f466(*(ushort *)((uint)bVar12 << 0x10 | 0xf73c) & 0xf);
  }
  (*mirror_status_f0ea_to_ports)();
  (*software_pwm_output_scheduler)();
  (*channel_event_or_periodic_dispatch)();
  return;
}



void tcu_gear_shift_history_reset(void)

{
  g_tcu_gear_ratio_hist_slot2_lo = 0x8000;
  g_tcu_gear_ratio_hist_slot2_hi = 0;
  DAT_0001ef08 = 0x8000;
  DAT_0001ef0a = 0;
  g_tcu_gear_ratio_hist_slot0_lo = 0x8000;
  DAT_0001ef06 = 0;
  DAT_0001ef18 = 0x8000;
  DAT_0001ef1a = 0;
  DAT_0001ef14 = 0x8000;
  DAT_0001ef16 = 0;
  DAT_0001ef10 = 0x8000;
  DAT_0001ef12 = 0;
  g_tcu_gear_ratio_hist_status_flags = 0;
  DAT_0001ef1e = 0;
  DAT_0001ef52 = 0;
  return;
}



void tcu_diagnostic_state_cold_init(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  ushort *puVar1;
  byte bVar2;
  
  bVar2 = 1;
  DAT_0001f730 = 0xffff;
  DAT_0001f626 = 0xffff;
  DAT_0001f624 = 0xffff;
  DAT_0001f622 = 0xffff;
  DAT_0001f620 = 0xffff;
  DAT_0001f62e = 0xffff;
  DAT_0001f62c = 0xffff;
  DAT_0001f62a = 0xffff;
  DAT_0001f628 = 0xffff;
  DAT_0001f656 = 0xff;
  DAT_0001f654 = 0xff;
  DAT_0001f652 = 0xff;
  DAT_0001f64e = 0xff;
  DAT_0001f658 = 0xff;
  DAT_0001f650 = 0xff;
  DAT_0001f65c = 0xff;
  if (DAT_00010300 != '\0') {
    DAT_0001f664 = 0xff;
    DAT_0001f662 = 0xff;
  }
  if (DAT_00010301 != '\0') {
    DAT_0001f66c = 0xff;
    DAT_0001f66a = 0xff;
    DAT_0001f668 = 0xff;
    DAT_0001f666 = 0xff;
  }
  if (DAT_00010316 != '\0') {
    DAT_0001f670 = 0xff;
    DAT_0001f66e = 0xff;
  }
  if (DAT_00010314 != '\0') {
    DAT_0001f65e = 0xff;
  }
  if (DAT_00010310 != '\0') {
    DAT_0001f734 = 0;
    DAT_0001f732 = 0;
  }
  DAT_0001f04e = DAT_00011006;
  DAT_0001f050 = DAT_00011006;
  DAT_0001f642 = DAT_00011008;
  DAT_0001f644 = DAT_00011008;
  coldinit_f6c0_f7c8_state_clear_and_intc_flag_read(param_3);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf73a) = 0;
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf738) = 0;
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf736) = 0;
  puVar1 = (ushort *)((uint)bVar2 << 0x10 | 0xf6c8);
  *puVar1 = *puVar1 & 0xffbf;
  *(ushort *)((uint)bVar2 << 0x10 | 0xf64a) = *(ushort *)((uint)bVar2 << 0x10 | 0xf64a) & 0xffcf;
  (*tcu_shift_timer_scale_init)();
  return;
}



void tcu_shift_flags_dispatch(void)

{
  ushort *puVar1;
  byte bVar2;
  
  bVar2 = 1;
  tcu_shift_solenoid_table_init();
  (*tcu_sensor_range_check_and_f04e_max_update)();
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf1f4) & 4) == 0) {
    *(ushort *)((uint)bVar2 << 0x10 | 0xf226) = *(ushort *)((uint)bVar2 << 0x10 | 0xf226) & 0x3fff;
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf730) = 0xffff;
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xeef8) = 0;
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xeefa) = 0;
  }
  else {
    (*tcu_torque_converter_slip_calc_far_trampoline)();
  }
  if ((*(ushort *)((uint)bVar2 << 0x10 | 0xf730) & 0xf) == 0xf) {
    puVar1 = (ushort *)((uint)bVar2 << 0x10 | 0xf746);
    *puVar1 = *puVar1 & 0xff7f;
    puVar1 = (ushort *)((uint)bVar2 << 0x10 | 0xf748);
    *puVar1 = *puVar1 & 0xff7f;
  }
  else {
    puVar1 = (ushort *)((uint)bVar2 << 0x10 | 0xf746);
    *puVar1 = *puVar1 | 0x80;
    puVar1 = (ushort *)((uint)bVar2 << 0x10 | 0xf748);
    *puVar1 = *puVar1 | 0x80;
  }
  return;
}



void tcu_shift_solenoid_table_init(void)

{
  undefined2 uVar1;
  byte bVar2;
  
  bVar2 = 1;
  (*axis_lookup_interp)();
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x2f8e);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf64e) = uVar1;
  uVar1 = table_lookup_interp(1,(void *)0x2ffa);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf652) = uVar1;
  uVar1 = table_lookup_interp(1,(void *)0x3066);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf654) = uVar1;
  uVar1 = table_lookup_interp(1,(void *)0x30d2);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf656) = uVar1;
  uVar1 = table_lookup_interp(1,(void *)0x2f22);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf65a) = uVar1;
  if (*(char *)((uint)bVar2 << 0x10 | 0x314) != '\0') {
    uVar1 = table_lookup_interp(1,(void *)0x31fa);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf65e) = uVar1;
  }
  if (*(char *)((uint)bVar2 << 0x10 | 0x300) != '\0') {
    uVar1 = table_lookup_interp(1,(void *)0x31c4);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf662) = uVar1;
    uVar1 = table_lookup_interp(1,(void *)0x31c4);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf664) = uVar1;
  }
  if (*(char *)((uint)bVar2 << 0x10 | 0x301) != '\0') {
    uVar1 = table_lookup_interp(1,(void *)0x31c4);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf666) = uVar1;
    uVar1 = table_lookup_interp(1,(void *)0x31c4);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf668) = uVar1;
    uVar1 = table_lookup_interp(1,(void *)0x31c4);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf66a) = uVar1;
    uVar1 = table_lookup_interp(1,(void *)0x31c4);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf66c) = uVar1;
  }
  if (*(char *)((uint)bVar2 << 0x10 | 0x316) != '\0') {
    uVar1 = table_lookup_interp(1,(void *)0x31fa);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf66e) = uVar1;
    uVar1 = table_lookup_interp(1,(void *)0x31fa);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf670) = uVar1;
  }
  (*axis_lookup_interp)();
  uVar1 = table_lookup_interp(1,(void *)0x313e);
  *(undefined2 *)((uint)bVar2 << 0x10 | 0xf65c) = uVar1;
  if (*(char *)((uint)bVar2 << 0x10 | 0x310) != '\0') {
    (*axis_lookup_interp)();
    (*axis_lookup_interp)();
    uVar1 = table_lookup_interp(1,(void *)0x31fa);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf732) = uVar1;
    uVar1 = table_lookup_interp(1,(void *)0x31fa);
    *(undefined2 *)((uint)bVar2 << 0x10 | 0xf734) = uVar1;
    if (0xf < *(ushort *)((uint)bVar2 << 0x10 | 0xf732)) {
      *(undefined2 *)((uint)bVar2 << 0x10 | 0xf732) = 0x10;
    }
    if (0xf < *(ushort *)((uint)bVar2 << 0x10 | 0xf734)) {
      *(undefined2 *)((uint)bVar2 << 0x10 | 0xf734) = 0x10;
    }
  }
  (*tcu_shift_timer_scale_init)();
  return;
}



void tcu_shift_timer_scale_init(void)

{
  undefined2 uVar1;
  undefined2 extraout_R1;
  undefined2 extraout_R1_00;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  
  bVar4 = 1;
  uVar2 = 10;
  if (DAT_00010fce < 10) {
    uVar2 = DAT_00010fce;
  }
  DAT_0001f646 = 0x20;
  uVar3 = uVar2 - 1;
  while ((uVar2 != 0xffff && (DAT_0001f646 = DAT_0001f646 << 1, DAT_0001f646 != 0))) {
    uVar3 = uVar3 - 1;
    uVar2 = uVar3;
  }
  uVar1 = (*identity_passthrough_arg1)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf612) = uVar1;
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf614) = extraout_R1;
  uVar1 = (*identity_passthrough_arg1)();
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf616) = uVar1;
  *(undefined2 *)((uint)bVar4 << 0x10 | 63000) = extraout_R1_00;
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf61c) = *(undefined2 *)((uint)bVar4 << 0x10 | 0x100a);
  return;
}



// [RENAMED 2026-08-08] Formerly tcu_dtc_status_check_dispatch. Called once
// from tcu_shift_flags_dispatch (0x2b9a9).
// 
// CORRECTED 2026-08-08: an earlier pass of this comment called ROM 0x2bdb3
// "the fault/DTC-set routine" -- WRONG, verified by decompiling it
// directly. 0x2bdb3 is update_max_f04e, a simple running-maximum helper
// (if F04E < value then F04E = value), NOT a DTC/fault-setting routine.
// So while this function's overall SHAPE (many range/plausibility checks,
// each calling out to a shared helper on a condition) still resembles a
// DTC-setting pattern structurally, the specific claim that it sets a
// fault flag via 0x2bdb3 was incorrect -- it's tracking a running maximum
// of SOME value (context suggests possibly a max-observed engine
// load/torque or similar), not recording a fault. Do not assume this
// function sets DTCs without further verification; "DTC" in its old name
// may itself have been a mis-assumption from an earlier session, not
// re-confirmed here.
// 
// CONFIRMED (2026-08-08, structure only, re-verify "DTC" framing before
// relying on it further): genuine multi-condition sensor-range checker.
// Cross-checks several already-independently-confirmed sensor values
// against expected ranges:
//   - engine_torque_pct_f17a (confirmed elsewhere this project) against
//     range tables at 0x101a/0x101c or 0x10ee/0x10f0 depending on a TCU
//     shift-mode flag (DAT_0001ef1c bit7).
//   - A TPS-axis interpolated value against F18C.
//   - coolant_temp_scaled_f130 (confirmed elsewhere this project) against
//     a threshold at 0xff4, plus a flags word at F204 bit7 and a word at
//     F202.
//   - RAM F158 (written by tps_delta_calc @0x214ea) and F14C against
//     thresholds at 0xff6/0xff8.
//   - Two status words F0FA/F0F8 XORed and checked against bit4.
//   - Under a separate mode (EF1C bit7 set), re-checks engine_torque_pct_
//     f17a and F18C against a different threshold set.
// 
// On any check "failing" (out of expected range), calls update_max_f04e
// (0x2bdb3) -- updates a running max value at F04E, does NOT set a fault
// flag as far as verified. Function's actual purpose is more likely
// tracking peak/extreme sensor readings for logging or adaptive-limit
// purposes than classic DTC-setting -- RENAME MAY NEED REVISITING once
// F04E's consumers are traced (not done this session).
// 
// RELATION TO MUT DTC WORK (2026-08-08, STATUS DOWNGRADED FROM EARLIER
// CLAIM): a real diagnostic tool (EvoScan, decompiled via de4dot+ILSpy,
// see tools_ilspy\decompiled_evoscan\frmMain.cs) reads MUT-II EFI DTC
// status via 3 sequential MUT RequestID reads: 0x3B (RAM F15A/F15B), 0x3C
// (RAM F123, "Oxygen Sensor #2"), 0x3D (RAM F125). F158 (read by THIS
// function) is in the same RAM neighborhood as F123/F125/F14C, which is
// suggestive, but the DIRECT connection between this function and the
// actual F15A/F123/F125 DTC bytes has NOT been established -- this
// function updates F04E, not any of the three DTC bytes. Treat the "DTC
// subsystem" link as a plausible neighborhood/proximity observation only,
// not a confirmed data-flow connection, until F04E and the real writers of
// F15A/F123/F125 are traced and shown to connect. See
// mut_verification_status.md "MAJOR RECONCILIATION..." (2026-08-08) for
// the full writeup and this correction.
// 
// NOT YET DONE: trace F04E's consumers to determine this function's real
// purpose; trace the actual writer(s) of F15A/F123/F125 (the real DTC
// bytes EvoScan reads) to see if they connect to this function or F158 at
// all.

void tcu_sensor_range_check_and_f04e_max_update(void)

{
  ushort *puVar1;
  ushort uVar2;
  undefined2 in_ram_00001006;
  undefined2 in_ram_00001008;
  ushort uVar3;
  byte bVar4;
  void *table;
  
  bVar4 = 1;
  if ((g_status_flags_f20e & 0x11) != 0) {
    DAT_0000f04e = in_ram_00001006;
    DAT_0000f050 = in_ram_00001006;
    DAT_0000f642 = in_ram_00001008;
    DAT_0000f644 = in_ram_00001008;
    return;
  }
  if ((g_tcu_gear_ratio_hist_status_flags & 0x80) == 0) {
    if ((DAT_0001101c <= engine_torque_pct_f17a) || (engine_torque_pct_f17a < DAT_0001101a)) {
      (*update_max_f04e)();
    }
    if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) & 4) == 0) {
      uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0x101e);
    }
    else {
      uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0x1020);
    }
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf18c);
    if ((uVar3 < *puVar1 || uVar3 == *puVar1) &&
       (*(ushort *)((uint)bVar4 << 0x10 | 0xf18c) < *(ushort *)((uint)bVar4 << 0x10 | 0x1022)))
    goto LAB_0002bcce;
  }
  else {
    if ((DAT_00010ff0 <= engine_torque_pct_f17a) || (engine_torque_pct_f17a < DAT_00010fee)) {
      (*update_max_f04e)();
    }
    (*axis_lookup_interp)();
    if ((*(ushort *)((uint)bVar4 << 0x10 | 0xf0f8) & 4) == 0) {
      table = (void *)0x2f06;
    }
    else {
      table = (void *)0x2f14;
    }
    uVar3 = table_lookup_interp(1,table);
    puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xf18c);
    if ((uVar3 < *puVar1 || uVar3 == *puVar1) &&
       (*(ushort *)((uint)bVar4 << 0x10 | 0xf18c) < *(ushort *)((uint)bVar4 << 0x10 | 0xff2)))
    goto LAB_0002bcce;
  }
  (*update_max_f04e)();
LAB_0002bcce:
  uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xf130);
  puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xff4);
  if ((*puVar1 <= uVar3 && uVar3 != *puVar1) ||
     ((*(ushort *)((uint)bVar4 << 0x10 | 0xf204) & 0x80) != 0)) {
    (*update_max_f04e)();
  }
  if (*(short *)((uint)bVar4 << 0x10 | 0xf202) != 0) {
    (*update_max_f04e)();
  }
  uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xf158);
  uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf14c);
  puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xff6);
  if ((*puVar1 <= uVar2 && uVar2 != *puVar1) ||
     (puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0xff8), *puVar1 <= uVar3 && uVar3 != *puVar1)) {
    (*update_max_f04e)();
  }
  if (((*(ushort *)((uint)bVar4 << 0x10 | 0xf0fa) ^ *(ushort *)((uint)bVar4 << 0x10 | 0xf0f8)) &
      0x10) != 0) {
    (*update_max_f04e)();
  }
  if ((*(ushort *)((uint)bVar4 << 0x10 | 0xef1c) & 0x80) != 0) {
    if (((*(ushort *)((uint)bVar4 << 0x10 | 0xf17a) < *(ushort *)((uint)bVar4 << 0x10 | 0x1010)) ||
        (uVar3 = *(ushort *)((uint)bVar4 << 0x10 | 0xf17a),
        puVar1 = (ushort *)((uint)bVar4 << 0x10 | 0x1012), *puVar1 <= uVar3 && uVar3 != *puVar1)) ||
       (*(ushort *)((uint)bVar4 << 0x10 | 0x1014) <= *(ushort *)((uint)bVar4 << 0x10 | 0xf18c))) {
      (*tcu_f050_max_update)();
    }
    if (*(ushort *)((uint)bVar4 << 0x10 | 0xf18c) < *(ushort *)((uint)bVar4 << 0x10 | 0x10c4)) {
      (*tcu_f050_max_update)();
    }
  }
  return;
}



// [EXISTING FUNCTION update_max_f04e, COMMENT ADDED 2026-08-08] Simple
// running-maximum helper: if (F04E < value) F04E = value. NOT a fault/DTC-
// set routine -- an earlier pass on the caller's plate comment
// (tcu_sensor_range_check_and_f04e_max_update, 0x2bc0f) incorrectly
// labeled this as "the fault/DTC-set routine"; corrected same session.
// Called 7 times from that one caller, each after a sensor-range
// plausibility check fails, always with the sensor value that failed the
// check as the argument -- so F04E accumulates the highest "out of
// expected range" reading seen across engine_torque_pct_f17a, TPS-derived,
// coolant_temp_scaled_f130, F158, F14C, and F0FA/F0F8-XOR checks. Real
// purpose (peak-value logging? adaptive limit? diagnostic snapshot input?)
// NOT YET DETERMINED -- would need F04E's own consumers traced. See
// mut_verification_status.md "MAJOR RECONCILIATION..." (2026-08-08) and
// its correction entry for full context.

void __stdcall_far update_max_f04e(undefined2 _unused_far_pad,ushort value)

{
  if (DAT_0001f04e < value) {
    DAT_0001f04e = value;
  }
  return;
}



void __stdcall_far tcu_f050_max_update(undefined2 _unused_far_pad,ushort value)

{
  if (DAT_0001f050 < value) {
    DAT_0001f050 = value;
  }
  return;
}



void tcu_torque_converter_slip_calc_far_trampoline(void)

{
  (*tcu_torque_converter_slip_calc)();
  return;
}



// [NEW 2026-07-15] tcu_torque_converter_slip_calc
// 
// Decompiler output for this function is still garbage (heavy CONCAT12/stack-
// address artifacts) even under the current grammar - this is decompiler
// presentation only; raw disassemble_function output is clean and was used for
// this analysis instead (see review.md's decompiler-reliability note).
// 
// Reads F714/F716/F718/F71A/F71C/F71E (6 rolling samples, no static writers
// found - same indirect/bank-prefixed blind spot as other MUT cells), smooths
// them via two 3-sample sums (evens vs odds), combines with a running total, and
// runs the result through div_u16_sat scaled by a threshold table indexed off
// F18C. Gated by hysteresis flags in F226 (bits 0x4000/0x8000/0xC000) and F6D6/
// F6C8, calls tcu_shift_quality_index_calc and tcu_shift_pattern_threshold_select
// for tiered thresholds. Structurally this is torque-converter slip detection
// (smoothing + saturation + hysteresis on what looks like a turbine/output speed
// delta) - consistent with the function name, but the F714-F71E input chain's
// ultimate producer is not yet traced.

void tcu_torque_converter_slip_calc(void)

{
  ushort *puVar1;
  short *psVar2;
  ushort uVar3;
  short sVar4;
  short sVar5;
  char in_ram_00000310;
  short sVar6;
  ushort uVar7;
  short sVar8;
  undefined2 uVar9;
  undefined2 extraout_R1;
  ushort extraout_R2;
  ushort unaff_R3;
  undefined2 uVar10;
  byte bVar11;
  ushort uStack_6;
  
  uVar7 = DAT_0000f6d4;
  uVar3 = DAT_0000f6d6;
  DAT_0000f6d6 = uVar3 | uVar7 & 10;
  uVar7 = DAT_0000f6d4;
  DAT_0000f6d4 = uVar7 & 0xfff5;
  uVar10 = DAT_0000f714;
  uVar10 = DAT_0000f716;
  uVar10 = DAT_0000f718;
  uVar10 = DAT_0000f71a;
  uVar10 = DAT_0000f71c;
  uVar10 = DAT_0000f71e;
  sVar8 = DAT_0000f714;
  sVar4 = DAT_0000f718;
  sVar6 = DAT_0000f71c;
  sVar6 = sVar8 + sVar4 + sVar6;
  sVar8 = DAT_0000f716;
  sVar4 = DAT_0000f71a;
  sVar5 = DAT_0000f71e;
  uStack_6 = sVar8 + sVar4 + sVar5 + sVar6;
  bVar11 = 0;
  if (in_ram_00000310 != '\0') {
    uStack_6 = div_u16_sat(uStack_6,8);
  }
  *(ushort *)((uint)bVar11 << 0x10 | 0xf72c) = uStack_6;
  if ((*(char *)((uint)bVar11 << 0x10 | 0x2bd) != '\0') &&
     (*(ushort *)((uint)bVar11 << 0x10 | 0xf782) < *(ushort *)((uint)bVar11 << 0x10 | 0xf72c))) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xf782) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xf72c);
  }
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf6d6) & 8) != 0) {
    if (((*(char *)((uint)bVar11 << 0x10 | 0x310) == '\0') &&
        (uVar7 = (*tiered_threshold_table_select)(),
        puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf18c), uVar7 < *puVar1 || uVar7 == *puVar1)) ||
       ((*(char *)((uint)bVar11 << 0x10 | 0x310) != '\0' &&
        (puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0x142a),
        *puVar1 <= uStack_6 && uStack_6 != *puVar1)))) {
      sVar8 = *(short *)((uint)bVar11 << 0x10 | 0xf736) + 1;
      *(short *)((uint)bVar11 << 0x10 | 0xf736) = sVar8;
      if (sVar8 == 0) {
        psVar2 = (short *)((uint)bVar11 << 0x10 | 0xf736);
        *psVar2 = *psVar2 + -1;
      }
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf6c8);
      *puVar1 = *puVar1 | 8;
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf226);
      *puVar1 = *puVar1 | 0x8000;
    }
    else {
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf226);
      *puVar1 = *puVar1 & 0x7fff;
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf6c8);
      *puVar1 = *puVar1 & 0xfff7;
    }
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf226);
    *puVar1 = *puVar1 & 0xbfff;
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf6c8);
    *puVar1 = *puVar1 & 0xfffb;
    if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf226) & 0xc000) != 0) {
      tcu_shift_quality_index_calc(unaff_R3);
    }
    *(ushort *)((uint)bVar11 << 0x10 | 0xf6d6) = *(ushort *)((uint)bVar11 << 0x10 | 0xf6d6) & 0xfff5
    ;
  }
  if ((((*(ushort *)((uint)bVar11 << 0x10 | 0xf1f4) & 4) != 0) &&
      ((*(ushort *)((uint)bVar11 << 0x10 | 0xeedc) & 1) != 0)) &&
     ((*(ushort *)((uint)bVar11 << 0x10 | 0xeefa) & 0xbf) == 0)) {
    *(undefined2 *)((uint)bVar11 << 0x10 | 0xeefa) = *(undefined2 *)((uint)bVar11 << 0x10 | 0xeef8);
  }
  uVar10 = 0xffff;
  if ((*(ushort *)((uint)bVar11 << 0x10 | 0xf20e) & 0x11) == 0) {
    if (*(char *)((uint)bVar11 << 0x10 | 0x310) == '\0') {
      uVar9 = (*tiered_threshold_table_select)();
      (*tiered_threshold_table_select)(sVar6,extraout_R1,uVar9);
      uVar7 = extraout_R2;
    }
    else {
      (*axis_lookup_interp)(0);
      uVar7 = table_lookup_interp(1,(void *)0x3212);
    }
    if (((*(ushort *)((uint)bVar11 << 0x10 | 0xf226) & 0x8000) == 0) ||
       (puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf18c), *puVar1 <= uVar7 && uVar7 != *puVar1)) {
      puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf226);
      *puVar1 = *puVar1 & 0x7fff;
    }
    else {
      uVar10 = (*tcu_shift_pattern_threshold_select)();
    }
    puVar1 = (ushort *)((uint)bVar11 << 0x10 | 0xf226);
    *puVar1 = *puVar1 & 0xbfff;
  }
  else {
    *(ushort *)((uint)bVar11 << 0x10 | 0xf226) = *(ushort *)((uint)bVar11 << 0x10 | 0xf226) & 0x3fff
    ;
  }
  DAT_0000f730 = uVar10;
  return;
}



ushort tiered_threshold_table_select
                 (undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  ushort uVar1;
  ushort extraout_R2;
  byte bVar2;
  void *table;
  
  bVar2 = 1;
  (*axis_lookup_interp)(param_1,param_2,param_4,2,0xd636,param_3);
  if (extraout_R2 < *(ushort *)((uint)bVar2 << 0x10 | 0xfe2)) {
    if (extraout_R2 < *(ushort *)((uint)bVar2 << 0x10 | 0xfe0)) {
      if (extraout_R2 < *(ushort *)((uint)bVar2 << 0x10 | 0xfde)) {
        if (extraout_R2 < *(ushort *)((uint)bVar2 << 0x10 | 0xfdc)) {
          return 0xffff;
        }
        table = (void *)0x2ece;
      }
      else {
        table = (void *)0x2edc;
      }
    }
    else {
      table = (void *)0x2eea;
    }
  }
  else {
    table = (void *)0x2ef8;
  }
  uVar1 = table_lookup_interp(1,table);
  return uVar1;
}



ushort * tcu_shift_quality_index_calc(ushort param_fp1)

{
  short sVar1;
  ushort *puVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  byte bVar6;
  short in_stack_00000004;
  short sStack_a;
  ushort uStack_8;
  ushort uStack_6;
  ushort *puStack_4;
  
  bVar6 = 1;
  puStack_4 = (ushort *)0x0;
  uStack_6 = 0;
  uStack_8 = DAT_00010fe6 >> 2;
  if (DAT_00010fec < uStack_8) {
    uStack_8 = DAT_00010fec;
  }
  if (DAT_00010310 != '\0') {
    uStack_8 = sat_mul_u16(uStack_8,8);
  }
  sStack_a = 0;
  uVar5 = 0;
  do {
    puVar2 = (ushort *)(in_stack_00000004 + uVar5);
    if (uStack_8 <= *puVar2) {
      uVar3 = 1;
      sVar4 = sStack_a + -1;
      sVar1 = sStack_a;
      while ((sVar1 != -1 && (uVar3 = uVar3 << 1, uVar3 != 0))) {
        sVar4 = sVar4 + -1;
        sVar1 = sVar4;
      }
      puVar2 = (ushort *)((ushort)puStack_4 | uVar3);
      uStack_6 = uStack_6 + 1;
      puStack_4 = puVar2;
    }
    uVar5 = uVar5 + 2;
    sStack_a = sStack_a + 1;
  } while (uVar5 < 0xc);
  if (1 < uStack_6) {
    puStack_4 = (ushort *)0x80;
  }
  if (((*(ushort *)((uint)bVar6 << 0x10 | 0xf1f4) & 4) != 0) &&
     (puVar2 = (ushort *)(*(ushort *)((uint)bVar6 << 0x10 | 0xeef8) & 0xbf), puVar2 == (ushort *)0x0
     )) {
    puVar2 = (ushort *)((ushort)puStack_4 | *(ushort *)((uint)bVar6 << 0x10 | 0xeef8) & 0xff40);
    *(ushort **)((uint)bVar6 << 0x10 | 0xeef8) = puVar2;
  }
  return puVar2;
}



ushort tcu_shift_pattern_threshold_select(ushort param_fp1,ushort param_fp2)

{
  ushort uVar1;
  ushort *in_stack_00000004;
  ushort uStack_8;
  ushort uStack_6;
  ushort uStack_4;
  
  uStack_6 = 2;
  uStack_4 = in_stack_00000004[1];
  if (in_stack_00000004[1] <= in_stack_00000004[3]) {
    uStack_6 = 8;
    uStack_4 = in_stack_00000004[3];
  }
  if (uStack_4 <= in_stack_00000004[5]) {
    uStack_6 = 0x20;
    uStack_4 = in_stack_00000004[5];
  }
  uStack_8 = 1;
  uVar1 = *in_stack_00000004;
  if (*in_stack_00000004 <= in_stack_00000004[2]) {
    uStack_8 = 4;
    uVar1 = in_stack_00000004[2];
  }
  if (uVar1 <= in_stack_00000004[4]) {
    uStack_8 = 0x10;
    uVar1 = in_stack_00000004[4];
  }
  if ((param_fp1 == 1) || (uStack_4 < uVar1)) {
    uStack_6 = uStack_8;
  }
  return ~uStack_6;
}



void stub_empty_0x2c129(void)

{
  return;
}



void tcu_periodic_dispatch(void)

{
  short *psVar1;
  byte bVar2;
  ushort unaff_retaddr;
  
  bVar2 = 1;
  div_u32_u16_sat(DAT_0001f5d6,DAT_0001f5d8,4);
  tcu_shift_ratio_buffer_update(unaff_retaddr);
  (*tcu_solenoid_diagnostic_calc)();
  tcu_gear_ratio_calc();
  tcu_lockup_clutch_state_update();
  if (*(short *)((uint)bVar2 << 0x10 | 0xf642) != 0) {
    psVar1 = (short *)((uint)bVar2 << 0x10 | 0xf642);
    *psVar1 = *psVar1 + -1;
  }
  if (*(short *)((uint)bVar2 << 0x10 | 0xf644) != 0) {
    psVar1 = (short *)((uint)bVar2 << 0x10 | 0xf644);
    *psVar1 = *psVar1 + -1;
  }
  return;
}



// [NEW 2026-07-15] tcu_shift_ratio_buffer_update
// 
// Sole caller: tcu_periodic_dispatch (@0x2c12b), which passes
// (F5D6 - F5D8) / 4 as the input sample. F5D6/F5D8 are written directly inside
// isr_ipu_ch2ch4_input_capture's falling-edge branch (@0x16a38/0x16a3c) - a
// second, independently-computed period-delta output from the same 2-tooth cam
// capture ISR that also feeds engine_torque_pct_scale_calc via the F5DE chain.
// See the ISR's plate comment for the full trace.
// 
// Structure (clean disassembly, no decompiler artifacts - verified against raw
// disassemble_function output):
//   - F61E..F62E: 8-deep shift-register history of the raw input sample.
//   - F630/F632: gear-indexed lookup (table @0x32b8, index = F5E8 << 1) fed
//     through a saturated division (pjsr @0x141ba) - F5E8 is a gear index
//     written inside tcu_shift_torque_and_knock_mgmt, not a speed value, so
//     F630/F632 is best read as a gear ratio, not raw RPM.
//   - F634..F63C: 4-deep shift-register of successive F630 deltas, clamped to
//     limits at 0x102A/0x102C (rate-of-change/acceleration filter on the ratio).
//   - F96E: stores the raw delta result for this cycle.
// 
// Net effect: converts the falling-edge cam period into a gear-ratio estimate
// with rate limiting, for TCU shift-quality/shift-ratio logic (see caller of
// this and tcu_torque_converter_slip_calc). Not itself an RPM output, but
// downstream of one - the F5D6/F5D8 input is the RPM-candidate signal.

void tcu_shift_ratio_buffer_update(ushort param_fp1)

{
  ushort uVar1;
  ushort uVar2;
  ushort a_lo;
  byte extraout_var;
  short sVar3;
  char extraout_R2;
  byte bVar4;
  ushort in_stack_00000004;
  
  bVar4 = 1;
  DAT_0001f62e = DAT_0001f62c;
  DAT_0001f62c = DAT_0001f62a;
  DAT_0001f62a = DAT_0001f628;
  DAT_0001f628 = DAT_0001f626;
  DAT_0001f626 = DAT_0001f624;
  DAT_0001f624 = DAT_0001f622;
  DAT_0001f622 = DAT_0001f620;
  DAT_0001f620 = DAT_0001f61e;
  DAT_0001f61e = in_stack_00000004;
  uVar1 = div_u32_u16_sat(0x800,0,in_stack_00000004);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf630) = uVar1;
  uVar1 = *(ushort *)((uint)bVar4 << 0x10 | 0xf632);
  uVar2 = (*inc_if_signed_flag)();
  uVar2 = muldiv_u16(*(ushort *)((uint)bVar4 << 0x10 | 0xf630),uVar2,0x8000);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf632) = uVar2;
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf63c) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xf63a);
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf63a) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xf638);
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf638) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xf636);
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf636) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xf634);
  uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0xf632);
  if (uVar2 < uVar1) {
    sVar3 = (ushort)extraout_var << 8;
    uVar2 = uVar1 - uVar2;
  }
  else {
    sVar3 = CONCAT11(extraout_var,1);
    uVar2 = uVar2 - uVar1;
  }
  if (0x7ffe < uVar2) {
    uVar2 = 0x7fff;
  }
  uVar1 = *(ushort *)((uint)bVar4 << 0x10 | 0xf646);
  uVar2 = (*muldiv_u16_shr8_sat)
                    (uVar2,0,sVar3,0,uVar2,*(undefined2 *)((uint)bVar4 << 0x10 | 0xf630));
  uVar1 = div_u32_u16_sat(uVar2,a_lo,uVar1);
  *(ushort *)((uint)bVar4 << 0x10 | 0xf96e) = uVar1;
  if ((extraout_R2 == '\0') && (uVar1 != 0)) {
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0x102a);
    uVar1 = -uVar1;
    if (uVar1 < uVar2) {
      *(ushort *)((uint)bVar4 << 0x10 | 0xf634) = uVar2;
      goto LAB_0002c2a0;
    }
  }
  else {
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0x102c);
    if (uVar2 <= uVar1) {
      *(ushort *)((uint)bVar4 << 0x10 | 0xf634) = uVar2;
      goto LAB_0002c2a0;
    }
  }
  *(ushort *)((uint)bVar4 << 0x10 | 0xf634) = uVar1;
LAB_0002c2a0:
  *(undefined2 *)((uint)bVar4 << 0x10 | 0xf640) = *(undefined2 *)((uint)bVar4 << 0x10 | 0xf63e);
  uVar1 = *(short *)((uint)bVar4 << 0x10 | 0xf634) - *(short *)((uint)bVar4 << 0x10 | 0xf638);
  if ((uVar1 & 0x8000) == 0) {
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0x102c);
    if (uVar2 <= uVar1) {
      *(ushort *)((uint)bVar4 << 0x10 | 0xf63e) = uVar2;
      return;
    }
  }
  else {
    uVar2 = *(ushort *)((uint)bVar4 << 0x10 | 0x102a);
    if (uVar1 < uVar2) {
      *(ushort *)((uint)bVar4 << 0x10 | 0xf63e) = uVar2;
      return;
    }
  }
  *(ushort *)((uint)bVar4 << 0x10 | 0xf63e) = uVar1;
  return;
}



void tcu_solenoid_diagnostic_calc(void)

{
  ushort *puVar1;
  short *psVar2;
  short *psVar3;
  short *psVar4;
  ushort uVar5;
  ushort uVar6;
  short sVar7;
  ushort uVar8;
  ushort uVar9;
  byte bVar10;
  ushort uStack_6;
  
  bVar10 = 1;
  DAT_0001f96a = 0xffff;
  tcu_solenoid_diag_flags_f648 = tcu_solenoid_diag_flags_f648 & 0xfc3c;
  DAT_0001f678 = DAT_0001f678 << 1;
  uStack_6 = DAT_0001f678;
  if (DAT_00010300 != '\0') {
    uStack_6 = DAT_0001f672 << 1;
    DAT_0001f672 = uStack_6;
  }
  if (DAT_00010301 != '\0') {
    uStack_6 = DAT_0001f674 << 1;
    DAT_0001f674 = uStack_6;
  }
  if (DAT_00010316 != '\0') {
    uStack_6 = DAT_0001f676 << 1;
    DAT_0001f676 = uStack_6;
  }
  if ((g_tcu_gear_ratio_hist_status_flags & 0x80) == 0) {
    if ((((ushort)(DAT_0001f640 - DAT_0001f63e) < 0x8000) &&
        (uStack_6 = (*clamp_u8)(), puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf652),
        *puVar1 <= uStack_6 && uStack_6 != *puVar1)) &&
       (uStack_6 = *(ushort *)((uint)bVar10 << 0x10 | 0xf644),
       uStack_6 < *(ushort *)((uint)bVar10 << 0x10 | 0x1018))) {
      uStack_6 = *(ushort *)((uint)bVar10 << 0x10 | 0x1018);
      *(ushort *)((uint)bVar10 << 0x10 | 0xf644) = uStack_6;
    }
  }
  else {
    if ((g_status_flags_f0f8 & 0x20) == 0) {
      uVar9 = DAT_00010ffc;
    }
    else {
      uVar9 = DAT_00010ffa;
    }
    if (((engine_torque_pct_f17a < uVar9) && ((DAT_0001f634 & 0x8000) == 0)) &&
       (uStack_6 = (*clamp_u8)(), puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf65a),
       *puVar1 <= uStack_6 && uStack_6 != *puVar1)) {
      uStack_6 = *(ushort *)((uint)bVar10 << 0x10 | 0x1008);
      *(ushort *)((uint)bVar10 << 0x10 | 0xf642) = uStack_6;
    }
    uVar9 = *(short *)((uint)bVar10 << 0x10 | 0xf638) - *(short *)((uint)bVar10 << 0x10 | 0xf636);
    if (((uVar9 < 0x8000) &&
        (uStack_6 = (*clamp_u8)(), puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf652),
        *puVar1 <= uStack_6 && uStack_6 != *puVar1)) &&
       (uStack_6 = *(ushort *)((uint)bVar10 << 0x10 | 0xf644),
       uStack_6 < *(ushort *)((uint)bVar10 << 0x10 | 0x1018))) {
      uStack_6 = *(ushort *)((uint)bVar10 << 0x10 | 0x1018);
      *(ushort *)((uint)bVar10 << 0x10 | 0xf644) = uStack_6;
    }
    if (*(short *)((uint)bVar10 << 0x10 | 0xf04e) == 0) {
      uStack_6 = 0;
      uVar8 = (*(ushort *)((uint)bVar10 << 0x10 | 0xf660) & 0x5600) >> 1;
      if ((*(ushort *)((uint)bVar10 << 0x10 | 0xf64c) & 0x10) != 0) {
        uStack_6 = 0x20;
      }
      if ((*(ushort *)((uint)bVar10 << 0x10 | 0xf636) & 0x8000) != 0) {
        uVar6 = (*clamp_u8)();
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf64e);
        uVar5 = uStack_6;
        if ((*puVar1 <= uVar6 && uVar6 != *puVar1) &&
           (uVar5 = uStack_6 | 0x10, *(char *)((uint)bVar10 << 0x10 | 0x2ff) != '\0')) {
          uVar5 = uStack_6 | 0x11;
        }
        uStack_6 = uVar5;
        if ((*(char *)((uint)bVar10 << 0x10 | 0x301) != '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf666), *puVar1 <= uVar6 && uVar6 != *puVar1
           )) {
          uVar8 = uVar8 | 8;
        }
        if ((*(char *)((uint)bVar10 << 0x10 | 0x314) != '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf65e), *puVar1 <= uVar6 && uVar6 != *puVar1
           )) {
          uStack_6 = uStack_6 | 0x8000;
        }
      }
      if ((*(ushort *)((uint)bVar10 << 0x10 | 0xf64c) & 4) != 0) {
        uStack_6 = uStack_6 | 8;
      }
      if (uVar9 < 0x8000) {
        uVar9 = (*clamp_u8)();
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf654);
        if (*puVar1 <= uVar9 && uVar9 != *puVar1) {
          uStack_6 = uStack_6 | 4;
        }
        if ((*(char *)((uint)bVar10 << 0x10 | 0x2ff) == '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xfea), *puVar1 <= uVar9 && uVar9 != *puVar1)
           ) {
          uStack_6 = uStack_6 | 1;
        }
        if ((*(char *)((uint)bVar10 << 0x10 | 0x300) != '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf662), *puVar1 <= uVar9 && uVar9 != *puVar1
           )) {
          uVar8 = uVar8 | 0x80;
        }
        if ((*(char *)((uint)bVar10 << 0x10 | 0x301) != '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf668), *puVar1 <= uVar9 && uVar9 != *puVar1
           )) {
          uVar8 = uVar8 | 4;
        }
        if (*(char *)((uint)bVar10 << 0x10 | 0x316) != '\0') {
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf66e);
          if (*puVar1 <= uVar9 && uVar9 != *puVar1) {
            uVar8 = uVar8 | 0x1000;
          }
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf670);
          if (*puVar1 <= uVar9 && uVar9 != *puVar1) {
            uVar8 = uVar8 | 0x400;
          }
        }
        if ((*(char *)((uint)bVar10 << 0x10 | 0x314) != '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0x1434), *puVar1 <= uVar9 && uVar9 != *puVar1
           )) {
          uStack_6 = uStack_6 | 0x4000;
        }
      }
      if ((ushort)(*(short *)((uint)bVar10 << 0x10 | 0xf634) -
                  *(short *)((uint)bVar10 << 0x10 | 0xf636)) < 0x8000) {
        uVar9 = (*clamp_u8)();
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf656);
        if (*puVar1 <= uVar9 && uVar9 != *puVar1) {
          uStack_6 = uStack_6 | 2;
        }
        if ((*(char *)((uint)bVar10 << 0x10 | 0x300) != '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf664), *puVar1 <= uVar9 && uVar9 != *puVar1
           )) {
          uVar8 = uVar8 | 0x40;
        }
        if ((*(char *)((uint)bVar10 << 0x10 | 0x301) != '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf66a), *puVar1 <= uVar9 && uVar9 != *puVar1
           )) {
          uVar8 = uVar8 | 2;
        }
        if (*(char *)((uint)bVar10 << 0x10 | 0x316) != '\0') {
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf66e);
          if (*puVar1 <= uVar9 && uVar9 != *puVar1) {
            uVar8 = uVar8 | 0x8000;
          }
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf670);
          if (*puVar1 <= uVar9 && uVar9 != *puVar1) {
            uVar8 = uVar8 | 0x4000;
          }
        }
        if ((*(char *)((uint)bVar10 << 0x10 | 0x314) != '\0') &&
           (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0x1438), *puVar1 <= uVar9 && uVar9 != *puVar1
           )) {
          uStack_6 = uStack_6 | 0x800;
        }
      }
      if (((*(char *)((uint)bVar10 << 0x10 | 0x301) != '\0') &&
          (*(byte *)((uint)bVar10 << 0x10 | 0xf634) < 0x80)) &&
         (uVar9 = (*clamp_u8)(), puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf66c),
         *puVar1 <= uVar9 && uVar9 != *puVar1)) {
        uVar8 = uVar8 | 1;
      }
      *(ushort *)((uint)bVar10 << 0x10 | 0xf64c) = uStack_6;
      if (((*(char *)((uint)bVar10 << 0x10 | 0x300) != '\0') ||
          (*(char *)((uint)bVar10 << 0x10 | 0x301) != '\0')) ||
         (*(char *)((uint)bVar10 << 0x10 | 0x316) != '\0')) {
        *(ushort *)((uint)bVar10 << 0x10 | 0xf660) = uVar8;
      }
      if ((uStack_6 & 0x16) == 0x16) {
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
        *puVar1 = *puVar1 | 0x80;
        *(short *)((uint)bVar10 << 0x10 | 0xf96a) = *(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 4;
        if (*(char *)((uint)bVar10 << 0x10 | 0x300) != '\0') {
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf672);
          *puVar1 = *puVar1 | 1;
        }
        if (*(char *)((uint)bVar10 << 0x10 | 0x2bd) != '\0') {
          sVar7 = *(short *)((uint)bVar10 << 0x10 | 0xf7c0) + 1;
          *(short *)((uint)bVar10 << 0x10 | 0xf7c0) = sVar7;
          if (sVar7 == 0) {
            psVar2 = (short *)((uint)bVar10 << 0x10 | 0xf7c0);
            *psVar2 = *psVar2 + -1;
          }
          sVar7 = *(short *)((uint)bVar10 << 0x10 | 0xf7c8) + 1;
          *(short *)((uint)bVar10 << 0x10 | 0xf7c8) = sVar7;
          if (sVar7 == 0) {
            psVar2 = (short *)((uint)bVar10 << 0x10 | 0xf7c8);
            *psVar2 = *psVar2 + -1;
          }
        }
      }
      if ((((*(char *)((uint)bVar10 << 0x10 | 0x300) == '\x01') ||
           ((*(char *)((uint)bVar10 << 0x10 | 0x300) == '\x02' &&
            ((*(ushort *)((uint)bVar10 << 0x10 | 0xf0f8) & 0x20) != 0)))) &&
          ((*(ushort *)((uint)bVar10 << 0x10 | 0xf672) & 4) != 0)) && ((uVar8 & 0xc0) == 0xc0)) {
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
        *puVar1 = *puVar1 | 0x80;
        *(short *)((uint)bVar10 << 0x10 | 0xf96a) =
             *(short *)((uint)bVar10 << 0x10 | 0xf5e8) * 0x10 + 8;
        if (*(char *)((uint)bVar10 << 0x10 | 0x2bd) != '\0') {
          sVar7 = *(short *)((uint)bVar10 << 0x10 | 0xf7c4) + 1;
          *(short *)((uint)bVar10 << 0x10 | 0xf7c4) = sVar7;
          if (sVar7 == 0) {
            psVar2 = (short *)((uint)bVar10 << 0x10 | 0xf7c4);
            *psVar2 = *psVar2 + -1;
          }
          sVar7 = *(short *)((uint)bVar10 << 0x10 | 0xf7cc) + 1;
          *(short *)((uint)bVar10 << 0x10 | 0xf7cc) = sVar7;
          if (sVar7 == 0) {
            psVar2 = (short *)((uint)bVar10 << 0x10 | 0xf7cc);
            *psVar2 = *psVar2 + -1;
          }
        }
      }
      if (((*(char *)((uint)bVar10 << 0x10 | 0x301) != '\0') && ((uVar8 & 0xf) == 0xf)) &&
         (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf674), *puVar1 = *puVar1 | 1,
         (*(ushort *)((uint)bVar10 << 0x10 | 0x1444) & *(ushort *)((uint)bVar10 << 0x10 | 0xf674))
         == *(ushort *)((uint)bVar10 << 0x10 | 0x1444))) {
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
        *puVar1 = *puVar1 | 0x80;
        *(short *)((uint)bVar10 << 0x10 | 0xf96a) =
             *(short *)((uint)bVar10 << 0x10 | 0xf5e8) * 0x10 + 0x88;
        if (*(char *)((uint)bVar10 << 0x10 | 0x2bd) != '\0') {
          sVar7 = *(short *)((uint)bVar10 << 0x10 | 0xf7c6) + 1;
          *(short *)((uint)bVar10 << 0x10 | 0xf7c6) = sVar7;
          if (sVar7 == 0) {
            psVar2 = (short *)((uint)bVar10 << 0x10 | 0xf7c6);
            *psVar2 = *psVar2 + -1;
          }
          sVar7 = *(short *)((uint)bVar10 << 0x10 | 0xf7ce) + 1;
          *(short *)((uint)bVar10 << 0x10 | 0xf7ce) = sVar7;
          if (sVar7 == 0) {
            psVar2 = (short *)((uint)bVar10 << 0x10 | 0xf7ce);
            *psVar2 = *psVar2 + -1;
          }
        }
      }
      if (((*(char *)((uint)bVar10 << 0x10 | 0x314) != '\0') &&
          (uVar9 = *(ushort *)((uint)bVar10 << 0x10 | 0xf10e),
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0x1432), *puVar1 <= uVar9 && uVar9 != *puVar1))
         && (((*(ushort *)((uint)bVar10 << 0x10 | 0xf0f8) & 0x24) == 0x20 &&
             ((uStack_6 & 0xc800) == 0xc800)))) {
        psVar3 = *(short **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 0x32f0);
        if (*(char *)((uint)bVar10 << 0x10 | 0x310) == '\0') {
          *psVar3 = *psVar3 + 1;
          if (*psVar3 == 0) {
            *psVar3 = *psVar3 + -1;
          }
        }
        else {
          *psVar3 = *(short *)((uint)bVar10 << 0x10 | 0xf734) + *psVar3;
        }
        *(undefined2 *)((uint)bVar10 << 0x10 | 0xf6d0) =
             *(undefined2 *)((uint)bVar10 << 0x10 | 0xf17a);
        *(undefined2 *)((uint)bVar10 << 0x10 | 0xf6d2) =
             *(undefined2 *)((uint)bVar10 << 0x10 | 0xf18c);
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf6d4);
        *puVar1 = *puVar1 | 1;
        *(short *)((uint)bVar10 << 0x10 | 0xf96a) = *(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 4;
      }
      uStack_6 = uStack_6 & 0x2b;
      if (uStack_6 == 0x2b) {
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
        *puVar1 = *puVar1 | 0x40;
        uStack_6 = *(short *)((uint)bVar10 << 0x10 | 0xf5e8) * 0x10 + 0x80;
        *(ushort *)((uint)bVar10 << 0x10 | 0xf96a) = uStack_6;
        if (*(char *)((uint)bVar10 << 0x10 | 0x2bd) != '\0') {
          sVar7 = *(short *)((uint)bVar10 << 0x10 | 0xf7c2) + 1;
          *(short *)((uint)bVar10 << 0x10 | 0xf7c2) = sVar7;
          if (sVar7 == 0) {
            psVar2 = (short *)((uint)bVar10 << 0x10 | 0xf7c2);
            *psVar2 = *psVar2 + -1;
          }
          uStack_6 = *(short *)((uint)bVar10 << 0x10 | 0xf7ca) + 1;
          *(ushort *)((uint)bVar10 << 0x10 | 0xf7ca) = uStack_6;
          if (uStack_6 == 0) {
            psVar2 = (short *)((uint)bVar10 << 0x10 | 0xf7ca);
            *psVar2 = *psVar2 + -1;
          }
        }
      }
      if (((*(char *)((uint)bVar10 << 0x10 | 0x316) != '\0') && ((uVar8 & 0xa900) == 0xa900)) &&
         (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf676), *puVar1 = *puVar1 | 1,
         (*(ushort *)((uint)bVar10 << 0x10 | 0xf676) & 0x10) != 0)) {
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
        *puVar1 = *puVar1 | 0x200;
        if (*(short *)((uint)bVar10 << 0x10 | 0xf5e8) == 0) {
          *(undefined2 *)((uint)bVar10 << 0x10 | 0xf96a) = 0xb0;
        }
        else {
          uStack_6 = (*(short *)((uint)bVar10 << 0x10 | 0xf5e8) + -1) * 0x10 + 0x80;
          *(ushort *)((uint)bVar10 << 0x10 | 0xf96a) = uStack_6;
        }
      }
      if (*(short *)((uint)bVar10 << 0x10 | 0xf642) == 0) {
        uStack_6 = *(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0xffcf;
        *(ushort *)((uint)bVar10 << 0x10 | 0xf648) = uStack_6;
      }
      else if ((*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0x80) == 0) {
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
        *puVar1 = *puVar1 | 0x10;
      }
      else {
        if (((*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0x20) == 0) ||
           (*(short *)((uint)bVar10 << 0x10 | 0xf6ca) != *(short *)((uint)bVar10 << 0x10 | 0xf5e8)))
        {
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
          *puVar1 = *puVar1 | 0x10;
        }
        else {
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
          *puVar1 = *puVar1 & 0xffef;
        }
        uStack_6 = *(ushort *)((uint)bVar10 << 0x10 | 0xf5e8);
        *(ushort *)((uint)bVar10 << 0x10 | 0xf6ca) = uStack_6;
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
        *puVar1 = *puVar1 | 0x20;
      }
      if (((*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0x10) == 0) &&
         (uStack_6 = 0, (*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0x2c0) != 0)) {
        if (((*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0xc0) != 0) &&
           ((*(ushort *)((uint)bVar10 << 0x10 | 0xf678) & 2) == 0)) {
          psVar3 = *(short **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 0x32cc);
          psVar4 = *(short **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 0x32e0);
          if (*(char *)((uint)bVar10 << 0x10 | 0x310) == '\0') {
            *psVar3 = *psVar3 + 1;
            if (*psVar3 == 0) {
              *psVar3 = *psVar3 + -1;
            }
            *psVar4 = *psVar4 + 1;
            if (*psVar4 == 0) {
              *psVar4 = *psVar4 + -1;
            }
          }
          else {
            *psVar3 = *(short *)((uint)bVar10 << 0x10 | 0xf732) + *psVar3;
            *psVar4 = *(short *)((uint)bVar10 << 0x10 | 0xf734) + *psVar4;
          }
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf678);
          *puVar1 = *puVar1 | 2;
        }
        if (((*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0x240) != 0) &&
           ((*(ushort *)((uint)bVar10 << 0x10 | 0xf678) & 4) == 0)) {
          psVar3 = *(short **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 0x32ca);
          psVar4 = *(short **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 0x32de);
          if (*(char *)((uint)bVar10 << 0x10 | 0x310) == '\0') {
            *psVar3 = *psVar3 + 1;
            if (*psVar3 == 0) {
              *psVar3 = *psVar3 + -1;
            }
            *psVar4 = *psVar4 + 1;
            if (*psVar4 == 0) {
              *psVar4 = *psVar4 + -1;
            }
          }
          else {
            *psVar3 = *(short *)((uint)bVar10 << 0x10 | 0xf732) + *psVar3;
            *psVar4 = *(short *)((uint)bVar10 << 0x10 | 0xf734) + *psVar4;
          }
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf678);
          *puVar1 = *puVar1 | 4;
        }
        if (((*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0x200) != 0) &&
           ((*(ushort *)((uint)bVar10 << 0x10 | 0xf678) & 8) == 0)) {
          psVar3 = *(short **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 13000);
          psVar4 = *(short **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 0x32dc);
          if (*(char *)((uint)bVar10 << 0x10 | 0x310) == '\0') {
            *psVar3 = *psVar3 + 1;
            if (*psVar3 == 0) {
              *psVar3 = *psVar3 + -1;
            }
            *psVar4 = *psVar4 + 1;
            if (*psVar4 == 0) {
              *psVar4 = *psVar4 + -1;
            }
          }
          else {
            *psVar3 = *(short *)((uint)bVar10 << 0x10 | 0xf732) + *psVar3;
            *psVar4 = *(short *)((uint)bVar10 << 0x10 | 0xf734) + *psVar4;
          }
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf678);
          *puVar1 = *puVar1 | 8;
        }
        *(undefined2 *)((uint)bVar10 << 0x10 | 0xf6d0) =
             *(undefined2 *)((uint)bVar10 << 0x10 | 0xf17a);
        *(undefined2 *)((uint)bVar10 << 0x10 | 0xf6d2) =
             *(undefined2 *)((uint)bVar10 << 0x10 | 0xf18c);
        *(ushort *)((uint)bVar10 << 0x10 | 0xf6d4) = *(ushort *)((uint)bVar10 << 0x10 | 0xf6d4) | 3;
        if ((*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0x8000) == 0) {
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
          *puVar1 = *puVar1 | 0x4000;
        }
        if ((*(ushort *)((uint)bVar10 << 0x10 | 0xf648) & 0x2000) == 0) {
          puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf648);
          *puVar1 = *puVar1 | 0x1000;
        }
        uStack_6 = *(ushort *)((uint)bVar10 << 0x10 | 0xf648) | 0xa000;
        *(ushort *)((uint)bVar10 << 0x10 | 0xf648) = uStack_6;
      }
    }
    else {
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xf64c) = 0;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xf660) = 0;
    }
  }
  (*tcu_shift_buffer_average_calc)(uStack_6);
  return;
}



void tcu_shift_buffer_average_calc(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  if ((((g_status_flags_f20e & 0x11) == 0) && ((ushort)(DAT_00011006 << 2) < DAT_0001ef70)) &&
     ((DAT_0001150a <= engine_torque_pct_f17a || (DAT_0001150c <= inj_pw_output_f18c)))) {
    DAT_0001f64a = DAT_0001f64a | 0x20;
  }
  if (DAT_0001f6cc == 0) {
    if ((ushort)(DAT_0001f6d8 + DAT_0001f6da + DAT_0001f6dc + DAT_0001f6de + DAT_0001f6e0 +
                DAT_0001f6e2) <
        (ushort)(DAT_0001f6e4 + DAT_0001f6e6 + DAT_0001f6e8 + DAT_0001f6ea + DAT_0001f6ec +
                DAT_0001f6ee)) {
      DAT_0001f714 = DAT_0001f6e4;
      DAT_0001f716 = DAT_0001f6e6;
      DAT_0001f718 = DAT_0001f6e8;
      DAT_0001f71a = DAT_0001f6ea;
      DAT_0001f71c = DAT_0001f6ec;
      DAT_0001f71e = DAT_0001f6ee;
    }
    else {
      DAT_0001f714 = DAT_0001f6d8;
      DAT_0001f716 = DAT_0001f6da;
      DAT_0001f718 = DAT_0001f6dc;
      DAT_0001f71a = DAT_0001f6de;
      DAT_0001f71c = DAT_0001f6e0;
      DAT_0001f71e = DAT_0001f6e2;
    }
    if (PTR_INTC_IPRA_000102bd._0_1_ != '\0') {
      if (DAT_0001f7d8 <= DAT_0001f7c8) {
        DAT_0001f7d8 = DAT_0001f7c8;
      }
      if (DAT_0001f7da <= DAT_0001f7ca) {
        DAT_0001f7da = DAT_0001f7ca;
      }
      if (DAT_0001f7dc <= DAT_0001f7cc) {
        DAT_0001f7dc = DAT_0001f7cc;
      }
      if (DAT_0001f7de <= DAT_0001f7ce) {
        DAT_0001f7de = DAT_0001f7ce;
      }
    }
    DAT_0001f6d4 = DAT_0001f6d4 | 8;
    if ((DAT_0001f64a & 0x20) == 0) {
      DAT_0001f64a = DAT_0001f64a & 0xffef;
    }
    else {
      DAT_0001f64a = DAT_0001f64a | 0x10;
    }
    DAT_0001f64a = DAT_0001f64a & 0xffdf;
  }
  else {
    if ((PTR_DAT_00010321._0_1_ == '\0') || (DAT_0001f04e == 0)) {
      DAT_0001f6cc = DAT_0001f6cc + -1;
    }
    if (DAT_0001f04e == 0) {
      if ((tcu_solenoid_diag_flags_f648 & 0x4000) == 0) {
        return;
      }
      tcu_solenoid_diag_flags_f648 = tcu_solenoid_diag_flags_f648 & 0xbfff;
      DAT_0001f6cc = DAT_00010fe8;
      return;
    }
    if (PTR_DAT_00010321._0_1_ != '\0') {
      return;
    }
    DAT_0001f6d4 = DAT_0001f6d4 & 0xfffd;
  }
  coldinit_f6c0_f7c8_state_clear_and_intc_flag_read(param_3);
  return;
}



void coldinit_f6c0_f7c8_state_clear_and_intc_flag_read(ushort param_fp1)

{
  ushort in_stack_00000004;
  
  if ((in_stack_00000004 & 1) != 0) {
    DAT_0001f6cc = DAT_00010fe8;
    DAT_0001f6dc = 0;
    DAT_0001f6da = 0;
    DAT_0001f6d8 = 0;
    DAT_0001f6e2 = 0;
    DAT_0001f6e0 = 0;
    DAT_0001f6de = 0;
    DAT_0001f6e8 = 0;
    DAT_0001f6e6 = 0;
    DAT_0001f6e4 = 0;
    DAT_0001f6ee = 0;
    DAT_0001f6ec = 0;
    DAT_0001f6ea = 0;
    if (PTR_INTC_IPRA_000102bd._0_1_ != '\0') {
      DAT_0001f7c8 = 0;
      DAT_0001f7ca = 0;
      DAT_0001f7cc = 0;
      DAT_0001f7ce = 0;
    }
    tcu_solenoid_diag_flags_f648 = tcu_solenoid_diag_flags_f648 & 0x7fff;
  }
  return;
}



ushort tcu_gear_ratio_calc(undefined2 param_1,undefined2 param_2,ushort param_3)

{
  ushort *puVar1;
  short *psVar2;
  ushort *puVar3;
  undefined2 *puVar4;
  ushort uVar5;
  ushort uVar6;
  undefined2 uVar7;
  short sVar8;
  ushort extraout_R1;
  undefined2 extraout_R1_00;
  ushort unaff_R3;
  char cVar9;
  byte bVar10;
  bool bVar11;
  ushort uStack_14;
  ushort uStack_12;
  ushort uStack_10;
  ushort uStack_e;
  ushort uStack_c;
  ushort uStack_a;
  ushort uStack_8;
  ushort uStack_6;
  short sStack_4;
  
  bVar10 = 1;
  uStack_8 = 0x8000;
  uStack_6 = 0;
  uStack_a = DAT_0001f622 + DAT_0001f624;
  bVar11 = CARRY2(DAT_0001f622,DAT_0001f624);
  while (bVar11) {
    bVar11 = false;
    uStack_a = uStack_a >> 1 | 0x8000;
    uStack_6 = uStack_6 >> 1 | (ushort)((byte)uStack_8 & 1) << 0xf;
    uStack_8 = uStack_8 >> 1;
  }
  uVar6 = muldiv_u16_scaled(uStack_8,uStack_6,DAT_0001f622,uStack_a);
  uStack_a = extraout_R1 * 2;
  uStack_c = uVar6 << 1 | extraout_R1 >> 0xf;
  puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf612);
  if ((uStack_c < *puVar1) ||
     ((uStack_c == *puVar1 &&
      (puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf614), uStack_a < *puVar1 || uStack_a == *puVar1
      )))) {
    puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xf616);
    bVar11 = uStack_c == *puVar1;
    if ((*puVar1 <= uStack_c && !bVar11) ||
       ((bVar11 && (*(ushort *)((uint)bVar10 << 0x10 | 63000) <= uStack_a)))) goto LAB_0002cb81;
    uStack_c = *(ushort *)((uint)bVar10 << 0x10 | 0xf616);
    uStack_a = *(ushort *)((uint)bVar10 << 0x10 | 63000);
    if (*(ushort *)((uint)bVar10 << 0x10 | 0x1032) <= *(ushort *)((uint)bVar10 << 0x10 | 0xf644))
    goto LAB_0002cb81;
  }
  else {
    uStack_c = *(ushort *)((uint)bVar10 << 0x10 | 0xf612);
    uStack_a = *(ushort *)((uint)bVar10 << 0x10 | 0xf614);
    if (*(ushort *)((uint)bVar10 << 0x10 | 0x1032) <= *(ushort *)((uint)bVar10 << 0x10 | 0xf644))
    goto LAB_0002cb81;
  }
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf644) = *(undefined2 *)((uint)bVar10 << 0x10 | 0x1032);
LAB_0002cb81:
  *(ushort *)((uint)bVar10 << 0x10 | 0xf5f6) = uStack_c;
  *(ushort *)((uint)bVar10 << 0x10 | 0xf5f8) = uStack_a;
  if ((((*(short *)((uint)bVar10 << 0x10 | 0xf04e) == 0) &&
       (*(short *)((uint)bVar10 << 0x10 | 0xf642) == 0)) &&
      (*(short *)((uint)bVar10 << 0x10 | 0xf050) == 0)) &&
     (*(short *)((uint)bVar10 << 0x10 | 0xf644) == 0)) {
    puVar3 = *(ushort **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 0x32b8);
    muldiv_u16_scaled(*puVar3,puVar3[1],*(ushort *)((uint)bVar10 << 0x10 | 0xf61c),0xffff);
    muldiv_u16_scaled(*(ushort *)((uint)bVar10 << 0x10 | 0xf5f6),
                      *(ushort *)((uint)bVar10 << 0x10 | 0xf5f8),
                      -*(short *)((uint)bVar10 << 0x10 | 0xf61c) - 1,0xffff);
    uVar7 = (*select_min_xy_triple)();
    puVar4 = *(undefined2 **)((int)(*(short *)((uint)bVar10 << 0x10 | 0xf5e8) << 1) + 0x32c0);
    puVar4[1] = extraout_R1_00;
    *puVar4 = uVar7;
    uVar6 = 0;
    if ((*(short *)((uint)bVar10 << 0x10 | 0xf5fa) == 0 &&
         *(short *)((uint)bVar10 << 0x10 | 0xf5fc) == 0) ||
       (uVar6 = *(ushort *)((uint)bVar10 << 0x10 | 0xf5fe) |
                *(ushort *)((uint)bVar10 << 0x10 | 0xf600), uVar6 == 0)) {
      bVar11 = false;
    }
    else {
      bVar11 = true;
    }
    if (!bVar11) {
      return uVar6;
    }
    uVar7 = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf5fc);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef04) = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf5fa);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef06) = uVar7;
    uVar7 = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf600);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef08) = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf5fe);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef0a) = uVar7;
    uVar7 = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf604);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef0c) = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf602);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef0e) = uVar7;
    uVar7 = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf608);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef10) = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf606);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef12) = uVar7;
    uVar7 = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf60c);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef14) = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf60a);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef16) = uVar7;
    uVar7 = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf610);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef18) = *(undefined2 *)((uint)bVar10 << 0x10 | 0xf60e);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xef1a) = uVar7;
    if ((*(ushort *)((uint)bVar10 << 0x10 | 0xef1c) & 0x80) == 0) {
      sVar8 = *(short *)((uint)bVar10 << 0x10 | 0xef1e) + 1;
      *(short *)((uint)bVar10 << 0x10 | 0xef1e) = sVar8;
      if (sVar8 == 0) {
        psVar2 = (short *)((uint)bVar10 << 0x10 | 0xef1e);
        *psVar2 = *psVar2 + -1;
      }
      if (*(ushort *)((uint)bVar10 << 0x10 | 0x1024) <= *(ushort *)((uint)bVar10 << 0x10 | 0xef1e))
      {
        puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xef1c);
        *puVar1 = *puVar1 | 0x80;
        coldinit_f6c0_f7c8_state_clear_and_intc_flag_read(param_3);
      }
    }
    uStack_10 = *(ushort *)((uint)bVar10 << 0x10 | 0xef04);
    uStack_e = *(ushort *)((uint)bVar10 << 0x10 | 0xef06);
    uStack_14 = *(ushort *)((uint)bVar10 << 0x10 | 0xef04);
    uStack_12 = *(ushort *)((uint)bVar10 << 0x10 | 0xef06);
    sStack_4 = 4;
    cVar9 = '\x05';
    do {
      uVar6 = *(ushort *)((int)sStack_4 + -0x10fc);
      uVar5 = *(ushort *)((int)sStack_4 + -0x10fa);
      if ((uVar6 | uVar5) != 0) {
        if ((uVar6 < uStack_14) || ((uVar6 == uStack_14 && (uVar5 <= uStack_12)))) {
          if ((uVar6 <= uStack_10) && ((uVar6 != uStack_10 || (uVar5 < uStack_e)))) {
            uStack_10 = uVar6;
            uStack_e = uVar5;
          }
        }
        else {
          uStack_14 = uVar6;
          uStack_12 = uVar5;
        }
      }
      sStack_4 = sStack_4 + 4;
      bVar11 = cVar9 != -1;
      cVar9 = cVar9 + -1;
    } while (bVar11);
    sVar8 = (uStack_14 - uStack_10) - (ushort)(uStack_12 < uStack_e);
    uVar7 = (*identity_passthrough_u16)
                      (sVar8,uStack_12 - uStack_e,uVar6 | uVar5,sVar8,uStack_12 - uStack_e);
    *(undefined2 *)((uint)bVar10 << 0x10 | 0xf61a) = uVar7;
    if ((*(char *)((uint)bVar10 << 0x10 | 0x2bd) != '\0') &&
       (*(ushort *)((uint)bVar10 << 0x10 | 0xf784) < *(ushort *)((uint)bVar10 << 0x10 | 0xf61a))) {
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xf784) =
           *(undefined2 *)((uint)bVar10 << 0x10 | 0xf61a);
    }
    uStack_c = *(ushort *)((uint)bVar10 << 0x10 | 0xf61a);
    puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0x1026);
    if (*puVar1 <= uStack_c && uStack_c != *puVar1) {
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef04) = 0x8000;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef06) = 0;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef08) = 0x8000;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef0a) = 0;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef0c) = 0x8000;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef0e) = 0;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef10) = 0x8000;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef12) = 0;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef14) = 0x8000;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef16) = 0;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef18) = 0x8000;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef1a) = 0;
      puVar1 = (ushort *)((uint)bVar10 << 0x10 | 0xef1c);
      *puVar1 = *puVar1 & 0xff7f;
      *(undefined2 *)((uint)bVar10 << 0x10 | 0xef1e) = 0;
      uStack_c = coldinit_f6c0_f7c8_state_clear_and_intc_flag_read(unaff_R3);
    }
  }
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf5fa) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf5fc) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf5fe) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf600) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf602) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf604) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf606) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf608) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf60a) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf60c) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf60e) = 0;
  *(undefined2 *)((uint)bVar10 << 0x10 | 0xf610) = 0;
  return uStack_c;
}



void tcu_lockup_clutch_state_update(void)

{
  ushort *puVar1;
  undefined2 uVar2;
  ushort uVar3;
  short *psVar4;
  byte bVar5;
  
  bVar5 = 1;
  DAT_0001f3d4 = DAT_0001f3d4 << 1;
  if (((((g_tcu_gear_ratio_hist_status_flags & 0x80) != 0) && (DAT_0001f04e == 0)) &&
      (DAT_0001f642 == 0)) && ((ushort)(DAT_0001f638 - DAT_0001f636) < 0x8000)) {
    uVar3 = (*clamp_u8)();
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf65c);
    if ((*puVar1 <= uVar3 && uVar3 != *puVar1) &&
       ((ushort)(*(short *)((uint)bVar5 << 0x10 | 0xf634) - *(short *)((uint)bVar5 << 0x10 | 0xf636)
                ) < 0x8000)) {
      uVar3 = (*clamp_u8)();
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf65c);
      if (*puVar1 <= uVar3 && uVar3 != *puVar1) {
        puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf3d4);
        *puVar1 = *puVar1 | 1;
      }
    }
  }
  if (*(short *)((uint)bVar5 << 0x10 | 0xf39e) == 1) {
    psVar4 = (short *)&DAT_0000f3d6;
  }
  else {
    psVar4 = (short *)&DAT_0000f3d8;
  }
  if (((*(ushort *)((uint)bVar5 << 0x10 | 0xf3d4) & 1) == 0) ||
     ((*(ushort *)((uint)bVar5 << 0x10 | 0xf3d4) & 7) != 5)) {
    *psVar4 = *(short *)((uint)bVar5 << 0x10 | 0x1028);
  }
  else if (*psVar4 != 0) {
    *psVar4 = *psVar4 + -1;
  }
  if (*(short *)((uint)bVar5 << 0x10 | 0xef90) == 0) {
    uVar2 = *(undefined2 *)((uint)bVar5 << 0x10 | 0x1028);
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf3da) = uVar2;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf3d8) = uVar2;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf3d6) = uVar2;
    puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf340);
    *puVar1 = *puVar1 & 0xfffe;
    *(undefined2 *)((uint)bVar5 << 0x10 | 0xf3dc) = 0xffff;
  }
  else if ((*(ushort *)((uint)bVar5 << 0x10 | 0xf340) & 1) == 0) {
    uVar3 = 0xffff;
    if (*(short *)((uint)bVar5 << 0x10 | 0xf3d6) == 0) {
      uVar3 = ~*(ushort *)((uint)bVar5 << 0x10 | 0x32b0);
    }
    else if (*(short *)((uint)bVar5 << 0x10 | 0xf3d8) == 0) {
      uVar3 = ~*(ushort *)((uint)bVar5 << 0x10 | 0x32b2);
    }
    *(ushort *)((uint)bVar5 << 0x10 | 0xf3dc) = uVar3;
    if ((uVar3 & 0xf) == 0xf) {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf340);
      *puVar1 = *puVar1 & 0xfffe;
    }
    else {
      puVar1 = (ushort *)((uint)bVar5 << 0x10 | 0xf340);
      *puVar1 = *puVar1 | 1;
    }
  }
  return;
}


