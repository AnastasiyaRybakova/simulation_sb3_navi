/*
 * silbot3_tele_keyboard.cpp
 *
 *  Created on: 2015. 11. 9.
 *      Author: zikprid
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h> // for usleep() only
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/X.h>

#include <string.h>
#include <ros/ros.h>
#include <silbot3_msgs/Device_Wheel_Msg.h>
#include <silbot3_msgs/Device_Arm_Msg.h>
#include <silbot3_msgs/Device_ErobotPantilt_Msg.h>
#include <std_msgs/String.h>

using namespace std;

#define VERSION         "0.1"
#define DEFAULT_DELAY   10000
#define BIT(c, x)       ( c[x/8]&(1<<(x%8)) )
#define TRUE            1
#define FALSE           0

#define KEYSYM_STRLEN   64

#define SHIFT_DOWN      1
#define LOCK_DOWN       5
#define CONTROL_DOWN    3
#define ISO3_DOWN       4
#define MODE_DOWN       5

/* It is pretty standard */
#define SHIFT_INDEX     1  /*index for XKeycodeToKeySym(), for shifted keys*/
#define MODE_INDEX      2
#define MODESHIFT_INDEX 3
#define ISO3_INDEX      4
#define ISO3SHIFT_INDEX 4

/* Global variables */
extern Display *disp;
extern int PrintUp;

Display *disp;
int PrintUp = FALSE;

char *KeyCodeToStr(int code, int down, int mod);

int usage()
{
    printf("%s\n%s\n%s\n%s\n%s%s%s\n",
           "USAGE: xspy -display <display> -delay <usecs> -up",
           "       Options: display, specifies an X display.",
           "           delay, determines the polling frequency (.1 sec is 100000 usecs)",
           "           up, gives up transitions for some keys.",
           "       Version ", VERSION, ", modified by Eugene ");
    exit(0);
}

int KeyModifiers(char *keys);

//FILE *logfile;

void cleaner(int sig)
{
    fprintf(stderr, "\nGot Ctrl+C... closing...\n");
    fflush(stdout);
//    fclose(logfile);
    exit(0);
}

int pan = 0;
int tilt = 0;
int xtion = 0;
int xySpeed = 300;
int tSpeed = 40;

ros::Publisher wheelPublisher;
ros::Publisher armPublisher;
ros::Publisher pantiltPublisher;
ros::Publisher messagePublisher;



void moveWheelXYT(double x, double y, double t)
{
	silbot3_msgs::Device_Wheel_Msg msg;
	msg.command = "WHEEL_EMERGENCY_MOVE_BY_VELOCITY_XYT";
	msg.dParams.push_back(x);
	msg.dParams.push_back(y);
	msg.dParams.push_back(t);

	wheelPublisher.publish(msg);
}

void wheelStop()
{
	silbot3_msgs::Device_Wheel_Msg msg;
	msg.command = "WHEEL_STOP";

	wheelPublisher.publish(msg);
}

void movePantilt(double pan, double tilt, double xtion, int speedPantilt, int speedXtion)
{
	silbot3_msgs::Device_ErobotPantilt_Msg msg;
	msg.command = "PANTILT_MOVE_ABSOLUTE_POSITION_ALL";
	msg.angles.push_back(pan);
	msg.angles.push_back(tilt);
	msg.angles.push_back(xtion);
	msg.speeds.push_back(speedPantilt);
	msg.speeds.push_back(speedPantilt);
	msg.speeds.push_back(speedXtion);

	pantiltPublisher.publish(msg);
}

void moveArm(int angle0, int angle1, int angle2, int angle3, int angle4, int angle5, int speed)
{
	silbot3_msgs::Device_Arm_Msg msg;
	msg.command = "ARM_MOVE_TO_POSITION_ALL_BOTH";
	msg.angles.push_back(angle0);
	msg.angles.push_back(angle1);
	msg.angles.push_back(angle2);
	msg.angles.push_back(angle3);
	msg.angles.push_back(angle4);
	msg.angles.push_back(angle5);
	msg.speeds.push_back(speed);
	armPublisher.publish(msg);
}

void emergencyWheel()
{
	silbot3_msgs::Device_Wheel_Msg msg;
	msg.command = "WHEEL_EMERGENCY_STOP";
	wheelPublisher.publish(msg);
}

void emergencyPantilt()
{
	silbot3_msgs::Device_ErobotPantilt_Msg msg;
	msg.command = "PANTILT_EMERGENCY_STOP";
	pantiltPublisher.publish(msg);
}

void emergencyArm()
{
	silbot3_msgs::Device_Arm_Msg msg;
	msg.command = "ARM_EMERGENCY_STOP";
	armPublisher.publish(msg);
}

