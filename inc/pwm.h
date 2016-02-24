#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

void init_pwm(void);
unsigned short get_pwm(unsigned char num);
void set_pwm(unsigned char num, unsigned short value);

#endif /* PWM_H_INCLUDED */
