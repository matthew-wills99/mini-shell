#ifndef SHELL_H
#define SHELL_H

extern char prompt_char;
extern int prompt_colour;

extern int last_status;

void shell_cleanup(void);

#endif