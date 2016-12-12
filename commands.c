// --------------------------------------------------------------
// Project : FabiGUI_GTK
// --------------------------------------------------------------
// File : commands.c
// Author : Benjamin Aigner
// Company : FH Technikum Wien
// Last update :
// Platform :
// --------------------------------------------------------------
// Description : <A brief explanation about the unit >
// --------------------------------------------------------------

#include "commands.h"

//char ButtonCommands [7][100] = {"AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n",""};
char ButtonCommands[15][6] = {"AT NC", "AT NE", "AT CL", "AT CR", "AT CM", "AT CD", "AT PL", "AT PR", "AT PM", "AT WU", "AT WD", "AT MX", "AT MY", "AT KW", "AT KP"};
                                                                                                                        //int   //int   //sting     //sting

struct atCommandType atCommands[]  = {
    {"ID"  , PARTYPE_NONE },  {"BM"  , PARTYPE_UINT }, {"CL"  , PARTYPE_NONE }, {"CR"  , PARTYPE_NONE },
    {"CM"  , PARTYPE_NONE },  {"CD"  , PARTYPE_NONE }, {"PL"  , PARTYPE_NONE }, {"PR"  , PARTYPE_NONE },
    {"PM"  , PARTYPE_NONE },  {"RL"  , PARTYPE_NONE }, {"RR"  , PARTYPE_NONE }, {"RM"  , PARTYPE_NONE },
    {"WU"  , PARTYPE_NONE },  {"WD"  , PARTYPE_NONE }, {"WS"  , PARTYPE_UINT }, {"MX"  , PARTYPE_INT  },
    {"MY"  , PARTYPE_INT  },  {"KW"  , PARTYPE_STRING},{"KP"  , PARTYPE_STRING},{"KR"  , PARTYPE_STRING},
    {"RA"  , PARTYPE_NONE },  {"SA"  , PARTYPE_STRING},{"LO"  , PARTYPE_STRING},{"LA"  , PARTYPE_NONE },
    {"LI"  , PARTYPE_NONE },  {"NE"  , PARTYPE_NONE }, {"DE"  , PARTYPE_NONE }, {"NC"  , PARTYPE_NONE },
};


uint8_t get_uint(char * str, int16_t * result)
{
    int num=0;
    if ((str==0)||(*str==0)) return (0);
    while (*str)
    {
      if ((*str >= '0') && (*str<='9'))
         num=num*10+(*str - '0');
      else return(0);
      str ++;
    }
    *result=num;
    return(1);
}

uint8_t get_int(char * str, int16_t * result)
{
    int16_t num,fact;
    if (str==0) return(0);
    if (*str =='-') {fact=-1; str++;} else fact=1;
    if (!get_uint(str,&num)) return(0);
    *result=num*fact;
    return(1);
}

void strup (char * str)   // convert to upper case letters
{
  if (!str) return;
  while (*str)
  {
    if ((*str>='a') && (*str<='z')) *str=*str-'a'+'A';
    str++;
  }
}


/*
void initButtons() {
     buttons[0].mode=CMD_NE;  // default function for first button: switch to next slot
     buttons[1].mode=CMD_KP; strcpy(buttons[1].keystring,"KEY_ESC ");;
     buttons[2].mode=CMD_NC;  // no command
     buttons[3].mode=CMD_KP; strcpy(buttons[3].keystring,"KEY_UP ");
     buttons[4].mode=CMD_KP; strcpy(buttons[4].keystring,"KEY_DOWN ");
     buttons[5].mode=CMD_KP; strcpy(buttons[5].keystring,"KEY_LEFT ");
     buttons[6].mode=CMD_KP; strcpy(buttons[6].keystring,"KEY_RIGHT ");
     buttons[7].mode=CMD_PL;   // press left mouse button
     buttons[8].mode=CMD_NC;   // no command
     buttons[9].mode=CMD_CD;   // click double
     buttons[10].mode=CMD_CR;  // click right
     buttons[11].mode=CMD_NC;  // no command
}
*/


