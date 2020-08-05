#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/select.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

const char *shiftnames[] = {
	"S-reserved",
	"S-esc",
	"!",
	"@",
	"#",
	"$",
	"%",
	"^",
	"&",
	"*",
	"(",
	")",
	"_",
	"+",
	"S-backspace",
	"S-tab",
	"Q",
	"W",
	"E",
	"R",
	"T",
	"Y",
	"U",
	"I",
	"O",
	"P",
	"{",
	"}",
	"S-enter",
	"S-leftctrl",
	"A",
	"S",
	"D",
	"F",
	"G",
	"H",
	"J",
	"K",
	"L",
	":",
	"\"",
	"~",
	"S-leftshift",
	"|",
	"Z",
	"X",
	"C",
	"V",
	"B",
	"N",
	"M",
	"<",
	">",
	"?",
	"S-rightshift",
	"S-kpasterisk",
	"S-leftalt",
	"S-space",
	"S-capslock",
	"S-f1",
	"S-f2",
	"S-f3",
	"S-f4",
	"S-f5",
	"S-f6",
	"S-f7",
	"S-f8",
	"S-f9",
	"S-f10",
	"S-numlock",
	"S-scrolllock",
	"S-kp7",
	"S-kp8",
	"S-kp9",
	"S-kpminus",
	"S-kp4",
	"S-kp5",
	"S-kp6",
	"S-kpplus",
	"S-kp1",
	"S-kp2",
	"S-kp3",
	"S-kp0",
	"S-kpdot",
	"S-UNDEFINED",
	"S-zenkakuhankaku",
	"S-102nd",
	"S-f11",
	"S-f12",
	"S-ro",
	"S-katakana",
	"S-hiragana",
	"S-henkan",
	"S-katakanahiragana",
	"S-muhenkan",
	"S-kpjpcomma",
	"S-kpenter",
	"S-rightctrl",
	"S-kpslash",
	"S-sysrq",
	"S-rightalt",
	"S-linefeed",
	"S-home",
	"S-up",
	"S-pageup",
	"S-left",
	"S-right",
	"S-end",
	"S-down",
	"S-pagedown",
	"S-insert",
	"S-delete",
	"S-macro",
	"S-mute",
	"S-volumedown",
	"S-volumeup",
	"S-power",
	"S-kpequal",
	"S-kpplusminus",
	"S-pause",
	"S-scale",
	"S-kpcomma",
	"S-hangeul",
	"S-hanja",
	"S-yen",
	"S-leftmeta",
	"S-rightmeta",
	"S-compose",
	"S-stop",
	"S-again",
	"S-props",
	"S-undo",
	"S-front",
	"S-copy",
	"S-open",
	"S-paste",
	"S-find",
	"S-cut",
	"S-help",
	"S-menu",
	"S-calc",
	"S-setup",
	"S-sleep",
	"S-wakeup",
	"S-file",
	"S-sendfile",
	"S-deletefile",
	"S-xfer",
	"S-prog1",
	"S-prog2",
	"S-www",
	"S-msdos",
	"S-coffee",
	"S-rotate_display",
	"S-cyclewindows",
	"S-mail",
	"S-bookmarks",
	"S-computer",
	"S-back",
	"S-forward",
	"S-closecd",
	"S-ejectcd",
	"S-ejectclosecd",
	"S-nextsong",
	"S-playpause",
	"S-previoussong",
	"S-stopcd",
	"S-record",
	"S-rewind",
	"S-phone",
	"S-iso",
	"S-config",
	"S-homepage",
	"S-refresh",
	"S-exit",
	"S-move",
	"S-edit",
	"S-scrollup",
	"S-scrolldown",
	"S-kpleftparen",
	"S-kprightparen",
	"S-new",
	"S-redo",
	"S-f13",
	"S-f14",
	"S-f15",
	"S-f16",
	"S-f17",
	"S-f18",
	"S-f19",
	"S-f20",
	"S-f21",
	"S-f22",
	"S-f23",
	"S-f24",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-playcd",
	"S-pausecd",
	"S-prog3",
	"S-prog4",
	"S-dashboard",
	"S-suspend",
	"S-close",
	"S-play",
	"S-fastforward",
	"S-bassboost",
	"S-print",
	"S-hp",
	"S-camera",
	"S-sound",
	"S-question",
	"S-email",
	"S-chat",
	"S-search",
	"S-connect",
	"S-finance",
	"S-sport",
	"S-shop",
	"S-alterase",
	"S-cancel",
	"S-brightnessdown",
	"S-brightnessup",
	"S-media",
	"S-switchvideomode",
	"S-kbdillumtoggle",
	"S-kbdillumdown",
	"S-kbdillumup",
	"S-send",
	"S-reply",
	"S-forwardmail",
	"S-save",
	"S-documents",
	"S-battery",
	"S-bluetooth",
	"S-wlan",
	"S-uwb",
	"S-unknown",
	"S-video_next",
	"S-video_prev",
	"S-brightness_cycle",
	"S-brightness_auto",
	"S-display_off",
	"S-wwan",
	"S-rfkill",
	"S-micmute",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-UNDEFINED",
	"S-UNDEFINED"
};

