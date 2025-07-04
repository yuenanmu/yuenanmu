#ifndef _ALL_INIT_H____
#define _ALL_INIT_H____
#define LED1                    (H2 )
#define LED2                    (B13)

#define KEY1                    (E2 )
#define KEY2                    (E3 )
#define KEY3                    (E4 )
#define KEY4                    (E5 )

#define SWITCH1                 (D3 )
#define SWITCH2                 (D4 )

extern int32 encoder1;
extern int32 encoder2;
extern int8 offset;

extern uint32 key1_count;
extern uint32 key2_count;
extern uint32 key3_count;
extern uint32 key4_count;
extern uint8  key1_flag;
extern uint8  key2_flag;
extern uint8  key3_flag;
extern uint8  key4_flag;
extern uint32 count_time;
void all_init();
void Key_Scan(void);
#endif 