void parseCommand (char * cmdstr)
{
    int8_t cmd=-1;
    int16_t num=0;

     // Serial.print("parseCommand:"); Serial.println(cmdstr);
    char * actpos = strtok(cmdstr," ");   // see a nice explaination of strtok here:  http://www.reddit.com/r/arduino/comments/2h9l1l/using_the_strtok_function/
    if (actpos)
    {
        int i;
        strup(actpos);

        for (i=0;(i<NUM_COMMANDS)&&(cmd==-1);i++)
        {
          if (!strcmp(actpos,atCommands[i].atCmd))  {
            // Serial.print ("partype="); Serial.println (pgm_read_byte_near(&(atCommands[i].partype)));
            switch (atCommands[i].partype)
            {
               case PARTYPE_UINT: actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=i ; break;
               case PARTYPE_INT:  actpos=strtok(NULL," ");  if (get_int(actpos, &num)) cmd=i ; break;
               case PARTYPE_STRING: actpos=strtok(NULL," "); cmd=i ; break;
               default: cmd=i; actpos=0; break;
            }
          }
        }
    }

    if (cmd>-1)  performCommand(cmd,num,actpos,0);
    else   printf("?\n");
}



uint8_t actButton=0;

void performCommand (uint8_t cmd, int16_t par1, char * keystring, int8_t periodicMouseMovement)
{

    if (actButton != 0)  // if last command was BM (set buttonmode): store current command for this button !!
    {
          printf("Got new mode for button %d:%d,%d,%s\n",actButton,cmd,par1,keystring);
          //printf("Button%d %s %d %s\n",actButton, atCommandsEnum[cmd],par1,keystring);
        //GUI_COM
          GUI_COM[activeSlot].Commands[actButton]=cmd;
          GUI_COM[activeSlot].par[actButton]=par1;
          if(keystring !=0)
                strncpy(GUI_COM[activeSlot].key[actButton], keystring, strlen(keystring));
          printf("%d:%d:%d:%s\n",actButton,GUI_COM[activeSlot].Commands[actButton],GUI_COM[activeSlot].par[actButton],GUI_COM[activeSlot].key[actButton]);


        //buttons[actButton-1].mode=cmd;
        //buttons[actButton-1].value=par1;
        //if (keystring==0) buttons[actButton-1].keystring[0]=0;
        //else strcpy(buttons[actButton-1].keystring,keystring);
        actButton=0;
        return;  // do not actually execute the command (just store it)
    }

    switch(cmd) {
        case CMD_ID:
               printf("%s\n",VERSION_STRING);
            break;
        case CMD_BM:
               printf("set mode for button %d\n",par1);
               if ((par1>0) && (par1<=NUMBER_OF_BUTTONS))
                   actButton=par1;
               else  printf("?\n");
            break;
        /*
        case CMD_CL:
                printf("Command found: CL");
                break;
        case CMD_CR:
                printf("Command found: CR");
                break;
        case CMD_CM:
                printf("Command found: CM");
                break;
        case CMD_CD:
                printf("Command found: CD");
                break;
        case CMD_PL:
                printf("Command found: PL");
                break;
        case CMD_PR:
                printf("Command found: PR");
                break;
        case CMD_PM:
                printf("Command found: PM");
                break;
        case CMD_RL:
                printf("Command found: RL");
                break;
        case CMD_RR:
                printf("Command found: RR");
                break;
        case CMD_RM:
                printf("Command found: RM");
                break;
        case CMD_WU:
                printf("Command found: WU");
                break;
        case CMD_WD:
                printf("Command found: WD");
                break;
        case CMD_WS:
                printf("Command found: WS");
                break;
        case CMD_MX:
                printf("Command found: MX");
                break;
        case CMD_MY:
                printf("Command found: MY");
                break;
        case CMD_KW:
                printf("Command found: KW");
                break;
        case CMD_KP:
                printf("Command found: KP");
                break;
        case CMD_KR:
                printf("Command found: KR");
                break;
        case CMD_RA:
                printf("Command found: RA");
                break;
        case CMD_SA:
                printf("Command found: SA");
                break;
        case CMD_LO:
                printf("Command found: LO");
                break;
        case CMD_LA:
                printf("Command found: LA");
                break;
        case CMD_LI:
                printf("Command found: LI");
                break;
        case CMD_NE:
                printf("Command found: NE");
                break;
        case CMD_DE:
                printf("Command found: DE");
                break;
        case CMD_NC:
                printf("Command found: NC");
                break;
        */

    }
}


void InitParsVar()
{
    int i;
    int j;
    int l;

    activeSlot=0;

    for (i=0; i<9; i++)
    {
        GUI_COM[i].Loadstate=0;

        for (j=0; j<30; j++)
        {
            GUI_COM[i].slotname[j]='\0';
        }

        for (j=0; j<30; j++)
        {
            GUI_COM[i].Commands[j]=0;
        }

        for (j=0; j<30; j++)
        {
            GUI_COM[i].par[j]=0;
        }
        for (j=0; j<30; j++)
        {
            for (l=0; l<15; l++)
            {
                GUI_COM[i].key[j][l]='\0';
            }
        }
    }
}