const char *keynames[] = {
	"reserved",
	"esc",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"0",
	"-",
	"=",
	"backspace",
	"tab",
	"q",
	"w",
	"e",
	"r",
	"t",
	"y",
	"u",
	"i",
	"o",
	"p",
	"[",
	"]",
	"enter",
	"leftctrl",
	"a",
	"s",
	"d",
	"f",
	"g",
	"h",
	"j",
	"k",
	"l",
	";",
	"'",
	"`",
	"leftshift",
	"\\",
	"z",
	"x",
	"c",
	"v",
	"b",
	"n",
	"m",
	",",
	".",
	"/",
	"rightshift",
	"kpasterisk",
	"leftalt",
	"space",
	"capslock",
	"f1",
	"f2",
	"f3",
	"f4",
	"f5",
	"f6",
	"f7",
	"f8",
	"f9",
	"f10",
	"numlock",
	"scrolllock",
	"kp7",
	"kp8",
	"kp9",
	"kpminus",
	"kp4",
	"kp5",
	"kp6",
	"kpplus",
	"kp1",
	"kp2",
	"kp3",
	"kp0",
	"kpdot",
	"UNDEFINED",
	"zenkakuhankaku",
	"102nd",
	"f11",
	"f12",
	"ro",
	"katakana",
	"hiragana",
	"henkan",
	"katakanahiragana",
	"muhenkan",
	"kpjpcomma",
	"kpenter",
	"rightctrl",
	"kpslash",
	"sysrq",
	"rightalt",
	"linefeed",
	"home",
	"up",
	"pageup",
	"left",
	"right",
	"end",
	"down",
	"pagedown",
	"insert",
	"delete",
	"macro",
	"mute",
	"volumedown",
	"volumeup",
	"power",
	"kpequal",
	"kpplusminus",
	"pause",
	"scale",
	"kpcomma",
	"hangeul",
	"hanja",
	"yen",
	"leftmeta",
	"rightmeta",
	"compose",
	"stop",
	"again",
	"props",
	"undo",
	"front",
	"copy",
	"open",
	"paste",
	"find",
	"cut",
	"help",
	"menu",
	"calc",
	"setup",
	"sleep",
	"wakeup",
	"file",
	"sendfile",
	"deletefile",
	"xfer",
	"prog1",
	"prog2",
	"www",
	"msdos",
	"coffee",
	"rotate_display",
	"cyclewindows",
	"mail",
	"bookmarks",
	"computer",
	"back",
	"forward",
	"closecd",
	"ejectcd",
	"ejectclosecd",
	"nextsong",
	"playpause",
	"previoussong",
	"stopcd",
	"record",
	"rewind",
	"phone",
	"iso",
	"config",
	"homepage",
	"refresh",
	"exit",
	"move",
	"edit",
	"scrollup",
	"scrolldown",
	"kpleftparen",
	"kprightparen",
	"new",
	"redo",
	"f13",
	"f14",
	"f15",
	"f16",
	"f17",
	"f18",
	"f19",
	"f20",
	"f21",
	"f22",
	"f23",
	"f24",
	"UNDEFINED",
	"UNDEFINED",
	"UNDEFINED",
	"UNDEFINED",
	"UNDEFINED",
	"playcd",
	"pausecd",
	"prog3",
	"prog4",
	"dashboard",
	"suspend",
	"close",
	"play",
	"fastforward",
	"bassboost",
	"print",
	"hp",
	"camera",
	"sound",
	"question",
	"email",
	"chat",
	"search",
	"connect",
	"finance",
	"sport",
	"shop",
	"alterase",
	"cancel",
	"brightnessdown",
	"brightnessup",
	"media",
	"switchvideomode",
	"kbdillumtoggle",
	"kbdillumdown",
	"kbdillumup",
	"send",
	"reply",
	"forwardmail",
	"save",
	"documents",
	"battery",
	"bluetooth",
	"wlan",
	"uwb",
	"unknown",
	"video_next",
	"video_prev",
	"brightness_cycle",
	"brightness_auto",
	"display_off",
	"wwan",
	"rfkill",
	"micmute",
	"UNDEFINED",
	"UNDEFINED",
	"UNDEFINED",
	"UNDEFINED",
	"UNDEFINED",
	"UNDEFINED",
	"UNDEFINED"
};