void releaseWheel()
{
	silbot3_msgs::Device_Wheel_Msg msg;
	msg.command = "WHEEL_EMERGENCY_RELEASE";
	wheelPublisher.publish(msg);
}

void releasePantilt()
{
	silbot3_msgs::Device_ErobotPantilt_Msg msg;
	msg.command = "PANTILT_EMERGENCY_RELEASE";
	pantiltPublisher.publish(msg);
}

void releaseArm()
{
	silbot3_msgs::Device_Arm_Msg msg;
	msg.command = "ARM_EMERGENCY_RELEASE";
	armPublisher.publish(msg);
}

void sendMessage(string message)
{
	std_msgs::String msg;
	msg.data = message;
	messagePublisher.publish(msg);
}


void Action(char* c)
{
//	printf("Action %s : ", c);
	string str = c;
	if (str.compare("i") == 0 ||str.compare("I") == 0)
	{
		moveWheelXYT(xySpeed, 0, 0);
	}
	else if (str.compare("j") == 0 ||str.compare("J") == 0)
	{
		moveWheelXYT(0, xySpeed, 0);
	}
	else if (str.compare("s") == 0 ||str.compare("S") == 0 || str.compare("k") == 0 ||str.compare("K") == 0)
	{
		wheelStop();
	}
	else if (str.compare("m") == 0 ||str.compare("M") == 0 || str.compare(",") == 0 ||str.compare("<") == 0)
	{
		moveWheelXYT(-xySpeed, 0, 0);
	}
	else if (str.compare("l") == 0 ||str.compare("L") == 0)
	{
		moveWheelXYT(0, -xySpeed, 0);
	}
	else if (str.compare("u") == 0 ||str.compare("U") == 0)
	{
		moveWheelXYT(0, 0, -tSpeed);
	}
	else if (str.compare("o") == 0 ||str.compare("O") == 0)
	{
		moveWheelXYT(0, 0, tSpeed);
	}
	else if (str.compare("r") == 0 ||str.compare("R") == 0)
	{
		tilt += 5;
		if(tilt >= 40)
		{
			tilt = 40;
		}
		movePantilt(pan, tilt, xtion, 35, 0);
	}
	else if (str.compare("d") == 0 ||str.compare("D") == 0)
	{
		pan -= 5;
		if (pan <= -65)
		{
			pan = -65;
		}
		movePantilt(pan, tilt, xtion, 35, 0);
	}
	else if (str.compare("f") == 0 ||str.compare("F") == 0)
	{
		tilt -= 5;
		if (tilt <= -10)
		{
			tilt = -10;
		}
		movePantilt(pan, tilt, xtion, 35, 0);
	}
	else if (str.compare("g") == 0 ||str.compare("G") == 0)
	{
		pan += 5;
		if (pan >= 65)
		{
			pan = 65;
		}
		movePantilt(pan, tilt, xtion, 35, 0);
	}
	else if (str.compare("y") == 0 ||str.compare("YJ") == 0)
	{
		xtion += 2;
		if (xtion >= 44)
		{
			xtion = 44;
		}
		movePantilt(pan, tilt, xtion, 0, 35);
	}
	else if (str.compare("h") == 0 ||str.compare("H") == 0)
	{
		xtion -= 2;
		if (xtion <= -10)
		{
			xtion = -10;
		}
		movePantilt(pan, tilt, xtion, 0, 35);
	}
	else if (str.compare("q") == 0 ||str.compare("Q") == 0)
	{
		releaseArm();
		releasePantilt();
		releaseWheel();
	}
	else if (str.compare("w") == 0 ||str.compare("W") == 0)
	{
		emergencyArm();
		emergencyPantilt();
		emergencyWheel();
	}
	else if (str.compare("p") == 0 ||str.compare("P") == 0)
	{
		emergencyWheel();
	}
	else if (str.compare("b") == 0 ||str.compare("B") == 0)
	{
		moveArm(90, 0, 0, 90, 0, 0, 50);
	}
	else if (str.compare("v") == 0 ||str.compare("V") == 0)
	{
		moveArm(90, 0, 0, 0, 0, 0, 50);
	}
	else if (str.compare("c") == 0 ||str.compare("C") == 0)
	{
		moveArm(0, 0, 0, 0, 0, 0, 50);
	}
	else if (str.compare("z") == 0 ||str.compare("Z") == 0)
	{
		sendMessage("S3H::stop");
	}
	else if (str.compare("x") == 0 ||str.compare("X") == 0)
	{
		sendMessage("S3H::exit");
	}
	else if (str.compare("{") == 0 ||str.compare("[") == 0)
	{
		xySpeed -= 50;
		tSpeed -= 4;
		if (xySpeed <= 50)
		{
			xySpeed = 50;
		}
		if (tSpeed <= 10)
		{
			tSpeed = 10;
		}
	}
	else if (str.compare("]") == 0 ||str.compare("}") == 0)
	{
		xySpeed += 50;
		tSpeed += 4;
		if (xySpeed >= 1000)
		{
			xySpeed = 1000;
		}
		if (tSpeed >= 90)
		{
			tSpeed = 90;
		}
	}
	else
	{
		char buff[6];
		sprintf(buff, "S3H::%s",c);
		string nm = buff;
//		printf("%s",nm.c_str());
		sendMessage(nm);
	}
}