int control = 0;
int shift = 0;
int meta = 0;
int alt = 0;

struct keystat {
	const char *keyseq;
	int n;
	struct keystat *next;
} *stats = NULL;

struct libevdev *keyboards[256];
size_t nkeyboards;

const char *toseq(uint16_t code)
{
	static char seq[256];
	int i = 0;

	if(alt) {
		seq[i++] = 'A';
		seq[i++] = '-';
	}
	if(meta) {
		seq[i++] = 'M';
		seq[i++] = '-';
	}
	if(control) {
		seq[i++] = 'C';
		seq[i++] = '-';
	}

	if(code < sizeof keynames / sizeof keynames[0])
		strcpy(seq + i, shift ? shiftnames[code] : keynames[code]);
	else
		strcpy(seq + i, "UNDEFINED");

	return seq;
}

void dump(int _)
{
	struct keystat *ent = stats;

	while(ent) {
		printf("%s:%d\n", ent->keyseq, ent->n);
		ent = ent->next;
	}

	exit(0);
}

int main()
{
	DIR *dh;
	struct dirent *ent;
	if(!(dh = opendir("/dev/input"))) {
		perror("opnedir");
		exit(-1);
	}

	while((ent = readdir(dh))) {
		int fd;
		struct libevdev *dev;
		char path[PATH_MAX];

		if(ent->d_type != DT_CHR)
			continue;

		sprintf(path, "/dev/input/%s", ent->d_name);

		if((fd = open(path, O_RDONLY|O_NONBLOCK)) < 0) {
			perror("open");
			exit(-1);
		}

		if(libevdev_new_from_fd(fd, &dev) < 0) {
			fprintf(stderr, "Failed to open device: %s\n", path);
			continue;
		}

		keyboards[nkeyboards++] = dev;
	}

	signal(SIGINT, dump);
	while(1) {
		fd_set fds;
		int maxfd = 0;

		FD_ZERO(&fds);
		for (int i = 0; i < nkeyboards; i++) {
			int fd = libevdev_get_fd(keyboards[i]);
			maxfd = maxfd > fd ? maxfd : fd;
			FD_SET(fd, &fds);
		}

		select(maxfd + 1, &fds, NULL, NULL, NULL);
		for (int i = 0; i < nkeyboards; i++) {
			int fd = libevdev_get_fd(keyboards[i]);
			if(FD_ISSET(fd, &fds)) {
				struct input_event ev;

				while(libevdev_next_event(keyboards[i], LIBEVDEV_READ_FLAG_NORMAL, &ev) == LIBEVDEV_READ_STATUS_SUCCESS) {
					if(ev.type == EV_KEY) {
						switch (ev.code) {
						case KEY_LEFTCTRL:
						case KEY_RIGHTCTRL:
							control = ev.value;
							break;
						case KEY_LEFTALT:
						case KEY_RIGHTALT:
							alt = ev.value;
							break;
						case KEY_LEFTMETA:
						case KEY_RIGHTMETA:
							meta = ev.value;
							break;
						case KEY_LEFTSHIFT:
						case KEY_RIGHTSHIFT:
							shift = ev.value;
							break;
						default:
							if(ev.value == 1) {
								const char *seq = toseq(ev.code);
								struct keystat *ent = stats;
								int found = 0;

								while(ent) {
									if(!strcmp(ent->keyseq, seq)) {
										found = 1;
										ent->n++;
										break;
									}

									ent = ent->next;
								}

								if(!found) {
									ent = malloc(sizeof(struct keystat));
									ent->next = stats;
									ent->keyseq = strdup(seq);
									ent->n = 1;

									stats = ent;
								}
							}
							break;
						}
					}
				}
			}
		}
	}
}