int main(int argc, char *argv[])
{

	ros::init(argc, argv, "Silbot3_tele_keyboard");

	ros::NodeHandle node;

    wheelPublisher = node.advertise<silbot3_msgs::Device_Wheel_Msg>("/DeviceNode/Wheel/commands", 1000);
    armPublisher = node.advertise<silbot3_msgs::Device_Arm_Msg>("/DeviceNode/Arm/commands", 1000);
    pantiltPublisher = node.advertise<silbot3_msgs::Device_ErobotPantilt_Msg>("/DeviceNode/Pantilt/commands", 1000);
    messagePublisher = node.advertise<std_msgs::String>("/ment_by_id", 1000);

    char    *hostname = ":0",
            *char_ptr,
            buf1[32],   buf2[32],
            *keys,
            *saved;
    int i,  delay=DEFAULT_DELAY;

    signal(SIGINT, cleaner);
    signal(SIGTERM, cleaner);

    /* get args */
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-help")) {
            usage();
        }
        else if (!strcmp(argv[i], "-display")) {
            i++;
            hostname = argv[i];
        }
        else if (!strcmp(argv[i], "-delay")) {
            i++;
            delay = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-newlines")) {
            i++;
            delay = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-up")) {
            PrintUp = TRUE;
        }
        else usage();
    }

    /* setup Xwindows */
    disp = XOpenDisplay(hostname);
    if (disp == NULL) {
        fprintf(stderr, "Cannot open X display: %s\n", hostname);
        exit(1);
    }
    XSynchronize(disp, TRUE);

    /* setup buffers */
    saved = buf1;
    keys = buf2;
    XQueryKeymap(disp, saved);

    time_t now, before = time(NULL);
    struct tm *ts;
    struct timeval tv;
    char *strtime;
    char unixtime[80];
    long milliseconds;

    now = time(NULL);
    before = now;
    ts = localtime(&now);
    strtime = (char*)malloc(1000 * sizeof(char));

    sprintf(strtime, "%d.%d_%d:%d:%d.log",
            ts->tm_mon + 1, ts->tm_mday,
            ts->tm_hour + 1, ts->tm_min + 1, ts->tm_sec + 1);
    fprintf(stderr, "opened %s for logging\n", strtime);
//    logfile = fopen(strtime, "w+");

    while (1) {
        now = time(NULL);
        ts = localtime(&now);
        /* find changed keys */
        XQueryKeymap(disp, keys);
        for (i = 0; i < 32*8; i++) {
            if (BIT(keys, i) != BIT(saved, i)) {
                register char *str;
                str = (char*) KeyCodeToStr(i, BIT(keys, i), KeyModifiers(keys));
                if ((BIT(keys, i) != 0 || PrintUp) && (str[0] != '\0')) {
                    strftime(unixtime, sizeof(unixtime), "%s", ts);
                    gettimeofday(&tv, NULL);
                    milliseconds = tv.tv_usec/1000;

                    printf("%s.%.3ld:%s\n", unixtime + 7, milliseconds, str);
//                    fprintf(logfile,
//                           "%s.%.3ld:%s\n", unixtime + 7, milliseconds, str);

                    Action(str);
                }
                if (difftime(now, before) > 3600) {
//                    fclose(logfile);
                    before = now;

                    sprintf(strtime, "%d.%d_%d:%d:%d.log",
                            ts->tm_mon + 1, ts->tm_mday,
                            ts->tm_hour + 1, ts->tm_min + 1, ts->tm_sec + 1);
                    fprintf(stderr, "opened %s for logging\n", strtime);
//                    logfile = fopen(strtime, "w+");
                }
//                fflush(logfile);
                fflush(stdout); /* in case user is writing to a pipe */
            }
        }

        /* swap buffers */
        char_ptr = saved;
        saved = keys;
        keys = char_ptr;

        usleep(delay);
    }
}

/* This part takes the keycode and makes an output string. */

/*
   Have a keycode, Look up keysym for it.
   Convert keysym into its string representation.
   if string is more than one character try to reduce it to one.
   if string still is more than one character, put it into the form
   (+string) or (-string) depending on whether the key is up or down.
   print out the string.
*/

struct conv {char from[20], to[5];} conv_table[] = {
    /* shift & control replaced with nothing, since they are appearent
      from the output */
    {"return",""},       {"escape","^["},       {"delete", "^H"},
    {"shift",""},        {"control",""},        {"tab","\t"},
    {"space", " "},      {"exclam", "!"},       {"quotedbl", "\""},
    {"numbersign", "#"}, {"dollar", "$"},       {"percent", "%"},
    {"ampersand", "&"},  {"apostrophe", "'"},   {"parenleft", "("},
    {"parenright", ")"}, {"asterisk", "*"},     {"plus", "+"},
    {"comma", ","},      {"minus", "-"},        {"period", "."},
    {"slash", "/"},      {"colon", ":"},        {"semicolon", ";"},
    {"less", "<"},       {"equal", "="},        {"greater", ">"},
    {"question", "?"},   {"at", "@"},           {"bracketleft", "["},
    {"backslash", "\\"}, {"bracketright", "]"}, {"asciicircum", "^"},
    {"underscore", "_"}, {"grave", "`"},        {"braceleft", "{"},
    {"bar", "|"},        {"braceright", "}"},   {"asciitilde", "~"},
    {"",""}
};

int StrToChar(char *data)
{
    int i = 0;
    while (conv_table[i++].from[0] != 0 || conv_table[i++].to[0] != 0) {
        if (!strncasecmp(data, conv_table[i].from,
                         strlen(conv_table[i].from))) {
            strcpy(data, conv_table[i].to);
            return TRUE;
        }
    }
    return FALSE;
}

char *KeyCodeToStr(int code, int down, int mod)
{
    static char *str, buf[KEYSYM_STRLEN + 1];
    int index;
    KeySym  keysym;
    /* get the keysym for the appropriate case */
    switch (mod) {
	    case SHIFT_DOWN:
		    index = SHIFT_INDEX;
		    break;
	    case ISO3_DOWN:
		    index = ISO3_INDEX;
		    break;
	    case MODE_DOWN:
		    index = MODE_INDEX;
		    break;
	    default:
		    index = 0;
    }

    keysym = XKeycodeToKeysym(disp, code, index);
    if (NoSymbol == keysym) return "";

    /* convert keysym to a string, copy it to a local area */
    str=XKeysymToString(keysym);

    if (strcmp(str,"ISO_Level3_Shift") == 0) {
	    keysym = XKeycodeToKeysym(disp, code, ISO3_INDEX);
	    str = XKeysymToString(keysym);
    }

    if (str == NULL) return "";
    strncpy(buf, str, KEYSYM_STRLEN); buf[KEYSYM_STRLEN] = 0;

    /* try to reduce strings to character equivalents */
    if (buf[1] != 0 && !StrToChar(buf)) {
    if (strcmp(buf, "Caps_Lock") == 0) return "";
        /* still a string, so put it in form (+str) or (-str) */
        if (down) strcpy(buf, "(+");
        else      strcpy(buf, "(-");
        strcat(buf, str);
        strcat(buf, ")");
        return buf;
    }
    if (buf[0] == 0) {
        return "";
    }
    if (mod == CONTROL_DOWN) {
        buf[2] = 0;
        buf[1] = toupper(buf[0]);
        buf[0] = '^';
    }
    if (mod == LOCK_DOWN) {
        buf[0] = toupper(buf[0]);
    }
    return buf;
}


/* returns which modifier is down, shift/caps or control */
int KeyModifiers(char *keys)
{
    static int setup = TRUE;
    static int width;
    static XModifierKeymap *mmap;
    int i;

    if (setup) {
        setup = FALSE;
        mmap = XGetModifierMapping(disp);
        width = mmap->max_keypermod;
    }
    for (i = 0; i < width; i++) {
        KeyCode code;

        code = mmap->modifiermap[ControlMapIndex*width+i];
        if (code && 0 != BIT(keys, code)) {return CONTROL_DOWN;}

        code = mmap->modifiermap[ShiftMapIndex*width  +i];
        if (code && 0 != BIT(keys, code)) {return SHIFT_DOWN;}

        code = mmap->modifiermap[LockMapIndex*width   +i];
        if (code && 0 != BIT(keys, code)) {return LOCK_DOWN;}

        code = mmap->modifiermap[Mod3MapIndex*width   +i];
        if (code && 0 != BIT(keys, code)) {return ISO3_DOWN;}

        code = mmap->modifiermap[Mod5MapIndex*width   +i];
        if (code && 0 != BIT(keys, code)) {return MODE_DOWN;}
    }
    return 0;
}

/*if usleep is missing
include <sys/types.h>
include <sys/time.h>
void usleep(x) {
   struct timeval  time;
   time.tv_sec= x/1000000;
   time.tv_usec=x%1000000;
   select(0, NULL, NULL, NULL, &time);
}
*/
